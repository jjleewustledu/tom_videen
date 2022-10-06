/* =============================================================================
 *	Function:		trimtensor
 *  Date:           Dec-95
 *  Author:         Tom Videen
 *	Description:	Trim tensor
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libnrimg/Pre3.0version/RCS/trimtensor.c,v 2.1 1999/12/13 18:21:08 tom Exp $";
#endif

#include <stdio.h>
#include <nrutil.h>

float            ***trimtensor (old, od1, od2, od3, nd1, nd2, nd3)
	float		 ***old;
	long			od1, od2, od3;		/* original dimensions */
	long			nd1, nd2, nd3;		/* new dimensions */

{
	float		***new;
	long		   i, j, k;
	long		   ix, jx, kx;	/* offset of new tensor */
	
	if ((new = f3tensor(1, nd3, 1, nd2, 1, nd1)) ==  NULL) {
		fprintf (stderr, "ERROR [trimtensor]: Cannot malloc new tensor\n");
		return (NULL);
	}
	if (od1 >= nd1)
		ix = (od1-nd1)/2;
	else
		ix = 0;
	if (od2 >= nd2)
		jx = (od2-nd2)/2;
	else
		jx = 0;
	if (od3 >= nd3)
		kx = (od3-nd3)/2;
	else
		kx = 0;

	for (k = 1; k <= nd3; k++)
		for (j = 1; j <= nd2; j++)
			for (i = 1; i <= nd1; i++)
				new[k][j][i] = old[k + kx][j + jx][i + ix];
	
	free_f3tensor (old, 1, od3, 1, od2, 1, od1);
	return (new);
}
