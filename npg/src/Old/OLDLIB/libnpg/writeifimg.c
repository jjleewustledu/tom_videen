/*$Id: writeifimg.c,v 1.4 1995/11/10 16:29:37 tom Exp $*/
/*$Log: writeifimg.c,v $
 * Revision 1.4  1995/11/10  16:29:37  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.3  1995/06/01  18:19:16  ty7777
 * Updated the error message.
 *
 * Revision 1.2  1995/06/01  17:38:29  ty7777
 * Added return values.
 **/

/*___________________________________________________________________________
 * File:	writeifimg.c
 *
 * Function:	int writeifimg (short *data, long dimension, BOOLEAN append_flag,
 *				char *ifimgfile)
 *
 * Arguments:	data,		short integer array.
 *		dimension,	dimension of the short integer array.
 *		append_flag,	TRUE: append; FALSE: create.
 *		ifimgfile,	The output file name. Data is either created for
 *				the file or appended.
 *
 * Description:	This function writes an one dimensional array to the output file.
 *		If this function is called multiple times to write multiple
 *		slices, the slices have to have the same dimension.
 *
 * Return:	SUCCEED for successful; FAIL for error.
 *
 * Author:	Tom Yang, May 31, 1995.
 *
 * History:	Created by Tom Yang on 05/31/95.
 *_________________________________________________________________________*/

#include <unistd.h>
#include <petutil.h>

static char     rcsid[] = "$Header: /home/npggw/tom/src/libnpg/RCS/writeifimg.c,v 1.4 1995/11/10 16:29:37 tom Exp $";

PUBLIC int      writeifimg (data, dimension, append_flag, ifimgfile)
	short          *data;
	long            dimension;
	BOOLEAN         append_flag;
	char           *ifimgfile;
{
	FILE           *fp;
	char            cbuffer[MAXLINE];
	long            i;

/*
 * Open or create if necessary the output file for writing.
 */
	if (append_flag)
		strcpy (cbuffer, "a+b");
	else
		strcpy (cbuffer, "w+b");
	fp = fopen (ifimgfile, "a+b");
	if (fp == NULL)
	{
		fprintf (stderr, "Error: in opening file %s\n", ifimgfile);
		return FAIL;
	}

/*
 * Go to the end of file.
 */
	fseek (fp, 0, SEEK_END);

/*
 * Write the data.
 */
	i = fwrite (data, sizeof (short), dimension, fp);
	if (i != dimension)
	{
		fprintf (stderr, "Error: Only %d objects are written out.\n", i);
		fclose (fp);
		return FAIL;
	}

	fclose (fp);

	return SUCCEED;
}
