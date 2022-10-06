/*	$Log: vector_dist.c,v $
 * Revision 1.3  1999/12/13  18:06:58  tom
 * tov revision
 *
 * Revision 1.2  1996/01/09  21:36:10  tom
 * changed path in include
 *
 * Revision 1.1  1995/07/21  20:32:23  tom
 * Initial revision
 *
	$Id: vector_dist.c,v 1.3 1999/12/13 18:06:58 tom Exp $ */
/*  ========================================================================
 *  Module:         vector_dist.c
 *  Date:           21-Jul-95
 *  Author:         Tom Videen
 *  Description:    Compute vector distance: x[] - y[]
 *  ========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libmatrix/Pre2.0version/RCS/vector_dist.c,v 1.3 1999/12/13 18:06:58 tom Exp $";
#endif

#include <math.h>
#include <nrutil.h>

float           vector_dist (x, y)
	float           x[], y[];
{
	float           dif, sqrdif;
	int             i;

	sqrdif = 0;
	for (i = 1; i <= 3; i++) {
		dif = x[i] - y[i];
		sqrdif += dif * dif;
	}
	return (sqrt (sqrdif));
}
