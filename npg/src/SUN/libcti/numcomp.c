/*$Id: numcomp.c,v 1.4 1996/09/25 16:28:32 tom Exp $*/
/*$Log: numcomp.c,v $
 *Revision 1.4  1996/09/25 16:28:32  tom
 *build in Solaris 2
 *
 * Revision 1.3  1994/03/03  17:33:34  ty7777
 * Change from id to header.
 *
 * Revision 1.2  1993/09/13  19:45:55  ty7777
 * Introduce rcsid.
 *
 * Revision 1.1  1993/03/04  19:53:20  ty7777
 * Initial revision
 **/

/*   @(#)numcomp.c	2.1   2/24/92     */

#ifndef lint
static char     sccsid[] = "@(#)numcomp.c	2.1   2/24/92  Copyright 1989-1991 CTI PET Systems, Inc.";
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/numcomp.c,v 1.4 1996/09/25 16:28:32 tom Exp $";
#endif

/*_____________________________________________________________________________
	description	Comparison function for qsort used in
			make_list() and parse_matnum().

	input		i and j point to two integer values that
		        are to be compared
	
	output		The function returns the difference between
			the integer values pointed to by i and j.

	written by	Will Tribbey	3/5/91

	modification history :

	WHO		DATE		CHANGE

_____________________________________________________________________________*/

int             numcomp (i, j)
	int            *i, *j;

{
	return (*i - *j);
}
