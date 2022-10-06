;
; Auto Save File For /export/home/fmri/DICOM_TOOLS/PROS/update_dicom_widget.pro
;
;  Fri Jul 21 14:40:30 EDT 2000
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




PRO update_dicom_Event, Event
Common sub_structure, sh

  WIDGET_CONTROL,Event.Id,GET_UVALUE=Ev

if strmid(Ev,0,5) eq 'FIELD' then begin
	sid = fix(strmid(ev,5,3))
	sh.(sid).value = event.value
endif
  CASE Ev OF 

  	'ACTION': BEGIN
      		Case event.value of
	 		 0: 	begin
				sh.action.value = 0
				widget_control, /destroy, event.top
	      			end
 			1: 	begin
				sh.action.value = 1
				widget_control, /destroy, event.top
	      			end
			else:widget_control, /destroy, event.top
		ENDCASE
     	END
  ELSE:x =1
  ENDCASE

END


; DO NOT REMOVE THIS COMMENT: END MAIN13
; CODE MODIFICATIONS MADE BELOW THIS COMMENT WILL BE LOST.



PRO update_dicom_widget, GROUP=Group,header = header

Common sub_structure, sh

  sh = header
  IF N_ELEMENTS(Group) EQ 0 THEN GROUP=0

  junk   = { CW_PDMENU_S, flags:0, name:'' }

tags = tag_names(header)
  MAIN13 = WIDGET_BASE(GROUP_LEADER=Group, $
      ROW=1, $
      MAP=1, $
      UVALUE='MAIN13',Title = 'DICOM UPDATE WIDGET')

  BASE2 = WIDGET_BASE(MAIN13, $
      COL=1, $
      MAP=1, $
      UVALUE='BASE2')

for i = 0,n_elements(tags) - 1 do begin
	next = header.(i) 
	if next.fixed ne 2 then begin
  		FIELD = CW_FIELD( BASE2,VALUE=next.value, $
		title = string(next.title,format = '(A20)'), $
 	     	ROW=1, $
 	     	STRING=1, $
		noedit = next.fixed,$
      		UVALUE='FIELD' + strtrim(i,2),/all_events)
		;if next.fixed eq 1 then widget_control,field,sensitive = 0
	endif
endfor

buttons = ['        CANCEL         ','          DONE         ']
quit = cw_bgroup(base2,buttons,$
		 uvalue = 'ACTION',/ROW)

  WIDGET_CONTROL, MAIN13, /REALIZE


 XMANAGER, 'update_dicom', MAIN13,/modal

sh.date.value = strtrim(sh.year.value,2) + strtrim(sh.month.value,2) + strtrim(sh.day.value,2)
header = sh

END
