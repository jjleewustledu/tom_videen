/*$Id: matrix_extra.c,v 2.1 2003/03/10 22:35:51 tom Exp $
 *$Log: matrix_extra.c,v $
 * Revision 2.1  2003/03/10  22:35:51  tom
 * *** empty log message ***
 *
 * Revision 2.0  2002/12/26  20:35:56  tom
 * remove unused functions
 * lint and splint cleaning
 *
 *Revision 1.10  2002/12/17 22:15:13  tom
 *fix gcc warnings
 *
 *Revision 1.9  1999/12/13 22:02:19  tom
 *partially cleaned up with lint
 *
 * Revision 1.8  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.7  1995/11/30  19:42:15  tom
 * TOV mod
 *
 * Revision 1.6  1995/10/30  16:54:44  tom
 * changed petutil/matrix.h to matrix.h
 *
 * Revision 1.5  1994/09/23  19:20:13  ty7777
 * Modified by Tom Yang to fix memory leaks.
 *
 * Revision 1.4  1994/03/03  17:32:07  ty7777
 * Change from id to header.
 *
 * Revision 1.3  1993/09/13  18:57:05  ty7777
 * Introduce acsid and change matrix.h to petutil/matrix.h.
 *
 * Revision 1.2  1993/06/17  17:30:45  ty7777
 * Same as the last one.
 *
 * Revision 1.1  1993/03/04  19:47:59  ty7777
 * Initial revision
 **/

/* @(#)matrix_extra.c	2.13 6/9/92  */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <matrix.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/Old/libcti/RCS/matrix_extra.c,v 2.1 2003/03/10 22:35:51 tom Exp $";
#endif

/*	Modifications:
*	29-Jan-1992	jgh	Removed a de-bug printf from write_matrix_data -- case 2
*				statement was: printf ("Saving data in VAX i*2 format\n");
*	5-Feb-1992	emp	Added explicit declarations and return values
*				 for those functions returning integer values.
*				 The return codes were coming back garbage for
*				 those routines that checked return values.
*	23-Sep-1994	TY	Modified to fix memory leaks.
*/

int      file_access_code (mode)
	char           *mode;
{
	int             access_code;

	if (strcmp (mode, "r") == 0)
		access_code = SUN_READ;
	else if (strcmp (mode, "w") == 0)
		access_code = SUN_WRITE;
	else if (strcmp (mode, "r+") == 0)
		access_code = SUN_WRITE;
	else
		access_code = 0;
	return (access_code);
}

int             file_size (fname)
	char           *fname;
{
	struct stat     stbuf;

	if (stat (fname, &stbuf) == -1)
		return (-1);
	return (stbuf.st_size);
}

int             file_exists (filename)
	char           *filename;
{
	struct stat     stbuf;

	if (stat (filename, &stbuf) == -1)
		return (0);
	return (1);
}

int             free_matrix_data (data)
	MatrixData     *data;
{
	if (data != NULL) {
		if (data->data_ptr != NULL)
			free (data->data_ptr);
		if (data->shptr != NULL)
			free (data->shptr);
		free (data);
	}
	return (0);
}

void     matrix_freelist (matdirlist)
	MatDirList     *matdirlist;
{
	MatDirNode     *node, *next;

	if (matdirlist == NULL)
		return;
	if (matdirlist->first != NULL) {
		node = matdirlist->first;
		do {
			next = node->next;
			free (node);
			node = next;
		}
		while (next != NULL);
	}
	free (matdirlist);
	return;
}

MatrixFile     *matrix_create (fname, mhead)
	char           *fname;
	Main_header    *mhead;
{
	FILE           *fptr;
	MatrixFile     *mfp;

	fptr = mat_create (fname, mhead);
	if (fptr != NULL) {
		mfp = (MatrixFile *) calloc (1, sizeof (MatrixFile));
		if (mfp == NULL)
			return (NULL);
		(void) strcpy (mfp->fname, fname);
		mfp->mode = file_access_code ("r+");
		mfp->fptr = fptr;
		mfp->mhptr = mhead;
		mfp->dirlist = NULL;
		return (mfp);
	} else
		return (NULL);
}

Main_header    *matrixx_init_main_header (fname, ftype, mh_proto)
	char           *fname;
	short           ftype;
	Main_header    *mh_proto;
{
	Main_header    *mhptr;

	mhptr = (Main_header *) calloc (1, sizeof (Main_header));
	if (mh_proto)
		bcopy (mh_proto, mhptr, sizeof (Main_header));
	mhptr->file_type = ftype;
	(void) strncpy (mhptr->original_file_name, fname, 20);
	return (mhptr);
}

int             matrix_data_init (data)
	MatrixData     *data;
{
	data->mat_type = -1;
	data->shptr = NULL;
	data->data_ptr = NULL;
	data->nviews = 0;
	data->nelements = 0;
	data->data_type = -1;
	return (0);
}

MatrixFile     *matrix_open (fname, mode)
	char           *fname, *mode;
{
	FILE           *fptr;
	MatrixFile     *matfile;
	Main_header    *mhead;
	int             stat;

	matfile = (MatrixFile *) calloc (1, sizeof (MatrixFile));
	if (matfile == NULL) {
		fprintf (stderr, "ERROR [matrix_open]: matfile calloc error\n");
		return (NULL);
	}
	mhead = (Main_header *) calloc (1, sizeof (Main_header));
	if (mhead == NULL) {
		fprintf (stderr, "ERROR [matrix_open]: main header calloc error\n");
		free (matfile->mhptr);
		free (matfile);
		return (NULL);
	}
	matfile->mode = file_access_code (mode);
	(void) strcpy (matfile->fname, fname);
	switch (matfile->mode) {
	case SUN_READ:
	case SUN_WRITE:
		fptr = mat_open (fname, mode);
		if (fptr != NULL) {
			matfile->fptr = fptr;
			stat = mat_read_main_header (fptr, mhead);
			if (stat != 0) {
				free (mhead);
				free (matfile);
				return (NULL);
			}
			matfile->mhptr = mhead;
			matfile->dirlist = mat_read_directory (fptr);
			return (matfile);
		} else {
			(void) matrix_close (matfile);
			free (mhead);
			free (matfile);
			return (NULL);
		}
	default:
		fprintf (stderr, "ERROR [matrix_open]: unknown file type and/or file acess mode\n");
		(void) matrix_close (matfile);
		free (mhead);
		free (matfile);
		return (NULL);
	}
}

int             matrix_close (matfile)
	MatrixFile     *matfile;
{

	if (matfile == NULL)
		return (0);
	if (matfile->mhptr != NULL)
		free (matfile->mhptr);
	if (matfile->mode == SUN_READ || matfile->mode == SUN_WRITE)
		matrix_freelist (matfile->dirlist);
	(void) mat_close (matfile->fptr);
	free (matfile);
	return (0);
}

struct MatDir  *matrix_find (matnum, matfile)
	int             matnum;
	MatrixFile     *matfile;
{
	MatDirNode     *node;
	struct MatDir  *matdir;

	if (matfile == NULL)
		return (NULL);
	if (matfile->dirlist == NULL)
		return (NULL);
	matdir = (struct MatDir *) calloc (1, sizeof (struct MatDir));
	if (matdir == NULL)
		return (NULL);
	matdir->matnum = 0;
	matdir->strtblk = 0;
	matdir->endblk = 0;
	matdir->matstat = 0;
	node = matfile->dirlist->first;
	while (node != NULL) {
		if (node->matnum == matnum) {
			matdir->matnum = node->matnum;
			matdir->strtblk = node->strtblk;
			matdir->endblk = node->endblk;
			matdir->matstat = node->matstat;
			break;
		}
		node = node->next;
	}
	if (node != NULL)
		return (matdir);
	else {
		free (matdir);
		return (NULL);
	}
}

int             mat_make_dir (fptr, nf, np, ng, nb, nd, acqm, size)
	FILE           *fptr;
	int             nf, np, ng, nb, nd, acqm, size;
{
	int             bufr[32][4];
	int             stat, count, dirblk, numdirblks, blkptr;
	int             fr, pl, ga, da, be, ac, total, index, na, nac;

	na = 1;
	if (acqm == 2)
		na = 2;
	else if (acqm == 3)
		na = 4;
	else if (acqm == 5)
		na = 6;
	nac = 0;
	if (na > 1)
		nac = 1;
	if (ng == 0)
		ng = 1;
	numdirblks = (nf * np * ng * (nb + 1) * (nd + 1) * (nac + 1) + 30) / 31;
	total = nf * np * ng * (nb + 1) * (nd + 1) * (nac + 1);
	dirblk = 2;
	blkptr = 1 + numdirblks;
	count = 0;
	index = 0;

	bzero ((char *) bufr, 512);		 /** initialize directory block to zero */
	bufr[0][1] = 2;				 /** first block back link points to itself */
	bufr[0][2] = 0;				 /** first block forward link initialize to NIL */

	for (fr = 1; fr <= nf; fr++)
		for (pl = 1; pl <= np; pl++)
			for (ga = 1; ga <= ng; ga++)
				for (be = 0; be <= nb; be++)
					for (da = 0; da <= nd; da++)
						for (ac = 0; ac <= nac; ac++) {
							count++;
							index++;
							bufr[0][0] = 31 - index;
							bufr[0][3] = index;
							bufr[index][0] =
								mat_numcod (fr, pl, ga, da + ac * 4, be);
							bufr[index][1] = ++blkptr;
							bufr[index][2] = (blkptr += size);
							bufr[index][3] = 1;
							if (index == 31 || count == total) {
								if (count == total)
									bufr[0][1] = 2;	/* forward link */
								else
									bufr[0][1] = dirblk + 1;
								swaw (bufr, bufr, 256);
								swap_byte (bufr, bufr, 512);
								stat = mat_wblk (fptr, dirblk, bufr, 1);
								if (stat != 0) {
									fprintf (stderr, "mat_wblk: lseek error\n");
									return (ERROR);
								}
								if (count != total) {	/* new directory block */
									bzero ((char *) bufr, 512);
									bufr[0][2] = dirblk++;	/* back link */
									bufr[0][0] = 31;
									bufr[0][3] = 0;
									index = 0;
								}
							}
						}
	return (0);
}

int             mat_mlist (dirlist)
	MatDirList     *dirlist;
{
	MatDirNode     *node;
	int             i;

	if (dirlist == NULL)
		fprintf (stderr, "dirlist is NULL\n");
	else if (dirlist->nmats == 0)
		fprintf (stderr, "dirlist is empty\n");
	else {
		fprintf (stderr, "A total of %d directory entries\n", dirlist->nmats);
		node = dirlist->first;
		i = 0;
		while (node != NULL) {
			i++;
			fprintf (stderr, "%d %d %d %d\n", node->matnum, node->strtblk, node->endblk,
				 node->matstat);
			node = node->next;
		}
		fprintf (stderr, "\nA Total of %d entries listed\n", i);
	}
	return (0);
}

int             insert_mdir (matdir, dirlist)
	MatDirList     *dirlist;
	struct MatDir   matdir;
{
	MatDirNode     *node;

	if (dirlist == NULL) {
		dirlist = (MatDirList *) calloc (1, sizeof (MatDirList));
		if (dirlist == NULL)
			return (ERROR);
		dirlist->nmats = 0;
		dirlist->first = NULL;
		dirlist->last = NULL;
	}
	node = (MatDirNode *) calloc (1, sizeof (MatDirNode));
	if (node == NULL)
		return (ERROR);
	node->matnum = matdir.matnum;
	node->strtblk = matdir.strtblk;
	node->endblk = matdir.endblk;
	node->matstat = matdir.matstat;
	node->next = NULL;
	if (dirlist->first == NULL) {		 /* if list was empty, add first node */
		dirlist->first = node;
		dirlist->last = node;
		dirlist->nmats = 1;
	} else {
		dirlist->last->next = node;
		dirlist->last = node;
		++(dirlist->nmats);
	}
	return (0);
}

MatDirBlk      *mat_rdirblk (fptr, blknum)
	FILE           *fptr;
	int             blknum;
{
	MatDirBlk      *matdirblk;
	int             i, j, stat, ndirs;
	int             dirbufr[MatBLKSIZE / 4];
	char            bytebufr[MatBLKSIZE];

	bzero (bytebufr, MatBLKSIZE);
	matdirblk = (MatDirBlk *) calloc (1, sizeof (MatDirBlk));
	if (matdirblk == NULL)
		return (NULL);

	stat = mat_rblk (fptr, blknum, bytebufr, 1);
	if (stat != 0) {
		free (matdirblk);
		return (NULL);
	}
	swap_byte (bytebufr, dirbufr, MatBLKSIZE);
	swaw (dirbufr, dirbufr, MatBLKSIZE / 2);
	matdirblk->nfree = dirbufr[0];
	matdirblk->nextblk = dirbufr[1];
	matdirblk->prvblk = dirbufr[2];
	matdirblk->nused = dirbufr[3];
	ndirs = (MatBLKSIZE / 4 - 4) / 4;
	for (i = 0; i < ndirs; i++) {
		matdirblk->matdir[i].matnum = 0;
		matdirblk->matdir[i].strtblk = 0;
		matdirblk->matdir[i].endblk = 0;
		matdirblk->matdir[i].matstat = 0;
	}
	for (i = 0; i < matdirblk->nused; i++) {
		j = i + 1;
		matdirblk->matdir[i].matnum = dirbufr[j * 4 + 0];
		matdirblk->matdir[i].strtblk = dirbufr[j * 4 + 1];
		matdirblk->matdir[i].endblk = dirbufr[j * 4 + 2];
		matdirblk->matdir[i].matstat = dirbufr[j * 4 + 3];
	}
	return (matdirblk);
}


MatDirList     *mat_read_directory (fptr)
	FILE           *fptr;
{
	struct MatDir   matdir;
	MatDirList     *dirlist;
	MatDirBlk      *matdirblk;
	int             i, blknum;

	dirlist = (MatDirList *) calloc (1, sizeof (MatDirList));
	if (dirlist == NULL)
		return (NULL);
	blknum = MatFirstDirBlk;
	do {
		matdirblk = mat_rdirblk (fptr, blknum);
		if (matdirblk == NULL) {
			free (dirlist);
			return (NULL);
		}
		for (i = 0; i < matdirblk->nused; i++) {
			matdir.matnum = matdirblk->matdir[i].matnum;
			matdir.strtblk = matdirblk->matdir[i].strtblk;
			matdir.endblk = matdirblk->matdir[i].endblk;
			matdir.matstat = matdirblk->matdir[i].matstat;
			(void) insert_mdir (matdir, dirlist);
		}
		blknum = matdirblk->nextblk;
		free ((char *) matdirblk);
	}
	while (blknum != MatFirstDirBlk);
	return (dirlist);
}

MatrixData     *matrix_read (mfp, matnum)
	MatrixFile     *mfp;
	int             matnum;
{
	MatrixData     *data;
	int             stat;

	if (mfp == NULL)
		return (NULL);
	if (mfp->mhptr == NULL)
		return (NULL);
	data = (MatrixData *) calloc (1, sizeof (MatrixData));
	if (data == NULL) {
		fprintf (stderr, "ERROR [matrix_read]: matdata structure calloc error\n");
		return (NULL);
	}
	data->mat_type = -1;
	free (data->data_ptr);
	data->nviews = 0;
	data->nelements = 0;
	data->data_type = -1;
	data->matnum = matnum;
	free (data->shptr);
	data->shptr = (char *) calloc (MatBLKSIZE, sizeof (char));
	if (data->shptr == NULL) {
		fprintf (stderr, "ERROR [matrix_read]: subheader calloc error\n");
		free (data);
		return (NULL);
	}
	if (mfp->mode == SUN_READ || mfp->mode == SUN_WRITE) {
		stat = read_sun_data (mfp, matnum, data);
		if (stat == 0)
			return (data);
		else {
			free (data);
			return (NULL);
		}
	} else {
		fprintf (stderr, "ERROR [matrix_read]: file opened in unknown mode\n");
		free (data);
		return (NULL);
	}
}

int             read_sun_data (mfp, matnum, data)
	MatrixFile     *mfp;
	MatrixData     *data;
	int             matnum;
{
	struct MatDir  *matdir;
	int             stat, nblks;
	Scan_subheader *scansub;
	Image_subheader *imagesub;
	Attn_subheader *attnsub;
	Norm_subheader *normsub;

	matdir = matrix_find (matnum, mfp);
	if (matdir == NULL)
		return (ERROR);
	nblks = matdir->endblk - matdir->strtblk;
	free (data->data_ptr);
	data->data_ptr = (char *) calloc (512 * nblks, sizeof (char));
	if (data->data_ptr == NULL) {
		fprintf (stderr, "ERROR [matrix_read]: data buffer calloc error\n");
		free (matdir);
		return (ERROR);
	}
	data->nblks = nblks;
	switch (mfp->mhptr->file_type) {
	case 1:
		stat = mat_read_scan_subheader (mfp->fptr, matdir->strtblk, data->shptr);
		if (stat != 0) {
			free (matdir);
			return (ERROR);
		}
		scansub = (Scan_subheader *) data->shptr;
		stat = read_matrix_data (mfp->fptr, matdir->strtblk + 1, nblks,
					 data->data_ptr, scansub->data_type);
		if (stat != 0) {
			free (matdir);
			return (ERROR);
		}
		data->data_type = scansub->data_type;
		data->nelements = scansub->dimension_1;
		data->nviews = scansub->dimension_2;
		break;
	case 2:
		stat = mat_read_image_subheader (mfp->fptr, matdir->strtblk, data->shptr);
		if (stat != 0) {
			free (matdir);
			return (ERROR);
		}
		imagesub = (Image_subheader *) data->shptr;
		stat = read_matrix_data (mfp->fptr, matdir->strtblk + 1, nblks,
					 data->data_ptr, imagesub->data_type);
		if (stat != 0) {
			free (matdir);
			return (ERROR);
		}
		data->data_type = imagesub->data_type;
		data->nelements = imagesub->dimension_1;
		data->nviews = imagesub->dimension_2;
		break;
	case 3:
		stat = mat_read_attn_subheader (mfp->fptr, matdir->strtblk, data->shptr);
		if (stat != 0) {
			free (matdir);
			return (ERROR);
		}
		attnsub = (Attn_subheader *) data->shptr;
		stat = read_matrix_data (mfp->fptr, matdir->strtblk + 1, nblks,
					 data->data_ptr, attnsub->data_type);
		if (stat != 0) {
			free (matdir);
			return (ERROR);
		}
		data->data_type = attnsub->data_type;
		data->nelements = attnsub->dimension_1;
		data->nviews = attnsub->dimension_2;
		break;
	case 4:
		stat = mat_read_norm_subheader (mfp->fptr, matdir->strtblk, data->shptr);
		if (stat != 0) {
			free (matdir);
			return (ERROR);
		}
		normsub = (Norm_subheader *) data->shptr;
		stat = read_matrix_data (mfp->fptr, matdir->strtblk + 1, nblks,
					 data->data_ptr, normsub->data_type);
		if (stat != 0) {
			free (matdir);
			return (ERROR);
		}
		data->data_type = normsub->data_type;
		data->nelements = normsub->dimension_1;
		data->nviews = normsub->dimension_2;
		break;
	default:
		free (matdir);
		return (ERROR);
	}
	data->mat_type = mfp->mhptr->file_type;
	free (matdir);
	return (0);
}

int             write_sun_data (mfp, matnum, data)
	MatrixFile     *mfp;
	MatrixData     *data;
	int             matnum;
{
	struct MatDir  *matdir, dir_entry;
	Scan_subheader *scansub;
	Image_subheader *imagesub;
	Attn_subheader *attnsub;
	Norm_subheader *normsub;
	int             stat, blkno;

	matdir = matrix_find (matnum, mfp);
	if (matdir == NULL) {
		blkno = mat_enter (mfp->fptr, matnum, data->nblks);
		dir_entry.matnum = matnum;
		dir_entry.strtblk = blkno;
		dir_entry.endblk = dir_entry.strtblk + data->nblks - 1;
		dir_entry.matstat = 1;
		(void) insert_mdir (dir_entry, mfp->dirlist);
		matdir = &dir_entry;
	} else {
		if (data->nblks > matdir->endblk - matdir->strtblk + 1)
			return (ERROR);
	}

	switch (mfp->mhptr->file_type) {
	case 1:
		scansub = (Scan_subheader *) data->shptr;
		stat = mat_write_scan_subheader (mfp->fptr, matdir->strtblk, data->shptr);
		if (stat != 0)
			return (ERROR);
		stat = write_matrix_data (mfp->fptr, matdir->strtblk + 1,
					  data->nblks, data->data_ptr, scansub->data_type);
		return (stat);
	case 2:
		imagesub = (Image_subheader *) data->shptr;
		stat = mat_write_image_subheader (mfp->fptr, matdir->strtblk, data->shptr);
		if (stat != 0)
			return (ERROR);
		stat = write_matrix_data (mfp->fptr, matdir->strtblk + 1,
					  data->nblks, data->data_ptr, imagesub->data_type);
		return (stat);
	case 3:
		attnsub = (Attn_subheader *) data->shptr;
		stat = mat_write_attn_subheader (mfp->fptr, matdir->strtblk, data->shptr);
		if (stat != 0)
			return (ERROR);
		stat = write_matrix_data (mfp->fptr, matdir->strtblk + 1,
					  data->nblks, data->data_ptr, attnsub->data_type);
		return (stat);
	case 4:
		normsub = (Norm_subheader *) data->shptr;
		stat = mat_write_norm_subheader (mfp->fptr, matdir->strtblk, data->shptr);
		if (stat != 0)
			return (ERROR);
		stat = write_matrix_data (mfp->fptr, matdir->strtblk + 1,
					  data->nblks, data->data_ptr, normsub->data_type);
		return (stat);
	default:				 /* default treated as sinogram */
		scansub = (Scan_subheader *) data->shptr;
		stat = mat_write_scan_subheader (mfp->fptr, matdir->strtblk, data->shptr);
		if (stat != 0)
			return (ERROR);
		stat = write_matrix_data (mfp->fptr, matdir->strtblk + 1,
					  data->nblks, data->data_ptr, scansub->data_type);
		return (stat);
	}
}

int             read_matrix_data (fptr, strtblk, nblks, dptr, dtype)
	FILE           *fptr;
	int             strtblk, nblks, dtype;
	char           *dptr;
{
	int             stat, i;

	stat = mat_rblk (fptr, strtblk, dptr, nblks);
	if (stat != 0)
		return (ERROR);
	switch (dtype) {
	case 1:				 /* byte format...no translation necessary */
		break;
	case 2:				 /* Vax I*2 */
		swap_byte (dptr, dptr, 512 * nblks);
		break;
	case 3:				 /* Vax I*4 */
		swap_byte (dptr, dptr, 512 * nblks);
		swaw (dptr, dptr, 256 * nblks);
		break;
	case 4:				 /* Vax R*4 */
		swap_byte (dptr, dptr, 512 * nblks);
		for (i = 0; i < nblks * 128; i++)
			((float *) dptr)[i] = get_vax_float ((short *) dptr, i * 2);
		break;
	case 5:				 /* IEEE R*4 */
		break;
	case 6:				 /* 68K I*2 */
		break;
	case 7:				 /* 68K I*4 */
		break;
	default:				 /* something else...treat as Vax I*2 */
		swap_byte (dptr, dptr, 512 * nblks);
		break;
	}
	return (0);
}

int             write_matrix_data (fptr, strtblk, nblks, dptr, dtype)
	FILE           *fptr;
	int             strtblk, nblks, dtype;
	char           *dptr;
{
	int             stat;

	switch (dtype) {
	case 1:				 /* byte format...no translation necessary */
		stat = mat_wblk (fptr, strtblk, dptr, nblks);
		break;
	case 2:				 /* Vax I*2 */
		stat = mat_write_idata (fptr, strtblk, dptr, 512 * nblks);
		break;
	case 4:				 /* Vax R*4 */
		stat = mat_write_fdata (fptr, strtblk, dptr, 512 * nblks);
		break;
	case 5:				 /* IEEE R*4 */
		stat = mat_wblk (fptr, strtblk, dptr, nblks);
		break;
	case 6:				 /* 68K I*2 */
		stat = mat_wblk (fptr, strtblk, dptr, nblks);
		break;
	case 7:				 /* 68K I*4 */
		stat = mat_wblk (fptr, strtblk, dptr, nblks);
		break;
	default:				 /* something else...treat as Vax I*2 */
		stat = mat_write_idata (fptr, strtblk, dptr, 512 * nblks);
	}
	return (stat);
}

int             read_main_header (fp, mhead)
	MatrixFile     *fp;
	Main_header    *mhead;
{
	int             stat;

	stat = mat_read_main_header (fp->fptr, mhead);
	return (stat);
}

int             update_main_header (fp, mhead)
	MatrixFile     *fp;
	Main_header    *mhead;
{
	int             stat;

	stat = mat_write_main_header (fp->fptr, mhead);
	return (stat);
}

int             write_main_header (fp, mhead)
	MatrixFile     *fp;
	Main_header    *mhead;
{
	int             stat;

	stat = mat_write_main_header (fp->fptr, mhead);
	return (stat);
}

int             read_image_subheader (fp, matnum, isub)
	MatrixFile     *fp;
	int             matnum;
	Image_subheader *isub;
{
	int             stat;
	struct MatDir   entry;

	if (fp->mhptr->file_type != 2)
		return (ERROR);
	switch (fp->mode) {
	case SUN_READ:
	case SUN_WRITE:
		stat = mat_lookup (fp->fptr, matnum, &entry);
		if (stat != 0)
			return (ERROR);
		stat = mat_read_image_subheader (fp->fptr, entry.strtblk, isub);
		return (stat);
	default:
		return (ERROR);
	}
}

int             write_image_subheader (fp, matnum, isub)
	MatrixFile     *fp;
	int             matnum;
	Image_subheader *isub;
{
	int             stat;
	struct MatDir   entry;

	if (fp->mhptr->file_type != 2)
		return (ERROR);
	switch (fp->mode) {
	case SUN_READ:
	case SUN_WRITE:
		stat = mat_lookup (fp->fptr, matnum, &entry);
		if (stat != 0)
			return (ERROR);
		stat = mat_write_image_subheader (fp->fptr, entry.strtblk, isub);
		return (stat);
	default:
		return (ERROR);
	}
}

int             read_scan_subheader (fp, matnum, ssub)
	MatrixFile     *fp;
	int             matnum;
	Scan_subheader *ssub;
{
	int             stat;
	struct MatDir   entry;

	if (fp->mhptr->file_type != 1)
		return (ERROR);
	switch (fp->mode) {
	case SUN_READ:
	case SUN_WRITE:
		stat = mat_lookup (fp->fptr, matnum, &entry);
		if (stat != 0)
			return (ERROR);
		stat = mat_read_scan_subheader (fp->fptr, entry.strtblk, ssub);
		if (stat != 0)
			return (ERROR);
		break;
	default:
		return (ERROR);
	}
	return (0);
}

int             write_sino_subheader (fp, matnum, ssub)
	MatrixFile     *fp;
	int             matnum;
	Scan_subheader *ssub;
{
	int             stat;
	struct MatDir   entry;

	if (fp->mhptr->file_type != 1)
		return (ERROR);
	switch (fp->mode) {
	case SUN_READ:
	case SUN_WRITE:
		stat = mat_lookup (fp->fptr, matnum, &entry);
		if (stat != 0)
			return (ERROR);
		stat = mat_write_scan_subheader (fp->fptr, entry.strtblk, ssub);
		return (stat);
	default:
		return (ERROR);
	}
}

int             read_attn_subheader (fp, matnum, asub)
	MatrixFile     *fp;
	int             matnum;
	Attn_subheader *asub;
{
	int             stat;
	struct MatDir   entry;

	if (fp->mhptr->file_type != 3)
		return (ERROR);
	switch (fp->mode) {
	case SUN_READ:
	case SUN_WRITE:
		stat = mat_lookup (fp->fptr, matnum, &entry);
		if (stat != 0)
			return (ERROR);
		stat = mat_read_attn_subheader (fp->fptr, entry.strtblk, asub);
		return (stat);
	default:
		return (ERROR);
	}
}

int             write_attn_subheader (fp, matnum, asub)
	MatrixFile     *fp;
	int             matnum;
	Attn_subheader *asub;
{
	int             stat;
	struct MatDir   entry;

	if (fp->mhptr->file_type != 3)
		return (ERROR);
	switch (fp->mode) {
	case SUN_READ:
	case SUN_WRITE:
		stat = mat_lookup (fp->fptr, matnum, &entry);
		if (stat != 0)
			return (ERROR);
		stat = mat_write_attn_subheader (fp->fptr, entry.strtblk, asub);
		return (stat);
	default:
		return (ERROR);
	}
}

int             read_norm_subheader (fp, matnum, nsub)
	MatrixFile     *fp;
	int             matnum;
	Norm_subheader *nsub;
{
	int             stat;
	struct MatDir   entry;

	if (fp->mhptr->file_type != 4)
		return (ERROR);
	switch (fp->mode) {
	case SUN_READ:
	case SUN_WRITE:
		stat = mat_lookup (fp->fptr, matnum, &entry);
		if (stat != 0)
			return (ERROR);
		stat = mat_read_norm_subheader (fp->fptr, entry.strtblk, nsub);
		return (stat);
	default:
		return (ERROR);
	}
}

int             write_norm_subheader (fp, matnum, nsub)
	MatrixFile     *fp;
	int             matnum;
	Norm_subheader *nsub;
{
	int             stat;
	struct MatDir   entry;

	if (fp->mhptr->file_type != 4)
		return (ERROR);
	switch (fp->mode) {
	case SUN_READ:
	case SUN_WRITE:
		stat = mat_lookup (fp->fptr, matnum, &entry);
		if (stat != 0)
			return (ERROR);
		stat = mat_write_norm_subheader (fp->fptr, entry.strtblk, nsub);
		return (stat);
	default:
		return (ERROR);
	}
}

