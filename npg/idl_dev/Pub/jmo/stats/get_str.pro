;********************
pro get_str_event,ev
;********************

common getint_comm,num,strs,intbase,g1,g2,g3,g4,g5,g_ok

case ev.id of

    g1: strs(0) = ev.value

    g2: strs(1) = ev.value

    g3: strs(2) = ev.value

    g4: strs(3) = ev.value

    g5: strs(4) = ev.value

    g_ok: widget_control,ev.top,/DESTROY

    else: stat=widget_message('Invalid event in get_str_event',/ERROR)

endcase

return
end

;************************************************
function get_str,number,labels,values,WIDTH=width
;************************************************

common getint_comm

intbase = widget_base(/COLUMN)

num = number
strs = strarr(num)
if(keyword_set(width)) then xsize = width else xsize = 15
for i=0,num-1 do strs(i) = values(i)
g1 = 0
g2 = 0
g3 = 0
g4 = 0
g5 = 0
case number of

    1: begin
	g1 = cw_field(intbase,TITLE=labels(0),VALUE=values(0), $
					XSIZE=xsize,/ALL_EVENTS)
       end

    2: begin
	g1 = cw_field(intbase,TITLE=labels(0),VALUE=values(0), $
					XSIZE=xsize,/ALL_EVENTS)
	g2 = cw_field(intbase,TITLE=labels(1),VALUE=values(1), $
					XSIZE=xsize,/ALL_EVENTS)
       end

    3: begin
	g1 = cw_field(intbase,TITLE=labels(0),VALUE=values(0), $
					XSIZE=xsize,/ALL_EVENTS)
	g2 = cw_field(intbase,TITLE=labels(1),VALUE=values(1), $
					XSIZE=xsize,/ALL_EVENTS)
	g3 = cw_field(intbase,TITLE=labels(2),VALUE=values(2), $
					XSIZE=xsize,/ALL_EVENTS)
       end

    4: begin
	g1 = cw_field(intbase,TITLE=labels(0),VALUE=values(0), $
					XSIZE=xsize,/ALL_EVENTS)
	g2 = cw_field(intbase,TITLE=labels(1),VALUE=values(1), $
					XSIZE=xsize,/ALL_EVENTS)
	g3 = cw_field(intbase,TITLE=labels(2),VALUE=values(2), $
					XSIZE=xsize,/ALL_EVENTS)
	g4 = cw_field(intbase,TITLE=labels(3),VALUE=values(3), $
					XSIZE=xsize,/ALL_EVENTS)
       end

    5: begin
	g1 = cw_field(intbase,TITLE=labels(0),VALUE=values(0), $
					XSIZE=xsize,/ALL_EVENTS)
	g2 = cw_field(intbase,TITLE=labels(1),VALUE=values(1), $
					XSIZE=xsize,/ALL_EVENTS)
	g3 = cw_field(intbase,TITLE=labels(2),VALUE=values(2), $
					XSIZE=xsize,/ALL_EVENTS)
	g4 = cw_field(intbase,TITLE=labels(3),VALUE=values(3), $
					XSIZE=xsize,/ALL_EVENTS)
	g5 = cw_field(intbase,TITLE=labels(4),VALUE=values(4), $
					XSIZE=xsize,/ALL_EVENTS)
       end


    else: begin
	print,string("Too many values requested, get_str")
	return,0
       end
    endcase

g_ok = widget_button(intbase,value='OK')

widget_control,intbase,/REALIZE
xmanager,'get_str',intbase,/MODAL

return,strs
end

