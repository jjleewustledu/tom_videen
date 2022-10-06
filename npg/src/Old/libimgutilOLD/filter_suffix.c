/*	$Id: filter_suffix.c,v 2.5 1999/12/13 17:04:17 tom Exp $
	$Log: filter_suffix.c,v $
 * Revision 2.5  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 2.4  1999/05/07  15:59:48  tom
 * add FILTER_GAUSS2D
 *
 * Revision 2.3  1998/01/30  16:50:27  tom
 * permit cutoff of .nnn
 *
 * Revision 2.2  1998/01/30  16:46:56  tom
 * permit cutoffs of .nnn
 *
 * Revision 2.1  1998/01/29  16:22:20  tom
 * fix so that cutoffs of .05 don't blow up
 *
 * Revision 2.0  1996/04/24  23:07:41  tom
 * new library release
 *
 * Revision 1.1  1996/03/12  23:00:34  tom
 * Initial revision
 * */
/* ===============================================================================
 *	Module:			filter_suffix.c
 *	Date:			Jun-95
 *	Author:			Tom Yang
 *	Description:	Creates a suffix for filtered image filenames.
 *	
 * ===============================================================================
 */
#ifndef lint
static char		rcsid[] = "$Header: /home/npggw/tom/src/libimgutilOLD/RCS/filter_suffix.c,v 2.5 1999/12/13 17:04:17 tom Exp $";
#endif 

#include <stdio.h>
#include <string.h>
#include <imgutil.h>

void     filter_suffix (filter_type, cutoff, order, suffix)
	int             filter_type;	/* FILTER_NONE, FILTER_GAUSS, FILTER_BUTTERWORTH */
	float           cutoff;			/* cycles/cm */
	int             order;			/* for Butterworth */
	char           *suffix;			/* suffix returned */
{
	char            cbuffer[20];
	float           magnified_cutoff;
	int             whole_magnified_cutoff;

	if (filter_type == FILTER_GAUSS)
		(void) strcpy (suffix, "_g");
	else if (filter_type == FILTER_GAUSS2D)
		(void) strcpy (suffix, "_2dg");
	else if (filter_type == FILTER_BUTTERWORTH) {
		(void) sprintf (cbuffer, "_%db", order);
		(void) strcpy (suffix, cbuffer);
	} else {
		(void) strcpy (suffix, "");
		return;
	}

	magnified_cutoff = cutoff * 10.;
	if (magnified_cutoff < 1.0) {
		magnified_cutoff = cutoff * 100.;
		(void) strcat (suffix,"0");
	}
	whole_magnified_cutoff = (int) magnified_cutoff;
	while (magnified_cutoff != (float) whole_magnified_cutoff) {
		magnified_cutoff = magnified_cutoff * 10.;
		whole_magnified_cutoff = (int) magnified_cutoff;
	}
	itoa (whole_magnified_cutoff, cbuffer);
	(void) strcat (suffix, cbuffer);
}
