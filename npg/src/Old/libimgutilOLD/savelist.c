/*	$Id: savelist.c,v 2.2 1999/12/13 17:04:17 tom Exp $
	$Log: savelist.c,v $
 * Revision 2.2  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 2.1  1998/03/12  15:02:08  tom
 * write negative means
 *
 * Revision 2.0  1996/04/24  23:07:41  tom
 * new library release
 *
 * Revision 1.1  1996/03/12  23:00:34  tom
 * Initial revision
 * */
/* ===============================================================================
 *	Module:			savelist.c
 *	Date:			Jul-93
 *	Author:			Tom Yang
 *	Description:	Writes a list of IMAGE_LIST_DATA to a file
 *	
 * ===============================================================================
 */
#ifndef lint
static char		rcsid[] = "$Header: /home/npggw/tom/src/libimgutilOLD/RCS/savelist.c,v 2.2 1999/12/13 17:04:17 tom Exp $";
#endif 

#include <stdio.h>
#include <string.h>
#include <imgutil.h>

int      savelist (list_filename, num_imagelist, imagelist)
	char           *list_filename;		/* file to write */
	int             num_imagelist;		/* number of records */
	IMAGE_LIST_DATA *imagelist;			/* data */
{
	FILE           *fp;
	int             i;

	fp = fopen (list_filename, "w");
	if (fp == (FILE *) NULL) {
		fprintf (stderr, "Error in opening %s&n", list_filename);
		return (ERROR);
	}
	for (i = 0; i < num_imagelist; i++) {
		if (strlen (imagelist[i].scanset2_id) > 0) {
			fprintf (fp,
				 "%s	%s	%8.2f	%s	%8.2f	%f	%s\n",
				 imagelist[i].subject_id,
				 imagelist[i].scanset1_id,
				 imagelist[i].mean1,
				 imagelist[i].scanset2_id,
				 imagelist[i].mean2,
				 imagelist[i].weight,
				 imagelist[i].sxrid);
		} else {
			fprintf (fp, "%s	%s	%8.2f	%s\n",
				 imagelist[i].subject_id,
				 imagelist[i].scanset1_id,
				 imagelist[i].mean1,
				 imagelist[i].sxrid);
		}
	}
	fclose (fp);
	return (OK);
}
