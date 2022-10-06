/*	$Log: matrix_trans.c,v $
 * Revision 1.2  1999/12/13  18:06:58  tom
 * tov revision
 *
 * Revision 1.1  1996/01/09  21:35:54  tom
 * Initial revision
 *
	$Id: matrix_trans.c,v 1.2 1999/12/13 18:06:58 tom Exp $ */
/*	========================================================================
 *	Module:			matrix_trans.c
 *	Date:			Jan-96
 *	Author:			Tom Videen
 *	Description:	Return transpose of matrix
 *	Input:
 *		a[m][n] -   matrix (m rows, n columns)
 *	Output:
 *		b[n][m] -   transpose (n rows, m columns)
 *	========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libmatrix/Pre2.0version/RCS/matrix_trans.c,v 1.2 1999/12/13 18:06:58 tom Exp $";
#endif

#include <nrutil.h>

float            **matrix_trans (a, m, n)
	float         **a;
	int             m, n;
{
	float         **b;
	int             i, j;

	b = matrix(1, n, 1, m);
	for (j = 1; j <= m; j++)
		for (i = 1; i <= n; i++)
				b[i][j] = a[j][i];
	return (b);
}
