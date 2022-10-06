;*************************************************************
pro rd_cub,filnam,imgout,MIN_PLANE=minplane,MAX_PLANE=maxplane
;*************************************************************

nargs=n_params()
if(nargs eq 0) then begin
     print,'Usage: rd_cube file-name'
     return
endif


; first get dimensions of the file on disc.
cmd = string('cubparam ',filnam,' 0 xdim') 
spawn,cmd,nx
cmd = string('cubparam ',filnam,' 0 ydim') 
spawn,cmd,ny
cmd = string('cubparam ',filnam,' 0 zdim') 
spawn,cmd,nz

; Convert dimensions to integer.
xdim = fix(nx)
ydim = fix(ny)
zdim = fix(nz)

if(keyword_set(maxplane)) then maxpln = maxplane else maxpln=zdim(0)-1
if(keyword_set(minplane)) then minpln = minplane else minpln=0

; Allocate storage.
imgtmp = fltarr(xdim(0),ydim(0),zdim(0))
imgout = fltarr(xdim(0),ydim(0),maxpln+1)

; Spawn process to write image to stdout, then read it.
cmd = string('cub2stdout ',filnam,' -minpln ',' -maxpln ',maxpln)
spawn,cmd,UNIT=fd
readu,fd,imgtmp
free_lun,fd

i = 0
for z=minpln,maxpln do begin
    imgout(*,*,z) = imgtmp(*,*,i)
    i = i + 1
endfor

return

end

;********************************************
pro rd_cub_subset,fullcubfil,subsetfil,imgout
;********************************************

nargs=n_params()
if(nargs eq 0) then begin
     print,'Usage: rd_cub_subset,fully-3d-cube-file,subset-cube-file,imgout'
     print,'Extracts images in "fully-3d-cube-file" corresponding to those in "subsetfill"'
     return
endif

; first get dimensions of the file on disc.
cmd = string('cubparam ',subsetfil,' 0 xdim') 
spawn,cmd,nx
cmd = string('cubparam ',subsetfil,' 0 ydim') 
spawn,cmd,ny
cmd = string('cubparam ',subsetfil,' 0 zdim') 
spawn,cmd,nz

; Convert dimensions to integer.
xdim = fix(nx)
ydim = fix(ny)
zdim = fix(nz)

img = fltarr(xdim(0),ydim(0))

; Allocate storage.
imgout = fltarr(xdim(0),ydim(0),zdim(0))

for z=0,zdim(0)-1 do begin
    ; Get plane of corresponding image in fully-3d file.
    cmd = string('cubparam ',subsetfil,' ',z,' pln_3d')
    spawn,cmd,plane

    ; write image to stdout, then read it.
    cmd = string('cubextract ',fullcubfil,' - ',' -pln ',plane) 
    spawn,cmd,UNIT=fd
    readu,fd,img
    imgout(*,*,z) = img
    free_lun,fd
endfor

return
end

;*******************************
pro wr_cub,filnam,template,image
;*******************************

nargs=n_params()
if(nargs eq 0) then begin
     print,'Usage: wr_cub,filnam,template,image'
     return
endif

; Convert dimensions to integer.
xdim = n_elements(image(*,0,0))
ydim = n_elements(image(0,*,0))
zdim = n_elements(image(0,0,*))

for z=0,zdim-1 do begin
    cmd = string('cubparam ',template,' z id') 
    spawn,cmd,id

    cmd = string('cubfrmstdin ',filnam,' ',xdim,' ',ydim,' ',z,' ',id,format='(a,a,a,i3,a,i3,a,i3,a,a)')
    spawn,cmd,UNIT=fd
    writeu,fd,image(*,*,z)
    readu,fd,response
    free_lun,fd
endfor

return

end


;************************************************
pro display,imgin,min,max,ZOOM=zoom,NPANES=npanes
;************************************************

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

if (keyword_set(npanes) eq 0) then npanes = zdim

img = fltarr(xdim,ydim)
for z=0,npanes-1 do begin
    min = min(imgin(*,*,z))
    max = max(imgin(*,*,z))
    scl = !LEN_COLORTAB/(max - min)
;    for y=0,ydim-1 do begin
;	tmp = scl*(imgin(*,y,z) - min)
;	img(0,y) = scl*(imgin(*,ydim - y - 1,z) - min)
;	img(0,ydim - y - 1) = tmp
;    endfor
    img = scl(0)*(imgin(*,*,z) - min)
    if(zoomx ne xdim) or (zoomy ne ydim) then begin
	img = rebin(img,zoomx,zoomy)
    endif
    tv,img,z,/ORDER
endfor
	
return
end

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

;******************************
pro plotfile,filnam,header,data
;******************************

nargs=n_params()
if(nargs lt 3) then begin
     print,'Usage: plotfile,filnam,header,data
     return
endif

xdim = n_elements(data(*,0))
ydim = n_elements(data(0,*))

openw,fd,filnam,/get_lun
printf,fd,'*'
for j=0,ydim-2 do begin
    printf,fd,header(j),' ',format='(a,a,$)'
endfor
printf,fd,header(ydim-1),format='(a)'

for i=0,xdim-1 do begin
     for j=0,ydim-2 do begin
        printf,fd,data(i,j),format='(g12.4,2x,$)'
     endfor
     printf,fd,data(i,ydim-1),format='(1x,g12.4)'
endfor
close,fd

return
end


;****************
function tile,xin
;****************

nargs=n_params()
if(nargs lt 1) then begin
    print,'Usage: xout = tile(xin)'
    print,'Tile 3D image into side-by-side 2D images.'
    return,xin
endif

xdim = n_elements(xin(*,0,0))
ydim = n_elements(xin(0,*,0))
zdim = n_elements(xin(0,0,*))
xdimout = xdim*zdim
xout = fltarr(xdimout,ydim)

for y=0,ydim-1 do begin
    for z=0,zdim-1 do begin
	x1 = z*xdim
	x2 = (z+1)*xdim - 1
	xout(x1:x2,y) = xin(*,y,z)
    endfor
endfor

return,xout
end

;******************
function bytscl,xin
;******************

nargs=n_params()
if(nargs lt 1) then begin
    print,'Usage: imgscl,xin,xout'
    print,'Scale image to range of 0 - 255'
    return,xin
endif

min = min(xin)
max = max(xin)
scl = 255/(max - min)
xout = scl*(xin - min)

return,xout
end

;**********************
pro wrtif,filnam,data,z
;**********************

nargs=n_params()
if(nargs eq 0) then begin
     print,'Usage: wrtif,filnam,data,z
     return
endif

xdim = n_elements(data(*,0,0))
ydim = n_elements(data(0,*,0))
zdim = n_elements(data(0,0,*))

xtile = tile(data(*,*,z))
xscl = bytscl(xtile)
tiff_write,filnam,xscl,1

return
end

;******************
pro wrtgif,filnam,x
;******************

nargs=n_params()
if(nargs eq 0) then begin
     print,'Usage: wrtgif,filnam,x'
     return
endif

xdim = n_elements(x(*,0,0))
ydim = n_elements(x(0,*,0))

xflip = fltarr(xdim,ydim)

;Layout images side-by-side.
xtile = tile(x)

; Flip image top to bottom.
for y=0,ydim-1 do begin
    tmp = xtile(*,y)
    xflip(0,y) = x(*,ydim - y - 1)
    xflip(0,ydim - y - 1) = tmp
endfor

;Scale to range of 0-255.
xscl = bytscl(xflip)

;Write as GIF image.
write_gif,filnam,xscl

return
end

;****************************************************************************
pro rd_cti,infile,stat,imgout,xdim,ydim,zdim,dxdy,dz,MIN_PLANE=minplane, $
	MAX_PLANE=maxplane,FRAME=frm,DATA=dat
;****************************************************************************
 
on_error, 0
if (n_params() lt 7) then begin
  print,"Usage: rd_cti,filnam,image,xdim,ydim,zdim,dxdy,dz,MIN_PLANE,MAX_PLANE"
  stat = -1
  return
endif
 
if(keyword_set(frm)) then frame = frm else frame = 1
if(keyword_set(dat))  then dat  = dat else dat  = 0
if(dat eq 1) then dat = 4 ;CTI screwed up numbering of data.
if(keyword_set(minplane)) then minpln = minplane else minpln=1

stat = recat6(infile,frame,minpln,1,dat,0,dt,/no_scale,/no_data,/zoom)
if (stat EQ !ECAT.ERROR) then begin
  print, "Error reading ",infile," frame ",frame
  stat = -1
  return
endif
xdim = dt.sh.dimension_1
ydim = dt.sh.dimension_2
zdim = dt.mh.file.nplanes
if(zdim eq 0) then zdim = 31 ;For 3d atn files with invalid main header.
if(strpos(infile,'img') ge 0) then begin
    dxdy = dt.sh.pixel_size
    dz   = dt.sh.slice_width
endif else begin
    if((strpos(infile,'scn') ge 0) or (strpos(infile,'atn') ge 0)) then begin
        dxdy = dt.sh.sample_distance
        dz   = dt.sh.sample_distance
    endif else begin
	dxdy = 1
	dz = 1
    endelse
endelse
if(dxdy eq 0.) then dxdy = .2086
if(dz eq 0.)   then dz = .338

if(keyword_set(maxplane)) then maxpln = maxplane else maxpln=zdim

imgout = fltarr(xdim,ydim,maxpln+1)

for plane=minpln,maxpln do begin
  data = dat
  stat = recat6(infile, frame, plane, 1, data, 0, dt, ZOOM=1)
  if (stat EQ !ECAT.ERROR) then begin
    print, "Error reading plane",plane," of ",infile
    stat = -1
    return
  endif
  if(dt.mh.file.file_type eq 2) then begin
      if(dt.sh.loss_corr_fctr gt 0) then $
          imgout(*,*,plane-1) = dt.data*dt.sh.loss_corr_fctr $
      else $
	  imgout(*,*,plane-1) = dt.data
  endif else begin 
      imgout(*,*,plane-1) = dt.data
  endelse
endfor
 
stat = 0
return
end

;**************************
function get_cti_dim,infile
;**************************

on_error, 0
dim = intarr(6)
dim(*) = 0
if (n_params() ne 1) then begin
  print,"Usage: get_cti_dim,infile
  return,dim
endif

;mindat = -1
;maxdat = 0
;for pln=1,31 do begin
;  for ndata=0,5 do begin
;    stat = recat6(infile,1,pln,1,ndata,0,img,/no_scale,/no_data,/zoom)
;    if(stat ne !ECAT.ERROR) then begin
;	if (mindat eq -1) then begin
;		mindat = ndata
;	endif else begin
;		maxdat = ndata
; 	endelse
;    endif else begin
;	goto,DATA
;    endelse
;  endfor
;endfor
;DATA:
mindat = 0
stat = recat6(infile,1,1,1,4,0,img,/no_scale,/no_data,/zoom)
if(stat ne !ECAT.ERROR) then $
    maxdat = 1 $
else $
    maxdat = 0


data = mindat
for pln = 1,31 do begin
    stat = recat6(infile,1,pln,1,data,0,img,/no_scale,/no_data,/zoom)
    if (stat NE !ECAT.ERROR) then begin
      goto,PLANE
    endif
endfor
dim(0) = -1
return,dim
PLANE:

dim(0) = img.sh.dimension_1
dim(1) = img.sh.dimension_2
dim(2) = img.mh.file.nplanes
dim(3) = img.mh.file.nframes
dim(4) = mindat
dim(5) = maxdat
if(dim(2) eq 0) then dim(2) = 256
if(dim(3) eq 0) then dim(3) = 1

return,dim
end

;***************************************
function rd_shdr,infile,plane,frame,data
;***************************************
 
on_error, 0
if (n_params() lt 3) then begin
  print,"Usage: rd_shdr,infile,plane,frame,data
  return,-1
endif
 
stat = recat6(infile,frame,plane,frame,data,0,dt,/no_scale,/no_data,/zoom)
if (stat EQ !ECAT.ERROR) then begin
  print, "Error reading ",infile," frame ",frame
  return,-1
endif

return,dt.sh
end

;***************************************
function rd_mhdr,infile,plane,frame,data
;***************************************
 
on_error, 0
if (n_params() lt 3) then begin
  print,"Usage: rd_mhdr,infile,plane,frame,data
  return,-1
endif
 
stat = recat6(infile,frame,plane,frame,data,0,dt,/no_scale,/no_data,/zoom)
if (stat EQ !ECAT.ERROR) then begin
  print, "Error reading ",infile," frame ",frame
  return,-1
endif

return,dt.mh
end

;******************************
function plnnum_f3d,ring1,ring2
;******************************
 
on_error, 0
if (n_params() lt 2) then begin
  print,"Usage: plnnum_f3d,ring1,ring2"
  print,"ring1 and ring2 are detector rings numbered from 1 ... n"
  return,-1
endif

rng1 = ring1 - 1
rng2 = ring2 - 1
pln = ishft(rng1 and '10'xl,5) or ishft(rng1 and '8'xl,4) or (rng1 and 7) or $
      ishft(rng2 and '10'xl,4) or ishft(rng2 and 'f'xl,3)
pln = pln + 1

return,pln
end
