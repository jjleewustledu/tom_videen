/*$Id: comment_info.c,v 1.2 1995/10/31 14:54:20 tom Exp $*/
/*$Log: comment_info.c,v $
 * Revision 1.2  1995/10/31  14:54:20  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.1  1995/10/10  15:30:55  ty7777
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

#include <petutil.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libpetf77/RCS/comment_info.c,v 1.2 1995/10/31 14:54:20 tom Exp $";

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
	comment_info_data->zorigin		= 31;

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
