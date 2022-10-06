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
imgout = fltarr(xdim(0),ydim(0),maxpln)

; Spawn process to write image to stdout, then read it.
cmd = string('cub2stdout ',filnam,' -minpln ',' -maxpln ',maxpln)
spawn,cmd,UNIT=fd
readu,fd,imgtmp
free_lun,fd

i = 0
for z=minpln-1,maxpln-1 do begin
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
	MAX_PLANE=maxplane,FRAME=frm,DATA=dat,BED=bed
;****************************************************************************
 
on_error, 0
if (n_params() lt 7) then begin
  print,"Usage: rd_cti7,filnam,image,xdim,ydim,zdim,dxdy,dz,MIN_PLANE,MAX_PLANE"
  stat = -1
  return
endif
 
if(keyword_set(frm)) then frame = frm else frame = 1
if(keyword_set(dat))  then dat  = dat else dat  = 0
if(keyword_set(bed))  then bed  = bed else bed  = 0
if(keyword_set(minplane)) then minpln = minplane else minpln=1
if(keyword_set(maxplane)) then maxpln = maxplane else maxpln = -1

if(rstrpos(infile,".") eq (strlen(infile)-2)) then begin
    xdat = dat
    rd_cti7,infile,stat,imgout,xdim,ydim,zdim,dxdy,dz,minpln, $
        maxpln,frame,xdat,bed
    return
endif

gate = 1
stat = recat6(infile,frame,1,gate,0,0,img,/NO_SCALE,/NO_DATA,/zoom)
if(stat eq !ECAT.ERROR) then begin
    gate = 0
    stat = recat6(infile,frame,1,gate,0,0,img,/NO_SCALE,/NO_DATA,/zoom)
    if(stat eq !ECAT.ERROR) then begin
	print,'Error reading ',infile
	return
    endif
endif

stat = recat6(infile,frame,minpln,gate,dat,bed,dt,/NO_SCALE,/NO_DATA,/zoom)
if (stat EQ !ECAT.ERROR) then begin
  print, "Error reading ",infile," frame ",frame
  stat = -1
  return
endif
xdim = dt.sh.dimension_1
ydim = dt.sh.dimension_2
zdim = dt.mh.file.nplanes
if(keyword_set(maxplane)) then maxpln = maxplane else maxpln=zdim
if(zdim eq 0) then zdim = 31 ;For 3d atn files with invalid main header.
if(dt.mh.file.file_type eq 2) then begin
    dxdy = dt.sh.pixel_size
    dz1   = dt.sh.slice_width
endif else begin
    if(dt.mh.file.file_type eq 1 or dt.mh.file.file_type eq 3) then begin
        dxdy = dt.sh.sample_distance
        dz1   = dt.sh.sample_distance
    endif else begin
	dxdy = 1
	dz1 = 1
    endelse
endelse
if(dxdy eq 0.) then dxdy = .2086
if(dz1 eq 0.)   then dz1 = .338


imgout = fltarr(xdim,ydim,maxpln)

dz = fltarr(maxpln)

for plane=minpln,maxpln do begin
  data = dat
  stat = recat6(infile, frame, plane, gate, data, 0, dt, ZOOM=1)
  if (stat EQ !ECAT.ERROR) then begin
    print, "Error reading plane",plane," of ",infile
    stat = -1
    return
  endif
  imgout(*,*,plane-1) = dt.data
  if(dt.mh.file.file_type eq 4) then begin
;	Normalization file.
        dz(plane-1) = dt.sh.ecat_calib_factor
  endif else begin
	dz(plane-1) = dz1
  endelse
endfor
 
stat = 0
return
end

;****************************************************************************
pro rd_cti7,infile,stat,imgout,xdim,ydim,zdim,dxdy,dz,minpln, $
	maxpln,frame,dat,bed
;****************************************************************************
 
on_error, 0
if (n_params() lt 7) then begin
  print,"Usage: rd_cti,filnam,image,xdim,ydim,zdim,dxdy,dz,MIN_PLANE,MAX_PLANE"
  stat = -1
  return
endif

gate = 1
stat = recat7(infile,frame,minpln,gate,dat,bed,dt,/NO_SCALE,/NO_DATA,/zoom)
if (stat EQ !ECAT.ERROR) then begin
  print, "Error reading ",infile," frame ",frame
  stat = -1
  return
endif

case !ECAT.file_types(dt.mh.file_type) of
    "V" : begin
;	Image volume.
	xdim = dt.sh.x_dimension
	ydim = dt.sh.y_dimension
	zdim = dt.sh.z_dimension
        dxdy = dt.sh.x_pixel_size
        dz1 = dt.sh.z_pixel_size
    end
    "S" : begin
;	Sinogram.
	xdim = dt.sh.num_r_elements
	ydim = dt.sh.num_angles
	zdim = dt.sh.num_z_elements
        dxdy = 1
        dz1 = 1
    end
    13: begin
;	Normalization file.
	xdim = dt.sh.num_r_elements
	ydim = dt.sh.num_angles
	zdim = dt.sh.num_z_elements
 	dxdy = 1
        dz1 = dt.sh.scale_factor
    end
    else: begin
        print,"Unidentified file type in rd_cti7"
	stat = -1
        return
    end
endcase
if(maxpln  lt 0) then maxpln = zdim
dz = fltarr(maxpln)
for plane=minpln,maxpln do $
    dz(plane-1) = dz1
if(dxdy eq 0.) then dxdy = .2086
if(dz1 eq 0.)   then dz1 = .338

stat = recat7(infile, frame, minpln, gate, dat, bed, dt, ZOOM=1)
if (stat EQ !ECAT.ERROR) then begin
    print, "Error reading plane",plane," of ",infile
    stat = -1
    return
endif

if(!ECAT.file_types(dt.mh.file_type) eq "S") then begin
    y = ydim
    ydim = maxpln
    maxpln = y
    zdim = maxpln
endif
imgout = reform(dt.data,xdim(0),ydim(0),maxpln)
 
stat = 0
return
end

;**************************
function getdim_7,infile
;*************************

on_error, 0
dim = intarr(7)
dim(*) = 0
if (n_params() ne 1) then begin
  print,"Usage: getdim_7,infile
  return,dim
endif

openr,unit,infile,/get_lun
stat = rmh7(unit,mh)
close,unit
if(stat EQ !ECAT.ERROR) then begin
    print,"Could not read main header of ",infile
    dim(0) = -1
    return,dim
endif

stat = recat7(infile,mh.num_frames,1,1,0,0,img,/NO_SCALE,/NO_DATA,/zoom)
if (stat eq !ECAT.ERROR) then begin
    dim(0) = -1
    return,dim
endif
mindat = 0
maxdat = 0

case !ECAT.file_types(mh.file_type) of
    "V" : begin
        dim(0) = img.sh.x_dimension
        dim(1) = img.sh.y_dimension
	dim(2) = img.mh.num_planes
	dim(6) = img.mh.num_bed_pos
    end
    "S" : begin
        dim(0) = img.sh.num_r_elements
        dim(1) = img.sh.num_angles
	dim(2) = img.sh.num_z_elements
	dim(6) = img.mh.num_bed_pos
    end
    else: begin
        print,"Image type not supported: ",!ECAT.file_types(dt.mh.file_type)
        dim(0) = -1
	return,dim
    end
endcase
dim(3) = img.mh.num_frames
dim(4) = mindat
dim(5) = maxdat
if(dim(2) eq 0) then dim(2) = 256
if(dim(3) eq 0) then dim(3) = 1

return,dim
end

;**************************
function get_cti_dim,infile
;**************************

on_error, 0
dim = intarr(7)
dim(*) = 0
if (n_params() ne 1) then begin
  print,"Usage: get_cti_dim,infile
  return,dim
endif

if(rstrpos(infile,".") eq (strlen(infile)-2)) then begin
    dim = getdim_7(infile)
    return,dim
endif

data = 0
frame = 1
gate = 1
stat = recat6(infile,frame,1,gate,data,0,img,/NO_SCALE,/NO_DATA,/zoom)
if (stat eq !ECAT.ERROR) then begin
   frame = 2
   stat = recat6(infile,frame,1,gate,data,0,img,/NO_SCALE,/NO_DATA,/zoom)
   if (stat eq !ECAT.ERROR) then begin
       print,'Error reading image.'
       return,dim
   endif
endif

stat = recat6(infile,frame,1,gate,0,0,dt,/NO_SCALE,/NO_DATA,/zoom)
if(stat eq !ECAT.ERROR) then begin
    gate = 0
    stat = recat6(infile,frame,1,gate,0,0,dum,/NO_SCALE,/NO_DATA,/zoom)
    if(stat eq !ECAT.ERROR) then begin
	print,'Error reading ',infile
	dim(0) = -1
	return,dim
    endif
endif

mindat = 0
dat = 1
stat = recat6(infile,frame,1,gate,dat,0,dum,/NO_SCALE,/NO_DATA,/zoom)
if(stat ne !ECAT.ERROR) then $
    maxdat = 1 $
else $
    maxdat = 0

data = mindat
for pln = 1,dt.mh.file.nplanes do begin
    stat = recat6(infile,frame,pln,gate,data,0,img,/NO_SCALE,/NO_DATA,/zoom)
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
dim(6) = img.mh.file.nbeds
if(dim(2) eq 0) then dim(2) = 256
if(dim(3) eq 0) then dim(3) = 1

return,dim
end

;**********************
function rd_mhdr,infile
;**********************
 
on_error, 0
if (n_params() lt 1) then begin
  print,"Usage: rd_mhdr,infile
  return,-1
endif

openr,unit,infile,/get_lun
if(rstrpos(infile,".") eq (strlen(infile)-2)) then begin
    stat = rmh7(unit,mh)
    if(stat EQ !ECAT.ERROR) then return,-1
endif else begin
    stat = rmh6(unit,mh)
    if(stat EQ !ECAT.ERROR) then return,-1
endelse
close,unit

return,mh
end

;***************************************
function rd_shdr,infile,plane,frame,data
;***************************************
 
on_error, 0
if (n_params() lt 3) then begin
  print,"Usage: rd_shdr,infile,plane,frame,data
  return,-1
endif

mh = rd_mhdr(infile)

gate = 1
bed = 0
openr,unit,infile,/get_lun
if(rstrpos(infile,".") eq (strlen(infile)-2)) then begin
;   type = 7
    stat = rdir7(unit,directory,dinfo)
    mats = mkmats(frame,plane,gate,data,bed,directory)
    if((mats(0) EQ !ECAT.ERROR) ) then begin
        return,-1
    endif
    if(rsubheaders7(mh.file_type,unit,mats(1,*),sh,scales) EQ !ECAT.ERROR) then $
           return,-1
endif else begin
;   type = 6
    stat = rdir6(unit,directory,dinfo)
    mats = mkmats(frame,plane,gate,data,bed,directory)
    if(rsubheaders6(mh.file_type,unit,mats(1,*),sh,scales) EQ !ECAT.ERROR) then $
           return,-1
endelse
close,unit

;gate = 1
;stat = recat6(infile,1,1,gate,0,0,img,/NO_SCALE,/NO_DATA,/zoom) $
;if(stat eq !ECAT.ERROR) then begin
;    gate = 0
;    stat = recat6(infile,1,1,gate,0,0,img,/NO_SCALE,/NO_DATA,/zoom) $
;    if(stat eq !ECAT.ERROR) then begin
;	print,'Error reading ',infile
;	return,-1
;    endif
;endif
 
;if(type eq 6) then $
;    stat = recat6(infile,frame,plane,gate,data,0,dt,/NO_SCALE,/NO_DATA,/zoom) $
;  else $
;    stat = recat6(infile,frame,plane,gate,data,0,dt,/NO_SCALE,/NO_DATA,/zoom)
;if (stat EQ !ECAT.ERROR) then begin
;  print, "Error reading ",infile," frame ",frame
;  return,-1
;endif

return,sh
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
