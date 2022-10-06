/*	$Id: getstr.c,v 1.3 2002/12/17 21:55:15 tom Exp tom $
 *	$Log: getstr.c,v $
 * Revision 1.3  2002/12/17  21:55:15  tom
 * fix pointer/cast errors found with gcc
 *
 *	Revision 1.2  1999/12/13 17:45:50  tom
 *	tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *  Function:       getstr
 *  Date:           Nov-95
 *  Author:         John Ollinger
 *  Description:    Get line from Interfile header.
 *
 *	Copyright 1991-1996, Washington University.
 *	All rights reserved. Unauthorized reproduction prohibited.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/RCS/getstr.c,v 1.3 2002/12/17 21:55:15 tom Exp tom $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char           *getstr (line)
	char           *line;
{
	char           *ptr, *ptr1;

	ptr = strstr (line, ":=");
	ptr += 2;
	ptr1 = strstr (line, "\n");
	if (ptr1 != NULL)
		free(ptr1);
	while (ptr[0] == ' ')
		++ptr;
	return (ptr);
}
