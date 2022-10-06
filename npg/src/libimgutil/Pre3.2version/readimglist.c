/*	$Id: readimglist.c,v 3.0 2003/08/21 15:16:40 tom Exp $
	$Log: readimglist.c,v $
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
 * */
/* ===============================================================================
 *	Module:			readimglist.c
 *	Date:			May-93
 *	Author:			Tom Yang
 *	Description:	Reads an image list file into an array of structures.
 *		If the control number is not specified, it is assigned -MAXINT
 *
 *  Reminder:	Every real number should contain at least a decimal point.
 *		The sxr_id is used to find sxr files which include any characters
 *		other than "<species><subject_id>.sxr". If an sxr id is present, it
 *		must start with a letter and contains the string ".sxr".
 *
 *  Algorithm:	if (a line contains 7 fields)
 *		  <subject_id><scanset1_id><mean1><scanset2_id><mean2><weight><sxrid>
 *		else if (a line contains 6 fields)
 *		  if (the last field is an integer or a real number)
 *		    <subject_id><scanset1_id><mean1><scanset2_id><mean2><weight>
 *		  else
 *		    <subject_id><scanset1_id><mean1><scanset2_id><mean2><sxrid>
 *		  endif
 *		else
 *		  Same as before.
 *		endif
 *
 * ===============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/readimglist.c,v 3.0 2003/08/21 15:16:40 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <imgutil.h>

int             readimglist (lstfile, imglist, lstsize)
	char           *lstfile;
	IMAGE_LIST_DATA *imglist;
	int            *lstsize;
{
	FILE           *fp;
	char            line[MAXLINE];
	char            scanset1_id[MAXLINE];
	char            string1[MAXLINE];
	char            string2[MAXLINE];
	char            string3[MAXLINE];
	char            string4[MAXLINE];
	char            subject_id[MAXLINE];
	char            sxrid[MAXLINE];
	int             i;

	fp = fopen (lstfile, "r");

	i = 0;
	while (fgets (line, MAXLINE, fp) != NULL) {
		if (sscanf (line, "%s %s", subject_id, scanset1_id) != 2 ||
		    strncmp (subject_id, "#", 1) == 0)
			continue;

/*
 * Set the default values
 */
		(void) strcpy (imglist[i].scanset1_id, "");
		imglist[i].mean1 = 0.0;
		(void) strcpy (imglist[i].scanset2_id, "");
		imglist[i].mean2 = 0.0;
		imglist[i].weight = 1.0;
		(void) strcpy (imglist[i].sxrid, "");

/*
 * <subject_id><scanset1_id><mean1><scanset2_id><mean2><weight><sxrid>
 */
		if (sscanf (line, "%s %s %s %s %s %s %s", subject_id, scanset1_id,
			    string1, string2, string3, string4, sxrid) == 7) {
			if (stringtype (string1) == TYPE_STRING ||
			    stringtype (string3) == TYPE_STRING ||
			    stringtype (string4) == TYPE_STRING ||
			    strstr (sxrid, SXR_EXT) == NULL)
				continue;

			(void) strcpy (imglist[i].subject_id, subject_id);
			(void) strcpy (imglist[i].scanset1_id, scanset1_id);
			imglist[i].mean1 = atof (string1);
			(void) strcpy (imglist[i].scanset2_id, string2);
			imglist[i].mean2 = atof (string3);
			imglist[i].weight = atof (string4);
			(void) strcpy (imglist[i].sxrid, sxrid);
			i++;
			continue;
		}
		if (sscanf (line, "%s %s %s %s %s %s", subject_id, scanset1_id,
			    string1, string2, string3, string4) == 6) {
			if (stringtype (string1) == TYPE_STRING ||
			    stringtype (string3) == TYPE_STRING)
				continue;

			(void) strcpy (imglist[i].subject_id, subject_id);
			(void) strcpy (imglist[i].scanset1_id, scanset1_id);
			imglist[i].mean1 = atof (string1);
			(void) strcpy (imglist[i].scanset2_id, string2);
			imglist[i].mean2 = atof (string3);

/*
 * <subject_id><scanset1_id><mean1><scanset2_id><mean2><sxrid>
 */
			if (stringtype (string4) == TYPE_STRING &&
			    strstr (string4, SXR_EXT) != NULL) {
				(void) strcpy (imglist[i].sxrid, string4);

/*
 * <subject_id><scanset1_id><mean1><scanset2_id><mean2><weight>
 */
			} else if (stringtype (string4) == TYPE_REAL ||
				   stringtype (string4) == TYPE_INTEGER) {
				imglist[i].weight = atof (string4);
			}
			i++;
			continue;
		}
		if (sscanf (line, "%s %s %s %s %s", subject_id, scanset1_id,
			    string1, string2, string3) == 5) {

/*
 * <subject_id><scanset1_id><mean1><scanset2_id><mean2>
 */
			if (strstr (string3, SXR_EXT) == NULL) {
				if (stringtype (string1) == TYPE_STRING ||
				    stringtype (string3) == TYPE_STRING)
					continue;

				imglist[i].mean1 = atof (string1);
				(void) strcpy (imglist[i].scanset2_id, string2);
				imglist[i].mean2 = atof (string3);
			} else {

/*
 * <subject_id><scanset1_id><mean1><scanset2_id><sxrid>
 */
				if (stringtype (string1) == TYPE_REAL) {
					if (stringtype (string1) == TYPE_STRING)
						continue;
					imglist[i].mean1 = atof (string1);
					(void) strcpy (imglist[i].scanset2_id, string2);
					(void) strcpy (imglist[i].sxrid, string3);
				} else {

/*
 * <subject_id><scanset1_id><scanset2_id><mean2><sxrid>
 */
					if (stringtype (string2) == TYPE_STRING)
						continue;
					(void) strcpy (imglist[i].scanset2_id, string1);
					imglist[i].mean2 = atof (string2);
					(void) strcpy (imglist[i].sxrid, string3);
				}
			}
			(void) strcpy (imglist[i].subject_id, subject_id);
			(void) strcpy (imglist[i].scanset1_id, scanset1_id);
			i++;
			continue;
		}
		if (sscanf (line, "%s %s %s %s", subject_id, scanset1_id, string1, string2) == 4) {
			if (strstr (string2, SXR_EXT) != NULL) {
/*
 * <subject_id><scanset1_id><scanset2_id><sxrid>
 */
				if (stringtype (string1) != TYPE_REAL) {
					(void) strcpy (imglist[i].scanset2_id, string1);
					(void) strcpy (imglist[i].sxrid, string2);

/*
 * <subject_id><scanset1_id><mean1><sxrid>
 */
				} else {
					imglist[i].mean1 = atof (string1);
					(void) strcpy (imglist[i].sxrid, string2);
				}
			} else {
/*
 * <subject_id><scanset1_id><scanset2_id><mean2>
 */
				if (stringtype (string1) != TYPE_REAL) {
					(void) strcpy (imglist[i].scanset2_id, string1);
					imglist[i].mean2 = atof (string2);

/*
 * <subject_id><scanset1_id><mean1><scanset2_id>
 */
				} else {
					imglist[i].mean1 = atof (string1);
					(void) strcpy (imglist[i].scanset2_id, string2);
				}
			}

			(void) strcpy (imglist[i].subject_id, subject_id);
			(void) strcpy (imglist[i].scanset1_id, scanset1_id);
			i++;
			continue;
		}
		if (sscanf (line, "%s %s %s", subject_id, scanset1_id, string1) == 3) {
			(void) strcpy (imglist[i].subject_id, subject_id);
			(void) strcpy (imglist[i].scanset1_id, scanset1_id);
/*
 * <subject_id><scanset1_id><sxrid>
 */
			if (strstr (string1, SXR_EXT) != NULL)
				(void) strcpy (imglist[i].sxrid, string1);
/*
 * <subject_id><scanset1_id><scanset2_id>
 */
			else if (stringtype (string1) != TYPE_REAL)
				(void) strcpy (imglist[i].scanset2_id, string1);
/*
 * <subject_id><scanset1_id><mean1>
 */
			else
				imglist[i].mean1 = atof (string1);
			i++;
			continue;
		}
/*
 * <subject_id><scanset1_id>
 */
		if (sscanf (line, "%s %s", subject_id, scanset1_id) == 2) {
			(void) strcpy (imglist[i].subject_id, subject_id);
			(void) strcpy (imglist[i].scanset1_id, scanset1_id);
			i++;
		}
	}
	(void) fclose (fp);
	*lstsize = i;
	if (*lstsize == 0) {
		(void) fprintf (stderr, "ERROR [readimglist]: %s contains no image lists\n", lstfile);
		return (ERROR);
	}
	return (OK);
}
