;*********************************
pro sinecorr,fi,pr,dsp,wd,stc,help
;*********************************

common linear_fit,fit_name,slope,intcpt,trend

if(help.enable eq !TRUE) then begin
    widget_control,help.id,SET_VALUE=string('Computes the magnitude and phase of the Fourier series term with the period specified in the dialog box.  Delay specifies the number of images to skip before processing begins.  The result is stored as the magnitude and phase in radians.')
endif
value = strarr(3)
label = strarr(3)
value(0) = string('46')
value(1) = string('0')
label(0) = string("Period (in images): ")
label(1) = string("Delay  (in images): ")
dims = get_str(2,label,value)
period = long(dims(0))
delay = long(dims(1))

handle_value,fi.hdr_handle(fi.n),hd
handle_value,fi.hdr_handle(hd.mother),hdr

if(strpos(fi.tails(hd.mother),fit_name) ge 0) then begin
    label = strarr(2)
    label(0) = string('Subtract trend')
    label(1) = string("Ignore trend")
    title = string('Use linear fit to subtract trend?')
    lcfit = get_boolean(title,label)
endif else begin
    lcfit = !FALSE
endelse

widget_control,/HOURGLASS
sincorr = fltarr(hdr.xdim,hdr.ydim)
coscorr = fltarr(hdr.xdim,hdr.ydim)
mag  = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
phase = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
paradigm = fltarr(hdr.tdim)
paradigm = fi.paradigm(hd.mother,*)

for z=0,hdr.zdim-1 do begin
    widget_control,wd.error,SET_VALUE=string('Plane: ',z+1)
    t = 0
    sincorr(*,*) = 0.
    coscorr(*,*) = 0.
    image = get_image(z,fi,stc,FILNUM=hd.mother+1)
    thresh = max(image(*,*))/2
    mask = image(*,*) gt thresh
    for t1=delay,hdr.tdim-1 do begin
        image = get_image(z+t1*hdr.zdim,fi,stc,FILNUM=hd.mother+1)
        if(lcfit eq !TRUE) then $
            image = image - t*slope(*,*,z)
        sinscl = sin(2*!PI*t/period)
        cosscl = cos(2*!PI*t/period)
        sincorr = sincorr + sinscl*image(*,*)
        coscorr = coscorr + cosscl*image(*,*)
        t = t + 1
    end

    mag(*,*,z) = sqrt(sincorr^2 + coscorr^2)
    phase(*,*,z) = mask*atan(sincorr(*,*)/coscorr(*,*))
end
    
filnum = hd.mother
name = strcompress(string(fi.tails(hd.mother),period,FORMAT='(a,"_phase_T",i)'),/REMOVE_ALL)
put_image,phase,fi,wd,dsp,name,dsp.xdim,dsp.ydim,dsp.zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,filnum,paradigm

name = strcompress(string(fi.tails(hd.mother),period,FORMAT='(a,"_mag_T",i)'),/REMOVE_ALL)
put_image,mag,fi,wd,dsp,name,dsp.xdim,dsp.ydim,dsp.zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,filnum,paradigm

widget_control,HOURGLASS=0

return
end
