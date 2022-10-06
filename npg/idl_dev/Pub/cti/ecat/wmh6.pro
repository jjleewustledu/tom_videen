;       $Revision:   1.1  $ $Date:   10/15/93 18:41:50  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:		
; 	WMH6
;
; PURPOSE:
; 	Write the ECAT6 main header.			
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:		
; 	result = wmh6 (unit,mh,FILE=file)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-
 
FUNCTION wmh6, unit, mh, FILE=file

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 2) THEN BEGIN
      IF (keyword_set(FILE)) THEN $
         set_ecat_error, 2, "WMH6"  $
      ELSE            $
         set_ecat_error, 2, "WMH6", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF

   wb = 0L
   IF (keyword_set(FILE)) THEN openu, unit, file, /get_lun
   point_lun, unit, 0
   out = bytarr(28) & writeu, unit, out & incr, wb, 28
   out = byte(mh.file.original_file_name)
   fill_arr, out, 20 & writeu, unit, out & incr, wb, 20 
   out = [mh.file.sw_version, mh.file.data_type, $
          mh.file.system_type, mh.file.file_type]
   byteorder, out & writeu, unit, out & incr, wb, 8
   out = byte(mh.file.node_id)
   fill_arr, out, 10 & writeu, unit, out & incr, wb, 10
   out = mh.file.scan_start(*, 0)
   byteorder, out & writeu, unit, out & incr, wb, 6
   out = mh.file.scan_start(*, 1)
   byteorder, out & writeu, unit, out & incr, wb, 6
   out = byte(mh.file.isotope_code)
   fill_arr, out, 8 & writeu, unit, out & incr, wb, 8
   out = stovf(mh.file.isotope_halflife)
   writeu, unit, out & incr, wb, 4
   out = byte(mh.file.radiopharmaceutical)
   fill_arr, out, 32 & writeu, unit, out & incr, wb, 32
   out = [mh.scan.gantry, mh.scan.bed_elevation]
   out = stovf(out) & writeu, unit, out & incr, wb, 12
   out = [mh.scan.rot_source_speed, mh.scan.wobble_speed, $
          mh.scan.transm_source_type]
   byteorder, out & writeu, unit, out & incr, wb, 6
   out = [mh.scan.axial_fov, mh.scan.transaxial_fov]
   out = stovf(out) & writeu, unit, out & incr, wb, 8
   out = [mh.scan.transaxial_samp_mode, mh.scan.coin_samp_mode, $
          mh.scan.axial_samp_mode]
   byteorder, out & writeu, unit, out & incr, wb, 6
   out = stovf(mh.scan.calibration_factor)
   writeu, unit, out & incr, wb, 4
   out = [mh.scan.calibration_units, mh.scan.compression_code]
   byteorder, out & writeu, unit, out & incr, wb, 4
   out = bytarr(188)
   out(0)  = byte(mh.pat(0))
   out(12) = byte(mh.pat(1))
   out(28) = byte(mh.pat(2))
   out(60) = byte(mh.pat(3))
   out(61) = byte(mh.pat(4))
   out(71) = byte(mh.pat(5))
   out(81) = byte(mh.pat(6))
   out(91) = byte(mh.pat(7))
   out(92) = byte(mh.pat(8))
   out(124) = byte(mh.pat(9))
   out(156) = byte(mh.pat(10))
   fill_arr, out, 188 & writeu, unit, out & incr, wb, 188
   out = [mh.scan.acquisition_type, mh.scan.bed_type, $
          mh.scan.septa_type]
   byteorder, out & writeu, unit, out & incr, wb, 6
   out = byte(mh.file.facility_name)
   fill_arr, out, 20 & writeu, unit, out & incr, wb, 20
   out = [mh.file.nplanes, mh.file.nframes, $
          mh.file.ngates, mh.file.nbeds]
   byteorder, out & writeu, unit, out & incr, wb, 8
   out = stovf(mh.scan.init_bed_position)
   writeu, unit, out & incr, wb, 4
   out = stovf(mh.scan.bed_offset(*))
   writeu, unit, out & incr, wb, 60
   out = stovf(mh.scan.plane_separation)
   writeu, unit, out & incr, wb, 4
   out = [mh.scan.lwr_sctr_thresh, mh.scan.true_thresh]
   byteorder, out & writeu, unit, out & incr, wb, 6
   out = stovf(mh.scan.collimator)
   writeu, unit, out & incr, wb, 4
   out = byte(mh.file.user_process_code)
   fill_arr, out, 10 & writeu, unit, out & incr, wb, 10
   out = (mh.scan.acquisition_mode)
   byteorder, out & writeu, unit, out & incr, wb, 2
   out = bytarr(38) & writeu, unit, out & incr, wb, 38
   
   IF (keyword_set(FILE)) THEN free_lun, unit
   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "WMH6", UNIT=unit
   RETURN, !ECAT.ERROR

END
