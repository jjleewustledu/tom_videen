/*$Id: sspct.h,v 2.0 2004/02/12 20:30:50 tom Exp $*/
/*$Log: sspct.h,v $
 * Revision 2.0  2004/02/12  20:30:50  tom
 * sync Feb 2004
 *
 * Revision 1.1  1998/08/26  19:49:44  tom
 * Initial revision
 *
 * Revision 1.2  1996/12/12  20:37:42  jane
 * Modifications related to correcting for gantry tilt in ct2analyze:
 *    1. in FIELD_NAME: TABLE_INC --> EXP_TIME ("TI" in original header)
 *    2. in SCAN_INFO: table_incr --> exp_time
 *    3. in SSPCT_INIT: added member table_incr
 *    4. fn. declar. for get_sspct_head(): added argument "reorient"
 *
 * Revision 1.1  1996/06/10  08:57:20  jane
 * Initial revision
 **/
/*  ===============================================================================
 *  Module:         sspct.h
 *  Date:           Apr 1996
 *  Author:         Jane Dunford-Shore
 *  Description:    main header file for libsspct
 *  ===============================================================================
 */

#ifndef SSPCT_DOT_H
#define SSPCT_DOT_H

#ifndef TIME_DOT_H
#include <time.h>
#endif

typedef enum {
	FALSE,					 /* FALSE = 0 */
	TRUE					 /* TRUE  = 1 */
}               BOOL;

typedef enum {
	CHAR_1,					 /* 1-byte character */
	UINT_2,					 /* 2-byte unsigned integer */
	UINT_4,					 /* 4-byte unsigned integer */
	INT_4,					 /* 4-byte signed integer */
	FLOAT					 /* floating-point number */
}               DATA_TYPE;			 /* Datatypes identified in SSPLUS header thus far */

typedef enum {
	UNIDENTIFIED,			 /* bytes not yet been identified */
	IMAGE,					 /* whether image is localizer or axial */
	PT_NAME_BIRTH,			 /* patient name and birth date */
	SCAN_ID,				 /* scan identifier */
	SLICE_DATE,				 /* slice date as printed to header by scanner software */
	SLICE_TIME,				 /* slice time as printed to header by scanner software */
	STUDY_DATE,				 /* study date as typed by technologist */
	STUDY_TIME,				 /* study time as typed by technologist */
	SCAN_NO,				 /* scan number -- usually 00001.ima is 1, etc. */
	EXP_TIME,				 /* exposure time */
	KV,						 /* kV */
	MAS,					 /* mAS */
	SLICE_WIDTH,			 /* slice width in mm */
	GANTRY_TILT,			 /* gantry tilt */
	ZOOM,					 /* zoom */
	CE_X,					 /* center of image in field, x axis */
	CE_Y,					 /* center of image in field, y axis */
	TABLE_POS,				 /* table position */
	PT_ORIENT,				 /* patient orientation */
	PIXELSIZE,				 /* pixel size */
	NUM_SLICES,				 /* number of slices */
	INFILE,					 /* name of original image file (*.ima) */
	OUTFILE,				 /* output file prefix ("p1023", etc.) */
	GLMIN,					 /* minimum pixel value */
	GLMAX,					 /* maximum pixel value */
	INSTITUTION,			 /* where the scan was done (Mallinckrodt Inst */
	SYSTEM,					 /* originating system (Siemens Somatom Plus S) */
	COMMENT1,				 /* field with values such as "H/SP CONTRAST" */
	COMMENT2,				 /* field with values such as "PET SCAN STUDY" */
	UNKNOWN1,				 /* field whose meaning is unknown, althoug it is */
							 /* known that field data is printed on ct films */
	LAST					 /* not an actual field -- for ending loop only */
}               FIELD_NAME;			 /* Field tags (indices) */

typedef struct {
	FIELD_NAME      field_name;		 /* descriptive tag (index) */
	int             offset;			 /* distance from beg. of file (first byte = 0) */
	int             length;			 /* length of field in bytes */
	DATA_TYPE       data_type;		 /* data type of field */
}               FIELD;				 /* Header field descriptions */

typedef enum {
	LOCALIZER,				 /* Image is localizer (scout) */
	AXIAL					 /* Image is axial section */
}               IMAGETYPE;			 /* Images are one of two types */

/*
 * Data structure to hold values that need to be passed to get_sspct_head()
 * -- values that cannot be extracted from header
 */

typedef struct {
	int             num_slices;		 /* number of slices in set specified by user */
	char            prefix[252];	 /* output filename prefix */
	char            infile[256];	 /* input filename */
	short           global_min;		 /* pixel minimum */
	short           global_max;		 /* pixel maximum */
	float           table_incr;		 /* table increment */
}               SSPCT_INIT;			 /* Initialization values for SSPCT header function */


/*
 * Data structures to hold values extracted from header, as well as some
 * computed values describing image set.
 */

typedef struct {
	char            pt_name_birth[26];	 /* patient first and last name and birth date */
}               PT_INFO;			 /* patient info. -- same for all slices in a set */

typedef struct {
	char            system[22];		 /* originating system (Somatom Plus S) */
	char            institution[26]; /* where the scan was done */
	char            scan_id[13];	 /* id assigned to scan (by technologist or machine? */
	int             num_slices;		 /* number of slices in set specified by user */
	char            prefix[256];	 /* output file prefix specified by user */
	char            date[13];		 /* study date as typed at console by technologist */
	char            time[12];		 /* study time as typed at console by technologist */
	char            exp_time[10];	 /* exposure time */
	char            mAs[9];			 /* tube current in milliamps? */
	char            kV[9];			 /* tube voltage in kilovolts */
	char            slice_width[9];	 /* slice width in mm */
	char            gantry_tilt[10]; /* gantry tilt in degrees */
	char            zoom[9];		 /* zoom */
	char            ce_x[10];		 /* center of image in field, x axis */
	char            ce_y[10];		 /* center of image in field, y axis */
	char            pt_orient[21];	 /* patient orientation */
	float           xpix, ypix, zpix; /* pixel size in mm */
	int             xdim, ydim, zdim; /* number of pixels in each dimension */
	short           global_min;		 /* minimum pixel value */
	short           global_max;		 /* maximum pixel value */
	char            comment1[25];	 /* "H/SP CONTRAST", for instance */
	char            comment2[25];	 /* "PET SCAN STUDY", for instance */
}               SCAN_INFO;			 /* scan info. -- same for all slices in a set */

typedef struct {
	char            filename[50];	 /* name of data file for particular slice */
	char            image_no[4];	 /* ordinal position of image in image set */
									 /* e.g., image in file 00001.ima is image 1, etc. */
	char            scan_no[4];		 /* slice number (often referred to as scan number) */
									 /* usually image from 00002.ima has scan no. of 1, etc. */
	IMAGETYPE       image_type;		 /* LOCALIZER = 0; AXIAL = 1 */
	struct tm       str_date;		 /* date/time structure (defined in time.h) */
									 /* numbers from scanner -- not string typed by technologist */
	char            date[11];		 /* study date in Interfile format ["1995:10:19"] */
	char            time[9];		 /* study time in Interfile format ["13:11:23"] */
	char            table_pos[10];	 /* table position in mm; may be negative number */
	char            unknown[9];		 /* "TH 7502" for scouts, "AH 7572" otherwise */
}               SLICE_INFO;			 /* slice info. -- different for each slice */

typedef struct {
	PT_INFO         pt;
	SCAN_INFO       scan;
	SLICE_INFO      slice;
}               SSPCT_HDR;

typedef struct dsr DSR;				 /* Analyze header structure (see ANALYZE.h) */

/*
 * Variable declarations
 */

extern const int COLUMNS;			 /* columns in pixel matrix */
extern const int ROWS;				 /* rows in pixel matrix */

extern const unsigned int LEN_HEADER; /* bytes in header portion of file */
extern const unsigned int LEN_DATA;	 /* bytes in data portion of file */

extern const int BITSPERPIX;		 /* pixel values are 16-bit unsigned */

extern const char TRANSVERSE;
extern const char CORONAL;
extern const char SAGITTAL;

extern const FIELD head_fields[];	 /* fields in header -- their indices (enums), */
									 /* offsets, lengths, and datatypes            */

/*
 * Function declarations
 */

extern float    dec2sun_float (unsigned char b[4]);
extern void     sspct_init (FIELD_NAME field_name, void *value);
extern void     get_sspct_head (FILE * fp, SSPCT_INIT hdrinit, SSPCT_HDR * head_info, DSR * anal_hdr, FILE * fp_ifh, int reorient);

#endif
