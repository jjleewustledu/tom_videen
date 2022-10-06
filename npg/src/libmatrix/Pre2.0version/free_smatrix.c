/*	$Log: free_smatrix.c,v $
 * Revision 1.2  1999/12/13  18:06:58  tom
 * tov revision
 *
 * Revision 1.1  1999/02/15  14:12:06  tom
 * Initial revision
 * */
/*	========================================================================
 *	Module:			free_smatrix.c
 *	Date:			Jan-99
 *	Author:			Tom Videen
 *	Description:	Free memory allocated to a 2D matrix of short integers.
 *					with range m[nyl..nyy][nxl..nxh] allocated by smatrix.
 *	========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libmatrix/Pre2.0version/RCS/free_smatrix.c,v 1.2 1999/12/13 18:06:58 tom Exp $";
#endif

#include <stdlib.h>
#define NR_END 1
#define FREE_ARG char*

void            free_smatrix (m, nyl, nyy, nxl, nxh)
	short          **m;
	long            nyl, nyy, nxl, nxh;

{
	free ((FREE_ARG) (m[nyl] + nxl - NR_END));
	free ((FREE_ARG) (m + nyl - NR_END));
}
