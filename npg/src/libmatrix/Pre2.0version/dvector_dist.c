/*	$Log: dvector_dist.c,v $
 * Revision 1.4  1999/12/13  18:06:58  tom
 * tov revision
 *
 * Revision 1.3  1996/01/09  21:35:59  tom
 * changed path in include
 *
 * Revision 1.2  1995/07/25  20:20:26  tom
 * *** empty log message ***
 *
 * Revision 1.1  1995/07/21  20:32:23  tom
 * Initial revision
 *
	$Id: dvector_dist.c,v 1.4 1999/12/13 18:06:58 tom Exp $ */
/*  ========================================================================
 *  Module:         dvector_dist.c
 *  Date:           21-Jul-95
 *  Author:         Tom Videen
 *  Description:    Compute vector distance: x[] - y[]
 *  ========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libmatrix/Pre2.0version/RCS/dvector_dist.c,v 1.4 1999/12/13 18:06:58 tom Exp $";
#endif

#include <math.h>
#include <nrutil.h>

double          dvector_dist (x, y)
	double          x[], y[];
{
	double          dif, sqrdif;
	int             i;

	sqrdif = 0;
	for (i = 1; i <= 3; i++) {
		dif = x[i] - y[i];
		sqrdif += dif * dif;
	}
	return (sqrt (sqrdif));
}
