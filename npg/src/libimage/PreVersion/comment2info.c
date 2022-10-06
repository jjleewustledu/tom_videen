/*	$Id: comment2info.c,v 1.2 1999/12/13 17:45:50 tom Exp $
	$Log: comment2info.c,v $
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* ===============================================================================
 *	Function:		comment2info
 *	Date:			Oct-93
 *	Author:			Tom Yang
 *	Description:	Fills MainHeader structure fields from the comment field
 *	History:
 *		Mar-96	TOV modification
 * ===============================================================================
 */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/comment2info.c,v 1.2 1999/12/13 17:45:50 tom Exp $";

#endif

#include <stdio.h>
#include <string.h>
#include <image.h>

void            comment2info (mh, comment)
	MainHeader     *mh;			 /* MainHeader */
	char           *comment;		 /* study_description */
{
	char            label[MAXLINE];
	char           *pattern;
	float           scaling_factor = 1;
	int             zorigin = 0;

/* Set default values */

	mh->orientation = 1;
	mh->scale_flag = 0;
	mh->pr_scaler = 1.0;
	mh->pr_filter = 0;
	mh->pett6_flag = 0;
	mh->zorigin = 0;

	if (comment && *comment) {
		pattern = strstr (comment, XFLIP_LABEL);
		if (pattern && *pattern)
			mh->orientation = 0;
		pattern = strstr (comment, SCALE_LABEL);
		if (pattern && *pattern && sscanf (pattern, "%s %f", label, &scaling_factor) == 2) {
			mh->scale_flag = 1;
			mh->pr_scaler = scaling_factor;
		}
		pattern = strstr (comment, GAUSS_FILTER_LABEL);
		if (pattern && *pattern)
			mh->pr_filter = 7;
		pattern = strstr (comment, BUTTERWORTH_FILTER_LABEL);
		if (pattern && *pattern)
			mh->pr_filter = 8;
		pattern = strstr (comment, PETTVI_IMAGE_LABEL);
		if (pattern && *pattern)
			mh->pett6_flag = 1;
		pattern = strstr (comment, ZORIGIN_LABEL);
		if (pattern && *pattern && sscanf (pattern, "%s %d", label, &zorigin) == 2)
			mh->zorigin = zorigin;
	}
	return;
}
