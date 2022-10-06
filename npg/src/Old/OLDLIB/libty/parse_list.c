/*$Id: parse_list.c,v 1.1 1995/10/09 16:49:34 ty7777 Exp $*/
/*$Log: parse_list.c,v $
 * Revision 1.1  1995/10/09  16:49:34  ty7777
 * Initial revision
 **/

/*    @(#)parse_list.c	2.1   2/24/92     */

#include <string.h>
#include "matrix.h"

#ifndef lint
static char sccsid[]="@(#)parse_list.c	2.1   2/24/92  Copyright 1989-1991 CTI PET Systems, Inc.";
#endif

static char rcsid [] = "$Header: /export/home/npggw/tom/src/libecat7/RCS/parse_list.c,v 1.1 1995/10/09 16:49:34 ty7777 Exp $";


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

int parse_list (buff, frames, planes, gates, beds)
	char	buff [], frames [], planes [], gates [], beds [];
{
	char	*p;

	p	= strtok (buff, ";");          /*  get frame list    */
	if (!p)   
		return (0);
	strcpy (frames, p);
	p	= strtok ((char *) NULL, ";");   /*   get plane list   */
	if (!p)   
		return (0);
	strcpy (planes, p);
	p	= strtok ((char *) NULL, ";");   /*   get gate list    */
	if (!p)   
		return (0);
	strcpy (gates, p);
	p	= strtok ((char *) NULL, ";");   /*   get bed list     */
	if (!p)   
		return (0);
	strcpy (beds, p);

	return (1);
}
