/* $Log: fillimglist.c,v $
 * Revision 2.3  2001/05/31  19:33:21  tom
 * initialize nfiles
 *
 * Revision 2.2  2000/07/18  15:46:09  tom
 * *** empty log message ***
 *
 * Revision 2.1  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 2.0  1996/04/24  23:07:41  tom
 * new library release
 *
 */
/*	===============================================================================
 *	Module:			fillimglist.c
 *	Date:			Apr-96
 *	Author:			Tom Videen
 *	Description:	Complete filenames in imglist.
 *	Input:
 *		prm   	 -   structure PARAMETER_DATA (imgutil.h)
 *		list     -   structure IMAGE_LIST_DATA (imgutil.h)
 *		lstsize  -   number of scan (pairs) in image list
 *		filtmode -   used to expand the image list (defined in imgutil.h)
 *			RECFILTER_MODE   0  use recfilter
 *			IMGFILTER_MODE   1  use imgfilter
 *			MSKFILTER_MODE   2  use mskfilter
 *	Output:
 *		scan     -   scan filenames
 *	===============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/fillimglist.c,v 2.3 2001/05/31 19:33:21 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <myvalues.h>			 /* ERROR, MAXPATH */
#include <imgutil.h>			 /* IMAGE_LIST_DATA, PARAMETER_DATA */

int            fillimglist (prm, imglist, lstsize, filtmode, scan)
	PARAMETER_DATA *prm;			 /* parameters */
	IMAGE_LIST_DATA *imglist;		 /* image list */
	int             lstsize;		 /* number of scan file (pairs) */
	int             filtmode;		 /* filter mode */
	char         ***scan;			 /* list of complete scan filenames */
{
	char          **imgname;
	char           *sxrname;
	char           *dtype;
	int             nfiles = 0;
	int             i, j;

/*
 * Get full names of activation, control and .sxr files.
 */

	imgname = (char **) malloc (6 * sizeof (char *));
	for (i = 0; i < 6; i++)
		imgname[i] = (char *) malloc (MAXPATH * sizeof (char));
	sxrname = (char *) malloc (MAXPATH * sizeof (char));
	dtype = (char *) malloc (MAXPATH * sizeof (char));

	for (i = 0; i < lstsize; i++) {
		if (buildnames (prm, &imglist[i], filtmode, &nfiles, imgname, sxrname) == ERROR) {
			fprintf (stderr, "ERROR [fillimglist] building filenames from %s %s %f %s %f %s\n",
				 imglist[i].subject_id, imglist[i].scanset1_id, imglist[i].mean1,
				 imglist[i].scanset2_id, imglist[i].mean2, imglist[i].sxrid);
			return (ERROR);
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
				return (ERROR);
		}
		(void) strcpy (dtype, "data");
	}
	for (i = 0; i < 6; i++)
		free (imgname[i]);
	free (imgname);
	free (sxrname);
	free (dtype);
	return (OK);
}
