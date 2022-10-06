/*  $Log: matrix.h,v $
 * Revision 1.2  1996/04/04  14:08:54  tom
 * *** empty log message ***
 *
 * Revision 1.1  1996/03/05  22:43:42  tom
 * Initial revision
 *
    $Id: matrix.h,v 1.2 1996/04/04 14:08:54 tom Exp $
 */
/* =============================================================================
 *  Module:         matrix.h
 *  Date:           Nov-95
 *  Author:         CTI matrix.h 2.1 10/4/91
 *  Description:    Routines for ECAT matrix file I/O (libcti.a)
 * =============================================================================
 */
#ifndef		matrix_dot_h_defined
#define		matrix_dot_h_defined

#ifndef	lint
static char     matrix_rcsid[] = "$Header: /usr/local/npg/include/RCS/matrix.h,v 1.2 1996/04/04 14:08:54 tom Exp $";
#endif

#include <stdio.h>

typedef short int word;

struct MatDir {
	int matnum;
	int strtblk;
	int endblk;
	int matstat;
};

typedef
	struct matdir {
	int	nmats,
	nmax;
	struct MatDir *entry;
} *Mat_dir;

struct Matval {
	int frame, plane, gate, data, bed;
};

typedef
	struct mat_main_header {
	char	original_file_name[20];
	word	sw_version;
	word	data_type;
	word	system_type;
	word	file_type;
	char	node_id[10];
	word	scan_start_day,
			scan_start_month,
			scan_start_year,
			scan_start_hour,
			scan_start_minute,
			scan_start_second;
	char	isotope_code[8];
	float	isotope_halflife;
	char	radiopharmaceutical[32];
	float	gantry_tilt,
			gantry_rotation,
			bed_elevation;
	word	rot_source_speed,
			wobble_speed,
			transm_source_type;
	float	axial_fov,
			transaxial_fov;
	word	transaxial_samp_mode,
			coin_samp_mode,
			axial_samp_mode;
	float	calibration_factor;
	word	calibration_units,
			compression_code;
	char	study_name[12],
			patient_id[16],
			patient_name[32],
			patient_sex,
			patient_age[10],
			patient_height[10],
			patient_weight[10],
			patient_dexterity,
			physician_name[32],
			operator_name[32],
			study_description[32];
	word	acquisition_type,
			bed_type,
			septa_type;
	char	facility_name[20];
	word	num_planes,
			num_frames,
			num_gates,
			num_bed_pos;
	float	init_bed_position,
			bed_offset[15],
			plane_separation;
	word	lwr_sctr_thres,
			lwr_true_thres,
			upr_true_thres;
	float	collimator;
	char	user_process_code [10];
	word 	acquisition_mode;
} Main_header;

typedef
	struct mat_scan_subheader{
	word	data_type,
			dimension_1,
			dimension_2,
			smoothing,
			processing_code;
	float	sample_distance,
			isotope_halflife;
	word	frame_duration_sec;
	int		gate_duration,
			r_wave_offset;
	float	scale_factor;
	word	scan_min,
			scan_max;
	int		prompts,
			delayed,
			multiples,
			net_trues;
	float	cor_singles[16],
			uncor_singles[16],
			tot_avg_cor,
			tot_avg_uncor;
	int		total_coin_rate,
			frame_start_time,
			frame_duration;
	float	loss_correction_fctr;
    int		phy_planes[8];  /* physical plane numbers that make up this logical plane */
} Scan_subheader;

typedef
	struct mat_image_subheader{
	word	data_type,
			num_dimensions,
			dimension_1,
			dimension_2;
	float	x_origin,
			y_origin,
			recon_scale,		/* Image ZOOM from reconstruction */
			quant_scale;		/* Scale Factor */
	word	image_min,
			image_max;
	float	pixel_size,
			slice_width;
	int		frame_duration,
			frame_start_time;
	word	slice_location,
			recon_start_hour,
			recon_start_minute,
			recon_start_sec;
	int		recon_duration;
	word	filter_code;
	int		scan_matrix_num,
			norm_matrix_num,
			atten_cor_matrix_num;
	float	image_rotation,
			plane_eff_corr_fctr,
			decay_corr_fctr,
			loss_corr_fctr,
			intrinsic_tilt;
	word	processing_code,
			quant_units,
			recon_start_day,
			recon_start_month,
			recon_start_year;
	float	ecat_calibration_fctr,
			well_counter_cal_fctr,
			filter_params[6];
	char	annotation[40];
} Image_subheader;

typedef
	struct mat_norm_subheader{
	word	data_type,
			dimension_1,
			dimension_2;
	float	scale_factor;
	word	norm_hour,
			norm_minute,
			norm_second,
			norm_day,
			norm_month,
			norm_year;
	float	fov_source_width,
			ecat_calib_factor;
} Norm_subheader;

typedef
	struct mat_attn_subheader{
	word	data_type,
			attenuation_type,
			dimension_1,
			dimension_2;
	float	scale_factor,
			x_origin,
			y_origin,
			x_radius,
			y_radius,
			tilt_angle,
			attenuation_coeff,
			sample_distance;
} Attn_subheader;

typedef
	struct matdirnode {
		int	matnum;
		int	strtblk;
		int	endblk;
		int	matstat;
		struct matdirnode *next;
} MatDirNode;

typedef
	struct matdirlist {
		int	nmats;
		MatDirNode *first;
		MatDirNode *last;
} MatDirList;

typedef
	struct matrixdata {
		int	mat_type;
		int	matnum;
		char *shptr;
		char *data_ptr;
		int	nviews;
		int	nelements;
		int	nblks;
		int	data_type;
} MatrixData;

typedef
	struct matrix_file {
		Main_header	*mhptr;
		MatDirList	*dirlist;
		FILE		*fptr;
		int			mode;
		char		fname[80];
} MatrixFile;

typedef
	struct matdirblk {
		int	nfree, nextblk, prvblk, nused;
		struct 	MatDir matdir[31];
} MatDirBlk;

typedef struct m6_header {
	MatrixFile		*mfp;
    Main_header		*mh;
    Image_subheader *sh;
}   M6header;

#define	MatBLKSIZE 512
#define	MatFirstDirBlk 2

#define TotalCounts   0
#define UnknownCalib  TotalCounts + 1
#define ECAT_COUNTS   UnknownCalib + 1
#define MCi_ML        ECAT_COUNTS + 1
#define LmrGlu        MCi_ML + 1              /* vax compatibility */
#define LmrGlu_mMole  LmrGlu + 1
#define LmrGlu_mGram  LmrGlu_mMole + 1
#define NCi_ML        LmrGlu_mGram + 1
#define WELL_COUNTS   NCi_ML + 1
#define BECQUERELS    WELL_COUNTS + 1	

#define	BYTE_TYPE	1
#define	VAX_I2		2
#define VAX_I4		3
#define VAX_R4		4
#define IEEE_R4		5
#define	M68K_I2		6
#define	M68K_I4		7
#define SUN_I2	M68K_I2
#define SUN_I4	M68K_I4
#define SUN_R4	IEEE_R4

#define	RAW_DATA	1
#define	IMAGE_DATA	2
#define	ATTN_DATA	3
#define	NORM_DATA	4
#define	SUN_READ	1
#define	SUN_WRITE	2
#define	ACS_READ	3
#define	ACS_WRITE	4
#define SUN_CREATE	5

#define ACQM_NORMAL      0      /* normal acquisition mode */
#define ACQM_RODTRN      1      /* rod transmission mode */
#define ACQM_RODTRN_KEEP 2      /* rod transmission mode keep all sinograms */
#define ACQM_DWIN_KEEP   3      /* dual window mode keep all sinograms */
#define ACQM_DWIN        4      /* dual window mode */
#define ACQM_SIMULT      5      /* simultaneous transmission/ emission mode */

#define ERROR	-1
#define OK 		0

#define Mlist_NULL (Mlist*) 0

typedef struct Matrix_List {
		int	nmats;
		int	frames;
		int	planes;
		int	gates;
		int	beds;
		int	*list;
} Mlist;

Mlist	*make_list (),
		*create_Mlist ();

int    	*parse_matnum (),
		parse_list (),
		numcomp (),
		extract_num (),
		match_list (),
		match_files ();

void    init_Mlist (),
		free_Mlist (),
		print_Mlist (),
		make_matlist (),
		make_num_list ();


Main_header	*matrixx_init_main_header ();
FILE	*mat_open();
int		mat_rblk();
int		mat_list();
int		mat_numcod ();
int		mat_read_attn_subheader ();
int		mat_read_image_subheader ();
int		mat_read_main_header ();
int		mat_read_scan_subheader ();
int		mat_write_attn_subheader ();
int		mat_write_image_subheader ();
int		mat_write_main_header ();
int		mat_write_norm_subheader ();
int		mat_write_scan_subheader ();
int 	free_matrix_data();

MatrixFile     *matrix_open ();
MatrixFile     *matrix_create ();
MatrixFile     *getfp6 ();
MatrixFile     *write_fp6 ();
MatrixData     *matrix_read ();
struct MatDir  *matrix_find ();

#endif		matrix_dot_h_defined
