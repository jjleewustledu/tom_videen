;****************************************************************************
pro display,imgin,min,max,ZOOM=zoom,NPANES=npanes,SCL=scl,FIXED_MIN=fixed_min
;****************************************************************************

nargs=n_params()
if(nargs eq 0) then begin
     print,'Usage: display,image[,ZOOM=zoom_factor]
     print,'Displays all 2d images in the 3D array "image".'
     return
endif

xdim = n_elements(imgin(*,0,0))
ydim = n_elements(imgin(0,*,0))
zdim = n_elements(imgin(0,0,*))

if keyword_set(zoom) then begin
     zoomx = xdim*zoom
     zoomy = ydim*zoom
endif else begin
     zoomx = xdim
     zoomy = ydim
endelse

if (keyword_set(NPANES) eq 0) then npanes = zdim
if (keyword_set(SCL)) then begin
    lscale = !FALSE 
    min = 0.
endif else begin
    lscale = !TRUE
endelse
if (keyword_set(FIXED_MIN)) then min = fixed_min

img = fltarr(xdim,ydim)
for z=0,npanes-1 do begin
    if (lscale eq !TRUE) then begin
        min = float(min(imgin(*,*,z)))
        max = float(max(imgin(*,*,z)))
	if(max ne min) then begin
            scl = !LEN_COLORTAB/(max - min)
            img = scl(0)*(imgin(*,*,z) - min)
	endif else begin
	    img(*,*) = 0.
	endelse
    endif else begin
	if(scl(0) ne 0.) then begin
            img = scl(0)*(imgin(*,*,z) - min)
	    img = img*(img gt 0)
	    mask = img le !LEN_COLORTAB
	    img = img*(img le !LEN_COLORTAB) + (img gt !LEN_COLORTAB)*!LEN_COLORTAB
	endif else begin
	    img(*,*) = 0.
	endelse
    endelse
    if(zoomx ne xdim) or (zoomy ne ydim) then begin
	img = rebin(img,zoomx,zoomy)
    endif
    tv,img,z,/ORDER
endfor
	
return
end
