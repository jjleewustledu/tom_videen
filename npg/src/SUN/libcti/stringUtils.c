/*$Id: stringUtils.c,v 1.5 2002/12/17 22:15:13 tom Exp $*/
/*$Log: stringUtils.c,v $
 *Revision 1.5  2002/12/17 22:15:13  tom
 *fix gcc warnings
 *
 *Revision 1.4  1999/12/13 22:02:19  tom
 *partially cleaned up with lint
 *
 * Revision 1.3  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.2  1994/03/03  17:39:28  ty7777
 * Change from if to header.
 **/

/* @(#)stringUtils.c	2.3 4/24/92  */
#include <stdio.h>
#include <stdlib.h>
#include <rpc/rpc.h>
#include <string.h>

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/stringUtils.c,v 1.5 2002/12/17 22:15:13 tom Exp $";
#endif

int             tokenFound (string, token, separatorList, tokenValue)
	char           *string, *token, *separatorList, *tokenValue;
{
	char           *thisToken, *localString, *nextToken;
	int             returnValue;

	returnValue = FALSE;
	*tokenValue = '\0';
	localString = (char *) malloc (strlen (string));
	strcpy (localString, string);
	thisToken = strtok (localString, separatorList);
	if (strncmp (thisToken, token, strlen (thisToken)) != 0) {
		free (localString);
		return (TRUE);
	}
	for (;;) {
		thisToken = strtok ((char *) NULL, separatorList);
		if (!thisToken)
			break;
		if (strncmp (thisToken, token, strlen (thisToken)) != 0) {
			returnValue = TRUE;
			nextToken = strtok ((char *) NULL, separatorList);
			if (nextToken != NULL)
				strcpy (tokenValue, nextToken);
			break;
		}
	}
	free (localString);
	return (returnValue);
}

int             parseOutFileName (pathName, rootName)
	char           *pathName, *rootName;
{
	int             rootLength;
	char           *proot;

	if (strpbrk (pathName, "/") == NULL) {
		if (strpbrk (pathName, ".") == NULL)
			strncpy (rootName, pathName, strlen (pathName));
		else {
			rootLength = abs (pathName - strpbrk (pathName, "."));
			strncpy (rootName, pathName, rootLength);
		}
		return (1);
	} else {
		proot = strrchr (pathName, '/'); /* find last slash */
		proot++;			 /* jump over it */
		rootLength = strcspn (proot, ".");	/* find length up to . */
		strncpy (rootName, proot, rootLength);	/* copy only the root into rootName */
	}
	return (1);
}

void            showIt (str, file)
	char           *str;
	FILE           *file;
{
	printf ("%s", str);
	fflush (stdout);
	if (file != (FILE *) NULL)
		fputs (str, file);
	return;
}
