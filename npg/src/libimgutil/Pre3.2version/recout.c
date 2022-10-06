/* $Log: recout.c,v $
 * Revision 3.0  2003/08/21  15:16:40  tom
 * 21Aug2003
 *
 * Revision 2.1  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 2.0  1996/04/24  23:07:41  tom
 * new library release
 *
 * Revision 1.1  1996/03/12  23:00:34  tom
 * Initial revision
 *
 * Revision 1.11  1995/11/10  18:48:16  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.10  1994/10/03  16:17:13  tom
 * standard format with indent
 *
 * Revision 1.9  1994/05/18  15:53:30  tom
 * changed $Id: recout.c,v 3.0 2003/08/21 15:16:40 tom Exp $ to $Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/recout.c,v 3.0 2003/08/21 15:16:40 tom Exp $
 * */
/*	========================================================================
 *	Module:			recout.c
 *	Date:			19-Aug-93
 *	Author:			Tom Videen
 *	Description:	Copy an input image's recfile to the output recfile
 *	
 *	Input:
 *		recfp   -   pointer to a new rec file
 *		filename -  name of the image file associated with an old rec file
 *	 
 *	Output:
 *		contents of old rec file copied to recfp
 *	 
 *	Algorithm:
 *	1)  Create the old rec file name.
 *	2)  Open the old rec file. 
 *	3)  If no old rec file, write "not found" to the new rec file.
 *		Otherwise, copy the contents of the old rec file to the new one.
 *	
 *	========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/recout.c,v 3.0 2003/08/21 15:16:40 tom Exp $";
#endif

#include <stdio.h>
#include <string.h>
#include <myvalues.h>	/* MAXLINE */

/*	-----------------------
 *	Function recout
 *	-----------------------
 */
void	recout (recfp, filename)
	FILE           *recfp;
	char           *filename;
{
	FILE           *fp;
	char            recfile[MAXPATH];
	char            line[MAXLINE];

	(void) strcpy (recfile, filename);
	(void) strcat (recfile, ".rec");
	fp = fopen (recfile, "r");
	if (fp == (FILE *) NULL)
		fprintf (recfp, "%s not found\n", recfile);
	else {
		while (fgets (line, MAXLINE, fp) != NULL)
			fprintf (recfp, "%s", line);
		(void) fclose (fp);
	}
}
