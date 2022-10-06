;***********************
pro plot_stimulus,fi,dsp
;***********************

handle_value,fi.hdr_handle(fi.n),hd
handle_value,fi.hdr_handle(hd.mother),hdr
tdim = hdr.tdim
if(tdim le 2) then begin
    stat=widget_message('*** Paradigm can only be defined for time-course data.  ***',/ERROR)
    return
endif
stimulus_base = widget_base(/COLUMN,TITLE=fi.tail)
plot = widget_base(stimulus_base,/COLUMN)
wplot = widget_draw(plot,XSIZE=!PARADIGM_WIDTH,YSIZE=!PARADIGM_HGHT)
widget_control,stimulus_base,/REALIZE
widget_control,GET_VALUE=plot_window,wplot
wset,plot_window
x_axis = findgen(tdim+1) + 1.

plot,x_axis,fi.paradigm(hd.mother,0:tdim-1),XMARGIN=5,COLOR=!WHITE_INDEX,XRANGE=[1,tdim+1], $
    YRANGE=[-2,1],XSTYLE=9,YSTYLE=20,PSYM=0,SYMSIZE=2,XTITLE='Frame', $
    POSITION=[!PARADIGM_OFFSET,!PARADIGM_Y0, $
    !PARADIGM_WIDTH-!PARADIGM_OFFSET,!PARADIGM_Y1],/DEVICE
wset,dsp.image_index

return
end
