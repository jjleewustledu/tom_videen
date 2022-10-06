/*	$Log: matrix_mult.c,v $
 * Revision 1.6  1999/12/13  18:06:58  tom
 * tov revision
 *
 * Revision 1.5  1996/03/12  23:26:57  tom
 * *** empty log message ***
 *
 * Revision 1.4  1995/07/21  22:33:16  tom
 * *** empty log message ***
 *
 * Revision 1.3  1995/07/20  19:14:32  tom
 * processes vectors
 *
 * Revision 1.2  1995/07/20  17:53:35  tom
 * multiplies matrix * vector
 *
	$Id: matrix_mult.c,v 1.6 1999/12/13 18:06:58 tom Exp $ */
/*	========================================================================
 *	Module:			matrix_mult.c
 *	Date:			20-Jul-95
 *	Author:			Tom Videen
 *	Description:	Multiply type float matrices:  c[m][p] = a[m][n] * b[n][p]
 *	Input:
 *		a[m][n] -   matrix (m rows, n columns)
 *		b[n][p] -   matrix (n rows, p columns)
 *		m, n, p -   dimensions of matrices (indexed from 1 as in nrecipes)
 *	Output:
 *		c[m][p] -   product matrix (m rows, p columns)
 *	========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libmatrix/Pre2.0version/RCS/matrix_mult.c,v 1.6 1999/12/13 18:06:58 tom Exp $";
#endif

void            matrix_mult (a, b, c, m, n, p)
	float         **a, **b, **c;
	int             m, n, p;
{
	int             i, j, k;

	for (i = 1; i <= m; i++) {
		for (j = 1; j <= p; j++) {
			c[i][j] = 0.0;
			for (k = 1; k <= n; k++)
				c[i][j] += a[i][k] * b[k][j];
		}
	}
}
