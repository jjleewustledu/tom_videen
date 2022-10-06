;**************************************
pro print_stats,dsp_image,dsp,wd,fi,stc
;**************************************

min = min(dsp_image(*,*))
max = max(dsp_image(*,*))
sum = total(dsp_image(*,*))
mean = sum/(dsp.xdim*dsp.ydim)
out = strcompress(string('Pln: ',dsp.plane,' Frm: ',dsp.frame,'  Min: ',min,' Max: ',max,' Mean: ',mean))
print,out
widget_control,wd.error,SET_VALUE=out
img = fltarr(dsp.xdim,dsp.ydim,dsp.zdim)
for z=0,dsp.zdim-1 do $
    img(*,*,z) = get_image(dsp.zdim*(dsp.frame-1) + z,fi,stc)
min = min(img)
max = max(img)
sum = total(img)
mean = sum/dsp.xdim/dsp.ydim/dsp.zdim
out = strcompress(string('Volume stats: ',' Frm: ',dsp.frame,'  Min: ',min,' Max: ',max,' Mean: ',mean))
print,out

return
end
