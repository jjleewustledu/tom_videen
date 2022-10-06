;*****************************
pro update_image,fi,dsp,wd,stc
;*****************************

handle_value,fi.hdr_handle(fi.n),hdr
dsp.xdim = hdr.xdim
dsp.ydim = hdr.ydim
dsp.zdim = hdr.zdim
dsp.tdim = hdr.tdim
dsp.type = hdr.array_type
if(dsp.plane ge dsp.zdim) then dsp.plane = dsp.zdim
if(dsp.plane le        0) then dsp.plane = 1
widget_control,wd.sld_pln,SET_SLIDER_MIN=1,SET_SLIDER_MAX=dsp.zdim
widget_control,wd.sld_pln,SET_VALUE=dsp.plane
if(dsp.frame ge dsp.tdim) then dsp.frame = dsp.tdim
if(dsp.frame le        0) then dsp.frame = 1
widget_control,wd.sld_frm,SET_SLIDER_MIN=1,SET_SLIDER_MAX=dsp.tdim
widget_control,wd.sld_frm,SET_VALUE=dsp.frame
if(fi.n ge fi.nfiles) then fi.n = fi.nfiles-1
dsp_image = get_image(dsp.zdim*(dsp.frame-1) + dsp.plane-1,fi,stc)
refresh_display,fi,dsp,stc,wd,IMAGE=dsp_image

return
end
