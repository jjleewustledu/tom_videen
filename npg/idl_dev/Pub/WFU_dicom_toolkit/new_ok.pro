;
; Auto Save File For /export/home/fmri/./new_ok.pro
;
;  Thu Nov 19 00:51:52 EST 1998
;



; CODE MODIFICATIONS MADE ABOVE THIS COMMENT WILL BE LOST.
; DO NOT REMOVE THIS COMMENT: BEGIN HEADER


; $Id: template.pro,v 1.4 1995/07/12 20:50:46 beth Exp $
;
; Copyright (c) 1994, Research Systems, Inc.  All rights reserved.
;	Unauthorized reproduction prohibited.
; (Of course, if you don't work for RSI, remove these lines or
;  modify to suit.)
;+
; NAME:
;	ROUTINE_NAME
;
; PURPOSE:
;	Tell what your routine does here.  I like to start with the words:
;	"This function (or procedure) ..."
;	Try to use the active, present tense.
;
; CATEGORY:
;	Put a category (or categories) here.  For example:
;	Widgets.
;
; CALLING SEQUENCE:
;	Write the calling sequence here. Include only positional parameters
;	(i.e., NO KEYWORDS). For procedures, use the form:
;
;	ROUTINE_NAME, Parameter1, Parameter2, Foobar
;
;	Note that the routine name is ALL CAPS and arguments have Initial
;	Caps.  For functions, use the form:
; 
;	Result = FUNCTION_NAME(Parameter1, Parameter2, Foobar)
;
;	Always use the "Result = " part to begin. This makes it super-obvious
;	to the user that this routine is a function!
;
; INPUTS:
;	Parm1:	Describe the positional input parameters here. Note again
;		that positional parameters are shown with Initial Caps.
;
; OPTIONAL INPUTS:
;	Parm2:	Describe optional inputs here. If you don't have any, just
;		delete this section.
;	
; KEYWORD PARAMETERS:
;	KEY1:	Document keyword parameters like this. Note that the keyword
;		is shown in ALL CAPS!
;
;	KEY2:	Yet another keyword. Try to use the active, present tense
;		when describing your keywords.  For example, if this keyword
;		is just a set or unset flag, say something like:
;		"Set this keyword to use foobar subfloatation. The default
;		 is foobar superfloatation."
;
; OUTPUTS:
;	Describe any outputs here.  For example, "This function returns the
;	foobar superflimpt version of the input array."  This is where you
;	should also document the return value for functions.
;
; OPTIONAL OUTPUTS:
;	Describe optional outputs here.  If the routine doesn't have any, 
;	just delete this section.
;
; COMMON BLOCKS:
;	BLOCK1:	Describe any common blocks here. If there are no COMMON
;		blocks, just delete this entry.
;
; SIDE EFFECTS:
;	Describe "side effects" here.  There aren't any?  Well, just delete
;	this entry.
;
; RESTRICTIONS:
;	Describe any "restrictions" here.  Delete this section if there are
;	no important restrictions.
;
; PROCEDURE:
;	You can describe the foobar superfloatation method being used here.
;	You might not need this section for your routine.
;
; EXAMPLE:
;	Please provide a simple example here. An example from the PICKFILE
;	documentation is shown below.
;
;	Create a PICKFILE widget that lets users select only files with 
;	the extensions 'pro' and 'dat'.  Use the 'Select File to Read' title 
;	and store the name of the selected file in the variable F.  Enter:
;
;		F = PICKFILE(/READ, FILTER = ['pro', 'dat'])
;
; MODIFICATION HISTORY:
; 	Written by:	Your name here, Date.
;	July, 1994	Any additional mods get described here.  Remember to
;			change the stuff above if you add a new keyword or
;			something!
;-

PRO TEMPLATE

  PRINT, "This is an example header file for documenting IDL routines"

END


; DO NOT REMOVE THIS COMMENT: END HEADER
; CODE MODIFICATIONS MADE BELOW THIS COMMENT WILL BE LOST.


; CODE MODIFICATIONS MADE ABOVE THIS COMMENT WILL BE LOST.
; DO NOT REMOVE THIS COMMENT: BEGIN MAIN13




PRO wok_Event, Event
COMMON okgroup,myreply

statusid = widget_info(Event.top,/child)
widget_control,statusid,get_uvalue = status

  WIDGET_CONTROL,Event.Id,GET_UVALUE=Ev

  CASE Ev OF 

  'BGROUP8': BEGIN
     	myreply = event.value
	widget_control, /destroy, event.top
      END
  ENDCASE
END


; DO NOT REMOVE THIS COMMENT: END MAIN13
; CODE MODIFICATIONS MADE BELOW THIS COMMENT WILL BE LOST.



function new_ok, GROUP=Group,replyback = replyback,buttons = buttons, message = message,title = title,bell = bell

COMMON okgroup,myreply
  IF N_ELEMENTS(Group) EQ 0 THEN GROUP=0

  junk   = { CW_PDMENU_S, flags:0, name:'' }
if keyword_set(bell) then begin
	bel = string(7B)
	print,bel
endif

if not(keyword_set(title)) then  title = '?'
if not(keyword_set(message)) then message = 'What to do'
if not(keyword_set(buttons)) then buttons = ['NO','YES']

  MAIN13 = WIDGET_BASE(GROUP_LEADER=Group, $
      ROW=1, $
      MAP=1, $
      TITLE=title, $
      UVALUE='MAIN13')

  BASE2 = WIDGET_BASE(MAIN13, $
      COLUMN=1, $
      MAP=1, $
      UVALUE='BASE2')

  BASE3 = WIDGET_BASE(BASE2, $
      ROW=1, $
      MAP=1, $
      UVALUE='BASE3')

text = message

minsize = 20
miny = 1
szm = size(text)
if szm(0) eq 0 then begin
	if strlen(text) gt minsize then minsize = strlen(text)
endif

if (szm(0) ge 1) then begin
	for i = 0,szm(1) - 1 do begin
		if strlen(text(i)) gt minsize then minsize = strlen(text(i))
	endfor
endif

sz2 = size(szm)
if (sz2(1) gt 3) then miny = szm(1)


  LABEL4 = WIDGET_TEXT( BASE3, $
      UVALUE='LABEL4', $
      VALUE=message,$
 xsize = minsize,ysize = miny)



  BGROUP8 = CW_BGROUP( BASE2, buttons, $
      ROW=1, $
      UVALUE='BGROUP8')


  WIDGET_CONTROL, MAIN13, /REALIZE

status = {statusid:base2,reply:0}
widget_control,base2,set_uvalue = status
XMANAGER, 'wok', MAIN13,/modal

replyback = myreply
return,replyback
END
