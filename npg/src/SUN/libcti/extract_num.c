/*$Id: extract_num.c,v 1.6 1999/12/13 22:02:19 tom Exp $*/
/*$Log: extract_num.c,v $
 *Revision 1.6  1999/12/13 22:02:19  tom
 *partially cleaned up with lint
 *
 * Revision 1.5  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.4  1995/10/30  16:54:44  tom
 * changed petutil/matrix.h to matrix.h
 *
 * Revision 1.3  1994/03/03  17:25:56  ty7777
 * Change from id to header.
 **/

/*     @(#)extract_num.c	2.1   2/24/92     */

#ifndef lint
static char     sccsid[] = "@(#)extract_num.c	2.1   2/24/92  Copyright 1989-1991 CTI PET Systems, Inc.";
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/extract_num.c,v 1.6 1999/12/13 22:02:19 tom Exp $";
#endif

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*_________________________________________________________________________
	description	extract_num() takes the string pointed to
			by p and extracts the first integer found in
			the string, sets add to the number of digits
	                in the integer and returns thevalue of the
			integer found.
			
	input		p is a pointer to a string that begins with a
			decimal digit.
	
	output		On exit, extract_num() returns the value of the
			leading integer in the string pointed to by p and
		        sets add to point to the number of the digits in
			the integer.
	
	called by	parse_matnum()
	
	written by	Will Tribbey	3/5/91

	modification history :

	WHO		DATE		CHANGE
_________________________________________________________________________*/

int             extract_num (p, add)
	char           *p;

	int            *add;
{
	int             i = 0;

	char           *q, numbuff[16];

	/*
	 * Initialize q and numbuff
	 */
	q = p;
	strcpy (numbuff, "");

	/*
	 * Find the digits of the leading integer
	 */
	while (isdigit (*q)) {
		numbuff[i] = *q;
		i++;
		q++;
	}
	numbuff[i] = '\0';

	/*
	 * Set add to the number of digits found and return the integer
	 */
	*add = i;
	return (atoi (numbuff));
}
