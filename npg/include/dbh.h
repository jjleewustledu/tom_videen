/*
 * (c) Copyright, 1986-1995
 * Biomedical Imaging Resource
 * Mayo Foundation
 *
 * dbh.h
 *
 * database sub-definitions
 */

struct header_key {	/* header_key */ /* off + size + cmp addr */
	int             sizeof_hdr;		 /* 0 + 4     1		*/
	char            data_type[10];	 /* 4 + 10    5		*/
	char            db_name[18];	 /* 14 + 18   15	*/
	int             extents;		 /* 32 + 4    33	*/
	short int       session_error;	 /* 36 + 2    37	*/
	char            regular;		 /* 38 + 1    39	*/
	char            hkey_un0;		 /* 39 + 1    40	*/
};		 /* total=40  */

struct image_dimension { /* image_dimension *//* off + size */
	short int       dim[8];			 /* 0 + 16    41	*/
	char            vox_units[4];	 /* 16 + 4    57	*/
	char            cal_units[8];	 /* 20 + 4    61	*/
	short int       unused1;		 /* 24 + 2    65	*/
	short int       datatype;		 /* 30 + 2    67	*/
	short int       bitpix;			 /* 32 + 2    69	*/
	short int       dim_un0;		 /* 34 + 2    71	*/
	float           pixdim[8];		 /* 36 + 32   73	*/

	/*
	 * pixdim[] specifies the voxel dimensions: pixdim[1] - voxel width pixdim[2] - voxel
	 * height pixdim[3] - interslice distance ..etc
	 */
	float           vox_offset;		 /* 68 + 4    109	*/
	float           roi_scale;		 /* 72 + 4    113	*/
	float           funused1;		 /* 76 + 4    117	*/
	float           funused2;		 /* 80 + 4    121	*/
	float           cal_max;		 /* 84 + 4    125	*/
	float           cal_min;		 /* 88 + 4    129	*/
	int             compressed;		 /* 92 + 4    131	*/
	int             verified;		 /* 96 + 4    135	*/
	int             glmax, glmin;	 /* 100 + 8   139	*/
};		 /* total=108 */

struct data_history { /* data_history *//* off + size */
	char            descrip[80];	 /* 0 + 80    149	*/
	char            aux_file[24];	 /* 80 + 24   229	*/
	char            orient;			 /* 104 + 1   253	*/
	char            originator[10];		 /* 105 + 10  254	*/
	char            generated[10];	 /* 115 + 10  264	*/
	char            scannum[10];	 /* 125 + 10  274	*/
	char            patient_id[10];	 /* 135 + 10  284	*/
	char            exp_date[10];	 /* 145 + 10  294	*/
	char            exp_time[10];	 /* 155 + 10  304	*/
	char            hist_un0[3];	 /* 165 + 3   314	*/
	int             views;			 /* 168 + 4   317	*/
	int             vols_added;		 /* 172 + 4   321	*/
	int             start_field;	 /* 176 + 4   325	*/
	int             field_skip;		 /* 180 + 4   329	*/
	int             omax, omin;		 /* 184 + 8   333	*/
	int             smax, smin;		 /* 192 + 8   341	*/
};		 /* total=200 */

struct dsr {	 /* dsr */			 /* off + size */
	struct header_key hk;			 /* 0 + 40    */
	struct image_dimension dime;	 /* 40 + 108  */
	struct data_history hist;		 /* 148 + 200 */
};									 /* total=348 */

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
