/*$Id: searchpath.c,v 1.2 1993/11/03 20:10:09 ty7777 Exp fred $*/
/*$Log: searchpath.c,v $
 * Revision 1.2  1993/11/03  20:10:09  ty7777
 * Moved return to separate ifs.
 *
 * Revision 1.1  1993/10/29  16:42:07  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________________
File Name:	searchpath.c

Function:	int searchpath (char *datapath, char *short_filename,
		struct INFO_TYPE *finfo, char *complete_filename)

Arguments:	datapath:		path to be search first.
		short_filename:		file name in short format, e.g. p2208ho1.img.
		finfo:			structure INFO_TYPE
					(defined in /usr/local/include/petutil/archive.h)
					cat, type, attr and fltr must be defined;
					spec is not used.
		complete_filename:	complete file name, e.g.
			/data/patient/individual/group22/p2284/pet/953_wob_fb_ramp/p2284ho1.img.

Return Value:	OK = 0,	file is found.
		ERROR   = -1,	file is not found.

Algorithm (Pseudocode):

		if (datapath is a valid directory and
		short_filename is in the datapath directory and image type matches) then
			return complete_filename;
		else if (datapath is a valid directory and
		short_filename.Z is in datapath and uncompress in datapath succeeds and
		image type matches) then
			return complete_filename;
		else if (short_filename is in local directory and image type matches) then
			return complete_filename;
		else if (short_filename.Z is in local directory and uncompress succeeds and
		image type matches) then
			return complete_filename;
		else if (short_filename is in /tmp directory and image type matches) then
			return complete_filename;
		else if (short_filename.Z is in /tmp and uncompress in /tmp succeeds and
		image type matches) then
			return complete_filename;
		else
			return ERROR;
		endif

Description:	Determine the complete path where an image file or a data file can be found.
		PETT VI images, ECAT images, and .sxr files have been tested.

Called by:	setpath () in setpath.c

Calling:

Author:		Tom Yang

History:	Created by Tom Yang 10/28/1993

___________________________________________________________________________________*/

#include <stdio.h>
#include <string.h>
#include <imgutil.h>

static char     rcsid[] = "$Header: /usr/local/src/archive/RCS/searchpath.c,v 1.2 1993/11/03 20:10:09 ty7777 Exp fred $";

PUBLIC int      searchpath (datapath, short_filename, finfo, complete_filename)
	char           *datapath;
	char           *short_filename;
	struct INFO_TYPE *finfo;
	char           *complete_filename;
{
	char            compress_filename[MAXLINE];
	char            datapath_compress_filename[MAXPATH];
	char            datapath_short_filename[MAXPATH];
	char            datapath_uncompress_cmd[MAXPATH];
	char            tmp_compress_filename[MAXPATH];
	char            tmp_short_filename[MAXPATH];
	char            tmp_uncompress_cmd[MAXPATH];
	char            uncompress_cmd[MAXLINE];
	int             fd;
	int             fdcmp;

/*
 * Construct filenames which will be searched in the datapath, local, and the /tmp
 * directory.
 */
	strcpy (datapath_short_filename, datapath);
	strcat (datapath_short_filename, "/");
	strcat (datapath_short_filename, short_filename);
	strcpy (datapath_compress_filename, datapath_short_filename);
	strcat (datapath_compress_filename, ".Z");
	sprintf (datapath_uncompress_cmd, "uncompress %s", datapath_compress_filename);

	strcpy (compress_filename, short_filename);
	strcat (compress_filename, ".Z");
	sprintf (uncompress_cmd, "uncompress %s", compress_filename);

	strcpy (tmp_short_filename, "/tmp/");
	strcat (tmp_short_filename, short_filename);
	strcpy (tmp_compress_filename, tmp_short_filename);
	strcat (tmp_compress_filename, ".Z");
	sprintf (tmp_uncompress_cmd, "uncompress %s", tmp_compress_filename);

	if ((fd = open (datapath_short_filename, O_RDONLY)) != -1 &&
	    check_imageattr (datapath_short_filename, finfo) == OK) {	/* Found in the datapath
										 * directory */
		strcpy (complete_filename, datapath_short_filename);

		close (fd);
		return OK;
	} else if ((fdcmp = open (datapath_compress_filename, O_RDONLY)) != -1 &&
		   system (datapath_uncompress_cmd) == 0 &&
		   check_imageattr (datapath_short_filename, finfo) == OK) {	/* Found the compressed
											 * file in the datapath
											 * directory */
		strcpy (complete_filename, datapath_short_filename);

		if (fd != -1)
			close (fd);
		close (fdcmp);
		return OK;
	} else if ((fd = open (short_filename, O_RDONLY)) != -1 &&
		   check_imageattr (short_filename, finfo) == OK) {	/* Found in the local
										 * directory */
		strcpy (complete_filename, short_filename);

		if (fdcmp != -1)
			close (fdcmp);
		close (fd);
		return OK;
	} else if ((fdcmp = open (compress_filename, O_RDONLY)) != -1 &&
		   system (uncompress_cmd) == 0 &&
		   check_imageattr (short_filename, finfo) == OK) {	/* Found the compressed
										 * file in the local
										 * directory */
		strcpy (complete_filename, short_filename);

		if (fd != -1)
			close (fd);
		close (fdcmp);
		return OK;
	} else if ((fd = open (tmp_short_filename, O_RDONLY)) != -1 &&
		   check_imageattr (tmp_short_filename, finfo) == OK) {	/* Found in the /tmp
										 * directory */
		strcpy (complete_filename, tmp_short_filename);

		if (fdcmp != -1)
			close (fdcmp);
		close (fd);
		return OK;
	} else if ((fdcmp = open (tmp_compress_filename, O_RDONLY)) != -1 &&
		   system (tmp_uncompress_cmd) == 0 &&
		   check_imageattr (tmp_short_filename, finfo) == OK) {	/* Found the compressed
										 * file in the /tmp
										 * directory */
		strcpy (complete_filename, tmp_short_filename);

		if (fd != -1)
			close (fd);
		close (fdcmp);
		return OK;
	} else {				 /* Nowhere can be be found by using the current
						  * scheme */
		return ERROR;
	}
}
