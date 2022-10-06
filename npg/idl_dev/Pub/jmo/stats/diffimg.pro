;************************************************
pro diffimg,fi,pr,dsp,wd,stc,help,DIFFIMG=diffimg
;************************************************

; Compute difference images by correlating with square waves of different
;  phases.

common linear_fit,fit_name,slope,intcpt,trend

if(help.enable eq !TRUE) then begin
    widget_control,help.id,SET_VALUE=string('Computes the difference image by subtracting the average valued during rest (activation) from the average value during activation (rest).  A linear trend can be subtracted from the data. (The prompt only appears if a linear fit has already been done).'+'  The percent change image is calculated by dividing the difference image by the mean resting image and multiplying by 100.'+' All pixels with a magnitude less than 10% of the per-plane peak are masked off to suppress large percent changes outside the brain.')
endif

if(keyword_set(diffimg)) then $
    ldiff = !TRUE $
else $
    ldiff = !FALSE

handle_value,fi.hdr_handle(fi.n),hd
handle_value,fi.hdr_handle(hd.mother),hdr

if(fi.paradigm_loaded(hd.mother) ne !TRUE) then begin
    stat=widget_message('*** No stimulus time-course defined.  ***',/ERROR)
    return
endif


value = strarr(1)
label = strarr(1)
value(0) = string('0')
label(0) = string("FWHM of Gaussian filter (pixels): ")
dims = get_str(1,label,value)
fwhm = float(dims(0))

if(fwhm gt 0.) then begin
    sigma = fwhm/2.35
    kdim = 2*round(2*sigma) - 1

    f = fltarr(kdim)
    krnl = fltarr(kdim,kdim)
    fm1 = gauss_pdf(0.5/sigma)
    f(0) = 2*(fm1 - 0.5)
    x = 1.5
    for i=1,kdim/2 do begin
	f0= gauss_pdf(x/sigma)
	f(i) = f0 - fm1
	fm1 = f0
	x = x + 1.
    end
    for i=0,kdim-1 do begin
        for j=0,kdim-1 do begin
	    ii = abs(i - kdim/2)
	    jj = abs(j - kdim/2)
	    krnl(i,j) = f(ii)*f(jj)
	end
    end
end

label = strarr(2)
label(0) = string('Subtract trend')
label(1) = string("Ignore trend")
title = string('Use linear fit to subtract trend?')
lcfit = get_boolean(title,label)
if(strpos(fi.tails(hd.mother),fit_name) lt 0 and lcfit eq !TRUE) then $
    linfit,hd.mother,fi,pr,dsp,wd,stc,help

widget_control,/HOURGLASS

len_plus = 0
len_minus = 0
for t=0,hdr.tdim-1 do begin
    if(fi.paradigm(hd.mother,t) gt 0) then $
	len_plus = len_plus + 1 
    if(fi.paradigm(hd.mother,t) lt 0) then $
	len_minus = len_minus + 1
endfor
plus = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
minus = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
mean = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
mask = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
paradigm = fltarr(hdr.tdim)
paradigm = fi.paradigm(hd.mother,*)

for z=0,hdr.zdim-1 do begin
    nplus = 0
    nminus = 0
    widget_control,wd.error,SET_VALUE=string('Plane: ',z+1)
    for t=0,hdr.tdim-1 do begin
	if(paradigm(t) ne 0) then begin
	    if(lcfit eq !TRUE) then $
                img = get_image(t*hdr.zdim+z,fi,stc,FILNUM=hd.mother+1,/SUBTRACT_TREND) $
 	    else $
                img = get_image(t*hdr.zdim+z,fi,stc,FILNUM=hd.mother+1)
	    if(fwhm gt 0.) then $
	        image = convol(img,krnl,/CENTER,/EDGE_TRUNCATE) $
	    else $
	        image = img
	    if(paradigm(t) lt 0) then begin
	        minus(*,*,z) = minus(*,*,z) + image
	        mean(*,*,z) = mean(*,*,z) + image
	        nminus = nminus + 1
            endif
	    if(paradigm(t) gt 0) then begin
	            plus(*,*,z) = plus(*,*,z) + image
	            nplus = nplus + 1
	    endif
	endif
    end
end

plus = plus/float(nplus) - minus/float(nminus)

mask = mean gt  0
minus = plus/(mean + (not mask)) ; Avoid division by zero.
; Mask out noise-only regions.
for z=0,hdr.zdim-1 do $
    mask(*,*,z) = mean(*,*,z) gt (.1*max(mean(*,*,z)))
minus = 100.*float(nminus)*mask*minus

    name = strcompress(string(fi.tails(hd.mother),lcfit,FORMAT='(a,"_percent_chng_f",i)'),/REMOVE_ALL)
    put_image,minus,fi,wd,dsp,name,hdr.xdim,hdr.ydim,hdr.zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,hd.mother,paradigm

    name = strcompress(string(fi.tails(hd.mother),lcfit,FORMAT='(a,"_diff_f",i)'),/REMOVE_ALL)
    put_image,plus,fi,wd,dsp,name,hdr.xdim,hdr.ydim,hdr.zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,hd.mother,paradigm

if(ldiff eq !TRUE) then begin
    diffimg(*,*,*) = plus(*,*,*)
endif

widget_control,HOURGLASS=0

return
end
