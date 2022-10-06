/*	$Id: c3tensor.c,v 1.5 1999/12/13 18:06:58 tom Exp $
	$Log: c3tensor.c,v $
 * Revision 1.5  1999/12/13  18:06:58  tom
 * tov revision
 *
 * Revision 1.4  1995/07/28  15:09:44  tom
 * *** empty log message ***
 *
 * Revision 1.3  1995/07/28  14:53:07  tom
 * *** empty log message ***
 *
 * Revision 1.2  1995/07/28  14:46:01  tom
 * debugged
 *
 * Revision 1.1  1995/07/24  20:53:14  tom
 * Initial revision
 * */
/*	========================================================================
 *	Module:			c3tensor.c
 *	Date:			28-Jul-95
 *	Author:			Tom Videen
 *	Description:	Allocate a 3D tensor of 8-bit char
 *					with range t[nzl..nzh][nyl..nyh][nxl..nxh]
 *	========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libmatrix/Pre2.0version/RCS/c3tensor.c,v 1.5 1999/12/13 18:06:58 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#define NR_END 1

char        ***c3tensor (nzl, nzh, nyl, nyh, nxl, nxh)
	long            nzl, nzh, nyl, nyh, nxl, nxh;

{
	long            i, j;
	long            npln = nzh - nzl + 1;
	long            nrow = nyh - nyl + 1;
	long            ncol = nxh - nxl + 1;
	char        ***t;

/* allocate pointers to pointers to planes */

	t = (char ***) malloc ((unsigned int) ((npln + NR_END) * sizeof (char **)));
	if (!t) {
		fprintf (stdout, "ERROR: allocation failure 1 in c3tensor()\n");
		exit (1);
	}
	t += NR_END;
	t -= nzl;

/* allocate pointers to rows and set pointers to them */

	t[nzl] = (char **) malloc ((unsigned int) ((npln * nrow + NR_END) * sizeof (char *)));
	if (!t[nzl]) {
		fprintf (stdout, "ERROR: allocation failure 2 in c3tensor()\n");
		exit (1);
	}
	t[nzl] += NR_END;
	t[nzl] -= nyl;

/* allocate rows and set pointers to them */

	t[nzl][nyl] = (char *) malloc ((unsigned int) ((npln * nrow * ncol + NR_END) * sizeof (char)));
	if (!t[nzl][nyl]) {
		fprintf (stdout, "ERROR: allocation failure 3 in c3tensor()\n");
		exit (1);
	}
	t[nzl][nyl] += NR_END;
	t[nzl][nyl] -= nxl;

	for (j = nyl + 1; j <= nyh; j++)
		t[nzl][j] = t[nzl][j - 1] + ncol;
	for (i = nzl + 1; i <= nzh; i++) {
		t[i] = t[i - 1] + nrow;
		t[i][nyl] = t[i - 1][nyl] + nrow * ncol;
		for (j = nyl + 1; j <= nyh; j++)
			t[i][j] = t[i][j - 1] + ncol;
	}

/* return pointer to array of pointers to planes */

	return t;
}
