;************************************************
function get_zprofile,row,col,fi,pr,dsp,wd,st,stc
;************************************************

xxx = intarr(16)
device,set_graphics=6,cursor_image=xxx             ;Set XOR mode, Blank cursor
cursor,x,y,/WAIT,/DEV	;Get current position.
widget_control,wd.error,SET_VALUE=string('Left: Select row, Right: Plot profile.')
   y1 = y - dsp.zoom*(pr.height-1)/2
   y2 = y + dsp.zoom*(pr.height-1)/2
   if(pr.height gt 1) then $
        row=(fix((!DRAW_SIZE-y2-1)/dsp.zoom)+fix((!DRAW_SIZE-y1-1)/dsp.zoom))/2 $
    else $
       	row = fix((!DRAW_SIZE - y1 -1)/dsp.zoom)
    plots,[0,!DRAW_SIZE],[y1,y1],COLOR=!d.table_size-1,/DEV,/NOCLIP
    if(pr.height gt 1) then $
        plots,[0,!DRAW_SIZE],[y2,y2],COLOR=!d.table_size-1,/DEV,/NOCLIP
    y1m1 = y1
    y2m1 = y2
    x1 = x - dsp.zoom*(pr.width-1)/2
    x2 = x + dsp.zoom*(pr.width-1)/2
    if(pr.width gt 1) then row=(x1+x2)/(2*dsp.zoom) else row=x1/dsp.zoom
    plots,[x1,x1],[0,!DRAW_SIZE],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
    if(pr.width gt 1) then $
        plots,[x2,x2],[0,!DRAW_SIZE],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
    x1m1 = x1
    x2m1 = x2
    repeat begin
        cursor,x,y,/WAIT,/DEV	;Get current position.
        y1 = y - dsp.zoom*(pr.height-1)/2
        y2 = y + dsp.zoom*(pr.height-1)/2
	y1p = !DRAW_SIZE - y1 - 1
	y2p = !DRAW_SIZE - y2 - 1
        if(y1p/dsp.zoom lt 0) then  y1 = !DRAW_SIZE - 1
        if(y1p/dsp.zoom gt dsp.ydim-1) then y1 = !DRAW_SIZE-1-dsp.zoom*(dsp.ydim-1)
        if(y2p/dsp.zoom lt 0) then  y2 = !DRAW_SIZE - 1
        if(y2p/dsp.zoom gt dsp.ydim-1) then y2 = !DRAW_SIZE-1-dsp.zoom*(dsp.ydim-1)
	x1 = x - dsp.zoom*(pr.width-1)/2
	x2 = x + dsp.zoom*(pr.width-1)/2
        if(x1/dsp.zoom lt 0) then x1 = 0
        if(x2/dsp.zoom lt 0) then x2 = 0
        if(x1/dsp.zoom gt dsp.xdim-1) then x1 = dsp.zoom*dsp.xdim
        if(x2/dsp.zoom gt dsp.xdim-1) then x2 = dsp.zoom*dsp.xdim
        if(y1 ne y1m1) or (y2 ne y2m1) then begin
            plots,[0,!DRAW_SIZE],[y1m1,y1m1],COLOR=!d.table_size-1,/DEV,/NOCLIP
    	    if(pr.height gt 1) then $
                plots,[0,!DRAW_SIZE],[y2m1,y2m1],COLOR=!d.table_size-1,/DEV,/NOCLIP
            plots,[0,!DRAW_SIZE],[y1,y1],COLOR=!d.table_size-1,/DEV,/NOCLIP
    	    if(pr.height gt 1) then $
                plots,[0,!DRAW_SIZE],[y2,y2],COLOR=!d.table_size-1,/DEV,/NOCLIP
            if(pr.height gt 1) then $
	        row = fix((2*!DRAW_SIZE - 2 - (y1 + y2))/2) $
	    else $
        	row = fix((!DRAW_SIZE - y1 -1)/dsp.zoom)
        endif
        if(x1 ne x1m1) or (x2 ne x2m1) then begin
            plots,[x1m1,x1m1],[0,!DRAW_SIZE],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
	    if(pr.width gt 1) then $
                plots,[x2m1,x2m1],[0,!DRAW_SIZE],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
            plots,[x1,x1],[0,!DRAW_SIZE],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
	    if(pr.width gt 1) then $
                plots,[x2,x2],[0,!DRAW_SIZE],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
    	    if(pr.width gt 1) then $
		col=(x1+x2)/(2*dsp.zoom) $
	    else $
		col=x1/dsp.zoom
        endif
        if(x1 ne x1m1) or (x2 ne x2m1) or (y1 ne y1m1) or (y2 ne y2m1) then $
            widget_control,wd.error,SET_VALUE=strcompress(string("(",col,",",row,")"),/REMOVE_ALL)
  	x1m1 = x1
    	x2m1 = x2
        y1m1 = y1
    	y2m1 = y2
    endrep until !err eq 4
    plots,[x1m1,x1m1],[0,!DRAW_SIZE],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
    if(pr.width gt 1) then $
        plots,[x2m1,x2m1],[0,!DRAW_SIZE],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
    plots,[0,!DRAW_SIZE],[y1m1,y1m1],COLOR=!d.table_size-1,/DEV,/NOCLIP
    if(pr.height gt 1) then $
         plots,[0,!DRAW_SIZE],[y2m1,y2m1],COLOR=!d.table_size-1,/DEV,/NOCLIP
    device,SET_GRAPHICS=3,/cursor_crosshair	;Resume normal graphics.
    rect_plot,x1,x2+dsp.zoom-1,y1,y2+dsp.zoom-1,COLOR=pr.color
    if(pr.height gt 1) then $
	row = fix((!DRAW_SIZE - 1 - (y1 + y2)/2)/dsp.zoom) $
    else $
	row = fix((!DRAW_SIZE - y1 -1)/dsp.zoom)
    if(pr.width gt 1) then $
	col=(x1+x2)/(2*dsp.zoom) $
    else $
	col=x1/dsp.zoom
    x1 = col - pr.width/2
    x2 = col + pr.width/2
    if(x1 lt 0) then x1 = 0
    if(x2 gt dsp.xdim-1) then x2 = dsp.xdim-1
    y1 = col - pr.height/2
    y2 = col + pr.height/2
    if(y1 lt 0) then y1 = 0
    if(y1 gt dsp.ydim-1) then y2 = dsp.xdim-1

    handle_value,fi.hdr_handle(fi.n),hdr
    handle_value,fi.hdr_handle(hdr.mother),hd

    if(pr.time_type eq !THRESH_TPROF) then begin
        statimg = get_image(dsp.zdim*(dsp.frame-1) + dsp.plane-1,fi,stc)
	max = max(statimg(x1:x2,y1:y2))
	thresh = pr.time_thresh*max
	mask = statimg gt thresh
	mask(0:x1-1,*) = 0
        mask(x2+1:dsp.xdim-1,*) = 0
        mask(*,0:y1-1) = 0
	mask(*,y2+1:dsp.ydim-1) = 0
	indices = where(mask,nind)
        for i=0,nind-1 do begin
	    x = indices(i)/dsp.ydim
            y = indices(i) - x*dsp.ydim
	    xx = x*dsp.zoom - dsp.zoom/2
	    yy = !DRAW_SIZE-1 - y*dsp.zoom - dsp.zoom/2
    	    rect_plot,xx,xx+dsp.zoom-1,yy,yy+dsp.zoom-1,COLOR=pr.color
	end
    endif
    prof = fltarr(hd.tdim)
    for i=0,hd.tdim-1 do begin
        image = get_image(i*dsp.zdim+dsp.plane-1,fi,stc,FILNUM=hdr.mother+1)
        if(pr.time_type eq !THRESH_TPROF) then begin
	    image = image*mask
	    prof(i) = total(image(x1:x2,y1:y2))/(pr.width*pr.height)
	endif else begin
	    prof(i) = total(image(x1:x2,y1:y2))/(pr.width*pr.height)
	endelse
    end

; Smooth profile with boxcar filter.
if(pr.boxcar_width gt 0) then $
    profile = smooth(prof(1:n_elements(prof)-1),pr.boxcar_width) $
else $
    profile = prof(1:n_elements(prof)-1)

return,profile
end

