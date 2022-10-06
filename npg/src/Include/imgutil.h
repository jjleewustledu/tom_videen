/*  $Log: imgutil.h,v $
 * Revision 1.9  1996/09/25  18:31:06  tom
 * add build_mskname
 *
 * Revision 1.8  1996/08/16  20:36:05  tom
 * add build_mnames
 *
 * Revision 1.7  1996/08/16  19:24:55  tom
 * add build_fname
 *
 * Revision 1.6  1996/07/17  22:54:32  tom
 * add MAP_EXT
 *
 * Revision 1.5  1996/05/23  15:28:18  tom
 * add petinfo
 *
 * Revision 1.4  1996/04/25  16:08:55  tom
 * *** empty log message ***
 *
 * Revision 1.3  1996/04/04  14:07:21  tom
 * *** empty log message ***
 *
 * Revision 1.2  1996/03/07  15:51:28  tom
 * *** empty log message ***
 *
 * Revision 1.1  1996/03/05  22:43:25  tom
 * Initial revision
 *
    $Id: imgutil.h,v 1.9 1996/09/25 18:31:06 tom Exp tom $
 */
/* =============================================================================
 *  Module:         imgutil.h
 *  Date:           Feb-96
 *  Author:         Tom Videen
 *  Description:    Prototypes for routines in libimgutil.a
 *	Library:		libimgutil.a
 * =============================================================================
 */
#ifndef imgutil_dot_h_defined
#define imgutil_dot_h_defined

#ifndef	lint
static char     imgutil_rcsid[] = "$Header: /usr/local/npg/include/RCS/imgutil.h,v 1.9 1996/09/25 18:31:06 tom Exp tom $";
#endif

#ifndef myvalues_dot_h_defined
#include <myvalues.h>
#endif

/*
 * constants
 */
#define DIFF_EXT		".dif"		 /* difference image extension */
#define HDR_EXT			".hdr"		 /* header file extension */
#define IFH_EXT			".ifh"		 /* ifh file extension */
#define IMAGE_EXT		".img"		 /* PET image extension */
#define REC_EXT			".rec"		 /* rec file extension */
#define	REG_EXT			".reg"		 /* reg file extension */
#define SXR_EXT			".sxr"		 /* sxr file extension */
#define MAP_EXT			".map"		 /* map file extension */
#define TOP_SXR_ID		"top"		 /* top sxr id */
#define BOT_SXR_ID		"bot"		 /* bottom sxr id */
#define MASK_CONTROL		0		 /* use control image as mask image */
#define MASK_SPECIFIED		1		 /* mask image is specified */
#define FILTER_NONE			0	 	 /* filter is none */
#define FILTER_GAUSS		1		 /* filter is gauss */
#define FILTER_BUTTERWORTH	2		 /* filter is butterworth */
#define MAX_IMAGE_LIST_SIZE	2000	 /* maximum image list size */
#define CUTOFF_MAGNIFIER	10		 /* cutoff 0.2 will generate _g2 or _?b2 */
#define STDIMAGE_MODE		0		 /* standard image mode */
#define DIFIMAGE_MODE		1		 /* difference image mode */
#define PREWELDED_IMAGE_MODE	2	 /* pre-welded image mode */
#define MULTITRACER_IMAGE_MODE	3	 /* multi-tracer image mode */
#define WELDED_IMAGE_MODE	4		 /* welded image mode, p2000ho1w2.img */
#define LITERAL_IMAGE_MODE	5		 /* literal image mode */
#define RECFILTER_MODE		0		 /* use recfilter */
#define IMGFILTER_MODE		1		 /* use imgfilter */
#define MSKFILTER_MODE		2		 /* use mskfilter */
#define ACTIVATION_SCAN		0		 /* activation (positive) scan indicator */
#define CONTROL_SCAN		1		 /* control (negative) scan indicator */
#define	MAX_MATRIX_LIST_SIZE	32	 /* maximum number of list in a matrix file */
#define	VOI_UNDEFINED		0		 /* the voi is not defined */
#define	VOI_CUBE			1	  	 /* the voi is cube-shaped */
#define	VOI_SPHERE			2	 	 /* the voi is sphere-shaped */
#define	VOI_CYLINDER		3	 	 /* the voi is cylindrical-shaped */
#define	PROC_NONE			0		 /* processing: none, used in voimean */
#define	PROC_CBF			1		 /* processing: cbf, used in voimean */
#define	PROC_CBV			2		 /* processing: cbv, used in voimean */
#define	PROC_OEF			3		 /* processing: oef, used in voimean */
#define	PROC_CMRO2			4		 /* processing: cmro2, used in voimean */
#define	PROC_DECAYCOR		5		 /* processing: decay-corrected tissue_activity */
#define	PROC_UNCORRECTED	6	 	 /* processing: uncorrected tissue_activity */
#define	REGIONCOL			1	 	 /* rgt_format: regioncol, used in voimean */
#define	REGIONROW			2	 	 /* rgt_format: regionrow, used in voimean */
#define	ALL_FRAMES			-99	 	 /* all frame, used in voimean */
#define	NONDECAY_HALFLIFE	0.000	 /* an indicator for non decay processing */
#define TYPE_STRING    0    	 	 /* data contained in string is a string */
#define TYPE_INTEGER   1    	 	 /* data contained in string is an integer */
#define TYPE_REAL      2    	 	 /* data contained in string is a real number */
#define PEAK_PER_SLICE          1    /* peak per slice type */
#define PEAK_PER_SCAN           2    /* peak per scan type */
#define PEAK_SLICE_UNDEFINED    -1   /* the peak slice is undefined */
#define XRAY_TYPE_RADIOLOGY     20   /* xray 3.0 with ECAT images not flipped */
#define XRAY_TYPE_NEUROLOGY     21   /* xray 3.1 with ECAT images flipped */
#define XRAY_TYPE_XRAY32        22   /* xray 3.2 with PET peak slice */
#define ECAT_NUM_SLICES         31   /* Default ECAT number of slices */
#define ENCODED_SLICE_WIDTH     128
#define DEFAULT_XORIGIN      64.5   /* default xorigin */
#define DEFAULT_YORIGIN      64.5   /* default yorigin */
#define DEFAULT_ZORIGIN      31  	/* default zorigin */
#define PROGRAM_XRAY30       "xray 3.0"
#define PROGRAM_XRAY31       "xray 3.1"
#define PROGRAM_XRAY32       "xray 3.2"


/*
 * structures
 */
typedef char    SHORT_FILE_NAME[MAXLINE];	 /* structure for short file name string. */

typedef struct {
	char            subject_id[MAXTOKEN];	 /* subject id, e.g. 2000 */
	char            scanset1_id[MAXTOKEN];	 /* scanset1 id (activation scan number) */
	float           mean1;			 		 /* scanset1 mean  */
	char            scanset2_id[MAXTOKEN];	 /* scanset2 id (control scan number) */
	float           mean2;			 		 /* scanset2 mean */
	float           weight;			 		 /* weighting factor, used by wgtlst and t88mean */
	char            sxrid[MAXTOKEN];	 	 /* sxr or map file id, e.g., tr1.sxr, top.sxr, bot.sxr */
}               IMAGE_LIST_DATA;			 /* structure for processing image list. */

typedef struct {
	char            datapath[MAXPATH];	 /* path name for searching */
	char            suffix[MAXTOKEN];	 /* suffix after ho1 */
	char            extension[MAXTOKEN]; /* input image extension */
	int             first_slice;		 /* slice index */
	int             last_slice;		 	 /* slice index */
	char            species[MAXTOKEN];	 /* species name, either "p" or "m" */
	char            tracer[MAXTOKEN];	 /* tracer, ho, co, oo, bu */
	char            ftracer[MAXTOKEN];	 /* flow tracer, used in multi-tracer image mode */
	char            vtracer[MAXTOKEN];	 /* volume tracer, used in multi-tracer image mode */
	char            category[MAXTOKEN];	 /* IP, IMR, PUB */
	char            scanner[MAXTOKEN];	 /* scanner name, pett6, ecat, or 953 */
	char            acquisition[MAXTOKEN]; /* acquisition, pett6: low, high; ecat: sta, wob, 3d */
	char            reconstruction[MAXTOKEN];/* pett6: fbold, fb, ml, fbdp; ecat: fb, 3dfb */
	char            recfilter[MAXTOKEN];	 /* reconstruction filter */
	int             recfilter_type;		 /* FILTER_NONE, FILTER_GAUSS,  FILTER_BUTTERWORTH */
	float           recfilter_cutoff;	 /* cutoff frequency for filtered reconstruction image */
	int             recfilter_order;	 /* order for Butterworth filtered image */
	int             mskimg_type;		 /* mask image type, MASK_CONTROL or MASK_SEPCIFIED */
	char            mskimg[MAXPATH];	 /* mask image name */
	int             mskpct;			 	 /* mask percentage, in [0, 100] */
	int             mskfilter_type;		 /* FILTER_NONE, FILTER_GAUSS, FILTER_BUTTERWORTH */
	float           mskfilter_cutoff;	 /* cutoff frequency for mask filter */
	int             mskfilter_order;	 /* order for mask Butterworth filter */
	int             imgfilter_type;		 /* FILTER_NONE, FILTER_GAUSS, FILTER_BUTTERWORTH */
	float           imgfilter_cutoff;	 /* cutoff frequency for image filter */
	int             imgfilter_order;	 /* order for image Butterworth filter */
	int             imglist_mode;		 /* STD_IMAGE_MODE, DIFIMAGE_MODE, etc */
	char            atlfile[MAXPATH];	 /* atlas file name */
	float           voi_norm;		 	 /* voi norm */
	int             voi_sd;			 	 /* 1: create .sd file; 0: no .sd file */
	int             voi_shape;		 	 /* VOI_CUBE, VOI_SPHERE */
	float           voi_xdim;			 /* x dimension of voi in mm */
	float           voi_ydim;		 	 /* y dimension of voi in mm */
	float           voi_zdim;		 	 /* z dimension of voi in mm */
	int             processing;		 	 /* method of processing */
	float           halflife;		 	 /* half life of a radio isotope in seconds */
	int             rgt_format;		 	 /* rgt file format */
	int             image_frames;		 /* specified the number of frames to be used */
	char            regfile[MAXPATH];	 /* region file name */
}               PARAMETER_DATA;			 /* structure used for reading parameter file */

typedef struct {
	int             type;			 	 /* SXR file type: xray 3.0, 20; 3.1, 21; xray 3.2, 22 */
	BOOLEAN         incorrect_dz;		 /* TRUE: header contains "incorrect delta-z" */
	int             peaktype;		 	 /* scan peak or slice peak */
	int             peakslice;		 	 /* slice (>= 1) which has the peak value */
	int             threshold;		 	 /* threshold value */
	float           pixel_size;		 	 /* pixel size (cm / pixel) */
	float           magnification_factor;/* magnification factor for the skull X ray */
	int             number_slices;		 /* number of slices */
	float           slice_separation;	 /* PET slice separation (cm) */
	int             reference_slice;	 /* PET reference slice from which measures are made */
	int             init_slice;		 	 /* reference slice number at time of initialization */
	float           zatlas_dimension;	 /* vertical dimension from center of AC-PC in atlas (cm) */
	float           apatlas_dimension;	 /* length of the AC-PC in the atlas (cm) */
	float           film_position;		 /* Film position */
	float           scan_position;		 /* Scan position */
	float           reference_offset;	 /* offset from reference slice, mm */
	float           xscale;			 	 /* x scaling factor */
	float           init_offset;		 /* initialization offset */
	float           apxray;			 	 /* length of the AC-PC line on the skull xray (cm) */
	float           degx;			 	 /* angle between PET slice and AC-PC line (degree) */
	float           zxray;			 	 /* distance from the center of the AC-PC to the
						  				  * vertex of the skull xray (cm) */
	float           dz;			 	 	 /* distance from the center of the AC-PC to
						  				  * reference slice using a line perpendicular to
						  				  * reference slice (cm) */
	float           dap;			 	 /* distance from the A-P center of the reference
						  				  * slice to the intersection point of the line
						  				  * perpendicular to the reference slice passing
						  				  * the center of AC-PC and the reference slice (cm) */
	float           apcenter;		 	 /* coordinate of A-P center for reference slice (pixel) */
	float           ear_separation;		 /* Right-Left separation of the ear markers (cm) */
	float           eardap;			 	 /* distance between the ear markers in the skull
						  				  * xray (cm) parallel to the AC-PC line */
	float           eardz;			 	 /* distance between the ear markers in the skull
						  				  * xray (cm) perpendicular to the AC-PC line */
	float           degy;			 	 /* angle of rotation about the y-axis (degree),
						  				  * computed from eardz and earsep */
	float           degz;			 	 /* angle of rotation about the z-axis (degree),
						  				  * computed from eardap and earsep */
	float          *rlpetdim;		 	 /* Right-Left dimension of PET slices (pixel) */
	float          *rlcenter;		 	 /* center of the right-left axis of PET slices (pixel) */
	float          *vertical_petdim;	 /* Top-Bottom dimension of PET slices (pixel) */
	float          *rlslicedim;		 	 /* Right-Left dimension of slices (cm) in Xray */
}               SXR_DATA;			 /* structure for skull X ray output */

/* 
 * Header file header, used in hdrinfo.
 */
typedef struct {
	char	scanner [14];
	char	scanname [8];
	char	scandate [8];
	short	slices;
	short	scantime;
	char	compound [10];
	char	filter [12];
	short	rcontype;
	short	resolution;
	char	procdate [8];
	char	initials [4];
	short	ntype;
	short	unused_1;
	char	piename [8];
	int		totalcnts;
	int		scancnts;
	int		scanst;
	int		scanlen;
	int		framelen;
	int		tau;
	int		pettcnv1;
	int		aflow1;
	int		bflow1;
	int		bvfactor1;
	int		aoxygen1;
	int		boxygen1;
	int		awater1;
	int		bwater1;
	int		o2cnts1;
	int		oxycont1;
	int		decay_factor1;
	int		pettcnv2;
	int		aflow2;
	int		bflow2;
	int		bvfactor2;
	int		aoxygen2;
	int		boxygen2;
	int		awater2;
	int		bwater2;
	int		o2cnts2;
	int		oxycont2;
	int		decay_factor2;
	int		pieslope;
	int		efactor;
	short	unused_2 [26];
} OS32_HEADER_DATA;		/* real numbers are represented in int format */

typedef struct {
	char	scanner [14];
	char	scanname [8];
	char	scandate [8];
	short	slices;
	short	scantime;
	char	compound [10];
	char	filter [12];
	short	rcontype;
	short	resolution;
	char	procdate [8];
	char	initials [4];
	short	ntype;
	short	unused_1;
	char	piename [8];
	float	totalcnts;
	float	scancnts;
	float	scanst;
	float	scanlen;
	float	framelen;
	float	tau;
	float	pettcnv1;
	float	aflow1;
	float	bflow1;
	float	bvfactor1;
	float	aoxygen1;
	float	boxygen1;
	float	awater1;
	float	bwater1;
	float	o2cnts1;
	float	oxycont1;
	float	decay_factor1;
	float	pettcnv2;
	float	aflow2;
	float	bflow2;
	float	bvfactor2;
	float	aoxygen2;
	float	boxygen2;
	float	awater2;
	float	bwater2;
	float	o2cnts2;
	float	oxycont2;
	float	decay_factor2;
	float	pieslope;
	float	efactor;
	short	unused_2 [26];
} HEADER_DATA;		/* real numbers are IEEE reals */

typedef struct {
	char            scanid[MAXTOKEN];
	float           scale_factor;		 /* scaling factor obtained from rmsmat */
	float           displacement[3];	 /* displacement in the x, y and z axes, in mm */
	float           rotation_angle[3];	 /* angle of rotation in radians */
	float           rotation_center[3];	 /* center of rotation, in mm */
}               MATRIX_DATA;			 /* This will define t4 */

typedef struct {
	float           x;
	float           y;
	float           z;
}               POINT_3D;

typedef struct {
	char            voiname[MAXLINE];	 /* voi name */
	int             voi_shape;		 	 /* VOI_CUBE, VOI_SPHERE */
	POINT_3D        center;			 	 /* center of voi (mm) */
	POINT_3D        dimension;		 	 /* x, y, z dimension of voi (mm) */
}               VOIDATA;			 /* structure used for reading atl and reg files */

/*	Functions */

int             buildnames ();
void            build_fname ();
void            build_mskname ();
int             build_mnames ();
int             filterimg ();
char           *filteredname ();
void            filter_suffix ();
void            freesxr ();
void            getatlas_hd6 ();
void            getatlas_t88 ();
int             get_finfo ();
int             gethdr ();
void            gethdrpath ();
char           *getpath ();
SXR_DATA       *getsxr ();
char           *getuser ();
void            intdate ();
void            itoa ();
void            lowercase ();
int             petinfo ();
int             processImage ();
int             processOxygen ();
int             readimglist ();
int             readprm ();
void			recout ();
int             savelist ();
int             savesxr ();
int             stringtype ();
int             writehdr ();

/*
 * macros
 */
#define ROUND(x)    (short int) ((x) > 0 ? ((x) + 0.5) : ((x) - 0.5)) /* round to int */
#define XtoSlice(x) DEFAULT_XORIGIN + ((x) / 2.0)       /* convert X value to slice number */
#define YtoSlice(y) DEFAULT_YORIGIN - ((y) / 2.0)       /* convert Y value to slice number */
#define ZtoSlice(z,z0) ROUND (z0 - z *0.5)				/* convert Z value to slice number */

#endif
