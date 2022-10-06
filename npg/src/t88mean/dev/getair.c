/*  $Log$ */
/* ===============================================================================
 *  Module:         getair.c
 *  Date:           Dec-96
 *  Author:         Tom Videen
 *  Description:
 *      Read an AIR file (versions 1 or 2) into the structure, air, and
 *      convert the transformation matrix into a matrix, mat, to be
 *		used in standard matrix multiplications.
 * ===============================================================================
*/
#include <stdio.h>
#include <myvalues.h>
#include <air.h>

#define MSIZE 4					 /* matrix dimension */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/airdif/RCS/getair.c,v 1.1 1996/08/22 21:52:42 tom Exp $";

#endif

int             getair (filename, mat)
	char           *filename;
	float         **mat;

{
	struct air1     oldair;
	struct air2     air;
	FILE           *fp;
	int             i, j;

	fp = fopen (filename, "rb");
	if (fp == NULL) {
		(void) fprintf (stderr, "ERROR: cannot open %s\n", filename);
		return (ERROR);
	}
	if (fread ((char *) &air, 1, sizeof (struct air2), fp) != sizeof (struct air2)) {
		/* Try reading it as an old 12 parameter air file */
		rewind (fp);
		if (fread ((char *) &oldair, 1, sizeof (struct air1), fp) != sizeof (struct air1)) {
			(void) fprintf (stderr, "ERROR: reading %s\n", filename);
			(void) fclose (fp);
			return (ERROR);
		}
		air.e[0][0] = oldair.e[1][0];
		air.e[0][1] = oldair.e[1][1];
		air.e[0][2] = oldair.e[1][2];
		air.e[0][3] = 0;
		air.e[1][0] = oldair.e[2][0];
		air.e[1][1] = oldair.e[2][1];
		air.e[1][2] = oldair.e[2][2];
		air.e[1][3] = 0;
		air.e[2][0] = oldair.e[3][0];
		air.e[2][1] = oldair.e[3][1];
		air.e[2][2] = oldair.e[3][2];
		air.e[2][3] = 0;
		air.e[3][0] = oldair.e[0][0];
		air.e[3][1] = oldair.e[0][1];
		air.e[3][2] = oldair.e[0][2];
		air.e[3][3] = 1;
	}
	(void) fclose (fp);

	for (i = 1; i <= MSIZE; i++)
		for (j = 1; j <= MSIZE; j++)
			mat[j][i] = (float) air.e[i - 1][j - 1];

	return (OK);
}
