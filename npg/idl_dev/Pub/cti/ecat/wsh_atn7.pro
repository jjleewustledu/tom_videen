;       $Revision:   1.1  $ $Date:   10/15/93 18:46:36  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	WSH_ATN7	
;
; PURPOSE:	
; 	Write the ECAT7 attenuation subheader		
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:		
; 	result = wsh_atn7 (unit,offset,sh)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION  wsh_atn7, unit, offset, sh

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 3) THEN BEGIN
      set_ecat_error, 2, "WSH_ATN7", UNIT =unit
      RETURN, !ECAT.ERROR
   ENDIF

   FOR i=0, n_elements(offset)-1 DO BEGIN
      wb = 0L
      point_lun, unit, offset(i) ; set the initial file pointer

      out = [sh(i).data_type, sh(i).num_dimensions, sh(i).atten_type, $
             sh(i).num_r_elements, sh(i).num_angles, $
             sh(i).num_z_elements, sh(i).ring_difference]
      IF (!LITTLE_ENDIAN) THEN byteorder, out
      writeu, unit, out & incr, wb, 14

      out = [sh(i).x_resolution, sh(i).y_resolution, $
             sh(i).z_resolution, sh(i).w_resolution, $
             sh(i).scale_factor, sh(i).x_offset, $
             sh(i).y_offset, sh(i).x_radius, $
             sh(i).y_radius, sh(i).tilt_angle, $
             sh(i).atten_coeff, sh(i).atten_min, $
             sh(i).atten_max, sh(i).skull_thickness]
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
      writeu, unit, out & incr, wb, 56

      out = [sh(i).num_add_coeff]
      IF (!LITTLE_ENDIAN) THEN byteorder, out
      writeu, unit, out & incr, wb, 2

      out = fltarr(9)
      out(0:7) = sh(i).add_atten_coeff
      out(8) = sh(i).edge_finding_threshold
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap

      writeu, unit, out & incr, wb, 36
      out = bytarr(404) & writeu, unit, out & incr, wb, 404
   ENDFOR

   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "WSH_ATN7", UNIT=unit
   RETURN, !ECAT.ERROR
END
