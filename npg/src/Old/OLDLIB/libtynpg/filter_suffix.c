/*$Id: filter_suffix.c,v 1.4 1995/12/14 21:53:17 ty7777 Exp $*/
/*$Log: filter_suffix.c,v $
 * Revision 1.4  1995/12/14  21:53:17  ty7777
 * Use NEWimglist.h and MAXTOKEN.
 *
 * Revision 1.3  1993/10/29  20:22:18  ty7777
 * Changed construction so that is compatible with the gauss ot butterworth script.
 *
 * Revision 1.2  1993/09/03  17:00:16  ty7777
 * Change imglist1.h to imglist.h
 *
 * Revision 1.1  1993/08/12  14:54:25  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________________
File Name:	filter_suffix.c

Function:	PUBLIC void filter_suffix (int filter_type, float cutoff, int order, char *suffix)

Arguments:	filter_type:	filter type. Possible values are FILTER_NONE,
				FILTER_GAUSS, and FILTER_BUTTERWORTH.
		cutoff:		cutoff frequency.
		order:		order of the Butterworth 3D filter. Not used for Gaussian 3D
				filter.
		suffix:		suffix returned. The string size is MAXTOKEN (32). The user of the
				function is responsible for allocating memory fot the suffix string.

Return:		None.

Description:	Construct a suffix string to be appended to a filtered image.

Algorithm: 
		if (filter_type is FILTER_GAUSS) then
			copy "_g" to suffix
		else if (filter_type is FILTER_BUTTERWORTH) then
			copy "_5b" to suffix where 5 is the order
		else
			copy null string to suffix
			return
		endif
		multiply the cutoff by a predefined multiplier and convert it to an integer
		convert the magnified cutoff to a string
		concatenate the cutoff string to suffix

Called by:	

Calling:	itoa () in itoa.c.			

Creator:	Tom Yang

History:	Created by Tom Yang on 06/28/1993

___________________________________________________________________________________*/

#include <petutil/NEWimglist.h>

static char rcsid [] = "$Header: /home/petsun4/ty7777/src/libnpg/RCS/filter_suffix.c,v 1.4 1995/12/14 21:53:17 ty7777 Exp $";


PUBLIC void filter_suffix (filter_type, cutoff, order, suffix)
	int	filter_type;
	float	cutoff;
	int	order;
	char	*suffix;
{
	char	cbuffer [MAXTOKEN];
	float	magnified_cutoff;
	int	whole_magnified_cutoff;

	if (filter_type == FILTER_GAUSS)
	{
		strcpy (suffix, "_g");
	}
	else if (filter_type == FILTER_BUTTERWORTH)
	{
		sprintf (cbuffer, "_%db", order);
		strcpy (suffix, cbuffer);
	}
	else
	{
		strncpy (suffix, "\0", MAXTOKEN);
		return;
	}

	magnified_cutoff	= cutoff * CUTOFF_MAGNIFIER;
	whole_magnified_cutoff	= (int) magnified_cutoff;
	if (magnified_cutoff == (float) whole_magnified_cutoff)
		itoa (whole_magnified_cutoff, cbuffer);
	else
		sprintf (cbuffer, ".2f", magnified_cutoff);
	strcat (suffix, cbuffer);
}
