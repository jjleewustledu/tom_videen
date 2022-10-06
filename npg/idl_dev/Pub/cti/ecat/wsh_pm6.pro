;       $Revision:   1.1  $ $Date:   10/15/93 18:47:10  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:		
; 	WSH_PM6
;
; PURPOSE:	
; 	Write the ecat6 polar map subheader		
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:	
; 	result = wsh_pm6 (unit,offset,sh)			
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION  wsh_pm6, unit, offset, sh

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 3) THEN BEGIN
      set_ecat_error, 2, "WSH_PM6", UNIT =unit
      RETURN, !ECAT.ERROR
   ENDIF

   FOR i=0, n_elements(offset)-1 DO BEGIN
      wb = 0L
      point_lun, unit, offset(i)

      out = [sh(i).type, sh(i).num_rings, sh(i).sect_per_ring]
      byteorder, out & writeu, unit, out & incr, wb, 68

      out = [sh(i).ring_pos]
      out = stovf(out) & writeu, unit, out & incr, wb, 128

      out = [sh(i).ring_angle, sh(i).start_angle, sh(i).long_axis_left, $
             sh(i).long_axis_right, sh(i).position_data, $
             sh(i).image_min, sh(i).image_max]
      byteorder, out & writeu, unit, out & incr, wb, 84

      out = [sh(i).scale_factor, sh(i).x_origin, sh(i).y_origin, $
             sh(i).rotation, sh(i).pixel_size]
      print, out
      out = stovf(out) & writeu, unit, out & incr, wb, 20

      out = [sh(i).frame_duration, sh(i).frame_start_time]
      byteorder, out, /lswap & writeu, unit, out & incr, wb, 8

      out = [sh(i).proc_code, sh(i).quant_units]
      byteorder, out & writeu, unit, out & incr, wb, 4

      out = byte(sh(i).annotation)
      fill_arr, out, 40 & writeu, unit, out & incr, wb, 40

      out = bytarr(160) & writeu, unit, out & incr, wb, 160

   ENDFOR

   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "WSH_PM6", UNIT=unit
   RETURN, !ECAT.ERROR
END
