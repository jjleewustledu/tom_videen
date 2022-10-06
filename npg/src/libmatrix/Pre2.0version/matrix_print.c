/*	$Log: matrix_print.c,v $
 * Revision 1.3  1999/12/13  18:06:58  tom
 * tov revision
 *
 * Revision 1.2  1995/07/20  19:14:32  tom
 * processes vectors
 *
 * Revision 1.1  1995/07/20  14:57:22  tom
 * Initial revision
 *
	$Id: matrix_print.c,v 1.3 1999/12/13 18:06:58 tom Exp $ */
/*	========================================================================
 *	Module:			matrix_print.c
 *	Date:			20-Jul-95
 *	Author:			Tom Videen
 *	Description:	Print type float matrix a[m][n]
 *					If n=1, print a vector.
 *	========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libmatrix/Pre2.0version/RCS/matrix_print.c,v 1.3 1999/12/13 18:06:58 tom Exp $";
#endif

#include <stdio.h>

void            matrix_print (a, m, n)
	float            *a;
	int             m, n;
{
	float         **aa;
	int             i, j;

	if (n == 1) {
		for (i = 1; i <= m; i++)
			fprintf (stdout, "%10.4f  ", a[i]);
		fprintf (stdout, "\n\n");
	} else {
		aa = (float **) a;
		for (i = 1; i <= m; i++) {
			for (j = 1; j <= n; j++)
				fprintf (stdout, "%10.4f  ", aa[i][j]);
			fprintf (stdout, "\n");
		}
		fprintf (stdout, "\n");
	}
}
