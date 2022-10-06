/* $Log: get_imglist.c,v $
 * Revision 1.4  2001/05/31  19:33:21  tom
 * initialize nfiles
 *
 * Revision 1.3  2000/07/18  15:46:03  tom
 * Interfile
 *
 * Revision 1.2  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 1.1  1997/03/19  22:32:43  tom
 * Initial revision
 * */
/*	===============================================================================
 *	Module:			get_imglist.c
 *	Date:			Mar-97
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
 *		imglist  -   structure IMAGE_LIST_DATA (imgutil.h)
 *		lstsize  -   number of scan (pairs) in image list
 *		scan     -   scan filenames
 *	===============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <myvalues.h>			 /* ERROR, MAXPATH */
#include <imgutil.h>			 /* IMAGE_LIST_DATA, PARAMETER_DATA, get_path */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/get_imglist.c,v 1.4 2001/05/31 19:33:21 tom Exp $";
#endif

int            get_imglist (prmfile, filtmode, prm, imglist, lstsize, scan)
	char           *prmfile;		 /* parameter filenames */
	int             filtmode;		 /* filter mode (used to expand imagelist)*/
	PARAMETER_DATA *prm;			 /* parameter structure (returned) */
	IMAGE_LIST_DATA *imglist;		 /* imagelist structure (returned) */
	int             lstsize;		 /* number of scan (pairs) in image list (returned) */
	char         ***scan;			 /* scan filenames (returned) */

{
	char          **imgname;
	char           *dtype;
	int             nfiles = 0;
	int             i, j;

/*
 * Read parameters into prm
 */

	if (readprm (prmfile, prm) == ERROR) {
		fprintf (stderr, "ERROR [get_imglist]: reading %s\n", prmfile);
		exit (ERROR);
	}

/*
 * Get full names of activation and control files.
 */

	imgname = (char **) malloc (6 * sizeof (char *));
	for (i = 0; i < 6; i++)
		imgname[i] = (char *) malloc (MAXPATH * sizeof (char));
	dtype = (char *) malloc (MAXPATH * sizeof (char));

	for (i = 0; i < lstsize; i++) {
		if (build_imgnames (prm, &imglist[i], filtmode, &nfiles, imgname) == ERROR) {
			fprintf (stderr, "ERROR [get_imglist] building filenames from %s %s %f %s %f %s\n",
				 imglist[i].subject_id, imglist[i].scanset1_id, imglist[i].mean1,
				 imglist[i].scanset2_id, imglist[i].mean2, imglist[i].sxrid);
			exit (ERROR);
		}
		if (nfiles < 6)
			for (j = nfiles; j < 6; j++)
				(void) strcpy (scan[i][j], "");
		(void) strcpy (dtype, prm->scanner);
		(void) strcat (dtype, "_");
		(void) strcat (dtype, prm->acquisition);
		(void) strcat (dtype, "_");
		(void) strcat (dtype, prm->reconstruction);
		(void) strcat (dtype, "_");
		(void) strcat (dtype, prm->recfilter);
		for (j = 0; j < nfiles; j++) {
			scan[i][j] = getpath (imgname[j], dtype, prm);
			if (strcmp (scan[i][j],"") == 0)
				exit (ERROR);
		}
	}
	for (i = 0; i < 6; i++)
		free (imgname[i]);
	free (imgname);
	free (dtype);
	return (OK);
}
