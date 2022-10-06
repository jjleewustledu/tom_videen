/*  $Log: imagio.h,v $
 * Revision 2.0  2004/02/12  20:30:50  tom
 * sync Feb 2004
 *
 * Revision 1.1  1996/03/05  22:43:22  tom
 * Initial revision
 *
    $Id: imagio.h,v 2.0 2004/02/12 20:30:50 tom Exp $
 */
/* =============================================================================
 *  Module:         imagio.h
 *  Date:           Nov-95
 *  Author:         John Ollinger
 *  Description:    Routines for image and sinogram file I/O (libimg.a)
 *	History:
 *		Modified by Tom Videen Dec-95
 * =============================================================================
 */
#ifndef imagio_dot_h_defined
#define imagio_dot_h_defined

#ifndef	lint
static char     imagio_rcsid[] = "$Header: /usr/local/npg/include/RCS/imagio.h,v 2.0 2004/02/12 20:30:50 tom Exp $";
#endif

#ifndef myvalues_dot_h_defined
#include <myvalues.h>
#endif
/*	The following are defined in  myvalues.h
	#define MAXTOKEN	32
	#define MAXFNAME	120	
	#define FALSE		0
	#define TRUE		1
	#define OK			0
	#define ERROR		-1
 */

#define MATRIX_REV6		1
#define MATRIX_REV7		2
#define INTERFILE		3
#define PETT6			4
#define LEN_MAINHEADER	256
#define LEN_SUBHEADER	256
#define REV7SCALER		0.000001 /* scale Version 7 pixels to a reasonable short range */
#define ATNSCALER		10000.0 /* scale attenuation pixels to a reasonable short range */
#define MAX_FILES       50      /* Max files that can be open at one time */

#define BYTE_DATA   1
#define SHORT_DATA  2
#define FLOAT_DATA  3

typedef enum {
    NoData, Sinogram, PetImage, AttenCor, Normalization,
    PolarMap, ByteVolume, PetVolume, ByteProjection,
    PetProjection, ByteImage, Short3dSinogram,
    Byte3dSinogram, Norm3d, NumDataSetTypes
} DataSetType;

typedef struct pet_mhdr {
	char           *data_file;		 /* File name */
	int             file_format;	 /* File format, MATRIX_REV6, MATRIX_REV7 eg. */
	int             number_format;	 /* Number type: e.g., 1=byte, 2=short, 3=float */
	int             version;		 /* Software version number */
	int     		filtyp;			 /* DataSetType (ECAT) or rec version (PETT VI) */
	int             nplane;			 /* Number of slices per frame 	 */
	int             nframe;			 /* Number of frames collected 	 */
	int             ngate;			 /* Number of gates collected 	 */
	int             nbed;			 /* Number of bed positions 	 */
	int             scanner_type;	 /* 953, 961, 6 */
	char            date_time[20];	 /* Creation time of original data */
	int             start_time;
	int				wobble_speed;
	char            patnum[16];		 /* Patient ID */
	float           bed_elevation;
	float           init_bed_pos;
	float           bed_off[16];
	float           slcthk;
	float           calibration_factor;
	char            isotope[8];
	char            radiopharmaceutical[32];
	int             orientation;	/* 0=neurological (right-on-right), 1=radiological */
	int             scale_flag;		/* post-recon scaling: 0=none, 1=scaled by pr_scaler */
	float           pr_scaler;		/* post-recon scaling factor; 0 if none */
	int             pr_filter;		/* see pettable.h for codes */
	int             pett6_flag;		/* 1 = ECAT file created from a PETT VI image */
	int             zorigin;		/* Slice number corresponding atlas z-origin */
	char            *sw_revision;	/* Software version which created this file */
	char            *filetime;		/* Creation time of this file */
	char			*gen_data;		/* Whatever */
	char			scanner[MAXTOKEN];			/* Scanner name for archive path */
	char			acquisition[MAXTOKEN];		/* Acquisition name for archive path */
	char			reconstruction[MAXTOKEN];	/* Reconstruction name for archive path */
	char			filtername[MAXTOKEN];		/* Filter name for archive path */
	void           *whole_header;				/* Main header of the input file */
}               MainHeader;

typedef struct subheader {
	int             number_format;	 /* Data type */
	int             file_format;
	int             ndim;
	int             dim1;			 /* X dimension */
	int             dim2;			 /* Y dimension */
	int             dim3;			 /* Z dimension */
	int             ring_diff;
	int             total;
	int             delayeds;
	int             multiples;
	int             trues;
	int             count_rate;
	float           scale_factor;
	float           calibration_factor;
	int             min;
	int             max;
	float           deadtime_corr;
	float           sample_size_1;
	float           sample_size_2;
	float           sample_size_3;
	int             start_time;
	int             duration;			/* msec */
	int				filter_code;
	float			filter_cutoff_frequency;
	float			filter_resolution;
	float			filter_ramp_slope;
	float			filter_order;
	float			filter_scatter_fraction;
	float			filter_scatter_slope;
    char           *comment;         /* User defined comment. */
	void           *whole_header;
}               SubHeader;

#endif imagio_dot_h_defined
