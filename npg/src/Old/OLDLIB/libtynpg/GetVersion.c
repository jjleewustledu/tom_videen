/*$Id: GetVersion.c,v 1.6 1995/12/14 19:57:46 ty7777 Exp $*/
/*$Log: GetVersion.c,v $
 * Revision 1.6  1995/12/14  19:57:46  ty7777
 * Use NEWimglist.h
 *
 * Revision 1.5  1994/11/08  21:22:02  ty7777
 * Working version.
 *
 * Revision 1.4  1994/11/08  20:28:47  ty7777
 * Added processing for Header.
 *
 * Revision 1.3  1994/03/03  19:56:25  ty7777
 * Replace id with header.
 *
 * Revision 1.2  1993/09/15  15:21:31  ty7777
 * Change the header file.
 *
 * Revision 1.1  1993/09/15  14:27:37  ty7777
 * Initial revision
 **/

/*_______________________________________________________________________
File:		GetVersion.c

Function:	int GetVersion (char *rcsid_string, char *program_version)

Arguments:	rcsid_string:		string containing RCS Header information.
		program_version:	main program name with version.

Return Value:	SUCCEED:	program_version is obtained successfully.
		FAIL:		rcsid_string does noe have the correct format.

Description:	This function gets the program name with version number from
		the rcsid string.
_______________________________________________________________________*/

#include <petutil/NEWimglist.h>

static char rcsid [] = "$Header: /home/petsun4/ty7777/src/libnpg/RCS/GetVersion.c,v 1.6 1995/12/14 19:57:46 ty7777 Exp $";

PUBLIC int GetVersion (rcsid_string, program_version)
	char	*rcsid_string;
	char	*program_version;
{
	char	cbuffer [MAXLINE];
	char	program [MAXLINE];
	char	*tail;
	char	version [MAXLINE];
	int	num_scanned;

	memset (cbuffer, 0, MAXLINE);
	memset (program, 0, MAXLINE);
	memset (version, 0, MAXLINE);
	num_scanned	= sscanf (rcsid_string, "%s %s %s", cbuffer, program, version);
	if (num_scanned == 3)
	{
		if (strstr (cbuffer, "Header") != NULL)
		{
			tail	= strrchr (program, '/');
			if (tail != NULL)
				strcpy (program_version, tail + 1);
			else
				strcpy (program_version, program);
		}
		else
		{
			strcpy (program_version, program);
		}
		strcat (program_version, " ");
		strcat (program_version, version);
	}
	else
		return FAIL;

	return SUCCEED;
}
