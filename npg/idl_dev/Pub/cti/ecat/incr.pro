;       $Revision:   1.1  $ $Date:   10/15/93 18:17:28  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
;	INCR	
;
; PURPOSE:	
;	Adds a value.		
;
; CATEGORY:
;	CTI Matrix utility			
;
; CALLING SEQUENCE:
;	incr, i, val		
;
; INPUTS:	
;	i - input value	
;	val - value to be added to i	
;
; OUTPUTS:
;	i - resulting value			
;
; HISTORY NOTES:		
;-

PRO	incr, i, val

   on_error, 2
   i = i + long(val)
   RETURN
END
