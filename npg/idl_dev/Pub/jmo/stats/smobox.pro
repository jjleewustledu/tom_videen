;*******************
pro smobox,fi,dsp,wd
;*******************

label = strarr(2)
label(0) = string('3D')
label(1) = string('2D')
title = string('Filter across 2D planes or 3D volume?')
dim3d = get_boolean(title,label)

value = strarr(1)
label = strarr(1)
value(0) = string('0')
label(0) = string("Width of Boxcar filter in pixels: ")
dims = get_str(1,label,value)
width = float(dims(0))

widget_control,/HOURGLASS

handle_value,fi.hdr_handle(fi.n),hdr
paradigm = fltarr(hdr.tdim)
paradigm = fi.paradigm(hdr.mother,*)

if(dim3d eq !TRUE) then begin
    img = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
    for z=0,hdr.zdim-1 do begin
	img(*,*,z) = get_image(dsp.zdim*(dsp.frame-1) + z,fi,stc)
    end
    simg = box_smoth(img,width)
    name = strcompress(string(fi.name,dsp.frame,width,FORMAT='(a,"_f",i2,"_3Dboxcar_",f5.2)'),/REMOVE_ALL)
endif else begin
    simg = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
    for z=0,hdr.zdim-1 do begin
	img = get_image(dsp.zdim*(dsp.frame-1) + z,fi,stc)
        simg(*,*,z) = box_smoth(img,width)
    end
    name = strcompress(string(fi.name,dsp.frame,width,FORMAT='(a,"_f",i2,"_2Dboxcar_",f5.2)'),/REMOVE_ALL)
endelse

put_image,simg,fi,wd,dsp,name,dsp.xdim,dsp.ydim,hdr.zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,fi.n,paradigm

widget_control,HOURGLASS=0

return
end
