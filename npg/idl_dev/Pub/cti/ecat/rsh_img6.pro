;       $Revision:   1.1  $ $Date:   10/15/93 18:35:16  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	RSH_IMG6	
;
; PURPOSE:
; 	Read the image subheader at the offset from the open file.			
;
; CATEGORY:
; 	CTI Matrix Utility.			
;
; CALLING SEQUENCE:		
; 	result =  rsh_img6 (unit,offset,sh)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION  rsh_img6, unit, offset, sh

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 3) THEN BEGIN
      set_ecat_error, 2, "RSH_IMG6", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF

   shead = !ECAT.sh_img6
   sh = [shead]
   FOR i=0, n_elements(offset)-1 DO BEGIN
      point_lun, unit, offset(i)+126L
      in=1 & readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
      shead.data_type = in
      readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
      shead.num_dimensions = in 
      readu, unit, in & in=intarr(2)
      readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
      shead.dimension_1 = in(0)
      shead.dimension_2 = in(1)
      in=intarr(12) & readu, unit, in 
      in=lonarr(4) & readu, unit, in & in=vtosf(in)
      shead.x_origin = in(0)
      shead.y_origin = in(1)
      shead.recon.recon_scale = in(2)
      shead.quant_scale = in(3)
      in=intarr(2) & readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
      shead.image_min = in(0)
      shead.image_max = in(1)
      in=intarr(2) & readu, unit, in & in=lonarr(2)
      readu, unit, in & in=vtosf(in)
      shead.pixel_size = in(0)
      shead.slice_width = in(1)
      in=lonarr(2) & readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in, /lswap
      shead.frame_duration = in(0)
      shead.frame_start_time = in(1)
      in=1 & readu, unit, in &IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
      shead.slice_location = in
      in=intarr(4) & readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
      shead.recon.recon_start(*, 0) = in(0:2)
      shead.recon.recon_duration = in(3)
      in=intarr(13) & readu, unit, in
      in=1 & readu, unit, in &IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
      shead.recon.filter_code = in
      in=lonarr(3) & readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in, /lswap
      shead.recon.scan_matrix_num = in(0)
      shead.recon.norm_matrix_num = in(1)
      shead.recon.atten_cor_matrix_num = in(2)
      in=intarr(23) & readu, unit, in
      in=lonarr(4) & readu, unit, in & in=vtosf(in)
      shead.image_rotation = in(0)
      shead.plane_eff_corr_fctr = in(1)
      shead.decay_corr_fctr = in(2)
      shead.loss_corr_fctr = in(3)
      in=intarr(32) & readu, unit, in
      in=1 & readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
      shead.processing_code = in
      readu, unit, in & in=intarr(4) & readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
      shead.quant_units = in(0)
      shead.recon.recon_start(*, 1) = in(1:3)
      in=lonarr(8) & readu, unit, in & in=vtosf(in)
      shead.ecat_calibration_fctr = in(0)
      shead.well_counter_cal_fctr = in(1)
      shead.recon.filter_params = in(2:7)
      in=bytarr(40) & readu, unit, in & in=string(in)
      shead.annotation = in 
      sh=[sh, shead]
   ENDFOR

   sh = sh(1:*)
   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "RSH_IMG6", UNIT=unit
   RETURN, !ECAT.ERROR
END
