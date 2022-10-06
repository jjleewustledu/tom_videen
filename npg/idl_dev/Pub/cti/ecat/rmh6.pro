;       $Revision:   1.1  $ $Date:   10/15/93 18:28:36  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:		
; 	RMH6
;
; PURPOSE:
; 	Read the main header of the open file or from the file
; 	indicated by the keyword file.			
;
; CATEGORY:			
; 	CTI Matrix utility
;
; CALLING SEQUENCE:		
; 	status = rmh6 (unit,mh,FILE=file)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-
 
FUNCTION rmh6, unit, mh, FILE=file

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 2) THEN BEGIN
      IF (keyword_set(FILE)) THEN $
         set_ecat_error, 2, "RMH6"  $
      ELSE            $
         set_ecat_error, 2, "RMH6", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF

   IF (keyword_set(FILE)) THEN openr, unit, file, /get_lun
   point_lun, unit, 28
   mh = {Main_Header6}
   in=bytarr(20) & readu, unit, in & in=string(in)
   mh.file.original_file_name = in
   in=intarr(4) & readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
   mh.file.sw_version = in(0)
   mh.file.data_type = in(1)
   mh.file.system_type = in(2)
   mh.file.file_type = in(3)
   in=bytarr(10) & readu, unit, in & in=string(in)
   mh.file.node_id = in
   in=intarr(3) & readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
   mh.file.scan_start(*, 0) = in
   readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
   mh.file.scan_start(*, 1) = in
   in = bytarr(8) & readu, unit, in & in=string(in)
   mh.file.isotope_code = in
   in=1L & readu, unit, in & in=vtosf(in)
   mh.file.isotope_halflife = in
   in=bytarr(32) & readu, unit, in & in=string(in)
   mh.file.radiopharmaceutical = in
   in=lonarr(3) & readu, unit, in & in=vtosf(in)
   mh.scan.gantry = in(0:1)
   mh.scan.bed_elevation = in(2)
   in=intarr(3) & readu, unit, in &IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
   mh.scan.rot_source_speed = in(0)
   mh.scan.wobble_speed = in(1)
   mh.scan.transm_source_type = in(2)
   in=lonarr(2) & readu, unit, in & in=vtosf(in)
   mh.scan.axial_fov = in(0)
   mh.scan.transaxial_fov = in(1)
   in=intarr(3) & readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
   mh.scan.transaxial_samp_mode = in(0)
   mh.scan.coin_samp_mode = in(1)
   mh.scan.axial_samp_mode = in(2)
   in=1L & readu, unit, in & in=vtosf(in)
   mh.scan.calibration_factor = in
   in=intarr(2) & readu, unit, in &IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
   mh.scan.calibration_units = in(0)
   mh.scan.compression_code = in(0)
   in=bytarr(188) & readu, unit, in
   mh.pat(0) = string(in(0:11))
   mh.pat(1) = string(in(12:27))
   mh.pat(2) = string(in(28:59))
   mh.pat(3) = string(in(60))
   mh.pat(4) = string(in(61:70))
   mh.pat(5) = string(in(71:80))
   mh.pat(6) = string(in(81:90))
   mh.pat(7) = string(in(91))
   mh.pat(8) = string(in(92:123))
   mh.pat(9) = string(in(124:155))
   mh.pat(10) = string(in(156:187))
   in=intarr(3) & readu, unit, in &IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
   mh.scan.acquisition_type = in(0)
   mh.scan.bed_type = in(1)
   mh.scan.septa_type = in(2)
   in=bytarr(20) & readu, unit, in & in=string(in)
   mh.file.facility_name = in
   in=intarr(4) & readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
   mh.file.nplanes = in(0)
   mh.file.nframes = in(1)
   mh.file.ngates = in(2)
   mh.file.nbeds = in(3)
   in=lonarr(17) & readu, unit, in & in=vtosf(in)
   mh.scan.init_bed_position = in(0)
   mh.scan.bed_offset = in(1:15)
   mh.scan.plane_separation = in(16)
   in=intarr(3) & readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
   mh.scan.lwr_sctr_thresh = in(0)
   mh.scan.true_thresh = in(1:2)
   in=1L & readu, unit, in & in=vtosf(in)
   mh.scan.collimator = in
   in=bytarr(10) & readu, unit, in & in=string(in)
   mh.file.user_process_code = in
   in=1 & readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
   mh.scan.acquisition_mode = in

   IF (keyword_set(FILE)) THEN free_lun, unit
   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "RMH6", UNIT=unit
   RETURN, !ECAT.ERROR

END
