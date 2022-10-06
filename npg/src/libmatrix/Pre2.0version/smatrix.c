/*	$Log: smatrix.c,v $
 * Revision 1.2  1999/12/13  18:06:58  tom
 * tov revision
 *
 * Revision 1.1  1999/02/15  14:12:06  tom
 * Initial revision
 * */
/*	========================================================================
 *	Module:			smatrix.c
 *	Date:			Jan-99
 *	Author:			Tom Videen
 *	Description:	Allocate a 2D matrix of short integers
 *					with range m[nyl..nyh][nxl..nxh]
 *	========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libmatrix/Pre2.0version/RCS/smatrix.c,v 1.2 1999/12/13 18:06:58 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#define NR_END 1

short        **smatrix (nyl, nyh, nxl, nxh)
	long            nyl, nyh, nxl, nxh;

{
	long            i;
	long            nrow = nyh - nyl + 1;
	long            ncol = nxh - nxl + 1;
	short         **m;

/* allocate pointers to pointers to rows */
 
    m = (short **) malloc ((unsigned int) ((nrow + NR_END) * sizeof (short *)));
	if (!m) {
		fprintf (stdout, "ERROR [smatrix]: malloc\n");
		exit (1);
	}
	m += NR_END;
	m -= nyl;

/* allocate pointers to cols and set pointers to them */

	m[nyl] = (short *) malloc ((unsigned int) ((nrow * ncol + NR_END) * sizeof (short *)));
	if (!m[nyl]) {
		fprintf (stdout, "ERROR [smatrix]: malloc\n");
		exit (1);
	}
	m[nyl] += NR_END;
	m[nyl] -= nxl;

	for (i = nyl + 1; i <= nyh; i++)
		m[i] = m[i - 1] + ncol;

/* return pointer to array of pointers to planes */

	return m;
}
