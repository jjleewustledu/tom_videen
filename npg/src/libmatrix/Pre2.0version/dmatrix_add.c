/*	$Log: dmatrix_add.c,v $
 * Revision 1.3  1999/12/13  18:06:58  tom
 * tov revision
 *
 * Revision 1.2  1995/07/24  21:47:37  tom
 * fixed
 *
 * Revision 1.1  1995/07/20  14:57:22  tom
 * Initial revision
 *
	$Id: dmatrix_add.c,v 1.3 1999/12/13 18:06:58 tom Exp $ */
/*	========================================================================
 *	Module:			dmatrix_add.c
 *	Date:			20-Jul-95
 *	Author:			Tom Videen
 *	Description:	Add type double matrices:  c[m][n] = a[m][n] + b[m][n]
 *	========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libmatrix/Pre2.0version/RCS/dmatrix_add.c,v 1.3 1999/12/13 18:06:58 tom Exp $";
#endif

void            dmatrix_add (a, b, c, m, n)
	double        **a, **b, **c;
	int             m, n;
{
	int             i, j;

	for (i = 1; i <= m; i++)
		for (j = 1; j <= n; j++)
			c[i][j] = a[i][j] + b[i][j];
}
