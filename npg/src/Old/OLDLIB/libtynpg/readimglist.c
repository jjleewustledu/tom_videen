/*$Id: readimglist.c,v 1.15 1995/12/14 20:06:30 ty7777 Exp $*/
/*$Log: readimglist.c,v $
 * Revision 1.15  1995/12/14  20:06:30  ty7777
 * Use NEWimglist.h.
 *
 * Revision 1.14  1995/05/22  19:32:39  ty7777
 * Modified the module to let it read unlimited number of list.
 *
 * Revision 1.13  1995/01/06  18:28:33  ty7777
 * Added initialization values.
 *
 * Revision 1.12  1995/01/03  21:12:48  ty7777
 * Added processing for the weight column.
 *
 * Revision 1.11  1994/03/03  20:39:41  ty7777
 * Replace id with header.
 *
 * Revision 1.10  1994/01/18  16:32:51  ty7777
 * Fixed the sxr file construction bug.
 *
 * Revision 1.9  1993/09/03  17:00:58  ty7777
 * Change imglist1.h to imglist.h
 *
 * Revision 1.8  1993/09/02  14:24:07  ty7777
 * Added processing for n1wn2 and n1fn2vn3.
 *
 * Revision 1.7  1993/08/09  20:25:05  ty7777
 * Changed FAIL to -MAXINT for scan numbers that are not specified.
 *
 * Revision 1.6  1993/07/28  19:25:25  ty7777
 * Adding rcsid.
 *
 * Revision 1.5  1993/07/28  18:40:25  ty7777
 * Processing comments start with #.
 *
 * Revision 1.4  1993/07/23  20:33:26  ty7777
 * Processing activation images only.
 *
 * Revision 1.3  1993/07/23  19:59:57  ty7777
 * Processing comments lines.
 *
 * Revision 1.2  1993/07/23  15:37:52  ty7777
 * Second checking in.
 *
 * Revision 1.1  1993/07/06  18:13:28  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		readimglist.c

  Function:	int readimglist (char *image_list_file, IMAGE_LIST_DATA *image_list, 
			int *num_image_list)

  Arguments:	image_list_file,	file containing a list of image files, 
					means, etc., passed in.
		image_list,		an array of structures, each structure 
					for a list, returned.
		num_image_list,		total number of list in file, returned.

  Description:	This function reads an image list file and put the information 
		it contains in an array of structures. If the control number is 
		not specified, it is assigned -MAXINT to indicate that it is not
		assigned.

  Return:	SUCCEED for successful; FAIL for error.

  Reminder:	Every real number should contain at least a decimal point. 
		The sxr_id is used to find sxr files which include any characters
		other than "<species><subject_id>.sxr". If an sxr id is present, it
		must start with a letter and contains the string ".sxr".

  Algorithm:	if (a line contains 7 fields) 
		  <subject_id><scanset1_id><mean1><scanset2_id><mean2><weight><sxrid>
		else if (a line contains 6 fields) 
		  if (the last field is an integer or a real number)
		    <subject_id><scanset1_id><mean1><scanset2_id><mean2><weight>
		  else
		    <subject_id><scanset1_id><mean1><scanset2_id><mean2><sxrid>
		  endif
		else
		  Same as before.
		endif

  Author:	Tom Yang.

  History:	Created by Tom Yang on 05/18/93. 
___________________________________________________________________________*/

#include <values.h>
#include <petutil/NEWimglist.h>

static char rcsid [] = "$Headser$";

PUBLIC int readimglist (image_list_file, image_list, num_image_list)
	char		*image_list_file;
	IMAGE_LIST_DATA	*image_list;
	int		*num_image_list;
{
	FILE	*fp;
	char	line [MAXLINE];
	char	scanset1_id [MAXLINE];
	char	string1 [MAXLINE];
	char	string2 [MAXLINE];
	char	string3 [MAXLINE];
	char	string4 [MAXLINE];
	char	subject_id [MAXLINE];
	char	sxrid [MAXLINE];
	int	i;

	fp	= fopen (image_list_file, "r");

	i	= 0;
	while (fgets (line, MAXLINE, fp) != NULL)
	{
		if (sscanf (line, "%s %s", subject_id, scanset1_id) != 2 ||
		strncmp (subject_id, "#", 1) == 0)
			continue;

		/*
		 * Setting the default values if image list does not contain them.
		 */
		strcpy (image_list [i].scanset1_id, "");
		image_list [i].mean1	= 0.0;
		strcpy (image_list [i].scanset2_id, "");
		image_list [i].mean2	= 0.0;
		image_list [i].weight	= 1.0;
		strcpy (image_list [i].sxrid, "");

		if (sscanf (line, "%s %s %s %s %s %s %s", subject_id, scanset1_id, 
		string1, string2, string3, string4, sxrid) == 7)
		{
		/*
		 * <subject_id><scanset1_id><mean1><scanset2_id><mean2><weight><sxrid>
		 */
			if (DetermineType (string1) == DATA_TYPE_STRING ||
			DetermineType (string3) == DATA_TYPE_STRING ||
			DetermineType (string4) == DATA_TYPE_STRING ||
			strstr (sxrid, SXR_EXT) == NULL)
				continue;

			strcpy (image_list [i].subject_id, subject_id);
			strcpy (image_list [i].scanset1_id, scanset1_id);
			image_list [i].mean1	= atof (string1);
			strcpy (image_list [i].scanset2_id, string2);
			image_list [i].mean2	= atof (string3);			
			image_list [i].weight	= atof (string4);			
			strcpy (image_list [i].sxrid, sxrid);
			i++;

			continue;
		}

		if (sscanf (line, "%s %s %s %s %s %s", subject_id, scanset1_id, 
		string1, string2, string3, string4) == 6)
		{
			if (DetermineType (string1) == DATA_TYPE_STRING ||
			DetermineType (string3) == DATA_TYPE_STRING)
				continue;

			strcpy (image_list [i].subject_id, subject_id);
			strcpy (image_list [i].scanset1_id, scanset1_id);
			image_list [i].mean1	= atof (string1);
			strcpy (image_list [i].scanset2_id, string2);
			image_list [i].mean2	= atof (string3);			

			if (DetermineType (string4) == DATA_TYPE_STRING &&
			strstr (string4, SXR_EXT) != NULL)
			{
			/*
		 	 * <subject_id><scanset1_id><mean1><scanset2_id><mean2><sxrid>
		 	 */
				strcpy (image_list [i].sxrid, string4);
			}
			else if (DetermineType (string4) == DATA_TYPE_REAL ||
			DetermineType (string4) == DATA_TYPE_INTEGER)
			{
			/*
		 	 * <subject_id><scanset1_id><mean1><scanset2_id><mean2><weight>
		 	 */
				image_list [i].weight	= atof (string4);			
			}

			i++;

			continue;
		}

		if (sscanf (line, "%s %s %s %s %s", subject_id, scanset1_id, 
		string1, string2, string3) == 5) 
		{
			if (strstr (string3, SXR_EXT) == NULL)
			{
			/*
			 * <subject_id><scanset1_id><mean1><scanset2_id><mean2>   
			 */
				if (DetermineType (string1) == DATA_TYPE_STRING ||
				DetermineType (string3) == DATA_TYPE_STRING)
					continue;

				image_list [i].mean1	= atof (string1);
				strcpy (image_list [i].scanset2_id, string2);
				image_list [i].mean2	= atof (string3);			
			}
			else
			{
				if (DetermineType (string1) == DATA_TYPE_REAL)
				{
				/*
				 * <subject_id><scanset1_id><mean1><scanset2_id><sxrid>   
				 */
					if (DetermineType (string1) == DATA_TYPE_STRING)
						continue;
					image_list [i].mean1	= atof (string1);
					strcpy (image_list [i].scanset2_id, string2);
					strcpy (image_list [i].sxrid, string3);
				}
				else
				{
				/*
				 * <subject_id><scanset1_id><scanset2_id><mean2><sxrid>   
				 */
					if (DetermineType (string2) == DATA_TYPE_STRING)
						continue;
					strcpy (image_list [i].scanset2_id, string1);
					image_list [i].mean2	= atof (string2);
					strcpy (image_list [i].sxrid, string3);
				}
			}

			strcpy (image_list [i].subject_id, subject_id);
			strcpy (image_list [i].scanset1_id, scanset1_id);

			i++;

			continue;
		}

		if (sscanf (line, "%s %s %s %s", subject_id, scanset1_id, string1, string2) == 4) 
		{
			if (strstr (string2, SXR_EXT) != NULL)
			{
				if (DetermineType (string1) != DATA_TYPE_REAL)
				{
				/*
				 * <subject_id><scanset1_id><scanset2_id><sxrid>   
				 */
					strcpy (image_list [i].scanset2_id, string1);
					strcpy (image_list [i].sxrid, string2);
				}
				else
				{
				/*
				 * <subject_id><scanset1_id><mean1><sxrid>   
				 */
					image_list [i].mean1	= atof (string1);
					strcpy (image_list [i].sxrid, string2);
				}
			}
			else
			{
				if (DetermineType (string1) != DATA_TYPE_REAL)
				{
				/*
				 * <subject_id><scanset1_id><scanset2_id><mean2>   
				 */
					strcpy (image_list [i].scanset2_id, string1);
					image_list [i].mean2	= atof (string2);
				}
				else
				{
				/*
				 * <subject_id><scanset1_id><mean1><scanset2_id>   
				 */
					image_list [i].mean1	= atof (string1);
					strcpy (image_list [i].scanset2_id, string2);
				}
			}

			strcpy (image_list [i].subject_id, subject_id);
			strcpy (image_list [i].scanset1_id, scanset1_id);
			i++;

			continue;
		}

		if (sscanf (line, "%s %s %s", subject_id, scanset1_id, string1) == 3)
		{
			strcpy (image_list [i].subject_id, subject_id);
			strcpy (image_list [i].scanset1_id, scanset1_id);

			if (strstr (string1, SXR_EXT) != NULL)
			/*
			 * <subject_id><scanset1_id><sxrid>
			 */
				strcpy (image_list [i].sxrid, string1);
			else if (DetermineType (string1) != DATA_TYPE_REAL)
			/*
			 * <subject_id><scanset1_id><scanset2_id>
			 */
				strcpy (image_list [i].scanset2_id, string1);
			else
			/*
			 * <subject_id><scanset1_id><mean1>
			 */
				image_list [i].mean1	= atof (string1);

			i++;

			continue;
		}

		if (sscanf (line, "%s %s", subject_id, scanset1_id) == 2) 
		{
		/*
		 * <subject_id><scanset1_id>
		 */
			strcpy (image_list [i].subject_id, subject_id);
			strcpy (image_list [i].scanset1_id, scanset1_id);
			i++;
		}
	}

	fclose (fp);

	*num_image_list	= i;

	if (*num_image_list == 0)
	{
		fprintf (stderr, "Error: File %s contains no image lists.\n", image_list_file); 
		return FAIL;
	}

	return SUCCEED;
}
