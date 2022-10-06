/*
 * (c) Copyright, 1986-1995
 * Biomedical Imaging Resource
 * Mayo Foundation
 *
 * dbh.h (modified for SPM5; see spm_read_hdr.m)
 *
 * database sub-definitions
 */

struct header_key {	/* header_key */ /* addr + size	*/
	int             sizeof_hdr;		 /* 0 + 4     */
	char            data_type[10];	 /* 4 + 10    */
	char            db_name[18];	 /* 14 + 18   */
	int             extents;		 /* 32 + 4    */
	short int       session_error;	 /* 36 + 2    */
	char            regular;		 /* 38 + 1    */
	char            hkey_un0;		 /* 39 + 1    */
};		 /* total=40  */

struct image_dimension { /* image_dimension *//* off + size */
	short int       dim[8];			 /* 40 + 16   */
	char            vox_units[4];	 /* 56 + 4    */
	char            cal_units[8];	 /* 60 + 4    */
	short int       unused1;		 /* 64 + 2    */
	short int       datatype;		 /* 70 + 2    */
	short int       bitpix;			 /* 72 + 2    */
	short int       dim_un0;		 /* 74 + 2    */
	float           pixdim[8];		 /* 76 + 32   */
	
	/* pixdim[] specifies the voxel dimensions: pixdim[1] - voxel width pixdim[2] - voxel
	 * height pixdim[3] - interslice distance ..etc
	 */
	float           vox_offset;		 /* 108 + 4   */
	float           roi_scale;		 /* 112 + 4   */
	float           funused1;		 /* 116 + 4   */
	float           funused2;		 /* 120 + 4   */
	float           cal_max;		 /* 124 + 4   */
	float           cal_min;		 /* 128 + 4   */
	int             compressed;		 /* 132 + 4   */
	int             verified;		 /* 136 + 4   */
	int             glmax, glmin;	 /* 140 + 8   */
};		 /* total=108 */

struct data_history { /* data_history *//* off + size */
	char            descrip[80];	 /* 148 + 80  */
	char            aux_file[40];	 /* 228 + 40  */
	float           xorigin1;	     /* 269 + 4   */
	float           yorigin1;	     /* 274 + 4   */
	float           zorigin1;	     /* 278 + 4   */
	short           scannum;		 /* 282 + 2   */
	char            patient_id[10];	 /* 284 + 10  */
	float           xorigin2;	     /* 294 + 4   */
	char            date_time[10];	 /* 298 + 10  */
	float           yorigin2;	     /* 308 + 4   */
	char            hist_un0[4];	 /* 312 + 4   */
	int             views;			 /* 316 + 4   */
	float           vols_added;		 /* 320 + 4   */
	float           zorigin2;	 	 /* 324 + 4   */
	int             field_skip;		 /* 328 + 4   */
	int             omax;			 /* 332 + 4   */
	int             omin;			 /* 336 + 4   */
	int             smax;			 /* 340 + 4   */
	int             smin;			 /* 344 + 4   */
};		 /* total=200 */

struct dsr {	 /* dsr */			 /* off + size */
	struct header_key hk;			 /* 0 + 40    */
	struct image_dimension dime;	 /* 40 + 108  */
	struct data_history hist;		 /* 148 + 200 */
};		/* total=348 */

/* Acceptable values for hdr.dime.datatype */

#define DT_NONE				0
#define DT_UNKNOWN			0
#define DT_BINARY			1
#define DT_UNSIGNED_CHAR	2
#define DT_SIGNED_SHORT		4
#define DT_SIGNED_INT		8
#define DT_FLOAT			16
#define DT_COMPLEX			32
#define DT_DOUBLE			64
#define DT_RGB				128
#define DT_ALL				255

typedef struct {
	float           real;
	float           imag;
}               COMPLEX;
