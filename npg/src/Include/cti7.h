/*  $Log: cti7.h,v $
 * Revision 1.2  1996/04/04  14:05:24  tom
 * *** empty log message ***
 *
 * Revision 1.1  1996/03/05  22:43:17  tom
 * Initial revision
 *
    $Id: cti7.h,v 1.2 1996/04/04 14:05:24 tom Exp $
 */
/* =============================================================================
 *  Module:         cti7.h
 *  Date:           Nov-95
 *  Author:         John Ollinger
 *  Description:    Routines for image and sinogram file I/O (libimg.a)
 *	History:
 *		Nov-95	Modified by TOV
 * =============================================================================
 */
#ifndef cti7_dot_h_defined
#define cti7_dot_h_defined

#ifndef	lint
static char     cti7_rcsid[] = "$Header: /usr/local/npg/include/RCS/cti7.h,v 1.2 1996/04/04 14:05:24 tom Exp $";
#endif

#define SF				48	/* change from 42 to 48 	*/
#define MAX_REC			SF	/* 42 Maximum number of directory records.	*/
							/* ATTENTION:changed from 42 to 48 for 15 tilts (S.Furuie 25Jan93)	*/
#define	NRAMENT			SF*31	/* Number of memory-resident directory entries. */
#define LRAMENT			SF*128	/* Length of memory resident directory in longwords.*/
#define	BEGSEEK			0		/* Seek from the beginning of the file.		*/
#define DNOD_REC		31		/* Number of directory nodes per disk record.	*/
#define LENREC			512		/* Length of one record in bytes.		*/
#define LENREC_L		128		/* Length of one record in longwords.		*/
#define LENREC_S		256		/* Length of one record in short words.		*/
#define NUMREC_SSHDR	2		/* Length of a scan subheader in records.	*/
#define NUMREC_ISHDR	1		/* Length of an image subheader in records.	*/
#define LEN_MAINHEADER7	256
#define LEN_SSHDR		(LENREC_S*NUMREC_SSHDR)
#define LEN_ISHDR		(LENREC_S*NUMREC_ISHDR)
#define DIR_NUM_1		2		/* Record containing first directory record.	*/
#define SIZE_DIR_MALLOC SF		/* Incremental size of directory malloc. */

#define USED			1		/* Denotes directory entry is used.		*/
#define UNUSED			0		/* Denotes directory entry is unused.		*/
#define REMOVED			-1		/* Denotes directory entry points to removed matrix. */

#define SHORT 1
#define FLOAT 2

struct	mat_num {
	long	frame;		/* Frame number.	*/
	long	slice;		/* Slice number. 	*/
	long	phase;		/* Phase number.	*/
};

struct  dirent {
    long    matnum;         /* Encoded image, slice and phase numbers. */
    long    begrec;         /* Beginning record number.     */
    long    endrec;         /* Ending record number.        */
    long    flag;           /* -1: removed, 0: unused, 1: used.*/
};
 
struct  dirhd {
    long    numfree;        /* Number of entries.	*/
    long    forptr;         /* Pointer to next directory record.	*/
    long    bakptr;         /* Pointer to previous directory record.*/
    long    numused;        /* Number of used entries.	*/
};

struct directory {
    struct dirhd    hdr;
    struct dirent   entry[DNOD_REC];
};

struct file {
	int		memsize;
	int		lstrec;
	struct directory *dir;
};

typedef struct XMAIN_HEAD {
	char magic_number[14];
	char original_file_name[32];
	short sw_version;
	short system_type;
	short file_type;				/* DataSetType (image.h) */
	char serial_number[10];
	int scan_start_time;
	char isotope_code[8];
	float isotope_halflife;
	char radiopharmaceutical[32];
	float gantry_tilt;
	float gantry_rotation;
	float bed_elevation;
	float intrinsic_tilt;
	short wobble_speed;
	short transm_source_type;
	float distance_scanned;
	float transaxial_fov;
	short angular_compression;
	short coin_samp_mode;
	short axial_samp_mode;
	float calibration_factor;
	short calibration_units;
	short calibration_units_label;
	short compression_code;
	char study_type[12];
	char patient_id[16];
	char patient_name[32];
	char patient_sex[1];
	char patient_dexterity[1];
	float patient_age;
	float patient_height;
	float patient_weight;
	int patient_birth_date;
	char physician_name[32];
	char operator_name[32];
	char study_description[32];
	short acquisition_type;
	short patient_orientation;
	char facility_name[20];
	short num_planes;
	short num_frames;
	short num_gates;
	short num_bed_pos;
	float init_bed_position;
	float bed_offset[15];
	float plane_separation;
	short lwr_sctr_thres;
	short lwr_true_thres;
	short upr_true_thres;
	char user_process_code[10];
	short acquisition_mode;
	float bin_size;
	float branching_fraction;
	int dose_start_time;
	float dosage;
	float well_counter_factor;
	char data_units[32];
	short septa_state;
	short fill[6];
} Main_header7;

typedef struct XSCAN_SUB {
	short data_type;
	short num_dimensions;
	short num_r_elements;
	short num_angles;
	short corrections_applied;
	short num_z_elements;
	short ring_difference;
	float x_resolution;
	float y_resolution;
	float z_resolution;
	float w_resolution;
	int gate_duration;
	int r_wave_offset;
	int num_accepted_beats;
	float scale_factor;
	short scan_min;
	short scan_max;
	int prompts;
	int delayed;
	int multiples;
	int net_trues;
	float cor_singles[16];
	float uncor_singles[16];
	float tot_avg_cor;
	float tot_avg_uncor;
	int total_coin_rate;
	int frame_start_time;
	int frame_duration;
	float loss_correction_fctr;
	short phy_planes[8];
	void *whole_header;
} Scan_subheader7;

typedef struct X3DSCAN_SUB {
	short data_type;
	short num_dimensions;
	short num_r_elements;
	short num_angles;
	short corrections_applied;
	short num_z_elements[64];
	short ring_difference;
	short storage_order;
	short axial_compression;
	float x_resolution;
	float y_resolution;
	float z_resolution;
	float w_resolution;
	int gate_duration;
	int r_wave_offset;
	int num_accepted_beats;
	float scale_factor;
	short scan_min;
	short scan_max;
	int prompts;
	int delayed;
	int multiples;
	int net_trues;
	float tot_avg_cor;
	float tot_avg_uncor;
	int total_coin_rate;
	int frame_start_time;
	int frame_duration;
	float loss_correction_fctr;
	float uncor_singles[128];
	void *whole_header;
} Scan3D_subheader7;

typedef struct XIMAGE_SUB {
	short data_type;
	short num_dimensions;
	short x_dimension;
	short y_dimension;
	short z_dimension;
	float x_offset;
	float y_offset;
	float z_offset;
	float recon_zoom;
	float scale_factor;
	short image_min;
	short image_max;
	float x_pixel_size;
	float y_pixel_size;
	float z_pixel_size;
	int frame_duration;
	int frame_start_time;
	short filter_code;
	float x_resolution;
	float y_resolution;
	float z_resolution;
	float num_r_elements;
	float num_angles;
	float z_rotation_angle;
	float decay_corr_fctr;
	int processing_code;
	int gate_duration;
	int r_wave_offset;
	int num_accepted_beats;
	float filter_cutoff_frequency;
	float filter_resolution;
	float filter_ramp_slope;
	short filter_order;
	float filter_scatter_fraction;
	float filter_scatter_slope;
	char annotation[40];
	float da_x_rotation_angle;
	float da_y_rotation_angle;
	float da_z_rotation_angle;
	float da_x_translation;
	float da_y_translation;
	float da_z_translation;
	float da_x_scale_factor;
	float da_y_scale_factor;
	float da_z_scale_factor;
	float rfilter_cutoff;
	float rfilter_resolution;
	short rfilter_code;
	short rfilter_order;
	float zfilter_cutoff;
	float zfilter_resolution;
	short zfilter_code;
	short zfilter_order;
	short reserved[145];
} Image_subheader7;

typedef struct XNORM_SUB {
	short data_type;
	short num_dimensions;
	short num_r_elements;
	short num_angles;
	short num_z_elements;
	short ring_difference;
	float scale_factor;
	float norm_min;
	float norm_max;
	float fov_source_width;
	float norm_quality_factor;
	short norm_quality_factor_code;
	short storage_order;
	short span;
	short z_elements[64];
	void *whole_header;
} Norm_subheader7;

typedef struct X3DNORM_SUB {
	short data_type;
	short num_r_elements;
	short num_transaxial_crystals;
	short num_crystal_rings;
	short crystals_per_ring;
	short num_geo_corr_planes;
	short uld;
	short lld;
	short scatter_energy;
	short norm_quality_factor_code;
	float norm_quality_factor;
	float ring_dtcor1[32];
	float ring_dtcor2[32];
	float crystal_dtcor[8];
	short span;
	short max_ring_diff;
	void *whole_header;
} Norm3D_subheader7;

typedef struct XATTEN_SUB {
	short data_type;
	short num_dimensions;
	short attenuation_type;
	short num_r_elements;
	short num_angles;
	short num_z_elements;
	short ring_difference;
	float x_resolution;
	float y_resolution;
	float z_resolution;
	float w_resolution;
	float scale_factor;
	float x_offset;
	float y_offset;
	float x_radius;
	float y_radius;
	float tilt_angle;
	float attenuation_coeff;
	float attenuation_min;
	float attenuation_max;
	float skull_thickness;
	short num_additional_atten_coeff;
	float additional_atten_coeff[8];
	float edge_finding_threshold;
	short storage_order;
	short span;
	short z_elements[64];
	void *whole_header;
} Attn_subheader7;

typedef struct m7_header {
	struct file		*fsr;
	Main_header7	*mh;
	Image_subheader7 *sh;
}	M7header;

#endif cti7_dot_h_defined
