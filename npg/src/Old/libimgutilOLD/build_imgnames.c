/*	$Log: build_imgnames.c,v $
 * Revision 1.2  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 1.1  1997/03/19  22:32:38  tom
 * Initial revision
 * */
/* ===============================================================================
 *	Module:			build_imgnames.c
 *	Date:			Jun-93
 *	Author:			Tom Yang
 *	Description:	Construct short filenames from prm and imglist structures.
 *	History:
 *		Aug-96	TOV	modified buildnames to handle map files instead of SXR files.
 *		Mar-97	TOV	modified eliminate map and sxr files from this routine.
 *
 *	Algorithm:
 *		if (imgmode == STDIMAGE_MODE) then
 *			fname [0]:
 *			<species><subject_id><tracer><scanset1_id><filter>[.<extension>]
 *
 *			fname [1]:
 *			<species><subject_id><tracer><scanset2_id><filter>[.<extension>]
 *		else if (imgmode == DIFIMAGE_MODE)
 *			fname [0]:
 *			<species><subject_id><tracer><scanset1_id>-<scanset2_id><suffix><filter>[.<extension>]
 *		else if (imgmode == PREWELDED_IMAGE_MODE)
 *			fname [0]:
 *			<species><subject_id><tracer><scanset1_n1><filter>[.<extension>]
 *
 *			fname [1]:
 *			<species><subject_id><tracer><scanset1_n2><filter>[.<extension>]
 *
 *			fname [2]:
 *			<species><subject_id><tracer><scanset2_n1><filter>[.<extension>]
 *
 *			fname [3]:
 *			<species><subject_id><tracer><scanset2_n2><filter>[.<extension>]
 *		else if (imgmode == MULTITRACER_IMAGE_MODE)
 *			fname [0]:
 *			<species><subject_id><tracer><scanset1_n1><filter>[.<extension>]
 *
 *			fname [1]:
 *			<species><subject_id><ftracer><scanset1_n2><filter>[.<extension>]
 *
 *			fname [2]:
 *			<species><subject_id><vtracer><scanset1_n3><filter>[.<extension>]
 *
 *			fname [3]:
 *			<species><subject_id><tracer><scanset2_n1><filter>[.<extension>]
 *
 *			fname [4]:
 *			<species><subject_id><ftracer><scanset2_n2><filter>[.<extension>]
 *
 *			fname [5]:
 *			<species><subject_id><vtracer><scanset2_n3><filter>[.<extension>]
 *		else if (imgmode == WELDED_IMAGE_MODE)
 *			fname [0]:
 *			<species><subject_id><tracer><scanset1_id>w<scanset2_id>[.<extension>]
 *		endif
 *
 * ===============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutilOLD/RCS/build_imgnames.c,v 1.2 1999/12/13 17:04:17 tom Exp $";
#endif

#include <stdio.h>
#include <string.h>
#include <imgutil.h>
#include <myvalues.h>

#define	MINUS	"-"
#define	WELD	"w"

int             build_imgnames (prm, imglist, filtmode, nfiles, fname)
	PARAMETER_DATA *prm;
	IMAGE_LIST_DATA *imglist;
	int             filtmode;		 /* IMGFILTER_MODE, MSKFILTER_MODE */
	int            *nfiles;			 /* returned */
	char          **fname;			 /* returned */

{
	int             imgmode;		 /* STDIMAGE_MODE, DIFIMAGE_MODE */
	char            cbuffer[MAXPATH];
	char            extension[MAXPATH];
	char            fsuffix[MAXPATH];
	char            string[MAXPATH];
	char            subject[MAXPATH];
	float           filter_cutoff = 0;
	int             filter_order = 0;
	int             filter_type = 0;
	int             scanset_field[6];
	int             i;
	void            filter_suffix ();

	(void) strcpy (cbuffer, prm->species);
	(void) strcat (cbuffer, imglist->subject_id);
	(void) strcpy (subject, cbuffer);
	(void) strcat (cbuffer, prm->tracer);
	imgmode = prm->imglist_mode;

	if (filtmode == RECFILTER_MODE) {
		filter_type = prm->recfilter_type;
		filter_cutoff = prm->recfilter_cutoff;
		filter_order = prm->recfilter_order;
	} else if (filtmode == IMGFILTER_MODE) {
		filter_type = prm->imgfilter_type;
		filter_cutoff = prm->imgfilter_cutoff;
		filter_order = prm->imgfilter_order;
	} else if (filtmode == MSKFILTER_MODE) {
		filter_type = prm->mskfilter_type;
		filter_cutoff = prm->mskfilter_cutoff;
		filter_order = prm->mskfilter_order;
	}
	if (imgmode == STDIMAGE_MODE) {
		(void) strcpy (fname[0], cbuffer);
		(void) strcat (fname[0], imglist->scanset1_id);
		*nfiles = 1;
		if (strlen (imglist->scanset2_id) > 0) {
			(void) strcpy (fname[1], cbuffer);
			(void) strcat (fname[1], imglist->scanset2_id);
			*nfiles = 2;
		}
	} else if (imgmode == DIFIMAGE_MODE) {
		if (strlen (imglist->scanset2_id) == 0) {
			fprintf (stderr, "ERROR [build_imgnames]: image list is not paired\n");
			return (ERROR);
		}
		(void) strcpy (fname[0], cbuffer);
		(void) strcat (fname[0], imglist->scanset1_id);
		(void) strcat (fname[0], MINUS);
		(void) strcat (fname[0], imglist->scanset2_id);
		*nfiles = 1;
	} else if (imgmode == PREWELDED_IMAGE_MODE) {
		if (sscanf (imglist->scanset1_id, "%dw%d", scanset_field, scanset_field + 1) != 2) {
			fprintf (stderr, "ERROR [build_imgnames]: scanset1_id (%s) is not of the form n1wn2\n",
				 imglist->scanset1_id);
			return (ERROR);
		}
		for (i = 0; i <= 1; i++) {
			(void) strcpy (fname[i], cbuffer);
			itoa (scanset_field[i], string);
			(void) strcat (fname[i], string);
		}
		*nfiles = 2;

		if (strlen (imglist->scanset2_id) > 0) {
			if (sscanf (imglist->scanset2_id, "%dw%d", scanset_field,
				    scanset_field + 1) != 2) {
				fprintf (stderr, "ERROR [build_imgnames]: scanset2_id (%s) is no of the form n1wn2\n",
					 imglist->scanset2_id);
				return (ERROR);
			}
			for (i = 2; i <= 3; i++) {
				(void) strcpy (fname[i], cbuffer);
				itoa (scanset_field[i - 2], string);
				(void) strcat (fname[i], string);
			}
			*nfiles = 4;
		}
	} else if (imgmode == MULTITRACER_IMAGE_MODE) {
		if (sscanf (imglist->scanset1_id, "%df%dv%d",
			    scanset_field, scanset_field + 1, scanset_field + 2) != 3) {
			fprintf (stderr, "ERROR [build_imgnames]: scanset1_id (%s) is not of the form n1fn2vn3\n",
				 imglist->scanset1_id);
			return (ERROR);
		}
		(void) strcpy (fname[0], cbuffer);
		(void) strcpy (fname[1], subject);
		(void) strcat (fname[1], prm->ftracer);
		(void) strcpy (fname[2], subject);
		(void) strcat (fname[2], prm->vtracer);
		for (i = 0; i <= 2; i++) {
			itoa (scanset_field[i], string);
			(void) strcat (fname[i], string);
		}
		*nfiles = 3;

		if (strlen (imglist->scanset2_id) > 0) {
			if (sscanf (imglist->scanset2_id, "%df%dv%d",
				    scanset_field, scanset_field + 1, scanset_field + 2) != 3) {
				fprintf (stderr, "ERROR [build_imgnames]: scanset1_id (%s) is not of the form n1fn2vn3\n",
					 imglist->scanset2_id);
				return (ERROR);
			}
			(void) strcpy (fname[3], cbuffer);
			(void) strcpy (fname[4], subject);
			(void) strcat (fname[4], prm->ftracer);
			(void) strcpy (fname[5], subject);
			(void) strcat (fname[5], prm->vtracer);
			for (i = 3; i <= 5; i++) {
				itoa (scanset_field[i - 3], string);
				(void) strcat (fname[i], string);
			}
			*nfiles = 6;
		}
	} else if (imgmode == WELDED_IMAGE_MODE) {
		if (strlen (imglist->scanset2_id) == 0) {
			fprintf (stderr, "ERROR [build_imgnames]: image list is not paired\n");
			return (ERROR);
		}
		(void) strcpy (fname[0], cbuffer);
		(void) strcat (fname[0], imglist->scanset1_id);
		(void) strcat (fname[0], WELD);
		(void) strcat (fname[0], imglist->scanset2_id);
		*nfiles = 1;
	} else {
		fprintf (stderr, "ERROR [build_imgnames]: Unknown image mode: %d\n", imgmode);
		return (ERROR);
	}

/*
 * Add extension
 */
	if (strlen (prm->extension) == 0 && imgmode == DIFIMAGE_MODE)
		(void) strcpy (extension, DIFF_EXT);
	else
		(void) strcpy (extension, prm->extension);
	for (i = 0; i < *nfiles; i++) {
		(void) strcat (fname[i], prm->suffix);
		if (filter_type != FILTER_NONE) {
			filter_suffix (filter_type, filter_cutoff, filter_order, fsuffix);
			(void) strcat (fname[i], fsuffix);
		}
		(void) strcat (fname[i], extension);
	}
	return (OK);
}
