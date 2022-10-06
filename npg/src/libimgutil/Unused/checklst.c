/*$Id: checklst.c,v 1.1 1993/10/29 16:46:11 ty7777 Exp fred $*/
/*$Log: checklst.c,v $
 * Revision 1.1  1993/10/29  16:46:11  ty7777
 * Initial revision
 */
/*______________________________________________________________________________
File Name:	checklst.c

Function:	PUBLIC int checklst (PARAMETER_DATA parameter_data, IMAGE_LIST_DATA image_list,
			int image_mode, BOOLEAN demigrate, int num_imgfiles,
			char short_imgfiles[][MAXLINE], char complete_imgfiles[][MAXPATH],
			char *short_sxrfile, char *complete_sxrfile)

Arguments:	parameter_data:		data structure contains all the parameters from
					the parameter file. PARAMETER_DATA is defined in imgutil.h
		image_list:		data structure containing scan identification,
					activation scan number, and control scan number.
					IMAGE_LIST_DATA is defined in imgutil.h
		image_mode:		image mode. Used here to determine the image
					category.
		demigrate:		TRUE will cause the file to be demigrated;
					FALSE will not demigrate if file is found on
					optical disk.
		num_imgfiles:		number of image files, returned.
		short_imgfiles:		short PET image files constructed from subject number,
					tracer, and scan number. The user of this function is
					responsible for allocating enough momory for this list of
					strings. The string size is MAXLINE ( = 256) defined in
					/usr/local/include/petutil/petutil.h.
		complete_imgfiles:	complete PET image files constructed from subject number,
					tracer, and scan number. The user of this function is
					responsible for allocating enough momory for this list of
					strings. The string size is MAXPATH ( = 1024) defined in
					/usr/local/include/petutil/archive.h.
		short_sxrfile:		short sxr file constructed. The user is responsible for
					allocating memory for this string. The recommended string
					size is MAXLINE ( = 256), returned.
		complete_sxrfile:	complete sxr file constructed. The user is responsible for
					allocating memory for this string. The recommended string
					size is MAXPATH ( = 1024), returned.

Return:		OK = 0, all the image files and the sxr file can be found.
		ERROR = -1, at least one of the image files or the sxr file cannot be found.

Description:	Check if the image files and the sxr file contructed from an image_list exist.

Algorithm:
		for (checkimg () return ERROR) do
			print error message
			return ERROR
		enddo
		for (checksxr () return ERROR) do
			print error message
			return ERROR
		enddo
		return OK

Called by:	The main program chklist.c.

Calling:	checkimg () and checksxr () in libpetarc.a.

Creator:	Tom Yang

History:	Created by Tom Yang on 06/30/1993

______________________________________________________________________________*/

#include <imgutil.h>
#include <myvalues.h>

static char     rcsid[] = "$Header: /usr/local/src/archive/RCS/checklst.c,v 1.1 1993/10/29 16:46:11 ty7777 Exp fred $";

PUBLIC int      checklst (parameter_data, image_list, image_mode, demigrate, num_imgfiles,
	                       short_imgfiles, complete_imgfiles, short_sxrfile, complete_sxrfile)
	PARAMETER_DATA  parameter_data;
	IMAGE_LIST_DATA image_list;
	int             image_mode;
	BOOLEAN         demigrate;
	int             num_imgfiles;
	char            short_imgfiles[][MAXLINE];
	char            complete_imgfiles[][MAXPATH];
	char           *short_sxrfile;
	char           *complete_sxrfile;
{
	if (checkimg (parameter_data, image_list, image_mode, demigrate, num_imgfiles,
		      short_imgfiles, complete_imgfiles) == ERROR) {
		fprintf (stderr, "Error in checkimg.\n");
		return ERROR;
	}
	if (checksxr (parameter_data, image_list, demigrate, short_sxrfile,
		      complete_sxrfile) == ERROR) {
		fprintf (stderr, "Warning: sxr data file %s does not exist.\n", short_sxrfile);
		return ERROR;
	}
	return OK;
}
