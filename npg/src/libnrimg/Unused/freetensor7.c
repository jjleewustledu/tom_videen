/* =============================================================================
 *	Function:		freetensor7
 *  Date:           Dec-95
 *  Author:         Tom Videen
 *	Description:	Free tensor and it main and subheaders.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/petsun19/tom/src/libnrimg/RCS/freetensor7.c,v 1.1 1996/03/12 22:47:27 tom Exp tom $";
#endif

#include <stdio.h>
#include <libimg.h>
#include <nr.h>
#include <nrutil.h>

void            freetensor7 (mh, sh, img, dim)
	MainHeader     *mh;
	SubHeader      *sh;
	float		 ***img;
	float		   *dim;

{
	free_f3tensor(img, 1, dim[2], 1, dim[1], 1, dim[0]);
	free_sh (sh);
	free_mh (mh);
}
