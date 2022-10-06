PRO M6header__define

   MH6 = {M6header,	$
          original_file_name:	'',	$
          sw_version:			0, 	$
          data_type:			0, 	$
          system_type:			0, 	$
          file_type:			0, 	$
          node_id:				'',	$
          scan_start_day:		0,	$
          scan_start_month:		0,	$
          scan_start_year:		0,	$
          scan_start_hour:		0,	$
          scan_start_minute:	0,	$
          scan_start_second:	0,	$
          isotope_code:			'',	$
          isotope_halflife:		0.0,	$
          dummy0:				0,	$
          radiopharmaceutical:	'',	$
          gantry_tilt:			0.0,	$
          gantry_rotation:		0.0,	$
          bed_elevation:		0.0, 	$
          rot_source_speed:		0, 	$
          wobble_speed:			0, 	$
          transm_source_type:	0, 	$
          dummy1:				0,	$
          axial_fov:			0.0, 	$
          transaxial_fov:		0.0, 	$
          transaxial_samp_mode:	0, 	$
          coin_samp_mode:		0, 	$
          axial_samp_mode:		0, 	$
          dummy2:				0,	$
          calibration_factor:	0.0, 	$
          calibration_units:	0, 	$
          compression_code:		0, 	$
          study_name:			'',	$
          patient_id:			'',	$
          patient_name:			'',	$
          patient_sex:			'',	$
          patient_age:			'',	$
          patient_height:		'',	$
          patient_weight:		'',	$
          patient_dexterity:	'',	$
          physician_name:		'',	$
          operator_name:		'',	$
          study_description:	'',	$
          acquisition_type:		0, 	$
          bed_type:				0, 	$
          septa_type:			0, 	$
          facility_name:		'',	$
          num_planes:			0, 	$
          num_frames:			0, 	$
          num_gates:			0, 	$
          num_bed_pos:			0, 	$
          dummy3:				0,	$
          init_bed_position:	0.0, 	$
          bed_offset:			fltarr (15), $
          plane_separation:		0.0, 	$
          lwr_sctr_thres:		0, 	$
          lwr_true_thres:		0, 	$
          upr_true_thres:		0, 	$
          dummy4:				0,	$
          collimator:			0.0, 	$
          user_process_code:	'',	$
          acquisition_mode:		0 	$
         }
   
   IH6 = {Subheader6_img,		$
          data_type:			0,	$
          num_dimensions:		0, 	$
          dimension_1:			0, 	$
          dimension_2:			0, 	$
          x_origin:				0.0, 	$
          y_origin:				0.0, 	$
          recon_scale:			0.0,	$
          quant_scale:			0.0, 	$
          image_min:			0, 	$
          image_max:			0, 	$
          pixel_size:			0.0, 	$
          slice_width:			0.0, 	$
          frame_duration:		0L, 	$
          frame_start_time:		0L, 	$
          slice_location:		0, 	$
          recon_start_hour:		0,	$
          recon_start_minute:	0,	$
          recon_start_sec:		0,	$
          recon_duration:		0L,	$
          filter_code:			0, 	$
          dummy1:				0,	$
          scan_matrix_num:		0L,	$
          norm_matrix_num:		0L, 	$
          atten_cor_matrix_num:	0L,	$
          image_rotation:		0.0, 	$
          plane_eff_corr_fctr:	0.0, 	$
          decay_corr_fctr:		0.0, 	$
          loss_corr_fctr:		0.0, 	$
          intrinsic_tilt:		0.0, 	$
          processing_code:		0, 	$
          quant_units:			0, 	$
          recon_start_day:		0,	$
          recon_start_month:	0,	$
          recon_start_year:		0,	$
          dummy2:				0,	$
          ecat_calibration_fctr:0.0, 	$
          well_counter_cal_fctr:0.0, 	$
          filter_params:		fltarr (6), $
          annotation:			'' $
         }

   SH6 = {Subheader6_scn,		$
          data_type:			0, 	$
          dimension_1:			0, 	$
          dimension_2:			0, 	$
          smoothing:			0, 	$
          processing_code:		0, 	$
          dummy0:				0,	$
          sample_distance:		0.0,	$
          isotope_halflife:		0.0, 	$
          frame_duration_sec:	0,	$
          dummy1:				0,	$
          gate_duration:		0L,	$
          r_wave_offset:		0L,	$
          scale_factor:			0.0,	$
          scan_min:				0, 	$
          scan_max:				0, 	$
          prompts:				0L, 	$
          delayed:				0L, 	$
          multiples:			0L, 	$
          net_trues:			0L, 	$
          cor_singles:			fltarr (16), $
          uncor_singles:		fltarr (16), $
          tot_avg_cor:			0.0, 	$
          tot_avg_uncor:		0.0, 	$
          total_coin_rate:		0L, 	$
          frame_start_time:		0L, 	$
          frame_duration:		0L, 	$
          loss_correction_fctr:	0.0, 	$
          phy_planes:			lonarr (8) 	$
         }

   NH6 = {Subheader6_nrm,		$
          data_type:			0, 	$
          dimension_1:			0, 	$
          dimension_2:			0, 	$
          scale_factor:			0.0, 	$
          norm_hour:			0, 	$
          norm_minute:			0, 	$
          norm_second:			0, 	$
          norm_day:				0, 	$
          norm_month:			0, 	$
          norm_year:			0, 	$
          fov_source_width:		0.0, 	$
          ecat_calib_factor:	0.0 	$
         }

   AH6 = {Subheader6_atn, 		$
          data_type:			0, 	$
          attenuation_type:		0, 	$
          dimension_1:			0, 	$
          dimension_2:			0, 	$
          scale_factor:			0.0, 	$
          x_origin:				0.0, 	$
          y_origin:				0.0, 	$
          x_radius:				0.0, 	$
          y_radius:				0.0, 	$
          tilt_angle:			0.0, 	$
          attenuation_coeff:	0.0, 	$
          sample_distance:		0.0 	$
         }

END
