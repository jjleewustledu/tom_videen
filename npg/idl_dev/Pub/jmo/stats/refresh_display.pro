;********************************************
pro refresh_display,fi,dsp,stc,wd,IMAGE=image
;********************************************
 

if (keyword_set(image) eq 0) then begin
    pln = dsp.zdim*(dsp.frame-1) + dsp.plane-1
    image = get_image(pln,fi,stc)
endif

dsp.sizex = fi.zoom(fi.n)*dsp.xdim
dsp.sizey = fi.zoom(fi.n)*dsp.ydim
dsp.zoom = fi.zoom(fi.n)
if(dsp.sizex ne !DRAW_SIZEX or dsp.sizey ne !DRAW_SIZEY or wd.draw lt 0) then begin
    if(wd.draw ge 0) then $
        widget_control,wd.drawbase,/DESTROY
    !DRAW_SIZEX = dsp.sizex
    !DRAW_SIZEY = dsp.sizey
    wd.drawbase = widget_base(TITLE='Working window',/COLUMN, $
	GROUP_LEADER=wd.leader)
    wd.draw = widget_draw(wd.drawbase,/BUTTON_EVENTS,COLORS=!LEN_COLORTAB, $
            XSIZE=dsp.sizex,YSIZE=dsp.sizey,RETAIN=2, $
	    KILL_NOTIFY=string('working_dead'))
    widget_control,wd.drawbase,/REALIZE
    xmanager,'stats_exec',wd.drawbase
    widget_control,GET_VALUE=i,wd.draw
    dsp.image_index = i
endif
wset,dsp.image_index
widget_control,wd.draw,/SHOW

if(dsp.type eq !OVERLAY_ARRAY) then begin
    scale = 1
    display,image,min,max,SCL=scale,FIXED_MIN=0,ZOOM=dsp.zoom
    loadct,!OVERLAY_SCALE,NCOLORS=!LEN_COLORTAB,FILE=!COLOR_TABLE,/SILENT
    dsp.color_table_index = fi.color_scale(fi.n)
    adjust_colors,fi
endif else begin
    if(dsp.scale eq !FIXED) then begin
        scale = !LEN_COLORTAB/float(dsp.fixed_max - dsp.fixed_min) 
        display,image,min,max,ZOOM=dsp.zoom,SCL=scale,FIXED_MIN=dsp.fixed_min
        dsp.min = dsp.fixed_min
        dsp.max = dsp.fixed_max
    endif else begin
        display,image,min,max,ZOOM=dsp.zoom
        dsp.min = min
        dsp.max = max
    endelse
    if(fi.color_scale(fi.n) ne dsp.color_table_index) then begin
        dsp.color_table_index = fi.color_scale(fi.n)
        loadct,fi.color_scale(fi.n),NCOLORS=!LEN_COLORTAB,FILE=!COLOR_TABLE,/SILENT
        adjust_colors,fi
    endif
endelse

if(dsp.color_scale eq !TRUE) then $
    showcolors,'Main',wd,dsp
 
return
end
