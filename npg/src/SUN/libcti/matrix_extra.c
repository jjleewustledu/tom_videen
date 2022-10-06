/*$Id: matrix_extra.c,v 1.10 2002/12/17 22:15:13 tom Exp tom $
 *$Log: matrix_extra.c,v $
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

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/matrix_extra.c,v 1.10 2002/12/17 22:15:13 tom Exp tom $";
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

/* Subroutine to return starting position of substring within string */
/* return index of t in s, -1 if none */

int             strindex (s, t)
	char            s[], t[];
{
	int             i, j, k;

	for (i = 0; s[i] != '\0'; i++) {
		for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++)
			if (k > 0 && t[k] == '\0')
				return i;
	}
	return -1;
}

int             is_acs (fname)
	char           *fname;

{
	if (strindex (fname, "/sd") == 0)	 /* allows access to multiple ACS disks */
		return (1);
	return (0);
}

/* Function to determine file open and access code for acs/sun matrix files */

int             file_access_code (fname, mode)
	char           *fname, *mode;
{
	int             access_code, i, acs, wr, rd;

	acs = 0;
	wr = 0;
	rd = 0;
	i = strindex (fname, "/sd");
	if (i == 0)
		acs = 1;
	if (strchr (mode, 'w') != NULL || strindex (mode, "r+") != -1)
		wr = 1;
	if (strchr (mode, 'r') != NULL)
		rd = 1;
	if (acs != 0 && wr != 0)
		access_code = ACS_WRITE;
	else if (acs != 0 && rd != 0)
		access_code = ACS_READ;
	else if (acs == 0 && wr != 0)
		access_code = SUN_WRITE;
	else if (acs == 0 && rd != 0)
		access_code = SUN_READ;
	else
		access_code = 0;
	return (access_code);
}

void             matrix_freelist (matdirlist)
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

int            matrix_close (matfile)
	MatrixFile     *matfile;
{

	if (matfile == NULL)
		return (OK);
	if (matfile->mhptr != NULL)
		free (matfile->mhptr);
	if (matfile->mode == SUN_READ || matfile->mode == SUN_WRITE)
		matrix_freelist (matfile->dirlist);
	(void) mat_close (matfile->fptr);
	free (matfile);
	return (OK);
}

int             mat_acs_build_dir (mfp)
	MatrixFile     *mfp;
{
	struct MatDir   matdir;
	MatDirList     *dirlist;
	MatDirBlk      *dirblk;
	char            buff[512];
	int             i, blkno, status;
	int             rtsRblk ();
	int				insert_mdir ();
	void			swap_byte ();
	void			swaw ();

	dirlist = (MatDirList *) malloc (sizeof (MatDirList));
	if (dirlist == NULL)
		return (-1);
	bzero ((char *) dirlist, sizeof (MatDirList));
	blkno = MatFirstDirBlk;
	do {
		status = rtsRblk (mfp->fname, blkno, buff);
		if (status == ERROR) {
			free (dirlist);
			return (ERROR);
		}
		swap_byte (buff, buff, 512);
		swaw (buff, buff, 256);
		dirblk = (MatDirBlk *) buff;
		for (i = 0; i < dirblk->nused; i++) {
			matdir.matnum = dirblk->matdir[i].matnum;
			matdir.strtblk = dirblk->matdir[i].strtblk;
			matdir.endblk = dirblk->matdir[i].endblk;
			matdir.matstat = dirblk->matdir[i].matstat;
			(void) insert_mdir (matdir, dirlist);
		}
		blkno = dirblk->nextblk;
	} while (blkno != MatFirstDirBlk);
	mfp->dirlist = dirlist;
	return (OK);
}

MatrixFile     *matrix_open (fname, mode)
	char           *fname, *mode;
{
	FILE           *fptr;
	MatrixFile     *matfile;
	MatDirList     *mat_read_directory ();
	Main_header    *mhead;
	int             status;
	int				rts_rmhd ();

/* allocate space for matfile data structure and initialize */

	matfile = (MatrixFile *) malloc (sizeof (MatrixFile));
	if (matfile == NULL) {
		perror ("matrix_open : matfile malloc error");
		return (NULL);
	}
	bzero ((char *) matfile, sizeof (MatrixFile));

/* allocate space for main header data structure and initialize */

	mhead = (Main_header *) malloc (sizeof (Main_header));
	if (mhead == NULL) {
		perror ("matrix_open : main header malloc error");
		free (matfile);
		return (NULL);
	}
	bzero ((char *) mhead, sizeof (Main_header));
	matfile->mode = file_access_code (fname, mode);
	(void) strcpy (matfile->fname, fname);
	switch (matfile->mode) {
	case ACS_READ:
	case ACS_WRITE:
		status = rts_rmhd (fname, mhead);
		if (status == ERROR) {
			(void) matrix_close (matfile);
			free (mhead);
			free (matfile);
			return (NULL);
		}
		matfile->mhptr = mhead;
		status = mat_acs_build_dir (matfile);
		if (status == ERROR) {
			(void) matrix_close (matfile);
			free (matfile);
			return (NULL);
		}
		return (matfile);
	case SUN_READ:
	case SUN_WRITE:
		fptr = mat_open (fname, mode);
		if (fptr != NULL) {
			matfile->fptr = fptr;
			(void) mat_read_main_header (fptr, mhead);
			matfile->mhptr = mhead;
			fprintf (stderr, "mat_read_directory\n");
			matfile->dirlist = mat_read_directory (fptr);
			fprintf (stderr, "return\n");
			return (matfile);
		} else {
			(void) matrix_close (matfile);
			return (NULL);
		}
	default:
		perror ("matrix_open: unknown file type and/or file acess mode");
		(void) matrix_close (matfile);
		return (NULL);
	}
}

MatrixFile     *matrix_create (fname, mhead)
	char           *fname;
	Main_header    *mhead;
{
	FILE           *fptr, *mat_create ();
	MatrixFile     *mfp;
	int				createAMatrixFile ();

	if (is_acs (fname)) {
		if ((createAMatrixFile (fname, mhead, 0)) != 0)
			return (NULL);
		mfp = matrix_open (fname, "r+");
	} else {
		fptr = mat_create (fname, mhead);
		if (fptr != NULL) {
			mfp = (MatrixFile *) malloc (sizeof (MatrixFile));
			bzero ((char *) mfp, sizeof (MatrixFile));
			if (mfp == NULL)
				return (NULL);
			(void) strcpy (mfp->fname, fname);
			mfp->mode = file_access_code (fname, "r+");
			mfp->fptr = fptr;
			mfp->mhptr = mhead;
			mfp->dirlist = NULL;
		}
	}
	return (mfp);
}

MatrixData     *matrix_read (mfp, matnum)
	MatrixFile     *mfp;
	int             matnum;
{
	MatrixData     *data;
	int             status;
	int				read_sun_data ();
	int				read_acs_data ();

	if (mfp == NULL)
		return (NULL);
	if (mfp->mhptr == NULL)
		return (NULL);

/* allocate space for MatrixData structure */

	data = (MatrixData *) malloc (sizeof (MatrixData));
	if (data == NULL) {
		perror ("matrix_read : matdata structure malloc error");
		return (NULL);
	}

/* initialize MatrixData structure */

	data->mat_type = -1;
	data->data_ptr = NULL;
	data->nviews = 0;
	data->nelements = 0;
	data->data_type = -1;
	data->matnum = matnum;

/* allocate space for subheader and initialize */

	data->shptr = (char *) malloc (MatBLKSIZE);
	if (data->shptr == NULL) {
		perror ("matrix_read : subheader malloc error");
		return (NULL);
	}
	bzero ((char *) data->shptr, MatBLKSIZE);
	if (mfp->mode == SUN_READ || mfp->mode == SUN_WRITE) {
		status = read_sun_data (mfp, matnum, data);
		if (status == OK)
			return (data);
		else
			return (NULL);
	} else if (mfp->mode == ACS_READ || mfp->mode == ACS_WRITE) {
		status = read_acs_data (mfp, matnum, data);
		if (status == OK)
			return (data);
		else
			return (NULL);
	} else {
		perror ("file opened in unknown mode");
		return (NULL);
	}
}


/*
 * Modified by Tom Yang on Sep. 23, 1994 to fix memory leak.
 */
int             read_sun_data (mfp, matnum, data)
	MatrixFile     *mfp;
	MatrixData     *data;
	int             matnum;
{
	struct MatDir  *matdir;
	int             nblks;
	int				read_matrix_data ();
	int				mat_read_norm_subheader ();
	Scan_subheader *scansub;
	Image_subheader *imagesub;
	Attn_subheader *attnsub;
	Norm_subheader *normsub;

	matdir = matrix_find (matnum, mfp);
	if (matdir == NULL)
		return (ERROR);
	nblks = matdir->endblk - matdir->strtblk;
	data->data_ptr = (char *) malloc (512 * nblks);
	if (data->data_ptr == NULL) {
		perror ("matrix_read : data buffer malloc error");
		return (ERROR);
	}
	bzero ((char *) data->data_ptr, 512 * nblks);
	data->nblks = nblks;
	switch (mfp->mhptr->file_type) {
	case 1:
		(void) mat_read_scan_subheader (mfp->fptr, matdir->strtblk, data->shptr);
		scansub = (Scan_subheader *) data->shptr;
		(void) read_matrix_data (mfp->fptr, matdir->strtblk + 1, nblks,
				  data->data_ptr, scansub->data_type);
		data->data_type = scansub->data_type;
		data->nelements = scansub->dimension_1;
		data->nviews = scansub->dimension_2;
		break;
	case 2:
		(void) mat_read_image_subheader (mfp->fptr, matdir->strtblk, data->shptr);
		imagesub = (Image_subheader *) data->shptr;
		(void) read_matrix_data (mfp->fptr, matdir->strtblk + 1, nblks,
				  data->data_ptr, imagesub->data_type);
		data->data_type = imagesub->data_type;
		data->nelements = imagesub->dimension_1;
		data->nviews = imagesub->dimension_2;
		break;
	case 3:
		(void) mat_read_attn_subheader (mfp->fptr, matdir->strtblk, data->shptr);
		attnsub = (Attn_subheader *) data->shptr;
		(void) read_matrix_data (mfp->fptr, matdir->strtblk + 1, nblks,
				  data->data_ptr, attnsub->data_type);
		data->data_type = attnsub->data_type;
		data->nelements = attnsub->dimension_1;
		data->nviews = attnsub->dimension_2;
		break;
	case 4:
		(void) mat_read_norm_subheader (mfp->fptr, matdir->strtblk, data->shptr);
		normsub = (Norm_subheader *) data->shptr;
		(void) read_matrix_data (mfp->fptr, matdir->strtblk + 1, nblks,
				  data->data_ptr, normsub->data_type);
		data->data_type = normsub->data_type;
		data->nelements = normsub->dimension_1;
		data->nviews = normsub->dimension_2;
		break;
	default:
		return (ERROR);
	}

	data->mat_type = mfp->mhptr->file_type;
	free (matdir);
	return (OK);
}


int             read_acs_data (mfp, matnum, data)/* assumming the file exists on acs disk */
	MatrixFile     *mfp;
	MatrixData     *data;
	int             matnum;
{
	Scan_subheader *scansub;
	Image_subheader *imagesub;
	Attn_subheader *attnsub;
	Norm_subheader *normsub;
	char            buff[512];
	int             status;
	int             nviews = 0;
	int             nprojs = 0;
	int             data_size = 0;
	int             data_out = 0;
	short          *scan_data, *image_data;
	int				rts_rshd ();
	int				rts_rdat ();
	int				convertScanHeaderFromVax ();
	int				convertImageHeaderFromVax ();
	int				convertAttnHeaderFromVax ();
	int				convertNormHeaderFromVax ();

	float          *norm_data, *attn_data;

	switch (mfp->mhptr->file_type) {
	case 1:
		status = rts_rshd (mfp->fname, matnum, buff);
		if (status == ERROR)
			return (ERROR);
		scansub = (Scan_subheader *) data->shptr;
		(void) convertScanHeaderFromVax (buff, scansub);
		nviews = scansub->dimension_2;
		nprojs = scansub->dimension_1;
		data_size = nviews * nprojs * 2;
		scan_data = (short *) malloc (data_size);
		if (scan_data == NULL)
			return (ERROR);
		status = rts_rdat (mfp->fname, matnum, scan_data, &data_out);
		if (status == ERROR) {
			free (scan_data);
			return (ERROR);
		}
		data->data_type = scansub->data_type;
		data->data_ptr = (char *) scan_data;
		break;
	case 2:
		status = rts_rshd (mfp->fname, matnum, buff);
		if (status == ERROR)
			return (ERROR);
		imagesub = (Image_subheader *) data->shptr;
		(void) convertImageHeaderFromVax (buff, imagesub);
		nviews = imagesub->dimension_2;
		nprojs = imagesub->dimension_1;
		data_size = nviews * nprojs * 2;
		image_data = (short *) malloc (data_size);
		if (image_data == NULL)
			return (ERROR);
		status = rts_rdat (mfp->fname, matnum, image_data, &data_out);
		if (status == ERROR) {
			free (image_data);
			return (ERROR);
		}
		data->data_type = imagesub->data_type;
		data->data_ptr = (char *) image_data;
		break;
	case 3:
		status = rts_rshd (mfp->fname, matnum, buff);
		if (status == ERROR)
			return (ERROR);
		attnsub = (Attn_subheader *) data->shptr;
		(void) convertAttnHeaderFromVax (buff, attnsub);
		nviews = attnsub->dimension_2;
		nprojs = attnsub->dimension_1;
		data_size = nviews * nprojs * 4;
		attn_data = (float *) malloc (data_size);
		if (attn_data == NULL)
			return (ERROR);
		status = rts_rdat (mfp->fname, matnum, attn_data, &data_out);
		if (status == ERROR) {
			free (attn_data);
			return (ERROR);
		}
		data->data_type = attnsub->data_type;
		data->data_ptr = (char *) attn_data;
		break;
	case 4:
		status = rts_rshd (mfp->fname, matnum, buff);
		if (status == ERROR)
			return (ERROR);
		normsub = (Norm_subheader *) data->shptr;
		(void) convertNormHeaderFromVax (buff, normsub);
		nviews = normsub->dimension_2;
		nprojs = normsub->dimension_1;
		data_size = nviews * nprojs * 4;
		norm_data = (float *) malloc (data_size);
		if (norm_data == NULL)
			return (ERROR);
		status = rts_rdat (mfp->fname, matnum, norm_data, &data_out);
		if (status == ERROR) {
			free (norm_data);
			return (ERROR);
		}
		data->data_type = normsub->data_type;
		data->data_ptr = (char *) norm_data;
		break;
	}
	if (data_out != data_size)
		return (ERROR);
	data->mat_type = mfp->mhptr->file_type;
	data->nviews = nviews;
	data->nelements = nprojs;
	data->nblks = (data_out + 511) / 512;
	return (OK);
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
	return (OK);
}


int             matrix_write (mfp, matnum, data)
	MatrixFile     *mfp;
	int             matnum;
	MatrixData     *data;
{
	int             status;
	int             write_sun_data ();
	int             write_acs_data ();

	if (mfp == NULL)
		return (ERROR);
	if (mfp->mhptr == NULL)
		return (ERROR);
	if (data == NULL)
		return (ERROR);
	if (data->nblks <= 0) {
		return (ERROR);
	}
	if (data->data_ptr == NULL)
		return (ERROR);
	if (data->shptr == NULL)
		return (ERROR);
	switch (mfp->mode) {
	case SUN_WRITE:
		status = write_sun_data (mfp, matnum, data);
		return (status);
	case ACS_WRITE:
		status = write_acs_data (mfp, matnum, data);
		return (status);
	default:
		status = ERROR;
		return (status);
	}
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
	int             status, blkno;
	int				mat_enter ();
	int				insert_mdir ();
	int				write_matrix_data ();

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
		status = mat_write_scan_subheader (mfp->fptr, matdir->strtblk, data->shptr);
		if (status < 0) return (status);
		status = write_matrix_data (mfp->fptr, matdir->strtblk + 1,
					    data->nblks, data->data_ptr, scansub->data_type);
		return (status);
	case 2:
		imagesub = (Image_subheader *) data->shptr;
		status = mat_write_image_subheader (mfp->fptr, matdir->strtblk, data->shptr);
		if (status < 0) return (status);
		status = write_matrix_data (mfp->fptr, matdir->strtblk + 1,
					    data->nblks, data->data_ptr, imagesub->data_type);
		return (status);
	case 3:
		attnsub = (Attn_subheader *) data->shptr;
		status = mat_write_attn_subheader (mfp->fptr, matdir->strtblk, data->shptr);
		if (status < 0) return (status);
		status = write_matrix_data (mfp->fptr, matdir->strtblk + 1,
					    data->nblks, data->data_ptr, attnsub->data_type);
		return (status);
	case 4:
		normsub = (Norm_subheader *) data->shptr;
		status = mat_write_norm_subheader (mfp->fptr, matdir->strtblk, data->shptr);
		if (status < 0) return (status);
		status = write_matrix_data (mfp->fptr, matdir->strtblk + 1,
					    data->nblks, data->data_ptr, normsub->data_type);
		return (status);
	default:				 /* default treated as sinogram */
		scansub = (Scan_subheader *) data->shptr;
		status = mat_write_scan_subheader (mfp->fptr, matdir->strtblk, data->shptr);
		if (status < 0) return (status);
		status = write_matrix_data (mfp->fptr, matdir->strtblk + 1,
					    data->nblks, data->data_ptr, scansub->data_type);
		return (status);
	}
}

int             write_acs_data (mfp, matnum, data)
	MatrixFile     *mfp;
	MatrixData     *data;
	int             matnum;
{
	int             status = 0;
	char            buff[512];
	int				rts_wshd ();
	int				rts_wdat ();
	int				convertScanHeaderToVax ();
	int				convertImageHeaderToVax ();
	int				convertAttnHeaderToVax ();
	int				convertNormHeaderToVax ();

	switch (mfp->mhptr->file_type) {
	case 1:
		status = convertScanHeaderToVax (buff, data->shptr);
		if (status != 0) return (status);
		status = rts_wshd (mfp->fname, matnum, buff);
		if (status != 0) return (status);
		break;
	case 2:
		status = convertImageHeaderToVax (buff, data->shptr);
		if (status != 0) return (status);
		status = rts_wshd (mfp->fname, matnum, buff);
		if (status != 0) return (status);
		break;
	case 3:
		status = convertAttnHeaderToVax (buff, data->shptr);
		if (status != 0) return (status);
		status = rts_wshd (mfp->fname, matnum, buff);
		if (status != 0) return (status);
		break;
	case 4:
		status = convertNormHeaderToVax (buff, data->shptr);
		if (status != 0) return (status);
		status = rts_wshd (mfp->fname, matnum, buff);
		if (status != 0) return (status);
		break;
	}
	status = rts_wdat (mfp->fname, matnum, data->data_type, data->data_ptr, 512 * data->nblks);
	return (status);
}

int             read_matrix_data (fptr, strtblk, nblks, dptr, dtype)
	FILE           *fptr;
	int             strtblk, nblks, dtype;
	char           *dptr;
{
	int             i;
	int             status;
	float           get_vax_float ();
	void			swap_byte ();
	void			swaw ();

	status = mat_rblk (fptr, strtblk, dptr, nblks);
	if (status != 0) return (ERROR);
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
	return (OK);
}

int             write_matrix_data (fptr, strtblk, nblks, dptr, dtype)
	FILE           *fptr;
	int             strtblk, nblks, dtype;
	char           *dptr;
{
	int             status = 0;
	int             mat_wblk ();
	int             mat_write_idata ();
	int             mat_write_fdata ();

	switch (dtype) {
	case 1:				 /* byte format...no translation necessary */
		status = mat_wblk (fptr, strtblk, dptr, nblks);
		break;
	case 2:				 /* Vax I*2 */
		status = mat_write_idata (fptr, strtblk, dptr, 512 * nblks);
		break;
	case 4:				 /* Vax R*4 */
		status = mat_write_fdata (fptr, strtblk, dptr, 512 * nblks);
		break;
	case 5:				 /* IEEE R*4 */
		status = mat_wblk (fptr, strtblk, dptr, nblks);
		break;
	case 6:				 /* 68K I*2 */
		status = mat_wblk (fptr, strtblk, dptr, nblks);
		break;
	case 7:				 /* 68K I*4 */
		status = mat_wblk (fptr, strtblk, dptr, nblks);
		break;
	default:				 /* something else...treat as Vax I*2 */
		status = mat_write_idata (fptr, strtblk, dptr, 512 * nblks);
	}
	return (status);
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
	matdir = (struct MatDir *) malloc (sizeof (struct MatDir));
	if (matdir == NULL) return (NULL);
	matdir->matnum = 0;
	matdir->strtblk = 0;
	matdir->endblk = 0;
	matdir->matstat = 0;
	node = matfile->dirlist->first;
	while (node != NULL) {
/*	   printf("node.matnum %x matnum %x\n", node->matnum, matnum); */
		if (node->matnum == matnum) {
			matdir->matnum = node->matnum;
			matdir->strtblk = node->strtblk;
			matdir->endblk = node->endblk;
			matdir->matstat = node->matstat;
			break;
		}
		node = node->next;
	}
/*
	printf("node : %x %d %d %d\n", node->matnum, node->strtblk,
				node->endblk, node->matstat) ;
	printf("matdir : %x %d %d %d\n", matdir->matnum, matdir->strtblk,
				matdir->endblk, matdir->matstat) ;
*/
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
	int             count, dirblk, numdirblks, blkptr;
	int             fr, pl, ga, da, be, ac, total, index, na, nac;
	int             status;
	int             mat_wblk ();
	void			swap_byte ();
	void			swaw ();

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
								status = mat_wblk (fptr, dirblk,
										   bufr, 1);
								if (status == ERROR) {
									printf ("mat_wblk: lseek error\n");
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
	return (OK);
}

int             mat_mlist (dirlist)
	MatDirList     *dirlist;
{
	MatDirNode     *node;
	int             i;

	if (dirlist == NULL)
		printf ("dirlist is NULL\n");
	else if (dirlist->nmats == 0)
		printf ("dirlist is empty\n");
	else {
		printf ("A total of %d directory entries\n", dirlist->nmats);
		node = dirlist->first;
		i = 0;
		while (node != NULL) {
			i++;
			printf ("%d %d %d %d\n", node->matnum, node->strtblk, node->endblk,
				node->matstat);
			node = node->next;
		}
		printf ("\nA Total of %d entries listed\n", i);
	}
	return (OK);
}

int             insert_mdir (matdir, dirlist)
	MatDirList     *dirlist;
	struct MatDir   matdir;
{
	MatDirNode     *node;

	if (dirlist == NULL) {
		dirlist = (MatDirList *) malloc (sizeof (MatDirList));
		if (dirlist == NULL)
			return (ERROR);
		dirlist->nmats = 0;
		dirlist->first = NULL;
		dirlist->last = NULL;
	}
	node = (MatDirNode *) malloc (sizeof (MatDirNode));
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
		(dirlist->last)->next = node;
		dirlist->last = node;
		++(dirlist->nmats);
	}
	return (OK);
}

MatDirBlk      *mat_rdirblk (fptr, blknum)
	FILE           *fptr;
	int             blknum;
{
	MatDirBlk      *matdirblk;
	int             i, j, err, ndirs;
	int             dirbufr[MatBLKSIZE / 4];
	char            bytebufr[MatBLKSIZE];
	void			swap_byte ();
	void			swaw ();

/*	printf("subroutine mat_rdirblk...\n") ; */
	bzero (bytebufr, MatBLKSIZE);
	matdirblk = (MatDirBlk *) malloc (MatBLKSIZE);
	if (matdirblk == NULL)
		return (NULL);

/*	printf("memory space allocated for dir block...\n") ; */
	err = mat_rblk (fptr, blknum, bytebufr, 1);
	if (err < 0)
		return (NULL);
	swap_byte (bytebufr, dirbufr, MatBLKSIZE);
	swaw (dirbufr, dirbufr, MatBLKSIZE / 2);
/*	printf("buffer swapped...\n") ; */
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
/*	printf("structure initialized, %d entries set to 0\n", i+1) ; */
/*	printf("Number of entries : %d\n", matdirblk->nused) ; */
	for (i = 0; i < matdirblk->nused; i++) {
		j = i + 1;
		matdirblk->matdir[i].matnum = dirbufr[j * 4 + 0];
		matdirblk->matdir[i].strtblk = dirbufr[j * 4 + 1];
		matdirblk->matdir[i].endblk = dirbufr[j * 4 + 2];
		matdirblk->matdir[i].matstat = dirbufr[j * 4 + 3];
	}

/*	printf("directory block copied...\n") ; */
	return (matdirblk);
}


MatDirList     *mat_read_directory (fptr)
	FILE           *fptr;
{
	struct MatDir   matdir;
	MatDirList     *dirlist;
	MatDirBlk      *matdirblk;
	int             i, blknum;

	dirlist = (MatDirList *) malloc (sizeof (MatDirList));
	if (dirlist == NULL)
		return (NULL);
	bzero ((char *) dirlist, sizeof (MatDirList));
	blknum = MatFirstDirBlk;
	fprintf (stderr, "mat_read_directory do\n");
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
	fprintf (stderr, "mat_read_directory done\n");
/*	mat_mlist(dirlist) ; */
	return (dirlist);
}

static int      matrixCounter = 0;

void            resetMatrixCounter ()
{
	matrixCounter = 0;
	return;
}

struct MatDir  *get_next_dir_entry (mfp)
	MatrixFile     *mfp;
{
	int             i;
	MatDirNode     *node;
	struct MatDir  *matdir;

	if (mfp == NULL)
		return (NULL);
	if (mfp->dirlist == NULL)
		return (NULL);
	if (matrixCounter == mfp->dirlist->nmats) {
		matrixCounter = 0;
		return (NULL);
	}
	matdir = (struct MatDir *) malloc (sizeof (struct MatDir));
	node = mfp->dirlist->first;
	for (i = 0; i < matrixCounter; i++)
		node = node->next;
	matrixCounter++;
	matdir->matnum = node->matnum;
	matdir->strtblk = node->strtblk;
	matdir->endblk = node->endblk;
	matdir->matstat = node->matstat;
	return (matdir);
}

int             mat_duplicate (outfile, mp)
	char           *outfile;
	MatrixFile     *mp;
{
	FILE           *fptr;
	char            buf[512];
	MatDirBlk      *dirblk;
	int             err, blkno, count;
	void			swap_byte ();
	void			swaw ();

	if (mp == NULL)
		return (0);
	if (mp->mhptr == NULL)
		return (0);
	fptr = mat_open (outfile, "w");
	if (fptr == NULL)
		return (0);
	err = mat_write_main_header (fptr, mp->mhptr);
	if (err < 0) {
		(void) mat_close (fptr);
		free (fptr);
		return (0);
	}
	blkno = 2;
	count = 0;
	do {
		err = mat_rblk (mp->fptr, blkno, buf, 1);
		if (err < 0)
			break;
		err = mat_wblk (fptr, blkno, buf, 1);
		if (err < 0)
			return (0);
		swap_byte (buf, buf, 512);
		swaw (buf, buf, 256);
		dirblk = (MatDirBlk *) buf;
		blkno = dirblk->nextblk;
		count++;
	}
	while (blkno != 2);
	(void) mat_close (fptr);
	return (count);
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
	return (OK);
}

#include	<sys/types.h>
#include	<sys/stat.h>

file_size (fname)
	char           *fname;
{
	struct stat     stbuf;

	if (stat (fname, &stbuf) == -1)
		return (-1);
	return (stbuf.st_size);
}


/* subroutine to see if file exists or not */

#include	<sys/types.h>
#include	<sys/stat.h>

int             file_exists (filename)
	char           *filename;
{
	struct stat     stbuf;

	if (stat (filename, &stbuf) == -1)
		return (0);
	return (1);
}

int             read_scan_subheader (fp, matnum, ssub)
	MatrixFile     *fp;
	int             matnum;
	Scan_subheader *ssub;
{
	int             status;
	char            buff[512];
	struct MatDir   entry;
	int				mat_lookup ();

	if (fp->mhptr->file_type != 1)
		return (ERROR);
	switch (fp->mode) {
	case ACS_READ:
	case ACS_WRITE:
		status = rts_rshd (fp->fname, matnum, buff);
		if (status == ERROR)
			return (ERROR);
		convertScanHeaderFromVax (buff, ssub);
		break;
	case SUN_READ:
	case SUN_WRITE:
		if (!mat_lookup (fp->fptr, matnum, &entry))
			return (ERROR);
		mat_read_scan_subheader (fp->fptr, entry.strtblk, ssub);
		break;
	default:
		return (ERROR);
	}
	return (OK);
}


int             read_norm_subheader (fp, matnum, nsub)
	MatrixFile     *fp;
	int             matnum;
	Norm_subheader *nsub;
{
	int             status;
	char            buff[512];
	struct MatDir   entry;

	if (fp->mhptr->file_type != 4)
		return (ERROR);
	switch (fp->mode) {
	case ACS_READ:
	case ACS_WRITE:
		status = rts_rshd (fp->fname, matnum, buff);
		if (status == ERROR)
			return (ERROR);
		convertNormHeaderFromVax (buff, nsub);
		break;
	case SUN_READ:
	case SUN_WRITE:
		if (!mat_lookup (fp->fptr, matnum, &entry))
			return (ERROR);
		mat_read_norm_subheader (fp->fptr, entry.strtblk, nsub);
		break;
	default:
		return (ERROR);
	}
	return (OK);
}

int             read_attn_subheader (fp, matnum, asub)
	MatrixFile     *fp;
	int             matnum;
	Attn_subheader *asub;
{
	int             status;
	char            buff[512];
	struct MatDir   entry;
	int				convertAttnHeaderFromVax ();

	if (fp->mhptr->file_type != 3)
		return (ERROR);
	switch (fp->mode) {
	case ACS_READ:
	case ACS_WRITE:
		status = rts_rshd (fp->fname, matnum, buff);
		if (status == ERROR)
			return (ERROR);
		convertAttnHeaderFromVax (buff, asub);
		break;
	case SUN_READ:
	case SUN_WRITE:
		if (!mat_lookup (fp->fptr, matnum, &entry))
			return (ERROR);
		mat_read_attn_subheader (fp->fptr, entry.strtblk, asub);
		break;
	default:
		return (ERROR);
	}
	return (OK);
}

int             read_image_subheader (fp, matnum, isub)
	MatrixFile     *fp;
	int             matnum;
	Image_subheader *isub;
{
	int             status;
	char            buff[512];
	struct MatDir   entry;

	if (fp->mhptr->file_type != 2)
		return (ERROR);
	switch (fp->mode) {
	case ACS_READ:
	case ACS_WRITE:
		status = rts_rshd (fp->fname, matnum, buff);
		if (status == ERROR)
			return (ERROR);
		convertImageHeaderFromVax (buff, isub);
		break;
	case SUN_READ:
	case SUN_WRITE:
		if (!mat_lookup (fp->fptr, matnum, &entry))
			return (ERROR);
		mat_read_image_subheader (fp->fptr, entry.strtblk, isub);
		break;
	default:
		return (ERROR);
	}
	return (OK);
}

int             update_main_header (fp, mhead)
	MatrixFile     *fp;
	Main_header    *mhead;
{
	int             status;
	int				rts_wmhd ();

	if (is_acs (fp->fname))
		status = rts_wmhd (fp->fname, mhead);
	else
		status = mat_write_main_header (fp->fptr, mhead);
	return (status);
}

int             write_main_header (fp, mhead)
	MatrixFile     *fp;
	Main_header    *mhead;
{
	int             status;

	if (is_acs (fp->fname))
		status = rts_wmhd (fp->fname, mhead);
	else
		status = mat_write_main_header (fp->fptr, mhead);
	return (status);
}

int             read_main_header (fp, mhead)
	MatrixFile     *fp;
	Main_header    *mhead;
{
	int             status;
	int				rts_rmhd ();

	if (is_acs (fp->fname))
		status = rts_rmhd (fp->fname, mhead);
	else
		status = mat_read_main_header (fp->fptr, mhead);
	return (status);
}

int             write_norm_subheader (fp, matnum, nsub)
	MatrixFile     *fp;
	int             matnum;
	Norm_subheader *nsub;
{
	int             status = 0;
	char            buff[512];
	struct MatDir   entry;

	if (fp->mhptr->file_type != 4)
		return (ERROR);
	switch (fp->mode) {
	case ACS_READ:
	case ACS_WRITE:
		convertNormHeaderToVax (buff, nsub);
		status = rts_wshd (fp->fname, matnum, buff);
	case SUN_READ:
	case SUN_WRITE:
		if (!mat_lookup (fp->fptr, matnum, &entry))
			return (ERROR);
		status = mat_write_norm_subheader (fp->fptr, entry.strtblk, nsub);
	default:
		return (ERROR);
	}
	return (status);
}

int             write_sino_subheader (fp, matnum, ssub)
	MatrixFile     *fp;
	int             matnum;
	Scan_subheader *ssub;
{
	int             status = 0;
	char            buff[512];
	struct MatDir   entry;

	if (fp->mhptr->file_type != 1)
		return (ERROR);
	switch (fp->mode) {
	case ACS_READ:
	case ACS_WRITE:
		convertScanHeaderToVax (buff, ssub);
		status = rts_wshd (fp->fname, matnum, buff);
	case SUN_READ:
	case SUN_WRITE:
		if (!mat_lookup (fp->fptr, matnum, &entry))
			return (ERROR);
		status = mat_write_scan_subheader (fp->fptr, entry.strtblk, ssub);
	default:
		return (ERROR);
	}
	return (status);
}


int             write_image_subheader (fp, matnum, isub)
	MatrixFile     *fp;
	int             matnum;
	Image_subheader *isub;
{
	int             status = 0;
	char            buff[512];
	struct MatDir   entry;

	if (fp->mhptr->file_type != 2)
		return (ERROR);
	switch (fp->mode) {
	case ACS_READ:
	case ACS_WRITE:
		convertImageHeaderToVax (buff, isub);
		status = rts_wshd (fp->fname, matnum, buff);
	case SUN_READ:
	case SUN_WRITE:
		if (!mat_lookup (fp->fptr, matnum, &entry))
			return (ERROR);
		status = mat_write_image_subheader (fp->fptr, entry.strtblk, isub);
	default:
		return (ERROR);
	}
	return (status);
}


int             write_attn_subheader (fp, matnum, asub)
	MatrixFile     *fp;
	int             matnum;
	Attn_subheader *asub;
{
	int             status = 0;
	char            buff[512];
	struct MatDir   entry;

	if (fp->mhptr->file_type != 3)
		return (ERROR);
	switch (fp->mode) {
	case ACS_READ:
	case ACS_WRITE:
		convertAttnHeaderToVax (buff, asub);
		status = rts_wshd (fp->fname, matnum, buff);
	case SUN_READ:
	case SUN_WRITE:
		if (!mat_lookup (fp->fptr, matnum, &entry))
			return (ERROR);
		status = mat_write_attn_subheader (fp->fptr, entry.strtblk, asub);
	default:
		return (ERROR);
	}
	return (status);
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

