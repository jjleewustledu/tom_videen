;       $Revision:   1.1  $ $Date:   10/15/93 18:46:24  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	WSH_ATN6	
;
; PURPOSE:	
; 	Write the ecat6 attenuation subheader.		
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:		
; 	result = wsh_atn6 (unit,offset,sh)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION  wsh_atn6, unit, offset, sh

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 3) THEN BEGIN
      set_ecat_error, 2, "WSH_ATN6", UNIT =unit
      RETURN, !ECAT.ERROR
   ENDIF

   FOR i=0, n_elements(offset)-1 DO BEGIN
      wb = 0L
      point_lun, unit, offset(i)
      out = bytarr(126) & writeu, unit, out & incr, wb, 126
      out = [sh(i).data_type, sh(i).attenuation_type]
      byteorder, out & writeu, unit, out & incr, wb, 4
      out = bytarr(2) & writeu, unit, out & incr, wb, 2
      out = [sh(i).dimension_1, sh(i).dimension_2]
      byteorder, out & writeu, unit, out & incr, wb, 4
      out = bytarr(46) & writeu, unit, out & incr, wb, 46
      out = [sh(i).scale_factor, sh(i).x_origin, $
             sh(i).y_origin, sh(i).x_radius, sh(i).y_radius, $
             sh(i).tilt_angle, sh(i).attenuation_coeff, $
             sh(i).sample_distance]
      out = stovf(out) & writeu, unit, out & incr, wb, 32
      out = bytarr(298) & writeu, unit, out & incr, wb, 298
   ENDFOR

   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "WSH_ATN6", UNIT=unit
   RETURN, !ECAT.ERROR
END

