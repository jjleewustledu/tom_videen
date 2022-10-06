/* $Log: get_input_lists.c,v $
 * Revision 2.1  1996/09/03  18:22:17  tom
 * add imgmode to handle OEF images properly
 *
 * Revision 2.0  1996/05/10  21:58:08  tom
 * new libimage
 * indexing from 1 on images and atlas
 *
 * Revision 1.11  1995/11/10  18:47:55  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.10  1994/10/03  16:17:13  tom
 * standard format with indent
 *
 * Revision 1.9  1994/05/18  15:53:30  tom
 * changed $Id: get_input_lists.c,v 2.1 1996/09/03 18:22:17 tom Exp tom $ to $Header: /home/npggw/tom/src/t88mean/RCS/get_input_lists.c,v 2.1 1996/09/03 18:22:17 tom Exp tom $
 *
 * Revision 1.8  1993/09/29  16:01:52  tom
 * added metabolic processing and hdr files (cbf, cbv, oef, cmor2)
 *
 * Revision 1.7  1993/08/26  17:26:35  tom
 * changed atlas y-dim to used 5 mm between brain and skull
 *
 * Revision 1.6  1993/08/20  20:24:07  tom
 * first version with Talairach 1988 atlas
 *
 * Revision 1.5  1993/08/19  22:55:48  tom
 * a corrected hd6mean-atlas version of t88mean
 *
 * Revision 1.4  1993/08/12  22:58:02  tom
 * equivalent to hd6mean
 *
 * Revision 1.3  1993/08/06  15:16:49  tom
 * reads prm and list files
 *
 * Revision 1.1  1993/08/05  22:22:58  tom
 * revised matrix_mult and eliminated matrix_point_mult
 * */
/*	===============================================================================
 *	Module:			get_input_lists.c
 *	Date:			28-Jul-93
 *	Author:			Tom Videen
 *	Description:	Read an input image list file and an input image parameter file.
 *
 *	Input:
 *		prmfile  -   parameter filename
 *		lstfile  -   image list filename
 *		filtmode -   used to expand the image list
 *			(IMGFILTER_MODE in imgutil.h)
 *
 *	Output:
 *		prm   	 -   structure PARAMETER_DATA (imgutil.h)
 *		list     -   structure IMAGE_LIST_DATA (imgutil.h)
 *		scan     -   scan filenames
 *		sxrfile  -   sxr filenames
 *		airfile  -   air filenames
 *		lstsize  -   number of scan (pairs) in image list
 *
 *	Algorithm:
 *	1)  Read the parameter file into prm.
 *	2)  Read the image list into list.
 *	3)  Get full names of activation  and control scans and store in scan.
 *	4)  Get full names of sxr files and store in sxrfile.
 *	5)  Get full names of air files and store in airfile.
 *
 *	History:		Modification of chklist.c
 *		11-Aug-93:	Changed for new checkimg.c (passes extension)
 *		07-Sep-93:	Added filtmode;
 *					New calls to build_short_names and chklist (eliminate checkimg)
 *					Replace INPUT_FILE_LIST structure with arrays scan and sxrfile (elim list.h)
 *	===============================================================================
 */
#include <stdio.h>
#include <string.h>
#include <values.h>
#include <myvalues.h>			 /* ERROR, MAXPATH */
#include <imgutil.h>			 /* IMAGE_LIST_DATA, PARAMETER_DATA, get_path */
#include <atlas.h>				 /* SCANLIST_ARRAY, SXRLIST_ARRAY */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/t88mean/RCS/get_input_lists.c,v 2.1 1996/09/03 18:22:17 tom Exp tom $";
#endif

void            get_input_lists (prmfile, lstsize, imgmode, filtmode, scan, sxrfile, airfile, prm, imglist,smgair)
	char           *prmfile;
	int             lstsize;
	int             imgmode;		 /* imagelist mode */
	int             filtmode;		 /* filter mode */
	PARAMETER_DATA *prm;			 /* parameters */
	IMAGE_LIST_DATA *imglist;		 /* image list */
	char         ***scan;
	char          **sxrfile;
	char         ***airfile;
	char           *smgair;

{
	char          **imgname;
	char           *sxrname;
	char           *airname;
	char           *dtype;
	int             nfiles = 0;
	int             i, j;

/*
 * Read parameters into prm
 */

	if (readprm (prmfile, prm) == ERROR) {
		fprintf (stderr, "ERROR [get_input_lists]: reading %s\n", prmfile);
		exit (ERROR);
	}
	prm->imglist_mode = imgmode;

/*
 * Get full names of activation, control and .sxr files.
 */

	imgname = (char **) malloc (6 * sizeof (char *));
	for (i = 0; i < 6; i++)
		imgname[i] = (char *) malloc (MAXPATH * sizeof (char));
	sxrname = (char *) malloc (MAXPATH * sizeof (char));
	airname = (char *) malloc (MAXPATH * sizeof (char));
	dtype = (char *) malloc (MAXPATH * sizeof (char));

	for (i = 0; i < lstsize; i++) {
		if (buildnames (prm, imglist[i], filtmode, &nfiles, imgname, sxrname) == ERROR) {
			fprintf (stderr, "ERROR [get_input_lists] building filenames from %s %s %f %s %f %s\n",
				 imglist[i].subject_id, imglist[i].scanset1_id, imglist[i].mean1,
				 imglist[i].scanset2_id, imglist[i].mean2, imglist[i].sxrid);
			exit (ERROR);
		}
		if (nfiles < 6)
			for (j = nfiles; j < 6; j++)
				strcpy (scan[i][j], "");
		strcpy (dtype, prm->scanner);
		strcat (dtype, "_");
		strcat (dtype, prm->acquisition);
		strcat (dtype, "_");
		strcat (dtype, prm->reconstruction);
		strcat (dtype, "_");
		strcat (dtype, prm->recfilter);
		for (j = 0; j < nfiles; j++) {
			scan[i][j] = getpath (imgname[j], dtype, prm);
			if (strcmp (scan[i][j],"") == 0)
				exit (ERROR);
		}
		strcpy (dtype, "data");
		sxrfile[i] = getpath (sxrname, dtype, prm);
		if (strcmp (sxrfile[i],"") == 0)
			exit (ERROR);

/* AIR filenames */

		if (strcmp(smgair,"yes") == 0) {
			for (j = nfiles; j < 6; j++)
				strcpy (airfile[i][j], "");
			strcpy (airname, prm->species);
			strcat (airname, imglist[i].subject_id);
			strcat (airname, prm->tracer);
			strcat (airname, imglist[i].scanset1_id);
			strcat (airname, ".air");
			airfile[i][0] = getpath (airname, dtype, prm);
			if (strcmp (airfile[i][0],"") == 0)
				exit (ERROR);
			if (nfiles == 2) {
				strcpy (airname, prm->species);
				strcat (airname, imglist[i].subject_id);
				strcat (airname, prm->tracer);
				strcat (airname, imglist[i].scanset2_id);
				strcat (airname, ".air");
				airfile[i][1] = getpath (airname, dtype, prm);
				if (strcmp (airfile[i][1],"") == 0)
					exit (ERROR);
			}
		}
	}
	for (i = 0; i < 6; i++)
		free (imgname[i]);
	free (imgname);
	free (sxrname);
	free (airname);
	free (dtype);
}
