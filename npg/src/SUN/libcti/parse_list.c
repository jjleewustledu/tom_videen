/*$Id: parse_list.c,v 1.6 1999/12/13 22:02:19 tom Exp $*/
/*$Log: parse_list.c,v $
 *Revision 1.6  1999/12/13 22:02:19  tom
 *partially cleaned up with lint
 *
 * Revision 1.5  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.4  1995/10/30  16:54:44  tom
 * changed petutil/matrix.h to matrix.h
 *
 * Revision 1.3  1994/03/03  17:34:22  ty7777
 * Change from rcs keyword id to header.
 *
 * Revision 1.2  1993/09/13  19:47:06  ty7777
 * Introduce rcsid.
 *
 * Revision 1.1  1993/03/04  19:54:24  ty7777
 * Initial revision
 **/

/*    @(#)parse_list.c	2.1   2/24/92     */

#include <string.h>

#ifndef lint
static char     sccsid[] = "@(#)parse_list.c	2.1   2/24/92  Copyright 1989-1991 CTI PET Systems, Inc.";
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/parse_list.c,v 1.6 1999/12/13 22:02:19 tom Exp $";
#endif


/*________________________________________________________________________
	description	parse_list() takes the input string
			buff and parses it to extract the substrings
			frames, planes, gates, beds, that specify lists
		        of frames, planes, gates, and beds.

	input		all inputs are character arrays.

	output		parse_list() copies the appropriate substring
			into frames, planes, gates, and beds and returns
		        a value of 1 on successful exit. The funtion returns
			a value of 0 when an error is encountered.

	called by	make_list()
	
	written by	Will Tribbey	3/6/91

	modification history :

	WHO		DATE		CHANGE
________________________________________________________________________*/

int             parse_list (buff, frames, planes, gates, beds)
	char            buff[], frames[], planes[], gates[], beds[];
{
	char           *p;

	p = strtok (buff, ";");			 /* get frame list    */
	if (!p)
		return (0);
	strcpy (frames, p);
	p = strtok ((char *) NULL, ";");	 /* get plane list   */
	if (!p)
		return (0);
	strcpy (planes, p);
	p = strtok ((char *) NULL, ";");	 /* get gate list    */
	if (!p)
		return (0);
	strcpy (gates, p);
	p = strtok ((char *) NULL, ";");	 /* get bed list     */
	if (!p)
		return (0);
	strcpy (beds, p);

	return (1);
}
