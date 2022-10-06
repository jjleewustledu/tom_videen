/* $Log: build_mskname.c,v $
 * Revision 3.0  2003/08/21  15:16:40  tom
 * 21Aug2003
 *
 * Revision 1.1  1996/09/25  18:30:09  tom
 * Initial revision
 * */
/* ===============================================================================
 *  Module:         build_mskname.c
 *  Date:           Jul-93
 *  Author:         Tom Yang
 *  Description:    Construct a filtered filename from rawname and mask filter specs.
 * ===============================================================================
 */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/build_mskname.c,v 3.0 2003/08/21 15:16:40 tom Exp $";
#endif

#include <stdio.h>
#include <string.h>
#include <myvalues.h>
#include <imgutil.h>

void            build_mskname (rawname, fullname, prm)
	char           *rawname;		 /* name of image file without filter */
	char           *fullname;		 /* name of image file including filter id */
	PARAMETER_DATA *prm;			 /* parameters */
{
	char            fsuffix[MAXFNAME];
	char           *pstr;

	if ((pstr = strrchr (rawname, '/')) != NULL)
		(void) strcpy (fullname, pstr + 1);
	else
		(void) strcpy (fullname, rawname);

	if ((pstr = strrchr (fullname, '.')) != NULL)
		(void) strcpy (pstr, "\0");

	(void) strcat (fullname, prm->suffix);

	if (prm->mskfilter_type != FILTER_NONE) {
		filter_suffix (prm->mskfilter_type, prm->mskfilter_cutoff, prm->mskfilter_order, fsuffix);
		(void) strcat (fullname, fsuffix);
	}
	(void) strcat (fullname, prm->extension);
}
