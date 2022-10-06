/* =============================================================================
 *	Function:		freetensor
 *  Date:           Dec-95
 *  Author:         Tom Videen
 *	Description:	Free tensor and it main and subheaders.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/petsun19/tom/src/libnrimg/RCS/freetensor.c,v 1.1 1995/12/12 15:31:15 tom Exp tom $";
#endif

#include <stdio.h>
#include <libimg.h>
#include <nr.h>
#include <nrutil.h>

void            freetensor (mh, sh, img)
	MainHeader     *mh;
	SubHeader      *sh;
	float		 ***img;

{
	long			dim1, dim2, dim3;

	dim1 = (long) sh->dim1;
	dim2 = (long) sh->dim2;
	dim3 = (long) sh->dim3;
	free_f3tensor(img, 1, dim3, 1, dim2, 1, dim1);
	free_sh (sh);
	free_mh (mh);
}
