;       $Revision:   1.1  $ $Date:   10/15/93 18:42:00  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:		
; 	WMH7
;
; PURPOSE:	
; 	Write the ecat7 main header.		
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:		
; 	result = wmh7 (unit,mh,FILE=file)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION wmh7, unit, mh, FILE=file

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 2) THEN BEGIN
      IF (keyword_set(FILE)) THEN $
         set_ecat_error, 2, "WMH7"  $
      ELSE            $
         set_ecat_error, 2, "WMH7", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF

   wb = 0L
   IF (keyword_set(FILE)) THEN openu, unit, file, /get_lun

                                ; set the file pointer 
   point_lun, unit, 0

   out = byte(mh.magic_number) 
   fill_arr, out, 14 & writeu, unit, out & incr, wb, 14

   out = byte(mh.original_file_name) 
   fill_arr, out, 32 & writeu, unit, out & incr, wb, 32

   out = [mh.sw_version, mh.system_type, mh.file_type] 
   IF (!LITTLE_ENDIAN) THEN byteorder, out
   writeu, unit, out & incr, wb, 6

   out = byte(mh.serial_number) 
   fill_arr, out, 10 & writeu, unit, out & incr, wb, 10

   out = [mh.scan_start_time] 
   IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
   writeu, unit, out & incr, wb, 4

   out = byte(mh.isotope_name) 
   fill_arr, out, 8 & writeu, unit, out & incr, wb, 8

   out = [mh.isotope_halflife] 
   IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
   writeu, unit, out & incr, wb, 4

   out = byte(mh.radiopharmaceutical) 
   fill_arr, out, 32 & writeu, unit, out & incr, wb, 32

   out = [mh.gantry.gantry_tilt, mh.gantry.gantry_rotation, $
          mh.gantry.bed_elevation, mh.gantry.intrinsic_tilt] 
   IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
   writeu, unit, out & incr, wb, 16

   out = [mh.gantry.wobble_speed, mh.gantry.transm_source_type]
   IF (!LITTLE_ENDIAN) THEN byteorder, out
   writeu, unit, out & incr, wb, 4

   out = [mh.gantry.distance_scanned, mh.gantry.transaxial_fov]
   IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
   writeu, unit, out & incr, wb, 8

   out = [mh.gantry.angular_compression, mh.gantry.coin_samp_mode, $
          mh.gantry.axial_samp_mode]
   IF (!LITTLE_ENDIAN) THEN byteorder, out
   writeu, unit, out & incr, wb, 6

   out = [mh.ecat_calibration_factor]
   IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
   writeu, unit, out & incr, wb, 4

   out = [mh.cal_units, mh.cal_units_label, $
          mh.compression_code]
   IF (!LITTLE_ENDIAN) THEN byteorder, out
   writeu, unit, out & incr, wb, 6

   out = bytarr(62)
   out(0) = byte(mh.study_name) 
   out(12) = byte(mh.patient_id)
   out(28) = byte(mh.patient_name)
   out(60) = byte(mh.patient_sex)
   out(61) = byte(mh.patient_dexterity)
   fill_arr, out, 62 & writeu, unit, out & incr, wb, 62

   out = [mh.patient_age, mh.patient_height, $
          mh.patient_weight]
   IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
   writeu, unit, out & incr, wb, 12

   out = [mh.patient_birth_date]
   IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
   writeu, unit, out & incr, wb, 4

   out = bytarr(96)
   out(0) = byte(mh.physician_name) 
   out(32) = byte(mh.operator_name)
   out(64) = byte(mh.study_desc)
   fill_arr, out, 96 & writeu, unit, out & incr, wb, 96

   out = [mh.acquisition_type, mh.patient_orientation]
   IF (!LITTLE_ENDIAN) THEN byteorder, out
   writeu, unit, out & incr, wb, 4

   out = byte(mh.facility_name) 
   fill_arr, out, 20 & writeu, unit, out & incr, wb, 20

   out = [mh.num_planes, mh.num_frames, $
          mh.num_gates, mh.num_bed_pos]
   IF (!LITTLE_ENDIAN) THEN byteorder, out
   writeu, unit, out & incr, wb, 8

   out = [mh.init_bed_position]
   IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
   writeu, unit, out & incr, wb, 4
   out = [mh.bed_position(*)]
   IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
   writeu, unit, out & incr, wb, 60
   out = [mh.plane_separation]
   IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
   writeu, unit, out & incr, wb, 4

   out = [mh.scan.lwr_sctr_thres, mh.scan.lwr_true_thres, $
          mh.scan.upr_true_thres]
   IF (!LITTLE_ENDIAN) THEN byteorder, out
   writeu, unit, out & incr, wb, 6

   out = byte(mh.scan.user_process_code) 
   fill_arr, out, 10 & writeu, unit, out & incr, wb, 10

   out = [mh.scan.acquisition_mode]
   IF (!LITTLE_ENDIAN) THEN byteorder, out
   writeu, unit, out & incr, wb, 2

   out = [mh.scan.bin_size, mh.scan.branching_fraction] 
   IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
   writeu, unit, out & incr, wb, 8

   out = [mh.scan.dose_start_time]
   IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
   writeu, unit, out & incr, wb, 4

   out = [mh.scan.dosage, mh.scan.well_counter_corr_factor] 
   IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
   writeu, unit, out & incr, wb, 8

   out = bytarr(46) & writeu, unit, out & incr, wb, 46
   
   IF (keyword_set(FILE)) THEN free_lun, unit
   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "WMH7", UNIT=unit
   RETURN, !ECAT.ERROR

END
