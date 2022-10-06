;+
; NAME:
;    NIhey_user
;
; PURPOSE:
;    Shows a small box with a message and a few buttons, e.g. "OK" and
;    "Cancel". Useful to ask confirmation for delicate operations.
;
; CATEGORY:
;    Widgets
;
; CALLING SEQUENCE:
;    answer = NIhey_user(prompt, choices)
;
; INPUTS:
;
;    PROMPT
;       Message displayed to the user
;
;    CHOICES
;       Array of strings, which will appear as the names of a set of buttons
;
;	
; KEYWORD PARAMETERS:
;
;    BEEP
;       When set, the computer will beep to draw the users attention
;
;    ASKSTRING
;       When set, the user will be asked to enter a string, which is returned
;       by NIhey_user.
;       The default value is the string in ASKSTRING.
;
;    ROW
;       When set, the buttons are organized in rows.
;
;    COLUMN
;       When set, the buttons are organized in columns.
;
; OUTPUTS:
;
;    ANSWER
;       Returns the name of the selected button
;
;
; COMMON BLOCKS:
;    NIhey_user_common, containing one string
;
; SIDE EFFECTS:
;    This is a model widget: all other widgets are temporarily switched off.
;
; RESTRICTIONS:
;    Model widget
;
; PROCEDURE:
;    Shows a box with buttons, waits for the user to click a button.
;    The widget then destroys itself and the routine returns the name
;    of the selected button. The name is passed from the event routine to the
;    main routine via the common block. A status variable cannot be used,
;    since the widget is destroyed by the event routine. Using the common
;    block has no adverse effets, since this is a model widget.
;
; EXAMPLE:
;    if NIhey_user('Do it?', ['yes', 'no'], /beep) eq 'yes' then begin
;       ....
;    endif
;
; MODIFICATION HISTORY:
; 	Written by:	Johan Nuyts, 15-May-1995
;-
;-----------------------------------------------------------------------

pro NIhey_user_event, ev

common NIhey_user_common, choice

widget_control, ev.id, get_uvalue= id
IF id EQ 0 THEN id = ev.id

widget_control, id, GET_VALUE = choice
widget_control, ev.handler, /DESTROY

end

;-----------------------------------------------------------------------

function NIhey_user, prompt, choices, BEEP = beep, ASKSTRING = askstring, $
                     row = row, column = column, title=title, $
                     group_leader = group_leader

common NIhey_user_common, choice

if n_elements(row) eq 0 then row = 0
if n_elements(column) eq 0 then column = 0
if row eq 0 and column eq 0 then row = 1
if n_elements(title) eq 0 then title = 'Question'

if n_elements(group_leader) gt 0 then begin
  widget_control, group_leader, tlb_get_offset = offset
  base_w = widget_base(/column, /modal, group_leader=group_leader, $
                       title=title, $
                       xoffset = offset(0), yoffset=offset(1))
endif else base_w = widget_base(/COLUMN, title = title)


for i = 0, n_elements(prompt)-1 do $
  row1_w = widget_label(base_w, VALUE = prompt(i))
row2_w = widget_base(base_w, row=row, column=column)

for i = 0, n_elements(choices) - 1 do begin
  dummy = widget_button(row2_w, VALUE = choices(i), uvalue=0)
endfor

if n_elements(askstring) gt 0 then begin
  text_w = widget_text(row2_w, VALUE = askstring, UVALUE = 0L, $
                      /EDIT, XSIZE = max([30, strlen(askstring) + 5]))
  ok_w = widget_button(row2_w, value='OK', uvalue=text_w)
endif

if keyword_set(beep) then print, string(7B)
widget_control, base_w, /REALIZE
if n_elements(askstring) gt 0 then $
  widget_control, text_w, /input_focus

if n_elements(group_leader) eq 0 $   
     then XManager, "NIhey_user", base_w, /MODAL $
     else XManager, "NIhey_user", base_w


return, choice(0)

end
