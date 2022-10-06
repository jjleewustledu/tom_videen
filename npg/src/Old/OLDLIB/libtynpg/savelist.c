/*$Id: savelist.c,v 1.6 1995/12/14 20:09:21 ty7777 Exp $*/
/*$Log: savelist.c,v $
 * Revision 1.6  1995/12/14  20:09:21  ty7777
 * Use NEWimglist.h
 *
 * Revision 1.5  1995/01/03  21:58:42  ty7777
 * Modified the output format.
 *
 * Revision 1.4  1995/01/03  21:44:26  ty7777
 * Added processing for weight.
 *
 * Revision 1.3  1994/11/22  16:02:30  ty7777
 * Modified the comments.
 *
 * Revision 1.2  1994/10/06  19:40:45  ty7777
 * Do not save a mena if it is 0.0.
 *
 * Revision 1.1  1994/09/28  15:26:19  ty7777
 * Initial revision
 **/

#ifndef lint
static char sccsid [] = "@(#)$Id: savelist.c,v 1.6 1995/12/14 20:09:21 ty7777 Exp $";
#endif

static char rcsid [] = "$Id: savelist.c,v 1.6 1995/12/14 20:09:21 ty7777 Exp $";

/*___________________________________________________________________________
  File:		savelist.c

  Function:	int savelist (char *list_filename, int num_imagelist, 
		IMAGE_LIST_DATA *imagelist)

  Arguments:	list_filename:	image list file to write.	
		num_imagelist:	number of records.
		imagelist:	array of IMAGE_LIST_DATA structure.

  Return:	SUCCEED:	successful.
		FAIL:		failed.

  Description:	This function writes a list of IMAGE_LIST_DATA to a file.

  Called by:	imgmean.	

  Calling:	

  Author:	Tom Yang, July 1993.

  History:	Created by Tom Yang on 07/28/93. 
___________________________________________________________________________*/

#include <values.h>
#include <petutil/NEWimglist.h>

PUBLIC int savelist (list_filename, num_imagelist, imagelist)
	char		*list_filename;
	int		num_imagelist;
	IMAGE_LIST_DATA	*imagelist;
{
	FILE	*fp;
	int	i;

	fp	= fopen (list_filename, "w");
	if (fp == (FILE *) NULL)
	{
		fprintf (stderr, "Error in opening %s&n", list_filename);
		return FAIL;
	}

	for (i = 0; i < num_imagelist; i++)
	{
		if (strlen (imagelist [i].scanset2_id) > 0)
		{
				fprintf (fp, 
				"%s	%s	%8.2f	%s	%8.2f	%f	%s\n", 
				imagelist [i].subject_id,
				imagelist [i].scanset1_id,
				imagelist [i].mean1,
				imagelist [i].scanset2_id,
				imagelist [i].mean2,
				imagelist [i].weight,
				imagelist [i].sxrid);
		}
		else
		{
			if (imagelist [i].mean1 > 0.0)
			{
				fprintf (fp, "%s	%s	%8.2f	%s\n", 
				imagelist [i].subject_id,
				imagelist [i].scanset1_id,
				imagelist [i].mean1,
				imagelist [i].sxrid);
			}
			else
			{
				fprintf (fp, "%s	%s	%s\n", 
				imagelist [i].subject_id,
				imagelist [i].scanset1_id,
				imagelist [i].sxrid);
			}
		}
	}

	fclose (fp);

	return SUCCEED;
}
