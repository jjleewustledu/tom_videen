/*	$Id: filteredname.c,v 3.0 2003/08/21 15:16:40 tom Exp $
	$Log: filteredname.c,v $
 * Revision 3.0  2003/08/21  15:16:40  tom
 * 21Aug2003
 *
 * Revision 2.3  2002/12/17  22:01:04  tom
 * fix pointer
 *
	Revision 2.2  1999/12/13 17:04:17  tom
	tov revision

 * Revision 2.1  1999/05/07  15:59:48  tom
 * add FILTER_GAUSS2D
 *
 * Revision 2.0  1996/04/24  23:07:41  tom
 * new library release
 *
 * Revision 1.1  1996/03/12  23:00:34  tom
 * Initial revision
 * */
/* ===============================================================================
 *	Module:			filteredname.c
 *	Date:			Dec-95
 *	Author:			Tom Videen
 *	Description:	Construct a filtered filename from a filename plus
 *					filter parameters.
 * ===============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/filteredname.c,v 3.0 2003/08/21 15:16:40 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <imgutil.h>
#include <myvalues.h>

char            *filteredname (fname, ftype, cutoff, order)
	char           *fname;
	int             ftype;
	float           cutoff;
	int             order;
{
	char           *newname;
	char           *cbuffer;
	char           *extension;
	char           *ptr;

	newname = (char *) malloc (MAXPATH);
	cbuffer = (char *) malloc (MAXPATH);

	if ((ptr = strrchr (fname, '/')) != NULL)
		(void) strcpy (newname, ptr + 1);
	else
		(void) strcpy (newname, fname);

	extension = strrchr (newname, '.');
	(void) strcpy (extension, NULL);

    if (ftype == FILTER_GAUSS) {
        (void) strcat (newname, "_g");
    	itoa ((int) (cutoff * 10), cbuffer);
    	(void) strcat (newname, cbuffer);
    } else if (ftype == FILTER_GAUSS2D) {
        (void) strcat (newname, "_2dg");
    	itoa ((int) (cutoff * 10), cbuffer);
    	(void) strcat (newname, cbuffer);
    } else if (ftype == FILTER_BUTTERWORTH) {
        (void) strcat (newname, "_");
    	itoa (order, cbuffer);
    	(void) strcat (newname, cbuffer);
    	itoa ((int) (cutoff * 10), cbuffer);
    	(void) strcat (newname, cbuffer);
    }

	extension = strrchr (fname, '.');
	if (extension != NULL)
		(void) strcat (newname, extension);
	free (cbuffer);
	return (newname);
}
