; Copyright (c) 1992-1993, Research Systems, Inc.  All rights reserved.
;	Unauthorized reproduction prohibited.


function XRegistered, NAME, NOSHOW = NOSHOW, NOMAP = NOMAP, GET_ID = GET_ID
;+
; NAME: 
;	XREGISTERED
;
; PURPOSE:
;	This function returns true if the widget named as its argument 
;	is currently registered with the XMANAGER as an exclusive widget, 
;	otherwise this routine returns false.
;
; CATEGORY:
;	Widgets.
;
; CALLING SEQUENCE:
;	Result = XREGISTERED(Name)
;
; INPUTS:
;	Name:	A string containing the name of the widget in question.
;
; KEYWORD PARAMETERS:
;	NOSHOW:	If the widget in question is registered, it is brought
;		to the front of all the other windows by default.  Set this
;		keyword to keep the widget from being brought to the front.
;       NOMAP:  Set this keyword to prevent remapping of the found
;               widget.
;       GET_ID: Set this keyword to have XREGISTERED return the widget
;               id.
;
; OUTPUTS:
;	If the named widget is registered, XREGISTERED returns the number
;	of instances of that name in the list maintained by XMANAGER.  
;	Otherwise, XREGISTERED returns 0.
;
; COMMON BLOCKS:
;	MANAGED
;
; SIDE EFFECTS:
;	Brings the widget to the front of the desktop if it finds one.
;
; RESTRICTIONS:
;	None.
;
; PROCEDURE:
;	Searches the list of exclusive widget names and if a match is found
;	with the one in question, the return value is modified.
;
; MODIFICATION HISTORY:
;	Written by Steve Richards, November, 1990
;	Jan, 92 - SMR	Fixed a bug where an invalid widget
;			was being referenced with 
;			WIDGET_CONTROL and the /SHOW keyword.
;	17 November 1993 - AB and SMR. Added ID validity checking to
;			fix a bug where already dead widgets were being
;			accessed.
;       19 Dec 96 - Added nomap and get_id keywords PMW
;-

  COMMON MANAGED, ids, names, $
		nummanaged, inuseflag, $
		backroutines, backids, backnumber, nbacks, validbacks, $
		blocksize, $
		cleanups, $
		outermodal

  if keyword_set(noshow)      then show = 0 else show = 1
  if keyword_set(nomap)       then map = 0  else map = 1
  if not(keyword_set(get_id)) then get_id = 0
  answer = 0					; Nothing matched yet

  IF(KEYWORD_SET(nummanaged)) THEN begin
    validids = WHERE(ids ne 0, answer)
    if (answer eq 0) then return, 0		; No valid ids
    registered = WHERE(names(validids) EQ NAME, answer)
    if (answer eq 0) then return, 0		; Name not registered
    registered = validids(registered)
  endif else return, 0				; Nothing being managed.

  ; Assertion: answer cannot be 0 at this point
  validids = WIDGET_INFO(ids(registered), /VALID)

  IF validids(0) EQ 1 then $
    WIDGET_CONTROL, ids(registered(0)), SHOW = show, map = map

  if get_id then return, ids(registered(0)) else return, answer

END


