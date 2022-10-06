;       $Revision:   1.1  $ $Date:   10/15/93 18:47:14  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	WSH_PM7	
;
; PURPOSE:	
; 	Write ecat7 polar map subheader.		
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:
; 	result = wsh_pm7 (unit,offset,sh)		
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION  wsh_pm7, unit, offset, sh

; written but not really tested yet.

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 3) THEN BEGIN
      set_ecat_error, 2, "WSH_PM7", UNIT =unit
      RETURN, !ECAT.ERROR
   ENDIF

   FOR i=0, n_elements(offset)-1 DO BEGIN
      wb = 0L
      point_lun, unit, offset(i)

      out = [sh(i).data_type, sh(i).type, sh(i).num_rings, sh(i).sect_per_ring]
      IF (!LITTLE_ENDIAN) THEN byteorder, out
      writeu, unit, out & incr, wb, 70

      out = [sh(i).ring_pos]
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
      writeu, unit, out & incr, wb, 128

      out = [sh(i).ring_angle, sh(i).start_angle, sh(i).long_axis_left, $
             sh(i).long_axis_right, sh(i).position_data, $
             sh(i).image_min, sh(i).image_max]
      IF (!LITTLE_ENDIAN) THEN byteorder, out
      writeu, unit, out & incr, wb, 84
      out = [sh(i).scale_factor, sh(i).pixel_size]
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
      writeu, unit, out & incr, wb, 8

      out = [sh(i).frame_duration, sh(i).frame_start_time]
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
      writeu, unit, out & incr, wb, 8

      out = [sh(i).proc_code, sh(i).quant_units]
      IF (!LITTLE_ENDIAN) THEN byteorder, out
      writeu, unit, out & incr, wb, 4

      out = sh(i).annotation
      writeu, unit, out & incr, wb, 40

      out = [sh(i).gate_duration, sh(i).r_wave_offset, $
             sh(i).num_acpt_beats]
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
      writeu, unit, out & incr, wb, 12

      out = sh(i).pm_protocol
      writeu, unit, out & incr, wb, 20

      out = sh(i).dbase_name
      writeu, unit, out & incr, wb, 30

      out = bytarr(110) & writeu, unit, out & incr, wb, 110

   ENDFOR

   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "WSH_PM7", UNIT=unit
   RETURN, !ECAT.ERROR
END
