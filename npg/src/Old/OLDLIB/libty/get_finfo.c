/*$Id: get_finfo.c,v 1.5 1995/11/10 16:28:12 tom Exp $
/*$Log: get_finfo.c,v $
 * Revision 1.5  1995/11/10  16:28:12  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.4  1994/03/03  20:06:11  ty7777
 * Replace id with header.
 *
 * Revision 1.3  1993/09/03  17:00:33  ty7777
 * Same as the last one.
 *
 * Revision 1.2  1993/07/28  19:28:56  ty7777
 * Adding rcsid.
 *
 * Revision 1.1  1993/07/21  19:19:57  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		get_finfo.c

  Function:	int get_finfo (PARAMETER_DATA parameter_data, struct INFO_TYPE *finfo)

  Arguments:	parameter_data,	a structure defined in /usr/local/include/petutil/imglist.h,
				passed in.
		finfo,		a structure defined in /usr/local/include/petutil/archive.h,
				returned.

  Description:	This function fills the structure finfo from the parameters defined in
		parameter_data. The finfo structure will be used in searching files in
		the archive.

  Return:	SUCCEED for successful.

  Author:	Tom Yang.

  History:	Created by Tom Yang on 06/22/93. 
___________________________________________________________________________*/

#include <imglist.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libnpg/RCS/get_finfo.c,v 1.5 1995/11/10 16:28:12 tom Exp $";

PUBLIC int get_finfo (parameter_data, finfo) 
	PARAMETER_DATA		parameter_data;
	struct INFO_TYPE	*finfo;
{
	finfo->cat	= parameter_data.category;
	finfo->type	= parameter_data.datatype;
	strcpy (finfo->attr.scanner, parameter_data.scanner);
	strcpy (finfo->attr.acq, parameter_data.acquisition);
	strcpy (finfo->attr.recon, parameter_data.reconstruction);
	strcpy (finfo->attr.fltr, parameter_data.recfilter);

	return SUCCEED;
}
