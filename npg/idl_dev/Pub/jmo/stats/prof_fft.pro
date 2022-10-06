;***********************
pro prof_fft,pr,dsp,help
;***********************
 
if(help.enable eq !TRUE) then begin
    value='Plot the power spectrum of the current profile.'
   widget_control,help.id,SET_VALUE=value
endif
handle_value,pr.profcmds(pr.cur_window),prd
len = n_elements(prd.profdat(*,1))
nzero = 0
for i=0,len-1 do $
    if(prd.profdat(i,1) eq 0.) then nzero =nzero + 1
prof = fltarr(len-nzero)
prof = prd.profdat(nzero:len-1,1)
len = len - nzero
data = fltarr(2*len) ; Zero pad for fft routine.
data = fltarr(len)
data(0:len-1) = prof
ft = fft(data,-1)
mag = abs(ft)
;mag(0) = mag(1)  ; DC component always zero because mean of time-series set
;	           to zero
mask = mag gt 1.e-12
mag = mag*mask + 1.e-12*(1-mask)
power = fltarr(len)
power(0:len-1) = smooth(mag(0:len-1),3)
title = string('Power spectrum of profile',pr.cur_window)
dum = 0
label = strcompress(string('Profile_',pr.cur_window,'_power_spectrum'),/REMOVE_ALL)
;profile,power,dum,pr,dsp,label,prd.profdat,TITLE=title,/YLOG
profile,power(0:len/2-1),dum,pr,dsp,label,prd.profdat,TITLE=title

ft(22:25) = 0.
ft(len-24:len-21) = 0
;ft(0) = 0
data = fft(ft,1)
d = float(data)
dum = 0
row = 0
name = ' '
;time_profile,d,row,pr,dsp,name,profdat
 
return
end

