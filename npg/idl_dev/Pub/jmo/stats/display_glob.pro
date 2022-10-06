;*****************************************************
pro display_glob,imgin,min,max,ZOOM=zoom,NPANES=npanes
;*****************************************************

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

min = min(imgin)
max = max(imgin)
scl = !LEN_COLORTAB/(max - min)

if (keyword_set(npanes) eq 0) then npanes = zdim

img = fltarr(xdim,ydim)
for z=0,npanes-1 do begin
    img = scl(0)*(imgin(*,*,z) - min)
    if(zoomx ne xdim) then begin
	img = rebin(img,zoomx,zoomy)
    endif
    tv,img,z,/ORDER
endfor

return
end
