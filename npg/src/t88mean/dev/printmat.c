/* $Log$ */
/*	===============================================================================
 *	Module:			printmat.c
 *	Date:			Dec-96
 *	Author:			Tom Videen
 *	Description:
 *		Print matrix to file
 *	===============================================================================
 */
static char     rcsid[] = "$Header: /home/npggw/tom/src/t88mean/RCS/t88mean.c,v 3.40 1996/11/21 17:56:00 tom Exp tom $";

#include <stdio.h>
#include <stdlib.h>
 
void printmat (fp, string, mat)
	FILE           *fp;
	char           *string;
	float         **mat;
{
	fprintf (fp, "Matrix:  %s\n", string);
	fprintf (fp, "Matrix: %9.3f %9.3f %9.3f %9.3f\n", mat[1][1], mat[1][2], mat[1][3], mat[1][4]);
	fprintf (fp, "Matrix: %9.3f %9.3f %9.3f %9.3f\n", mat[2][1], mat[2][2], mat[2][3], mat[2][4]);
	fprintf (fp, "Matrix: %9.3f %9.3f %9.3f %9.3f\n", mat[3][1], mat[3][2], mat[3][3], mat[3][4]);
	fprintf (fp, "Matrix: %9.3f %9.3f %9.3f %9.3f\n", mat[4][1], mat[4][2], mat[4][3], mat[4][4]);
}
