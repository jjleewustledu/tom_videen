/*	$Log: dmatrix_print.c,v $
 * Revision 1.3  1999/12/13  18:06:58  tom
 * tov revision
 *
 * Revision 1.2  1995/07/20  19:14:32  tom
 * processes vectors
 *
	$Id: dmatrix_print.c,v 1.3 1999/12/13 18:06:58 tom Exp $ */
/*	========================================================================
 *	Module:			dmatrix_print.c
 *	Date:			20-Jul-95
 *	Author:			Tom Videen
 *	Description:	Print type double matrix a[m][n]
 *					If n=1, print a vector.
 *	========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libmatrix/Pre2.0version/RCS/dmatrix_print.c,v 1.3 1999/12/13 18:06:58 tom Exp $";
#endif

#include <stdio.h>

void            dmatrix_print (a, m, n)
	double            *a;
	int             m, n;
{
	double         **aa;
	int             i, j;

	if (n == 1) {
		for (i = 1; i <= m; i++)
			fprintf (stdout, "%10.4f  ", a[i]);
		fprintf (stdout, "\n\n");
	} else {
		aa = (double **) a;
		for (i = 1; i <= m; i++) {
			for (j = 1; j <= n; j++)
				fprintf (stdout, "%10.4f  ", aa[i][j]);
			fprintf (stdout, "\n");
		}
		fprintf (stdout, "\n");
	}
}
