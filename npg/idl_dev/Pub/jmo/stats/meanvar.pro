;***************************
pro meanvar,fi,pr,dsp,wd,stc
;***************************


value = strarr(1)
label = strarr(1)
value(0) = string('0')
label(0) = string("Delay  (in images): ")
dims = get_str(1,label,value)
delay = long(dims(0))

widget_control,/HOURGLASS
handle_value,fi.hdr_handle(fi.n),hdr

mean  = fltarr(dsp.xdim,dsp.ydim,dsp.zdim)
stdev = fltarr(dsp.xdim,dsp.ydim,dsp.zdim)

for z=0,dsp.zdim-1 do begin
    widget_control,wd.error,SET_VALUE=string('Plane: ',z+1)
    n = 0
    image = get_image(z+dsp.zdim*dsp.tdim/2,fi,stc)
    thresh = max(image(*,*))/2
    mask = image(*,*) gt thresh
    for i=delay,dsp.tdim-1 do begin
        image = get_image(z+i*dsp.zdim,fi,stc)
        mean(*,*,z)  =  mean(*,*,z) + image(*,*)
        stdev(*,*,z) = stdev(*,*,z) + image(*,*)^2
        n = n + 1
    end

    stdev(*,*,z)  = sqrt((n*stdev(*,*,z) - mean(*,*,z)^2)/(n*(n-1)))
    mean(*,*,z) = mean(*,*,z)/n
end
    
name1 = fi.name
name = strcompress(string(name1,'_mean'),/REMOVE_ALL)
put_image,mean,fi,wd,dsp,name,dsp.xdim,dsp.ydim,dsp.zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,fi.n

name = strcompress(string(name1,'_stdev'),/REMOVE_ALL)
put_image,stdev,fi,wd,dsp,name,dsp.xdim,dsp.ydim,dsp.zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,fi.n

widget_control,HOURGLASS=0

return
end
