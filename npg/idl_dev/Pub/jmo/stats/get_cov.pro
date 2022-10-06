;**************************************************
pro get_cov,fi,pr,dsp,wd,stc,help,x_pos,y_pos,z_pos
;**************************************************

; Compute difference images by correlating with square waves of different
;  phases.

common linear_fit,fit_name,slope,intcpt,trend

if(help.enable eq !TRUE) then begin
    widget_control,help.id,SET_VALUE=string('Computes the difference image by subtracting the average valued during rest (activation) from the average value during activation (rest).  A linear trend can be subtracted from the data. (The prompt only appears if a linear fit has already been done).')
endif

handle_value,fi.hdr_handle(fi.n),hd
handle_value,fi.hdr_handle(hd.mother),hdr

if(fi.paradigm_loaded(hd.mother) ne !TRUE) then begin
    stat=widget_message('*** No stimulus time-course defined.  ***',/ERROR)
    return
endif


value = strarr(3)
label = strarr(3)
value(0) = string('0.')
label(0) = string("FWHM of Gaussian filter (pixels): ")
dims = get_str(1,label,value)
fwhm   = float(dims(0))

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

widget_control,/HOURGLASS

m = fltarr(hdr.xdim,hdr.ydim)
var = fltarr(hdr.xdim,hdr.ydim)
cov = fltarr(hdr.xdim,hdr.ydim)
cc = fltarr(hdr.xdim,hdr.ydim)
paradigm = fltarr(hdr.tdim)
paradigm = fi.paradigm(hd.mother,*)
N = 0

for t1=0,hdr.tdim-1 do begin
    if(paradigm(t1) lt 0) then begin
        img = get_image(t1*hdr.zdim+z_pos,fi,stc,FILNUM=hd.mother+1)
	m = m + img
	cov = cov + img(x_pos,y_pos)*img
	var = var + img*img
	N = N + 1
    endif
end

var =  (N*var - m*m)/(N*(N-1))
cov =  (N*cov - m(x_pos,y_pos)*m)/(N*(N-1))
cc = cov/sqrt(var*var(x_pos,y_pos))

zdim = 1
name = strcompress(string(fi.tails(hd.mother),x_pos,y_pos,z_pos+1,FORMAT='(a,"_cov_",i,"_",i,"_",i)'),/REMOVE_ALL)
put_image,cov,fi,wd,dsp,name,hdr.xdim,hdr.ydim,zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,hd.mother,paradigm

name = strcompress(string(fi.tails(hd.mother),x_pos,y_pos,z_pos+1,FORMAT='(a,"_var_",i,"_",i,"_",i)'),/REMOVE_ALL)
put_image,var,fi,wd,dsp,name,hdr.xdim,hdr.ydim,zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,hd.mother,paradigm

name = strcompress(string(fi.tails(hd.mother),x_pos,y_pos,z_pos+1,FORMAT='(a,"_cc_",i,"_",i,"_",i)'),/REMOVE_ALL)
put_image,cc,fi,wd,dsp,name,hdr.xdim,hdr.ydim,zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,hd.mother,paradigm

widget_control,HOURGLASS=0

return
end
