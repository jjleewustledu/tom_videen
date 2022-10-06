/* $Log: sxr_xcenter.c,v $
 * Revision 2.0  1996/05/10  21:59:12  tom
 * new libimage
 * indexing from 1 on images and atlas
 *
 * Revision 1.10  1995/11/10  18:48:18  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.9  1994/10/03  16:17:13  tom
 * standard format with indent
 *
 * Revision 1.8  1994/05/18  15:53:30  tom
 * changed $Id: sxr_xcenter.c,v 2.0 1996/05/10 21:59:12 tom Exp $ to $Header: /home/npggw/tom/src/t88mean/RCS/sxr_xcenter.c,v 2.0 1996/05/10 21:59:12 tom Exp $
 * */
/*	========================================================================
 *	Module:			sxr_xcenter.c
 *	Date:			18-Aug-93
 *	Author:			Tom Videen
 *	Description:	Find the mean right-left center of a set of PET slices
 *		(defined in the structure "sxr").
 *		For PETT VI (or < 16 slices), throw out the top and bottom slice;
 *		For ECAT, throw out the top 4 and bottom 4 slices.
 *
 *	Input:
 *		sxr	-	sxr file structure
 *
 *	Output:
 *		center[0]	-	mean right-left (x) center of the PET image
 *
 *	Algorithm:
 *	1)  If number_slices < 16, skip the top and bottom slices
 *	otherwise skip the top 3 and bottom 3 slices.
 *	2)  Compute the mean rlcenter of the included PET slices.
 *
 *	Programming Note:  sxr file has rlcenter in FORTRAN-array indices (1,2,....)
 *	       The center returned used to have a C-array index (0,1,...)
 *	       It was change (May 96) to have a Numerical Recipes array index (1,2,....)
 *	========================================================================
 */
#include <stdio.h>
#include <imgutil.h>				 /* SXR_DATA */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/t88mean/RCS/sxr_xcenter.c,v 2.0 1996/05/10 21:59:12 tom Exp $";

#endif

/*	-----------------------
 *	Function sxr_xcenter
 *	-----------------------
 */
void            sxr_xcenter (sxr, center)
	SXR_DATA        sxr;			 /* information from sxr file */
	float          *center;

{
	float           xctr;
	int             first, last;
	int             i;

	if (sxr.number_slices < 16) {
		first = 1;
		last = sxr.number_slices - 1;
	} else {
		first = 4;
		last = sxr.number_slices - 4;
	}

	xctr = 0;

	for (i = first; i < last; ++i)
		xctr += sxr.rlcenter[i];

	center[0] = xctr / (last - first);
}
