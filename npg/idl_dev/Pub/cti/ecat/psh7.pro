;       $Revision:   1.1  $ $Date:   10/15/93 18:23:20  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:		
; 	PSH7
;
; PURPOSE:	
; 	Print ECAT7 subheader.		
;
; CATEGORY:
; 	CTI Matrix utility.			
;
; CALLING SEQUENCE:	
; 	psh7, sh
; 
; INPUTS:
; 	sh - subheader to be printed			
;
; OUTPUTS:	
; 	subheader information printed to the screen.		
;
; HISTORY NOTES:		
;-

PRO  psh7, sh
   
   on_error, 2
   IF (n_params() NE 1) THEN BEGIN
      set_ecat_error, 2, "PSH7"
      RETURN
   ENDIF

   ind = where(tag_names(sh) EQ "RECON", cnt)
   help, /struct, sh
   IF (cnt GT 0) THEN help, /struct, sh.loc

END
