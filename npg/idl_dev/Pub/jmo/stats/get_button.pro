;********************
pro get_but_event,ev
;********************

common getbut_comm,num,strs,intbase,buttons,index

index = ev.value
widget_control,ev.top,/DESTROY

return
end

;*************************************
function get_button,labels,TITLE=title
;*************************************

common getbut_comm

intbase = widget_base(/COLUMN)

if(keyword_set(TITLE)) then $
    stat = widget_label(intbase,VALUE=title)

index = -1
buttons = cw_bgroup(intbase,labels,EVENT_FUNCT=get_but_event,/EXCLUSIVE,/RETURN_INDEX)

widget_control,intbase,/REALIZE
xmanager,'get_but',intbase,/MODAL

return,index
end

