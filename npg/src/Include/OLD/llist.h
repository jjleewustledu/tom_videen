/*$Id: llist.h,v 1.2 1995/12/22 21:35:17 tom Exp $*/
/*$Log: llist.h,v $
 * Revision 1.2  1995/12/22  21:35:17  tom
 * lint
 *
 * Revision 1.1  1995/10/30  18:24:32  tom
 * Initial revision
 *
 * Revision 1.2  1994/03/03  20:54:17  ty7777
 * Replace id with header.
 *
 * Revision 1.1  1993/11/11  17:48:47  ty7777
 * Initial revision
 **/

#ifndef _LIST_INCLUDED
#define _LIST_INCLUDED

#ifndef lint
static char     rcsid_llist[] = "$Header: /home/npggw/tom/include/RCS/llist.h,v 1.2 1995/12/22 21:35:17 tom Exp $";

#endif

#include <package.h>

typedef void   *pointer;

typedef struct linked_list_node {
	pointer        *data;
	struct linked_list_node *previous;
	struct linked_list_node *next;
}               llnode;

typedef struct linked_list {
	int             size;
	llnode         *head;
	llnode         *tail;
	llnode         *cursor;
}              *llist;

/*
 * Function prototypes
 */
PUBLIC llist    llinit ( /* void */ );
PUBLIC int      lladdhead ( /* llist, pointer */ );
PUBLIC int      lladd ( /* llist, pointer */ );
PUBLIC int      lldelete ( /* llist */ );
PUBLIC int      llsize ( /* llist */ );
PUBLIC int      llhead ( /* llist */ );
PUBLIC int      lltail ( /* llist */ );
PUBLIC int      llnext ( /* llist */ );
PUBLIC int      llprevious ( /* llist */ );
PUBLIC pointer  llretrieve ( /* llist */ );

/*
 * User defined functions.
 */
void            (*llprint) ( /* llist */ );
int             (*llcmp) ( /* pointer, pointer */ );

#endif	/* _LIST_INCLUDED */
