/* @(#)show_header.c	1.2 7/27/92 */

#include "matrix.h"

main( argc, argv)
  int argc;
  char **argv;
{
	int matnum;
	char fname[256];
	MatrixFile *mptr;
	MatrixData *matrix;

	if (argc<2)
	  crash( "usage    : %s matspec\n", argv[0]);
	matspec( argv[1], fname, &matnum);
	mptr = matrix_open( fname, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	if (!mptr)
	  crash( "%s    : can't open file '%s'\n", argv[0], fname);
	if (matnum==0)
	{
	  show_main_header( mptr->mhptr);
	  exit(0);
	}
	matrix = matrix_read( mptr, matnum, MAT_SUB_HEADER);
	if (!matrix)
	  crash( "%s    : matrix not found\n", argv[0]);
	switch( mptr->mhptr->file_type)
	{
	  case 1    :
		show_scan_subheader( matrix->shptr);
		break;
	  case 2    :
		show_image_subheader( matrix->shptr);
		break;
	  case 3    :
		show_attn_subheader( matrix->shptr);
		break;
	  case 4    :
		show_norm_subheader( matrix->shptr);
		break;
	  default    :
		crash("%s    : unknown matrix file type (%d)\n", argv[0], mptr->mhptr->file_type);
	}
	exit(0);
}

show_main_header( mh)
  Main_header *mh;
{
	int ft, i, n;
	char *ftstr, tod[256];
	static char *ftypes[]={ "Scan", "Image", "Atten", "Norm" };

	printf("Matrix Main Header\n");
	printf("Original File Name          : %-20s\n", mh->original_file_name);
	printf("Software Version            : %d\n", mh->sw_version);
	printf("Data type                   : %d\n", mh->data_type);
	printf("System type                 : %d\n", mh->system_type);
	ft = mh->file_type;
	ftstr = "UNKNOWN";
	if (ft>0 && ft<=4) ftstr = ftypes[ft-1];
	printf("File type                   : %d (%s)\n", ft, ftstr);
	printf("Node Id                     : %-10s\n", mh->node_id);
	day_time( tod, mh->scan_start_day, mh->scan_start_month, 
	  mh->scan_start_year, mh->scan_start_hour, mh->scan_start_minute,
	  mh->scan_start_second);
	printf("Scan TOD                    : %s\n", tod);
	strncpy( tod, mh->isotope_code, 8);
	tod[8]='\0';
	printf("Isotope                     : %-8s\n", tod);
	printf("Isotope Half-life           : %0.5g sec.\n", mh->isotope_halflife);
	printf("Radiopharmaceutical         : %-32s\n", mh->radiopharmaceutical);
	printf("Gantry Tilt Angle           : %0.1f deg.\n", mh->gantry_tilt);
	printf("Gantry Rotation Angle       : %0.1f deg.\n", mh->gantry_rotation);
	printf("Bed Elevation               : %0.1f cm.\n", mh->bed_elevation);
	printf("Rotating Source Speed       : %d rpm\n", mh->rot_source_speed);
	printf("Wobble Speed                : %d rpm\n", mh->wobble_speed);
	printf("Transmission Source Type    : %d\n", mh->transm_source_type);
	printf("Axial FOV Width             : %0.2f cm.\n", mh->axial_fov);
	printf("Transaxial FOV Width        : %0.2f cm.\n", mh->transaxial_fov);
	printf("Transaxial Sampling Mode    : %d\n", mh->transaxial_samp_mode);
	printf("Coincidence Mode            : %d\n", mh->coin_samp_mode);
	printf("Axial Sampling Mode         : %d\n", mh->axial_samp_mode);
	printf("Calibration Factor          : %0.5e\n", mh->calibration_factor);
	printf("Calibration Units           : %d\n", mh->calibration_units);
	printf("Compression Code            : %d\n", mh->compression_code);
	printf("Study Name                  : %-12s\n", mh->study_name);
	printf("Patient ID                  : %-16s\n", mh->patient_id);
	printf("        Name                : %-32s\n", mh->patient_name);
	printf("        Sex                 : %c\n", mh->patient_sex);
	printf("        Age                 : %-10s\n", mh->patient_age);
	printf("        Height              : %-10s\n", mh->patient_height);
	printf("        Weight              : %-10s\n", mh->patient_weight);
	printf("        Dexterity           : %c\n", mh->patient_dexterity);
	printf("Physician Name              : %-32s\n", mh->physician_name);
	printf("Operator Name               : %-32s\n", mh->operator_name);
	printf("Study Description           : %-32s\n", mh->study_description);
	printf("Acquisition Type            : %d\n", mh->acquisition_type);
	printf("Bed Type                    : %d\n", mh->bed_type);
	printf("Septa Type                  : %d\n", mh->septa_type);
	printf("Facility Name               : %-20s\n", mh->facility_name);
	printf("Number of Planes            : %d\n", mh->num_planes);
	printf("Number of Frames            : %d\n", mh->num_frames);
	printf("Number of Gates             : %d\n", mh->num_gates);
	printf("Number of Bed Positions     : %d\n", mh->num_bed_pos);
	printf("Initial Bed Position        : %0.3f cm.\n", mh->init_bed_position);
	n=mh->num_bed_pos-1;
	if (n>16) n=0;
	for (i=0; i<n; i++)
	  printf("  Bed Position %2d        %0.3f cm.\n", i+2, mh->bed_offset[i]);
	printf("Plane Separation            : %0.4f cm.\n", mh->plane_separation);
	printf("Scatter Lower Threshold     : %d Kev\n", mh->lwr_sctr_thres);
	printf("True Lower Threshold        : %d Kev\n", mh->lwr_true_thres);
	printf("True Upper Threshold        : %d Kev\n", mh->upr_true_thres);
	printf("User Processing Code        : %-10s\n", mh->user_process_code);
        printf("Dosage (bequerels/cc)       : %0.5e\n", mh->dosage);
}

show_scan_subheader( sh)
  Scan_subheader *sh;
{
	int i,j;

	printf("Scan Matrix Sub-header\n");
	printf("Data type                   : %d\n", sh->data_type);
	printf("Dimension_1 (nprojs)        : %d\n", sh->dimension_1);
	printf("Dimension_2 (nviews)        : %d\n", sh->dimension_2);
	printf("Smoothing                   : %d\n", sh->smoothing);
	printf("Processing code             : %d\n", sh->processing_code);
	printf("Sample_distance             : %0.5f cm.\n", sh->sample_distance);
	printf("Isotope_halflife            : %0.4g sec.\n", sh->isotope_halflife);
	printf("Frame duration              : %d sec.\n", sh->frame_duration_sec);
	printf("Gate duration               : %d msec.\n", sh->gate_duration);
	printf("R-wave Offset               : %d msec.\n", sh->r_wave_offset);
	printf("Scale factor                : %0.5f\n", sh->scale_factor);
	printf("Scan_min                    : %d\n", sh->scan_min);
	printf("Scan_max                    : %d\n", sh->scan_max);
	printf("Prompt Events               : %d\n", sh->prompts);
	printf("Delayed Events              : %d\n", sh->delayed);
	printf("Multiple Events             : %d\n", sh->multiples);
	printf("Net True Events             : %d\n", sh->net_trues);
	for (i=0; i<16; i++)
	  printf("Avg Singles Bucket %2d       : %0.4E / %0.4E\n", i,
		sh->cor_singles[i], sh->uncor_singles[i]);
	printf("Average Singles (C/U)       : %0.4E / %0.4E\n", sh->tot_avg_cor,
						sh->tot_avg_uncor);
	printf("Total Coincidence Rate      : %d\n", sh->total_coin_rate);
	printf("Frame Start Time            : %d msec.\n", sh->frame_start_time);
	printf("Frame Duration              : %d msec.\n", sh->frame_duration);
	printf("Loss Correction Factor      : %0.5f\n", sh->loss_correction_fctr);
}

show_image_subheader( ih)
  Image_subheader *ih;
{
	char tod[256], *fcode;
	static char *fcodes[]={
		"None", "Ramp", "Butterworth", "Hanning", "Hamming", "Parzen",
		"Shepp", "Exponential"};
	int f;

	printf("Image Matrix Sub-header\n");
	printf("Data type                   : %d\n", ih->data_type);
	printf("Num_dimensions              : %d\n", ih->num_dimensions);
	printf("Dimension_1                 : %d\n", ih->dimension_1);
	printf("Dimension_2                 : %d\n", ih->dimension_2);
	printf("X_origin                    : %0.3f cm.\n", ih->x_origin);
	printf("Y_origin                    : %0.3f cm.\n", ih->y_origin);
	printf("Zoom (recon_scale)          : %0.3f\n", ih->recon_scale);
	printf("Quant_scale                 : %0.5e\n", ih->quant_scale);
	printf("Quant_units                 : %d\n", ih->quant_units);
	printf("Processing Code             : %d\n", ih->processing_code);
	printf("Image_min                   : %d\n", ih->image_min);
	printf("Image_max                   : %d\n", ih->image_max);
	printf("Pixel Size                  : %0.4f mm.\n", 10.0*ih->pixel_size);
	printf("Slice width                 : %0.4f cm.\n", ih->slice_width);
	printf("Frame Duration              : %d msec.\n", ih->frame_duration);
	printf("Frame Start Time            : %d msec.\n", ih->frame_start_time);
	printf("Slice Location              : %d\n", ih->slice_location);
	day_time( tod, ih->recon_start_day, ih->recon_start_month,
		ih->recon_start_year, ih->recon_start_hour,
		ih->recon_start_minute, ih->recon_start_sec);
	printf("Reconstruction TOD          : %s\n", tod);
	fcode = "UNKNOWN";
	f=ih->filter_code;
	if (f<0) f=-f;
	if (f<8) fcode=fcodes[f];
	printf("Filter Type                 : %d (%s)\n", ih->filter_code, fcode);
	printf("Filter Parameters           : %f,%f,%f,%f,%f,%f\n",
	  ih->filter_params[0], ih->filter_params[1], ih->filter_params[2],
	  ih->filter_params[3], ih->filter_params[4], ih->filter_params[5]);
	printf("Image Rotation Angle        : %0.2f degrees\n", ih->image_rotation);
	printf("Plane Efficiency Factor     : %0.5f\n", ih->plane_eff_corr_fctr);
	printf("Decay Correction Factor     : %0.5f\n", ih->decay_corr_fctr);
	printf("Loss Correction Factor      : %0.5f\n", ih->loss_corr_fctr);
	printf("ECAT Calibration Factor     : %0.5e\n", ih->ecat_calibration_fctr);
	printf("Well Counter Calib Fctr     : %0.5e\n", ih->well_counter_cal_fctr);
	printf("Annotation                  : %-40s\n", ih->annotation);
}

day_time( str, day, month, year, hour, minute, sec)
  char *str;
  int day, month, year, hour, minute, sec;
{
	static char *months="JanFebMarAprMayJunJulAugSepOctNovDec";
	char mstr[4];

	if (day<1) day=1;
	if (day>31) day=31;
	if (month<1) month=1;
	if (month>12) month=12;
	if (year<1900) year+=1900;
	if (year<1970) year=1970;
	if (year>9999) year=9999;
	if (hour<0) hour=0;
	if (hour>23) hour=23;
	if (minute<0) minute=0;
	if (minute>59) minute=59;
	if (sec<0) sec=0;
	if (sec>59) sec=59;
	strncpy( mstr, months+(month-1)*3, 3);
	sprintf( str, "%02d-%s-%4d %02d:%02d:%02d", day, mstr, year, 
		hour, minute, sec);
}
show_attn_subheader( ah)
  Attn_subheader *ah;
{
	printf("Attenuation Matrix Sub-header\n");
	printf("Data type                   : %d\n", ah->data_type);
	printf("Attenuation_type            : %d\n", ah->attenuation_type);
	printf("Dimension_1                 : %d\n", ah->dimension_1);
	printf("Dimension_2                 : %d\n", ah->dimension_2);
	printf("Scale Factor                : %0.5g\n", ah->scale_factor);
	printf("X origin                    : %0.4f cm.\n", ah->x_origin);
	printf("Y origin                    : %0.4f cm.\n", ah->y_origin);
	printf("X radius                    : %0.4f cm.\n", ah->x_radius);
	printf("Y radius                    : %0.4f cm.\n", ah->y_radius);
	printf("Tilt Angle                  : %0.2f deg.\n", ah->tilt_angle);
	printf("Attenuation Coefficient     : %0.3f 1/cm.\n", ah->attenuation_coeff);
	printf("Sample Distance             : %0.5f cm.\n", ah->sample_distance);
}

show_norm_subheader( nh)
  Norm_subheader *nh;
{
	char tod[256];

	printf("Normalization Matrix Sub-header\n");
	printf("Data type                   : %d\n", nh->data_type);
	printf("Dimension_1                 : %d\n", nh->dimension_1);
	printf("Dimension_2                 : %d\n", nh->dimension_2);
	printf("Scale Factor                : %0.5g\n", nh->scale_factor);
	day_time( tod, nh->norm_day, nh->norm_month, nh->norm_year,
		nh->norm_hour, nh->norm_minute, nh->norm_second);
	printf("Normalization TOD           : %s\n", tod);
	printf("FOV Source Width            : %0.4f cm.\n", nh->fov_source_width);
}
