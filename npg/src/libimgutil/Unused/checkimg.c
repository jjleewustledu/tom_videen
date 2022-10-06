/*$Id: checkimg.c,v 1.1 1993/10/29 16:45:03 ty7777 Exp fred $*/
/*$Log: checkimg.c,v $
 * Revision 1.1  1993/10/29  16:45:03  ty7777
 * Initial revision
 */
/*______________________________________________________________________________
File Name:	checkimg.c

Function:	PUBLIC int checkimg (PARAMETER_DATA parameter_data, IMAGE_LIST_DATA image_list,
			int image_mode, BOOLEAN demigrate, int num_imgfiles,
			char short_imgfiles[][MAXLINE], char complete_imgfiles[][MAXPATH])

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
					strings. The string size is MAXLINE ( = 256) defined in myvalues.h
		complete_imgfiles:	complete PET image files constructed from subject number,
					tracer, and scan number. The user of this function is
					responsible for allocating enough momory for this list of
					strings. The string size is MAXPATH ( = 1024) defined in
					/usr/local/include/petutil/archive.h.

Return:		OK = 0, all the image files can be found.
		ERROR = -1, at least one of the image files cannot be found.

Description:	Check if the image files contructed from an image_list exist.

Algorithm:
		set status to OK
		for (each image file in short_image_files) do
			if (the short image file cannot be found by using setpath ()) then
				set status to ERROR
			endif
		enddo
		return status

Called by:	The library function checklst () in libpetarc.a.

Calling:	get_finfo () in libhp.a.
		setpath () in libpetarc.a.

Author:		Tom Yang

History:	Created by Tom Yang on 06/30/1993

______________________________________________________________________________*/

#include <imgutil.h>

static char     rcsid[] = "$Header: /usr/local/src/archive/RCS/checkimg.c,v 1.1 1993/10/29 16:45:03 ty7777 Exp fred $";

PUBLIC int      checkimg (parameter_data, image_list, image_mode, demigrate, num_imgfiles,
			                  short_imgfiles, complete_imgfiles)
	PARAMETER_DATA  parameter_data;
	IMAGE_LIST_DATA image_list;
	int             image_mode;
	BOOLEAN         demigrate;
	int             num_imgfiles;
	char            short_imgfiles[][MAXLINE];
	char            complete_imgfiles[][MAXPATH];
{
	char            image_category[MAXNAME];
	char            subject_id[MAXNAME];
	int             i;
	int             status;
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
 * Check the individual images
 */
	if (image_mode == STDIMAGE_MODE)
		strcpy (image_category, "standard");
	else if (image_mode == DIFIMAGE_MODE)
		strcpy (image_category, "difference");
	else if (image_mode == PREWELDED_IMAGE_MODE)
		strcpy (image_category, "pre-welded");
	else if (image_mode == MULTITRACER_IMAGE_MODE)
		strcpy (image_category, "multi-tracer");

	status = OK;
	for (i = 0; i < num_imgfiles; i++) {
		if (setpath (parameter_data.datapath, short_imgfiles[i], subject_id, &finfo,
			     complete_imgfiles[i], demigrate) != OK) {
			fprintf (stderr, "Error: %s image file %s does not exist.\n",
				 image_category, short_imgfiles[i]);
			status = ERROR;
		}
	}
	return status;
}
