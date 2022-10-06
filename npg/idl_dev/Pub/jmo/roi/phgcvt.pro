;*********************
function phgcvt,filnam
;*********************

; Returns 128x192x16 scatter array.

w = dblarr(!NANGMC,!NRAYMC,!NRINGMC,!NRINGMC,!NSCATMC,!NSCATMC)
out = fltarr(160,192,16)
get_lun,lu
openr,lu,filnam
readu,lu,w
close,lu

cmd = string("ls -l ",filnam," | gawk '{print $4}'")
spawn,cmd,length
len = fix(length)
test = fix(8*!NPHGTOT)
if(len(0) eq test(0)) then begin
;    Must be data file from alpha, (8 byte floats)
    x = osf2sun(reform(w,!NPHGTOT))
    x = reform(x,!NANGMC,!NRAYMC,!NRINGMC,!NRINGMC,!NSCATMC,!NSCATMC)
endif else begin
    x = w
endelse

for i=0,!NSCATMC-1 do begin
    for j=0,!NSCATMC-1 do begin
	y = fltarr(!NANGMC,!NRAYMC)
	for k=0,!NRINGMC-2 do begin
	    for m=0,!NRINGMC-2 do begin
		if((k ne 0) or (m ne 0)) then $
		    y = y + x(*,*,i,j,k,m)
	    endfor
	endfor
        y = reverse(y,2) ; Flip columns.
	z = y(44:47,*)
	yy = reverse(y,1) ; Flip rows.
	y(4:47,*) = yy(0:43,*) ; Rotate by 15 degrees.
        y(0:3,*) = z
	z = rebin(transpose(y),2*!NRAYCTI,!NANGCTI)
	i1 = (2*!NRAYCTI - 160)/2
	i2 = i1 + 160 - 1
	out(*,*,j*!NSCATMC+i) = z(i1:i2,*)
;	out(*,*,j*!NSCATMC+i) = rebin(transpose(y),!NRAYCTI,!NANGCTI)
    endfor
endfor

return,out

end
