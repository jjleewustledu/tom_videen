/*$Id: matrix.h,v 1.2 1995/01/30 20:07:53 ty7777 Exp $*/
/*$Log: matrix.h,v $
 * Revision 1.2  1995/01/30  20:07:53  ty7777
 * Added program version.
 **/

/* @(#)matrix.h	1.4 1/28/92 */

/* prevent recursive definition */
#ifndef		matrix_dot_h_defined
#define		matrix_dot_h_defined

#include <stdio.h>
#include <sys/types.h>

#define		MatBLKSIZE 512
#define		MatFirstDirBlk 2
#define		MatMagic 0x67452301

static char rcsid_matrix [] = "$Header: /home/npggw/tom/src/3drecon/RCS/matrix.h,v 1.2 1995/01/30 20:07:53 ty7777 Exp $";

typedef short int word;

struct	MatDir {
	int matnum;
	int strtblk;
	int endblk;
	int matstat };


typedef
	struct matdir
	{ int	nmats,
		nmax;
	  struct MatDir *entry;
	}
*Mat_dir;

	FILE *mat_open();
	int mat_rblk();
	int mat_list();

struct Matval {
	int frame, plane, gate, data, bed;
	};

typedef
	struct mat_main_header {
	char		original_file_name[20];
	word		sw_version;
	word		data_type;
	word		system_type;
	word		file_type;
	char		node_id[10];
	word		scan_start_day,
			scan_start_month,
			scan_start_year,
			scan_start_hour,
			scan_start_minute,
			scan_start_second;
	char		isotope_code[8];
	float		isotope_halflife;
	char		radiopharmaceutical[32];
	float		gantry_tilt,
			gantry_rotation,
			bed_elevation;
	word		rot_source_speed,
			wobble_speed,
			transm_source_type;
	float		axial_fov,
			transaxial_fov;
	word		transaxial_samp_mode,
			coin_samp_mode,
			axial_samp_mode;
	float		calibration_factor;
	word		calibration_units,
			compression_code;
	char		study_name[12],
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
	word		acquisition_type,
			bed_type,
			septa_type;
	char		facility_name[20];
	word		num_planes,
			num_frames,
			num_gates,
			num_bed_pos;
	float		init_bed_position,
			bed_offset[15],
			plane_separation;
	word		lwr_sctr_thres,
			lwr_true_thres,
			upr_true_thres;
	float		collimator;
	char		user_process_code[10];
	}
Main_header;

typedef
	struct mat_scan_subheader{
	word		data_type,
			dimension_1,
			dimension_2,
			smoothing,
			processing_code;
	float		sample_distance,
			isotope_halflife;
	word		frame_duration_sec;
	int		gate_duration,
			r_wave_offset;
	float		scale_factor;
	word		scan_min,
			scan_max;
	int		prompts,
			delayed,
			multiples,
			net_trues;
	float		cor_singles[16],
			uncor_singles[16],
			tot_avg_cor,
			tot_avg_uncor;
	int		total_coin_rate,
			frame_start_time,
			frame_duration;
	float		loss_correction_fctr;
	}
Scan_subheader;

typedef
	struct mat_image_subheader{
	word		data_type,
			num_dimensions,
			dimension_1,
			dimension_2;
	float		x_origin,
			y_origin,
			recon_scale,			/* Image ZOOM from reconstruction */
			quant_scale;			/* Scale Factor */
	word		image_min,
			image_max;
	float		pixel_size,
			slice_width;
	int		frame_duration,
			frame_start_time;
	word		slice_location,
			recon_start_hour,
			recon_start_minute,
			recon_start_sec;
	int		recon_duration;
	word		filter_code;
	int		scan_matrix_num,
			norm_matrix_num,
			atten_cor_matrix_num;
	float		image_rotation,
			plane_eff_corr_fctr,
			decay_corr_fctr,
			loss_corr_fctr,
			intrinsic_tilt ;
	word		processing_code,
			quant_units,
			recon_start_day,
			recon_start_month,
			recon_start_year;
	float		ecat_calibration_fctr,
			well_counter_cal_fctr,
			filter_params[6];
	char		annotation[40];
	}
Image_subheader;

typedef
	struct mat_norm_subheader{
	word		data_type,
			dimension_1,
			dimension_2;
	float		scale_factor;
	word		norm_hour,
			norm_minute,
			norm_second,
			norm_day,
			norm_month,
			norm_year;
	float		fov_source_width;
	}
Norm_subheader;

typedef
	struct mat_attn_subheader{
	word		data_type,
			attenuation_type,
			dimension_1,
			dimension_2;
	float		scale_factor,
			x_origin,
			y_origin,
			x_radius,
			y_radius,
			tilt_angle,
			attenuation_coeff,
			sample_distance;
	}
Attn_subheader;

/* Additional definitions for new matrix file interface. P. Luk, 3-Apr-90 */

typedef enum {			/* matrix data types */
		GENERIC,
		BYTE_TYPE,
		VAX_I2,
		VAX_I4,
		VAX_R4,
		SUN_R4,
		SUN_I2,
		SUN_I4
	      }
MatrixDataType;

#define M68K_I2 SUN_I2
#define M68K_I4 SUN_I4
#define IEEE_R4 SUN_R4

#define MAT_WORD SUN_I2
#define MAT_LONG SUN_I4
#define MAT_REAL SUN_R4
#define MAT_FLOAT MAT_REAL

#define MAT_SUB_HEADER 255	/* operation to sub-header only */

typedef enum {			/* matrix file types */
		MAT_UNKNOWN_FTYPE,
		MAT_SCAN_DATA,
		MAT_IMAGE_DATA,
		MAT_ATTN_DATA,
		MAT_NORM_DATA
	      }
MatrixFileType;

#define RAW_DATA MAT_SCAN_DATA
#define SCAN_DATA MAT_SCAN_DATA
#define IMAGE_DATA MAT_IMAGE_DATA
#define ATTN_DATA MAT_ATTN_DATA
#define NORM_DATA MAT_NORM_DATA

#define UNKNOWN	GENERIC

typedef
	struct matdirnode
	{
		int		matnum ;
		int		strtblk ;
		int		endblk ;
		int		matstat ;
		struct matdirnode *next ;
	}
MatDirNode ;

typedef
	struct matdirlist
	{
		int	nmats ;
		MatDirNode *first ;
		MatDirNode *last ;
	}
MatDirList ;

typedef
	struct matrix_file
	{
		char		*fname ;	/* file path */
		Main_header	*mhptr ;	/* pointer to main header */
		MatDirList	*dirlist ;	/* directory */
		FILE		*fptr ;		/* file ptr for I/O calls */
		int		acs ;		/* ACS flag for ACS files */
	}
MatrixFile ;

MatrixFile *matrix_open(), *matrix_create();

typedef
	struct matrixdata
	{
		int		matnum ;	/* matrix number */
		MatrixFile	*matfile ;	/* pointer to parent */
		MatrixFileType	mat_type ;	/* type of matrix? */
		MatrixDataType	data_type ;	/* type of data */
		caddr_t		shptr ;		/* pointer to sub-header */
		caddr_t		data_ptr ;	/* pointer to data */
		int		data_size ;	/* size of data in bytes */
		int		xdim;		/* dimensions of data */
		int		ydim;		/* y dimension */
		int		zdim;		/* for volumes */
		float		scale_factor ;	/* valid if data is int? */
		float		pixel_size;	/* xdim data spacing (cm) */
		float		y_size;		/* ydim data spacing (cm) */
		float		z_size;		/* zdim data spacing (cm) */
		float		data_min;	/* min value of data */
		float		data_max;	/* max value of data */
	}
MatrixData ;

MatrixData *matrix_read(), *matrix_object_create();
Main_header *matrix_init_main_header();

typedef
	struct matdirblk
	{
		int	nfree, nextblk, prvblk, nused ;
		struct 	MatDir matdir[31] ;
	}
MatDirBlk ;

typedef enum {		/* matrix file access modes */
		MAT_READ_WRITE,		/* allow read and/or write */
		MAT_READ_ONLY,		/* only allow read */
		MAT_CREATE,		/* create if non-existant */
		MAT_OPEN_EXISTING,	/* open existing file read/write */
		MAT_CREATE_NEW_FILE	/* force new file creation */
	      }
MatrixFileAccessMode;

/* object creation attributes */

typedef enum
	{
		MAT_NULL,
		MAT_XDIM,
		MAT_YDIM,
		MAT_ZDIM,
		MAT_DATA_TYPE,
		MAT_SCALE_FACTOR,
		MAT_PIXEL_SIZE,
		MAT_Y_SIZE,
		MAT_Z_SIZE,
		MAT_DATA_MAX,
		MAT_DATA_MIN,
		MAT_PROTO
	}
MatrixObjectAttribute;

#ifndef ERROR
#define ERROR	-1
#endif
#ifndef OK
#define OK 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#endif		matrix_dot_h_defined
