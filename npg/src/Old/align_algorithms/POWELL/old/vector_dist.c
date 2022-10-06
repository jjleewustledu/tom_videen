/*	$Log$
	$Id$ */
/*  ========================================================================
 *  Module:         vector_dist.c
 *  Date:           20-Jul-95
 *  Author:         Tom Videen
 *  Description:    Compute vector distance: x[] - y[]
 *  ========================================================================
 */

#include <math.h>
#include <nrecipes/kr/nr.h>
#include <nrecipes/kr/nrutil.h>

static char     rcsid[] = "$Header$";

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
