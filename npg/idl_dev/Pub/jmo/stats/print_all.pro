;*******************
pro print_all,dsp,fi
;*******************

set_plot,'ps'
plot_file = strcompress(string(getenv("HOME"),"/idl.ps"),/REMOVE_ALL)
device,YSIZE=18,XSIZE=22.5,FILENAME=fi.printfile,/LANDSCAPE

handle_value,fi.hdr_handle(fi.n),hdr
img = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
case hdr.xdim of 
    64: begin
        if(hdr.zdim eq 17) then begin
            ncol = 5 
            img = fltarr(320,256)
        endif else begin
            ncol = 4
            img = fltarr(256,256)
        endelse
    end
    48: begin
	case hdr.zdim of
	    17: begin
                ncol = 6 
                img = fltarr(288,192)
	    end
	    16: begin
                ncol = 8
                img = fltarr(384,128)
	    end
	    48: begin
                ncol = 8
                img = fltarr(384,384)
	    end
	    else: begin
        	stat=widget_message('Cannot format these image dimensions.',/ERROR)
        	return
	    end
	endcase
    end
    else: begin
        stat=widget_message('Cannot format these image dimensions.',/ERROR)
        return
    end
endcase
zoff = hdr.zdim*(dsp.frame-1)
for z=0,hdr.zdim-1 do begin
    row = hdr.ydim*(z/ncol)
    col = hdr.xdim*(z - ncol*(row/hdr.ydim))
    image = get_image(z+zoff,fi,stc)
    if(dsp.scale ne !FIXED) then begin
        min = float(min(image(*,*)))
        max = float(max(image(*,*)))
        scl = !LEN_COLORTAB/(max - min)
        img(col:col+hdr.xdim-1,row:row+hdr.ydim-1) = scl*(image(*,*) - min)
    endif else begin
        scl = !LEN_COLORTAB/float(dsp.fixed_max - dsp.fixed_min)
	min = dsp.fixed_min
        image = scl*(image(*,*) - min)
        mask = image le !LEN_COLORTAB
        image = image*(image le !LEN_COLORTAB) + (image gt !LEN_COLORTAB)*!LEN_COLORTAB
print,z,col,row
        img(col:col+hdr.xdim-1,row:row+hdr.ydim-1) = image
    endelse
end
tv,img,/ORDER

device,/CLOSE
cmd = string('lpr -',!PRINTER,' ',fi.printfile)
label = 'Revise printer command: '
revised_cmd = get_str(1,label,cmd,WIDTH=60)
spawn,revised_cmd
set_plot,'X'

return
end
