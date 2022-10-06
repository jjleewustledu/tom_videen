;       $Revision:   1.1  $ $Date:   10/15/93 18:46:44  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	WSH_IMG6	
;
; PURPOSE:
; 	Write the ecat6 image subheader.			
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:	
; 	result = wsh_img6 (unit,offset,sh)	
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION wsh_img6, unit, offset, sh

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 3) THEN BEGIN
      set_ecat_error, 2, "WSH_IMG6", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF
   
   FOR i=0, n_elements(offset)-1 DO BEGIN
      point_lun, unit, offset(i)
      wb = 0L
      out = bytarr(126) & writeu, unit, out & incr, wb, 126
      out = [sh(i).data_type, sh(i).num_dimensions]
      byteorder, out & writeu, unit, out & incr, wb, 4
      out = bytarr(2) & writeu, unit, out & incr, wb, 2
      out = [sh(i).dimension_1, sh(i).dimension_2]
      byteorder, out & writeu, unit, out & incr, wb, 4
      out = bytarr(24) & writeu, unit, out & incr, wb, 24
      out = [sh(i).x_origin, sh(i).y_origin, $
             sh(i).recon.recon_scale, sh(i).quant_scale]
      out = stovf(out) & writeu, unit, out & incr, wb, 16
      out = [sh(i).image_min, sh(i).image_max]
      byteorder, out & writeu, unit, out & incr, wb, 4
      out = bytarr(4) & writeu, unit, out & incr, wb, 4
      out = [sh(i).pixel_size, sh(i).slice_width]
      out = stovf(out) & writeu, unit, out & incr, wb, 8
      out = [sh(i).frame_duration, sh(i).frame_start_time]
      byteorder, out, /lswap & writeu, unit, out & incr, wb, 8
      out = [sh(i).slice_location, sh(i).recon.recon_start(*, 0)]
      byteorder, out & writeu, unit, out & incr, wb, 8
      out = sh(i).recon.recon_duration
      byteorder, out, /lswap & writeu, unit, out & incr, wb, 4
      out = bytarr(24) & writeu, unit, out & incr, wb, 24
      out = sh(i).recon.filter_code
      byteorder, out & writeu, unit, out & incr, wb, 2
      out = [sh(i).recon.scan_matrix_num, $
             sh(i).recon.norm_matrix_num, $
             sh(i).recon.atten_cor_matrix_num]
      byteorder, out, /lswap & writeu, unit, out & incr, wb, 12
      out = bytarr(46) & writeu, unit, out & incr, wb, 46
      out = [sh(i).image_rotation, sh(i).plane_eff_corr_fctr, $
             sh(i).decay_corr_fctr, sh(i).loss_corr_fctr]
      out = stovf(out) & writeu, unit, out & incr, wb, 16
      out = bytarr(64) & writeu, unit, out & incr, wb, 64
      out = sh(i).processing_code
      byteorder, out & writeu, unit, out & incr, wb, 2
      out = bytarr(2) & writeu, unit, out & incr, wb, 2
      out = [sh(i).quant_units, sh(i).recon.recon_start(*, 1)]
      byteorder, out & writeu, unit, out & incr, wb, 8
      out = [sh(i).ecat_calibration_fctr, sh(i).well_counter_cal_fctr]
      out = stovf(out) & writeu, unit, out & incr, wb, 8
      out = sh(i).recon.filter_params(*)
      out = stovf(out) & writeu, unit, out & incr, wb, 24
      out = byte(sh(i).annotation)
      fill_arr, out, 40 & writeu, unit, out & incr, wb, 40
      out = bytarr(52) & writeu, unit, out & incr, wb, 52

   ENDFOR 
   RETURN, !ECAT.OK
   io_error : set_ecat_error, 1, "WSH_IMG6", UNIT=unit
   RETURN, !ECAT.ERROR
END
