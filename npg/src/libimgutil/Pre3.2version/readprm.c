/*	$Id: readprm.c,v 3.0 2003/08/21 15:16:40 tom Exp $
	$Log: readprm.c,v $
 * Revision 3.0  2003/08/21  15:16:40  tom
 * 21Aug2003
 *
 * Revision 2.13  2002/12/12  21:46:16  tom
 * change MAXFLOAT to HUGE
 *
	Revision 2.12  1999/12/13 17:04:17  tom
	tov revision

 * Revision 2.11  1999/10/12  13:15:52  tom
 * *** empty log message ***
 *
 * Revision 2.10  1999/07/15  15:42:38  tom
 * message for sphere
 *
 * Revision 2.9  1999/07/15  15:38:07  tom
 * reject sphere
 *
 * Revision 2.8  1999/07/15  15:11:54  tom
 * change VOI_SPHERE to VOI_OLD_SPHERE or VOI_NEW_SPHERE
 *
 * Revision 2.7  1999/06/30  16:57:04  tom
 * add PROC_ODIV
 *
 * Revision 2.6  1999/05/07  15:59:48  tom
 * add FILTER_GAUSS2D
 *
 * Revision 2.5  1998/09/18  13:47:04  tom
 * add scanner 962
 *
 * Revision 2.4  1998/05/27  19:12:19  tom
 * add voi_order
 *
 * Revision 2.3  1997/06/02  20:05:01  tom
 * add voi_zero_chk
 *
 * Revision 2.2  1997/01/03  15:52:30  tom
 * add fb and fbu to recfilter types
 *
 * Revision 2.1  1996/09/03  15:44:35  tom
 * add 2d
 *
 * Revision 2.0  1996/04/24  23:07:41  tom
 * new library release
 *
 * Revision 1.1  1996/03/12  23:00:34  tom
 * Initial revision
 * */
/* ===============================================================================
 *	Module:			readprm.c
 *	Date:			May-95
 *	Author:			Tom Yang
 *	Description:	Read an image parameter file.
 * ===============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/readprm.c,v 3.0 2003/08/21 15:16:40 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <imgutil.h>

#define LOW_RESOLUTION	0
#define HIGH_RESOLUTION	1
#define STATIONARY		2
#define WOBBLE			3
#define THREE_DIM		4
#define TWO_DIM			5
#define FBOLD			0
#define FB				1
#define ML				2
#define FBDGP			3
#define FBP3D			4
#define FBP3DJMON		5
#define FBP3DJMOS		6

static char    *acquisition_string[] = {"low",
	"high",
	"sta",
	"wob",
	"3d",
"2d"};

static char    *reconstruction_string[] = {"fbold",
	"fb",
	"ml",
	"fbdgp",
	"3dbp",
	"3dbpjmon",
"3dbpjmos"};

static void     error_message ();
static void     error_message (keyword, parameter, file)
	char           *keyword;
	char           *parameter;
	char           *file;
{
	fprintf (stderr, "ERROR [readprm]: Incorrect parameter (%s) for keyword (%s) in %s\n",
		 parameter, keyword, file);
}

int             readprm (prmfile, prm)
	char           *prmfile;
	PARAMETER_DATA *prm;
{
	FILE           *fp;
	FILE           *pathfp;
	char            label[MAXLINE];
	char            line[MAXLINE];
	char            original_parameter[MAXLINE];
	char            parameter[MAXLINE];
	char            parameter2[MAXLINE];
	char            parameter3[MAXLINE];
	float           x = 0;
	float           y = 0;
	float           z = 0;
	int             first_slice = 0;
	int             last_slice = 0;

	fp = fopen (prmfile, "r");
	if (fp == (FILE *) NULL) {
		fprintf (stderr, "ERROR [readprm]: cannot open parameter file %s\n", prmfile);
		return (ERROR);
	}
/*
 * Initialization
 */
	(void) strcpy (prm->datapath, "");
	(void) strcpy (prm->suffix, "");
	(void) strcpy (prm->extension, "");
	prm->first_slice = 0;
	prm->last_slice = 0;
	(void) strcpy (prm->species, "");
	(void) strcpy (prm->tracer, "");
	(void) strcpy (prm->ftracer, "");
	(void) strcpy (prm->vtracer, "");
	(void) strcpy (prm->category, "IP");
	(void) strcpy (prm->scanner, "");
	(void) strcpy (prm->acquisition, "");
	(void) strcpy (prm->reconstruction, "");
	(void) strcpy (prm->recfilter, "");
	prm->recfilter_type = 0;
	prm->recfilter_cutoff = 0;
	prm->recfilter_order = 0;
	prm->mskimg_type = 0;
	(void) strcpy (prm->mskimg, "");
	prm->mskpct = 0;
	prm->mskfilter_type = 0;
	prm->mskfilter_cutoff = 0;
	prm->mskfilter_order = 0;
	prm->imgfilter_type = 0;
	prm->imgfilter_cutoff = 0;
	prm->imgfilter_order = 0;
	prm->imglist_mode = STDIMAGE_MODE;
	(void) strcpy (prm->atlfile, "");
	prm->voi_norm = 0.0;
	prm->voi_sd = FALSE;
	prm->voi_shape = VOI_UNDEFINED;
	prm->voi_xdim = 0;
	prm->voi_ydim = 0;
	prm->voi_zdim = 0;
	prm->voi_order = 0;
	prm->voi_mode = 0;
	prm->processing = PROC_NONE;
	prm->halflife = HUGE;
	prm->rgt_format = REGIONCOL;
	prm->image_frames = 1;
	(void) strcpy (prm->regfile, "");
	prm->voi_zero_chk = FALSE;

	while (fgets (line, MAXLINE, fp) != NULL) {
		if (sscanf (line, "%s %s", label, parameter) == 2) {
			if (strncmp (label, "#", 1) == 0)
				continue;

			(void) strcpy (original_parameter, parameter);
			lowercase (label);
			lowercase (parameter);

			if (strncmp (label, "datapath:", 9) == 0) {
				(void) strcpy (parameter2, original_parameter);
				(void) strcat (parameter2, "/.");
				pathfp = fopen (parameter2, "r");
				if (pathfp != (FILE *) NULL)
					(void) strcpy (prm->datapath, original_parameter);
				else {
					fprintf (stderr, "ERROR [readprm]: datapath %s does not exist\n",
						 parameter);
					return (ERROR);
				}
				continue;
			}
			if (strncmp (label, "suffix:", 7) == 0 && strlen (original_parameter) > 0) {
				(void) strcpy (prm->suffix, original_parameter);
				continue;
			}
			if (strncmp (label, "extension:", 10) == 0
			    && strlen (original_parameter) > 0) {
				if (strlen (original_parameter) > 0
				    && strncmp (parameter, ".", 1) != 0) {
					(void) strcpy (prm->extension, ".");
					(void) strcat (prm->extension, original_parameter);
				} else
					(void) strcpy (prm->extension, original_parameter);
				continue;
			}
			if (strncmp (label, "slices:", 8) == 0) {
				if (sscanf (parameter, "%dto%d", &first_slice, &last_slice) == 2) {
					if (first_slice <= last_slice) {
						prm->first_slice = first_slice;
						prm->last_slice = last_slice;
					} else {
						prm->first_slice = last_slice;
						prm->last_slice = first_slice;
					}
				}
				continue;
			}
			if (strncmp (label, "species:", 8) == 0) {
				(void) strcpy (prm->species, parameter);
				continue;
			}
			if (strncmp (label, "tracer:", 7) == 0) {

				/*
				 * if (strncmp (parameter, "ho", 2) == 0 || strncmp (parameter,
				 * "co", 2) == 0 || strncmp (parameter, "oo", 2) == 0 || strncmp
				 * (parameter, "bu", 2) == 0)
				 */
				(void) strcpy (prm->tracer, original_parameter);
				continue;
			}
			if (strncmp (label, "f-tracer:", 9) == 0) {
				(void) strcpy (prm->ftracer, original_parameter);
				continue;
			}
			if (strncmp (label, "v-tracer:", 9) == 0) {
				(void) strcpy (prm->vtracer, original_parameter);
				continue;
			}
			if (strncmp (label, "category:", 9) == 0) {
				if (strncmp (parameter, "pet", 3) == 0)
					(void) strcpy (prm->category, "IP");
				else if (strncmp (parameter, "individual", 10) == 0)
					(void) strcpy (prm->category, "IP");
				else if (strncmp (parameter, "mri", 3) == 0)
					(void) strcpy (prm->category, "IMR");
				else if (strncmp (parameter, "pub", 3) == 0)
					(void) strcpy (prm->category, "PUB");
				else {
					error_message (label, parameter, prmfile);
					return (ERROR);
				}
				continue;
			}
			if (strncmp (label, "scanner:", 8) == 0) {
				if (strncmp (parameter, "pett6", 5) == 0 ||
				    strncmp (parameter, "953", 3) == 0 ||
				    strncmp (parameter, "961", 3) == 0 ||
				    strncmp (parameter, "962", 3) == 0)
					(void) strcpy (prm->scanner, parameter);
				else if (strncmp (parameter, "ecat", 4) == 0)
					(void) strcpy (prm->scanner, "953");
				else {
					error_message (label, parameter, prmfile);
					return (ERROR);
				}
				continue;
			}
			if (strncmp (label, "acquisition:", 12) == 0) {
				if (strncmp (parameter, "low_resolution", 14) == 0)
					(void) strcpy (prm->acquisition,
						       acquisition_string[LOW_RESOLUTION]);
				else if (strncmp (parameter, "high_resolution", 15) == 0)
					(void) strcpy (prm->acquisition,
						       acquisition_string[HIGH_RESOLUTION]);
				else if (strncmp (parameter, "stationary", 10) == 0)
					(void) strcpy (prm->acquisition,
						       acquisition_string[STATIONARY]);
				else if (strncmp (parameter, "wobble", 6) == 0)
					(void) strcpy (prm->acquisition,
						       acquisition_string[WOBBLE]);
				else if (strncmp (parameter, "3d", 2) == 0)
					(void) strcpy (prm->acquisition,
						       acquisition_string[THREE_DIM]);
				else if (strncmp (parameter, "2d", 2) == 0)
					(void) strcpy (prm->acquisition,
						       acquisition_string[TWO_DIM]);
				else {
					error_message (label, parameter, prmfile);
					return (ERROR);
				}
				continue;
			}
			if (strncmp (label, "reconstruction:", 15) == 0) {
				if (strncmp (parameter, "filtered_backprojection_old", 27) == 0)
					(void) strcpy (prm->reconstruction,
						       reconstruction_string[FBOLD]);
				else if (strncmp (parameter, "filtered_backprojection", 23) == 0)
					(void) strcpy (prm->reconstruction,
						       reconstruction_string[FB]);
				else if (strncmp (parameter, "maximum_likelihood", 18) == 0)
					(void) strcpy (prm->reconstruction,
						       reconstruction_string[ML]);
				else if (strncmp (parameter,
						  "filtered_backprojection_Dave_Politte", 36) == 0)
					(void) strcpy (prm->reconstruction,
						       reconstruction_string[FBDGP]);
				else if (strncmp (parameter, "3d_filtered_backprojection", 26) == 0)
					(void) strcpy (prm->reconstruction,
						       reconstruction_string[FBP3D]);
				else if (strncmp (parameter, "3d_filtered_backprojection_jmo_norm", 35) == 0)
					(void) strcpy (prm->reconstruction,
						       reconstruction_string[FBP3DJMON]);
				else if (strncmp (parameter, "3d_filtered_backprojection_jmo_scat", 35) == 0)
					(void) strcpy (prm->reconstruction,
						       reconstruction_string[FBP3DJMOS]);
				else if (strncmp (parameter, "fbu", 3) == 0)
					(void) strcpy (prm->reconstruction, "fbu");
				else if (strncmp (parameter, "fb", 2) == 0)
					(void) strcpy (prm->reconstruction, "fb");
				else {
					error_message (label, parameter, prmfile);
					return (ERROR);
				}
				continue;
			}
			if (strncmp (label, "recfilter:", 10) == 0) {
				if (strncmp (parameter, "filterg", 7) == 0 ||
				    strncmp (parameter, "filterf", 7) == 0 ||
				    strncmp (parameter, "ramp", 4) == 0 ||
				    strncmp (parameter, "12", 2) == 0)
					(void) strcpy (prm->recfilter, parameter);

				if ((strncmp (parameter, "gauss", 5) == 0 ||
				     strncmp (parameter, "butterworth", 11) == 0) &&
				    sscanf (line, "%s %s %s", label, parameter, parameter2) == 3) {
					(void) strcpy (prm->recfilter, parameter);

					if (strncmp (parameter, "gauss2d", 7) == 0)
						prm->recfilter_type = FILTER_GAUSS2D;
					else if (strncmp (parameter, "gauss", 5) == 0)
						prm->recfilter_type = FILTER_GAUSS;
					else if (sscanf (line, "%s %s %s %s", label, parameter,
							 parameter2, parameter3) == 4) {
						prm->recfilter_type = FILTER_BUTTERWORTH;
						if (stringtype (parameter3) == TYPE_INTEGER)
							prm->recfilter_order = atoi (parameter3);
						else {
							fprintf (stderr, "ERROR [readprm]: recfilter_order error\n");
							return (ERROR);
						}
					}
					if (stringtype (parameter2) == TYPE_REAL ||
					    stringtype (parameter2) == TYPE_INTEGER)
						prm->recfilter_cutoff = atof (parameter2);
					else {
						fprintf (stderr, "ERROR [readprm]: recfilter_cutoff (%s)\n",
							 parameter2);
						return (ERROR);
					}
				}
				continue;
			}
			if (strncmp (label, "mskimg:", 7) == 0) {
				if (strncmp (parameter, "control_image", 13) == 0) {
					prm->mskimg_type = MASK_CONTROL;
				} else if (strncmp (parameter, "specified", 9) == 0) {
					prm->mskimg_type = MASK_SPECIFIED;
					if (sscanf (line, "%s %s %s", label, parameter, parameter2) == 3) {
						(void) strcpy (prm->mskimg, parameter2);
					}
				} else {
					error_message (label, parameter, prmfile);
					return (ERROR);
				}
				continue;
			}
			if (strncmp (label, "mskpct:", 7) == 0) {
				if (stringtype (parameter) == TYPE_INTEGER)
					prm->mskpct = atoi (parameter);
				else {
					fprintf (stderr, "mskpct parameter error\n");
					return (ERROR);
				}
				continue;
			}
			if (strncmp (label, "mskfilter:", 10) == 0) {
				if ((strncmp (parameter, "gauss", 5) == 0 ||
				     strncmp (parameter, "butterworth", 11) == 0) &&
				    sscanf (line, "%s %s %s", label, parameter, parameter2) == 3) {
					if (strncmp (parameter, "gauss2d", 7) == 0)
						prm->mskfilter_type = FILTER_GAUSS2D;
					else if (strncmp (parameter, "gauss", 5) == 0)
						prm->mskfilter_type = FILTER_GAUSS;
					else if (sscanf (line, "%s %s %s %s", label, parameter,
							 parameter2, parameter3) == 4) {
						prm->mskfilter_type = FILTER_BUTTERWORTH;
						if (stringtype (parameter3) == TYPE_INTEGER)
							prm->mskfilter_order = atoi (parameter3);
						else {
							fprintf (stderr, "mskfilter_order error\n");
							return (ERROR);
						}
					}
					if (stringtype (parameter2) == TYPE_REAL ||
					    stringtype (parameter2) == TYPE_INTEGER)
						prm->mskfilter_cutoff = atof (parameter2);
					else {
						fprintf (stderr, "mskfilter_cutoff error\n");
						return (ERROR);
					}
				}
				continue;
			}
			if (strncmp (label, "imgfilter:", 10) == 0) {
				if ((strncmp (parameter, "gauss", 5) == 0 ||
				     strncmp (parameter, "butterworth", 11) == 0) &&
				    sscanf (line, "%s %s %s", label, parameter, parameter2) == 3) {
					if (strncmp (parameter, "gauss2d", 7) == 0)
						prm->imgfilter_type = FILTER_GAUSS2D;
					else if (strncmp (parameter, "gauss", 5) == 0)
						prm->imgfilter_type = FILTER_GAUSS;
					else if (sscanf (line, "%s %s %s %s", label, parameter,
							 parameter2, parameter3) == 4) {
						prm->imgfilter_type = FILTER_BUTTERWORTH;
						if (stringtype (parameter3) == TYPE_INTEGER)
							prm->imgfilter_order = atoi (parameter3);
						else {
							fprintf (stderr, "imgfilter_order error\n");
							return (ERROR);
						}
					}
					if (stringtype (parameter2) == TYPE_REAL ||
					    stringtype (parameter2) == TYPE_INTEGER)
						prm->imgfilter_cutoff = atof (parameter2);
					else {
						fprintf (stderr, "imgfilter_cutoff error\n");
						return (ERROR);
					}
				}
				continue;
			}
			if (strncmp (label, "imglist_mode:", 13) == 0
			    || strncmp (label, "imgmode:", 8) == 0
			    || strncmp (label, "weld_lstmode:", 13) == 0) {
				if (strncmp (parameter, "standard", 8) == 0) {
					prm->imglist_mode = STDIMAGE_MODE;
				} else if (strncmp (parameter, "diff", 4) == 0) {
					prm->imglist_mode = DIFIMAGE_MODE;
				} else if (strncmp (parameter, "prewelded", 9) == 0) {
					prm->imglist_mode = PREWELDED_IMAGE_MODE;
				} else if (strncmp (parameter, "multitracer", 11) == 0) {
					prm->imglist_mode = MULTITRACER_IMAGE_MODE;
				} else if (strncmp (parameter, "welded", 6) == 0) {
					prm->imglist_mode = WELDED_IMAGE_MODE;
				} else if (strncmp (parameter, "literal", 7) == 0) {
					prm->imglist_mode = LITERAL_IMAGE_MODE;
				}
				continue;
			}
			if (strncmp (label, "atlfile:", 8) == 0) {
				(void) strcpy (prm->atlfile, original_parameter);
				continue;
			}
			if (strncmp (label, "voi_norm:", 9) == 0) {
				if (stringtype (parameter) == TYPE_REAL ||
				    stringtype (parameter) == TYPE_INTEGER)
					prm->voi_norm = atof (parameter);
				else {
					fprintf (stderr, "ERROR [readprm]: voi_norm (%s)\n", parameter);
					return (ERROR);
				}
				continue;
			}
			if (strncmp (label, "voi_sd:", 7) == 0) {
				if (strncmp (parameter, "yes", 3) == 0)
					prm->voi_sd = TRUE;
				continue;
			}
			if (strncmp (label, "voi:", 4) == 0) {
				if (sscanf (line, "%s %s %f %f %f", label, parameter, &x, &y, &z) == 5
				    && strncmp (parameter, "cube", 4) == 0) {
					prm->voi_shape = VOI_CUBE;
					prm->voi_xdim = x;
					prm->voi_ydim = y;
					prm->voi_zdim = z;
				} else if (sscanf (line, "%s %s %f", label, parameter, &x) == 3
					   && strncmp (parameter, "old_sphere", 10) == 0) {
					prm->voi_shape = VOI_OLD_SPHERE;
					prm->voi_xdim = x;
					prm->voi_ydim = x;
					prm->voi_zdim = x;
				} else if (sscanf (line, "%s %s %f", label, parameter, &x) == 3
					   && strncmp (parameter, "new_sphere", 10) == 0) {
					prm->voi_shape = VOI_NEW_SPHERE;
					prm->voi_xdim = x;
					prm->voi_ydim = x;
					prm->voi_zdim = x;
				} else if (sscanf (line, "%s %s %f %f", label, parameter, &x, &z) == 4
					   && strncmp (parameter, "cylinder", 8) == 0) {
					prm->voi_shape = VOI_CYLINDER;
					prm->voi_xdim = x;
					prm->voi_ydim = x;
					prm->voi_zdim = z;
				} else if (sscanf (line, "%s %s %f", label, parameter, &x) == 3
					   && strncmp (parameter, "sphere", 6) == 0) {
					error_message (label, parameter, prmfile);
					fprintf (stderr, "    You must use either 'old_sphere' or 'new_sphere'\n");
					return (ERROR);
				}
				continue;
			}
			if (strncmp (label, "voi_order:", 10) == 0) {
				if (stringtype (parameter) == TYPE_INTEGER)
					prm->voi_order = atoi (parameter);
				else {
					fprintf (stderr, "ERROR [readprm]: voi_order (%s)\n", parameter);
					return (ERROR);
				}
				continue;
			}
			if (strncmp (label, "voi_mode:", 8) == 0) {
				if (strncmp (parameter, "single", 6) == 0)
					prm->voi_mode = 0;
				else if (strncmp (parameter, "paired", 6) == 0)
					prm->voi_mode = 1;
				continue;
			}
			if (strncmp (label, "processing:", 11) == 0) {
				if (strncmp (parameter, "none", 4) == 0)
					prm->processing = PROC_NONE;
				else if (strncmp (parameter, "cbf", 3) == 0)
					prm->processing = PROC_CBF;
				else if (strncmp (parameter, "cbv", 3) == 0)
					prm->processing = PROC_CBV;
				else if (strncmp (parameter, "oef", 3) == 0)
					prm->processing = PROC_OEF;
				else if (strncmp (parameter, "cmro2", 5) == 0)
					prm->processing = PROC_CMRO2;
				else if (strncmp (parameter, "decay-corrected", 15) == 0)
					prm->processing = PROC_DECAYCOR;
				else if (strncmp (parameter, "uncorrected", 11) == 0)
					prm->processing = PROC_UNCORRECTED;
				else if (strncmp (parameter, "odiv", 4) == 0)
					prm->processing = PROC_ODIV;
				continue;
			}
			if (strncmp (label, "halflife:", 9) == 0) {
				prm->halflife = atof (parameter);
				if (prm->halflife >= 999999.00 || prm->halflife <= 0.0)
					prm->halflife = NONDECAY_HALFLIFE;
				continue;
			}
			if (strncmp (label, "rgt_format:", 11) == 0) {
				if (strncmp (parameter, "regioncol", 9) == 0)
					prm->rgt_format = REGIONCOL;
				else if (strncmp (parameter, "regionrow", 9) == 0)
					prm->rgt_format = REGIONROW;
				continue;
			}
			if (strncmp (label, "frames:", 7) == 0) {
				if (strncmp (parameter, "all", 3) == 0)
					prm->image_frames = ALL_FRAMES;
				else if (stringtype (parameter) == TYPE_INTEGER)
					prm->image_frames = atoi (parameter);
				continue;
			}
			if (strncmp (label, "regfile:", 8) == 0) {
				(void) strcpy (prm->regfile, parameter);
			}
			if (strncmp (label, "voi_zero_chk:", 13) == 0) {
				if (strncmp (parameter, "yes", 3) == 0)
					prm->voi_zero_chk = TRUE;
				continue;
			}
		}
	}
	(void) fclose (fp);
	return (OK);
}
