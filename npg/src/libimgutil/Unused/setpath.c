/*$Id: setpath.c,v 1.5 1993/11/03 20:12:23 ty7777 Exp $*/
/*$Log: setpath.c,v $
 */
/*___________________________________________________________________________________
File Name:	setpath.c

Function:	int setpath (char *datapath, char *short_filename, char *id,
		struct INFO_TYPE *finfo, char *complete_filename, BOOLEAN demigrate)

Arguments:	datapath:		path to be search first.

		short_filename:		file name in short format, e.g. p2208ho1.img.

		id:			subject id (p1234 or m445) or composite id.
					(pnp34 or mnp34).

		finfo:			structure INFO_TYPE
					(defined in /usr/local/include/petutil/archive.h)
					cat, type, attr and fltr must be defined;
					spec is not used.

		complete_filename:	complete file name, e.g.

			/data/patient/individual/group22/p2284/pet/953_wob_fb_ramp/p2284ho1.img.

		demigrate:		demigrate can have the value of 0 (FASLE) or 1 (TRUE).
					TRUE will cause any files migrated to optical storage to
					be moved back on to magnetic disk.

Return Value:	(defined in /usr/local/include/petutil/...)
		SUCCEED = 0,	file exists in a directory contained in the complete file name,
				or when demigrate is 0 and the file has been migrated to optical
				disk.
		ERROR   = -1,	some error condition prevented retrieve from locating or accessing
				the target directory or file.
		NWP     = 1,	no write permission, file could not be demigrated because this
				process does not have write permission in the target directory.
		FDE     = 4,	file doesn't exist.

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
		else if (file exists in the archive and
		it can be accessed from magnetic disk) then
			return complete_filename
		else if (file exists in the archive and has migrated to optical disk) then
			set complete_filename to NULL
		else
			return (error code)
		endif

Description:	Determine the complete path where an image file or a data file can be found.
		PETT VI images, ECAT images, and .sxr files have been tested.

Called by:	checkimg () in checkimg.c

Calling:	Library function in libpetarc.a
		retrieve ().

Author:		Tom Yang

History:	Created by Tom Yang on 03/04/1993

___________________________________________________________________________________*/

#include <stdio.h>
#include <string.h>
#include <imgutil.h>

#ifndef lint
static char     rcsid[] = "$Header: /home/petsun4/ty7777/src/archive/RCS/setpath.c,v 1.5 1993/11/03 20:12:23 ty7777 Exp $";
#endif

int      setpath (datapath, short_filename, id, finfo, complete_filename, demigrate)
	char           *datapath;
	char           *short_filename;
	char           *id;
	struct INFO_TYPE *finfo;
	char           *complete_filename;
	BOOLEAN         demigrate;
{
	char           *pathname;
	int             err;

	if (searchpath (datapath, short_filename, finfo, complete_filename) == SUCCEED) {
		return SUCCEED;
	} else if ((pathname = retrieve (short_filename, id, finfo, demigrate, &err))
		   != (char *) NULL && err == OK) {	/* Found in the archive path */
		strcpy (complete_filename, pathname);
		strcat (complete_filename, "/");
		strcat (complete_filename, short_filename);
		free (pathname);
		return SUCCEED;
	} else if (demigrate == FALSE && err == FEO) {
		complete_filename = (char *) NULL;
		if (pathname != (char *) NULL)
			free (pathname);
		return SUCCEED;
	} else {				 /* Nowhere can be be found by using the current
						  * scheme */
		if (pathname != (char *) NULL)
			free (pathname);
		return err;
	}
}
