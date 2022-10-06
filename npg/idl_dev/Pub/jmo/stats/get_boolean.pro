;***********************
pro get_boolean_event,ev
;***********************

common get_bool_comm,num,strs,intbase,get_true,get_false

strs = !TRUE
case ev.id of

    get_true: strs = !TRUE

    get_false: strs = !FALSE

    else: stat = widget_message(string("Invalid event in get_str_event"),/ERROR)

endcase

widget_control,ev.top,/DESTROY

return
end

;********************************
function get_boolean,title,labels
;********************************

common get_bool_comm

intbase = widget_base(/COLUMN)
wlabel = widget_base(intbase,/COLUMN)
buttons = widget_base(intbase,/ROW)

wroinum   = widget_label(wlabel,value=title)
get_true = widget_button(buttons,VALUE=labels(0))
get_false = widget_button(buttons,VALUE=labels(1))

widget_control,intbase,/REALIZE
xmanager,'get_boolean',intbase,/MODAL

return,strs
end

