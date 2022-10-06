/*$Id: readmat.c,v 1.3 1995/12/14 20:07:27 ty7777 Exp $*/
/*$Log: readmat.c,v $
 * Revision 1.3  1995/12/14  20:07:27  ty7777
 * Use NEWimglist.h and MAXTOKEN.
 *
 * Revision 1.2  1994/03/03  20:40:35  ty7777
 * Replace id with heasder.
 *
 * Revision 1.1  1993/12/30  22:07:48  ty7777
 * Initial revision
 **/

/*_________________________________________________________________
  File:		readmat.c

  Function:	int readmat (char *matrix_file, float *mmppix, 
		MATRIX_DATA *matrix_list, int *list_size);

  Arguments:	matrix_file:	matrix file name.	
		mmppix:		voxel size.
		matrix_list:	list of matrices.
		list_size:	number of matrices.

  Return Value:	SUCCEED:	successful;
		FAIL:		failed.

  Description:	This function reads the information contained in
		a matrix file.
  Calling:

  Called by:	BuildTransform () in BuildTransform.c.

  Author:	Tom Yang on 10/12/93. 
_________________________________________________________________*/

#include <unistd.h>
#include <petutil/NEWimglist.h>

#define	MMPPIX_LABEL	"mmppix:"	/* Label used for mmppix */

static char rcsid [] = "$Header: /home/petsun4/ty7777/src/libnpg/RCS/readmat.c,v 1.3 1995/12/14 20:07:27 ty7777 Exp $";

PUBLIC int readmat (matrix_file, mmppix, matrix_list, list_size)
	char			*matrix_file;
	float			*mmppix;
	MATRIX_DATA		*matrix_list;
	int			*list_size;
{
	FILE	*fp;
	char	mmppix_label [MAXTOKEN];
	char	scanid [MAXTOKEN];
	char	line [MAXLINE];
	float	alpha;
	float	beta;
	float	cx;
	float	cy;
	float	cz;
	float	dx;
	float	dy;
	float	dz;
	float	gamma;
	float	scale_factor;
	float	vx;
	float	vy;
	float	vz;
	int	i;
	int	num_scanned;

	/*
	 * Initialization.
	 */
	*list_size	= 0;
	mmppix [0]	= 0.0;
	mmppix [1]	= 0.0;
	mmppix [2]	= 0.0;

	/*
	 * Open matrix file.
	 */
	fp = fopen (matrix_file, "r");
	if (fp == (FILE *) NULL)
	{
		fprintf (stderr, "Error in opening 6-parameter matrix file %s\n", matrix_file);
		return FAIL;
	}

	/*
	 * Get number of lines containing transformation parameters.
	 */
	i	= 0;
	while (fgets (line, MAXLINE, fp) != NULL)
	{
		num_scanned	= sscanf (line, "%s %f %f %f %f %f %f %f %f %f %f", 
				scanid, &scale_factor, &dx, &dy, &dz, 
				&alpha, &beta, &gamma, &cx, &cy, &cz);
		if (num_scanned == 11)
		{
			i++;
		}
		else 
		{
			num_scanned	= sscanf (line, "%s %f %f %f",
					mmppix_label, &vx, &vy, &vz);
			if (num_scanned == 4 && strcmp (mmppix_label, MMPPIX_LABEL) == 0)
			{
				mmppix [0]	= vx;	
				mmppix [1]	= vy;	
				mmppix [2]	= vz;	
			}
		}
	}

	*list_size	= i;
	if (*list_size == 0 || *list_size > MAX_MATRIX_LIST_SIZE)
	{
		if (*list_size == 0)
		{
			fprintf (stderr, 
		"The matrix file %s contains no information on transformation.\n", matrix_file);
		}
		else if (*list_size > MAX_MATRIX_LIST_SIZE)
		{
			fprintf (stderr, 
			"Error: The matrix file %s contains %d lines of parameters\n", 
			matrix_file, *list_size);
			fprintf (stderr, 
		"       which is greater than the maximum number of allowed lines %d\n",
			MAX_MATRIX_LIST_SIZE);
		}
		return FAIL;
	}

	/*
	 * Go back to the begining of the file.
	 * Put the data on the list.
	 */
	fseek (fp, 0L, SEEK_SET);
	i	= 0;
	while (fgets (line, MAXLINE, fp) != NULL)
	{
		num_scanned	= sscanf (line, "%s %f %f %f %f %f %f %f %f %f %f", 
				scanid, &scale_factor, &dx, &dy, &dz, 
				&alpha, &beta, &gamma, &cx, &cy, &cz);
		if (num_scanned == 11)
		{
			strcpy (matrix_list [i].scanid, scanid);
			matrix_list [i].scale_factor		= scale_factor;
			matrix_list [i].displacement [0]	= dx;
			matrix_list [i].displacement [1]	= dy;
			matrix_list [i].displacement [2]	= dz;
			matrix_list [i].rotation_angle [0]	= deg2radian (alpha);
			matrix_list [i].rotation_angle [1]	= deg2radian (beta);
			matrix_list [i].rotation_angle [2]	= deg2radian (gamma);
			matrix_list [i].rotation_center [0]	= cx;
			matrix_list [i].rotation_center [1]	= cy;
			matrix_list [i].rotation_center [2]	= cz;
			i++;
		}
	}

	fclose (fp);

	return SUCCEED;
}
