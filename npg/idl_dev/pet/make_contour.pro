;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;
;	**********   MAKE_CONTOUR.PRO   **********
;
;	Read sinogram stack, search contour and display them	
;
;	Original Version     	July 17,1992	: C. Michel UCL
;
;	Modifications
;		   		August  7,1992 	: roi drawing only if image is there
;		   		August 10,1992	: Calculate attenuation
;		  	 	August 11,1992	: Modify UI & add reconstruction
;
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
pro make_filter

common 	comsc, 	scnfile,scans,nf,np,ng,nb,mats,specs,index,maxindex,f,p,g,b, $
		scan1,nprojs,nviews,plane_sep,sixel_size,scan2,nviews2, $
		old_border_thr, modscan2  
common  comfi,	fft_size,filter,cutoff,order,filfun

distfun = FINDGEN(fft_size)				; ramp	
distfun = distfun < (fft_size- distfun)			; symmetrize around fft_size/2
distfun(0) = 1e-4					; avoid division by 0
x = distfun/(2.*nprojs*cutoff)				; 2.*nprojs*cutoff (=.5) is the max frequency
case filter of
1: filfun = x*(x le 1)					; ideal
2: filfun = x / (1. + x^(2 * order))			; butter
3: filfun = x*(x le 1.)*(0.5+0.5*cos(!pi*x))		; hann
4: filfun = x*(x le 1.)*(0.54+0.46*cos(!pi*x))		; hamm
5: filfun = x*((x le .5)*(1.-6.*x*x*(1-x)) + (x gt .5 and x le 1.)*(2.*(1.-x)^3)) ; parzen
endcase
filfun=filfun*cutoff					; constant slope 
end
;-----------------------------------------------------------------------------------------------------
pro bkproj

common 	comsc, 	scnfile,scans,nf,np,ng,nb,mats,specs,index,maxindex,f,p,g,b, $
		scan1,nprojs,nviews,plane_sep,sixel_size,scan2,nviews2, $
		old_border_thr, modscan2 
common  comfi,	fft_size,filter,cutoff,order,filfun
common  comim,	imgfile,images,image1,img_size,slice_width,zoom,xoff,yoff,rotoff, $
		image2,image_there
common  combo,	angle,border,dborder,border_there
common comcol, 	cred,cwhite
common 	comwi,	xmcbase, xmcdraw,xscbase,xscdraw,ximdraw,label1,label2,ldraw1,ldraw2,ldraw3, $
		win_num1,win_num2,win_num3,history,text2,xdraw1,xdraw2,xdraw3, $
		draw1,draw2,draw3,message,matlist,matlabel, $
		control_previous, control_next, control_showhat,control_attenu, $
		ordersl,orderlabel,cutoffsl,cutofflabel,zoomsl,zoomlabel,lf

	image1(*,*)=0.		; reset images
	image2(*,*)=0.
	WSET, win_num3
	erase
	zoomf = zoom/100.
	pixel_size = sixel_size(index)*float(nprojs)/(float(img_size)*zoomf)
	zf = float(nprojs) / float(img_size)/ zoomf
	WIDGET_CONTROL, history, SET_VALUE = " Pixel size "+strcompress(string(pixel_size))+" cm",/append
	while (fft_size lt 2*nprojs) do fft_size = fft_size * 2
	fft_offset = (fft_size - nprojs) / 2
	proj = fltarr(fft_size)
	;
	make_filter		; make 1-D filter
	ftypes = ["Ramp","Butterworth", "Hann", "Hamming","Parzen"]
	info = ' Filter function :'+ ftypes(filter-1) 
	if (filter eq 2) then info = info + ' order ' + strcompress(string(order))
	info = info + ' with cutoff at '+ strcompress(string(cutoff))
	WIDGET_CONTROL, history, SET_VALUE = info,/append
	info =  " Translation = (" + strcompress(STRING(xoff))+ ',' + strcompress(string(yoff)) + $
		") Rotation = " + strcompress(STRING(rotoff*180./!pi))
  	WIDGET_CONTROL, history, SET_VALUE = info,/append
;
;	backproject filtered projection
;
	rx0 = img_size/2. - xoff/pixel_size
	ry0 = img_size/2. + yoff/pixel_size
	WIDGET_CONTROL, message, SET_VALUE = "Backprojection starting ... be patient"
	dt = !pi/float(nviews2)
	t = systime(1)
	for j=0,nviews2-1 do begin
	   for i=0,nprojs-1 do proj(i+fft_offset) = scan2(i,j)
	   ftproj = FFT(proj, -1)
	   proj = float(FFT(ftproj * filfun, 1))
	   theta = j*dt+ rotoff
	   rdx = cos(theta)*zf
	   rdy = sin(theta)*zf
	   r0 = fft_size/2 - rx0*rdx - ry0*rdy
	   info='Now backprojecting view '+ strcompress(string(j)) + ' (from '+ strcompress(string(nviews2-1))+')'
	   WIDGET_CONTROL, message, SET_VALUE = info
	   bkproj_view, proj, r0, rdx, rdy
	end
	scalef = (!pi /nviews2) * nprojs		; 1st part from bkp, 2d due to fft
	image1 = image1 * scalef
	info = ' Image maximum is ' + strcompress(string(max(image1)))
  	WIDGET_CONTROL, history, SET_VALUE = info,/append
	t_recon = systime(1)-t
	image_there = 1
	info = 	' Reconstruction took ' + strcompress(string(t_recon)) + ' seconds '
	WIDGET_CONTROL, history, SET_VALUE = info,/append
;
;	Fill header
;
	;images.xdim = img_size
	;images.ydim = img_size
	;images.sh.slice_width = slice_width
	;images.sh.pixel_size = pixel_size
	;images.sh.x_origin = xoff
	;images.sh.y_origin = yoff
	;images.sh.image_rotation = rotoff
;
;	Display image
;
	image2 = rebin(image1,256,256)>0
	imax= max(image2)
	if (border_there) then begin				; overlay roi
	  xsh = fix(xoff / pixel_size) 				; in pixels
	  ysh = fix(yoff / pixel_size)				; in pixels
	  zoom = sixel_size(index)/pixel_size			; when header field is not filled
	  xcen = (img_size/2)					; image center
	  ycen = (img_size/2)-1					;  ... Check this carefully with a perfect circle
	  for i=0,2*nviews2-1 do begin
	    st=sin(angle(i)+rotoff)					; contains angle from 0-pi
	    ct=cos(angle(i)+rotoff)
	    ix = fix(-(border(i+nviews2)*ct - dborder(i+nviews2)*st)*zoom)+ xcen - xsh
	    iy = fix( (border(i+nviews2)*st + dborder(i+nviews2)*ct)*zoom)+ ycen - ysh
	    ix = ix >0 <(img_size-1)					; clipping
	    iy = iy >0 <(img_size-1)
	    if(!order eq 1) then iy = img_size-1 -iy
	    image2(2*ix,2*iy)= imax/256.  				; accounts for 256 image2 size 
 	  end
	end
	TVSCL, image2
	msg = ' Displaying image ' + strcompress(specs(index))
        WIDGET_CONTROL, history, set_value = msg,/append
	WIDGET_CONTROL, message, SET_VALUE = "Ready"
end 

;-----------------------------------------------------------------------------------------------------
pro bkproj_view, proj, r0, rdx, rdy

common  comim,	imgfile,images,image1,img_size,slice_width,zoom,xoff,yoff,rotoff, $
		image2,image_there

  for y=0,img_size-1 do begin
    r = r0
    for x=0,img_size-1 do begin
      j = fix(r) & w = r-j
      image1(x,y) = image1(x,y) + (1.0-w)*proj(j) + w*proj(j+1)
      r  = r + rdx
    end
  r0 = r0 + rdy
  end 
end
;-----------------------------------------------------------------------------------------------------
pro attenuation,nviews,nprojs,sixel_size,new=new

common comat,  atnfile,atten,mu,old_mu
common comco,  contour,ncpts,head_axis
common 	comwi,	xmcbase, xmcdraw,xscbase,xscdraw,ximdraw,label1,label2,ldraw1,ldraw2,ldraw3, $
		win_num1,win_num2,win_num3,history,text2,xdraw1,xdraw2,xdraw3, $
		draw1,draw2,draw3,message,matlist,matlabel, $
		control_previous, control_next, control_showhat,control_attenu, $
		ordersl,orderlabel,cutoffsl,cutofflabel,zoomsl,zoomlabel,lf

if(keyword_set(new)) then begin
   atten=fltarr(nprojs,nviews)
   atten(*,*)=1.
end

theta =findgen(nviews)*!pi/nviews			; Angle from 0 to pi
yint=fltarr(ncpts)					; max intersection number
;t = systime(1)
for i=0,nviews-1 do begin
   info='Now forward projecting view '+ strcompress(string(i)) + ' (from '+ strcompress(string(nviews-1))+')'
   WIDGET_CONTROL, message, SET_VALUE = info
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
;-----------------------------------------------------------------------------------------------------
pro rotate_contour, contour, ncpts, theta, rotated_contour

  if(theta eq 0.) then  return
  sint = sin(theta)
  cost = cos(theta)
  for i=0,ncpts-1 do begin
    rotated_contour(0,i) = cost*contour(0,i) + sint*contour(1,i)
    rotated_contour(1,i) = cost*contour(1,i) - sint*contour(0,i)
  end
end
;-----------------------------------------------------------------------------------------------------
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
;-----------------------------------------------------------------------------------------------------
pro centroid,x,a,f,pder

	f = a(0)*cos(x+a(1))
	if n_params(0) le 2 then return	    ;need partial?
	pder = fltarr(n_elements(x),2)	    ;yes, make array.
	pder(0,0) = cos(x+a(1))
	pder(0,1) = -a(0)*sin(x+a(1))
	return
end
;-----------------------------------------------------------------------------------------------------
function centroid_fit,x,y,a

	cm=check_math(0.,1.)		; Don't print math error messages.
	n = n_elements(y)		;# of points.
	return,curfit(x,y,replicate(1.,n),a,sigmaa,funct='centroid',/quiet) 
end
;-----------------------------------------------------------------------------------------------------
pro read_norm_911

common  comno,	normfile,norms,norm,norm_there
common 	comwi,	xmcbase, xmcdraw,xscbase,xscdraw,ximdraw,label1,label2,ldraw1,ldraw2,ldraw3, $
		win_num1,win_num2,win_num3,history,text2,xdraw1,xdraw2,xdraw3, $
		draw1,draw2,draw3,message,matlist,matlabel, $
		control_previous, control_next, control_showhat,control_attenu, $
		ordersl,orderlabel,cutoffsl,cutofflabel,zoomsl,zoomlabel, lf

	normfile = '/topo6b/mich/data/cti/911/normalsta.nrm'
	stat=recat(normfile,1,1,1,0,0,norms)
	norm=norms.data(*,*,0)
	WIDGET_CONTROL, history, SET_VALUE = " 911 Normalization file read ",/append
	norm_there = 1
end
;-----------------------------------------------------------------------------------------------------
pro read_scan, new=new

common 	comsc, 	scnfile,scans,nf,np,ng,nb,mats,specs,index,maxindex,f,p,g,b, $
		scan1,nprojs,nviews,plane_sep,sixel_size,scan2,nviews2, $
		old_border_thr, modscan2  
common  comim,	imgfile,images,image1,img_size,slice_width,zoom,xoff,yoff,rotoff, $
		image2,image_there
common  comno,	normfile,norms,norm,norm_there
common 	comwi,	xmcbase, xmcdraw,xscbase,xscdraw,ximdraw,label1,label2,ldraw1,ldraw2,ldraw3, $
		win_num1,win_num2,win_num3,history,text2,xdraw1,xdraw2,xdraw3, $
		draw1,draw2,draw3,message,matlist,matlabel, $
		control_previous, control_next, control_showhat,control_attenu, $
		ordersl,orderlabel,cutoffsl,cutofflabel,zoomsl,zoomlabel, lf
common comcol, 	cred,cwhite
common colors,	r_orig,g_orig,b_orig,r_curr,g_curr,b_curr

	ON_ERROR, 2
	data_dir = '/topo6b/mich/data/cti'
	cd, data_dir, current=curdir
	scnfile = pickfile(GROUP=group,TITLE='Select CTI scan',FILTER=['scn'])
	cd, curdir
	if (scnfile eq '') then return
	openr,unit,scnfile,/get_lun
	stat=rmh(unit,mh)
	if(stat eq !ECAT.ERROR) then print, !ECAT.ERROR
	nf = mh.file.nframes
	np = mh.file.nplanes
	ng = mh.file.ngates
	nb = mh.file.nbeds
	f = 0 & p = 0 & g = 0 & b = 0
	if (nf gt 0) then f='*'
	if (np gt 0) then p='*'
	if (ng gt 0) then g='*'
	if (nb gt 0) then b='*'
	expand,mh,f,p,g,b
	f=fix(f) & p=fix(p) & g=fix(g) & b=fix(b)
	mf = n_elements(f)
	mp = n_elements(p)
	mg = n_elements(g)
	mb = n_elements(b)
	ntot  = mf*mp*mg*mb
	maxindex = ntot-1
	mats=lonarr(5,ntot)
	specs=strarr(ntot)
	stat = rdir(unit,dir)
	free_lun,unit
	for i =0, ntot-1 do begin
		mats(*,i) = matval(dir(0,i))
		f= mats(0,i) & p= mats(1,i) & g= mats(2,i) & d= mats(3,i) & b= mats(4,i)
		specs(i) = strcompress('frame '+ string(f) +' plane '+string(p)+' gate '+string(g)+ $
			' data '+string(d)+' bed '+string(b))
	end
	;print,mats
	f= mats(0,0) & p =mats(1,0) & g= mats(2,0) & d= mats(3,0) & b= mats(4,0)
	if (nf gt 0) then f = '*'
	if (np gt 0) then p = '*'
	WIDGET_CONTROL, message, SET_VALUE = " Reading scan file, please be patient ..."
	stat=recat(scnfile,f,p,g,d,b,scans)
	if(stat eq !ECAT.ERROR) then print, !ECAT.ERROR
	nprojs	= scans.xdim
	nviews	= scans.ydim
	plane_sep = scans.mh.scan.plane_separation
	sixel_size = scans.sh.sample_distance

	info =  ' => New scan loaded ' + lf  $
 		+ ' Scan              : ' + scnfile + lf $
		+ ' Dimensions        : ' + string(nprojs) +'x' + string(nviews) + lf $
		+ ' Plane separation  : ' + string(plane_sep) + lf
        WIDGET_CONTROL, history, set_value = info,/append
	IF(NOT KEYWORD_SET(NEW)) THEN BEGIN	; destroy previous boards
		WIDGET_CONTROL, xscbase, destroy=1
	END
	xscbase = WIDGET_BASE(xmcdraw,/column)
	xscdraw = WIDGET_BASE(xscbase,/row)
	xdraw1 = WIDGET_BASE(xscdraw,/column)
	xdraw2 = WIDGET_BASE(xscdraw,/column)
	ldraw1 = WIDGET_LABEL(xdraw1, VALUE = "Original scan")
	draw1 = WIDGET_DRAW(xdraw1, $
		/BUTTON_EVENTS, $	;generate events when buttons pressed
		/FRAME, $
		UVALUE = 'DRAW_WIN_EVENT1', $
		RETAIN = 2, $		;make sure draw is redrawn when covered
		XSIZE = nprojs, $
		YSIZE = nviews)

	nviews2=nviews & modscan2=0
	ldraw2 = WIDGET_LABEL(xdraw2, VALUE = " Byte Scaled ")
	draw2 = WIDGET_DRAW(xdraw2, $
		/BUTTON_EVENTS, $	;generate events when buttons pressed
		/FRAME, $
		UVALUE = 'DRAW_WIN_EVENT2', $
		RETAIN = 2, $		;make sure draw is redrawn when covered
		XSIZE = nprojs, $
		YSIZE = nviews2)
  	matlabel = WIDGET_LABEL(xscbase, VALUE = "Select matrix or use << and >> buttons")
  	matlist = WIDGET_LIST(xscbase,VALUE = specs, UVALUE = 'SETMATRIX', YSIZE = 5)
;
end
;-----------------------------------------------------------------------------------------------------
pro display_scan

common 	comsc, 	scnfile,scans,nf,np,ng,nb,mats,specs,index,maxindex,f,p,g,b, $
		scan1,nprojs,nviews,plane_sep,sixel_size,scan2,nviews2, $
		old_border_thr, modscan2  
common 	comwi,	xmcbase, xmcdraw,xscbase,xscdraw,ximdraw,label1,label2,ldraw1,ldraw2,ldraw3, $
		win_num1,win_num2,win_num3,history,text2,xdraw1,xdraw2,xdraw3, $
		draw1,draw2,draw3,message,matlist,matlabel, $
		control_previous, control_next, control_showhat,control_attenu, $
		ordersl,orderlabel,cutoffsl,cutofflabel,zoomsl,zoomlabel, lf
common comcol, 	cred,cwhite
common colors,	r_orig,g_orig,b_orig,r_curr,g_curr,b_curr
;
	WIDGET_CONTROL, draw1, GET_VALUE = win_num1
	WIDGET_CONTROL, draw2, GET_VALUE = win_num2
	WSET, win_num1
	scan1=scans.data(*,*,index)>0
        TVSCL, scan1
	IF(nviews2 lt nviews) THEN BEGIN	; destroy previous boards
	  nviews2 = nviews
	  WIDGET_CONTROL, xdraw2, destroy=1
	  xdraw2 = WIDGET_BASE(xscdraw,/column)
	  ldraw2 = WIDGET_LABEL(xdraw2, VALUE = " Byte Scaled ")
	  draw2 = WIDGET_DRAW(xdraw2, /BUTTON_EVENTS,/FRAME, UVALUE = 'DRAW_WIN_EVENT2', $
				 RETAIN = 2, XSIZE = nprojs, YSIZE = nviews2)
	END
	scan2 = bytscl(scan1)
	WSET, win_num2
	erase
        TV, scan2
	msg = ' Displaying scan ...' + specs(index) + lf $
		+ ' Sixel size ' + string(sixel_size(index)) 
        WIDGET_CONTROL, history, set_value = msg,/append
	WIDGET_CONTROL, message, SET_VALUE = " Ready"
end
;-----------------------------------------------------------------------------------------------------
pro xmc_event, event

common  comco, 	contour,ncpts,head_axis
common 	comsc, 	scnfile,scans,nf,np,ng,nb,mats,specs,index,maxindex,f,p,g,b, $
		scan1,nprojs,nviews,plane_sep,sixel_size,scan2,nviews2, $
		old_border_thr, modscan2  
common  comim,	imgfile,images,image1,img_size,slice_width,zoom,xoff,yoff,rotoff, $
		image2,image_there
common  comfi,	fft_size,filter,cutoff,order,filfun
common  combo,	angle,border,dborder,border_there
common  comno,	normfile,norms,norm,norm_there
common  comat,  atnfile,atten,mu,old_mu
common 	comwi,	xmcbase, xmcdraw,xscbase,xscdraw,ximdraw,label1,label2,ldraw1,ldraw2,ldraw3, $
		win_num1,win_num2,win_num3,history,text2,xdraw1,xdraw2,xdraw3, $
		draw1,draw2,draw3,message,matlist,matlabel, $
		control_previous, control_next, control_showhat,control_attenu, $
		ordersl,orderlabel,cutoffsl,cutofflabel,zoomsl,zoomlabel, lf
common comcol, 	cred,cwhite
common colors,	r_orig,g_orig,b_orig,r_curr,g_curr,b_curr

ON_ERROR, 2

WIDGET_CONTROL, event.id, GET_UVALUE = eventval

;HELP, /STRUCT, event 
	
CASE eventval OF

   'DRAW_WIN_EVENT1': $
	BEGIN
		WSET, win_num1
		IF event.press EQ 1 THEN BEGIN
		  i= event.x & j = event.y
		  if !order eq 1 then k = nviews-1 - j else k = j
		  l = k/(nviews/nviews2)
		  if( l le 0 or l gt nviews2-1)  then begin
			print, 'nviews= ',nviews,' nviews2=',nviews2,' i=',i,' j=',j,' k=',k,' l=',l
			return
		  endif
		  p1 = string(i) + string(k)
		  p2 = string(i) + string(l)
		  if (i ge 1 and i le nprojs-1 and k ge 1 and k le nviews-1) then begin
		     v1 = string(scan1(i,k)) & v2 = string(long(scan2(i,l)))
		  endif else begin
		     v1 = 'undefined' & v2 = 'undefined'
		  endelse
		  info1 =  ' Left  ('+ strcompress(p1) +') ='+ v1
		  info2 =  ' Right ('+ strcompress(p2) +') ='+ v2
		  info = ' Sixel value ...'+ lf + info1 + lf + info2 
                  WIDGET_CONTROL, text2, set_value = info
		ENDIF
		IF event.press EQ 0 THEN BEGIN
               	  WIDGET_CONTROL, text2, set_value = ''
		ENDIF
	END
   
  'DRAW_WIN_EVENT2': $
	BEGIN
		WSET, win_num2
		IF event.press EQ 1 THEN BEGIN
		  i= event.x & j = event.y
		  if !order eq 1 then k = nviews2-1 - j else k = j
		  l = k*(nviews/nviews2)
		  if( l lt 0 or l gt nviews-1)  then begin
			print, 'nviews= ',nviews,' nviews2=',nviews2,' i=',i,' j=',j,' k=',k,' l=',l
			return
		  endif
		  p1= string(i) + string(l)
		  p2= string(i) + string(k)
		  if (i ge 1 and i le nprojs-1 and k ge 1 and k le nviews2-1) then begin
		     v1 = string(scan1(i,l)) & v2 = string(long(scan2(i,k)))
		  endif else begin
		     v1 = 'undefined' & v2 = 'undefined'
		  endelse
		  info1 =  ' Left  ('+ strcompress(p1) +') ='+ v1
		  info2 =  ' Right ('+ strcompress(p2) +') ='+ v2
		  info = ' Sixel value ...'+ lf + info1 + lf + info2 
                  WIDGET_CONTROL, text2, set_value = info
		ENDIF
		IF event.press EQ 0 THEN BEGIN
        	  WIDGET_CONTROL, text2, set_value = ''
		ENDIF
	END

  'DRAW_WIN_EVENT3': $
	BEGIN
		WSET, win_num3
		IF event.press EQ 1 THEN BEGIN
		  i= event.x & j = event.y
		  if !order eq 1 then k = 255 - j else k = j
		  if (i ge 1 and i le 255 and k ge 1 and k le 255) then begin
		     v1 = string(image2(i,k))
		  endif else begin
		     v1 = 'undefined'
		  endelse
		  p1= string(i) + string(k)
		  info =  ' Image value  ('+ strcompress(p1) +') ='+ v1
                  WIDGET_CONTROL, text2, set_value = info
		ENDIF
		IF event.press EQ 0 THEN BEGIN
        	  WIDGET_CONTROL, text2, set_value = ''		; erase
		ENDIF
	END

 'PROJECTION': BEGIN
		maxv = max(scan1) 			;Get extrema
		minv = max([1.,maxv/1000.])
		window,/free ,xs=300, ys=300,retain=2,title='Total projection' 
		new_w = !d.window
		wset,new_w
		ptot=fltarr(nprojs)
		for i=0,nprojs-1 do for j=0,nviews-1 do ptot(i) = scan1(i,j)
		plot_io,findgen(nprojs),ptot,yrange=[minv,maxv],xstyle=1,psym=10		             
		xyouts,.03,.97,specs(index),/normal
		xyouts,.03,.03,scnfile,/normal
           END

   'NORM911': BEGIN
		if (norm_there eq 0) then read_norm_911
		WSET, win_num1
		scan1= scan1*norm	             
                TVSCL, scan1
		scan2 = bytscl(scan1)
		modscan2 = 0
		IF(nviews2 lt nviews) THEN BEGIN	; destroy previous rebinned board
	  	  nviews2 = nviews
	 	  WIDGET_CONTROL, xdraw2, destroy=1
	 	  xdraw2 = WIDGET_BASE(xscdraw,/column)
	  	  ldraw2 = WIDGET_LABEL(xdraw2, VALUE = " Byte Scaled ")
	  	  draw2 = WIDGET_DRAW(xdraw2,/BUTTON_EVENTS,/FRAME, $
	  		UVALUE = 'DRAW_WIN_EVENT2',RETAIN = 2, XSIZE = nprojs, YSIZE = nviews2)
		END
		WSET, win_num2
		erase
       		TV, scan2
		info = ' Normalizing 911 scan'            
         	WIDGET_CONTROL, history, set_value = info,/append
         	WIDGET_CONTROL, ldraw1, set_value = 'Normalized scan'
		WIDGET_CONTROL, message, SET_VALUE = " Ready"
          END
 

 'CONTOUR': BEGIN
		if (modscan2 eq 1) then begin
		  scan2 = bytscl(scan1)
                  nviews2=nviews
	          modscan2 = 0
        	  WIDGET_CONTROL, history, set_value = $
			 ' Scan2 was modified, restoring original first....',/append
	 	  WIDGET_CONTROL, xdraw2, destroy=1
	 	  xdraw2 = WIDGET_BASE(xscdraw,/column)
	  	  ldraw2 = WIDGET_LABEL(xdraw2, VALUE = " Byte Scaled ")
	  	  draw2 = WIDGET_DRAW(xdraw2,/BUTTON_EVENTS,/FRAME, $
	  		UVALUE = 'DRAW_WIN_EVENT2',RETAIN = 2, XSIZE = nprojs, YSIZE = nviews2)
		  WSET, win_num2
        	  TV, scan2
		  return
		endif
		border_thr=old_border_thr
		xvaredit,border_thr,name='Border Threshold in %'
		ncpts =  2*nviews2
           	info = ' Border threshold set at '+ strcompress(string(border_thr)) +' %' + lf + $
		       ' It contains '+ strcompress(string(ncpts)) + ' points'
         	WIDGET_CONTROL, history, set_value = info,/append
		old_border_thr=border_thr          
		border_thr = border_thr/100.
		thr =  border_thr * max(scan2)
		scan2=(scan2>thr)-thr					; threshold             
  		wset,win_num2
                TV, scan2
;
		border=fltarr(4*nviews2)				; border extension avoids discontinuities
		angle =findgen(4*nviews2)*!pi/nviews2			; angle from 0 to 4pi
		proj =fltarr(nprojs)					; projection value
		objc=fltarr(nviews2)					; object center
		proc=nprojs/2						; projection center
;
;		Border detection using threshold technique
;
		for i=0,nviews2-1 do begin
		  for j=0,nprojs-1 do proj(j) = scan2(j,i)		; projection
		  proj = smooth(proj,3)					; smoothed
		  pmax = max(proj,jmax)					; not robust if isolated hot cluster
		  j=jmax						; since max may be outside the object....
		  while (proj(j) gt 0.1) and (j gt 0) do j=j-1		; border left search 
		  bl = j+1 						; save it
		  j = bl						; 
		  while (proj(j) gt 0.1) and (j lt nprojs-1) do j=j+1	; border right search 
		  br = j-1						; save it
		  border(i)      = br-proc				; right border extension
		  border(i+nviews2)   = proc-bl 			; left border
		  border(i+2*nviews2) = br-proc	 			; right border
 		  border(i+3*nviews2) = proc-bl 				; left border extension
		end
;
;		Mark sinogram with smoothed borders and centroid
;
		border = smooth(float(border),7)			; smooth border 
		for i=0,nviews2-1 do begin
  		  jl = fix(proc-border(i+nviews2))
		  jr = fix(nprojs - (proc-border(i+2*nviews2)))
		  objc(i) = (border(i+2*nviews2) - border(i+nviews2))/2.		; center = (br-bl)/2
		  jc=proc+fix(objc(i))
		  scan2(jr,i)=cred					; mark smooth border right
		  scan2(jl,i)=cred					; mark smooth border left
 		  scan2(jc,i)=cwhite					; mark center
		  modscan2 = 1						; scan2 is modified
		end
;
;		Find centroid
;
		ang =findgen(nviews2)*!pi/nviews2			; Angle from 0 to pi
		pars = [(max(objc)-min(objc))/2., 1.]			; initial amplitude and phase
		fit = centroid_fit(ang,objc,pars)
		head_axis = fltarr(3)
		head_axis = [ pars(0)*cos(pars(1))*sixel_size(index) , $
			      pars(0)*sin(pars(1))*sixel_size(index) , $
			      (float(maxindex)/2.-(float(index)))*plane_sep ]
		strx = ' x = ' + strcompress(string(head_axis(0)))
   		stry = ' y = ' + strcompress(string(head_axis(1)))
		strz = ' z = ' + strcompress(string(head_axis(2)))
		xoff = head_axis(0)					; set initial reconstruction values
		yoff = head_axis(1)					; rotoff still missing ...
		msg = ' Centroid coordinates '+ strx + stry + strz
	       	WIDGET_CONTROL, history, set_value = msg,/append
;
;		Contour from border
;
		dborder= smooth(deriv(angle,border),9)			; smooth also derivative
		contour = fltarr(3,2*nviews2)
		for i=0,2*nviews2-1 do begin
		   st=sin(angle(i))					; contains angle from 0-pi
		   ct=cos(angle(i))
		   contour(0,i) = -(border(i+nviews2)*ct - dborder(i+nviews2)*st)*sixel_size(index)
		   contour(1,i) =  (border(i+nviews2)*st + dborder(i+nviews2)*ct)*sixel_size(index)
		   contour(2,i) =  (float(maxindex)/2.-(float(index)))*plane_sep 
 		end
;
;		Contour length
;
		dx = contour(0,2*nviews2-1) - contour(0,0)			; between first and last point
		dy = contour(1,2*nviews2-1) - contour(1,0)
		length = sqrt(dx^2+dy^2)
		for i=0,2*nviews2-2 do begin
		  dx = contour(0,i+1) - contour(0,i)
		  dy = contour(1,i+1) - contour(1,i)
		  length = length + sqrt(dx^2+dy^2)
		end		
		msg = ' Contour length = '+ strcompress(string(length))+' cm'
	       	WIDGET_CONTROL, history, set_value = msg,/append
;
;		display border
;
		window,/free ,xs=300, ys=300,retain=2,title='Sinogram Border'
		new_w = !d.window
		wset,new_w
		plot,angle,border,yrange=[-20,nprojs/2],xstyle=1,psym=10,col=cred
		oplot,angle,dborder,xstyle=1,psym=10,line=2,col=cred
		xyouts,.03,.97,specs(index),/normal,col=cred
		xyouts,.03,.03,scnfile,/normal,col=cred
         	WIDGET_CONTROL, ldraw2, set_value = 'Detected border'
  		wset,win_num2
                TV, scan2
		widget_control, control_showhat,sensitive=1
		widget_control, control_attenu,sensitive=1
		border_there = 1
;
		WIDGET_CONTROL, message, SET_VALUE = " Ready"
         END

  'SHOWHAT':  showhat,GROUP=group,size=500

 'ATTENUATION': BEGIN
		mu=old_mu
		xvaredit,mu,name='Attenuation coefficient in cm-1 '
		old_mu= mu
          	info = ' Calculating attenuation with mu = '+strcompress(string(mu))+' cm-1'
         	WIDGET_CONTROL, history, set_value = info,/append
	        atnfile = scnfile
	        strput,atnfile,'atn',strpos(atnfile,'scn') 	; atn extension 
		WIDGET_CONTROL, message, SET_VALUE = " Calculating attenuation, please be patient ..."
		t = systime(1)
		attenuation,nviews2,nprojs,sixel_size(index),/new
		t_atten = systime(1)-t
		scan2 = bytscl(rebin(scan1,nprojs,nviews2)*atten)
		scan1 = scan1 * rebin(atten,nprojs,nviews)
		modscan2 = 0
		WSET, win_num2
        	TV, scan2
         	WIDGET_CONTROL, ldraw2, set_value = 'Byte scaled'

		WSET, win_num1
                TVSCL, scan1
		info = 	' It took ' + strcompress(string(t_atten)) + ' seconds ' + lf + $
			' ... File' + atnfile + ' (not yet) created'		; not yet ....
         	WIDGET_CONTROL, history, set_value = info,/append
         	WIDGET_CONTROL, ldraw1, set_value = 'Attenuation corrected scan'
		WIDGET_CONTROL, message, SET_VALUE = " Ready"
	        END

'RECON': BEGIN            
            WIDGET_CONTROL, message, set_value = ' Starting reconstruction, be patient ...'
	    scan2 = rebin(scan1,nprojs,nviews2)
	    bkproj
          END

'RAMP': begin
     	WIDGET_CONTROL, ORDERSL, SENSITIVE = 0
     	WIDGET_CONTROL, ORDERlabel, SENSITIVE = 0
	filter = 1
	end

'BUTTER':begin
     	WIDGET_CONTROL, ORDERSL, SENSITIVE = 1
     	WIDGET_CONTROL, ORDERlabel, SENSITIVE = 1
 	filter = 2
	end

'HANN': begin
      	WIDGET_CONTROL, ORDERSL, SENSITIVE = 0
     	WIDGET_CONTROL, ORDERlabel, SENSITIVE = 0
	filter = 3
	end

'HAMM':begin
     	WIDGET_CONTROL, ORDERSL, SENSITIVE = 0
      	WIDGET_CONTROL, ORDERlabel, SENSITIVE = 0
	filter = 4
	end

'PARZEN': begin
     	WIDGET_CONTROL, ORDERSL, SENSITIVE = 0
     	WIDGET_CONTROL, ORDERlabel, SENSITIVE = 0
	filter = 5
	end

'ORDER': begin
	  widget_control, ordersl, get_value = temp
	  if(temp ne order) then begin
	    order = temp
	  endif
  	WIDGET_CONTROL, orderlabel, SET_VALUE = "Order " + strcompress(STRING(order))
  	end

'CUTOFF':BEGIN
	widget_control, cutoffsl, get_value = temp
 	cutoff = temp/20. * 0.5
  	WIDGET_CONTROL, cutofflabel, SET_VALUE = "Cutoff at " + strcompress(STRING(cutoff))
	END
 
'TRANSLATION':BEGIN
	transl= [xoff,yoff]
	xvaredit,transl,name='Translation'
	xoff=transl(0) & yoff=transl(1)
	END

'ROTATION':BEGIN
	rotdeg=rotoff*180./!pi
	xvaredit,rotdeg,name='Rotation'
	rotoff= rotdeg*!pi/180.
	END

'ZOOM': begin
	  widget_control, zoomsl, get_value = temp
	  if(temp ne zoom) then begin
	    zoom = temp
	  endif
        zoomf = zoom/100.
  	WIDGET_CONTROL, zoomlabel, SET_VALUE = "Zoom " + strcompress(STRING(zoomf))
	pixel_size = sixel_size(index)*float(nprojs)/(float(img_size)*zoomf)
	WIDGET_CONTROL, message, SET_VALUE = "Pixel size "+strcompress(string(pixel_size))+" cm"
  	end

    'REBIN': BEGIN
		if (modscan2 eq 1) then begin
		  scan2 = bytscl(scan1)
                  nviews2=nviews
	          modscan2 = 0
        	  WIDGET_CONTROL, history, set_value = $
			 ' Scan2 was modified, restoring original first....',/append
	 	  WIDGET_CONTROL, xdraw2, destroy=1
	 	  xdraw2 = WIDGET_BASE(xscdraw,/column)
	  	  ldraw2 = WIDGET_LABEL(xdraw2, VALUE = " Byte Scaled ")
	  	  draw2 = WIDGET_DRAW(xdraw2,/BUTTON_EVENTS,/FRAME, $
	  		UVALUE = 'DRAW_WIN_EVENT2',RETAIN = 2, XSIZE = nprojs, YSIZE = nviews2)
		  WSET, win_num2
        	  TVSCL, scan2
		  return
		endif
		nviews2=nviews2/2
		scan2=rebin(scan2,nprojs,nviews2)
             	info = ' Angle rebinned by a factor ' + strcompress(string(nviews/nviews2))           
         	WIDGET_CONTROL, history, set_value = info,/append
		WIDGET_CONTROL, xdraw2, destroy=1
		xdraw2 = WIDGET_BASE(xscdraw,/column)
		txt = 'Rebinned by ' + strcompress(string(nviews/nviews2))
		ldraw2 = WIDGET_LABEL(xdraw2, VALUE = txt)
		draw2 = WIDGET_DRAW(xdraw2, $
			/BUTTON_EVENTS, $	;generate events when buttons pressed
			/FRAME, $
			UVALUE = 'DRAW_WIN_EVENT2', $
			RETAIN = 2, $		;make sure draw is redrawn when covered
			XSIZE = nprojs, $
			YSIZE = nviews2)
                TV, scan2
             END
 

   'CLUT': BEGIN
		xloadct, GROUP = event.top
		r_curr(0)= 255 	& g_curr(0)= 255 	& b_curr(0)=255	; white
		r_curr(1)= 255	& g_curr(1)=   0 	& b_curr(1)=  0	; red
		tvlct,r_curr, g_curr, b_curr				; Load clut
		r_orig = r_curr & g_orig = g_curr & b_orig = b_curr
             END

   'LOADSCAN': BEGIN
		read_scan
		index = 0
		display_scan
		widget_control, control_next,/sensitive		; more than 1 matrix 
	  	if(index eq maxindex) then widget_control, control_next,sensitive=0
		widget_control, control_previous,sensitive=0
	   end

   'NEXT' : $
	 begin
	  index=index+1
	  display_scan
	  if(index eq maxindex) then widget_control, control_next,sensitive=0
	  if(index eq 1) then widget_control, control_previous,/sensitive
	 end

   'PREVIOUS' : $
	begin
	  index=index-1
	  display_scan
	  if(index eq maxindex-1) then widget_control, control_next,/sensitive
	  if(index eq 0) then widget_control, control_previous,sensitive=0
	end

    'SETMATRIX' : $
	begin
	  previndex = index
	  index=event.index
	  if (index eq previndex) then return
	  display_scan
	  widget_control, control_next,sensitive=1
	  widget_control, control_previous,sensitive=1
	  if(index eq maxindex) then widget_control, control_next,sensitive=0
	  if(index eq 0) then widget_control, control_previous,sensitive=0
	end

    'DONE' : WIDGET_CONTROL, event.top,/DESTROY
ENDCASE
END

PRO make_contour, GROUP=GROUP

common 	comsc, 	scnfile,scans,nf,np,ng,nb,mats,specs,index,maxindex,f,p,g,b, $
		scan1,nprojs,nviews,plane_sep,sixel_size,scan2,nviews2, $
		old_border_thr, modscan2  
common  comim,	imgfile,images,image1,img_size,slice_width,zoom,xoff,yoff,rotoff, $
		image2,image_there
common  comfi,	fft_size,filter,cutoff,order,filfun
common  comno,	normfile,norms,norm,norm_there
common  comat,  atnfile,atten,mu,old_mu
common  combo,	angle,border,dborder,border_there
common 	comwi,	xmcbase, xmcdraw,xscbase,xscdraw,ximdraw,label1,label2,ldraw1,ldraw2,ldraw3, $
		win_num1,win_num2,win_num3,history,text2,xdraw1,xdraw2,xdraw3, $
		draw1,draw2,draw3,message,matlist,matlabel, $
		control_previous, control_next, control_showhat,control_attenu, $
		ordersl,orderlabel,cutoffsl,cutofflabel,zoomsl,zoomlabel, lf
common comcol, 	cred,cwhite
common colors,	r_orig,g_orig,b_orig,r_curr,g_curr,b_curr

ON_ERROR, 2

lf = "\x0a"
border_there = 0
old_border_thr = 15.
old_mu = 0.096
modscan2=0
fft_size = 512		; min value
img_size = 128		; already slow enough
norm_there = 0
image_there = 0
index=0
thick = !p.thick
psym = !p.psym
!x.charsize=1 & !y.charsize=1 & !z.charsize=1
image1=fltarr(128,128)
image2=fltarr(256,256)

xmcbase = WIDGET_BASE(TITLE = 'Brain Calculated Attenuation and Reconstruction',/COLUMN)

xmcontrol1 = WIDGET_BASE(xmcbase,/ROW)
control_loadscn = WIDGET_BUTTON(xmcontrol1, VALUE = "Load Scan", UVALUE = 'LOADSCAN')
control_previous = WIDGET_BUTTON(xmcontrol1, VALUE = "<<", UVALUE = 'PREVIOUS')
control_next = WIDGET_BUTTON(xmcontrol1, VALUE = ">>", UVALUE = 'NEXT')
control_proj = WIDGET_BUTTON(xmcontrol1, VALUE = "Projection", UVALUE = 'PROJECTION')
control_clut = WIDGET_BUTTON(xmcontrol1, VALUE = "CLUT", UVALUE = 'CLUT')
control_exit = WIDGET_BUTTON(xmcontrol1, VALUE = "Exit", UVALUE = 'DONE')
xmcontrol2 = WIDGET_BASE(xmcbase,/ROW)
control_rebin = WIDGET_BUTTON(xmcontrol2, VALUE = "Rebin", UVALUE = 'REBIN')
control_contour = WIDGET_BUTTON(xmcontrol2, VALUE = "Contour", UVALUE = 'CONTOUR')
control_showhat = WIDGET_BUTTON(xmcontrol2, VALUE = "Display Hat", UVALUE = 'SHOWHAT')
control_norm911 = WIDGET_BUTTON(xmcontrol2, VALUE = "Normalize_911", UVALUE = 'NORM911')
;control_normal = WIDGET_BUTTON(xmcontrol2, VALUE = "Normalize", UVALUE = 'NORMALIZE')
control_attenu = WIDGET_BUTTON(xmcontrol2, VALUE = "Calculate Attenuation", UVALUE = 'ATTENUATION')
control_recon = WIDGET_BUTTON(xmcontrol2, VALUE = "Reconstruct Image", UVALUE = 'RECON')

status = WIDGET_BASE(xmcbase, /FRAME,/ROW, XPAD = 10, SPACE = 30)
statuslabel = WIDGET_LABEL(status, VALUE = " Status ")
message = WIDGET_LABEL(status, VALUE = " Starting  ... ")

history = WIDGET_TEXT(xmcbase, VALUE = 'History ....', xsize=70,YSIZE = 8,/scroll,/FRAME)
label1 = WIDGET_LABEL(xmcbase, VALUE = 'Use left mouse to see coordinates and value')
text2 = WIDGET_TEXT(xmcbase, VALUE = '', xsize=70,YSIZE = 3,/FRAME)

xmcdraw = WIDGET_BASE(xmcbase,/row)
xscdraw = WIDGET_BASE(xmcdraw,/row)
ximdraw = WIDGET_BASE(xmcdraw,/row)
xdraw3 = WIDGET_BASE(ximdraw,/column)
ldraw3 = WIDGET_LABEL(xdraw3, VALUE = " Image ")
draw3 = WIDGET_DRAW(xdraw3,/BUTTON_EVENTS, /FRAME, UVALUE = 'DRAW_WIN_EVENT3', $
		 RETAIN = 2, XSIZE = 256, YSIZE = 256)

xoff = 0. & yoff = 0. & rotoff = 0. & filter = 3 & order = 2 & cutoff = 0.5

bkpars = WIDGET_BASE(xdraw3,/ROW)
translation = WIDGET_BUTTON(bkpars, VALUE = " Translation (x,y) ", UVALUE = 'TRANSLATION')
rotate = WIDGET_BUTTON(bkpars, VALUE = " Rotation ", UVALUE = 'ROTATION')
zoombase=WIDGET_BASE(xdraw3,/column)
zoomsl = WIDGET_SLIDER(zoombase,VALUE=150,MINIMUM=1,MAXIMUM=1000,UVALUE = 'ZOOM',/SUPPRESS_VALUE)
zoom = 150.
zoomlabel = WIDGET_LABEL(zoombase, VALUE = "Zoom " + strcompress(STRING(zoom/100.)))

fpars = WIDGET_BASE(xdraw3,/ROW)
ftypes = ["Ramp","Butter", "Hann", "Hamming","Parzen"]
fuser = ["RAMP","BUTTER", "HANN", "HAMM","PARZEN"]
XMenu, ftypes, fpars, UVALUE=fuser,/EXCLUSIVE, /ROW, BUTTONS = filterset, TITLE = "Filter"

cutordbase = WIDGET_BASE(xdraw3,/row)
cutoffbase=WIDGET_BASE(cutordbase,/column)
cutoffsl = WIDGET_SLIDER(cutoffbase,XSIZE=100,VALUE=20,MINIMUM=1,MAXIMUM=20, $
		UVALUE = 'CUTOFF',/SUPPRESS_VALUE)
cutofflabel = WIDGET_LABEL(cutoffbase, VALUE = "Cutoff at " + strcompress(STRING(cutoff)))
orderbase=WIDGET_BASE(cutordbase,/column)
ordersl = WIDGET_SLIDER(orderbase,XSIZE=100,VALUE=2,MINIMUM=1,MAXIMUM=10,UVALUE = 'ORDER',/SUPPRESS_VALUE)
orderlabel = WIDGET_LABEL(orderbase, VALUE = "Order " + strcompress(string(order)))

print,' Reading first scan, please be patient ...'
read_scan,/new
if( scnfile eq '') then return

WIDGET_CONTROL, xmcbase, /REALIZE
WIDGET_CONTROL, draw3, GET_VALUE = win_num3
WIDGET_CONTROL, filterset(2), /SET_BUTTON		; Hann
WIDGET_CONTROL, ORDERSL, SENSITIVE = 0
WIDGET_CONTROL, ORDERlabel, SENSITIVE = 0

display_scan
widget_control, control_next,/sensitive		; more than 1 matrix 
if(index eq maxindex) then widget_control, control_next,sensitive=0
widget_control, control_previous,sensitive=0
widget_control, control_showhat,sensitive=0
widget_control, control_attenu,sensitive=0

loadct,14	; UCLA
cred =1 & cwhite=0
r_curr(0)= 255 	& g_curr(0)= 255 	& b_curr(0)=255	; white
r_curr(1)= 255	& g_curr(1)=   0 	& b_curr(1)=  0	; red
tvlct,r_curr, g_curr, b_curr				; Load clut
r_orig = r_curr & g_orig = g_curr & b_orig = b_curr

XMANAGER, "xmc", xmcbase, GROUP_LEADER=GROUP

END


