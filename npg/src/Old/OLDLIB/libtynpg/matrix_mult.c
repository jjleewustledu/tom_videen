/*$Id: matrix_mult.c,v 1.2 1994/03/03 20:32:57 ty7777 Exp $*/
/*$Log: matrix_mult.c,v $
 * Revision 1.2  1994/03/03  20:32:57  ty7777
 * Replace id with header.
 *
 * Revision 1.1  1994/02/04  20:46:27  ty7777
 * Initial revision
 **/

/*	========================================================================
 *	Module:		matrix_mult.c
 *	Date:		21-Jun-93
 *	Author:		Tom Videen
 *	Description:	Multiply matrices:  c[n][m] = a[n][p] * b[p][m]
 *			when c and b are points, m = 1
 *	========================================================================
 */

#include <petutil/petutil.h>

static char rcsid []= "$Header: /home/petsun4/ty7777/src/libnpg/RCS/matrix_mult.c,v 1.2 1994/03/03 20:32:57 ty7777 Exp $";

/*	-----------------------
 *	Function matrix_mult
 *	-----------------------
 */
PUBLIC void matrix_mult (a, b, c, n, m, p)
	float	*a, *b, *c;
	int	n, m, p;
{
	int	i, j, k;

	for (i = 0; i < n; ++i) {
		for (j = 0 ; j < m; ++j) {
			*(c + m*i + j)	= 0.0;
			for (k = 0; k < p; ++k)
				*(c + m*i + j) += *(a + p*i + k) * *(b + m*k + j);
		}
	}
	return;
}
