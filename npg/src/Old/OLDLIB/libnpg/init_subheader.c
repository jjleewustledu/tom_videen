/*$Id: init_subheader.c,v 1.4 1995/11/10 16:28:36 tom Exp $*/
/*$Log: init_subheader.c,v $
 * Revision 1.4  1995/11/10  16:28:36  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.3  1995/10/13  17:17:10  ty7777
 * Use matrix7.h.
 *
 * Revision 1.2  1994/03/03  20:25:35  ty7777
 * Replace id with header.
 *
 * Revision 1.1  1993/03/03  20:36:19  ty7777
 * Initial revision
 **/

#ifndef lint
static char sccsid[]="@(#)init_image_subheader.c	1/14/93 Copyright Neural Pet Group, Washington University 1992";
#endif
static char rcsid [] = "$Header: /home/npggw/tom/src/libnpg/RCS/init_subheader.c,v 1.4 1995/11/10 16:28:36 tom Exp $";

/*___________________________________________________________________________________
 *
 *	File Name:	init_image_subheader.c
 *	Function Name:	matrixx_init_image_subheader
 *	Arguments:	image_subheader_proto:	prototype image subheader to be copied.
 *						gate, data, and bed numbers. This pointer
 *						can be set before calling or can be NULL.
 *	Return Value:	Image_subheader *:	Pointer to a Image_subheader structure.
 *						If the image_subheader_proto pointer is
 *						NULL, all the fields of the structure 
 *						pointed by the returned Image_subheader 
 *						pointer is set to either 0 or NULL string.
 *						Free memory after the pointer is no 
 *						longer used with the free () function 
 *						from the C library.
 *	Description:	Create and initialize an image subheader structure. 
 *	Called by:	filter.c in filter.bin
 *	Calling:	calloc (), bcopy ().
 *	Author:		Tom Yang.
 *	History:	Created by Tom Yang on 01/14/1993.
 *
___________________________________________________________________________________*/

#include <petutil.h>
#include <matrix7.h>

Image_subheader *matrixx_init_image_subheader (image_subheader_proto)
	Image_subheader	*image_subheader_proto;
{
	Image_subheader	*image_subheader;

	image_subheader = (Image_subheader*) calloc (1, sizeof (Image_subheader));
	if (image_subheader_proto)
		bcopy (image_subheader_proto, image_subheader, sizeof (Image_subheader));
	return image_subheader;
}
