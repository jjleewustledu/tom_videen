/* 
 * General definition and includes for the image list checking software
 * Creator: Tom Yang
 * Date: 06/30/93
 */

#ifndef IMGLIST_INCLUDED
#define IMGLIST_INCLUDED

static char rcsid_imglist [] = "$Header: /usr/local/include/petutil/RCS/imglist.h,v 1.30 1995/05/22 19:34:03 ty7777 Exp $";

/*
#include <petutil/archive.h>
*/

/* 
 * constants
 */
#define DIFF_EXT		".dif"	/* difference image extension */
#define HDR_EXT			".hdr"	/* header file extension */
#define IFH_EXT			".ifh"	/* ifh file extension */
#define IMAGE_EXT		".img"	/* PET image extension */
#define REC_EXT			".rec"	/* rec file extension */
#define	REG_EXT			".reg"	/* reg file extension */
#define SXR_EXT			".sxr"	/* sxr file extension */
#define TOP_SXR_ID		"top"	/* top sxr id */
#define BOT_SXR_ID		"bot"	/* bottom sxr id */
#define MASK_CONTROL		0	/* use control image as mask image */
#define MASK_SPECIFIED		1	/* mask image is specified */
#define FILTER_NONE		0	/* filter is none */
#define FILTER_GAUSS		1	/* filter is gauss */
#define FILTER_BUTTERWORTH	2	/* filter is butterworth */
#define MAX_IMAGE_LIST_SIZE	2000	/* maximum image list size */
#define CUTOFF_MAGNIFIER	10	/* cutoff 0.2 will generate _g2 or _?b2 */
#define STDIMAGE_MODE		0	/* standard image mode, p2000ho3_5b7.img, p2000ho1_5b7.img */
#define DIFIMAGE_MODE		1	/* difference image mode, p2000ho3-1a_5b7.img */
#define PREWELDED_IMAGE_MODE	2	/* pre-welded image mode, p2000ho1_5b7.img, p2000ho2_5b7.img, */
					/* p2000ho3_5b7.img, p2000ho4_5b7.img */
#define MULTITRACER_IMAGE_MODE	3	/* multi-tracer image mode, p2000ho1_5b7.img, p2000ho2_5b7.img, */
					/* p2000ho3_5b7.img, p2000ho4_5b7.img, p2000ho5_5b7.img, */
					/* p2000ho6_5b7.img */
#define WELDED_IMAGE_MODE	4	/* welded image mode, p2000ho1w2.img*/
#define LITERAL_IMAGE_MODE	5	/* literal image mode */
#define RECFILTER_MODE		0	/* use recfilter */
#define IMGFILTER_MODE		1	/* use imgfilter */
#define MSKFILTER_MODE		2	/* use mskfilter */
#define NO_ALIGNMENT		"n"	/* no alignment for rmsimg */
#define ALIGNMENT		"a"	/* alignment for rmsimg */
#define ACTIVATION_SCAN		0	/* activation (positive) scan indicator */
#define CONTROL_SCAN		1	/* control (negative) scan indicator */
#define	MAX_MATRIX_LIST_SIZE	32	/* maximum number of list in a matrix file */
#define	VOI_UNDEFINED		0	/* the voi is not defined */
#define	VOI_CUBE		1	/* the voi is cube shaped */
#define	VOI_SPHERE		2	/* the voi is sphere shaped */
#define	PROCESSING_CBF		0	/* processing: cbf, used in voimean */
#define	PROCESSING_CBV		1	/* processing: cbv, used in voimean */
#define	PROCESSING_TISACT	2	/* processing: tissue_activity, used in voimean */
#define	PROCESSING_WELLCOUNTS	3	/* processing: well counts, used in voimean */
#define	PROCESSING_OEF		4	/* processing: oef, used in voimean */
#define	PROCESSING_CMRO2	5	/* processing: cmro2, used in voimean */
#define	PROCESSING_NONE		6	/* processing: none, used in voimean */
#define	REGIONCOL		1	/* rgt_format: regioncol, used in voimean */
#define	REGIONROW		2	/* rgt_format: regionrow, used in voimean */
#define	ALL_FRAMES		-99	/* all frame, used in voimean */
#define	NONDECAY_HALFLIFE	0.000	/* an indicator for non decay processing */


/* 
 * structures
 */
typedef char SHORT_FILE_NAME [MAXLINE];	/* structure for short file name string. */

typedef struct {
	char	subject_id [MAXNAME];	/* subject id, e.g. 2000 */
	char	scanset1_id [MAXNAME];	/* scanset1 id, usually the activation number, e.g., 3 */
	float	mean1;			/* scanset1 mean, usually the mean of activation image, */
					/* e.g., 1367.70 */
	char	scanset2_id [MAXNAME];	/* scanset2 id, usually the control image number, e.g., 2 */
	float	mean2;			/* scanset2 mean, usually the mean of control image, */
					/* e.g., 1364.96 */
	float	weight;			/* weighting factor, used by wgtlst and t88mean */
	char	sxrid [MAXNAME];	/* sxr file id, e.g., tr1.sxr, top.sxr, bot.sxr, etc. */
} IMAGE_LIST_DATA;			/* structure for processing image list. */

typedef struct {
	char	datapath [MAXPATH];	/* path name for searching */
	char	suffix [MAXNAME];	/* suffix after ho1 */
	char	extension [MAXNAME];	/* input image extension */
	int	first_slice;		/* slice index to define the begining of of a range */
	int	last_slice;		/* slice index to define the ending of of a range */
	char	species [MAXNAME];	/* species name, either "p" or "m" */
	char	tracer [MAXNAME];	/* tracer, ho, co, oo, bu */
	char	ftracer [MAXNAME];	/* flow tracer, used in multi-tracer image mode */
	char	vtracer [MAXNAME];	/* volume tracer, used in multi-tracer image mode */
	int	category;		/* INDIVIDUAL, COMPOSITE, PHANTOM */
	int	datatype;		/* PET_DATA, PET_IMAGE, COMMON_DATA, 
					MRI_DATA, MRI_IMAGE */
	char	scanner [MAXNAME];	/* scanner name, pett6, ecat, or 953 */
	char	acquisition [MAXNAME];	/* acquisition, pett6: low, high; ecat: sta, wob, 3d */
	char	reconstruction [MAXNAME];/* pett6: fbold, fb, ml, fbdp; ecat: fb, 3dfb */
	char	recfilter [MAXNAME];	/* reconstruction filter, pett6: filterg, filterf; ecat: ramp */
	int	recfilter_type;		/* FILTER_NONE, FILTER_GAUSS, or FILTER_BUTTERWORTH */
	float	recfilter_cutoff;	/* cutoff frequency for the filtered reconstruction image */
	int	recfilter_order;	/* order for the Butterworth filtered reconstruction image */
	int	mskimg_type;		/* mask image type, MASK_CONTROL or MASK_SEPCIFIED */
	char	mskimg [MAXPATH];	/* mask image name */
	int	mskpct;			/* mask percentage, in [0, 100] */
	int	mskfilter_type;		/* FILTER_NONE, FILTER_GAUSS, or 
					FILTER_BUTTERWORTH */
	float	mskfilter_cutoff;	/* cutoff frequency for mask filter */
	int	mskfilter_order;	/* order for mask Butterworth filter */
	int	imgfilter_type;		/* FILTER_NONE, FILTER_GAUSS, or 
					FILTER_BUTTERWORTH */
	float	imgfilter_cutoff;	/* cutoff frequency for image filter */
	int	imgfilter_order;	/* order for image Butterworth filter */
	char	rms_align [MAXNAME];	/* n, no alignment; a, 6 parameter translation, rotation */
	int	imglist_mode;		/* image list mode. STD_IMAGE_MODE: standard; */
					/* DIFIMAGE_MODE: difference image mode */
					/* PRE_WELDED_IMAGE_MODE: pre-welded image mode */
					/* MULTI_TRACER_IMAGE_MODE: multi-tracer image mode */
					/* WELDED_IMAGE_MODE: welded image mode */
					/* LITERAL_IMAGE_MODE: literal image mode */
	char	atlfile [MAXPATH];	/* atlas file name */
	float	voi_norm;		/* voi norm */
	int	voi_sd;			/* 1: create .sd file; 0: no .sd file */
	int	voi_shape;		/* VOI_CUBE, VOI_SPHERE */
	float	voi_xdim;		/* x dimension of voi in mm */
	float	voi_ydim;		/* y dimension of voi in mm */
	float	voi_zdim;		/* z dimension of voi in mm */
	int	processing;		/* method of processing */
	float	halflife;		/* half life of a radio isotope in seconds */
	int	rgt_format;		/* rgt file format */
	int	image_frames;		/* specified the number of frames to be used */
	char	regfile [MAXPATH];	/* region file name */
} PARAMETER_DATA;			/* structure used for reading parameter file */

typedef struct {
	char	scanid [MAXNAME];	
	float	scale_factor;		/* scaling factor obtained from rmsmat */
	float	displacement [3];	/* displacement in the x, y and z axes, in mm */	
					/* i.e., dx, dy, dz */
	float	rotation_angle [3];	/* angle of rotation about the x, y and z axes, in radians */
					/* i.e., alpha, beta, gamma */
	float	rotation_center [3];	/* center of rotation, in mm */	
					/* i.e., cx, cy, cz */
} MATRIX_DATA;				/* This will define t4 */		

typedef struct {
	char		voiname [MAXLINE];	/* voi name */
	int		voi_shape;		/* VOI_CUBE, VOI_SPHERE */
	FLOAT_POINT3D	center;			/* center of voi, in mm */
	FLOAT_POINT3D	dimension;		/* x, y, z dimension of voi, in mm */
} VOIDATA;					/* structure used for reading atl and reg files. */ 


/* 
 * Global functions
 */
PUBLIC int	CheckDataFile ();
PUBLIC int	ProcessImage ();
PUBLIC int	ProcessOxygen ();
PUBLIC void	build_filter_filename ();
PUBLIC int	build_shortnames ();
PUBLIC int	checkimg ();
PUBLIC int	checklst ();
PUBLIC int	checksxr ();
PUBLIC int	filter_image ();
PUBLIC void	filter_suffix ();
PUBLIC int	get_finfo ();
PUBLIC int	gethdrfile ();
PUBLIC int	readatl ();
PUBLIC int	readimglist ();
PUBLIC int	readmat ();
PUBLIC int	readparm ();
PUBLIC char	*retrieve ();
PUBLIC int	savelist ();
PUBLIC int	searchpath ();
PUBLIC int	template ();

#endif /* IMGLIST_INCLUDED */
