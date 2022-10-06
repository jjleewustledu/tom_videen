MH7 = {Main_header7,	$
	magic_number:	'',	$
	original_file_name:	'',	$
	sw_version:	0,	$
	system_type:	0,	$
	file_type:	0,	$
	serial_number:	'',	$
	scan_start_time:	0L,	$
	isotope_code:	'',	$
	isotope_halflife:	0.0,	$
	radiopharmaceutical:	'',	$
	gantry_tilt:	0.0,	$
	gantry_rotation:	0.0,	$
	bed_elevation:	0.0,	$
	intrinsic_tilt:	0.0,	$
	wobble_speed:	0,	$
	transm_source_type:	0,	$
	distance_scanned:	0.0,	$
	transaxial_fov:	0.0,	$
	angular_compression:	0,	$
	coin_samp_mode:	0,	$
	axial_samp_mode:	0,	$
	calibration_factor:	0.0,	$
	calibration_units:	0,	$
	calibration_units_label:	0,	$
	compression_code:	0,	$
	study_type:	'',	$
	patient_id:	'',	$
	patient_name:	'',	$
	patient_sex:	'',	$
	patient_dexterity:	'',	$
	patient_age:	0.0,	$
	patient_height:	0.0,	$
	patient_weight:	0.0,	$
	patient_birth_date:	0L,	$
	physician_name:	'',	$
	operator_name:	'',	$
       study_description:	'',	$
	acquisition_type:	0,	$
	patient_orientation:	0,	$
	facility_name:	'',	$
	num_planes:	0,	$
	num_frames:	0,	$
	num_gates:	0,	$
	num_bed_pos:	0,	$
	init_bed_position:	0.0,	$
	bed_offset:	0.0,	$
	plane_separation:	0.0,	$
	lwr_sctr_thres:	0,	$
	lwr_true_thres:	0,	$
	upr_true_thres:	0,	$
	user_process_code:	'',	$
	acquisition_mode:	0,	$
	bin_size:	0.0,	$
	branching_fraction:	0.0,	$
	dose_start_time:	0L,	$
	dosage:	0.0,	$
	well_counter_factor:	0.0,	$
	data_units:	'',	$
	septa_state:	0,	$
	fill:	intarr (6),	$
}

IH7 = {Image_subheader7,	$
	data_type:	0,	$
	num_dimensions:	0,	$
	x_dimension:	0,	$
	y_dimension:	0,	$
	z_dimension:	0,	$
	x_offset:	0.0,	$
	y_offset:	0.0,	$
	z_offset:	0.0,	$
	recon_zoom:	0.0,	$
	scale_factor:	0.0,	$
	image_min:	0,	$
	image_max:	0,	$
	x_pixel_size:	0.0,	$
	y_pixel_size:	0.0,	$
	z_pixel_size:	0.0,	$
	frame_duration:	0L,	$
	frame_start_time:	0L,	$
	filter_code:	0,	$
	x_resolution:	0.0,	$
	y_resolution:	0.0,	$
	z_resolution:	0.0,	$
	num_r_elements:	0.0,	$
	num_angles:	0.0,	$
	z_rotation_angle:	0.0,	$
	decay_corr_fctr:	0.0,	$
	processing_code:	0L,	$
	gate_duration:	0L,	$
	r_wave_offset:	0L,	$
	num_accepted_beats:	0L,	$
	filter_cutoff_frequency:	0.0,	$
	filter_resolution:	0.0,	$
	filter_ramp_slope:	0.0,	$
	filter_order:	0,	$
	filter_scatter_fraction:	0.0,	$
	filter_scatter_slope:	0.0,	$
	annotation:	'',	$
	da_x_rotation_angle:	0.0,	$
	da_y_rotation_angle:	0.0,	$
	da_z_rotation_angle:	0.0,	$
	da_x_translation:	0.0,	$
	da_y_translation:	0.0,	$
	da_z_translation:	0.0,	$
	da_x_scale_factor:	0.0,	$
	da_y_scale_factor:	0.0,	$
	da_z_scale_factor:	0.0,	$
	rfilter_cutoff:	0.0,	$
	rfilter_resolution:	0.0,	$
	rfilter_code:	0,	$
	rfilter_order:	0,	$
	zfilter_cutoff:	0.0,	$
	zfilter_resolution:	0.0,	$
	zfilter_code:	0,	$
	zfilter_order:	0,	$
	reserved:	intarr (145),	$
}

SH7 = {Scan_subheader7,	$
	data_type:	0,	$
	num_dimensions:	0,	$
	num_r_elements:	0,	$
	num_angles:	0,	$
	corrections_applied:	0,	$
	num_z_elements:	0,	$
	ring_difference:	0,	$
	x_resolution:	0.0,	$
	y_resolution:	0.0,	$
	z_resolution:	0.0,	$
	w_resolution:	0.0,	$
	gate_duration:	0L,	$
	r_wave_offset:	0L,	$
	num_accepted_beats:	0L,	$
	scale_factor:	0.0,	$
	scan_min:	0,	$
	scan_max:	0,	$
	prompts:	0L,	$
	delayed:	0L,	$
	multiples:	0L,	$
	net_trues:	0L,	$
	cor_singles:	fltarr (16),	$
	uncor_singles: fltarr (16),	$
	tot_avg_cor:	0.0,	$
	tot_avg_uncor:	0.0,	$
	total_coin_rate:	0L,	$
	frame_start_time:	0L,	$
	frame_duration:	0L,	$
	loss_correction_fctr:	0.0,	$
	phy_planes:	intarr (8),	$
	void *whole_header
}

S3DH7 = {Scan3D_subheader7,	$
	data_type:	0,	$
	num_dimensions:	0,	$
	num_r_elements:	0,	$
	num_angles:	0,	$
	corrections_applied:	0,	$
	num_z_elements:	intarr (64),	$
	ring_difference:	0,	$
	storage_order:	0,	$
	axial_compression:	0,	$
	x_resolution:	0.0,	$
	y_resolution:	0.0,	$
	z_resolution:	0.0,	$
	w_resolution:	0.0,	$
	gate_duration:	0L,	$
	r_wave_offset:	0L,	$
	num_accepted_beats:	0L,	$
	scale_factor:	0.0,	$
	scan_min:	0,	$
	scan_max:	0,	$
	prompts:	0L,	$
	delayed:	0L,	$
	multiples:	0L,	$
	net_trues:	0L,	$
	tot_avg_cor:	0.0,	$
	tot_avg_uncor:	0.0,	$
	total_coin_rate:	0L,	$
	frame_start_time:	0L,	$
	frame_duration:	0L,	$
	loss_correction_fctr:	0.0,	$
	uncor_singles:	fltarr (128),	$
	void *whole_header
}

NH7 = {Norm_subheader7,	$
	data_type:	0,	$
	num_dimensions:	0,	$
	num_r_elements:	0,	$
	num_angles:	0,	$
	num_z_elements:	0,	$
	ring_difference:	0,	$
	scale_factor:	0.0,	$
	norm_min:	0.0,	$
	norm_max:	0.0,	$
	fov_source_width:	0.0,	$
	norm_quality_factor:	0.0,	$
	norm_quality_factor_code:	0,	$
	storage_order:	0,	$
	span:	0,	$
	z_elements:	intarr (64),	$
	void *whole_header
}

N3DH7 = {Norm3D_subheader7,	$
	data_type:	0,	$
	num_r_elements:	0,	$
	num_transaxial_crystals:	0,	$
	num_crystal_rings:	0,	$
	crystals_per_ring:	0,	$
	num_geo_corr_planes:	0,	$
	uld:	0,	$
	lld:	0,	$
	scatter_energy:	0,	$
	norm_quality_factor_code:	0,	$
	norm_quality_factor:	0.0,	$
	ring_dtcor1:	fltarr (32),	$
	ring_dtcor2:	fltarr (32),	$
	crystal_dtcor:	fltarr (8),	$
	span:	0,	$
	max_ring_diff:	0,	$
	void *whole_header
}

AH7 = {Attn_subheader7,	$
	data_type:	0,	$
	num_dimensions:	0,	$
	attenuation_type:	0,	$
	num_r_elements:	0,	$
	num_angles:	0,	$
	num_z_elements:	0,	$
	ring_difference:	0,	$
	x_resolution:	0.0,	$
	y_resolution:	0.0,	$
	z_resolution:	0.0,	$
	w_resolution:	0.0,	$
	scale_factor:	0.0,	$
	x_offset:	0.0,	$
	y_offset:	0.0,	$
	x_radius:	0.0,	$
	y_radius:	0.0,	$
	tilt_angle:	0.0,	$
	attenuation_coeff:	0.0,	$
	attenuation_min:	0.0,	$
	attenuation_max:	0.0,	$
	skull_thickness:	0.0,	$
	num_additional_atten_coeff:	0,	$
	additional_atten_coeff:	fltarr (8),	$
	edge_finding_threshold:	0.0,	$
	storage_order:	0,	$
	span:	0,	$
	z_elements:	intarr (64),	$
	void *whole_header
}

typedef struct m7_header {
	struct file		*fsr
	Main_header7	*mh
	Image_subheader7 *sh
}	M7header
