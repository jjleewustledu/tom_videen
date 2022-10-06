;       $Revision:   1.2  $ $Date:   11/22/93 13:59:18  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	RSH_ATN6	
;
; PURPOSE:	
; 	Read the attenuation file subheader at the offset from the 
; 	open file.		
;
; CATEGORY:
; 	CTI Matrix utility.			
;
; CALLING SEQUENCE:		
; 	status = rsh_atn6 (unit,offset,sh)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION  rsh_atn6, unit, offset, sh

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 3) THEN BEGIN
      set_ecat_error, 2, "RSH_ATN6", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF

   shead = !ECAT.sh_atn6
   sh = [shead]
   FOR i=0, n_elements(offset)-1 DO BEGIN
      point_lun, unit, offset(i)+126L
      in=intarr(5) & readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
      shead.data_type = in(0)
      shead.attenuation_type = in(1)
      shead.dimension_1 = in(3)
      shead.dimension_2 = in(4)
      in=intarr(23) & readu, unit, in
      in=lonarr(8) & readu, unit, in & in=vtosf(in)
      shead.scale_factor = in(0)
      shead.x_origin = in(1)
      shead.y_origin = in(2)
      shead.x_radius = in(3)
      shead.y_radius = in(4)
      shead.tilt_angle = in(5)
      shead.attenuation_coeff = in(6)
      shead.sample_distance = in(7)
      sh = [sh, shead]
   ENDFOR

   sh = sh(1:*)
   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "RSH_ATN6", UNIT=unit
   RETURN, !ECAT.ERROR
END
