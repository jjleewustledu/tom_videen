/* =============================================================================
 *	Function:		padtensor
 *  Date:           Dec-95
 *  Author:         Tom Videen
 *	Description:	Pad tensor
 *	Note:			Copy top and bottom slices to 3 adjacent slices and then
 *					ramp down to zero over next 6 slices.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libnrimg/Pre3.0version/RCS/padtensor.c,v 2.2 2002/08/20 16:40:27 tom Exp $";
#endif

#include <stdio.h>
#include <nrutil.h>

float            ***padtensor (old, od1, od2, od3, nd1, nd2, nd3)
	float		 ***old;
	long			od1, od2, od3;		/* original dimensions */
	long			nd1, nd2, nd3;		/* new dimensions */

{
	float		***new;
	long		   i, j, k;
	long		   ix, jx, kx;	/* offset of new tensor */
	long		   k1, k2;		/* first and last original slice in new */
	float		   f;			/* scaling fraction */
	
	if ((new = f3tensor(1, nd3, 1, nd2, 1, nd1)) ==  NULL) {
		fprintf (stderr, "ERROR [padtensor]: Cannot malloc new tensor\n");
		return (NULL);
	}
	if (nd1 >= od1)
		ix = (nd1-od1)/2;
	else
		ix = 0;
	if (nd2 >= od2)
		jx = (nd2-od2)/2;
	else
		jx = 0;
	if (nd3 >= od3)
		kx = (nd3-od3)/2;
	else
		kx = 0;

	for (k = 1; k <= nd3; k++)
		for (j = 1; j <= nd2; j++)
			for (i = 1; i <= nd1; i++)
				new[k][j][i] = 0.;
	
	for (k = 1; k <= od3; k++)
		for (j = 1; j <= od2; j++)
			for (i = 1; i <= od1; i++)
				new[k + kx][j + jx][i + ix] = old[k][j][i];
	
	if (kx >= 9) {
		k1 = kx+1;
		k2 = kx+od3;
		for (k = 1; k <= 9 ; k++) {
			if (k < 4)
				f = 1.0;
			else
				f = (float) (10-k) / 7.;
			for (j = 1; j <= nd2; j++) {
				for (i = 1; i <= nd1; i++) {
					new[k1-k][j][i] = f * new[k1][j][i];
					new[k2+k][j][i] = f * new[k2][j][i];
				}
			}
		}
	} 
	
	free_f3tensor (old, 1, od3, 1, od2, 1, od1);
	return (new);
}
