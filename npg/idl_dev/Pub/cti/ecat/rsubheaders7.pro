;       $Revision:   1.1  $ $Date:   10/15/93 18:36:02  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:		
; 	RSUBHEADERS7
;
; PURPOSE:	
; 	Read the appropriate type subheaders in the open file unit
; 	at the given offsets into the parameter sh.		
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:
; 	result = rsubheaders7 (ftype,unit,offsets,sh,scales)		
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION rsubheaders7, ftype, unit, offsets, sh, scales

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 5) THEN BEGIN
      set_ecat_error, 2, "RSUBHEADERS7", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF

   CASE !ECAT.file_types(ftype) of

      "IMG" : $
         BEGIN
         stat = rsh_img7(unit, offsets, sh)
         IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
         scales = sh(0:n_elements(sh)-1).scale_factor
      END

      "SCN" : $
         BEGIN
         stat = rsh_scn7(unit, offsets, sh)
         IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
         scales = sh(0:n_elements(sh)-1).scale_factor
      END

      "NRM" : $
         BEGIN
         stat = rsh_nrm7(unit, offsets, sh)
         IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
         scales = sh(0:n_elements(sh)-1).scale_factor
      END

      "ATN" : $
         BEGIN
         stat = rsh_atn7(unit, offsets, sh)
         IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
         scales = sh(0:n_elements(sh)-1).scale_factor
      END
      "PM" : $
         BEGIN
         stat = rsh_pm7(unit, offsets, sh)
         IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
         scales = sh(0:n_elements(sh)-1).scale_factor
      END

      ELSE : BEGIN
         set_ecat_error, 15, "RSUBHEADERS7", UNIT=unit
         RETURN, !ECAT.ERROR
      END
   ENDCASE

   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "RSUBHEADERS7", UNIT=unit
   RETURN, !ECAT.ERROR

END
