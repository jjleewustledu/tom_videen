/* @(#)show_header.c	1.2 7/27/92 */
/*
 * Jun-1995 :Updated by Sibomana@topo.ucl.ac.be for ECAT 7.0 support
 */

#include "matrix.h"

main( argc, argv)
  int argc;
  char **argv;
{
	int matnum;
	char fname[256];
	MatrixFile *mptr;
	MatrixData *matrix;
	struct MatDir matdir;


	if (argc<2)
	  crash( "usage    : %s matspec\n", argv[0]);
	matspec( argv[1], fname, &matnum);
	mptr = matrix_open( fname, MAT_READ_ONLY, Short3dSinogram);
	if (!mptr)
	  crash( "%s    : can't open Sinogram file '%s'\n", argv[0], fname);
	if (matnum==0)
	{
	  show_main_header( mptr->mhptr);
	  exit(0);
	}
	if (matrix_find( mptr, matnum, &matdir))
	  crash( "%s    : matrix not found\n", argv[0]);
	matrix = matrix_read( mptr, matnum, MAT_SUB_HEADER);
	if (!matrix)
	  crash( "%s    : matrix not found\n", argv[0]);
	show_Scan3D_subheader(matrix->shptr, mptr->fptr, matdir.strtblk, matdir.endblk);
}

static char *ftypes[]= {
	"Scan", "Image", "Atten", "Norm", "PolarMap",
	"ByteVolume", "PetVolume", "ByteProjection", "PetProjection", "ByteImage",
	"Short3dSinogram", "Byte3dSinogram", "Norm3d"};

static char *dtypes[] = {
	"UnknownMatDataType", "ByteData", "VAX_Ix2", "VAX_Ix4",
    "VAX_Rx4", "IeeeFloat", "SunShort", "SunLong" };
show_main_header( mh)
  Main_header *mh;
{
	int ft, i;
	char *ftstr, tod[256];

	printf("Matrix Main Header\n");
	printf("Original File Name          : %-20s\n", mh->original_file_name);
	printf("Software Version            : %d\n", mh->sw_version);
/*		moved in individual matrices
	printf("Data type                   : %d\n", mh->data_type);
*/
	printf("System type                 : %d\n", mh->system_type);
	ft = mh->file_type;
	ftstr = "UNKNOWN";
	if (ft>0 && ft<=13) ftstr = ftypes[ft-1];
	printf("File type                   : %d (%s)\n", ft, ftstr);
	printf("Node Id                     : %-10s\n", mh->serial_number);
	printf("Scan TOD                    : %s\n",ctime(&mh->scan_start_time));
/*
	day_time( tod, mh->scan_start_day, mh->scan_start_month, 
	  mh->scan_start_year, mh->scan_start_hour, mh->scan_start_minute,
	  mh->scan_start_second);
*/
	strncpy( tod, mh->isotope_code, 8);
	tod[8]='\0';
	printf("Isotope                     : %-8s\n", tod);
	printf("Isotope Half-life           : %0.5g sec.\n", mh->isotope_halflife);
	printf("Radiopharmaceutical         : %-32s\n", mh->radiopharmaceutical);
	printf("Gantry Tilt Angle           : %0.1f deg.\n", mh->gantry_tilt);
	printf("Gantry Rotation Angle       : %0.1f deg.\n", mh->gantry_rotation);
	printf("Bed Elevation               : %0.1f cm\n", mh->bed_elevation);
/*  ELIMINATED
	printf("Rotating Source Speed       : %d rpm\n", mh->rot_source_speed);
*/
	printf("Wobble Speed                : %d rpm\n", mh->wobble_speed);
	printf("Transmission Source Type    : %d\n", mh->transm_source_type);
	printf("Axial FOV Width             : %0.2f cm\n", mh->distance_scanned);
	printf("Transaxial FOV Width        : %0.2f cm\n", mh->transaxial_fov);
/*  ELIMINATED
	printf("Transaxial Sampling Mode    : %d\n", mh->transaxial_samp_mode);
*/
	printf("Coincidence Mode            : %d\n", mh->coin_samp_mode);
	printf("Axial Sampling Mode         : %d\n", mh->axial_samp_mode);
	printf("Calibration Factor          : %0.5e\n", mh->calibration_factor);
	printf("Calibration Units           : %d\n", mh->calibration_units);
	printf("Compression Code            : %d\n", mh->compression_code);
	printf("Study Name                  : %-12s\n", mh->study_name);
	printf("Patient ID                  : %-16s\n", mh->patient_id);
	printf("        Name                : %-32s\n", mh->patient_name);
	printf("        Sex                 : %c\n", mh->patient_sex[0]);
	printf("        Age                 : %g\n", mh->patient_age);
	printf("        Height              : %g\n", mh->patient_height);
	printf("        Weight              : %g\n", mh->patient_weight);
	printf("        Dexterity           : %c\n", mh->patient_dexterity[0]);
	printf("Physician Name              : %-32s\n", mh->physician_name);
	printf("Operator Name               : %-32s\n", mh->operator_name);
	printf("Study Description           : %-32s\n", mh->study_description);
	printf("Acquisition Type            : %d\n", mh->acquisition_type);
/*	ELIMINATED
	printf("Bed Type                    : %d\n", mh->bed_type);
	printf("Septa Type                  : %d\n", mh->septa_type);
*/
	printf("Facility Name               : %-20s\n", mh->facility_name);
	printf("Number of Planes            : %d\n", mh->num_planes);
	printf("Number of Frames            : %d\n", mh->num_frames);
	printf("Number of Gates             : %d\n", mh->num_gates);
	printf("Number of Bed Positions     : %d\n", mh->num_bed_pos);
	printf("Initial Bed Position        : %.1f cm\n", mh->init_bed_position);
	for (i=0; i<mh->num_bed_pos; i++)
	  printf("  Offset %2d       %.1f cm\n", i+1, mh->bed_offset[i]);
	printf("Plane Separation            : %0.4f cm\n", mh->plane_separation);
	printf("Scatter Lower Threshold     : %d Kev\n", mh->lwr_sctr_thres);
	printf("True Lower Threshold        : %d Kev\n", mh->lwr_true_thres);
	printf("True Upper Threshold        : %d Kev\n", mh->upr_true_thres);
	printf("User Processing Code        : %-10s\n", mh->user_process_code);
}

show_scan_subheader( sh)
  Scan_subheader *sh;
{
	int i,j;

	printf("Scan Matrix Sub-header\n");
	printf("Data type                   : %d(%s)\n", sh->data_type);
	printf("Dimension_1 (ring elements) : %d\n", sh->num_r_elements);
	printf("Dimension_2 (angles)        : %d\n", sh->num_angles);
	printf("corrections_applied         : %d\n", sh->corrections_applied);
	printf("num_z_elements              : %d\n", sh->num_z_elements);
	printf("x resolution (sample distance)     : %g\n", sh->x_resolution);
	printf("y resolution                : %g\n", sh->y_resolution);
	printf("z resolution                : %g\n", sh->z_resolution);
	printf("w resolution                : %g\n", sh->w_resolution);
/*	ELIMINATED
	printf("Processing code             : %d\n", sh->processing_code);
*/
/*		moved to main header
	printf("Isotope_halflife            : %0.4g sec.\n", sh->isotope_halflife);
*/
/*	ELIMINATED
	printf("Frame duration              : %d sec.\n", sh->frame_duration_sec);
*/
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

show_Scan3D_subheader( sh, fp, strtblk, endblk)
  FILE *fp;
  int strtblk, endblk;
  Scan3D_subheader *sh;
{
	int i,j, blk;
	short *p, *buf = NULL, *tmp = NULL;
	short v, m, M;

	printf("Scan Matrix Sub-header\n");
	printf("Data type                   : %d\n", sh->data_type);
	printf("Num_dimensions              : %d\n", sh->num_dimensions);
	printf("Dimension_1(num_r_elements) : %d\n", sh->num_r_elements);
	printf("Dimension_2 (num_angles)    : %d\n", sh->num_angles);
	printf("corrections_applied         : %d\n", sh->corrections_applied);
	printf("num_z_elements              : %d", sh->num_z_elements[0]);
	for (i=1; sh->num_z_elements[i] != 0; i++)
		printf ("+%d", sh->num_z_elements[i]);
	printf("\n");
	printf("ring difference             : %d\n", sh->ring_difference);
	printf("storage order               : %d\n", sh->storage_order);
	printf("axial compression           : %d\n", sh->axial_compression);
	printf("x resolution (sample distance)     : %g\n", sh->x_resolution);
	printf("v resolution                : %g\n", sh->v_resolution);
	printf("z resolution                : %g\n", sh->z_resolution);
	printf("w resolution                : %g\n", sh->w_resolution);
	printf("Gate duration               : %d msec.\n", sh->gate_duration);
	printf("R-wave Offset               : %d msec.\n", sh->r_wave_offset);
	printf("Scale factor                : %0.5f\n", sh->scale_factor);
	if (sh->scan_max <= 0) {
		fprintf(stderr, "recomputing  extrema, this will take a while ...");
		blk = strtblk+2;
		if (fseek(fp, (blk-1)*MatBLKSIZE, 0) == EOF) crash("fseek error\n");
		buf = (short*)malloc(MatBLKSIZE); 
		if (ntohs(1) != 1) tmp = (short*)malloc(MatBLKSIZE);
		M = 0; m = 1;
		for (; blk <= endblk; blk++) {
			if (fread(buf,1,MatBLKSIZE, fp) != MatBLKSIZE)
				crash("fread error\n");
			if (tmp) {
				swab(buf,tmp,MatBLKSIZE); p = tmp;
			} else p = buf;
			for (i=0; i<MatBLKSIZE/2; i++, p++) {
				v = *p;
				if (v>M) M = v;
				else if (v<m) m = v;
			}
		}
		sh->scan_min = m;
		sh->scan_max = M;
		fprintf(stderr, "done\n");
	}
	printf("Scan_min                    : %d\n", sh->scan_min);
	printf("Scan_max                    : %d\n", sh->scan_max);
	printf("Prompt Events               : %d\n", sh->prompts);
	printf("Delayed Events              : %d\n", sh->delayed);
	printf("Multiple Events             : %d\n", sh->multiples);
	printf("Net True Events             : %d\n", sh->net_trues);
	printf("Average Singles (C/U)       : %0.4E / %0.4E\n", sh->tot_avg_cor,
						sh->tot_avg_uncor);
	printf("Total Coincidence Rate      : %d\n", sh->total_coin_rate);
	printf("Frame Start Time            : %d msec.\n", sh->frame_start_time);
	printf("Frame Duration              : %d msec.\n", sh->frame_duration);
	printf("Loss Correction Factor      : %0.5f\n", sh->loss_correction_fctr);
 	printf("Uncorrected Singles        :\n");
	for (i=0; i<8; i++) 	{
		for (j=0; j<8; j++)
 			printf("\t%g",sh->uncor_singles[i*8+j]);
		printf("\n");
	}
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
	int i=0;
	printf("Attenuation Matrix Sub-header\n");
	printf("Data type                   : %d(%s)\n",
		ah->data_type, dtypes[ah->data_type]);
	printf("Attenuation_type            : %d\n", ah->attenuation_type);
	printf("Dimension_1                 : %d\n", ah->num_r_elements);
	printf("Dimension_2                 : %d\n", ah->num_angles);
	printf("ring_difference             : %d\n",ah->ring_difference);
	printf("span                        : %d\n",ah->span);
	printf("z_elements                  : %d", ah->z_elements[0]);
	for (i=1; ah->z_elements[i] != 0; i++)
		printf ("+%d", ah->z_elements[i]);
	printf("\n");
	printf("Scale Factor                : %0.5g\n", ah->scale_factor);
	printf("x resolution                : %g\n", ah->x_resolution);
	printf("y resolution                : %g\n", ah->y_resolution);
	printf("z resolution                : %g\n", ah->z_resolution);
	printf("w resolution                : %g\n", ah->w_resolution);
	printf("X origin                    : %0.4f cm\n", ah->x_offset);
	printf("Y origin                    : %0.4f cm\n", ah->y_offset);
	printf("X radius                    : %0.4f cm\n", ah->x_radius);
	printf("Y radius                    : %0.4f cm\n", ah->y_radius);
	printf("Tilt Angle                  : %0.2f deg\n", ah->tilt_angle);
	printf("Attenuation Coefficient     : %0.3f 1/cm\n", ah->attenuation_coeff);
/*	ELIMINATED
	printf("Sample Distance             : %0.5f cm\n", ah->sample_distance);
*/
}
show_norm_subheader( nh)
  Norm_subheader *nh;
{
	char tod[256];

	printf("Normalization Matrix Sub-header\n");
	printf("Data type                   : %d(%s)\n",
		nh->data_type, dtypes[nh->data_type]);
	printf("Num_dimensions              : %d\n", nh->num_dimensions);
	printf("Dimension_1(num_r_elements) : %d\n", nh->num_r_elements);
	printf("Dimension_2 (num_angles)    : %d\n", nh->num_angles);
	printf("num_z_elements              : %d\n", nh->num_z_elements);
	printf("ring difference             : %d\n", nh->ring_difference);
	printf("Scale Factor                : %0.5g\n", nh->scale_factor);
/*	MOVED TO MAIN HEADER
	day_time( tod, nh->norm_day, nh->norm_month, nh->norm_year,
		nh->norm_hour, nh->norm_minute, nh->norm_second);
	printf("Normalization TOD           : %s\n", tod);
*/
	printf("FOV Source Width            : %0.4f cm\n", nh->fov_source_width);
	printf("storage order               : %d\n", nh->storage_order);
	printf("Norm_min                    : %d\n", nh->norm_min);
	printf("Norm_max                    : %d\n", nh->norm_max);
}

show_norm3d_subheader( nh)
  Norm3D_subheader *nh;
{
	printf("Normalization Matrix Sub-header\n");
	printf("Data type                   : %d(%s)\n",
		nh->data_type, dtypes[nh->data_type]);
	printf("number of radial elements   : %d\n", nh->num_r_elements);
	printf("number of transaxial crystals  : %d\n",nh->num_transaxial_crystals);
	printf("number of crystal rings        : %d\n", nh->num_crystal_rings);
	printf("number of crystals per ring    : %d\n", nh->crystals_per_ring);
	printf("number of plane geometric corrections : %d\n",
		nh->num_geo_corr_planes);
}

