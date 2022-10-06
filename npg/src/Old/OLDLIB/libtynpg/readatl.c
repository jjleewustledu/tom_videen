/*$Id: readatl.c,v 1.3 1995/12/14 20:05:32 ty7777 Exp $*/
/*$Log: readatl.c,v $
 * Revision 1.3  1995/12/14  20:05:32  ty7777
 * Use NEWimglist.h.
 *
 * Revision 1.2  1994/03/03  20:36:31  ty7777
 * Replace id with header.
 *
 * Revision 1.1  1994/02/21  21:46:11  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		readatl.c

  Function:	int readatl (char *atlfile, BOOLEAN *atlas, int *nvoi, VOIDATA *voilist)

  Arguments:	atlfile,	atlas file name.
		atlas,		atlas, either HD6 or T88.
		nvoi,		total number of vois.
		voilist,	list containing the voi names and center coordinates.

  Return:	SUCCEED: successful; FAIL: error.

  Description:	This function reads an atlas file and put the 3D points on a
		list.

  Called by:	locate.c.	

  Calling:	None.	

  Author:	Tom Yang.

  History:	Created by Tom Yang on 02/02/94. 
___________________________________________________________________________*/

#include <petutil/NEWimglist.h>

static char rcsid [] = "$Header: /home/petsun4/ty7777/src/libnpg/RCS/readatl.c,v 1.3 1995/12/14 20:05:32 ty7777 Exp $";

PUBLIC int readatl (atlfile, atlas, nvoi, voilist)
	char		*atlfile;
	BOOLEAN		*atlas;
	int		*nvoi;
	VOIDATA		*voilist;
{
	FILE	*atlfp;
	char	line [MAXLINE];
	char	name [MAXLINE];
	float	x;
	float	y;
	float	z;
	int	nvoi_declared;
	int	type;

	atlfp	= fopen (atlfile, "r");
	if (atlfp == NULL)
	{
		fprintf (stderr, "Error in opening atlas file %s\n", atlfile);
		return FAIL;
	}

	/*
	 * Read the file type and atlas.
	 */
	if (fgets (line, MAXLINE, atlfp) == NULL 
	|| sscanf (line, "%d %s", &type, name) != 2)
	{
		fprintf (stderr, "Error in reading type and atlas from %s.\n", atlfile);
		return FAIL;
	}

	if (strncmp (name, "hd6", 3) == 0)
		*atlas	= HD6_BRAIN;
	else if (strncmp (name, "t88", 3) == 0)
		*atlas	= T88_BRAIN;
	else
	{
		fprintf (stderr, "Error: atlas %s currently not defined.\n", name);
		return FAIL;
	}

	/*
	 * read the description line.
	 */
	if (fgets (line, MAXLINE, atlfp) == NULL)
	{
		fprintf (stderr, "The atlas file %s contains no data.\n", atlfile);
		return FAIL;
	}

	/*
	 * Read in the number of volumes defined in the atlas file.
	 */
	if (fgets (line, MAXLINE, atlfp) == NULL 
	|| sscanf (line, "%d", &nvoi_declared) != 1)
	{
		fprintf (stderr, "Error in reading number of vois from %s.\n", atlfile);
		return FAIL;
	}

	*nvoi	= 0;
	while (fgets (line, MAXLINE, atlfp) != NULL && *nvoi < nvoi_declared)
	{
		if (sscanf (line, "%s %f %f %f", name, &x, &y, &z) == 4) 
		{
			voilist [*nvoi].center.x	= x;
			voilist [*nvoi].center.y	= y;
			voilist [*nvoi].center.z	= z;
			strcpy (voilist [*nvoi].voiname, name);
			(*nvoi)++;
		}
	}

	/*
	 * Check if the actual number is the same as the number defined in the
	 * atlas file.
	 */
	if (*nvoi != nvoi_declared)
	{
		fprintf (stderr, 
		"Error: the actual (%d) and the declared (%d) number of vois in %s do not match.\n", 
		*nvoi, nvoi_declared, atlfile);
		return FAIL;
	}

	fclose (atlfp);

	return SUCCEED;
}
