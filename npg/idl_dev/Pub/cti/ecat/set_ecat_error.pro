;       $Revision:   1.1  $ $Date:   10/15/93 18:40:38  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	SET_ECAT_ERROR	
;
; PURPOSE:	
; 	Basic error routine for all ECAT matrix manipulation functions.		
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:		
; 	set_ecat_error,code,module,FILE=file,UNIT=unit
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

PRO  set_ecat_error, code, module, FILE=file, UNIT=unit

   on_error, 2
   m = ""
   c = 0
   f = ""
   IF (n_params() GT 0) THEN c = code
   IF (n_params() GT 1) THEN m = module + ": "
   IF (keyword_set(file)) THEN f = " -> " + file
   IF (keyword_set(unit)) THEN BEGIN
      f = fstat(unit)
      file = f.name
      f = " -> " + file
      free_lun, unit
   ENDIF
   
   !ECAT.ECAT_ERROR = !MSG_PREFIX + m + !ECAT.ERROR_CODES(c) + f
   IF (NOT !quiet) THEN print, !ECAT.ECAT_ERROR
   RETURN
END
