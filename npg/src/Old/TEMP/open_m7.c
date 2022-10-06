/*  $Id: open_m7.c,v 1.6 2002/03/13 21:56:41 tom Exp $
 *  $Log: open_m7.c,v $
 *  Revision 1.6  2002/03/13 21:56:41  tom
 *  lint cleaning
 *
 * Revision 1.5  2000/09/27  19:03:28  tom
 * *** empty log message ***
 *
 * Revision 1.4  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.3  1999/05/14  17:58:01  tom
 * eliminate warning
 *
 * Revision 1.2  1998/12/18  15:55:49  tom
 * initialize new structures under m7 to null
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		open_m7
 *  Date:           Mar 2003
 *  Author:         Tom Videen
 *	Description:	Get file pointer to Matrix 7 file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/open_m7.c,v 1.6 2002/03/13 21:56:41 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <image.h>
#include <libimage.h>
#include <endian.h>

int             open_m7 (file)
	ImageFile      *file;
{
	FILE           *fp;
	struct file    *fsr;
	struct directory *dir;
	char           *fname;
	char           *mode;
	char            cmd[MAXPATH];
	long            buffer[LENREC_L];
	int             i, j;
	int             recnum = 2;		 /* directory in record 2 */
	int             nrec = 1;

	fname = file->name;
	mode = file->mode;

	if (strcmp (mode, "w") == 0) {
		if (file->fp != NULL)
			(void) fclose (file->fp);
		if ((fp = fopen (file->name, "r")) != NULL) {
			(void) fclose (fp);
			(void) sprintf (cmd, "rm %s\n", fname);		/* delete existing file if write mode */
			(void) system (cmd);
		}
		if ((file->fp = fopen (fname, mode)) == NULL) {
			(void) fprintf (stderr, "ERROR [open_m7]: creating %s\n", fname);
			return (ERROR);
		}
		if ((dir = (struct directory *) calloc (1, (unsigned) sizeof (struct directory))) == NULL) {
			(void) fprintf (stderr, "ERROR [open_m7]: calloc directory structure for %s\n", fname);
			return (ERROR);
		}
		dir->hdr.numfree = DNOD_REC;
		dir->hdr.forptr = DIR_NUM_1;
		dir->hdr.bakptr = 0;
		dir->hdr.numused = 0;
		for (j = 0; j < DNOD_REC; j++)
			dir-> entry[j].flag = UNUSED;
		bcopy (dir, buffer, LENREC);

		/* CONSTCOND */
		if (__BYTE_ORDER == __LITTLE_ENDIAN) {
			swap_word ((short *) buffer, LENREC_S);
			swap_byte ((char *) buffer, LENREC);
		}
		if (wrtrec (file->fp, (char *) buffer, recnum, nrec) != 0) {
			(void) fprintf (stderr, "ERROR [open_m7]: writing directory to %s\n", fname);
			free (dir);
			return (ERROR);
		}
		(void) fclose (file->fp);
		free (dir);
		(void) strcpy (file->mode, "r+");
	}

/*	Open the file */

	if ((file->fp = fopen (fname, mode)) == NULL) {
		(void) fprintf (stderr, "ERROR [open_m7]: opening %s\n", fname);
		return (ERROR);
	}
	if (file->m7 == NULL) {
		if ((file->m7 = (M7header *) calloc (1, sizeof (M7header))) == NULL) {
			(void) fprintf (stderr, "ERROR [open_m7]: calloc mainheader\n");
			return (ERROR);
		}
		file->m7->fsr = NULL;
		file->m7->mh = NULL;
		file->m7->sh = NULL;
		file->m7->sc = NULL;
		file->m7->s3d = NULL;
		file->m7->nm = NULL;
		file->m7->nrm = NULL;
		file->m7->atn = NULL;
	}
	if (file->m7->fsr == NULL) {
		if ((file->m7->fsr = (struct file *) calloc (1, sizeof (struct file))) == NULL) {
			(void) fprintf (stderr, "ERROR [open_m7]: calloc file structure\n");
			return (ERROR);
		}
		fsr = file->m7->fsr;
		fsr->memsize = SIZE_DIR_MALLOC;
		fsr->dir = (struct directory *) calloc (1, (unsigned) (SIZE_DIR_MALLOC * sizeof (struct directory)));
		if (fsr->dir == NULL) {
			(void) fprintf (stderr, "ERROR [open_m7]: calloc directory structure\n");
			return (ERROR);
		}
		for (i = 0; i < fsr->memsize; i++)
			for (j = 0; j < DNOD_REC; j++)
				fsr->dir[i].entry[j].flag = UNUSED;
		fsr->dir[0].hdr.forptr = DIR_NUM_1;
		fsr->dir[0].hdr.bakptr = 0;
		fsr->lstrec = 0;
	}
	return (OK);
}
