pro attenuation,nviews,nprojs,sixel_size,new=new

common comat,  atnfile,atten,mu,old_mu
common comco,  contour,ncpts,head_axis

if(keyword_set(new)) then begin
   atten=fltarr(nprojs,nviews)
   atten(*,*)=1.
end

theta =findgen(nviews)*!pi/nviews			; Angle from 0 to pi
yint=fltarr(ncpts)					; max intersection number
;t = systime(1)
for i=0,nviews-1 do begin
  rotated_contour=contour				; copy contour
  rotate_contour, contour,ncpts,-theta(i),rotated_contour
  close_contour,rotated_contour,ncpts,closed_contour
  xv = closed_contour(0,*)
  yv = closed_contour(1,*)
  xmin=min(xv)
  xmax=max(xv)
  for j=0,nprojs-1 do begin
    x = sixel_size * (j-nprojs/2)
    if (x ge xmin and x le xmax) then begin
	nint=0
	for k=0,ncpts-1 do begin
	  s_min = xv(k)
	  s_max = xv(k+1)
          if (s_min gt s_max) then begin
	    s_min = s_max
	    s_max = xv(k)
	  end
	  if (x ge s_min and x lt s_max) then begin
	     sl=(yv(k+1) -yv(k))/(xv(k+1)-xv(k))
	     yint(nint) = yv(k)+sl*(x-xv(k))
             nint=nint+1
	  end
	end
	if (nint mod 2) then print,' Odd number of intersections at cell ',i,j
	y_len=0.
	if(nint eq 2) then y_len=abs(yint(1)-yint(0))
	if(nint gt 2) then begin
	  ysort = yint(sort(yint))
	  for k=0,nint-1,2 do y_len = y_len + ysort(k+1)-ysort(k)
	end 
	atten(j,i) = atten(j,i) * exp(y_len*mu)
    end
  end
  ;print,' View ',i,' Time ',systime(1)-t
end

end

pro rotate_contour, contour, ncpts, theta, rotated_contour

  if(theta eq 0.) then  return
  sint = sin(theta)
  cost = cos(theta)
  for i=0,ncpts-1 do begin
    rotated_contour(0,i) = cost*contour(0,i) + sint*contour(1,i)
    rotated_contour(1,i) = cost*contour(1,i) - sint*contour(0,i)
  end
end

pro close_contour, contour, ncpts, closed_contour

  closed_contour=fltarr(3,ncpts+1)
  for i=0,ncpts-1 do begin
    closed_contour(0,i) = contour(0,i)
    closed_contour(1,i) = contour(1,i)
    closed_contour(2,i) = contour(2,i)
  end
  closed_contour(0,ncpts) = contour(0,0)
  closed_contour(1,ncpts) = contour(1,0)
  closed_contour(2,ncpts) = contour(2,0)
end
