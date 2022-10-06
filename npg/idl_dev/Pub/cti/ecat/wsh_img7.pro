;       $Revision:   1.1  $ $Date:   10/15/93 18:46:50  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	WSH_IMG7	
;
; PURPOSE:
; 	Write the ECAT7 image subheader.			
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:
; 	result = wsh_img7 (unit,offset,sh)		
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION wsh_img7, unit, offset, sh

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 3) THEN BEGIN
      set_ecat_error, 2, "WSH_IMG7", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF
   
   FOR i=0, n_elements(offset)-1 DO BEGIN
      point_lun, unit, offset(i) ; set the initial file pointer
      wb = 0L

      out = [sh(i).data_type, sh(i).num_dimensions, sh(i).x_dimension, $
             sh(i).y_dimension, sh(i).z_dimension]
      IF (!LITTLE_ENDIAN) THEN byteorder, out
      writeu, unit, out & incr, wb, 10

      out = [sh(i).z_offset, sh(i).x_offset, sh(i).y_offset, $
             sh(i).recon_zoom, sh(i).scale_factor]
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
      writeu, unit, out & incr, wb, 20

      out = [sh(i).image_min, sh(i).image_max]
      IF (!LITTLE_ENDIAN) THEN byteorder, out
      writeu, unit, out & incr, wb, 4

      out = [sh(i).x_pixel_size, sh(i).y_pixel_size, $
             sh(i).z_pixel_size]
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
      writeu, unit, out & incr, wb, 12

      out = [sh(i).frame_duration, sh(i).frame_start_time]
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
      writeu, unit, out & incr, wb, 8

      out = [sh(i).filter_code]
      IF (!LITTLE_ENDIAN) THEN byteorder, out
      writeu, unit, out & incr, wb, 2

      out = [sh(i).x_res, sh(i).y_res, sh(i).z_res, $
             sh(i).x_rot_angle, sh(i).y_rot_angle, $
             sh(i).z_rot_angle, sh(i).dec_corr_fctr]
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
      writeu, unit, out & incr, wb, 28

      out = [sh(i).corr_applied, sh(i).gate_duration, $
             sh(i).r_wave_offset, $
             sh(i).num_acpt_beats]
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
      writeu, unit, out & incr, wb, 16

      out = [sh(i).fil_cutoff_freq, sh(i).fil_dc_comp, $
             sh(i).fil_rs]
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
      writeu, unit, out & incr, wb, 12

      out = [sh(i).fil_order]
      IF (!LITTLE_ENDIAN) THEN byteorder, out
      writeu, unit, out & incr, wb, 2

      out = [sh(i).fil_sf, sh(i).fil_scat_slope]
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
      writeu, unit, out & incr, wb, 8

      out = byte(sh(i).annotation)
      fill_arr, out, 40 & writeu, unit, out & incr, wb, 40

      out = [sh(i).loc.da_x_rot_angle, sh(i).loc.da_y_rot_angle, $
             sh(i).loc.da_z_rot_angle, sh(i).loc.da_x_trans, $
             sh(i).loc.da_y_trans, sh(i).loc.da_z_trans, $
             sh(i).loc.da_x_scale_f, sh(i).loc.da_y_scale_f, $
             sh(i).loc.da_z_scale_f]
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
      writeu, unit, out & incr, wb, 36

      out = bytarr(316) & writeu, unit, out & incr, wb, 316

   ENDFOR 
   RETURN, !ECAT.OK
   io_error : set_ecat_error, 1, "WSH_IMG7", UNIT=unit
   RETURN, !ECAT.ERROR
END
