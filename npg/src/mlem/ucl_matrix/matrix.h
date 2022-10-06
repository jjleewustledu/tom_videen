/* @(#)matrix.h	1.5 2/8/93 */

/* prevent recursive definition */
#ifndef		matrix_h
#define		matrix_h

#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>

#define		MatBLKSIZE 512
#define		MatFirstDirBlk 2
#define		MatMagic 0x67452301
#define V7 70
#define MatBLKSIZE 512
#define MagicNumLen 14
#define NameLen 32
#define IDLen 16
#define NumDataMasks 12

typedef enum {
	NoData, Sinogram, PetImage, AttenCor, Normalization,
	PolarMap, ByteVolume, PetVolume, ByteProjection,
	PetProjection, ByteImage, Short3dSinogram, Byte3dSinogram, Norm3d,
	Float3dSinogram,InterfileImage, NumDataSetTypes
} DataSetType;

typedef enum {
	UnknownMatDataType, ByteData, VAX_Ix2, VAX_Ix4,
	VAX_Rx4, IeeeFloat, SunShort, SunLong, NumMatrixDataTypes, ColorData,
	BitData
} MatrixDataType;

enum CalibrationStatus {Uncalibrated, Calibrated, Processed,
	    NumCalibrationStatus};


enum ScanType {UndefScan, BlankScan,
        TransmissionScan, StaticEmission,
        DynamicEmission, GatedEmission,
        TransRectilinear, EmissionRectilinear,
        NumScanTypes};

enum CurrentModels {E921, E961, E953, E953B, E951, E951R,
                    E962, E925, NumEcatModels};


enum OldDisplayUnits {TotalCounts, UnknownEcfUnits, EcatCountsPerSec,
	UCiPerCC, LMRGlu, LMRGluUmol, LMRGluMg, NCiPerCC, WellCounts,
	BecquerelsPerCC, MlPerMinPer100g, MlPerMinPer1g, NumOldUnits};

enum PatientOrientation {FeetFirstProne, HeadFirstProne,
	    FeetFirstSupine, HeadFirstSupine,
	    FeetFirstRight, HeadFirstRight,
	    FeetFirstLeft, HeadFirstLeft, UnknownOrientation, NumOrientations};

enum SeptaPos {SeptaExtended, SeptaRetracted, NoSeptaInstalled};

enum Sino3dOrder {ElAxVwRd, ElVwAxRd, NumSino3dOrders};

enum ProcessingCode { NotProc=0, Norm=1, Atten_Meas=2, Atten_Calc=4,
	Smooth_X=8, Smooth_Y=16, Smooth_Z=32, Scat2d=64, Scat3d=128,
	ArcPrc=256, DecayPrc=512, OnComPrc=1024 };


extern int numDisplayUnits;
extern char* datasettype[NumDataSetTypes];
extern char* dstypecode[NumDataSetTypes];
extern char* scantype[NumScanTypes];
extern char* scantypecode[NumScanTypes];
extern char* customDisplayUnits[];
extern float ecfconverter[NumOldUnits];
extern char* calstatus[NumCalibrationStatus];
extern char* modelnumbers[NumEcatModels];
extern char* sexcode;
extern char* dexteritycode;
extern char* typeFilterLabel[NumDataMasks];

/*
 ecat 6.4 compatibility definitions
*/
typedef enum {	      /* matrix data types */
	    GENERIC,
	    BYTE_TYPE,
	    VAX_I2,
	    VAX_I4,
	    VAX_R4,
	    SUN_R4,
	    SUN_I2,
	    SUN_I4
	      }
MatrixDataType_64;

#define MAT_SUB_HEADER 255  /* operation to sub-header only */

typedef enum {	      /* matrix file types */
	    MAT_UNKNOWN_FTYPE,
	    MAT_SCAN_DATA,
	    MAT_IMAGE_DATA,
	    MAT_ATTN_DATA,
	    MAT_NORM_DATA
	      }
MatrixFileType_64;

/*
	end of ecat 6.4 definitions
*/

struct	MatDir {
	int matnum;
	int strtblk;
	int endblk;
	int matstat; };


typedef
	struct matdir
	{ int	nmats,
		nmax;
	  struct MatDir *entry;
	}
*Mat_dir;


struct Matval {
	int frame, plane, gate, data, bed;
	};

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

typedef struct XMAIN_HEAD {
	char magic_number[14];
	char original_file_name[32];
	short sw_version;
	short system_type;
	short file_type;
	char serial_number[10];
	short align_0;						/* 4 byte alignment purpose */
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
	short align_1;
	float calibration_factor;
	short calibration_units;
	short calibration_units_label;
	short compression_code;
	char study_name[12];
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
	short align_2;
	float bin_size;
	float branching_fraction;
	int dose_start_time;
	float dosage;
	float well_counter_factor;
	char data_units[32];
	short septa_state;
	short align_3;
} Main_header;

typedef struct XSCAN_SUB {
	short data_type;
	short num_dimensions;
	short num_r_elements;
	short num_angles;
	short corrections_applied;
	short num_z_elements;
	short ring_difference;
	short align_0;
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
} Scan_subheader;

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
	float v_resolution;
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
} Scan3D_subheader;

typedef struct XIMAGE_SUB {
	short data_type;
	short num_dimensions;
	short x_dimension;
	short y_dimension;
	short z_dimension;
	short align_0;
	float z_offset;
	float x_offset;
	float y_offset;
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
	short align_1;
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
	short align_2;
	float filter_scatter_fraction;
	float filter_scatter_slope;
	char annotation[40];
	float mt_1_1;
	float mt_1_2;
	float mt_1_3;
	float mt_2_1;
	float mt_2_2;
	float mt_2_3;
	float mt_3_1;
	float mt_3_2;
	float mt_3_3;
	float rfilter_cutoff;
	float rfilter_resolution;
	short rfilter_code;
	short rfilter_order;
	float zfilter_cutoff;
	float zfilter_resolution;
	short zfilter_code;
	short zfilter_order;
	float mt_1_4;
	float mt_2_4;
	float mt_3_4;
	short scatter_type;
	short recon_type;
	short recon_views;
	short align_3;
} Image_subheader;

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
	short align_0;
} Norm_subheader;

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
} Norm3D_subheader;

typedef struct XATTEN_SUB {
	short data_type;
	short num_dimensions;
	short attenuation_type;
	short num_r_elements;
	short num_angles;
	short num_z_elements;
	short ring_difference;
	short align_0;
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
	short align_1;
	float additional_atten_coeff[8];
	float edge_finding_threshold;
	short storage_order;
	short span;
	short z_elements[64];
} Attn_subheader;

typedef
	struct matrix_file
	{
		char		*fname ;	/* file path */
		Main_header	*mhptr ;	/* pointer to main header */
		MatDirList	*dirlist ;	/* directory */
		FILE		*fptr ;		/* file ptr for I/O calls */
		int		acs ;		/* ACS flag for ACS files */
		enum { ECAT6, ECAT7, Interfile } file_format;
		char  **interfile_header;
	}
MatrixFile ;

typedef
	struct matrixdata
	{
		int		matnum ;	/* matrix number */
		MatrixFile	*matfile ;	/* pointer to parent */
		DataSetType	mat_type ;	/* type of matrix? */
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
		float       x_origin;       /* x origin of data */
		float       y_origin;       /* y origin of data */
		float       z_origin;       /* z origin of data */
	}
MatrixData ;

#if defined(__STDC__) ||defined(__cplusplus)
#if defined(__cplusplus)
extern "C" {
/*
 * high level user functions
 */
#endif
void SWAB(const void *from, void *to, int len);
void SWAW(const short *from, short *to, int len);
int find_bmin(const u_char*, int size);
int find_bmax(const u_char*, int size);
int find_smin(const short*, int size);
int find_smax(const short*, int size);
int find_imin(const int*, int size);
int find_imax(const int*, int size);
float find_fmin(const float*, int size);
float find_fmax(const float*, int size);
int matspec(const char* specs, char* fname ,int* matnum);
char* is_analyze(const char* );
MatrixFile* matrix_create(const char*,int,Main_header*);
MatrixFile* matrix_open(const char*,int,int);
MatrixData* matrix_read(MatrixFile*, int matnum, int type);
MatrixData* matrix_read_slice(MatrixFile*, MatrixData* volume, int slice_num,
	int segment);
int matrix_write(MatrixFile*, int matnum, MatrixData*);
int mat_numcod(int frame, int plane, int gate, int data, int bed);
int mat_numdoc(int, struct Matval*);
void free_matrix_data(MatrixData*);
void matrix_perror(const char*);
int matrix_close(MatrixFile*);
int matrix_find(MatrixFile*, int matnum, struct MatDir*);
int crash(char *fmt, ...);
/*
 * low level functions prototypes, don't use
 */
int unmap64_main_header(char *buf, Main_header *h);
int unmap64_scan_subheader(char *buf, Scan_subheader*, Main_header*);
int unmap64_attn_header(char *buf, Attn_subheader*, Main_header*);
int unmap64_image_header(char *buf, Image_subheader*, Main_header*);
int unmap64_norm_header(char *buf, Norm_subheader*, Main_header*);
int unmap_main_header(char *buf, Main_header *h);
int unmap_Scan3D_header(char *buf, Scan3D_subheader*);
int unmap_scan_header(char *buf, Scan_subheader*);
int unmap_image_header(char *buf, Image_subheader*);
int unmap_attn_header(char *buf, Attn_subheader*);
int unmap_norm3d_header(char *buf, Norm3D_subheader*);
int unmap_norm_header(char *buf, Norm_subheader*);
int mat_read_main_header(FILE *fptr, Main_header *h);
int mat_read_scan_header( FILE *, Main_header*, int blknum, Scan_subheader*);
int mat_read_Scan3D_header( FILE *, Main_header*, int blknum, Scan3D_subheader*);
int mat_read_image_subheader(FILE *, Main_header*, int blknum, Image_subheader*);
int mat_read_attn_subheader(FILE *, Main_header*, int blknum, Attn_subheader*);
int mat_read_norm_subheader(FILE *, Main_header*, int blknum, Norm_subheader*);
int mat_read_norm3d_subheader(FILE *, Main_header*, int blknum, Norm3D_subheader*);
Mat_dir mat_read_dir(FILE *, Main_header*, char *selector);
int mat_read_matrix_data(FILE *, Main_header*, int blk, int nblks, short* bufr);
int read_matrix_data(FILE*, int strtblk, int nblks, char* dptr, int dtype);
void bufWrite(char* s, char* buf, int* i, int len);
void bufWrite_s(short val, char* buf, int* i);
void bufWrite_i(int val, char* buf, int* i);
void bufWrite_f(float val, char* buf, int* i);
void bufRead(char* s, char* buf, int* i, int len);
void bufRead_s(short*, char* buf, int* i);
void bufRead_i(int*, char* buf, int* i);
void bufRead_f(float*, char* buf, int* i);
#if defined(__cplusplus)
}
#endif
#else /* __cplusplus */
extern float find_fmin(), find_fmax();
extern MatrixFile *matrix_open(), *matrix_create();
extern MatrixData *matrix_read(), *matrix_object_create(), *matrix_read_slice();
extern Main_header *matrix_init_main_header();
extern FILE* mat_open();
extern MatDirList *mat_read_directory();
extern void free_matrix_data();
extern void bufWrite(), bufWrite_s(), bufWrite_i(), bufWrite_f();
extern void bufRead(), bufRead_s(), bufRead_i(), bufRead_f();
extern char* is_analyze();
#endif	/* __cplusplus */
extern int ecat_default_version;
#endif	/* 	matrix_h */
