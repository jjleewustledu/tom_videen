/*  $Id$
    $Log$
 */
/* =============================================================================
 *  Module:         make_num_list.c
 *  Date:           2/24/92
 *  Author:         Will Tribbey, ver 2.1
 *  Description:    
 *					make_num_list() takes the list of matrix numbers
 *			        in the array mlist->list and compares it with the
 *					requested list of matrix numbers so that mlist->frames,
 *					mlist->planes, etc, can be filled in correctly.
 *		
 *		input		The input consists of pointers to arrays containing
 *			        lists of frame numbers, plane numbers, gates numbers,
 *			        and bed numbers. Along with each pointer is an integer
 *			     	whose value is the length of its corresponding array.
 *		
 *		output		On exit, mlist->frames, mlist->planes, mlist->gates,
 *					mlist->beds, and mlist->nmats will have the correct values.
 *  History:
 *      Nov-95   Modified TOV
 * =============================================================================
 */
#include <matrix.h>

static char     rcsid[] = "$Header: /export/home/npggw/tom/src/libecat7/RCS/make_num_list.c,v 1.1 1995/10/09 16:47:17 ty7777 Exp $";

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
