;************************************************
function get_profile,row,fi,pr,dsp,wd,st,stc,help
;************************************************

; Get profile through an image and return vector of values and row.
; row: Row number of profile.

device, set_graphics=6             ;Set XOR mode
pln = dsp.zdim*(dsp.frame-1) + dsp.plane-1
image = get_image(pln,fi,stc)
cursor,x,y,/WAIT,/DEV	;Get current position.
if(help.enable eq !TRUE) then begin
    value='Left button: Select row, Middle button: Exit profile mode, Right button: Plot profile.'+'The column/row are displayed in the dialog window.'+'  The selected coordinates are converted to image coordinates for computation of the profile.'+'  After it is plotted, the region represented by the profile is outlined with a green line.'
    widget_control,help.id,SET_VALUE=value
endif
if(pr.horzprof eq !TRUE) then begin
   y1 = y - dsp.zoom*(pr.height-1)/2
   y2 = y + dsp.zoom*(pr.height-1)/2
   if(pr.height gt 1) then $
        row=(fix((!DRAW_SIZEY-y2-1)/dsp.zoom)+fix((!DRAW_SIZEY-y1-1)/dsp.zoom))/2 $
    else $
       	row = fix((!DRAW_SIZEY - y1 -1)/dsp.zoom)
    widget_control,wd.error,SET_VALUE=string('Row: ',row)
    y1 = y - dsp.zoom*(pr.height-1)/2
    y2 = y1 + dsp.zoom*(pr.height-1)
    plots,[0,!DRAW_SIZEY],[y1,y1],COLOR=!D.TABLE_SIZE-1,/DEV,/NOCLIP
    if(pr.height gt 1) then $
        plots,[0,!DRAW_SIZEY],[y2,y2],COLOR=!D.TABLE_SIZE-1,/DEV,/NOCLIP
    y1m1 = y1
    y2m1 = y2
    ym1 = y
    repeat begin
        cursor,x,y,/WAIT,/DEV	;Get current position.
        row = fix((!DRAW_SIZEY - y -1)/dsp.zoom)
        if(y ne ym1) then begin
            row = fix((!DRAW_SIZEY - y -1)/dsp.zoom)
            if(pr.height gt 1) then begin
                y1 = !DRAW_SIZEY - 1 - (dsp.zoom*(row - pr.height/2) - dsp.zoom/2)
                y2 = !DRAW_SIZEY - 1 - (dsp.zoom*(row + pr.height/2) + dsp.zoom/2)
	    endif else begin
                y1 = !DRAW_SIZEY - 1 - (dsp.zoom*(row - pr.height/2))
	    endelse
            widget_control,wd.error,SET_VALUE=string('Row: ',row)
            plots,[0,!DRAW_SIZEY],[y1m1,y1m1],COLOR=!D.TABLE_SIZE-1,/DEV,/NOCLIP
    	    if(pr.height gt 1) then $
                plots,[0,!DRAW_SIZEY],[y2m1,y2m1],COLOR=!D.TABLE_SIZE-1,/DEV,/NOCLIP
            plots,[0,!DRAW_SIZEY],[y1,y1],COLOR=!D.TABLE_SIZE-1,/DEV,/NOCLIP
    	    if(pr.height gt 1) then $
                plots,[0,!DRAW_SIZEY],[y2,y2],COLOR=!D.TABLE_SIZE-1,/DEV,/NOCLIP
    	    y1m1 = y1
    	    y2m1 = y2
	    ym1 = y
        endif
    endrep until !err eq 4
    plots,[0,!DRAW_SIZEY],[y1,y1],COLOR=!D.TABLE_SIZE-1,/DEV,/NOCLIP
    if(pr.height gt 1) then $
        plots,[0,!DRAW_SIZEY],[y2,y2],COLOR=!D.TABLE_SIZE-1,/DEV,/NOCLIP
    device,SET_GRAPHICS=3	;Resume normal graphics.
    if(pr.height gt 1) then begin
	row1 = row - pr.height/2
	row2 = row + pr.height/2
	if(row1 lt 0) then row1 = 0
	if(row1 gt dsp.ydim-1) then row1 = dsp.ydim-1
	if(row2 lt 0) then row2 = 0
	if(row2 gt dsp.ydim-1) then row2 = dsp.ydim-1
        prof = total(image(*,row1:row2),2)/(row2 - row1 + 1)
    endif else begin
	if(row gt dsp.ydim-1) then row = dsp.ydim-1
        prof = image(*,row)
    endelse
    y1 = !DRAW_SIZEY - 1 - (dsp.zoom*(row - pr.height/2) - dsp.zoom/2)
    y2 = !DRAW_SIZEY - 1 - (dsp.zoom*(row + pr.height/2) + dsp.zoom/2)
    rect_plot,0,dsp.zoom*dsp.xdim,y1,y2,COLOR=pr.color
    pr.coord(0) = row
endif else begin ; Vertical profile.
    x1 = x - dsp.zoom*(pr.width-1)/2
    x2 = x1 + dsp.zoom*(pr.width-1)/2
    if(pr.width gt 1) then row=(x1+x2)/(2*dsp.zoom) else row=x1/dsp.zoom
    widget_control,wd.error,SET_VALUE=string('Column: ',row)
    plots,[x1,x1],[0,!DRAW_SIZEX],COLOR=pr.color,/DEV,/NOCLIP
    if(pr.width gt 1) then $
        plots,[x2,x2],[0,!DRAW_SIZEX],COLOR=pr.color,/DEV,/NOCLIP
    x1m1 = x1
    x2m1 = x2
    xm1 = x
    repeat begin
        cursor,x,y,/WAIT,/DEV	;Get current position.
        if(x ne xm1) then begin
	    row = x/dsp.zoom
            widget_control,wd.error,SET_VALUE=string('Column: ',row)
            if(pr.width gt 1) then begin
                x1 = dsp.zoom*(row - pr.width/2) - dsp.zoom/2
                x2 = dsp.zoom*(row + pr.width/2) + dsp.zoom/2
	    endif else begin
                x1 = dsp.zoom*(row - pr.width/2)
	    endelse
            plots,[x1m1,x1m1],[0,!DRAW_SIZEX],COLOR=pr.color,/DEV,/NOCLIP
	    if(pr.width gt 1) then $
                plots,[x2m1,x2m1],[0,!DRAW_SIZEX],COLOR=pr.color,/DEV,/NOCLIP
            plots,[x1,x1],[0,!DRAW_SIZEX],COLOR=pr.color,/DEV,/NOCLIP
	    if(pr.width gt 1) then $
                plots,[x2,x2],[0,!DRAW_SIZEX],COLOR=pr.color,/DEV,/NOCLIP
    	    x1m1 = x1
    	    x2m1 = x2
	    xm1 = x
        endif
    endrep until !err eq 4
    plots,[x1m1,x1m1],[0,!DRAW_SIZEX],COLOR=pr.color,/DEV,/NOCLIP
    if(pr.width gt 1) then $
        plots,[x2m1,x2m1],[0,!DRAW_SIZEX],COLOR=pr.color,/DEV,/NOCLIP
    device,SET_GRAPHICS=3	;Resume normal graphics.
    if(pr.width gt 1) then begin
	row1 = row - pr.width/2
	row2 = row + pr.width/2
	if(row1 lt 0) then row1 = 0
	if(row1 gt dsp.ydim-1) then row1 = dsp.ydim-1
	if(row2 lt 0) then row2 = 0
	if(row2 gt dsp.ydim-1) then row2 = dsp.ydim-1
        prof = total(image(row1:row2,*),1)/(row2 - row1 + 1)
    endif else begin
	if(row lt 0) then row = 0
	if(row gt dsp.ydim-1) then row = dsp.ydim-1
        prof = image(row,*)
    endelse
    x1 = dsp.zoom*(row - pr.width/2) - dsp.zoom/2
    x2 = dsp.zoom*(row + pr.width/2) + dsp.zoom/2
    rect_plot,x1,x2,!DRAW_SIZEX-dsp.zoom*dsp.xdim,!DRAW_SIZEX,COLOR=pr.color
    pr.coord(1) = row
endelse


; Smooth profile with boxcar filter.
if(pr.boxcar_width gt 0) then $
    profile = smooth(prof,pr.boxcar_width) $
else $
    profile = prof

return,profile

end
