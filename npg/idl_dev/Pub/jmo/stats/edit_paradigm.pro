;*************************
pro edit_paradigm_event,ev
;*************************

common get_paradigm_comm,paradigm,tdim,wplot,wslider,wrest,wact,wbad, $
	wdone,plot_window,image_index,xm1,frame,x_axis,paradigm_old, $
	wcancel

strs = !TRUE
case ev.id of

wslider: begin
    wset,plot_window
    xxx = intarr(16)
    device,SET_GRAPHICS=6,CURSOR_IMAGE=xxx
    widget_control,wslider,GET_VALUE=frame
    x = fix((!PARADIGM_WIDTH-2*!PARADIGM_OFFSET-1)*float(frame-1)/tdim) $
	+ !PARADIGM_OFFSET
    if(xm1 ge 0) then $
        plots,[xm1,xm1],[0,!PARADIGM_HGHT],COLOR=!D.TABLE_SIZE-1,/DEV,/NOCLIP
    plots,[x,x],[0,!PARADIGM_HGHT],COLOR=!D.TABLE_SIZE-1,/DEV,/NOCLIP
    wset,image_index
    device,SET_GRAPHICS=3,/CURSOR_CROSSHAIR     ;Resume normal graphics.
    xm1 = x
end

wrest: begin
    paradigm(frame-1) = -1
    wset,plot_window
    plot,x_axis,paradigm,XMARGIN=5,COLOR=!WHITE_INDEX,XRANGE=[1,tdim+1], $
	YRANGE=[-2,1],XSTYLE=9,YSTYLE=20,PSYM=3,SYMSIZE=2,XTITLE='Frame', $
	POSITION=[!PARADIGM_OFFSET,!PARADIGM_Y0, $
	!PARADIGM_WIDTH-!PARADIGM_OFFSET,!PARADIGM_Y1],/DEVICE
    xm1 = -1	; Don't try to cover up last line in wslider:
    wset,image_index
end

wact: begin
    paradigm(frame-1) = 1
    wset,plot_window
    plot,x_axis,paradigm,XMARGIN=5,COLOR=!WHITE_INDEX,XRANGE=[1,tdim+1], $
	YRANGE=[-2,1],XSTYLE=9,YSTYLE=20,PSYM=3,SYMSIZE=2,XTITLE='Frame', $
	POSITION=[!PARADIGM_OFFSET,!PARADIGM_Y0, $
	!PARADIGM_WIDTH-!PARADIGM_OFFSET,!PARADIGM_Y1],/DEVICE
    xm1 = -1	; Don't try to cover up last line in wslider:
    wset,image_index
end

wbad: begin
    paradigm(frame-1) = 0
    wset,plot_window
    plot,x_axis,paradigm,XMARGIN=5,COLOR=!WHITE_INDEX,XRANGE=[1,tdim+1], $
	YRANGE=[-2,1],XSTYLE=9,YSTYLE=20,PSYM=3,SYMSIZE=2,XTITLE='Frame', $
	POSITION=[!PARADIGM_OFFSET,!PARADIGM_Y0, $
	!PARADIGM_WIDTH-!PARADIGM_OFFSET,!PARADIGM_Y1],/DEVICE
    xm1 = -1	; Don't try to cover up last line in wslider:
    wset,image_index
end

wdone: widget_control,ev.top,/DESTROY

wcancel: begin
    paradigm = paradigm_old
    widget_control,ev.top,/DESTROY
end

else: stat=widget_message('*** Invalid event in edit_paradigm.  ***',/ERROR)

endcase

return
end

;***********************
pro edit_paradigm,fi,dsp
;***********************

common get_paradigm_comm

handle_value,fi.hdr_handle(fi.n),hd
handle_value,fi.hdr_handle(hd.mother),hdr
tdim = hdr.tdim
if(tdim le 2) then begin
    stat=widget_message('*** Paradigm can only be defined for time-course data.  ***',/ERROR)
    return
endif
paradigm = fltarr(tdim)
paradigm_old = fltarr(tdim)
paradigm(0:tdim-1) = fi.paradigm(hd.mother,0:tdim-1)
paradigm_old(0:tdim-1) = fi.paradigm(hd.mother,0:tdim-1)
xm1 = -1

intbase = widget_base(/COLUMN,TITLE='Stimulus')
plot = widget_base(intbase,/COLUMN)
slider = widget_base(intbase,/ROW)
buttons = widget_base(intbase,/ROW)

wplot = widget_draw(plot,XSIZE=!PARADIGM_WIDTH,YSIZE=!PARADIGM_HGHT)
wslider = widget_slider(slider,Title='Frame Number',SCROLL=1, $
	/DRAG,MINIMUM=1,MAXIMUM=tdim,XSIZE=!PARADIGM_WIDTH)
frame=1
wrest = widget_button(buttons,VALUE='Rest')
wact = widget_button(buttons,VALUE='Activation')
wbad = widget_button(buttons,VALUE='Ignore')
wdone = widget_button(buttons,VALUE='Done')
wcancel = widget_button(buttons,VALUE='Cancel')

widget_control,intbase,/REALIZE

x_axis = indgen(tdim)
x_axis(*) = x_axis(*) + 1
widget_control,GET_VALUE=plot_window,wplot
wset,plot_window
plot,x_axis,paradigm,XMARGIN=5,COLOR=!WHITE_INDEX,XRANGE=[1,tdim+1], $
	YRANGE=[-2,1],XSTYLE=9,YSTYLE=20,PSYM=3,SYMSIZE=2,XTITLE='Frame', $
	POSITION=[!PARADIGM_OFFSET,!PARADIGM_Y0, $
	!PARADIGM_WIDTH-!PARADIGM_OFFSET,!PARADIGM_Y1],/DEVICE
image_index = dsp.image_index
wset,dsp.image_index

xmanager,'edit_paradigm',intbase,/MODAL

fi.paradigm(hd.mother,0:tdim-1) = paradigm(0:tdim-1)
fi.paradigm_code(hd.mother) = encode_paradigm(paradigm,tdim)
fi.paradigm_loaded(hd.mother) = !TRUE

return
end

