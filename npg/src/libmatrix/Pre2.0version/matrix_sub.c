/*	$Log: matrix_sub.c,v $
 * Revision 1.2  1999/12/13  18:06:58  tom
 * tov revision
 *
 * Revision 1.1  1995/07/24  21:43:46  tom
 * Initial revision
 *
	$Id: matrix_sub.c,v 1.2 1999/12/13 18:06:58 tom Exp $ */
/*	========================================================================
 *	Module:			matrix_sub.c
 *	Date:			20-Jul-95
 *	Author:			Tom Videen
 *	Description:	Add type float matrices:  c[m][n] = a[m][n] - b[m][n]
 *	========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libmatrix/Pre2.0version/RCS/matrix_sub.c,v 1.2 1999/12/13 18:06:58 tom Exp $";
#endif

void            matrix_sub (a, b, c, m, n)
	float         **a, **b, **c;
	int             m, n;
{
	int             i, j;

	for (i = 1; i <= m; i++)
		for (j = 1; j <= n; j++)
			c[i][j] = a[i][j] - b[i][j];
}
