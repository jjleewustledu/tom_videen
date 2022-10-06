/*$Id: filter_image.c,v 1.5 1996/04/03 17:54:43 ty7777 Exp $*/
/*$Log: filter_image.c,v $
 * Revision 1.5  1996/04/03  17:54:43  ty7777
 * Save before making any further changes.
 *
 * Revision 1.4  1995/12/14  21:52:03  ty7777
 * Added error message.
 *
 * Revision 1.3  1995/12/14  21:50:35  ty7777
 * Use NEWimglist.h and MAXTOKEN.
 *
 * Revision 1.2  1993/11/08  17:27:08  ty7777
 * Added -nt to command.
 *
 * Revision 1.1  1993/10/29  20:21:28  ty7777
 * Initial revision
 **/

/*______________________________________________________________________________
File Name:	filter_image.c

Function:	PUBLIC int filter_image (char *rawimage_name, int filter_type, 
		float cutoff, int order, char *filtered_image_name)

Arguments:	rawimage_name:		raw image name. 
		filter_type:		filter type.
		cutoff:			cutoff frequency.
		order:			order of Butterworth filters.
		filtered_image_name:	filtered image name.

Return:		SUCCEED = 0, successful.
		FAIL	= -1, failed.

Description:	This function will filter an image.

Called by:	template () in template.c of imgmean.
		filter3d.c in filter3d.

Calling:	

Author:		Tom Yang

History:	Created by Tom Yang on 07/27/1993

______________________________________________________________________________*/

#include <petutil/NEWimglist.h>

static char rcsid [] = "$Header: /home/petsun4/ty7777/src/libnpg/RCS/filter_image.c,v 1.5 1996/04/03 17:54:43 ty7777 Exp $";

PUBLIC int filter_image (rawimage_name, filter_type, cutoff, order, filtered_image_name)
	char	*rawimage_name;
	int	filter_type;
	float	cutoff;
	int	order;
	char	*filtered_image_name;
{
	char	cbuffer [MAXTOKEN];
	char	command [MAXPATH];	

	if (filter_type == FILTER_GAUSS)
	{
		strcpy (command, "gauss ");
	}
	else if (filter_type == FILTER_BUTTERWORTH)
	{
		strcpy (command, "butterworth ");
	}
	strcat (command, rawimage_name);
	strcat (command, " ");
	strcat (command, filtered_image_name);
	strcat (command, " ");

	sprintf (cbuffer, " %f", cutoff);
	strcat (command, cbuffer);

	if (filter_type == FILTER_BUTTERWORTH)
	{
		sprintf (cbuffer, " %d", order);
		strcat (command, cbuffer);
	}
	strcat (command, " -nr");

	if (system (command) != 0)
	{
		fprintf (stderr, "Error: %s\n", command);
		fprintf (stderr, "Error in filtering ECAT image %s\n", rawimage_name);
		return FAIL;
	}

	return SUCCEED;
}
