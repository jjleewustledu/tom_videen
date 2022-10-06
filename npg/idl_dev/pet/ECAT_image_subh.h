; ECAT_image_subh.h

;	RH = 	{   Img_Recon_Info,			$
;			recon_start:intarr(3,2),	$
;			recon_duration:0L,		$
;			recon_scale:0.0,		$
;			filter_code:0,			$
;			filter_params:fltarr(6),	$
;			scan_matrix_num:0L,		$
;			norm_matrix_num:0L,		$
;			atten_cor_matrix_num:0L		$
;		}
;
;

	IH2 =	{   Img_Subheader_2,			$
			data_type:0,			$
			num_dimensions:0,		$
			dimension_1:0,			$
			dimension_2:0,			$
			x_origin:0.0,			$
			y_origin:0.0,			$
			recon_scale:0.0,		$
			quant_scale:0.0,		$
			image_min:0,			$
			image_max:0,			$
			pixel_size:0.0,			$
			slice_width:0.0,		$
			frame_duration:0L,		$
			frame_start_time:0L,		$
			slice_location:0,		$
			fill_1:bytarr(10),		$
			filter_code:0,			$
			fill_2:bytarr(12),		$
			image_rotation:0.0,		$
			plane_eff_corr_fctr:0.0,	$
			decay_corr_fctr:0.0,		$
			loss_corr_fctr:0.0,		$
			intrinsic_tilt:0.0,		$
			fill_3:bytarr(18),		$
			filter_params:fltarr(6),	$
			annotation:bytarr(40)		$
		}

