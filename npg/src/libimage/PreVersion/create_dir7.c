/*	$Log: create_dir7.c,v $
 * Revision 1.4  2002/03/13  21:56:41  tom
 * lint cleaning
 *
 * Revision 1.3  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.2  1997/05/13  14:51:20  tom
 * fix bug in creation of >32 frame files
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 *	$Id: create_dir7.c,v 1.4 2002/03/13 21:56:41 tom Exp $ */
/* =============================================================================
 *	Function:		create_dir7
 *	Description:	Create directory entry for an open file.
 *	Returns:		Beginning record
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/create_dir7.c,v 1.4 2002/03/13 21:56:41 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <image.h>
#include <libimage.h>

int             create_dir7 (file, matnum, length)
	ImageFile      *file ;
	int             matnum;		 	 /* Matrix number of desired matrix */
	int             length;		 	 /* Length of matrix to be created in records */
{
	FILE           *fp;			 	 /* file pointer */
	struct file    *fsr;
	int             recnum;
	int             begrec;		 	 /* Beginning record number of matrix */
	int             current_rec; 	 /* Current record number */
	int             lenrd;
	int             more;	 		 /* TRUE: More records to search */
	int             memsize_old;
	int             irec = 0;
	int             highrec = 2;
	int             free1 = FALSE;	 /* Flag for first free entry */
	int             ifree = 0;		 /* Index to first free entry */
	int             i, j;

	if (file->fp == NULL) {
		fprintf (stderr, "ERROR [create_dir7]: No open file %s\n", file->name);
		return (ERROR);
	}
	fp = file->fp;
	if (file->m7 == NULL) {
		fprintf (stderr, "ERROR [create_dir7]: Version 7 header is undefined\n");
		return (ERROR);
	}
	if (file->m7->fsr == NULL) {
		fprintf (stderr, "ERROR [create_dir7]: Version 7 file is not open\n");
		return (ERROR);
	}
	fsr = file->m7->fsr;

/* Search directory entries on disk if not last dir rec. */

	more = TRUE;
	while (more == TRUE) {
		if (fsr->lstrec > MAX_REC) {
			fprintf (stderr, "ERROR [create_dir7]: Exceeded buffer space for directory entries\n");
			return (ERROR);
		}
		if (irec < fsr->lstrec) {	 			/* Search records in memory */
			++irec;
			for (i = 0; i < DNOD_REC; i++) {	/* Loop through all directory entries */
				if (fsr->dir[irec].entry[i].flag == USED) {	/* Entry is used--skip tests */
					if (fsr->dir[irec].entry[i].endrec > highrec)
						highrec = fsr->dir[irec].entry[i].endrec;
				} else if (fsr->dir[irec].entry[i].flag == REMOVED) {	/* Matrix removed--check size */
					if ((fsr->dir[irec].entry[i].endrec
					     - fsr->dir[irec].entry[i].begrec + 1) >= length) {	/* Large enough */
						begrec = fsr->dir[irec].entry[i].begrec;
						fsr->dir[irec].entry[i].endrec = begrec + length - 1;
						fsr->dir[irec].entry[i].flag = USED;
						fsr->dir[irec].entry[i].matnum = matnum;
						++fsr->dir[irec].hdr.numused;
						--fsr->dir[irec].hdr.numfree;
						recnum = fsr->dir[irec - 1].hdr.forptr;
						if (wrtrec (fp, (char *) &fsr->dir[fsr->lstrec], recnum, 1) != 0) {
							fprintf (stderr, "ERROR [create_dir7]: Writing record\n");
							return (ERROR);
						} else
							return (OK);
					}
					if (fsr->dir[irec].entry[i].endrec > highrec)
						highrec = fsr->dir[irec].entry[i].endrec;
				} else if (fsr->dir[irec].entry[i].flag == UNUSED) {	/* Unused entry */
					if (free1 == FALSE) {		/* Save first free for later use */
						free1 = TRUE;
						ifree = i;
					}
				} else {
					fprintf (stderr, "ERROR [create_dir7]: Invalid directory record\n");
					fprintf (stderr, "dir[%d].entry[%d].flag: %d\n",
						 irec, i, (int) fsr->dir[irec].entry[i].flag);
				}
			}
			if ((fsr->dir[irec].hdr.forptr == DIR_NUM_1) & (fsr->lstrec > 0))
				more = FALSE;

/*	Read next directory record from disk */

		} else {
			if (fsr->lstrec >= fsr->memsize - 1) {			/* Allocate more memory */
				memsize_old = fsr->memsize;
				fsr->memsize += SIZE_DIR_MALLOC;
				fsr->dir = (struct directory *) realloc ((char *) fsr->dir,
					   (unsigned) (fsr->memsize * sizeof (struct directory)));
				if (fsr->dir == NULL) {
					fprintf (stderr, "ERROR [create_dir7]: Error allocating directory\n");
					return (ERROR);
				}
				for (i = memsize_old; i < fsr->memsize; i++)	/* Initialize structure */
					for (j = 0; j < DNOD_REC; j++)
						fsr->dir[i].entry[j].flag = UNUSED;
			}
			current_rec = fsr->dir[fsr->lstrec].hdr.forptr;
			if (current_rec > highrec)
				highrec = current_rec;
			++fsr->lstrec;
			lenrd = 1;		 /* Read one record */
			if (rdrec (fp, &fsr->dir[fsr->lstrec], current_rec, lenrd) != 0) {
				fprintf (stderr, "ERROR [create_dir7]: Error reading directory\n");
				return (ERROR);
			}
		}
	}	/* while (more == TRUE) */

/* Unused entry found, use it. */

	if (free1 == TRUE) {
		begrec = highrec + 1;
		fsr->dir[fsr->lstrec].entry[ifree].begrec = begrec;
		fsr->dir[fsr->lstrec].entry[ifree].endrec = begrec + length - 1;
		/* highrec = fsr->dir[fsr->lstrec].entry[ifree].endrec; */
		fsr->dir[fsr->lstrec].entry[ifree].flag = USED;
		fsr->dir[fsr->lstrec].entry[ifree].matnum = matnum;
		++fsr->dir[fsr->lstrec].hdr.numused;
		--fsr->dir[fsr->lstrec].hdr.numfree;

/* No unused entries, create new directory record. */

	} else {
		++fsr->lstrec;
		if (fsr->lstrec >= fsr->memsize) {			/* Allocate more memory */
			fsr->memsize += SIZE_DIR_MALLOC;
			fsr->dir = (struct directory *)
				realloc ((char *) fsr->dir, (unsigned) (fsr->memsize * sizeof (struct directory)));
			if (fsr->dir == NULL) {
				fprintf (stderr, "ERROR [create_dir7]: Could not realloc memory for image directory\n");
				return (ERROR);
			}
		}
		fsr->dir[fsr->lstrec].hdr.numused = 1;
		fsr->dir[fsr->lstrec].hdr.numfree = DNOD_REC - 1;
		if (fsr->lstrec == 1)		 /* This is first directory record. */
			fsr->dir[fsr->lstrec].hdr.bakptr = 0;
		else
			fsr->dir[fsr->lstrec].hdr.bakptr = fsr->dir[fsr->lstrec - 2].hdr.forptr;
		fsr->dir[fsr->lstrec].hdr.forptr = 2;

/*	If not first directory record, update forward pointer in last directory record */

		if (fsr->lstrec > 1) {
			++highrec;
			current_rec = highrec;
			fsr->dir[fsr->lstrec - 1].hdr.forptr = current_rec;
			recnum = fsr->dir[fsr->lstrec].hdr.bakptr;
			if (wrtrec (fp, (char *) &fsr->dir[fsr->lstrec - 1], recnum, 1) != 0) {
				fprintf (stderr, "ERROR [create_dir7]: Error occurred during write\n");
				return (ERROR);
			}
		}
/* Write first entry to new directory record. */

		fsr->dir[fsr->lstrec].entry[0].matnum = matnum;
		fsr->dir[fsr->lstrec].entry[0].begrec = highrec + 1;
		begrec = fsr->dir[fsr->lstrec].entry[0].begrec;
		fsr->dir[fsr->lstrec].entry[0].endrec = fsr->dir[fsr->lstrec].entry[0].begrec + length - 1;
		/* highrec = fsr->dir[fsr->lstrec].entry[0].endrec; */
		fsr->dir[fsr->lstrec].entry[0].flag = USED;
	}

	recnum = fsr->dir[fsr->lstrec - 1].hdr.forptr;
	if (wrtrec (fp, fsr->dir[fsr->lstrec], recnum, 1) != 0)
		return (ERROR);
	else
		return (begrec);
}
