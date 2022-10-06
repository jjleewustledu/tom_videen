;
; Auto Save File For /export/home/fmri/DICOM_TOOLS/PROS/dicom_options.pro
;
;  Sat Jul 22 22:39:41 EDT 2000
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




PRO dicom_options_Event, Event

COMMON dicom_options,myreply
  WIDGET_CONTROL,Event.Id,GET_UVALUE=Ev
	statusid = widget_info(Event.top,/child)
	widget_control,statusid,get_uvalue = status
  CASE Ev OF 

  'BGROUP3': BEGIN
      IF Event.Select THEN Sel = 'On' ELSE Sel = 'Off'
      CASE Event.Value OF
      0: begin
		;Print,'Button Generate Dicom Turned ', Sel
		status.generate = event.select
	 end
      1: begin
		;Print,'Button Send Dicom Turned ', Sel
		status.send = event.select
	end
      2: begin
		;Print,'Button Display Fusion Turned ', Sel
		status.display = event.select
	end
      ELSE: Message,'Unknown button pressed'
      ENDCASE
	widget_control,status.statusid,set_uvalue = status
      END

  'BGROUP4': BEGIN
      IF Event.Select THEN Sel = 'On' ELSE Sel = 'Off'
      CASE Event.Value OF
      0: begin
		;Print,'Button Generate Dicom Turned ', Sel
		status.display = event.select
	 end
      ELSE: Message,'Unknown button pressed'
      ENDCASE
	widget_control,status.statusid,set_uvalue = status
      END


 'bhosts': BEGIN
	widget_control,event.id,get_value = value
	status.sendlist = value
	widget_control,status.statusid,set_uvalue = status
      END


   'hosts': BEGIN
     	status.host = status.host_list(event.index)
	status.select = event.index
	widget_control,statusid,set_uvalue = status
      END
  'BGROUP8': BEGIN
      CASE Event.Value OF
      0: begin
	;Print,'Button DONE Pressed'
	myreply = status
	err = 0
	if status.send and total(status.sendlist) eq 0 then begin
		err = 1
	 	badboy,'You must select a destination for send!'
	endif
	if not(err) then widget_control, /destroy, event.top
	end
      ELSE: Message,'Unknown button pressed'
      ENDCASE
      END
  ENDCASE
END


; DO NOT REMOVE THIS COMMENT: END MAIN13
; CODE MODIFICATIONS MADE BELOW THIS COMMENT WILL BE LOST.



PRO dicom_options, GROUP=Group,reply = reply,quiet = quiet

COMMON dicom_options,myreply

generate = 1
send = 1
display = 0
  IF N_ELEMENTS(Group) EQ 0 THEN GROUP=0
  junk   = { CW_PDMENU_S, flags:0, name:'' }
dlookup = create_dicom_lookup()

  MAIN13 = WIDGET_BASE(GROUP_LEADER=Group, $
      COLUMN=1, $
      MAP=1, $
      TITLE='DICOM OPTIONS', $
      UVALUE='MAIN13')

  BASE2 = WIDGET_BASE(MAIN13, $
      ROW=1, $
      MAP=1, $
      UVALUE='BASE2')

  Btns298 = [ $
    'Generate Dicom', $
    'Send Dicom','Display Fusion' ]
  BGROUP3 = CW_BGROUP( BASE2, Btns298, $
      COLUMN=1, $
      NONEXCLUSIVE=1, $
      UVALUE='BGROUP3',/frame)

;  Btns298 = [ $
;    'Display Fusion' ]
;  BGROUP4 = CW_BGROUP( MAIN13, Btns298, $
;      COLUMN=1, $
;      NONEXCLUSIVE=1, $
;      UVALUE='BGROUP4')


;  host_list = [ $
;    'PACS', $
;    'AW_NEURO1', $
;    'AW_NEURO2', $
;    'AW_NEURO3', $
;    'SMN']
host_list = dlookup.hosts
 hselect = 0
  ;hosts = WIDGET_DROPLIST( BASE2,VALUE=host_list, $
  ;    UVALUE='hosts')
;widget_control,hosts,set_droplist_select = hselect		
sendlist = intarr(n_elements(host_list))
sendlist(0) = 1
if n_elements(sendlist) gt 1 then sendlist(1) = 1
if n_elements(sendlist) gt 2 then sendlist(2) = 1
  bhosts = cw_bgroup(BASE2,host_list, $
      /nonexclusive,UVALUE='bhosts',label_top = 'Destination',/frame)
widget_control,bhosts,set_value = sendlist	

  Btns506 = [ $
    'DONE' ]
  BGROUP8 = CW_BGROUP( MAIN13, Btns506, $
      ROW=1, $
      UVALUE='BGROUP8')
	
	status = {statusid:base2,host_list:host_list,host:host_list(hselect),$
		send:send,generate:generate,lookup:dlookup,select:hselect,display:display,sendlist:sendlist}
	myreply = status
widget_control,status.statusid,set_uvalue = status
widget_control,bgroup3,set_value = [generate,send,display]
if not(keyword_set(quiet)) then begin
	print,string(7b)
 	 WIDGET_CONTROL, MAIN13, /REALIZE
	  XMANAGER, 'dicom_options', MAIN13,/modal
endif
  reply = myreply
END
