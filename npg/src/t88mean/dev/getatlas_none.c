/*	$Id: getatlas_none.c,v 2.0 1996/05/10 21:58:42 tom Exp $
	$Log: getatlas_none.c,v $
 * Revision 2.0  1996/05/10  21:58:42  tom
 * new libimage
 * indexing from 1 on images and atlas
 *
 * Revision 1.2  1995/11/10  18:47:58  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.1  1995/08/31  21:58:40  tom
 * Initial revision
 * */
/* ===============================================================================
 *	Module:			getatlas_none.c
 *	Date:			31-Aug-95
 *	Author:			Tom Videen
 *	Description:	Set parameters for no atlas transformation.
 *					Take most of the parameters from an input image file.
 * ===============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>	
#include <libimage.h>
#include <atlas.h>			 /* BRAIN_ATLAS */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/t88mean/RCS/getatlas_none.c,v 2.0 1996/05/10 21:58:42 tom Exp $";
#endif

void            getatlas_none (atlas, scanname)
	BRAIN_ATLAS    *atlas;			 /* brain atlas definition */
	char           *scanname;

{
    ImageFile      *file;
    Index           indx;

    indx.frm = 1;
    indx.pln = 1;
    indx.gate = 1;
    indx.data = 0;
    indx.bed = 0;

	strcpy (atlas->name, "None");
    file = make_imgfile (scanname);
    if (open_imgfile (file) == ERROR) {
        fprintf (stderr, "ERROR [template]: Cannot open %s\n", scanname);
        exit (ERROR);
    }
    if (get_mh (file) == ERROR) {
        fprintf (stderr, "ERROR [template]: Cannot get MainHeader from %s\n", scanname);
        exit (ERROR);
    }
    if (get_sh (file, &indx) == ERROR) {
        fprintf (stderr, "ERROR [template]: Cannot read file subheader in %s\n", scanname);
		exit (ERROR);
	}
	atlas->nx = file->sh->dim1;
	atlas->ny = file->sh->dim2;
	atlas->nz = file->sh->dim3;
	atlas->pixel_size = file->sh->sample_size_1;
	atlas->slice_separation = file->sh->sample_size_3;
	atlas->z0 = 0;
	free_imgfile (file);
}
