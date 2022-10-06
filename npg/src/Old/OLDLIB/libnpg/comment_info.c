/*	$Id: comment_info.c,v 1.10 1995/11/10 23:29:58 tom Exp $
	$Log: comment_info.c,v $
 * Revision 1.10  1995/11/10  23:29:58  tom
 * *** empty log message ***
 * */
/* ===============================================================================
 *	Module:			comment_info.c
 *	Date:			10/05/93
 *	Author:			Tom Yang
 *	Description:	Get info from main_header comment field
 *	Return:			SUCCEED or Fail.
 *	History:
 *		10-Nov-95	TOV modification
 * ===============================================================================
 */
#include <petutil.h>

static char     rcsid[] = "$Header: /home/npggw/tom/src/libnpg/RCS/comment_info.c,v 1.10 1995/11/10 23:29:58 tom Exp $";

PUBLIC int      comment_info (comment, comment_info_data)
	char           *comment;			/* study_description from ECAT Main_header */
	COMMENT_INFO   *comment_info_data;	/* information in Main_header comment */
{
	char            label[MAXLINE];
	char           *pattern;
	float           scaling_factor;
	int             i;
	int             zorigin;

/*
 * Setting default values.
 */
	comment_info_data->xflip_flag = FALSE;
	comment_info_data->scale_flag = FALSE;
	comment_info_data->scale = 1.0;
	comment_info_data->gauss3d_filter = FALSE;
	comment_info_data->butterworth3d_filter = FALSE;
	comment_info_data->pett6_flag = FALSE;
	comment_info_data->zorigin = 0;

	if (comment && *comment) {
		pattern = strstr (comment, XFLIP_LABEL);
		if (pattern && *pattern) {
			comment_info_data->xflip_flag = TRUE;
		}
		pattern = strstr (comment, SCALE_LABEL);
		if (pattern && *pattern &&
		    sscanf (pattern, "%s %f", label, &scaling_factor) == 2) {
			comment_info_data->scale_flag = TRUE;
			comment_info_data->scale = scaling_factor;
		}
		pattern = strstr (comment, GAUSS_FILTER_LABEL);
		if (pattern && *pattern) {
			comment_info_data->gauss3d_filter = TRUE;
		}
		pattern = strstr (comment, BUTTERWORTH_FILTER_LABEL);
		if (pattern && *pattern) {
			comment_info_data->butterworth3d_filter = TRUE;
		}
		pattern = strstr (comment, PETTVI_IMAGE_LABEL);
		if (pattern && *pattern) {
			comment_info_data->pett6_flag = TRUE;
		}
		pattern = strstr (comment, ZORIGIN_LABEL);
		if (pattern && *pattern &&
		    sscanf (pattern, "%s %d", label, &zorigin) == 2) {
			comment_info_data->zorigin = zorigin;
		}
	}
	return SUCCEED;
}

PUBLIC int      info2comment (comment_info_data, new_comment)
	COMMENT_INFO   comment_info_data;	/* information in Main_header comment */
	char           *new_comment;		/* study_description from ECAT Main_header */
{
	BOOLEAN         first;
	char            comments[MAXLINE];
	char            scaling_factor[MAXLINE];
	char            zorigin[MAXLINE];

	first = TRUE;
	if (comment_info_data.xflip_flag) {
		strcpy (comments, XFLIP_LABEL);
		first = FALSE;
	}
	if (comment_info_data.scale_flag) {
		sprintf (scaling_factor, "%s %f", SCALE_LABEL, comment_info_data.scale);
		if (!first) {
			strcat (comments, " ");
			strcat (comments, scaling_factor);
		} else
			strcpy (comments, scaling_factor);
		first = FALSE;
	}
	if (comment_info_data.zorigin > 0) {
		sprintf (zorigin, "%s %d", ZORIGIN_LABEL, comment_info_data.zorigin);
		if (!first) {
			strcat (comments, " ");
			strcat (comments, zorigin);
		} else
			strcpy (comments, zorigin);
		first = FALSE;
	}
	if (comment_info_data.gauss3d_filter) {
		if (!first) {
			strcat (comments, " ");
			strcat (comments, GAUSS_FILTER_LABEL);
		} else
			strcpy (comments, GAUSS_FILTER_LABEL);
		first = FALSE;
	}
	if (comment_info_data.butterworth3d_filter) {
		if (!first) {
			strcat (comments, " ");
			strcat (comments, BUTTERWORTH_FILTER_LABEL);
		} else
			strcpy (comments, BUTTERWORTH_FILTER_LABEL);
		first = FALSE;
	}
	if (comment_info_data.pett6_flag) {
		if (!first) {
			strcat (comments, " ");
			strcat (comments, PETTVI_IMAGE_LABEL);
		} else
			strcpy (comments, PETTVI_IMAGE_LABEL);
		first = FALSE;
	}
	strncpy (new_comment, comments, 32);

	return SUCCEED;
}
