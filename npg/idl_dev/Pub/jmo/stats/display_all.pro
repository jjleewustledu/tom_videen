;**************************
pro display_all,dsp,fi,help
;**************************

handle_value,fi.hdr_handle(fi.n),hdr
img = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
disp = widget_base(title=fi.tail,/COLUMN)
zoom1 = 128/hdr.xdim
if(zoom1 lt 1) then zoom1 = 1
if(dsp.zoom < zoom1) then zoom1 = dsp.zoom
if(dsp.type eq !OVERLAY_ARRAY) then begin
    case dsp.zoom of
	1: zoom1 = .5
	2: zoom1 = 1.
	4: zoom1 = 2.
	else: zoom1 = .5
    endcase
endif
zoom1 = dsp.zoom
dx = hdr.xdim*zoom1
dy = hdr.ydim*zoom1
ncol = fix(1024/dx)
nrow = fix(hdr.zdim/ncol)
if(nrow*ncol lt hdr.zdim) then nrow = nrow + 1
xsize = ncol*dx
ysize = nrow*dy
if(xsize gt 1024 or ysize gt 512) then begin
    wimg = widget_draw(disp,XSIZE=1024,YSIZE=1024,RETAIN=2, $
	X_SCROLL_SIZE=xsize,Y_SCROLL_SIZE=850,/SCROLL, $
	KILL_NOTIFY=string('displayall_dead'))
endif else begin
    wimg = widget_draw(disp,xsize=ncol*dx+4,ysize=nrow*dy+4,RETAIN=2, $
	KILL_NOTIFY=string('displayall_dead'))
endelse
widget_control,disp,/REALIZE
widget_control,GET_VALUE=index,wimg
wset,index
dsp.cur_window = dsp.nwindows
dsp.nwindows = dsp.nwindows + 1
dsp.id(dsp.cur_window) = index
dsp.base(dsp.cur_window) = disp
dsp.draw(dsp.cur_window) = wimg
img = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
zoff = hdr.zdim*(dsp.frame-1)
for z=0,hdr.zdim-1 do $
    img(*,*,z) = get_image(z+zoff,fi,stc)
case dsp.scale of
    !GLOBAL: display_glob,img,min,max,ZOOM=zoom1
    !LOCAL: display,img,min,max,ZOOM=zoom1
    !FIXED: begin
	scale = !LEN_COLORTAB/float(dsp.fixed_max - dsp.fixed_min)
        display,img,min,max,ZOOM=ZOOM1,SCL=scale,FIXED_MIN=dsp.fixed_min
        end
    else: stat=widget_message('Illegal display mode.',/ERROR)
endcase
wset,dsp.image_index

return
end
