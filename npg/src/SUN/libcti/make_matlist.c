/*$Id: make_matlist.c,v 1.7 1999/12/13 22:02:19 tom Exp $*/
/*$Log: make_matlist.c,v $
 *Revision 1.7  1999/12/13 22:02:19  tom
 *partially cleaned up with lint
 *
 * Revision 1.6  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.5  1995/10/30  16:54:44  tom
 * changed petutil/matrix.h to matrix.h
 *
 * Revision 1.4  1994/09/23  19:18:09  ty7777
 * Modified by Tom Yang to fix memory leaks.
 *
 * Revision 1.3  1994/03/03  17:26:48  ty7777
 * Change from rcs keyword id to header.
 *
 * Revision 1.2  1993/09/13  19:41:29  ty7777
 * Introduce rcsid.
 *
 * Revision 1.1  1993/03/04  19:43:16  ty7777
 * Initial revision
 **/

/*     @(#)make_matlist.c	2.1   2/24/92     */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/make_matlist.c,v 1.7 1999/12/13 22:02:19 tom Exp $";
#endif

#include <stdlib.h>
#include <matrix.h>

/*______________________________________________________________________________
	description	make_matlist() takes the lists of frames,
			planes, gates, and beds and creates a list
			of matrix numbers.

	input		The input consists of pointers to arrays containing
		        lists of frame numbers, plane numbers, gates numbers,
		        and bed numbers. Along with each pointer is an integer
		     	whose value is the length of its corresponding array.
		        Also passed as input is a pointer to an Mlist structure
		        into which the list of matrix numbers will be put.

	output		On exit, mlist->list will be the list of matrix numbers
			of length mlist->nmats.

	called by	make_list()

	written by	Will Tribbey	3/6/91

	history:	Modified by Tom Yang on Sep. 23, 1994 to fix memory leaks.

______________________________________________________________________________*/

void            make_matlist (file1, file2, f, nf, p, np, g, ng, b, nb, mlist)
	MatrixFile     *file1, *file2;		 /* files from which  list is to be made         */

	int            *f, nf,			 /* pointer to frame array and length of array   */
	               *p, np,			 /* pointer to frame array and length of array   */
	               *g, ng,			 /* pointer to frame array and length of array   */
	               *b, nb;			 /* pointer to frame array and length of array   */

	Mlist          *mlist;			 /* pointer to Mlist structure to be filled      */
{
	struct MatDir  *matdir_file1;

	struct MatDir  *matdir_file2;

	struct MatDir  *matrix_find ();

	int             i, j, k, l, m, matnum, matflag, nmats = 0;

	for (i = 0; i < nf; i++) {
		for (j = 0; j < np; j++) {
			for (k = 0; k < ng; k++) {
				for (l = 0; l < nb; l++) {
					matnum = mat_numcod (f[i], p[j], g[k], 0, b[l]);
					matflag = 0;
					for (m = 0; m < nmats; m++) {

						/*
						 * Check for duplicate matnums
						 */
						if (matnum == mlist->list[m])
							matflag = 1;
					}
					if (!matflag) {	/* if flag not set, look up matrix with
							 * matnum  */
						if (file2) {
							matdir_file1 = matrix_find (matnum, file1);
							matdir_file2 = matrix_find (matnum, file2);
							if (matdir_file1 && matdir_file2) {
								nmats++;
								mlist->list[nmats - 1] = matnum;
							}
							if (matdir_file1)
								free (matdir_file1);
							if (matdir_file2)
								free (matdir_file2);
						} else {
							matdir_file1 = matrix_find (matnum, file1);
							if (matdir_file1) {
								nmats++;
								mlist->list[nmats - 1] = matnum;
								free (matdir_file1);
							}
						}
					}
				}
			}
		}
	}

	mlist->nmats = nmats;

	return;
}
