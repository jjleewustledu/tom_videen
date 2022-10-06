/* $Log: build_fname.c,v $
 * Revision 1.2  1996/09/25  18:29:46  tom
 * use imgfilter
 *
 * Revision 1.1  1996/08/16  19:46:01  tom
 * Initial revision
 *
 */
/* ===============================================================================
 *  Module:         build_fname.c
 *  Date:           Jul-93
 *  Author:         Tom Yang
 *  Description:    Construct a filtered filename from rawname and filter specs.
 *  History:
 *      Aug-96  TOV Modified.
 * ===============================================================================
 */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutilOLD/RCS/build_fname.c,v 1.2 1996/09/25 18:29:46 tom Exp $";

#endif

#include <stdio.h>
#include <string.h>
#include <myvalues.h>
#include <imgutil.h>

void            build_fname (rawname, fullname, prm)
	char           *rawname;		 /* name of image file without filter */
	char           *fullname;		 /* name of image file including filter id */
	PARAMETER_DATA *prm;			 /* parameters */
{
	char            fsuffix[MAXFNAME];
	char           *pstr;

	if ((pstr = strrchr (rawname, '/')) != NULL)
		strcpy (fullname, pstr + 1);
	else
		strcpy (fullname, rawname);

	if ((pstr = strrchr (fullname, '.')) != NULL)
		strcpy (pstr, "\0");

    strcat (fullname, prm->suffix);

	if (prm->imgfilter_type != FILTER_NONE) {
		filter_suffix (prm->imgfilter_type, prm->imgfilter_cutoff, prm->imgfilter_order, fsuffix);
		strcat (fullname, fsuffix);
	}
	strcat (fullname, prm->extension);
}
