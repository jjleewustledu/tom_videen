/*	$Log: vector_mult.c,v $
 * Revision 1.2  1999/12/13  18:06:58  tom
 * tov revision
 *
 * Revision 1.1  1996/03/12  23:27:09  tom
 * Initial revision
 *
	$Id: vector_mult.c,v 1.2 1999/12/13 18:06:58 tom Exp $ */
/*	========================================================================
 *	Module:			vector_mult.c
 *	Date:			04-Mar-96
 *	Author:			Tom Videen
 *	Description:	Multiply type float matrix times vector:  c[m] = a[m][n] * b[n]
 *	Input:
 *		a[m][n] -   matrix (m rows, n columns)
 *		b[n] 	-   vector (n rows)
 *		m, n	-   dimensions of matrices and vectors (indexed from 1 as in nrecipes)
 *	Output:
 *		c[m]	-   product vector (m rows)
 *	========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libmatrix/Pre2.0version/RCS/vector_mult.c,v 1.2 1999/12/13 18:06:58 tom Exp $";
#endif

void            vector_mult (a, b, c, m, n)
	float         **a, *b, *c;
	int             m, n;
{
	int             i, j;

	for (i = 1; i <= m; i++) {
		c[i] = 0.0;
		for (j = 1; j <= n; j++)
			c[i] += a[i][j] * b[j];
	}
}
