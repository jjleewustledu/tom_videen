/*$Id: comment_info.c,v 1.8 1995/10/25 14:40:32 ty7777 Exp $*/
/*$Log: comment_info.c,v $
 * Revision 1.8  1995/10/25  14:40:32  ty7777
 * Same as the last version.
 *
 * Revision 1.7  1994/09/13  16:02:45  ty7777
 * Changed the default zorigin to 0.
 *
 * Revision 1.6  1994/05/21  05:23:47  ty7777
 * Set the default of the flip flag to FALSE again.
 *
 * Revision 1.5  1994/05/12  18:39:27  ty7777
 * Change the default value of flip to TRUE upon Avi's request.
 *
 * Revision 1.4  1994/04/15  16:25:45  ty7777
 * Added function declaration.
 *
 * Revision 1.3  1994/04/15  15:01:28  ty7777
 * Added processing for zorigin.
 *
 * Revision 1.2  1994/03/03  20:00:47  ty7777
 * Replace id with header for rcsid.
 *
 * Revision 1.1  1993/10/05  21:08:57  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		comment_info.c

  Function:	PUBLIC int comment_info (char *comment, COMMENT_INFO * comment_info_data)
		PUBLIC int info2comment (COMMENT_INFO comment_info_data, char *new_comment)

  Arguments:	comment:		study_description from the ECAT Main_header.
		comment_info_data:	information contained in the Main_header comment.

  Return:	SUCCEED:	successful; 
		FAIL:		error.

  Description:	This function gets the information contained in the comment part
		of the ECAT Main_header structure.

  Called by:	Ecat2Pett6 () and getrealimg () in libhp.a.	

  Calling:	

  Author:	Tom Yang

  History:	Created by Tom Yang on 10/05/93. 
___________________________________________________________________________*/

#include <petutil/petutil.h>

static char rcsid [] = "$Header: /home/petsun4/ty7777/src/libnpg/RCS/comment_info.c,v 1.8 1995/10/25 14:40:32 ty7777 Exp $";

PUBLIC int comment_info (comment, comment_info_data)
	char		*comment;
	COMMENT_INFO	*comment_info_data;
{
	char	label [MAXLINE];
	char	*pattern;
	float	scaling_factor;
	int	i;
	int	zorigin;

	/*
	 * Setting default values.
	 */
	comment_info_data->xflip_flag		= FALSE;
	comment_info_data->scale_flag		= FALSE;
	comment_info_data->scale		= 1.0;
	comment_info_data->gauss3d_filter	= FALSE;
	comment_info_data->butterworth3d_filter	= FALSE;
	comment_info_data->pett6_flag		= FALSE;
	comment_info_data->zorigin		= 0;

	if (comment && *comment)
	{
		pattern	= strstr (comment, XFLIP_LABEL);
		if (pattern && *pattern)
		{
			comment_info_data->xflip_flag	= TRUE;
		}

		pattern	= strstr (comment, SCALE_LABEL);
		if (pattern && *pattern &&
		sscanf (pattern, "%s %f", label, &scaling_factor) == 2)
		{
			comment_info_data->scale_flag	= TRUE;
			comment_info_data->scale	= scaling_factor;
		}

		pattern	= strstr (comment, GAUSS_FILTER_LABEL);
		if (pattern && *pattern)
		{
			comment_info_data->gauss3d_filter	= TRUE;
		}

		pattern	= strstr (comment, BUTTERWORTH_FILTER_LABEL);
		if (pattern && *pattern)
		{
			comment_info_data->butterworth3d_filter	= TRUE;
		}

		pattern	= strstr (comment, PETTVI_IMAGE_LABEL);
		if (pattern && *pattern)
		{
			comment_info_data->pett6_flag	= TRUE;
		}

		pattern	= strstr (comment, ZORIGIN_LABEL);
		if (pattern && *pattern &&
		sscanf (pattern, "%s %d", label, &zorigin) == 2)
		{
			comment_info_data->zorigin	= zorigin;
		}
	}

	return SUCCEED;
}

PUBLIC int info2comment (comment_info_data, new_comment)
	COMMENT_INFO	comment_info_data;
	char		*new_comment;
{
	BOOLEAN	first;
	char	comments [MAXLINE];
	char	scaling_factor [MAXLINE];
	char	zorigin [MAXLINE];

	first	= TRUE;
	if (comment_info_data.xflip_flag)
	{
		strcpy (comments, XFLIP_LABEL);
		first		= FALSE;
	}

	if (comment_info_data.scale_flag)
	{
		sprintf (scaling_factor, "%s %f", SCALE_LABEL, comment_info_data.scale);
		if (!first)
		{
			strcat (comments, " ");
			strcat (comments, scaling_factor); 
		}
		else
			strcpy (comments, scaling_factor); 
		first	= FALSE;
	}

	if (comment_info_data.zorigin > 0)
	{
		sprintf (zorigin, "%s %d", ZORIGIN_LABEL, comment_info_data.zorigin);
		if (!first)
		{
			strcat (comments, " ");
			strcat (comments, zorigin); 
		}
		else
			strcpy (comments, zorigin); 
		first	= FALSE;
	}

	if (comment_info_data.gauss3d_filter)
	{
		if (!first)
		{
			strcat (comments, " ");
			strcat (comments, GAUSS_FILTER_LABEL); 
		}
		else
			strcpy (comments, GAUSS_FILTER_LABEL); 
		first	= FALSE;
	}

	if (comment_info_data.butterworth3d_filter)
	{
		if (!first)
		{
			strcat (comments, " ");
			strcat (comments, BUTTERWORTH_FILTER_LABEL); 
		}
		else
			strcpy (comments, BUTTERWORTH_FILTER_LABEL); 
		first	= FALSE;
	}

	if (comment_info_data.pett6_flag)
	{
		if (!first)
		{
			strcat (comments, " ");
			strcat (comments, PETTVI_IMAGE_LABEL); 
		}
		else
			strcpy (comments, PETTVI_IMAGE_LABEL); 
		first	= FALSE;
	}

	strncpy (new_comment, comments, 32);

	return SUCCEED;
}
