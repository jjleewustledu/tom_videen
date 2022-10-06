/* $Log: get_maplist.c,v $
 * Revision 3.0  2003/08/21  15:16:40  tom
 * 21Aug2003
 *
 * Revision 1.2  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 1.1  1997/03/19  22:32:47  tom
 * Initial revision
 * */
/*	===============================================================================
 *	Module:			get_maplist.c
 *	Date:			19-Mar-97
 *	Author:			Tom Videen
 *	Description:	Create and locate map filenames
 *
 *	Input:
 *		prm   	 - structure PARAMETER_DATA (imgutil.h)
 *		imglist  - structure IMAGE_LIST_DATA (imgutil.h)
 *		lstsize  - number of scans (or pairs) in imglist
 *		ext		 - extension
 *
 *	Output:
 *		mapfile  - map filenames
 *	===============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <myvalues.h>			 /* ERROR, MAXPATH */
#include <imgutil.h>			 /* IMAGE_LIST_DATA, PARAMETER_DATA, get_path */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/get_maplist.c,v 3.0 2003/08/21 15:16:40 tom Exp $";
#endif

int            get_maplist (prm, imglist, lstsize, ext, mapfile)
	PARAMETER_DATA *prm;			 /* parameters */
	IMAGE_LIST_DATA *imglist;		 /* image list */
	int             lstsize;
	char           *ext;
	char          **mapfile;

{
	char            subject[MAXPATH];
	char            mapname[MAXPATH];
	char            dtype[5];
	int             i;

	*mapname = *dtype = '\0';
	(void) strcpy (dtype, "data");
	for (i = 0; i < lstsize; i++) {
		*subject = '\0';
    	(void) strcpy (subject, prm->species);
    	(void) strcat (subject, imglist->subject_id);
    	(void) strcpy (mapname, subject);
    	if (strlen (imglist->sxrid) > 0)
        	(void) strcat (mapname, imglist->sxrid);
    	else
        	(void) strcat (mapname, ext);
		mapfile[i] = getpath (mapname, dtype, prm);
		if (strcmp (mapfile[i],"") == 0)
			exit (ERROR);
	}
	return (OK);
}
