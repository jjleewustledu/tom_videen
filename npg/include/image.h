/*  $Id: image.h,v 2.0 2004/02/12 20:30:50 tom Exp $
 *	$Log: image.h,v $
 * Revision 2.0  2004/02/12  20:30:50  tom
 * sync Feb 2004
 *
 * Revision 1.10  2004/01/08  15:36:50  tom
 * *** empty log message ***
 *
 * Revision 1.9  2003/01/23  15:06:07  tom
 * add processing_code to Subheader
 *
 * Revision 1.8  2000/07/17  19:29:02  tom
 * change ifh to ih
 *
 * Revision 1.7  2000/04/05  19:58:35  tom
 * add type ANALYZE
 *
 * Revision 1.6  1999/12/15  20:52:54  tom
 * year 2000; increase date_time to 22 characters
 *
 * Revision 1.5  1998/12/10  18:42:57  tom
 * *** empty log message ***
 *
 * Revision 1.4  1996/08/02  12:37:44  tom
 * add REV7NSCALE
 *
 * Revision 1.3  1996/07/18  18:55:54  tom
 * add decay_corr to SubHeader
 *
 * Revision 1.2  1996/04/05  21:08:09  tom
 * add comments
 *
 * Revision 1.1  1996/04/04  14:06:23  tom
 * Initial revision
 *
 */
/* =============================================================================
 *  Module:         image.h
 *  Date:           Mar 1996
 *  Author:         Tom Videen
 *  Description:    Routines for image and sinogram file I/O (libimage.a)
 *	History:
 *		Modified version of imagio.h by John Ollinger
 * =============================================================================
 */
#ifndef image_dot_h_defined
#define image_dot_h_defined

/*@unused@*/
#ifndef	lint
static char     image_rcsid[] = "$Header: /usr/local/npg/include/RCS/image.h,v 2.0 2004/02/12 20:30:50 tom Exp $";
#endif

#ifndef myvalues_dot_h_defined
#include <myvalues.h>
#endif

#ifndef cti7_dot_h_defined
#include <cti7.h>
#endif

#ifndef cti6_dot_h_defined
#include <cti6.h>
#endif

#ifndef interfile_dot_h_defined
#include <interfile.h>
#endif

#ifndef pett6_dot_h_defined
#include <pett6.h>
#endif

#define MATRIX_REV6		1
#define MATRIX_REV7		2
#define INTERFILE		3
#define PETT6			4
#define ANALYZE			5
#define REV7SCALER		0.000001	 /* scale Version 7 pixels to a reasonable short range */
#define REV7NSCALE		20.0		 /* scaler for non-quantitative Version 7 images */
#define ATNSCALER		10000.0		 /* scale attenuation pixels to a reasonable short range */
#define MAX_FILES       50			 /* Max files that can be open at one time */

#define BYTE_DATA   1
#define SHORT_DATA  2
#define FLOAT_DATA  3
#define INT_DATA    4

typedef enum {
	NoData, Sinogram, PetImage, AttenCor, Normalization,
	PolarMap, ByteVolume, PetVolume, ByteProjection,
	PetProjection, ByteImage, Short3dSinogram,
	Byte3dSinogram, Norm3d, NumDataSetTypes
}   DataSetType;

typedef struct pet_mhdr {
	char           *data_file;		 		/* File name */
	int             file_format;	 		/* File format, MATRIX_REV6, MATRIX_REV7 eg. */
	int             number_format;	 		/* Number type: e.g., 1=byte, 2=short, 3=float */
	int             sw_version;		 		/* Software version number */
	int             filetype;		 		/* DataSetType (ECAT) or rec version (PETT VI) */
	int             nplane;			 		/* Number of slices per frame 	 */
	int             nframe;			 		/* Number of frames collected 	 */
	int             ngate;			 		/* Number of gates collected 	 */
	int             nbed;			 		/* Number of bed positions 	 */
	int             scanner_type;	 		/* 953, 961, 6 */
	char            date_time[22];	 		/* Creation time of original data */
	int             start_time;
	int             wobble_speed;
	char            patnum[16];		 		/* Patient ID */
	float           bed_elevation;
	float           init_bed_pos;
	float           bed_off[16];
	float           slicesep;		 		/* slice separation */
	float           calibration_factor;
	char            isotope[8];
	char            radiopharmaceutical[32];
	int             orientation;	 		/* 0=neurological (right-on-right), 1=radiological */
	int             scale_flag;		 		/* post-recon scaling: 0=none, 1=scaled by pr_scaler */
	float           pr_scaler;		 		/* post-recon scaling factor; 0 if none */
	int             pr_filter;		 		/* see pettable.h for codes */
	int             pett6_flag;		 		/* 1 = ECAT file created from a PETT VI image */
	int             zorigin;		 		/* Slice number corresponding atlas z-origin */
	char           *sw_revision;	 		/* Software version which created this file */
	char           *filetime;		 		/* Creation time of this file */
	char           *gen_data;		 		/* Whatever */
	char            scanner[MAXTOKEN];	 	/* Scanner name for archive path */
	char            acquisition[MAXTOKEN];	 /* Acquisition name for archive path */
	char            reconstruction[MAXTOKEN];/* Reconstruction name for archive path */
	char            filtername[MAXTOKEN];	 /* Filter name for archive path */
}   MainHeader;

typedef struct subheader {
	int             number_format;	 		/* Data type */
	int             file_format;
	int             ndim;
	int             dim1;			 		/* X dimension (pixels) */
	int             dim2;			 		/* Y dimension (pixels) */
	int             dim3;			 		/* Z dimension (slices) */
	int             ring_diff;
	int             span;
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
	float           decay_corr;
	float           sample_size_1;			/* cm */
	float           sample_size_2;			/* cm */
	float           sample_size_3;			/* cm */
	int             start_time;
	int             duration;		 		/* msec */
	int             storage_order;
	int             filter_code;
	float           filter_cutoff_frequency;
	float           filter_resolution;
	float           filter_ramp_slope;
	float           filter_order;
	float           filter_scatter_fraction;
	float           filter_scatter_slope;
	int				processing_code;
	char           *comment;		 		/* User defined comment. */
}   SubHeader;

typedef struct image_file {
	char           *name;		/* Filename */
	FILE           *fp;			/* file pointer (for Interfile and PETT VI) */
	char            mode[3];	/* file access mode 'r','w','a' */
	MainHeader     *mh;			/* generic mainheader */
	SubHeader      *sh;			/* generic subheader */
	IFheader       *ih;		 	/* Interfile header */
	M6header       *m6;		 	/* Matrix 6 header */
	M7header       *m7;		 	/* Matrix 7 header */
	PETT6HDR       *p6;		 	/* PETT VI header */
}   ImageFile;

typedef struct image_index {
	int            frm;			/* frame number */
	int            pln;			/* plane number */
	int            gate;		/* gate number */
	int            data;		/* data number */
	int            bed;			/* bed position */
}   Index;

#endif	/* image_dot_h_defined */
