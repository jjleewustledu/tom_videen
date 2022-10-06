/*  $Id: getmatpos.c,v 1.3 2002/12/17 21:55:15 tom Exp $
    $Log: getmatpos.c,v $
 * Revision 1.3  2002/12/17  21:55:15  tom
 * fix pointer/cast errors found with gcc
 *
    Revision 1.2  1999/12/13 17:45:50  tom
    tov revision

 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		getmatpos
 *  Date:           Nov-95
 *  Author:         John Ollinger
 *	Description:	Get beginning and ending record numbers of matrix.
 *	History:
 *		Mod Mar-95	TOV
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/getmatpos.c,v 1.3 2002/12/17 21:55:15 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <image.h>
#include <libimage.h>

int             getmatpos (file, matnum, endrec)
	ImageFile      *file;
	int             matnum;			 /* Matrix number of desired matrix, sinogram etc. */
	int            *endrec;			 /* Ending record number of matrix. 		 */
{
	struct file    *fsr;
	int             begrec, irec, memsize_old, lenrd, recnum;
	int             i, j;

/*
 *	Loop through all directory entries in RAM.
 *	Entry #0 is directory header, skip it.
 */
	fsr = file->m7->fsr;
	for (irec = 1; irec <= fsr->lstrec; irec++) {
		for (i = 0; i < DNOD_REC; i++) {
			if (fsr->dir[irec].entry[i].matnum == matnum) {
				begrec = (int) fsr->dir[irec].entry[i].begrec;
				*endrec = (int) fsr->dir[irec].entry[i].endrec;
				return (begrec);
			}
		}
	}
/*
 * Matrix not found in memory-resident directory entries.
 * Are there more more entries on disk? If so, read into memory.
 * First, check to see that pointers are initialized.
 */
	if (fsr->dir[0].hdr.forptr == 0)
		fsr->dir[0].hdr.forptr = DIR_NUM_1;

	while (((fsr->dir[fsr->lstrec].hdr.forptr != DIR_NUM_1) & (fsr->lstrec > 0)) | (fsr->lstrec == 0)) {
		if (fsr->lstrec > MAX_REC) {
			(void) fprintf (stderr, "ERROR [getmatpos]: Exceeded buffer space to store directory entries.\n");
			--fsr->lstrec;
			return (ERROR);
		}
		if (fsr->lstrec >= fsr->memsize - 1) {	/* Allocate more memory */
			memsize_old = fsr->memsize;
			fsr->memsize += SIZE_DIR_MALLOC;
			fsr->dir = (struct directory *)
				realloc ((char *) fsr->dir, (size_t) (fsr->memsize * sizeof (struct directory)));
			if (fsr->dir == NULL) {
				(void) fprintf (stderr, "ERROR [getmatpos]: Error allocating directory\n");
				return (ERROR);
			}
			for (i = memsize_old; i < fsr->memsize; i++)	/* Initialize structure */
				for (j = 0; j < DNOD_REC; j++)
					fsr->dir[i].entry[j].flag = UNUSED;
		}
		if (fsr->lstrec == 0)
			recnum = DIR_NUM_1;
		else
			recnum = (int) fsr->dir[fsr->lstrec].hdr.forptr;

		++fsr->lstrec;			 /* Increment number of last record read from disc.	 */
		lenrd = 1;			 /* Read one record.	 */
		if (rdrec (file->fp, (char *) &fsr->dir[fsr->lstrec], recnum, lenrd) != 0) {
			(void) fprintf (stderr, "ERROR [getmatpos]: Error reading directory\n");
			return (ERROR);
		}

/* Search newly read entries for matrix. */

		for (i = 0; i < DNOD_REC; i++) {
			if (fsr->dir[fsr->lstrec].entry[i].matnum == matnum) {
				begrec = (int) fsr->dir[fsr->lstrec].entry[i].begrec;
				*endrec = (int) fsr->dir[fsr->lstrec].entry[i].endrec;
				return (begrec);
			}
		}
	}

/* Matrix not found, return error. */

	return (ERROR);
}
