PRO M7header_init, mh7, sh7
   
   mh7.magic_number = string(replicate(32b,14))
   mh7.original_file_name = string(replicate(32b,32))
   mh7.sw_version = 0
   mh7.system_type = 0
   mh7.file_type = 0
   mh7.serial_number = string(replicate(32b,12))
   mh7.scan_start_time = 0L
   mh7.isotope_code = string(replicate(32b,8))
   mh7.isotope_halflife = 0.0
   mh7.radiopharmaceutical = string(replicate(32b,32))
   mh7.gantry_tilt = 0.0
   mh7.gantry_rotation = 0.0
   mh7.bed_elevation = 0.0
   mh7.intrinsic_tilt = 0.0
   mh7.wobble_speed = 0
   mh7.transm_source_type = 0
   mh7.distance_scanned = 0.0
   mh7.transaxial_fov = 0.0
   mh7.angular_compression = 0
   mh7.coin_samp_mode = 0
   mh7.axial_samp_mode = 0
   mh7.calibration_factor = 0.0
   mh7.calibration_units = 0
   mh7.calibration_units_label = 0
   mh7.compression_code = 0
   mh7.study_type = string(replicate(32b,12))
   mh7.patient_id = string(replicate(32b,16))
   mh7.patient_name = string(replicate(32b,32))
   mh7.patient_sex = string(replicate(32b,1))
   mh7.patient_dexterity = string(replicate(32b,1))
   mh7.patient_age = 0.0
   mh7.patient_height = 0.0
   mh7.patient_weight = 0.0
   mh7.patient_birth_date = 0L
   mh7.physician_name = string(replicate(32b,32))
   mh7.operator_name = string(replicate(32b,32))
   mh7.study_description = string(replicate(32b,32))
   mh7.acquisition_type = 0
   mh7.patient_orientation = 0
   mh7.facility_name = string(replicate(32b,20))
   mh7.num_planes = 0
   mh7.num_frames = 0
   mh7.num_gates = 0
   mh7.num_bed_pos = 0
   mh7.init_bed_position = 0.0
   FOR i=0,14 DO mh7.bed_offset[i] = 0.0
   mh7.plane_separation = 0.0
   mh7.lwr_sctr_thres = 0
   mh7.lwr_true_thres = 0
   mh7.upr_true_thres = 0
   mh7.user_process_code = string(replicate(32b,10))
   mh7.acquisition_mode = 0
   mh7.bin_size = 0.0
   mh7.branching_fraction = 0.0
   mh7.dose_start_time = 0L
   mh7.dosage = 0.0
   mh7.well_counter_factor = 0.0
   mh7.data_units = string(replicate(32b,32))
   mh7.septa_state = 0
   FOR i=0,5 DO mh7.fill[i] = 0

   sh7.data_type = 0
   sh7.num_dimensions = 0
   sh7.x_dimension = 0
   sh7.y_dimension = 0
   sh7.z_dimension = 0
   sh7.x_offset = 0.0
   sh7.y_offset = 0.0
   sh7.z_offset = 0.0
   sh7.recon_zoom = 0.0
   sh7.scale_factor = 0.0
   sh7.image_min = 0
   sh7.image_max = 0
   sh7.x_pixel_size = 0.0
   sh7.y_pixel_size = 0.0
   sh7.z_pixel_size = 0.0
   sh7.frame_duration = 0L
   sh7.frame_start_time = 0L
   sh7.filter_code = 0
   sh7.x_resolution = 0.0
   sh7.y_resolution = 0.0
   sh7.z_resolution = 0.0
   sh7.num_r_elements = 0.0
   sh7.num_angles = 0.0
   sh7.z_rotation_angle = 0.0
   sh7.decay_corr_fctr = 0.0
   sh7.processing_code = 0L
   sh7.gate_duration = 0L
   sh7.r_wave_offset = 0L
   sh7.num_accepted_beats = 0L
   sh7.filter_cutoff_frequency = 0.0
   sh7.filter_resolution = 0.0
   sh7.filter_ramp_slope = 0.0
   sh7.filter_order = 0
   sh7.filter_scatter_fraction = 0.0
   sh7.filter_scatter_slope = 0.0
   sh7.annotation = string(replicate(32b,40))
   sh7.da_x_rotation_angle = 0.0
   sh7.da_y_rotation_angle = 0.0
   sh7.da_z_rotation_angle = 0.0
   sh7.da_x_translation = 0.0
   sh7.da_y_translation = 0.0
   sh7.da_z_translation = 0.0
   sh7.da_x_scale_factor = 0.0
   sh7.da_y_scale_factor = 0.0
   sh7.da_z_scale_factor = 0.0
   sh7.rfilter_cutoff = 0.0
   sh7.rfilter_resolution = 0.0
   sh7.rfilter_code = 0
   sh7.rfilter_order = 0
   sh7.zfilter_cutoff = 0.0
   sh7.zfilter_resolution = 0.0
   sh7.zfilter_code = 0
   sh7.zfilter_order = 0
   FOR i=0,144 DO sh7.reserved[i] = 0

END
