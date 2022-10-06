/*	$Id: info2comment.c,v 1.2 1999/12/13 17:45:50 tom Exp $
	$Log: info2comment.c,v $
	Revision 1.2  1999/12/13 17:45:50  tom
	tov revision

 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* ===============================================================================
 *	Function:		info2comment
 *	Date:			Oct-93
 *	Author:			Tom Yang
 *	Description:	Fills a comment field for an ECAT file main header
 *					from info in the MainHeader structure.
 *	History:
 *		Mar-96	TOV modification
 * ===============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/info2comment.c,v 1.2 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <string.h>
#include <image.h>

void            info2comment (mh, new_comment)
	MainHeader     *mh;			 		/* MainHeader */
	char           *new_comment;		/* comment field */
{
	int             first;
	char            comments[MAXLINE];
	char            scaling_factor[MAXLINE];
	char            zorigin[MAXLINE];

	first = TRUE;
	(void) strcpy (comments, "");
	if (mh->orientation == 0) {
		(void) strcpy (comments, XFLIP_LABEL);
		first = FALSE;
	}
	if (mh->scale_flag == 1) {
		(void) sprintf (scaling_factor, "%s %f", SCALE_LABEL, mh->pr_scaler);
		if (! first) {
			(void) strcat (comments, " ");
			(void) strcat (comments, scaling_factor);
		} else
			(void) strcpy (comments, scaling_factor);
		first = FALSE;
	}
	if (mh->zorigin > 0) {
		(void) sprintf (zorigin, "%s %d", ZORIGIN_LABEL, mh->zorigin);
		if (! first) {
			(void) strcat (comments, " ");
			(void) strcat (comments, zorigin);
		} else
			(void) strcpy (comments, zorigin);
		first = FALSE;
	}
	if (mh->pr_filter == 7) {
		if (! first) {
			(void) strcat (comments, " ");
			(void) strcat (comments, GAUSS_FILTER_LABEL);
		} else
			(void) strcpy (comments, GAUSS_FILTER_LABEL);
		first = FALSE;
	}
	if (mh->pr_filter == 8) {
		if (! first) {
			(void) strcat (comments, " ");
			(void) strcat (comments, BUTTERWORTH_FILTER_LABEL);
		} else {
			(void) strcpy (comments, BUTTERWORTH_FILTER_LABEL);
		}
		first = FALSE;
	}
	if (mh->pett6_flag) {
		if (! first) {
			(void) strcat (comments, " ");
			(void) strcat (comments, PETTVI_IMAGE_LABEL);
		} else
			(void) strcpy (comments, PETTVI_IMAGE_LABEL);
	}
	(void) strncpy (new_comment, comments, 32);
	return;
}
