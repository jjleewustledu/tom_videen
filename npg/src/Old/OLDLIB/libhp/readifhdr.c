/*$Id: readifhdr.c,v 1.7 1995/10/30 22:39:01 tom Exp $*/
/*$Log: readifhdr.c,v $
 * Revision 1.7  1995/10/30  22:39:01  tom
 * change include to remove "petutil"
 *
 * Revision 1.6  1995/06/13  17:21:43  ty7777
 * Same as the last version.
 *
 * Revision 1.5  1995/06/01  16:17:55  ty7777
 * Use line array to read.
 **/

/*___________________________________________________________________________
  File:		readifhdr.c

  Function:	int readifhdr (char *ifhdrfile, BOOLEAN print_message, int maxlines,
		int *nlines, char *lines [])

  Arguments:	ifhdrfile,	file name for the interfile header, passed in.
		print_message,	TRUE: print error message if there is an error;
				FALSE: no error message will be printed, passed in.
		maxlines,	maximum of lines that can be read. In general, this 
				should be the same as the array size of lines. Passed in.
		nlines,		number of lines contained in the header file, returned.		
		lines,		array of lines, returned.

  Description:	This function reads a interfile header file and copies the lines to
		an array of strings.

  Return:	SUCCEED for successful; FAIL for error.

  Author:	Tom Yang, May 30, 1995.

  History:	Created by Tom Yang on 05/30/95. 
___________________________________________________________________________*/

#include <stdio.h>
#include <petutil.h>
#include <ifh.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libhp/RCS/readifhdr.c,v 1.7 1995/10/30 22:39:01 tom Exp $";

PUBLIC int readifhdr (ifhdrfile, print_message, maxlines, nlines, lines) 
	char	*ifhdrfile;
	BOOLEAN	print_message;
	int	maxlines;
	int	*nlines;
	char	*lines [];
{
	FILE	*fp;
	char	line [MAXLINE];
	char	*pline;

	fp	= fopen (ifhdrfile, "r");
	if (fp == (FILE *) NULL)
	{
		if (print_message)
			fprintf (stderr, "Error in opening header file %s\n", ifhdrfile);
		return FAIL;
	}

	*nlines	= 0;
	while (fgets (line, MAXLINE, fp) != NULL && *nlines < maxlines)
	{
		pline	= (char *) malloc (strlen (line) * sizeof (char));
		if (pline == NULL)
		{
			if (print_message)
				fprintf (stderr, "Error in allocating memory for pline\n");
			return FAIL;
		}

		strcpy (pline, line);
		lines [(*nlines)++]	= pline;
	}

	fclose (fp);

	return SUCCEED;
}
