;*********************************
pro wilcoxon,fi,pr,dsp,wd,stc,help
;*********************************

; Compute two-tailed Wilcoxon test.

common linear_fit,fit_name,slope,intcpt,trend

if(help.enable eq !TRUE) then begin
    x='Computes the two-tailed wilcoxon test. (See Erich Lehmann, "Nonparametric: Statistical Methods Based on Rank," 1975, Holden Day) ' + 'This is a nonparametric test based on rank.  It makes no assumptions about the underlying distribution.' + ' Unlike the KS test, quantization in the data is explicitly accounted for.  ' + 'The significance probability relies on a Gaussian approximation for the rank (Note: this is the probability of the computed RANK, not of the measured data).  ' + 'This approximation becomes inaccurate for very low significance probabilities but is accurate for higher probabilities.  It is very reliable for .05 and .01 significance levels.'+  '  Calculated images are: (1) WCprob, the raw significance probability (negatives are deactivations), (2) WClogp, the log of the significance probability, '+' WCsig, the significance of the result (significance levels of .05, .02, .01, .005, .002, ,.001, 1e-4 ... 1e-7 are displayed by summing thresholded images).'
    widget_control,help.id,SET_VALUE=x
endif

;Get image header.
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

label = strarr(2)
label(0) = string('Subtract trend')
label(1) = string("Ignore trend")
title = string('Use linear fit to subtract trend?')
lcfit = get_boolean(title,label)
if(strpos(fi.tails(hd.mother),fit_name) lt 0 and lcfit eq !TRUE) then $
    linfit,hd.mother,fi,pr,dsp,wd,stc,help

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

; Assume there could be one extra half-period for len_minus.
len_plus = 0
len_minus = 0
for t=0,hdr.tdim-1 do begin
    if(fi.paradigm(hd.mother,t) gt 0) then $
	len_plus = len_plus + 1
    if(fi.paradigm(hd.mother,t) lt 0) then $
	len_minus = len_minus + 1
endfor
minus = fltarr(hdr.xdim,hdr.ydim,len_minus)
plus  = fltarr(hdr.xdim,hdr.ydim,len_plus)
prob = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
logp = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
sig = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
prob1 = fltarr(hdr.xdim,hdr.ydim)
sig1 = fltarr(hdr.xdim,hdr.ydim)
sig2 = fltarr(hdr.xdim,hdr.ydim)
paradigm = fltarr(hdr.tdim)
paradigm = fi.paradigm(hd.mother,*)

for z=0,hdr.zdim-1 do begin
    nplus = 0
    nminus = 0
    t = 0
;   Compute mask.
    image = get_image(z,fi,stc,FILNUM=hd.mother+1)
    thresh = max(image(*,*))/5.
    mask    = float(image(*,*) gt thresh)
    maskbar = float(image(*,*) le thresh)

    widget_control,wd.error,SET_VALUE=string('Plane: ',z+1)
;   First arrange the plane in an array dimensioned (x,y,t)
    for t=0,hdr.tdim-1 do begin
	if(fi.paradigm(hd.mother,t) ne 0) then begin
            if(lcfit eq !TRUE) then $
                image = get_image(t*hdr.zdim+z,fi,stc,FILNUM=hd.mother+1,/SUBTRACT_TREND) $
	    else $
                image = get_image(t*hdr.zdim+z,fi,stc,FILNUM=hd.mother+1)
	    if(fwhm gt 0.) then $
	        img = convol(image,krnl,/CENTER,/EDGE_TRUNCATE) $
	    else $
	        img = image
	    if(fi.paradigm(hd.mother,t) lt 0) then begin
                minus(*,*,nminus) = img
                nminus = nminus + 1
	    endif
	    if(fi.paradigm(hd.mother,t) gt 0) then begin
                plus(*,*,nplus) = img
                nplus = nplus + 1
	    endif
        endif
    end
;   Now do the Wilcoxon test on each pixel.
    passtype = bytarr(7)
    passtype(3:6) = 1
    stat=call_external(!SHARE_LIB,'_wilcoxon',plus,minus, $
	prob1,hdr.xdim,hdr.ydim,nplus,nminus,/F_VALUE,VALUE=passtype)
;    prob(*,*,z) = mask(*,*)*prob1
    prob(*,*,z) = prob1
    mask    = float(prob(*,*,z) gt 0.)
    maskbar = float(prob(*,*,z) lt 0.)
    maskzro = float(prob(*,*,z) eq 0.)
    logp(0,0,z) = 1.e-10
    logp(*,*,z) = -alog10(prob(*,*,z)*mask + maskbar + maskzro) $
                +  alog10(-prob(*,*,z)*maskbar + maskzro + mask)
    prob1 = mask*prob(*,*,z) - maskbar*prob(*,*,z)
    sig1 =  prob1 lt .0000001
    sig(*,*,z) = 7*sig1
    sig2 = (prob1 lt .000001)
    sig(*,*,z) = sig(*,*,z) + 6*(sig2 and (not sig1))
    sig1 = (prob1 lt .00001)
    sig(*,*,z) = sig(*,*,z) + 5*(sig1 and (not sig2))
    sig2 = (prob1 lt .0001)
    sig(*,*,z) = sig(*,*,z) + 4*(sig2 and (not sig1))
    sig1 = (prob1 lt .001)
    sig(*,*,z) = sig(*,*,z) + 3*(sig1 and (not sig2))
    sig2 = (prob1 lt .002)
    sig(*,*,z) = sig(*,*,z) + 2.69*(sig2 and (not sig1))
;    sig1 = (prob1 lt .005)
;    sig(*,*,z) = sig(*,*,z) + 4*(sig1 and (not sig2))
;    sig2 = (prob1 lt .01)
;    sig(*,*,z) = sig(*,*,z) + 3*(sig2 and (not sig1))
;    sig1 = (prob1 lt .02)
;    sig(*,*,z) = sig(*,*,z) + 2*(sig1 and (not sig2))
;    sig2 = (prob1 lt .05)
;    sig(*,*,z) = sig(*,*,z) +   (sig2 and (not sig1))
    sig(*,*,z) = (mask - maskbar)*sig(*,*,z)
;   Ensure image is not zero to eliminate arithmetic error.
    if(total(sig(*,*,z)) eq 0) then sig(0,0,z) =1.e-10
end

;n_reg_size = fltarr(1000)
;mult_comp,prob,mult_comp_mask,n_reg_size
;sig = sig*mult_comp_mask

;name = strcompress(string(fi.tails(fi.n),FORMAT='(a,"_sig_mult_comp")'),/REMOVE_ALL)
;put_image,mult_comp_mask,fi,wd,dsp,name,hdr.xdim,hdr.ydim,hdr.zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,hdr.mother,paradigm

;name = strcompress(string(fi.tails(hd.mother),FORMAT='(a,"_WCprob")'),/REMOVE_ALL)
;put_image,prob,fi,wd,dsp,name,hdr.xdim,hdr.ydim,hdr.zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,hd.mother,paradigm

name = strcompress(string(fi.tails(hd.mother),FORMAT='(a,"_KSD")'),/REMOVE_ALL)
;put_image,D,fi,wd,dsp,name,hdr.xdim,hdr.ydim,hdr.zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,hd.mother,paradigm

name = strcompress(string(fi.tails(hd.mother),FORMAT='(a,"_WCsig")'),/REMOVE_ALL)
put_image,sig,fi,wd,dsp,name,hdr.xdim,hdr.ydim,hdr.zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,hd.mother,paradigm

;sig = sig*(mult_comp_mask gt 0)
;name = strcompress(string(fi.tails(hd.mother),FORMAT='(a,"_WCsig_multcomp")'),/REMOVE_ALL)
;put_image,sig,fi,wd,dsp,name,hdr.xdim,hdr.ydim,hdr.zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,hd.mother,paradigm

name = strcompress(string(fi.tails(hd.mother),FORMAT='(a,"_WClogp")'),/REMOVE_ALL)
put_image,logp,fi,wd,dsp,name,hdr.xdim,hdr.ydim,hdr.zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,hd.mother,paradigm

widget_control,HOURGLASS=0

return
end
