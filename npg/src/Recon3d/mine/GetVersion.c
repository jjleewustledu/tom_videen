/*$Id: GetVersion.c,v 1.3 1995/02/17 21:45:32 ty7777 Exp $*/
/*$Log: GetVersion.c,v $
 * Revision 1.3  1995/02/17  21:45:32  ty7777
 * Deleted .c,v stuff.
 *
 * Revision 1.2  1995/02/17  17:00:32  ty7777
 * Get rid of .c,v in program name.
 *
 * Revision 1.1  1995/01/24  21:53:24  ty7777
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

#include <petutil/imglist.h>

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/GetVersion.c,v 1.3 1995/02/17 21:45:32 ty7777 Exp $";

extern int GetVersion (rcsid_string, program_version)
	char	*rcsid_string;
	char	*program_version;
{
	char	string [MAXLINE];
	char	program [MAXLINE];
	char	program_name [MAXLINE];
	char	*sbuffer;
	char	version [MAXLINE];
	int	num_scanned;

	memset (string, 0, MAXLINE);
	memset (program, 0, MAXLINE);
	memset (version, 0, MAXLINE);
	num_scanned	= sscanf (rcsid_string, "%s %s %s", string, program, version);
	if (num_scanned == 3)
	{
		if (strstr (string, "Header") != NULL)
		{
			sbuffer	= strrchr (program, '/');
			if (sbuffer != NULL)
				strcpy (program_name, sbuffer + 1);
			else
				strcpy (program_name, program);
		}
		else
		{
			strcpy (program_name, program);
		}


		sbuffer		= strrchr (program_name, '.');
		if (sbuffer && *sbuffer)
			strncpy (program_version, program_name, strlen (program_name)  - strlen (sbuffer));
		else
			strcpy (program_version, program_name);

		strcat (program_version, " ");
		strcat (program_version, version);
	}
	else
		return FAIL;

	return SUCCEED;
}
