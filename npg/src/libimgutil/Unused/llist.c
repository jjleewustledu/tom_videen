/*  $Id$
    $Log$
 */
/* =============================================================================
 *  Module:         llist.c
 *  Date:           Jun-91
 *  Author:         Todd Gamble and Tom Yang
 *  Description:    
 *		Two-way linked list primitives. Generic data types as list elements
 *		are supported by pointer indirection. This code is derived from a
 *		similar set of primitives described in "Reusable Data Structures for C"
 *		by Roger Sessions (Prentice Hall, 1989).
 *	
 *		The exportable functions, user definable functions, and data types
 *		are contained in the "list.h" header file.
 *		This module uses the conventions described in the file
 *		"package.h" for function return values and other constants.
 *
 *  History:
 *      Nov-95   Modified TOV
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libnpg/RCS/llist.c,v 1.4 1995/11/10 16:28:40 tom Exp $";
#endif

#include <llist.h>
#include <petutil.h>

/* =============================================================================
 *  Function:	llcreate
 *  	Runtime allocation of links. Returns a pointer to the allocated
 *  	space or NULL on a memory allocation error.
 * =============================================================================
 */

static llnode *llcreatenode ()
{
	llnode         *n;

	n = (llnode *) malloc (sizeof (llnode));
	if (n == (llnode *) NULL)
		return ((llnode *) NULL);
	else {
		n->data = NULL;
		n->previous = (llnode *) NULL;
		n->next = (llnode *) NULL;
		return (n);
	}
}

/* =============================================================================
 *  Function:	llinit
 *		Initialize memory for the linked list control area. This includes
 *		space for head, tail, and cursor pointers and a size variable. This
 *		function is called automatically when lladd is used on an uninitialized list.
 * =============================================================================
 */

extern llist    llinit ()
{
	llist           l;

	l = (llist) malloc (sizeof (struct linked_list));
	if (l != (llist) NULL) {
		l->head = (llnode *) NULL;
		l->tail = (llnode *) NULL;
		l->cursor = (llnode *) NULL;
		l->size = 0;
		return (l);
	} else {				 /* added by Tom Yang on 3/11/91 */
		return ((llist) NULL);
	}
}

/* =============================================================================
 *  Function:	lladd
 *		This function allocates space for a new link, retains a pointer to the
 *		new data in the link, and inserts the new link directly after the
 *		current cursor position. After insertion the cursor is set to the new link.
 * =============================================================================
 */

extern int      lladd (l, d)
	llist           l;
	pointer         d;
{
	llnode         *new;

	if ((new = llcreatenode ()) == (llnode *) NULL)
		return (FAIL);
	else {
		if (l->size == 0) {
			l->cursor = new;
			l->head = new;
			l->tail = new;
			new->next = (llnode *) NULL;
			new->previous = (llnode *) NULL;
		} else {
			new->next = l->cursor->next;
			new->previous = l->cursor;
			if (l->tail == l->cursor)
				l->tail = new;
			else
				l->cursor->next->previous = new;
			l->cursor->next = new;
			l->cursor = new;
		}
		new->data = d;
		l->size++;
		return (SUCCEED);
	}
}

/* =============================================================================
 *  Function:	lladdhead
 *		This function allocates space for a new link, retains a pointer to the
 *		new data in the link, and inserts the new link directly before the
 *		current head position. After insertion the cursor is set to the new
 *		link (i.e., the head).
 * =============================================================================
 */

extern int      lladdhead (l, d)
	llist           l;
	pointer         d;
{
	llnode         *new;

	if ((new = llcreatenode ()) == (llnode *) NULL)
		return (FAIL);
	else {
		if (l->size == 0) {
			l->cursor = new;
			l->head = new;
			l->tail = new;
			new->next = (llnode *) NULL;
			new->previous = (llnode *) NULL;
		} else {
			new->next = l->head;
			new->previous = (llnode *) NULL;
			l->head->previous = new;
			l->head = new;
			l->cursor = new;
		}
		new->data = d;
		l->size++;
		return (SUCCEED);
	}
}

/* =============================================================================
 *  Function:	lladdtail
 *		This function allocates space for a new link, retains a pointer to the
 *		new data in the link, and inserts the new link directly after the
 *		current tail position. After insertion the cursor is set to the new
 *		link (i.e., the tail).
 * =============================================================================
 */

extern int      lladdtail (l, d)
	llist           l;
	pointer         d;
{
	llnode         *new;

	if ((new = llcreatenode ()) == (llnode *) NULL)
		return (FAIL);
	else {
		if (l->size == 0) {
			l->cursor = new;
			l->head = new;
			l->tail = new;
			new->next = (llnode *) NULL;
			new->previous = (llnode *) NULL;
		} else {
			new->next = (llnode *) NULL;
			new->previous = l->tail;
			l->tail->next = new;
			l->tail = new;
			l->cursor = new;
		}
		new->data = d;
		l->size++;
		return (SUCCEED);
	}
}

/* =============================================================================
 *  Function:	lldelete
 *		This function deletes and frees the link located at the cursor
 *		position. The function fails when there are no items in the list.
 *		After deletion the cursor is set to the head of the list.
 * =============================================================================
 */

extern int      lldelete (l)
	llist           l;
{
	llnode         *before, *after, *delete;


	if (l->size > 0) {
		delete = l->cursor;

		if ((l->head == l->cursor) && (l->tail == l->cursor)) {
			l->head = (llnode *) NULL;
			l->tail = (llnode *) NULL;
		} else if (l->head == l->cursor) {
			l->head = l->head->next;
			l->head->previous = (llnode *) NULL;
		} else if (l->tail == l->cursor) {
			l->tail = l->tail->previous;
			l->tail->next = (llnode *) NULL;
		} else {
			before = l->cursor->previous;
			after = l->cursor->next;
			before->next = after;
			after->previous = before;
		}

		free (delete);
		l->size--;
		l->cursor = l->head;
		return (SUCCEED);
	} else {
		return (FAIL);
	}
}


/* =============================================================================
 *  Function:	llsize
 *		This function returns the current size of the list. The behavior of
 *		llsize is undefined when the list has not been initialized.
 * =============================================================================
 */

extern int      llsize (l)
	llist           l;
{
	return (l->size);
}

/*
 *	The cursor is positioned at the head of the list.
 */

extern int      llhead (l)
	llist           l;
{
	l->cursor = l->head;
	return (SUCCEED);
}

/* =============================================================================
 *  Function:	lltail
 *		The	cursor is positioned at the tail of the list.
 * =============================================================================
 */

extern int      lltail (l)
	llist           l;
{
	l->cursor = l->tail;
	return (SUCCEED);
}


/* =============================================================================
 *  Function:	llnext
 *		Moves the cursor to the next link in the list. If the cursor is at
 *		the tail of the list then FAIL is returned.
 * =============================================================================
 */

extern int      llnext (l)
	llist           l;
{
	if (l->size <= 0)
		return (FAIL);
	if (l->cursor->next == NULL)
		return (FAIL);
	l->cursor = l->cursor->next;
	return (SUCCEED);
}

/* =============================================================================
 *  Function:	llprevious
 *		Moves the cursor to the previous link in the list. If the cursor is at
 *		the head of the list then FAIL is returned.
 * =============================================================================
 */

extern int      llprevious (l)
	llist           l;
{
	if (l->cursor->previous == NULL)
		return (FAIL);
	else {
		l->cursor = l->cursor->previous;
		return (SUCCEED);
	}
}


/* =============================================================================
 *  Function:	llretrieve
 *		Returns a pointer to the data item addressed by the link where the
 *		cursor is currently located. Returns NULL when there is no data or
 *		when the list is empty.
 * =============================================================================
 */

extern pointer llretrieve (l)
	llist           l;
{
	return (l->cursor->data);
}


/* =============================================================================
 *  Function:	llinsert
 *		Inserts an item into a sorted list. Uses the user defined
 *		comparison function llcmp() to test items in the list for ordering.
 *		llcmp(a,b) should return -1 when a<b, 0 when a==b, and 1 when
 *		a>b. There is no return value. The cursor is set to the head of
 *		the list before return.
 * =============================================================================
 */

extern void     llinsert (l, p)
	llist           l;
	pointer         p;
{
	int             ret;

	if (llsize (l) == 0) {
		lladd (l, p);
		return;
	}
	lltail (l);

	do {
		if ((*llcmp) (p, llretrieve (l)) >= 0) {
			lladd (l, p);
			return;
		}
	} while (llprevious (l) != FAIL);

	lladdhead (l, p);
	llhead (l);
}


/* =============================================================================
 *  Function:	llsort
 * 		Sorts a list in ascending order by insertion sort using llinsert().
 *		The new list is returned. Note that the old list memory is freed.
 * =============================================================================
 */

extern llist llsort (l)
	llist           l;
{
	pointer         p;
	llist           s;

	s = llinit ();
	llhead (l);
	while (llsize (l) > 0) {
		p = llretrieve (l);
		llinsert (s, p);
		lldelete (l);
	}
	free (l);				 /* BUG need to free every link */
	return (s);
}
