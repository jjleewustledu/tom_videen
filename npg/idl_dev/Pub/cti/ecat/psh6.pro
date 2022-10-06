;       $Revision:   1.1  $ $Date:   10/15/93 18:23:14  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	PSH6	
;
; PURPOSE:
; 	Print the ECAT6 subheaders to the screen.			
;
; CATEGORY:	
; 	CTI Matrix utility.		
;
; CALLING SEQUENCE:		
; 	psh6, sh
;
; INPUTS:	
; 	sh - subheader to be printed		
;
; OUTPUTS:	
; 	subheader printed to the screen.		
;
; HISTORY NOTES:		
;-

PRO psh6, sh
   
   on_error, 2
   IF (n_params() NE 1) THEN BEGIN
      set_ecat_error, 2, "PSH6"
      RETURN
   ENDIF

   ind = where(tag_names(sh) EQ "RECON", cnt)
   help, /struct, sh
   IF (cnt GT 0) THEN help, /struct, sh.recon

END
