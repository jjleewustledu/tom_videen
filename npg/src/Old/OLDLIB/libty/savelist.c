/*  $Id$
    $Log$
 */
/* ===============================================================================
 *  Module:         savelist.c
 *  Date:           Jan-93
 *  Author:         Tom Yang
 *  Description:    Write a list of IMAGE_LIST_DATA to a file.
 *  Return:         SUCCEED or FAIL
 *  History:
 *      Nov-95   Modified TOV
 * ===============================================================================
 */
#include <values.h>
#include <imglist.h>

static char     rcsid[] = "$Id: savelist.c,v 1.6 1995/11/10 16:29:24 tom Exp $";

PUBLIC int      savelist (list_filename, num_imagelist, imagelist)
	char           *list_filename;
	int             num_imagelist;
	IMAGE_LIST_DATA *imagelist;
{
	FILE           *fp;
	int             i;

	fp = fopen (list_filename, "w");
	if (fp == (FILE *) NULL) {
		fprintf (stderr, "Error in opening %s&n", list_filename);
		return FAIL;
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
			if (imagelist[i].mean1 > 0.0) {
				fprintf (fp, "%s	%s	%8.2f	%s\n",
					 imagelist[i].subject_id,
					 imagelist[i].scanset1_id,
					 imagelist[i].mean1,
					 imagelist[i].sxrid);
			} else {
				fprintf (fp, "%s	%s	%s\n",
					 imagelist[i].subject_id,
					 imagelist[i].scanset1_id,
					 imagelist[i].sxrid);
			}
		}
	}
	fclose (fp);
	return SUCCEED;
}
