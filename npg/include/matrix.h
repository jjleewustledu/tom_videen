/*  $Log: matrix.h,v $
 * Revision 2.0  2004/02/12  20:30:50  tom
 * sync Feb 2004
 *
 * Revision 1.8  2003/02/18  17:05:59  tom
 * cleaned up while consider port of libmatrix.a
 *
 * Revision 1.7  2002/12/20  17:54:51  tom
 * add functions for libcti.a
 *
 * Revision 1.6  2002/11/27  18:24:31  tom
 * *** empty log message ***
 *
 * Revision 1.5  2000/02/18  16:00:46  tom
 * change word to short
 *
 * Revision 1.4  1999/12/14  15:38:44  tom
 * add mat_close mat_numdoc matrix_close matrix_write
 *
 * Revision 1.3  1998/01/30  14:20:56  tom
 * add comments
 *
 * Revision 1.2  1996/04/04  14:08:54  tom
 * *** empty log message ***
 *
 * Revision 1.1  1996/03/05  22:43:42  tom
 * Initial revision
 *
    $Id: matrix.h,v 2.0 2004/02/12 20:30:50 tom Exp $
 */
/* =============================================================================
 *  Module:         matrix.h	(libcti.a)
 *  Date:           Feb 2003
 *  Author:         CTI matrix.h 2.1 10/4/91
 *  Description:    Routines for ECAT matrix file I/O (libcti.a)
 * =============================================================================
 */
#ifndef		matrix_dot_h_defined
#define		matrix_dot_h_defined

/*@unused@*/
#ifndef	lint
static char     matrix_rcsid[] = "$Header: /usr/local/npg/include/RCS/matrix.h,v 2.0 2004/02/12 20:30:50 tom Exp $";
#endif

#include <stdio.h>

struct MatDir {
	int             matnum;
	int             strtblk;
	int             endblk;
	int             matstat;
};

typedef
struct matdir {
	int             nmats;
	int             nmax;
	struct MatDir  *entry;
}              *Mat_dir;

struct Matval {
	int             frame, plane, gate, data, bed;
};

typedef
struct mat_main_header {
	char            original_file_name[20];
	short           sw_version;
	short           data_type;
	short           system_type;
	short           file_type;
	char            node_id[10];
	short           scan_start_day, scan_start_month, scan_start_year, scan_start_hour, scan_start_minute, scan_start_second;
	char            isotope_code[8];
	float           isotope_halflife;
	char            radiopharmaceutical[32];
	float           gantry_tilt, gantry_rotation, bed_elevation;
	short           rot_source_speed, wobble_speed, transm_source_type;
	float           axial_fov, transaxial_fov;
	short           transaxial_samp_mode, coin_samp_mode, axial_samp_mode;
	float           calibration_factor;
	short           calibration_units, compression_code;
	char            study_name[12], patient_id[16], patient_name[32], patient_sex, patient_age[10], patient_height[10], patient_weight[10], patient_dexterity, physician_name[32], operator_name[32], study_description[32];
	short           acquisition_type, bed_type, septa_type;
	char            facility_name[20];
	short           num_planes, num_frames, num_gates, num_bed_pos;
	float           init_bed_position, bed_offset[15], plane_separation;
	short           lwr_sctr_thres, lwr_true_thres, upr_true_thres;
	float           collimator;
	char            user_process_code[10];
	short           acquisition_mode;
}               Main_header;

typedef
struct mat_scan_subheader {
	short           data_type, dimension_1, dimension_2, smoothing, processing_code;
	float           sample_distance, isotope_halflife;
	short           frame_duration_sec;
	int             gate_duration, r_wave_offset;
	float           scale_factor;
	short           scan_min, scan_max;
	int             prompts, delayed, multiples, net_trues;
	float           cor_singles[16], uncor_singles[16], tot_avg_cor, tot_avg_uncor;
	int             total_coin_rate, frame_start_time, frame_duration;
	float           loss_correction_fctr;
	int             phy_planes[8];		 /* physical plane numbers that make up this
						  * logical plane */
}               Scan_subheader;

typedef
struct mat_image_subheader {
	short           data_type, num_dimensions, dimension_1, dimension_2;
	float           x_origin, y_origin, recon_scale,	/* Image ZOOM from reconstruction */
	                quant_scale;		 /* Scale Factor */
	short           image_min, image_max;
	float           pixel_size,		 /* cm */
	                slice_width;		 /* cm */
	int             frame_duration, frame_start_time;
	short           slice_location, recon_start_hour, recon_start_minute, recon_start_sec;
	int             recon_duration;
	short           filter_code;
	int             scan_matrix_num, norm_matrix_num, atten_cor_matrix_num;
	float           image_rotation, plane_eff_corr_fctr, decay_corr_fctr, loss_corr_fctr, intrinsic_tilt;
	short           processing_code, quant_units, recon_start_day, recon_start_month, recon_start_year;
	float           ecat_calibration_fctr, well_counter_cal_fctr, filter_params[6];
	char            annotation[40];
}               Image_subheader;

typedef
struct mat_norm_subheader {
	short           data_type, dimension_1, dimension_2;
	float           scale_factor;
	short           norm_hour, norm_minute, norm_second, norm_day, norm_month, norm_year;
	float           fov_source_width, ecat_calib_factor;
}               Norm_subheader;

typedef
struct mat_attn_subheader {
	short           data_type, attenuation_type, dimension_1, dimension_2;
	float           scale_factor, x_origin, y_origin, x_radius, y_radius, tilt_angle, attenuation_coeff, sample_distance;
}               Attn_subheader;

typedef
struct matdirnode {
	int             matnum;
	int             strtblk;
	int             endblk;
	int             matstat;
	struct matdirnode *next;
}               MatDirNode;

typedef
struct matdirlist {
	int             nmats;
	MatDirNode     *first;
	MatDirNode     *last;
}               MatDirList;

typedef
struct matrixdata {
	int             mat_type;
	int             matnum;
	char           *shptr;
	char           *data_ptr;
	int             nviews;
	int             nelements;
	int             nblks;
	int             data_type;
}               MatrixData;

typedef
struct matrix_file {
	Main_header    *mhptr;
	MatDirList     *dirlist;
	FILE           *fptr;
	int             mode;
	char            fname[80];
}               MatrixFile;

typedef
struct matdirblk {
	int             nfree, nextblk, prvblk, nused;
	struct MatDir   matdir[31];
}               MatDirBlk;

typedef struct m6_header {
	MatrixFile     *mfp;
	Main_header    *mh;
	Image_subheader *sh;
	Scan_subheader *scn;
	Attn_subheader *atn;
	Norm_subheader *nrm;
}               M6header;

#define	MatBLKSIZE 512
#define	MatFirstDirBlk 2

#define TotalCounts   0
#define UnknownCalib  TotalCounts + 1
#define ECAT_COUNTS   UnknownCalib + 1
#define MCi_ML        ECAT_COUNTS + 1
#define LmrGlu        MCi_ML + 1		 /* vax compatibility */
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

#define ACQM_NORMAL      0			 /* normal acquisition mode */
#define ACQM_RODTRN      1			 /* rod transmission mode */
#define ACQM_RODTRN_KEEP 2			 /* rod transmission mode keep all sinograms */
#define ACQM_DWIN_KEEP   3			 /* dual window mode keep all sinograms */
#define ACQM_DWIN        4			 /* dual window mode */
#define ACQM_SIMULT      5			 /* simultaneous transmission/ emission mode */

#define ERROR	-1
#define OK 		0

#define Mlist_NULL (Mlist*) 0

typedef struct Matrix_List {
	int             nmats;
	int             frames;
	int             planes;
	int             gates;
	int             beds;
	int            *list;
}               Mlist;

Mlist          *make_list (),
               *create_Mlist ();

int            *parse_matnum (),
                parse_list (),
                numcomp (),
                extract_num (),
                match_list (),
                match_files ();

void            init_Mlist (),
                free_Mlist (),
                print_Mlist (),
                make_matlist (),
                make_num_list ();

/*	matrix.c	*/
void            swaw ();
void            swap_byte ();
float           get_vax_float ();
int             get_vax_long ();
void            sunftovaxf ();
void            sunltovaxl ();
int             str_find ();
int             str_replace ();
FILE           *mat_create ();
FILE           *mat_open ();
int             mat_close ();
int             mat_numcod ();
int             mat_lookup ();
int             mat_rblk ();
int             mat_wblk ();
int             mat_enter ();
int             mat_read_main_header ();
int             mat_read_image_subheader ();
int             mat_read_scan_subheader ();
int             mat_read_attn_subheader ();
int             mat_read_norm_subheader ();
int             mat_write_main_header ();
int             mat_write_idata ();
int             mat_write_fdata ();
int             mat_write_image_subheader ();
int             mat_write_scan_subheader ();
int             mat_write_attn_subheader ();
int             mat_write_norm_subheader ();

/*	matrix_extra.c	*/
int				file_access_code ();
int				file_size ();
int				file_exists ();
int             free_matrix_data ();
void			matrix_freelist ();
MatrixFile     *matrix_create ();
Main_header    *matrixx_init_main_header ();
int				matrix_data_init ();
MatrixFile     *matrix_open ();
int             matrix_close ();
MatrixData     *matrix_read ();
struct MatDir  *matrix_find ();
int				mat_make_dir ();
int             mat_mlist ();
int             insert_mdir ();
MatDirBlk	   *mat_rdirblk ();
MatDirList	   *mat_read_directory ();
int             read_sun_data ();
int             write_sun_data ();
int             read_matrix_data ();
int             write_matrix_data ();
int             read_main_header ();
int             update_main_header ();
int             write_main_header ();
int             read_image_subheader ();
int             write_image_subheader ();
int             read_scan_subheader ();
int             write_sino_subheader ();
int             read_attn_subheader ();
int             write_attn_subheader ();
int             read_norm_subheader ();
int             write_norm_subheader ();

/*	displayMatrixData.c	*/
void    		displayMainHeader ();
void    		displayScanHeader ();
void    		displayImageHeader ();
void    		displayAttnHeader ();
void    		displayNormHeader ();
void    		displayData ();
void    		displayFloatData ();
void    		dumpHexData ();
void            convertScanHeaderFromVax ();
void            convertImageHeaderFromVax ();
void            convertAttnHeaderFromVax ();
void            convertNormHeaderFromVax ();
void            convertScanHeaderToVax ();
void            convertImageHeaderToVax ();
void            convertAttnHeaderToVax ();
void            convertNormHeaderToVax ();

#endif						 /* matrix_dot_h_defined */
