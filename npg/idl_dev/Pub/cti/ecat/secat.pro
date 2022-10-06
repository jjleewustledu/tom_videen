;       $Revision:   1.1  $ $Date:   10/15/93 18:36:08  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	SECAT	
;
; PURPOSE:
; 	Print !ECAT to the screen			
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:		
; 	secat,struct,ECAT=ecat
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

PRO secat, struct, ECAT=ecat

   IF ((n_params() EQ 0) OR keyword_set(ECAT)) THEN help, /struct, !ECAT
   IF (n_params() GT 0) THEN help, /struct, struct
END
