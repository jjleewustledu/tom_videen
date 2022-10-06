/*$Id: build_filter_filename.c,v 1.2 1995/10/30 22:39:01 tom Exp $*/
/*$Log: build_filter_filename.c,v $
 * Revision 1.2  1995/10/30  22:39:01  tom
 * change include to remove "petutil"
 *
 * Revision 1.1  1995/10/13  17:12:29  ty7777
 * Initial revision
 **/

/*______________________________________________________________________________
File Name:	build_filter_filename.c

Function:	PUBLIC void build_filter_filename (char *rawimage_name, 
		int filter_type, float cutoff, int order, char *filtered_image_name)

Arguments:	rawimage_name:		raw image file name.
		filter_type:		filter type.
		cutoff:			cutoff frequence.
		order:			order of a Butterworth filter.
		filtered_image_name:	filter image file name.

Return:		None.

Description:	This function constructs a filter file name based on the filter
		mode and raw image name.

Called by:	The template () in template.c of imgmean.
		filter3d.c in filter3d.

Calling:	

Creator:	Tom Yang

History:	Created by Tom Yang on 07/27/1993

______________________________________________________________________________*/

#include <imglist.h>
#include <matrix7.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libhp/RCS/build_filter_filename.c,v 1.2 1995/10/30 22:39:01 tom Exp $";

PUBLIC void build_filter_filename (rawimage_name, filter_type, cutoff, order, 
	filtered_image_name)
	char	*rawimage_name;
	int	filter_type;
	float	cutoff;
	int	order;
	char	*filtered_image_name;
{
	char	cbuffer [MAXNAME];
	char	*extension;
	char	short_filename [MAXLINE];
	char	*pstring;
	int	magnified_cutoff;

	/*
	 * Get the short_filename.
	 */
	strncpy (short_filename, "\0", MAXLINE);
	if ((pstring = strrchr (rawimage_name, '/')) != NULL) 
	{
		strcpy (short_filename, pstring + 1);
	}
	else
		strcpy (short_filename, rawimage_name);

	/*
	 * Get the root of the short file name.
	 */
	if ((extension = strrchr (short_filename, '.')) != NULL &&
	strlen (extension) < strlen (short_filename))
	{
		strncpy (filtered_image_name, short_filename,
			strlen (short_filename) - strlen (extension));
	}
	else
		strcpy (filtered_image_name, short_filename);

	filter_suffix (filter_type, cutoff, order, cbuffer);
	strcat (filtered_image_name, cbuffer);
	strcat (filtered_image_name, IMAGE_EXT);
}
