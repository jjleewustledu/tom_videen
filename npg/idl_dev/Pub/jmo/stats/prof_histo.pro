;*************************
pro prof_histo,pr,dsp,help
;*************************

if(help.enable eq !TRUE) then begin
    value='Plot a histogram of values in the most recently plotted profile.'
    widget_control,help.id,SET_VALUE=value
endif
handle_value,pr.profcmds(pr.cur_window),prd
prof = prd.profdat(*,1)
xmax = max(prof)
len = n_elements(prof)
mean = total(prof)
sig = sqrt((len*total(prof^2) - mean^2)/len/(len-1))
binsize = sig/4
mean = mean/len
prof(*) = prof(*) - mean
histo = histogram(prof,OMIN=hmin,OMAX=hmax,MIN=-4*sig,MAX=4*sig,BINSIZE=binsize)
;print,binsize,sig,mean,hmin,hmax
title = string("Histogram of profile")
dum = 0
label = strcompress(string('Profile_',pr.cur_window,'_histo'),/REMOVE_ALL)
profile,histo,dum,pr,dsp,label,prd.profdat,TITLE=title,XMIN=hmin/sig,XMAX=hmax/sig

return
end
