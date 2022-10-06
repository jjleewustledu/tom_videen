/*$Id: mat_get_spec.c,v 1.6 1999/12/13 22:02:19 tom Exp $*/
/*$Log: mat_get_spec.c,v $
 *Revision 1.6  1999/12/13 22:02:19  tom
 *partially cleaned up with lint
 *
 * Revision 1.5  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.4  1995/10/30  16:54:44  tom
 * changed petutil/matrix.h to matrix.h
 *
 * Revision 1.3  1994/03/03  17:28:50  ty7777
 * Change from id to header.
 *
 * Revision 1.2  1993/09/13  19:43:42  ty7777
 * Introduce rcsid.
 *
 * Revision 1.1  1993/03/04  19:45:45  ty7777
 * Initial revision
 **/

/* @(#)mat_get_spec.c	2.1  10/14/91 */
/*
*	Module:		mat_get_spec.c
*	Author:		E. Pearson
*	Date:		May 1991
*	Description:	Return the actual number of frames, planes, gates, and bed
*			positions in a file.
*		Inputs:	 filename of matrix file (any type)
*		Outputs:  number of frames (int *)
*				  number of planes (int *)
*				  number of gates  (int *)
*				  number of beds   (int *)
*
*				All the return paramters are 1 based.
*/

#include	<stdio.h>
#include	<matrix.h>

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/mat_get_spec.c,v 1.6 1999/12/13 22:02:19 tom Exp $";
#endif

int             mat_get_spec (file, num_frames, num_planes, num_gates, num_bed)
	char           *file;
	int            *num_frames, *num_planes, *num_gates, *num_bed;
{
	struct MatDir   matrixlist[5000];
	FILE           *fptr;
	int             status, num_matrices, i;
	struct Matval   matnum;

	/* initialization */
	status = 0;
	*num_frames = 0;
	*num_planes = 0;
	*num_gates = 0;
	*num_bed = 0;

	/* open the specified file */
	fptr = mat_open (file, "r");
	if (fptr != NULL) {
		/* get the matrix entries */
		num_matrices = mat_list (fptr, matrixlist, 5000);

		for (i = 0; i < num_matrices; i++) {
			mat_numdoc (matrixlist[i].matnum, &matnum);

			if (matnum.frame > *num_frames)
				(*num_frames)++;

			if (matnum.plane > *num_planes)
				(*num_planes)++;

			if (matnum.gate > *num_gates)
				(*num_gates)++;

			if (matnum.bed > *num_bed)
				(*num_bed)++;
		}

		/*
		 * bed is zero based in the matrix number, but all numbers returned from this
		 * function will be one based
		 */
		(*num_bed)++;
		mat_close (fptr);
	} else
		status = 1;

	return (status);
}
