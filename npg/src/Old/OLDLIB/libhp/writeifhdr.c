/*$Id: writeifhdr.c,v 1.2 1995/10/30 22:39:01 tom Exp $*/
/*$Log: writeifhdr.c,v $
 * Revision 1.2  1995/10/30  22:39:01  tom
 * change include to remove "petutil"
 *
 * Revision 1.1  1995/06/01  16:55:43  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		writeifhdr.c

  Function:	int writeifhdr (char *ifhdrfile, BOOLEAN print_message,
		int nlines, char *lines [])

  Arguments:	ifhdrfile,	file name for the interfile header, passed in.
		print_message,	TRUE: print error message if there is an error;
				FALSE: no error message will be printed, passed in.
		nlines,		number of lines contained in the header file, passed in.
		lines,		array of lines, passed in.

  Description:	This function writes a interfile header file and copies the lines to
		an array of strings.

  Return:	SUCCEED for successful; FAIL for error.

  Author:	Tom Yang, May 31, 1995.

  History:	Created by Tom Yang on 05/30/95.
___________________________________________________________________________*/

#include <stdio.h>
#include <petutil.h>
#include <ifh.h>

static char     rcsid[] = "$Header: /home/npggw/tom/src/libhp/RCS/writeifhdr.c,v 1.2 1995/10/30 22:39:01 tom Exp $";

PUBLIC int      writeifhdr (ifhdrfile, print_message, nlines, lines)
	char           *ifhdrfile;
	BOOLEAN         print_message;
	int             nlines;
	char           *lines[];
{
	FILE           *fp;
	int             i;

	fp = fopen (ifhdrfile, "w");
	if (fp == (FILE *) NULL) {
		if (print_message)
			fprintf (stderr, "Error in opening header file %s\n", ifhdrfile);
		return FAIL;
	}
	for (i = 0; i < nlines; i++) {
		if (strrchr (lines[i], '\n') == NULL)
			fprintf (fp, "%s\n", lines[i]);
		else
			fprintf (fp, "%s", lines[i]);
	}

	fclose (fp);

	return SUCCEED;
}
