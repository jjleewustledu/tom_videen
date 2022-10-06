/*$Id: checksxr.c,v 1.1 1993/10/29 16:47:00 ty7777 Exp fred $*/
/*$Log: checksxr.c,v $
 * Revision 1.1  1993/10/29  16:47:00  ty7777
 * Initial revision
 */
/*______________________________________________________________________________
File Name:	checksxr.c

Function:	PUBLIC int checksxr (PARAMETER_DATA parameter_data, IMAGE_LIST_DATA image_list,
			BOOLEAN demigrate, char *short_sxrfile, char *complete_sxrfile)

Arguments:	parameter_data:		data structure contains all the parameters from
					the parameter file. PARAMETER_DATA is defined in imgutil.h
		image_list:		data structure containing scan identification,
					activation scan number, and control scan number.
					IMAGE_LIST_DATA is defined in imgutil.h
		demigrate:		TRUE will cause the file to be demigrated;
					FALSE will not demigrate if file is found on
					optical disk.
		short_sxrfile:		short sxr file constructed. The user is responsible for
					allocating memory for this string. The recommended string
					size is MAXLINE ( = 256), returned.
		complete_sxrfile:	complete sxr file constructed. The user is responsible for
					allocating memory for this string. The recommended string
					size is MAXPATH ( = 1024), returned.

Return:		OK = 0, the sxr file can be found.
		ERROR = -1, the sxr file cannot be found.

Description:	Check if the sxr file contructed from an image_list exist.

Algorithm:
		if (the short sxr file cannot be found by using setpath ()) then
			return ERROR
		endif
		return OK

Called by:	checklst in checklst.c.
		the main program in sxrweld.c.

Calling:	get_finfo () in libhp.a.
		setpath () in libpetarc.a.

Author:		Tom Yang

History:	Created by Tom Yang on 06/30/1993

______________________________________________________________________________*/

#include <imgutil.h>

#ifndef lint
static char     rcsid[] = "$Header: /usr/local/src/archive/RCS/checksxr.c,v 1.1 1993/10/29 16:47:00 ty7777 Exp fred $";
#endif

PUBLIC int      checksxr (parameter_data, image_list, demigrate, short_sxrfile, complete_sxrfile)
	PARAMETER_DATA  parameter_data;
	IMAGE_LIST_DATA image_list;
	BOOLEAN         demigrate;
	char           *short_sxrfile;
	char           *complete_sxrfile;
{
	char            subject_id[MAXNAME];
	struct INFO_TYPE finfo;

/*
 * Construct the subject id.
 */
	strcpy (subject_id, parameter_data.species);
	strcat (subject_id, image_list.subject_id);

/*
 * Fill the finfo structure for searching in archive.
 */
	get_finfo (parameter_data, &finfo);

/*
 * Change to INDIVIDUAL and COMMON_DATA to search sxr file.
 */
	finfo.cat = INDIVIDUAL;
	finfo.type = COMMON_DATA;

/*
 * Check the sxr file
 */
	if (setpath (parameter_data.datapath, short_sxrfile, subject_id, &finfo,
		     complete_sxrfile, demigrate) != OK) {
		return ERROR;
	}
	return OK;
}
