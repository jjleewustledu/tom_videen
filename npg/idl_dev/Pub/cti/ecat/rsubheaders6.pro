;       $Revision:   1.2  $ $Date:   12/07/93 14:07:54  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:		
; 	RSUBHEADERS6
;
; PURPOSE:
; 	Read the appropriate type subheaders in the open file
; 	unit at the given offsets into the parameter sh.			
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:		
; 	result = rsubheaders6 (ftype,unit,offsets,sh,scale)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION rsubheaders6, ftype, unit, offsets, sh, scales

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 5) THEN BEGIN
      set_ecat_error, 2, "RSUBHEADERS6", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF

   CASE !ECAT.file_types(ftype) of

      "IMG" : $
         BEGIN
         stat = rsh_img6(unit, offsets, sh)
         IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
         IF (sh(0).ecat_calibration_fctr NE 0.0) THEN BEGIN
            scales = sh(0:n_elements(sh)-1).quant_scale * $
               sh(0:n_elements(sh)-1).ecat_calibration_fctr
         ENDIF ELSE BEGIN
            scales = sh(0:n_elements(sh)-1).quant_scale
         ENDELSE
      END

      "SCN" : $
         BEGIN
         stat = rsh_scn6(unit, offsets, sh)
         IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
         scales = sh(0:n_elements(sh)-1).scale_factor
      END

      "NRM" : $
         BEGIN
         stat = rsh_nrm6(unit, offsets, sh)
         IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
         scales = sh(0:n_elements(sh)-1).scale_factor
      END

      "ATN" : $
         BEGIN
         stat = rsh_atn6(unit, offsets, sh)
         IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
         scales = sh(0:n_elements(sh)-1).scale_factor
      END

      "PM" : $
         BEGIN
         stat = rsh_pm6(unit, offsets, sh)
         IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
         scales = sh(0:n_elements(sh)-1).scale_factor
      END

      ELSE : BEGIN
         set_ecat_error, 15, "RSUBHEADERS6", UNIT=unit
         RETURN, !ECAT.ERROR
      END
   ENDCASE

   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "RSUBHEADERS6", UNIT=unit
   RETURN, !ECAT.ERROR

END
