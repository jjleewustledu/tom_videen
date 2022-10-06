/*	$Log: dmatrix_mult.c,v $
 * Revision 1.5  1999/12/13  18:06:58  tom
 * tov revision
 *
 * Revision 1.4  1995/07/21  22:33:21  tom
 * *** empty log message ***
 *
 * Revision 1.3  1995/07/20  19:14:32  tom
 * processes vectors
 *
	$Id: dmatrix_mult.c,v 1.5 1999/12/13 18:06:58 tom Exp $ */
/*	========================================================================
 *	Module:			dmatrix_mult.c
 *	Date:			20-Jul-95
 *	Author:			Tom Videen
 *	Description:	Multiply type double matrices:  c[m][p] = a[m][n] * b[n][p]
 *					when c and b are points, p = 1
 *	Input:
 *		a[m][n] -   matrix (m rows, n columns)
 *		b[n][p] -   matrix (n rows, p columns)
 *		m, n, p -   dimensions of matrices (indexed from 1 as in nrecipes)
 *	Output:
 *		c[m][p] -   product matrix (m rows, p columns)
 *	========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libmatrix/Pre2.0version/RCS/dmatrix_mult.c,v 1.5 1999/12/13 18:06:58 tom Exp $";
#endif

void            dmatrix_mult (a, b, c, m, n, p)
	double         **a, *b, *c;
	int             m, n, p;
{
	double         **bb, **cc;
	int             i, j, k;

	if (p == 1) {
		for (i = 1; i <= m; i++) {
			c[i] = 0.0;
			for (j = 1; j <= n; j++)
				c[i] += a[i][j] * b[j];
		}
	} else {
		bb = (double **) b;
		cc = (double **) c;
		for (i = 1; i <= m; i++) {
			for (j = 1; j <= p; j++) {
				cc[i][j] = 0.0;
				for (k = 1; k <= n; k++)
					cc[i][j] += a[i][k] * bb[k][j];
			}
		}
	}
}
