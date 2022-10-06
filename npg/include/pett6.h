/*  $Log: pett6.h,v $
 * Revision 2.0  2004/02/12  20:30:50  tom
 * sync Feb 2004
 *
 * Revision 1.3  2004/01/08  15:36:50  tom
 * *** empty log message ***
 *
 * Revision 1.2  2002/11/27  18:24:38  tom
 * *** empty log message ***
 *
 * Revision 1.1  1996/03/05  22:43:59  tom
 * Initial revision
 *
    $Id: pett6.h,v 2.0 2004/02/12 20:30:50 tom Exp $
 */
/* =============================================================================
 *  Module:         pett6.h
 *  Date:           Nov 1995
 *  Author:         Tom Videen
 *  Description:    Routines for PETT VI image file I/O (libimg.a)
 * =============================================================================
 */
#ifndef pett6_dot_h_defined
#define pett6_dot_h_defined

/*@unused@*/
#ifndef	lint
static char     pett6_rcsid[] = "$Header: /usr/local/npg/include/RCS/pett6.h,v 2.0 2004/02/12 20:30:50 tom Exp $";
#endif

#include <sys/types.h>

#define TRUE        1
#define FALSE       0
#define	RECLEN				256
#define	PETT6_IMAGE			2
#define	VERSION_OLD			1		/* pre-1988 PETT VI reconstruction */
#define VERSION_NEW			2		/* 1988 TOV modification to PETT VI reconstruction */
#define VERSION_ML			3		/* Maximum-likelihood reconstruction */
#define VERSION_DGP			4		/* Dave Politte filtered backprojection */
#define VERSION_ECAT		5		/* ECAT image written in PETT VI format */
#define	PETT6_PIXEL_SIZE	0.286	/* PETT6 image pixel size, cm / pixel */ 
#define	PETT6_SLICE_SEP		1.44	/* PETT6 slice separation, cm */ 
#define	SMG_PIXEL_SIZE		0.2		/* SMG 49 image pixel size, cm / pixel */ 
#define	SMG_SLICE_SEP		0.2		/* SMG 49 image slice separation, cm */ 
#define PETT6_DIM			100	
#define PETT6_SLICE_BYTES	80 * RECLEN	/* bytes per slice */

#define FILTERG             -6		/* used in MainHeader->pr_filter */
#define FILTERF             -5		/* used in MainHeader->pr_filter */

/*	Strings which appear in the study_description field of ECAT main headers */
#define XFLIP_LABEL			"X:r"		/* X axis reversed or right on right */
#define SCALE_LABEL			"SF:"		/* post-recon scaling factor */
#define PETTVI_IMAGE_LABEL	"PETT VI"	/* created from PETT VI file */
#define GAUSS_FILTER_LABEL	"3D Gauss"	/* filtered with 3D gaussian */
#define BUTTERWORTH_FILTER_LABEL "3D Butterworth"	/* Butterworth filtered */
#define ZORIGIN_LABEL		"z0:"		/* slice number for atlas z-origin */

/* 
 * PETT VI Image Header File Format 
 *
 * This C struct represents the structure of PETT VI headers as represented on 
 * Concurrent OS/32 systems.  The headers are repeated before each slice
 * in an image file on the Concurrent system.  Note that floating point 
 * numbers require conversion to IEEE format for use on UNIX systems.
 *
 */

typedef struct {
	char	file_flag [2];		/* "**" standard, "++" condensed */
	char	run_name [6];		/* specify the tracer name: OC1, OC2, OO1, OO2, etc. */
	short	rotation;			/* 0 = no, 1 = yes */
	short	wobble_rate;		/* wobbles per second, normally 1 */
	short	nwobble_pts;		/* number of wobble points, norm 16 */
	short	scan_time;			/* scan time in seconds */
	char	subject_id [8];		/* single / paried */
	char	date [6];			/* MMDDYY */
	short	resolution;			/* 0 = low, 1 = high */
	short	isotope_code;		/* always 1 */
	short	slice_number;		/* 1--49 */
	long	decay_constant;		/* decay constant for isotope, int rep. for float*/
	short	image_orientation;	/* rotation, 0 = old fmt., 1 = new fmt. */
	short	time_per_frame;		/* scan time of 1 frame, in seconds */
	long	total_counts;		/* total counts in slice */
	char	compound [10];		/* isotope used for scan */
	short	unused_1 [2];		/* empty space */
	short	number_of_slices;	/* number of images in series, 7, 14, 32, 49 */
	short	unused_2 [5];		/* empty space */
	short	file_type;			/* type of image file */
	short	ecat_orientation;	/* 1: flipped; others: not flipped */
	short	rotation_increment;	/* 0 = 1/4 angle, 1 = 1/2 angle */
	char	filter [12];		/* filter used in reconstruction */
	short 	unused_4 [2];		/* empty space */
	short	un_norm_average;	/* uncorrected normalization average */
	short	norm_average;		/* normalization average */
	long	total_raw_events;	/* slice total raw events */
	long	total_random_events;/* slice total random events */
	long	raw_random_events;	/* slice total raw - random events */
	short 	unused_5 [13];		/* empty space */
	short 	norm_run_time;		/* scan time in sec for norm. run */
	short 	unused_6 [8];		/* empty space */
	short 	metabolic_data [16];/* Metabolic data storage (DX'ed images only) */
	short 	unused_7 [26];		/* empty space */
	short 	pett_number;		/* PETT number (6) */
	short 	model_number;		/* Model number (0) */
	char	pett_location [12];	/* MALLINCKRODT */
} PETT6HDR;

/* 
 * PETT Encoded Header File Format 
 *
 * This C struct contains the structure of ENCODED headers as represented on 
 * Concurrent OS/32 systems. Note that floating point numbers require 
 * conversion to IEEE format for use on UNIX systems.
 *
 * 07/10/90 M. Todd Gamble
 */

typedef struct {
	u_char 	fileform [10];	/* type of form: ENCODED or not */
	u_short	code;			/* type of encoding  */
	u_short pnum;			/* PETT number (6-8) */
	u_short nslcs;			/* number of slices in file */
	u_short nrecs;			/* number of records in file */
	u_short rec [49];		/* record no.where slice data begins */
	short	unused [70];	/* empty space */
} ENCODEDHDR;

#endif /* pett6_dot_h_defined */
