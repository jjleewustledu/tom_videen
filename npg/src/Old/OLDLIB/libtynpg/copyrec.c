/*$Id: copyrec.c,v 1.4 1995/12/14 20:03:53 ty7777 Exp $*/
/*$Log: copyrec.c,v $
 * Revision 1.4  1995/12/14  20:03:53  ty7777
 * Use NEWimglist.h
 *
 * Revision 1.3  1994/03/03  20:02:03  ty7777
 * Replace id with header in rcsid.
 *
 * Revision 1.2  1993/11/04  19:48:00  ty7777
 * Working version.
 *
 * Revision 1.1  1993/11/04  19:36:12  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		copyrec.c

  Function:	PUBLIC void copyrec (FILE *recfp, char *data_filename)

  Arguments:	recfp,		the file pointer for the rec file. The recfile 
				must be open before calling this function.
		data_filename,	the data file name. It must be a valid
				data file. If the data file is the rec file 
				of an image, data_filename should be the
				complete name of the rec file instead of
				the image file.

  Return:	None.

  Description:	Copy a data file to the output recfile.


  Called by:	the main program imageweld.c.
		WeldImage () in WeldImage.c.

  Calling:	

  Author:	Tom Yang.

  History:	Created by Tom Yang on 11/04/93. 
___________________________________________________________________________*/

#include <petutil/NEWimglist.h>      

static char rcsid [] = "$Header: /home/petsun4/ty7777/src/libnpg/RCS/copyrec.c,v 1.4 1995/12/14 20:03:53 ty7777 Exp $";

PUBLIC void copyrec (recfp, data_filename)
	FILE	*recfp;
	char	*data_filename;
{
	FILE	*fp;
	char	line [MAXLINE];
	char	item [MAXLINE];

	fp	= fopen (data_filename, "r");

	if (fp == (FILE *) NULL)
		fprintf (recfp, "%s not found\n", data_filename);
	else 
	{
		while (fgets (line, MAXLINE, fp) != NULL)
		{
			if (sscanf (line, "%s", item) == 1)
			{
				fprintf (recfp, "%s", line);
			}
		}
		fclose (fp);
	}
}
			
