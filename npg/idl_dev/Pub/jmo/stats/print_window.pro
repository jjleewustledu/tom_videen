;***************************
pro print_window,dsp,cstm,pr
;***************************

id = get_id(dsp,cstm,pr)
if(id lt 0) then begin
    stat=widget_message('*** No valid windows.  ***',/ERROR)
    return
endif

;Read bitmap from console.
wset,id
image = tvrd()
wset,dsp.image_index

xdim = n_elements(image(*,0))
ydim = n_elements(image(0,*))

; Dump to another window.
;base = widget_base(TITLE='tmp',/COLUMN)
;draw = widget_draw(base,xsize=xdim,ysize=ydim,RETAIN=2)
;widget_control,base,/REALIZE
;widget_control,GET_VALUE=id,draw
;wset,id
;tv,image

;Dump to postscript file and spawn print command.
set_plot,'ps'
psfile = strcompress(string(getenv("HOME"),"/idl.ps"),/REMOVE_ALL)
device,YSIZE=18,XSIZE=22.5,FILENAME=psfile,/LANDSCAPE
tv,image
device,/CLOSE
set_plot,'X'
cmd = string('lpr -',!PRINTER,' ',psfile)
label = 'Revise printer command: '
revised_cmd = get_str(1,label,cmd,WIDTH=60)
spawn,revised_cmd

return
end
