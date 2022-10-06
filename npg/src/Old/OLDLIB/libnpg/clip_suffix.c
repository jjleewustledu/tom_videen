/*$Id: clip_suffix.c,v 1.2 1995/11/10 16:27:59 tom Exp $*/
/*$Log: clip_suffix.c,v $
 * Revision 1.2  1995/11/10  16:27:59  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.1  1995/06/13  19:38:17  ty7777
 * Initial revision
 **/

/* Copyright 1993 Roger P. Woods, M.D. */
/* Modified: 7/15/93 */

/************************************************************************/
/*void clip_suffix							*/
/*									*/
/*This routine will clip the .x (e.g. .img) suffix from a file name	*/
/************************************************************************/

#include <AIR.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libnpg/RCS/clip_suffix.c,v 1.2 1995/11/10 16:27:59 tom Exp $";

void clip_suffix (filename)
char *filename;
{
	/* If filename contains a '.' */
	if (!(strrchr(filename,'.')==NULL)){
		/* if filename contains a '/' */
		if(!(strrchr(filename,'/')==NULL)){
			/* if '/' comes after '.' */
			if(strrchr(filename,'/')>strrchr(filename,'.')){
				return;
			}
		}
		filename[strlen(filename)-strlen(strrchr(filename,'.'))]='\0';
	}
}



