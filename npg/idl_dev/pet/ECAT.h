;+
; NAME:
; ECAT.h
;
; PURPOSE:
; Defines structures in ECAT sinogram and image files
; and error codes used in ECAT.pro routines.
;
; CATEGORY:
; IMAGE PROCESSING: CTI/PET
;
; CALLING SEQUENCE:
; @ECAT.h
;
; INPUTS:
; None.
;
; OUTPUTS:
; None.
;
; MODIFICATION HISTORY:
; Written by: Will Tribbey, CTI.
; Dec 01, 1993: Copy from Ming Xu (Nuclear Medicine)
; Dec 01, 1993: TOV added header.
;-

	FH = 	{   MH_File_Info,			$
			original_file_name:'',		$
			sw_version:0,			$
			data_type:0,			$
			system_type:0,			$
			file_type:0,			$
			node_id:'',			$
			nframes:0,			$
			nplanes:0,			$
			ngates:0,			$
			nbeds:0,			$
			scan_start:intarr(3,2),		$
			isotope_code:'',		$
			isotope_halflife:0.0,		$
			radiopharmaceutical:'',		$		
			facility_name:'',		$
			user_process_code:''		$
		}

	PI =	{   MH_Patient_Info,			$
			patient_info:strarr(11)		$
		}

	SI = 	{   MH_Scan_Info,			$
			gantry:fltarr(2),		$
			bed_elevation:0.0,		$
			rot_source_speed:0,		$
			wobble_speed:0,			$
			transm_source_type:0,		$
			axial_fov:0.0,			$
			transaxial_fov:0.0,		$
			transaxial_samp_mode:0,		$
			coin_samp_mode:0,		$
			axial_samp_mode:0,		$
			calibration_factor:0.0,		$
			calibration_units:0,		$
			compression_code:0,		$
			acquisition_type:0,		$
			bed_type:0,			$
			septa_type:0,			$
			init_bed_position:0.0,		$
			bed_offset:fltarr(15),		$
			plane_separation:0.0,		$
			lwr_sctr_thresh:0,		$
			true_thresh:intarr(2),		$
			collimator:0.0,			$
	 		acquisition_mode:0		$
		}			

	MH = 	{   Main_Header,			$
			file:{MH_File_Info},		$
			pat:strarr(11),			$
			scan:{MH_Scan_Info}		$
		}

	SH =	{   Scn_Subheader,			$
			data_type:0,			$
			dimension_1:0,			$
			dimension_2:0,			$
			smoothing:0,			$
			processing_code:0,		$
			sample_distance:0.0,		$
			isotope_halflife:0.0,		$
			frame_duration_sec:0,		$
			gate_duration:0L,		$
			r_wave_offset:0L,		$
			scale_factor:0.0,		$
			scan_min:0,			$
			scan_max:0,			$
			prompts:0L,			$
			delayed:0L,			$
			multiples:0L,			$
			net_trues:0L,			$
			cor_singles:fltarr(16),		$
			uncor_singles:fltarr(16),	$
			tot_avg_cor:0.0,		$
			tot_avg_uncor:0.0,		$
			total_coin_rate:0L,		$
			frame_start_time:0L,		$
			frame_duration:0L,		$
			loss_correction_fctr:0.0,	$
      	                phy_planes:lonarr(8) 		$
		}

	RH = 	{   Img_Recon_Info,			$
			recon_start:intarr(3,2),	$
			recon_duration:0L,		$
			recon_scale:0.0,		$
			filter_code:0,			$
			filter_params:fltarr(6),	$
			scan_matrix_num:0L,		$
			norm_matrix_num:0L,		$
			atten_cor_matrix_num:0L		$
		}


	IH =	{   Img_Subheader,			$
			data_type:0,			$
			num_dimensions:0,		$
			dimension_1:0,			$
			dimension_2:0,			$
			x_origin:0.0,			$
			y_origin:0.0,			$
			quant_scale:0.0,		$
			image_min:0,			$
			image_max:0,			$
			pixel_size:0.0,			$
			slice_width:0.0,		$
			frame_duration:0L,		$
			frame_start_time:0L,		$
			slice_location:0,		$
			image_rotation:0.0,		$
			plane_eff_corr_fctr:0.0,	$
			decay_corr_fctr:0.0,		$
			loss_corr_fctr:0.0,		$
			intrinsic_tilt:0.0,		$
			processing_code:0,		$
			quant_units:0,			$
			ecat_calibration_fctr:0.0,	$
			well_counter_cal_fctr:0.0,	$
			annotation:'',			$
			recon:{Img_Recon_Info}		$
		}

	NH = 	{   Nrm_Subheader,			$
			data_type:0,			$
			dimension_1:0,			$
			dimension_2:0,			$
			scale_factor:0.0,		$
			norm_hour:0,			$
			norm_minute:0,			$
			norm_second:0,			$
			norm_day:0,			$
			norm_month:0,			$
			norm_year:0,			$
			fov_source_width:0.0,		$
			ecat_calib_factor:0.0		$
		}

	AH = 	{   Atn_Subheader,			$
			data_type:0,			$
			attenuation_type:0,		$
			dimension_1:0,			$
			dimension_2:0,			$
			scale_factor:0.0,		$
			x_origin:0.0,			$
			y_origin:0.0,			$
			x_radius:0.0,			$
			y_radius:0.0,			$
			tilt_angle:0.0,			$
			attenuation_coeff:0.0,		$	
			sample_distance:0.0		$
		}

	PM = 	{   Pm_Subheader,		$
			type:0,			$
			num_rings:0,		$
			sect_per_ring:intarr(32), $
			ring_pos:fltarr(32),	$
			ring_angle:intarr(32),	$
			start_angle:0,		$
			long_axis_left:intarr(3),  $
			long_axis_right:intarr(3), $
			position_data:0,	$
			image_min:0,		$
			image_max:0,		$
			scale_factor:0.0,	$
			x_origin:0.0,		$
			y_origin:0.0,		$
			rotation:0.0,		$
			pixel_size:0.0,		$
			frame_duration:0L,	$
			frame_start_time:0L,	$
			proc_code:0,		$
			quant_units:0,		$
			annotation:''		$
		}

	delvar,AH,FH,IH,MH,NH,PI,RH,SH,SI,PM

	defsysv,'!ECAT',{ECAT_DATA,			$
				file_types:strarr(6),	$
				data_types:strarr(8),	$
				unit_types:strarr(10),	$
				acq_modes:strarr(6),	$
				mh:{Main_Header},	$
				sh_scn:{Scn_Subheader},	$
				sh_img:{Img_Subheader},	$
				sh_nrm:{Nrm_Subheader},	$
				sh_atn:{Atn_Subheader},	$
				sh_pm:{Pm_Subheader},	$
				blksize:512L,		$
				dirblk1:2L,		$
				OK:0,			$
				ERROR:-1,		$
				ECAT_ERROR:"",		$
				ERROR_CODES:strarr(17)  $
			}

	!ECAT.file_types = [ 	"GENERIC",		$
				"SCN",			$
				"IMG",			$
				"ATN",			$
				"NRM",			$
				"PM"			$
			   ]

	!ECAT.data_types = [	"GENERIC",		$
				"BYTE",			$
				"VAX_I2",		$
				"VAX_I4",		$
				"VAX_R4",		$
				"SUN_R4",		$
				"SUN_I2",		$
				"SUN_I4"		$
			   ]

	!ECAT.unit_types = [	"TOTAL_CTS",		$
				"UNKNOWN",		$
				"ECAT",			$
				"mCi/ml",		$
				"MRGlu",		$
				"MRGlu_umol",		$
				"MRGlu_mg",		$
				"nCi/ml",		$
				"WELL_COUNTS",		$
				"BQ"			$
			   ]

	!ECAT.acq_modes =  [	"ACQM_NORMAL",		$
				"ACQM_RODTRN",		$
				"ACQM_RODTRN_KEEP",	$
				"ACQM_DWIN_KEEP",	$
				"ACQM_DWIN",		$
				"ACQM_SIMULT"		$
			   ]

	!ECAT.error_codes(0)  = "ECAT ERROR"
	!ECAT.error_codes(1)  = "IO ERROR"
	!ECAT.error_codes(2)  = "UNMATCHED PARAMETERS"
	!ECAT.error_codes(3)  = "NUMBER OF MATRICES = 0"
	!ECAT.error_codes(4)  = "UNRECOGNIZED DATA TYPE"
	!ECAT.error_codes(5)  = "INSUFFICIENT DIRECTORY SPACE"
	!ECAT.error_codes(6)  = "FILE NOT OPEN"
	!ECAT.error_codes(7)  = "NO MAIN HEADER PARAMETER"
	!ECAT.error_codes(8)  = "NEW FILE WITH NO DATA"
	!ECAT.error_codes(9)  = "NUMBER OF MATRICES NOT EQUAL TO NUMBER OF SUBHEADERS"
	!ECAT.error_codes(10) = "DATA DIMENSIONS NOT 2D OR 3D"
	!ECAT.error_codes(11) = "NUMBER OF DATA SLICES NOT EQUAL TO NUMBER OF MATRICES"
	!ECAT.error_codes(12) = "FIRST MATRIX NOT FOUND"
	!ECAT.error_codes(13) = "OLD DATA DIMENSIONS NOT EQUAL TO NEW DATA DIMENSIONS"
	!ECAT.error_codes(14) = "MATRIX LIST DOES NOT MATCH DIRECTORY"
	!ECAT.error_codes(15) = "UNRECOGNIZED FILE TYPE"
	!ECAT.error_codes(16) = "NUMBER OF SCALE FACTORS NOT EQUAL TO NUMBER OF SUBHEADERS"
