MH6 = {Main_Header6,	$
       original_file_name:	'', 	$
       sw_version:		0, 	$
       data_type:		0, 	$
       system_type:		0, 	$
       file_type:		0, 	$
       node_id:			'', 	$
       nframes:			0, 	$
       nplanes:			0, 	$
       ngates:			0, 	$
       nbeds:			0, 	$
       scan_start:		intarr (3, 2), $
       isotope_code:		'', 	$
       isotope_halflife:	0.0,	$
       radiopharmaceutical:	'', 	$
       facility_name:		'', 	$
       user_process_code:	'' 	$
       patient_info:		strarr (11), $
       gantry:			fltarr (2), $
       bed_elevation:		0.0, 	$
       rot_source_speed:	0, 	$
       wobble_speed:		0, 	$
       transm_source_type:	0, 	$
       axial_fov:		0.0, 	$
       transaxial_fov:		0.0, 	$
       transaxial_samp_mode:	0, 	$
       coin_samp_mode:		0, 	$
       axial_samp_mode:		0, 	$
       calibration_factor:	0.0, 	$
       calibration_units:	0, 	$
       compression_code:	0, 	$
       acquisition_type:	0, 	$
       bed_type:		0, 	$
       septa_type:		0, 	$
       init_bed_position:	0.0, 	$
       bed_offset:		fltarr (15), $
       plane_separation:	0.0, 	$
       lwr_sctr_thresh:		0, 	$
       true_thresh:		intarr (2), $
       collimator:		0.0, 	$
       acquisition_mode:	0 	$
      }
IH6 = {Img_Subheader6,	$
       data_type:		0,	$
       num_dimensions:		0, 	$
       dimension_1:		0, 	$
       dimension_2:		0, 	$
       x_origin:		0.0, 	$
       y_origin:		0.0, 	$
       quant_scale:		0.0, 	$
       image_min:		0, 	$
       image_max:		0, 	$
       pixel_size:		0.0, 	$
       slice_width:		0.0, 	$
       frame_duration:		0L, 	$
       frame_start_time:	0L, 	$
       slice_location:		0, 	$
       image_rotation:		0.0, 	$
       plane_eff_corr_fctr:	0.0, 	$
       decay_corr_fctr:		0.0, 	$
       loss_corr_fctr:		0.0, 	$
       intrinsic_tilt:		0.0, 	$
       processing_code:		0, 	$
       quant_units:		0, 	$
       ecat_calibration_fctr:	0.0, 	$
       well_counter_cal_fctr:	0.0, 	$
       annotation:		'', 	$
       recon_start:		intarr (3, 2), $
       recon_duration:		0L,	$
       recon_scale:		0.0,	$
       filter_code:		0, 	$
       filter_params:		fltarr (6), $
       scan_matrix_num:		0L,	$
       norm_matrix_num:		0L, 	$
       atten_cor_matrix_num:	0L	$
      }

SH6 = {Scn_Subheader6,	$
       data_type:		0, 	$
       dimension_1:		0, 	$
       dimension_2:		0, 	$
       smoothing:		0, 	$
       processing_code:		0, 	$
       sample_distance:		0.0,	$
       isotope_halflife:	0.0, 	$
       frame_duration_sec:	0,	$
       gate_duration:		0L,	$
       r_wave_offset:		0L,	$
       scale_factor:		0.0,	$
       scan_min:		0, 	$
       scan_max:		0, 	$
       prompts:			0L, 	$
       delayed:			0L, 	$
       multiples:		0L, 	$
       net_trues:		0L, 	$
       cor_singles:		fltarr (16), $
       uncor_singles:		fltarr (16), $
       tot_avg_cor:		0.0, 	$
       tot_avg_uncor:		0.0, 	$
       total_coin_rate:		0L, 	$
       frame_start_time:	0L, 	$
       frame_duration:		0L, 	$
       loss_correction_fctr:	0.0, 	$
       phy_planes:		lonarr (8) $
      }

NH6 = {Nrm_Subheader6,	$

       data_type:		0, 	$
       dimension_1:		0, 	$
       dimension_2:		0, 	$
       scale_factor:		0.0, 	$
       norm_hour:		0, 	$
       norm_minute:		0, 	$
       norm_second:		0, 	$
       norm_day:		0, 	$
       norm_month:		0, 	$
       norm_year:		0, 	$
       fov_source_width:	0.0, 	$
       ecat_calib_factor:	0.0 	$
      }

AH6 = {Atn_Subheader6, 	$
       data_type:		0, 	$
       attenuation_type:	0, 	$
       dimension_1:		0, 	$
       dimension_2:		0, 	$
       scale_factor:		0.0, 	$
       x_origin:		0.0, 	$
       y_origin:		0.0, 	$
       x_radius:		0.0, 	$
       y_radius:		0.0, 	$
       tilt_angle:		0.0, 	$
       attenuation_coeff:	0.0, 	$
       sample_distance:		0.0 	$
      }


delvar, MH6, IH6, SH6, NH6, AH6

defsysv, '!ECAT6', { $
      ECAT_DATA, $
      file_types:	strarr (6), $
      data_types:	strarr (8), $
      unit_types:	strarr (10), $
      acq_modes:	strarr (6), $
      mh:		{Main_Header6}, $
      sh_scn:		{Scn_Subheader6}, $
      sh_img:		{Img_Subheader6}, $
      sh_nrm:		{Nrm_Subheader6}, $
      sh_atn:		{Atn_Subheader6}, $
      blksize:		512L, 	$
      dirblk1:		2L, 	$
      OK:		0, 	$
      ERROR:		-1, 	$
      ECAT_ERROR:	"", 	$
      ERROR_CODES:	strarr (18) $
      }

!ECAT6.file_types =["GENERIC", $
                   "SCN", 	$
                   "IMG", 	$
                   "ATN", 	$
                   "NRM", 	$
                   "PM" 	$
                  ]
!ECAT6.data_types =["GENERIC", $
                   "BYTE",   $
                   "VAX_I2", $
                   "VAX_I4", $
                   "VAX_R4", $
                   "SUN_R4", $
                   "SUN_I2", $
                   "SUN_I4"  $
                  ]
!ECAT6.unit_types =["TOTAL_CTS", $
                   "UNKNOWN", $
                   "ECAT", $
                   "mCi/ml", $
                   "MRGlu", $
                   "MRGlu_umol", $
                   "MRGlu_mg", $
                   "nCi/ml", $
                   "WELL_COUNTS", $
                   "BQ" $
                  ]
!ECAT6.acq_modes =["ACQM_NORMAL", $
                  "ACQM_RODTRN", $
                  "ACQM_RODTRN_KEEP", $
                  "ACQM_DWIN_KEEP", $
                  "ACQM_DWIN", $
                  "ACQM_SIMULT" $
                 ]
!ECAT6.error_codes (0) = "ECAT ERROR"
!ECAT6.error_codes (1) = "IO ERROR"
!ECAT6.error_codes (2) = "UNMATCHED PARAMETERS"
!ECAT6.error_codes (3) = "NUMBER OF MATRICES = 0"
!ECAT6.error_codes (4) = "UNRECOGNIZED DATA TYPE"
!ECAT6.error_codes (5) = "INSUFFICIENT DIRECTORY SPACE"
!ECAT6.error_codes (6) = "FILE NOT OPEN"
!ECAT6.error_codes (7) = "NO MAIN HEADER PARAMETER"
!ECAT6.error_codes (8) = "NEW FILE WITH NO DATA"
!ECAT6.error_codes (9) = "NUMBER OF MATRICES NOT EQUAL TO NUMBER OF SUBHEADERS"
!ECAT6.error_codes (10) = "DATA DIMENSIONS NOT 2D OR 3D"
!ECAT6.error_codes (11) = "NUMBER OF DATA SLICES NOT EQUAL TO NUMBER OF MATRICES"
!ECAT6.error_codes (12) = "FIRST MATRIX NOT FOUND"
!ECAT6.error_codes (13) = "OLD DATA DIMENSIONS NOT EQUAL TO NEW DATA DIMENSIONS"
!ECAT6.error_codes (14) = "MATRIX LIST DOES NOT MATCH DIRECTORY"
!ECAT6.error_codes (15) = "UNRECOGNIZED FILE TYPE"
!ECAT6.error_codes (16) = "NUMBER OF SCALE FACTORS NOT EQUAL TO NUMBER OF SUBHEADERS"
!ECAT6.error_codes (17) = "MAIN HEADER MUST BE UPDATED WHEN APPEND KEYWORD IS SET"
