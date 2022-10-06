/*$Id: mat_get_spec.c,v 1.1 1995/10/09 16:47:35 ty7777 Exp $*/
/*$Log: mat_get_spec.c,v $
 * Revision 1.1  1995/10/09  16:47:35  ty7777
 * Initial revision
 **/

/* @(#)mat_get_spec.c	2.1  10/14/91 */
/* 
*
*	Module:		mat_get_spec.c
*
*	Author:		E. Pearson
*
*	Date:		May 1991
*
*	Description:	Return the actual number of frames, planes, gates, and bed
*			positions in a file.
*
*			Inputs:	 filename of matrix file (any type)
*
*			Outputs:  number of frames (int *)
*				  number of planes (int *)
*				  number of gates  (int *)
*				  number of beds   (int *)
*
*				All the return paramters are 1 based.
*
*	Modifications:
*	
*			
*/

#include	<stdio.h>
#include	<sys/file.h>
#include	"matrix.h"

#ifndef lint
static char sccsid[]="@(#)mat_get_spec.c	2.1  10/14/91  Copyright(c) 1989-1991 CTI PET Systems, Inc.";
#endif

static char rcsid [] = "$Heasder$";

int mat_get_spec (file, num_frames, num_planes, num_gates, num_bed)
char *file;
int  *num_frames, *num_planes, *num_gates, *num_bed;
{
  struct MatDir matrixlist[5000];
  FILE *fopen(), *fptr;
  int status, num_matrices, i;
  struct Matval	matnum;

  /* initialization */
  status = 0;
  *num_frames = 0;
  *num_planes = 0;
  *num_gates = 0;
  *num_bed = 0;

  /* open the specified file */
  fptr = mat_open (file, "r");
  if (fptr != NULL)
  { 
    /* get the matrix entries */
    num_matrices = mat_list( fptr, matrixlist, 5000);

    for (i=0; i<num_matrices; i++)
    {
      mat_numdoc (matrixlist[i].matnum, &matnum);
 
     if (matnum.frame > *num_frames)
	(*num_frames)++;
    
      if (matnum.plane > *num_planes)
	(*num_planes)++;
    
      if (matnum.gate > *num_gates)
	(*num_gates)++;
        
      if (matnum.bed > *num_bed)
	(*num_bed)++;
    }

    /* bed is zero based in the matrix number, but all numbers returned from this
	function will be one based  */
    (*num_bed)++;
    mat_close (fptr);
  }
  else
    status = 1;

  return(status);
}
