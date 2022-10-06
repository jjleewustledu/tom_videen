;       $Revision:   1.1  $ $Date:   10/15/93 18:47:34  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	WSUBHEADER7	
;
; PURPOSE:	
; 	Write the appropriate ecat7 subheaders to the file at the
; 	specified offsets.		
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:	
; 	result = wsubheader7 (unit,ftype,offsets,sh,SCALE=scale)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION wsubheader7, unit, ftype, offsets, sh, SCALE=scale

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 4) THEN BEGIN
      set_ecat_error, 2, "WSUBHEADER7", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF
   sf = 0
   nscales = n_elements(sh)
   IF (keyword_set(SCALE)) THEN sf = 1
   IF (NOT sf) THEN scale = 1.0
   nsubs = n_elements(sh)
   IF (sf) THEN nscales = n_elements(scale)
   IF (nscales NE nsubs) THEN BEGIN
      set_ecat_error, 16, "WSUBHEADER7", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF

   CASE !ECAT.file_types(ftype) of

      "IMG" : $
         BEGIN
         IF (sf) THEN sh(*).scale_factor = scale
         stat = wsh_img7(unit, offsets, sh)
         IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
      END

      "SCN" : $
         BEGIN
         IF (sf) THEN sh(*).scale_factor = scale
         stat = wsh_scn7(unit, offsets, sh)
         IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
      END

      "NRM" : $
         BEGIN
         IF (sf) THEN sh(*).scale_factor = scale
         stat = wsh_nrm7(unit, offsets, sh)
         IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
      END

      "ATN" : $
         BEGIN
         IF (sf) THEN sh(*).scale_factor = scale
         stat = wsh_atn7(unit, offsets, sh)
         IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
      END

      ELSE : BEGIN
         set_ecat_error, 15, "WSUBHEADER7", UNIT=unit
         RETURN, !ECAT.ERROR
      END
   ENDCASE

   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "WSUBHEADERS7", UNIT=unit
   RETURN, !ECAT.ERROR

END

