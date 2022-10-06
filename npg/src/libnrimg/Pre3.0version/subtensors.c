/* $Log: subtensors.c,v $
 * Revision 2.2  1999/12/13  18:21:08  tom
 * tov revision
 *
 * Revision 2.1  1996/05/15  19:05:47  tom
 * change calling parameters
 *
 * Revision 2.0  1996/04/24  23:00:22  tom
 * new release
 *
 * Revision 1.1  1996/02/28  20:40:31  tom
 * Initial revision
 *
 */
/*	========================================================================
 *	Module:			subtensors.c
 *	Date:			Feb-96
 *	Author:			Tom Videen
 *	Description:	Compute difference between 2 tensors
 *
 *	Input:
 *		a   -   initial image
 *		b   -   subtrahend
 *		n[3] - dimensions
 *
 *	Output:
 *		a   -   difference
 *	========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libnrimg/Pre3.0version/RCS/subtensors.c,v 2.2 1999/12/13 18:21:08 tom Exp $";
#endif

/*	-----------------------
 *	Function subtensors
 *	-----------------------
 */
void            subtensors (a, b, n)
	float        ***a, ***b;
	long           *n;
{
	long            i, j, k;

	for (i = 1; i <= n[2]; ++i) {
		for (j = 1; j <= n[1]; ++j) {
			for (k = 1; k <= n[0]; ++k)
				a[i][j][k] -= b[i][j][k];
		}
	}
}
