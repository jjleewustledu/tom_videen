/*  ================================================================================
 *  File Name:      shortensors.c
 *  Date:           Jan-99
 *  Author:         Tom Videen
 *  Description:    Supplements to Numerical Recipes nrutil.c
 *  =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libnrimg/Pre3.0version/RCS/shortensors.c,v 1.4 1999/12/13 18:21:08 tom Exp $";
#endif

#include <stdlib.h>
#include <nrutil.h>

/* Allocate a short 3tensor with range t[nrl..nrh][ncl..nch][ndl..ndh] */
/* and return pointer to array of pointers to rows */

short        ***s3tensor (nrl, nrh, ncl, nch, ndl, ndh)
	long            nch, ncl, ndh, ndl, nrh, nrl;
{
	long            i, j;
	long            nrow = nrh - nrl + 1;
	long            ncol = nch - ncl + 1;
	long            ndep = ndh - ndl + 1;
	short        ***t;

	/* allocate pointers to pointers to rows */

	t = (short ***) calloc ((unsigned int)(nrow + 1), sizeof (short **));
	if (!t)
		nrerror ("allocation failure 1 in s3tensor()");
	t += 1;
	t -= nrl;

	/* allocate pointers to rows and set pointers to them */

	t[nrl] = (short **) calloc ((unsigned int)(nrow * ncol + 1), sizeof (short *));
	if (!t[nrl])
		nrerror ("allocation failure 2 in s3tensor()");
	t[nrl] += 1;
	t[nrl] -= ncl;

	/* allocate rows and set pointers to them */

	t[nrl][ncl] = (short *) calloc ((unsigned int)(nrow * ncol * ndep + 1), sizeof (short));
	if (!t[nrl][ncl])
		nrerror ("allocation failure 3 in s3tensor()");
	t[nrl][ncl] += 1;
	t[nrl][ncl] -= ndl;

	for (j = ncl + 1; j <= nch; j++)
		t[nrl][j] = t[nrl][j - 1] + ndep;
	for (i = nrl + 1; i <= nrh; i++) {
		t[i] = t[i - 1] + ncol;
		t[i][ncl] = t[i - 1][ncl] + ncol * ndep;
		for (j = ncl + 1; j <= nch; j++)
			t[i][j] = t[i][j - 1] + ndep;
	}
	return t;
}

/* free a short s3tensor allocated by s3tensor() */

void            free_s3tensor (t, nrl, nrh, ncl, nch, ndl, ndh)
	short        ***t;
	long            nch, ncl, ndh, ndl, nrh, nrl;
{
	free ((char *) (t[nrl][ncl] + ndl - 1));
	free ((char *) (t[nrl] + ncl - 1));
	free ((char *) (t + nrl - 1));
}
