/*$Id: analyze.h,v 1.1 1995/10/16 17:18:04 ty7777 Exp $*/
/*$Log: analyze.h,v $
 * Revision 1.1  1995/10/16  17:18:04  ty7777
 * Initial revision
 **/

typedef struct _Analyze_header {
	int size;
	char data_type[10];
	char db_name[18];
	int extents;
	short session_error;
	char regular;
	char hkey_un0;
	short dim[8];
	char vox_units[4];
	char cal_units[8];
	short unused1;
	short data_type;	/* 2: byte, 4: short */
	short bitpix;
	short dim_un0;
	float pixdim;
	float vox_offset;
	float scale_factor;
	float funused2;
	float funused3;
	float cal_max;
	float cal_min;
	int compressed;
	int verified;
	int glmax;
	int glmin;
/*	skip to position 148 */
	char descrip[80];
	char aux_file[24];
	char orient;
	unsigned short origin[5];
	char study_name[10];
	char patient_name[10];
	char patient_id[10];
	int scan_start_time;
	char exp_date[6];
	char exp_time[10]
	char hist_un0[3];
	int views;
	int vols_added;
	int start_field;
	int field_skip;
	int omax;
	int omin;
	int smax;
	int smin;
} Analyze_header;
