/*  $Id$
    $Log$
 */
/* =============================================================================
 *  Module:         make_matlist.c
 *  Date:			Mar-91
 *  Author:         Will Tribbey (CTI)
 *  Description:    Creates list of matrix numbers from frames, planes, gates, beds.
 *
 *		input		The input consists of pointers to arrays containing
 *			        lists of frame numbers, plane numbers, gates numbers,
 *			        and bed numbers. Along with each pointer is an integer
 *			     	whose value is the length of its corresponding array.
 *			        Also passed as input is a pointer to an Mlist structure
 *			        into which the list of matrix numbers will be put.
 *	
 *		output		On exit, mlist->list will be the list of matrix numbers
 *					of length mlist->nmats.
 *  History:
 *      Nov-95   Modified TOV
 * =============================================================================
 */
#include <matrix.h>

static char     rcsid[] = "$Header: /export/home/npggw/tom/src/libecat7/RCS/make_matlist.c,v 1.1 1995/10/09 16:46:54 ty7777 Exp $";

void            make_matlist (file1, file2, f, nf, p, np, g, ng, b, nb, mlist)
	MatrixFile     *file1, *file2;	 /* files from which  list is to be made         */
	int            *f, nf,			 /* pointer to frame array and length of array   */
	               *p, np,			 /* pointer to frame array and length of array   */
	               *g, ng,			 /* pointer to frame array and length of array   */
	               *b, nb;			 /* pointer to frame array and length of array   */
	Mlist          *mlist;			 /* pointer to Mlist structure to be filled      */
{
	struct MatDir   matdir_file1;
	struct MatDir   matdir_file2;
	int             i, j, k, l, m, matnum, matflag, nmats = 0, er;
	int             status1;
	int             status2;

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
							status1 = matrix_find (file1, matnum, &matdir_file1);
							status2 = matrix_find (file2, matnum, &matdir_file2);
							if (status1 != ERROR && status2 != ERROR) {
								nmats++;
								mlist->list[nmats - 1] = matnum;
							}
						} else {
							status1 = matrix_find (file1, matnum, &matdir_file1);
							if (status1 != ERROR) {
								nmats++;
								mlist->list[nmats - 1] = matnum;
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
