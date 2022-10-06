/*$Id: make_num_list.c,v 1.6 1999/12/13 22:02:19 tom Exp $*/
/*$Log: make_num_list.c,v $
 *Revision 1.6  1999/12/13 22:02:19  tom
 *partially cleaned up with lint
 *
 * Revision 1.5  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.4  1995/10/30  16:54:44  tom
 * changed petutil/matrix.h to matrix.h
 *
 * Revision 1.3  1994/03/03  17:27:51  ty7777
 * Change from rcs keyword id to header.
 *
 * Revision 1.2  1993/09/13  19:42:33  ty7777
 * Introduce rcsid.
 *
 * Revision 1.1  1993/03/04  19:44:20  ty7777
 * Initial revision
 **/

/*     @(#)make_num_list.c	2.1   2/24/92    */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/make_num_list.c,v 1.6 1999/12/13 22:02:19 tom Exp $";
#endif

#include <matrix.h>

/*
	description	make_num_list() takes the list of matrix numbers
		        in the array mlist->list and compares it with the
			requested list of matrix numbers so that mlist->frames,
			mlist->planes, etc, can be filled in correctly.

	input		The input consists of pointers to arrays containing
		        lists of frame numbers, plane numbers, gates numbers,
		        and bed numbers. Along with each pointer is an integer
		     	whose value is the length of its corresponding array.

	output		On exit, mlist->frames, mlist->planes, mlist->gates,
			mlist->beds, and mlist->nmats will have the correct
			values.
	called by	make_list()
	written by	Will Tribbey	3/6/91
	modification history :
*/

void            make_num_list (f, nf, p, np, g, ng, b, nb, mlist)
	int            *f, nf,			 /* pointer to frame array and length of array    */
	               *p, np,			 /* pointer to frame array and length of array    */
	               *g, ng,			 /* pointer to frame array and length of array    */
	               *b, nb;			 /* pointer to frame array and length of array    */

	Mlist          *mlist;			 /* pointer to Mlist whose list is being checked  */
{
	struct Matval   mval;

	int             i, j, fctr = 0, pctr = 0, gctr = 0, bctr = 0;

	for (i = 0; i < mlist->nmats; i++) {
		mat_numdoc (mlist->list[i], &mval);
		for (j = 0; j < nf; j++) {
			if (mval.frame == f[j]) {
				fctr++;
				f[j] = -1;
			}
		}
		for (j = 0; j < np; j++) {
			if (mval.plane == p[j]) {
				pctr++;
				p[j] = -1;
			}
		}
		for (j = 0; j < ng; j++) {
			if (mval.gate == g[j]) {
				gctr++;
				g[j] = -1;
			}
		}
		for (j = 0; j < nb; j++) {
			if (mval.bed == b[j]) {
				bctr++;
				b[j] = -1;
			}
		}
	}

	mlist->nmats = fctr * pctr * gctr * bctr;
	mlist->frames = fctr;
	mlist->planes = pctr;
	mlist->gates = gctr;
	mlist->beds = bctr;

	return;
}
