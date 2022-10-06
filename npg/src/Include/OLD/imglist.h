/*$Id: imglist.h,v 1.3 1996/02/27 21:29:55 tom Exp $*/
/*$Log: imglist.h,v $
 * Revision 1.3  1996/02/27  21:29:55  tom
 * *** empty log message ***
 *
 * Revision 1.30  1995/05/22  19:34:03  ty7777
 * Changed the MAX_IMAGE_LIST_SIZE from 200 to 3000.
 *
 * Revision 1.29  1995/05/19  14:29:53  ty7777
 * Added IFH_EXT.
 *
 * Revision 1.28  1995/04/18  16:27:28  ty7777
 * Same as the last one.
 *
 * Revision 1.27  1995/04/17  18:22:24  ty7777
 * Added voi_sd.
 *
 * Revision 1.26  1995/01/03  20:39:29  ty7777
 * Added weight to the image list structure.
 *
 * Revision 1.25  1994/09/28  15:28:32  ty7777
 * Added savelist ().
 *
 * Revision 1.24  1994/09/26  16:04:40  ty7777
 * Added constants TOP_SXR and BOT_SXR.
 *
 * Revision 1.23  1994/04/14  15:40:20  ty7777
 * Change the NONDECAY_HALFLIFE from 999999.00 to 0.00.
 *
 * Revision 1.22  1994/04/13  15:20:02  ty7777
 * Added well counts.
 *
 * Revision 1.21  1994/04/08  15:24:58  ty7777
 * Added gethdrfile.
 *
 * Revision 1.20  1994/04/08  14:55:41  ty7777
 * Added NONDECAY_HALFLIFE.
 *
 * Revision 1.19  1994/04/08  14:48:18  ty7777
 * Added ProcessImage () and ProcessOxygen ().
 *
 * Revision 1.18  1994/04/04  21:54:29  ty7777
 * Added voi_norm.
 *
 * Revision 1.17  1994/03/25  22:25:34  ty7777
 * Introduces imglist_mode and deleted the weld_lstmode.
 *
 * Revision 1.16  1994/03/23  16:42:01  ty7777
 * Added HDR_EXT.
 *
 * Revision 1.15  1994/03/22  16:10:29  ty7777
 * Added ducomentation for halflife.
 *
 * Revision 1.14  1994/03/22  16:01:34  ty7777
 * Added half life and tissue_activity.
 *
 * Revision 1.13  1994/03/18  23:16:14  ty7777
 * Moved PROCESSING_NONE to the end.
 *
 * Revision 1.12  1994/03/03  20:53:27  ty7777
 * Replace id with header.
 *
 * Revision 1.11  1994/03/03  17:07:08  ty7777
 * Added constants and structures for processing.
 *
 * Revision 1.10  1994/02/25  21:56:28  ty7777
 * Same as before.
 *
 * Revision 1.9  1994/02/25  21:54:36  ty7777
 * Changed dimension to size.
 *
 * Revision 1.8  1994/02/25  20:17:27  ty7777
 * Added dimension and voi_shape to VOIDATA.
 *
 * Revision 1.7  1994/02/22  20:45:46  ty7777
 * Introduce the VOI related contants.
 *
 * Revision 1.6  1994/02/22  20:36:27  ty7777
 * Added voi_shape, voi_xdim, voi_ydim, and voi_zdim to PARAMETER_DATA.
 *
 * Revision 1.5  1994/02/21  21:45:39  ty7777
 * Added REG_EXT.
 *
 * Revision 1.4  1994/02/21  21:40:50  ty7777
 * Added VOIDATA.
 *
 * Revision 1.3  1994/02/02  21:17:12  ty7777
 * Added atlfile name.
 *
 * Revision 1.2  1994/01/10  21:42:54  ty7777
 * Changed the MAX_MATRIX_LIST_SIZE.
 *
 * Revision 1.1  1993/11/11  17:47:51  ty7777
 * Initial revision
 **/

/*
 * General definition and includes for the image list checking software
 * Creator: Tom Yang
 * Date: 06/30/93
 */

#ifndef IMGLIST_INCLUDED
#define IMGLIST_INCLUDED

#ifndef lint
static char     rcsid_imglist[] = "$Header: /home/npggw/tom/include/RCS/imglist.h,v 1.3 1996/02/27 21:29:55 tom Exp $";
#endif

/* #include <petutil.h> */
/*	#include <arc.h> */

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
#define TOP_SXR_ID		"top"		 /* top sxr id */
#define BOT_SXR_ID		"bot"		 /* bottom sxr id */
#define MASK_CONTROL		0		 /* use control image as mask image */
#define MASK_SPECIFIED		1		 /* mask image is specified */
#define FILTER_NONE			0		 /* filter is none */
#define FILTER_GAUSS		1		 /* filter is gauss */
#define FILTER_BUTTERWORTH	2       /* filter is butterworth */
#define MAX_IMAGE_LIST_SIZE	2000    /* maximum image list size */
#define CUTOFF_MAGNIFIER	10	    /* cutoff 0.2 will generate _g2 or _?b2 */
#define STDIMAGE_MODE		0       /* standard image mode */
#define DIFIMAGE_MODE		1		/* difference image mode */
#define PREWELDED_IMAGE_MODE	2	/* pre-welded image mode */
#define MULTITRACER_IMAGE_MODE	3	/* multi-tracer image mode */
#define WELDED_IMAGE_MODE	4		/* welded image mode, p2000ho1w2.img */
#define LITERAL_IMAGE_MODE	5		/* literal image mode */
#define RECFILTER_MODE		0		/* use recfilter */
#define IMGFILTER_MODE		1		/* use imgfilter */
#define MSKFILTER_MODE		2		/* use mskfilter */
#define NO_ALIGNMENT		"n"		/* no alignment for rmsimg */
#define ALIGNMENT			"a"		/* alignment for rmsimg */
#define ACTIVATION_SCAN		0		/* activation (positive) scan indicator */
#define CONTROL_SCAN		1		/* control (negative) scan indicator */
#define	MAX_MATRIX_LIST_SIZE	32	/* maximum number of list in a matrix file */
#define	VOI_UNDEFINED		0		/* the voi is not defined */
#define	VOI_CUBE			1		/* the voi is cube shaped */
#define	VOI_SPHERE			2		/* the voi is sphere shaped */
#define	PROCESSING_CBF		0		/* processing: cbf, used in voimean */
#define	PROCESSING_CBV		1		/* processing: cbv, used in voimean */
#define	PROCESSING_TISACT	2		/* processing: tissue_activity, used in voimean */
#define	PROCESSING_WELLCOUNTS	3	/* processing: well counts, used in voimean */
#define	PROCESSING_OEF		4		/* processing: oef, used in voimean */
#define	PROCESSING_CMRO2	5		/* processing: cmro2, used in voimean */
#define	PROCESSING_NONE		6		/* processing: none, used in voimean */
#define	REGIONCOL			1		/* rgt_format: regioncol, used in voimean */
#define	REGIONROW			2		/* rgt_format: regionrow, used in voimean */
#define	ALL_FRAMES			-99		/* all frame, used in voimean */
#define	NONDECAY_HALFLIFE	0.000	/* an indicator for non decay processing */

/*
 * structures
 */
typedef char    SHORT_FILE_NAME[MAXLINE]; /* structure for short file name string. */

typedef struct {
	char            subject_id[MAXTOKEN]; /* subject id, e.g. 2000 */
	char            scanset1_id[MAXTOKEN];/* scanset1 id (activation scan number) */
	float           mean1;                /* scanset1 mean  */
	char            scanset2_id[MAXTOKEN];/* scanset2 id (control scan number) */
	float           mean2;                /* scanset2 mean */
	float           weight;               /* weighting factor, used by wgtlst and t88mean */
	char            sxrid[MAXTOKEN];      /* sxr file id, e.g., tr1.sxr, top.sxr, bot.sxr */
}        IMAGE_LIST_DATA;          /* structure for processing image list. */

typedef struct {
	char            datapath[MAXPATH];	/* path name for searching */
	char            suffix[MAXTOKEN];	/* suffix after ho1 */
	char            extension[MAXTOKEN];/* input image extension */
	int             first_slice;		/* slice index */
	int             last_slice;		 	/* slice index */
	char            species[MAXTOKEN];	/* species name, either "p" or "m" */
	char            tracer[MAXTOKEN];	/* tracer, ho, co, oo, bu */
	char            ftracer[MAXTOKEN];	/* flow tracer, used in multi-tracer image mode */
	char            vtracer[MAXTOKEN];	/* volume tracer, used in multi-tracer image mode */
	int             category;		 	/* INDIVIDUAL, COMPOSITE, PHANTOM */
	int             datatype;   	 	/* PET_DATA, PET_IMAGE, COMMON_DATA, MRI_DATA, MRI_IMAGE */
	char            scanner[MAXTOKEN];	/* scanner name, pett6, ecat, or 953 */
	char            acquisition[MAXTOKEN];	 /* acquisition, pett6: low, high; ecat: sta, wob, 3d */
	char            reconstruction[MAXTOKEN];/* pett6: fbold, fb, ml, fbdp; ecat: fb, 3dfb */
	char            recfilter[MAXTOKEN];	 /* reconstruction filter */
	int             recfilter_type;		/* FILTER_NONE, FILTER_GAUSS,  FILTER_BUTTERWORTH */
	float           recfilter_cutoff;	/* cutoff frequency for filtered reconstruction image */
	int             recfilter_order;	/* order for Butterworth filtered image */
	int             mskimg_type;		/* mask image type, MASK_CONTROL or MASK_SEPCIFIED */
	char            mskimg[MAXPATH];	/* mask image name */
	int             mskpct;			 	/* mask percentage, in [0, 100] */
	int             mskfilter_type;		/* FILTER_NONE, FILTER_GAUSS, FILTER_BUTTERWORTH */
	float           mskfilter_cutoff;	/* cutoff frequency for mask filter */
	int             mskfilter_order;	/* order for mask Butterworth filter */
	int             imgfilter_type;		/* FILTER_NONE, FILTER_GAUSS, FILTER_BUTTERWORTH */
	float           imgfilter_cutoff;	/* cutoff frequency for image filter */
	int             imgfilter_order;	/* order for image Butterworth filter */
	char            rms_align[MAXTOKEN];/* n, no alignment; a, 6 parameter translation, rotation */
	int             imglist_mode;		/* STD_IMAGE_MODE, DIFIMAGE_MODE, etc */
	char            atlfile[MAXPATH];	/* atlas file name */
	float           voi_norm;		 	/* voi norm */
	int             voi_sd;			 	/* 1: create .sd file; 0: no .sd file */
	int             voi_shape;			/* VOI_CUBE, VOI_SPHERE */
	float           voi_xdim;			/* x dimension of voi in mm */
	float           voi_ydim;		 	/* y dimension of voi in mm */
	float           voi_zdim;		 	/* z dimension of voi in mm */
	int             processing;		 	/* method of processing */
	float           halflife;		 	/* half life of a radio isotope in seconds */
	int             rgt_format;		 	/* rgt file format */
	int             image_frames;		/* specified the number of frames to be used */
	char            regfile[MAXPATH];	/* region file name */
}      PARAMETER_DATA;			/* structure used for reading parameter file */

typedef struct {
	char            scanid[MAXTOKEN];
	float           scale_factor;		 /* scaling factor obtained from rmsmat */
	float           displacement[3];	 /* displacement in the x, y and z axes, in mm */
	float			rotation_angle[3];   /* angle of rotation in radians */
	float           rotation_center[3];	 /* center of rotation, in mm */
}       MATRIX_DATA;		 /* This will define t4 */

typedef struct {
    float           x;
    float           y;
    float           z;
}       POINT_3D;

typedef struct {
	char            voiname[MAXLINE];	/* voi name */
	int             voi_shape;		 	/* VOI_CUBE, VOI_SPHERE */
	POINT_3D   		center;			 	/* center of voi (mm) */
	POINT_3D   		dimension;		 	/* x, y, z dimension of voi (mm) */
}       VOIDATA;			 /* structure used for reading atl and reg files */


/*
 * Global functions
 */
int             CheckDataFile ();
int             ProcessImage ();
int             ProcessOxygen ();
char           *filteredname ();
int             build_shortnames ();
int             checkimg ();
int             checklst ();
int             checksxr ();
int             filterimg ();
int             get_finfo ();
int             gethdrfile ();
int             readatl ();
int             readimglist ();
int             readmat ();
int             readparm ();
char           *retrieve ();
int             savelist ();
int             searchpath ();
int             template ();

#endif						 /* IMGLIST_INCLUDED */
