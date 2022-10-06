/*$Id: matrix_extra.c,v 1.5 1995/03/14 15:43:27 ty7777 Exp $*/
/*$Log: matrix_extra.c,v $
 * Revision 1.5  1995/03/14  15:43:27  ty7777
 * deleted get_next_entry.
 *
 * Revision 1.4  1995/03/09  21:30:20  ty7777
 * Added matrixCounter.
 *
 * Revision 1.3  1995/03/09  21:28:31  ty7777
 * Added get_next_dir_entry ().
 *
 * Revision 1.2  1995/01/30  17:06:22  ty7777
 * Added program version.
 **/

/* @(#)matrix_extra.c	1.11 11/4/92 */

#include	<stdio.h>
#include	<fcntl.h>
#include	<string.h>
#include	"matrix.h"

static char sccsid[]="@(#)matrix_extra.c	1.11 11/4/92 Copyright 1991,1992 CTI Pet Systems, Inc.";
static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/matrix_extra.c,v 1.5 1995/03/14 15:43:27 ty7777 Exp $";

typedef enum {
	MAT_OK,
	MAT_MALLOC_ERR,
	MAT_MHDREAD_ERR,
        MAT_ACSDIR_ERR,
        MAT_ACS_FILE_NOT_FOUND,
        MAT_NFS_FILE_OPEN_ERR,
        MAT_NFS_CREATE_ERR,
        MAT_FILE_TYPE_NOT_MATCH,
        MAT_READ_FROM_NILFPTR,
        MAT_NOMHD_FILE_OBJECT,
        MAT_NIL_SHPTR,
        MAT_NIL_DATA_PTR,
        MAT_MATRIX_NOT_FOUND,
        MAT_UNKNOWN_FILE_TYPE,
        MAT_READ_ACS_SUBHD_ERR,
        MAT_READ_ACS_DATA_ERR,
        MAT_ACS_DATA_SIZE_MISMATCH,
        MAT_NFS_FILE_NOT_FOUND,
        MAT_ACS_READ_SUBHD_ERR,
        MAT_NODIRPTR_FILE_OBJECT,
        MAT_READ_ACS_DIRBLK_ERR,
        MAT_ACS_CREATE_ERR,
        MAT_BAD_ATTRIBUTE,
	MAT_BAD_FILE_ACCESS_MODE,
	MAT_END_ERRORS
	}
MatrixErrorCode;

static char* mat_errors[] =
	{
		"MAT_NO_ERROR",
		"MAT_MALLOC_ERR",
		"MAT_MHDREAD_ERR",
		"MAT_ACSDIR_ERR",
		"MAT_ACS_FILE_NOT_FOUND",
		"MAT_NFS_FILE_OPEN_ERR",
		"MAT_NFS_CREATE_ERR",
		"MAT_FILE_TYPE_NOT_MATCH",
		"MAT_READ_FROM_NILFPTR",
		"MAT_NOMHD_FILE_OBJECT",
		"MAT_NIL_SHPTR",
		"MAT_NIL_DATA_PTR",
		"MAT_MATRIX_NOT_FOUND",
		"MAT_UNKNOWN_FILE_TYPE",
		"MAT_READ_ACS_SUBHD_ERR",
		"MAT_READ_ACS_DATA_ERR",
		"MAT_ACS_DATA_SIZE_MISMATCH",
		"MAT_NFS_FILE_NOT_FOUND",
		"MAT_ACS_READ_SUBHD_ERR",
		"MAT_NODIRPTR_FILE_OBJECT",
		"MAT_READ_ACS_DIRBLK_ERR",
		"MAT_ACS_CREATE_ERR",
		"MAT_BAD_ATTRIBUTE",
		"MAT_BAD_FILE_ACCESS_MODE",
		"MAT_END_ERRORS"
	};

MatrixErrorCode mat_errno ;

MatrixFile *matrix_open(fname, fmode, mtype)

  char	*fname ;
  int	fmode, mtype ;
{
  MatrixFile *mptr ;
  MatDirList *mat_read_directory() ;
  Main_header *mhead ;
  int	status;
  char *omode;

	omode = "r+";	/* assume READ_WRITE access mode */
	if (fmode == MAT_READ_ONLY) omode = "r";
	mat_errno = OK ;

	/* allocate space for MatrixFile data structure */
	mptr = (MatrixFile *) calloc(1, sizeof(MatrixFile)) ;
	if (mptr == NULL)
	{
	     mat_errno = MAT_MALLOC_ERR ;
	     return(NULL) ;
	}

	/* allocate space for main header data structure and initialize */
	mptr->mhptr = (Main_header *) calloc(1, sizeof(Main_header)) ;
	if (mptr->mhptr == NULL)
	{
	     mat_errno = MAT_MALLOC_ERR ;
	     free( mptr);
	     return(NULL) ;
	}
	mptr->acs = is_acs(fname) ;

	/* read the main header from the file */

	if (mptr->acs)
	{
		status = rts_rmhd(fname, mptr->mhptr) ;
		if (status == ERROR)
		{
		  mat_errno = MAT_ACS_FILE_NOT_FOUND ;
		  free( mptr->mhptr);
		  free( mptr);
		  return (NULL);
		}
	}
	else	/* nfs matrix file */
	{
		mptr->fptr = mat_open(fname, omode) ;
		if (mptr->fptr == NULL)
		{
		  mat_errno = MAT_NFS_FILE_OPEN_ERR ;
		  free( mptr->mhptr);
		  free( mptr);
		  return (NULL);
		}
		mat_read_main_header(mptr->fptr, mptr->mhptr) ;
	}

	/*
	   if the matrix type doesn't match the requested type, that's
	   an error. Specify matrix type GENERIC to open any type.
	*/
	if (mtype != GENERIC && mtype != mptr->mhptr->file_type)
	{
		mat_errno = MAT_FILE_TYPE_NOT_MATCH ;
		free( mptr->mhptr);
		free( mptr);
		return (NULL);
	}

	mptr->fname = (char*) malloc( 1+strlen(fname));
	strcpy(mptr->fname, fname) ;

	/* 
		read and store the matrix file directory.
	*/

	if (mptr->acs)
		mat_acs_build_dir( mptr);
	else
		mptr->dirlist = mat_read_directory( mptr->fptr);

	if (mat_errno ==OK)
	   return(mptr) ;
	else
	{
	    matrix_close( mptr);
	    return(NULL) ;
	}
}

MatrixFile *matrix_create( fname, fmode, proto_mhptr)
  char *fname;
  MatrixFileAccessMode fmode;
  Main_header *proto_mhptr;
{
	MatrixFile *mptr=NULL;
	FILE *fptr, *mat_create();

	mat_errno = MAT_OK;
	switch( fmode)
	{
	  case MAT_READ_WRITE:
	  case MAT_OPEN_EXISTING:
		mptr = matrix_open( fname, MAT_READ_WRITE, proto_mhptr->file_type);
		if (mptr) break;
		if (mat_errno != MAT_NFS_FILE_NOT_FOUND) break;
	/*
	*	if we got an NFS_FILE_NOT_FOUND error, then try to
	*	create the matrix file.
	*/
	  case MAT_CREATE:
	  case MAT_CREATE_NEW_FILE:
		mat_errno = MAT_OK;
		if (is_acs(fname))
		{
		  mat_errno = MAT_ACS_CREATE_ERR;
		  break;
		}
		fptr = mat_create( fname, proto_mhptr);
		if (!fptr)
		{
		  mat_errno = MAT_NFS_CREATE_ERR;
		  break;
		}
		mptr = (MatrixFile*) calloc( 1, sizeof(MatrixFile));
		if (!mptr)
		{
		  mat_errno = MAT_MALLOC_ERR;
		  break;
		}
		mptr->fptr = fptr;
		mptr->fname = (char*) malloc( strlen(fname)+1);
		strcpy( mptr->fname, fname);
		mptr->mhptr = proto_mhptr;
		mptr->dirlist = mat_read_directory(fptr);
		break;
	  default:
		mat_errno = MAT_BAD_FILE_ACCESS_MODE;
		break;
	}
	return mptr;
}

matrix_close(mptr)

  MatrixFile *mptr ;
{
	if (mptr == NULL) return ;
	if (mptr->mhptr != NULL) free(mptr->mhptr) ;
	if (mptr->dirlist != NULL) matrix_freelist(mptr->dirlist) ;
	mat_close(mptr->fptr);
	if (mptr->fname) free(mptr->fname);
	free(mptr) ;
}

MatrixData *matrix_read(mptr, matnum, dtype)

  MatrixFile *mptr ;
  int	matnum, dtype ;
{
  int	status ;
  MatrixData *data ;
  struct MatDir *matdir, *matrix_find() ;

	mat_errno = OK ;
	if (mptr == NULL) mat_errno = MAT_READ_FROM_NILFPTR ;
	else if (mptr->mhptr == NULL) mat_errno = MAT_NOMHD_FILE_OBJECT ;
	if (mat_errno != OK) return(NULL) ;

	/* allocate space for MatrixData structure and initialize */
	data = (MatrixData *) calloc( 1, sizeof(MatrixData)) ;
	if (data == NULL)
	{
	   mat_errno = MAT_MALLOC_ERR ;
	   return(NULL) ;
	}

	/* allocate space for subheader and initialize */
	data->shptr = (caddr_t) calloc(1, MatBLKSIZE) ;
	if (data->shptr == NULL)
	{
	   mat_errno = MAT_MALLOC_ERR ;
	   return(NULL) ;
	}

	if (mptr->acs)
	   status = read_acs_data(mptr, matnum, data, dtype) ;
	else
	   status = read_sun_data(mptr, matnum, data, dtype) ;

	if (status != OK)
	{
	   free_matrix_data(data) ;
	   data = NULL ;
	}
	else if ( dtype != GENERIC && data->data_type != dtype)
	  matrix_convert_data( data, dtype) ;
	return(data) ;
}
	

matrix_write(mptr, matnum, data, dtype)

  MatrixFile *mptr ;
  MatrixData *data ;
  int	matnum, dtype ;
{
  int	status ;

	mat_errno = OK ;
	if (mptr == NULL) mat_errno = MAT_READ_FROM_NILFPTR ;
	else if (mptr->mhptr == NULL) mat_errno = MAT_NOMHD_FILE_OBJECT ;
	else if (data->shptr == NULL) mat_errno = MAT_NIL_SHPTR ;
	else if (data->data_ptr == NULL) mat_errno = MAT_NIL_DATA_PTR ;
	if (mat_errno != OK) return (ERROR) ;

	if (mptr->acs)
	   status = write_acs_data(mptr, matnum, data) ;
	else
	   status = write_sun_data(mptr, matnum, data) ;

	return(status) ;
} 

float find_fmax( fdata, nvals)
  float *fdata;
  int nvals;
{
	float fmax;
	int i;

	for (fmax=*fdata, i=1; i<nvals; i++)
	  if (fdata[i]>fmax) fmax = fdata[i];
	return fmax;
}

read_sun_data(mptr, matnum, data, dtype) 

  MatrixFile	*mptr ;
  MatrixData	*data ;
  int	matnum , dtype;

 {
  struct MatDir *matdir, *matrix_find() ;
  int	nblks, status,data_size ;
  Scan_subheader *scansub ;
  Image_subheader *imagesub ;
  Attn_subheader *attnsub ;
  Norm_subheader *normsub ;

	matdir = matrix_find(matnum, mptr) ;
	if (matdir == NULL)
	{
	   mat_errno = MAT_MATRIX_NOT_FOUND ;
	   return(ERROR) ;
	}

	data->matnum = matnum;
	data->matfile = mptr;
	data->mat_type = mptr->mhptr->file_type;
	nblks = matdir->endblk - matdir->strtblk ;
	data_size = data->data_size = 512*nblks;
	if (dtype != MAT_SUB_HEADER)
	{
	  data->data_ptr = (caddr_t) calloc(1, data_size) ;
	  if (data->data_ptr == NULL)
	  {
	     mat_errno = MAT_MALLOC_ERR ;
	     return(ERROR) ;
	  }
	} 
	switch(mptr->mhptr->file_type)
	{
	   case SCAN_DATA :
		mat_read_scan_subheader(mptr->fptr,
		  matdir->strtblk, data->shptr) ;
		scansub = (Scan_subheader *) data->shptr ;
		data->data_type = scansub->data_type ;
		data->xdim = scansub->dimension_1 ;
		data->ydim = scansub->dimension_2 ;
		data->scale_factor = scansub->scale_factor ;
		data->pixel_size = scansub->sample_distance;
		data->data_max = scansub->scan_max * scansub->scale_factor ;
		if (dtype == MAT_SUB_HEADER) break;
		read_matrix_data(mptr->fptr, matdir->strtblk+1, nblks,
		  data->data_ptr, scansub->data_type) ;
		break ;
	   case IMAGE_DATA :
		mat_read_image_subheader(mptr->fptr,
		  matdir->strtblk, data->shptr);
		imagesub = (Image_subheader *) data->shptr ;
		data->data_type = imagesub->data_type ;
		data->xdim = imagesub->dimension_1 ;
		data->ydim = imagesub->dimension_2 ;
		data->scale_factor = imagesub->quant_scale ;
		data->pixel_size = imagesub->pixel_size;
		data->data_max = imagesub->image_max * imagesub->quant_scale;
		if (dtype == MAT_SUB_HEADER) break;
		read_matrix_data(mptr->fptr, matdir->strtblk+1, nblks,
		  data->data_ptr, imagesub->data_type) ;
		break ;
	   case ATTN_DATA :
	      	mat_read_attn_subheader(mptr->fptr,
		  matdir->strtblk, data->shptr);
		attnsub = (Attn_subheader *) data->shptr ;
		data->data_type = attnsub->data_type ;
		data->xdim = attnsub->dimension_1 ;
		data->ydim = attnsub->dimension_2 ;
		data->scale_factor = attnsub->scale_factor ;
		data->pixel_size = attnsub->sample_distance;
		if (dtype == MAT_SUB_HEADER) break;
		read_matrix_data(mptr->fptr, matdir->strtblk+1, nblks,
		  data->data_ptr, attnsub->data_type) ;
		data->data_max = data->scale_factor * 
		  find_fmax( data->data_ptr, data->xdim * data->ydim);
		break ;
	   case NORM_DATA :
		mat_read_norm_subheader(mptr->fptr,
		  matdir->strtblk, data->shptr) ;
		normsub = (Norm_subheader *) data->shptr ;
		data->data_type = normsub->data_type ;
		data->xdim = normsub->dimension_1 ;
		data->ydim = normsub->dimension_2 ;
		data->scale_factor = normsub->scale_factor ;
		if (dtype == MAT_SUB_HEADER) break;
		read_matrix_data(mptr->fptr, matdir->strtblk+1, nblks,
		  data->data_ptr, normsub->data_type) ;
		data->data_max = data->scale_factor * 
		  find_fmax( data->data_ptr, data->xdim * data->ydim);
		break ;
	    default :
		mat_errno = MAT_UNKNOWN_FILE_TYPE ;
		return(ERROR) ;
		break ;
	}
	return(OK) ;
}


read_acs_data(mptr, matnum, data, dtype)
	/* assumming the file exists on acs disk */

  MatrixFile	*mptr ;
  MatrixData	*data ;
  int	matnum, dtype;

{
  struct MatDir *matdir, *matrix_find() ;
  int	nblks, data_size ;
  Scan_subheader *scansub ;
  Image_subheader *imagesub ;
  Attn_subheader *attnsub ;
  Norm_subheader *normsub ;
  char	buff[512] ;
  int	status, ydim, xdim, data_out ;
  short	*scan_data, *image_data ;
  float	*norm_data, *attn_data ;
  float scale_factor ;

	matdir = matrix_find(matnum, mptr) ;
	if (matdir == NULL)
	{
	   mat_errno = MAT_MATRIX_NOT_FOUND ;
	   return(ERROR) ;
	}

	data->mat_type = mptr->mhptr->file_type ;
	data->matnum = matnum;
	data->matfile = mptr;
	nblks = matdir->endblk - matdir->strtblk ;
	data_size = data->data_size = 512*nblks;
	if (dtype != MAT_SUB_HEADER)
	{
	  data->data_ptr = (caddr_t) calloc(1, data_size) ;
	  if (data->data_ptr == NULL)
	  {
	     mat_errno = MAT_MALLOC_ERR ;
	     return(ERROR) ;
	  }
	} 
	status = rts_rshd(mptr->fname, matnum, buff) ;
	if (status == ERROR)
	{
	   mat_errno = MAT_READ_ACS_SUBHD_ERR ;
	   return(ERROR) ;
	}
	switch (mptr->mhptr->file_type)
	{
	    case SCAN_DATA :
		scansub = (Scan_subheader *) data->shptr ;
		convertScanHeaderFromVax(buff, scansub) ;
		data->xdim = xdim = scansub->dimension_1 ;
		data->ydim = ydim = scansub->dimension_2 ;
		data->data_type = scansub->data_type ;
		data->data_size = xdim*ydim*sizeof(short)  ;
		data->scale_factor = scansub->scale_factor ;
		data->pixel_size = scansub->sample_distance ;
		data->data_max = scansub->scan_max * scansub->scale_factor;
		if (dtype == MAT_SUB_HEADER) break;
		status = rts_rdat(mptr->fname, matnum, data->data_ptr, &data_out);
		if (status == ERROR)
		{
		    mat_errno = MAT_READ_ACS_DATA_ERR ;
		    return(ERROR) ;
		}		
		break ;
	    case IMAGE_DATA:
		imagesub = (Image_subheader *) data->shptr ;
		convertImageHeaderFromVax(buff, imagesub) ;
		data->xdim = xdim = imagesub->dimension_1 ;
		data->ydim = ydim = imagesub->dimension_2 ;
		data->data_type = imagesub->data_type ;
		data->data_size = xdim*ydim*2 ;
		data->scale_factor = imagesub->quant_scale ;
		data->pixel_size = imagesub->pixel_size;
		data->data_max = imagesub->image_max * imagesub->quant_scale;
		if (dtype == MAT_SUB_HEADER) break;
		status = rts_rdat(mptr->fname, matnum, data->data_ptr, &data_out);
		if (status == ERROR)
		{
		   mat_errno = MAT_READ_ACS_DATA_ERR ;
		   return(ERROR) ;
		}
		break ;
	    case ATTN_DATA:
		attnsub = (Attn_subheader *) data->shptr ;
		convertAttnHeaderFromVax(buff, attnsub) ;
		data->xdim = attnsub->dimension_1 ;
		data->ydim = attnsub->dimension_2 ;
		data->data_type = attnsub->data_type ;
		data->data_size = data->xdim*data->ydim*sizeof(float) ;
		data->scale_factor = attnsub->scale_factor ;
		data->pixel_size = attnsub->sample_distance ;
		if (dtype == MAT_SUB_HEADER) break;
		status = rts_rdat(mptr->fname, matnum, data->data_ptr, &data_out);
		if (status == ERROR)
		{
		   mat_errno = MAT_READ_ACS_DATA_ERR ;
		   return(ERROR) ;
		}
		data->data_max = data->scale_factor * 
		  find_fmax( data->data_ptr, data->xdim * data->ydim);
		break ;
	    case NORM_DATA:
		normsub = (Norm_subheader *) data->shptr ;
		convertNormHeaderFromVax(buff, normsub) ;
		data->xdim = normsub->dimension_1 ;
		data->ydim = normsub->dimension_2 ;
		data->data_type = normsub->data_type ;
		data->data_size = data->xdim*data->ydim*sizeof(float) ;
		data->scale_factor = normsub->scale_factor ;
		if (dtype == MAT_SUB_HEADER) break;
		status = rts_rdat(mptr->fname, matnum, data->data_ptr, &data_out);
		if (status == ERROR)
		{
		   mat_errno = MAT_READ_ACS_DATA_ERR ;
		   return(ERROR) ;
		}
		data->data_max = data->scale_factor * 
		  find_fmax( data->data_ptr, data->xdim * data->ydim);
		break ;
	}
	return(OK) ;
}


write_sun_data(mptr, matnum, data)

  MatrixFile *mptr ;
  MatrixData *data ;
  int	matnum ;
{
  struct MatDir *matdir, *matrix_find(), dir_entry ;
  Scan_subheader *scansub ;
  Image_subheader *imagesub ;
  Attn_subheader *attnsub ;
  Norm_subheader *normsub ;
  int	status, blkno, nblks ;
	
	status = OK ;
	matdir = matrix_find(matnum, mptr) ;
	nblks = (data->data_size+511)/512;
	if (matdir == NULL)
	{
	   blkno = mat_enter(mptr->fptr, matnum, nblks) ;
	   dir_entry.matnum = matnum ;
	   dir_entry.strtblk = blkno ;
	   dir_entry.endblk = dir_entry.strtblk + nblks - 1 ;
	   dir_entry.matstat = 1 ;
	   insert_mdir(dir_entry, mptr->dirlist) ;
	   matdir = &dir_entry ;
	}

	switch(mptr->mhptr->file_type)
	{
	   case SCAN_DATA :
	      scansub = (Scan_subheader *) data->shptr ;
	      mat_write_scan_subheader(mptr->fptr, matdir->strtblk, data->shptr);
	      status = write_matrix_data(mptr->fptr, matdir->strtblk+1,
			nblks, data->data_ptr, scansub->data_type) ;
	      break ;
	   case IMAGE_DATA :
	      imagesub = (Image_subheader *) data->shptr ;
	      mat_write_image_subheader(mptr->fptr,matdir->strtblk,data->shptr);
	      status = write_matrix_data(mptr->fptr, matdir->strtblk+1,
			nblks, data->data_ptr, imagesub->data_type) ;
	      break ;
	   case ATTN_DATA :
	      attnsub = (Attn_subheader *) data->shptr ;
	      mat_write_attn_subheader(mptr->fptr, matdir->strtblk, data->shptr);
	      status = write_matrix_data(mptr->fptr, matdir->strtblk+1,
			nblks, data->data_ptr, attnsub->data_type) ;
	      break ;
	   case NORM_DATA :
	      normsub = (Norm_subheader *) data->shptr ;
	      mat_write_norm_subheader(mptr->fptr, matdir->strtblk, data->shptr);
	      status = write_matrix_data(mptr->fptr, matdir->strtblk+1,
			nblks, data->data_ptr, normsub->data_type) ;
	      break ;
	   default :	/* default treated as sinogram */
	      scansub = (Scan_subheader *) data->shptr ;
	      mat_write_scan_subheader(mptr->fptr, matdir->strtblk, data->shptr);
	      status = write_matrix_data(mptr->fptr, matdir->strtblk+1,
			nblks, data->data_ptr, scansub->data_type) ;
	      break ;
	}
	return(status) ;
}

write_acs_data(mptr, matnum, data)

  MatrixFile	*mptr ;
  MatrixData	*data ;
  int	matnum ;
{
  int	status, data_size ;
  char	buff[512] ;

	switch(mptr->mhptr->file_type)
	{
	   case SCAN_DATA :
		convertScanHeaderToVax(buff, data->shptr) ;
		status = rts_wshd(mptr->fname, matnum, buff) ;
		if (status == ERROR)
		{
		   mat_errno = MAT_READ_ACS_SUBHD_ERR ;
		   return(ERROR) ;
		}
		break ;
	   case IMAGE_DATA :
		convertImageHeaderToVax(buff, data->shptr) ;
		status = rts_wshd(mptr->fname, matnum, buff) ;
		if (status == ERROR)
		{
		   mat_errno = MAT_READ_ACS_SUBHD_ERR ;
		   return(ERROR) ;
		}
		break ;
	    case ATTN_DATA :
		convertAttnHeaderToVax(buff, data->shptr) ;
		status = rts_wshd(mptr->fname, matnum, buff) ;
		if (status == ERROR)
		{
		   mat_errno = MAT_READ_ACS_SUBHD_ERR ;
		   return(ERROR) ;
		}
		break ;
	    case NORM_DATA :
		convertNormHeaderToVax(buff, data->shptr) ;
		status = rts_wshd(mptr->fname, matnum, buff) ;
		if (status == ERROR)
		{
		   mat_errno = MAT_READ_ACS_SUBHD_ERR ;
		   return(status) ;
		}
		break ;
	}
	status = rts_wdat(mptr->fname, matnum, 
	  data->data_type, data->data_ptr, data->data_size) ;
	if (status == ERROR) mat_errno = MAT_READ_ACS_DATA_ERR ;
	return(status) ;
}

read_raw_acs_data( fname, strtblk, nblks, dptr, dtype)
  char *fname;
  int strtblk, nblks, dtype;
  char * dptr;
{
	int i, err;
	float get_vax_float() ;

	for (i=0; i<nblks; i++)
	{
	  err = rtsRblk( fname, strtblk+i, dptr+512*i);
	  if (err) return ERROR;
	}
	switch( dtype)
	{
		case BYTE_TYPE:
		  break;
		case VAX_I2:
		  swap_byte (dptr, dptr, 512*nblks);
		  break;
		case VAX_I4:
		  swap_byte (dptr, dptr, 512*nblks);
		  swaw( dptr, dptr, 256*nblks);
		  break;
		case VAX_R4:
		  swap_byte ( dptr, dptr, 512*nblks);
		  for (i=0; i<nblks*128; i++)
		    ((float *)dptr)[i] = get_vax_float( (short *)dptr, i*2) ;
		  break;
		case SUN_R4:
		case SUN_I2:
		case SUN_I4:
		  break;
		default:	/* something else...treat as Vax I*2 */
		  swap_byte (dptr, dptr, 512*nblks);
		  break;
	}
	return OK;
}


read_matrix_data( fptr, strtblk, nblks, dptr, dtype)
  FILE *fptr;
  int strtblk, nblks, dtype;
  char * dptr;
{
	int i, err;
	float get_vax_float() ;

	err = mat_rblk( fptr, strtblk, dptr, nblks);
	if (err) return ERROR;
	switch( dtype)
	{
		case BYTE_TYPE:
		  break;
		case VAX_I2:
		  swap_byte (dptr, dptr, 512*nblks);
		  break;
		case VAX_I4:
		  swap_byte (dptr, dptr, 512*nblks);
		  swaw( dptr, dptr, 256*nblks);
		  break;
		case VAX_R4:
		  swap_byte (dptr, dptr, 512*nblks);
		  for (i=0; i<nblks*128; i++)
		    ((float *)dptr)[i] = get_vax_float( (short *)dptr, i*2) ;
		  break;
		case SUN_R4:
		case SUN_I2:
		case SUN_I4:
		  break;
		default:	/* something else...treat as Vax I*2 */
		  swap_byte (dptr, dptr, 512*nblks);
		  break;
	}
	return OK;
}


write_matrix_data( fptr, strtblk, nblks, dptr, dtype)
  FILE *fptr;
  int strtblk, nblks, dtype;
  char * dptr;
{
	int err ;
	int i;

	switch( dtype)
	{
		case BYTE_TYPE:
		  err = mat_wblk( fptr, strtblk, dptr, nblks);
		  break;
		case VAX_I2:
		case VAX_I4:
		  err = mat_write_idata( fptr, strtblk, dptr, 512*nblks);
		  break;
		case VAX_R4:
		  err = mat_write_fdata( fptr, strtblk, dptr, 512*nblks);
		  break;
		case SUN_R4:
		case SUN_I2:
		case SUN_I4:
		  err = mat_wblk( fptr, strtblk, dptr, nblks);
		  break;
		default:	/* something else...treat as Vax I*2 */
		  err = mat_write_idata( fptr, strtblk, dptr, 512*nblks);
		  break;
	}
	return err;
}


struct MatDir *matrix_find(matnum, matfile)

  int	matnum ;
  MatrixFile *matfile ;

{
  MatDirNode	*node ;
  struct MatDir	*matdir ;
  int	size ;

	if (matfile == NULL) return(NULL) ;
	if (matfile->dirlist == NULL) return(NULL) ;	
	size = sizeof(struct MatDir) ;
	matdir = (struct MatDir *) malloc(sizeof(struct MatDir)) ;
	matdir->matnum = 0 ;
	matdir->strtblk = 0 ;
	matdir->endblk = 0 ;
	matdir->matstat = 0 ;
	node = matfile->dirlist->first ;
	while (node != NULL)
	{
	   if (node->matnum == matnum)
	   {
		matdir->matnum = node->matnum ;
		matdir->strtblk = node->strtblk ;
		matdir->endblk = node->endblk ;
		matdir->matstat = node->matstat ;
		break ;
	   }
	   node = node->next ;
	}
	if (node != NULL) return(matdir) ;
	else return(NULL) ;
}
	

matrix_freelist(matdirlist)

  MatDirList	*matdirlist ;

{
  MatDirNode	*node, *next ;

	if (matdirlist == NULL) return ;
	if (matdirlist->first != NULL)
	{
	  node = matdirlist->first ;
	  do
	  {
	    next = node->next ;
	    free(node) ;
	    node = next ;
	  }
	  while(next != NULL) ;
	}
	free(matdirlist) ;
}

insert_mdir(matdir, dirlist)

  MatDirList	*dirlist ;
  struct MatDir	matdir ;

{
  MatDirNode	*node ;
 
	if (dirlist == NULL)
	{
	    dirlist = (MatDirList *) malloc(sizeof(MatDirList)) ;
	    if (dirlist == NULL) return(ERROR) ;
	    dirlist->nmats = 0 ;
	    dirlist->first = NULL ;
	    dirlist->last = NULL ;
	}
	node = (MatDirNode *) malloc(sizeof(MatDirNode)) ;
	if (node == NULL) return(ERROR) ;

	node->matnum = matdir.matnum ;
	node->strtblk = matdir.strtblk ;
	node->endblk = matdir.endblk ;
	node->matstat = matdir.matstat;
	node->next = NULL ;

	if (dirlist->first == NULL)	/* if list was empty, add first node */
	{
	   dirlist->first = node ;
	   dirlist->last = node ;
	   dirlist->nmats = 1 ;
	}
	else
	{
	   (dirlist->last)->next = node ;
	   dirlist->last = node ;
	   ++(dirlist->nmats) ;
	}
}

MatDirBlk *mat_rdirblk(fptr, blknum)

  FILE	*fptr ;
  int	blknum ;

{
  MatDirBlk *matdirblk ;
  int	i, j, err, ndirs ;
  int	dirbufr[MatBLKSIZE/4] ;
  char	bytebufr[MatBLKSIZE] ;

	bzero(bytebufr, MatBLKSIZE) ;
	matdirblk = (MatDirBlk *) malloc(MatBLKSIZE) ;
	if (matdirblk == NULL) return(NULL) ;
	

	err = mat_rblk(fptr, blknum, bytebufr, 1) ;
	if (err < 0) return(NULL) ;
	swap_byte (bytebufr, dirbufr, MatBLKSIZE) ;
	swaw(dirbufr, dirbufr, MatBLKSIZE/2) ;


	matdirblk->nfree = dirbufr[0] ;
	matdirblk->nextblk = dirbufr[1] ;
	matdirblk->prvblk = dirbufr[2] ;
	matdirblk->nused = dirbufr[3] ;

	ndirs = (MatBLKSIZE/4-4)/4 ;
	for (i=0 ; i<ndirs ; i++)
	{	
	   matdirblk->matdir[i].matnum = 0 ;
	   matdirblk->matdir[i].strtblk = 0 ;
	   matdirblk->matdir[i].endblk = 0 ;
	   matdirblk->matdir[i].matstat = 0 ;
	}

	for (i=0; i<matdirblk->nused ; i++)
	{
	   j = i+1 ;
	   matdirblk->matdir[i].matnum = dirbufr[j*4+0] ;
	   matdirblk->matdir[i].strtblk = dirbufr[j*4+1] ;
	   matdirblk->matdir[i].endblk = dirbufr[j*4+2] ;
	   matdirblk->matdir[i].matstat = dirbufr[j*4+3] ;
	}
	return(matdirblk) ;
}


MatDirList *mat_read_directory(fptr)

  FILE	*fptr ;
{
  struct MatDir matdir ;
  MatDirList	*dirlist ;
  MatDirBlk	*matdirblk, *mat_rdirblk() ;
  int		i, blknum ;

	dirlist = (MatDirList *) malloc(sizeof(MatDirList)) ;
	if (dirlist == NULL) return(NULL) ;
	bzero((char *) dirlist, sizeof(MatDirList)) ;

	blknum = MatFirstDirBlk ;
	do
	{
	   matdirblk = mat_rdirblk(fptr, blknum) ;
	   if (matdirblk == NULL) return(NULL) ;
	   for (i=0 ; i<matdirblk->nused ; i++)
	   {
		matdir.matnum = matdirblk->matdir[i].matnum ;
		matdir.strtblk = matdirblk->matdir[i].strtblk ;
		matdir.endblk = matdirblk->matdir[i].endblk ;
		matdir.matstat = matdirblk->matdir[i].matstat ;
		insert_mdir(matdir, dirlist) ;
	   }
	   blknum = matdirblk->nextblk ;
	   free((char *) matdirblk) ;
	}
	while (blknum != MatFirstDirBlk) ;
	return(dirlist) ;
}

mat_acs_build_dir(mptr)

  MatrixFile *mptr ;

{
  struct MatDir matdir ;
  MatDirList *dirlist ;
  MatDirBlk *dirblk ;
  char	buff[512] ;
  int i, blkno, status ;

	dirlist = (MatDirList *) malloc(sizeof(MatDirList)) ;
	if (dirlist == NULL)
	{
	   mat_errno = MAT_NODIRPTR_FILE_OBJECT ;
	   return(ERROR) ;
	}
	bzero((char *) dirlist, sizeof(MatDirList)) ;
	blkno = MatFirstDirBlk ;
	do
	{
	   status = rtsRblk(mptr->fname, blkno, buff) ;
	   if (status == ERROR)
	   {
	      mat_errno = MAT_READ_ACS_DIRBLK_ERR ;
	      return(ERROR) ;
	   }
	   swap_byte (buff, buff, 512) ;
	   swaw(buff, buff, 256) ;
	   dirblk = (MatDirBlk *) buff ;
	   for (i=0 ; i<dirblk->nused ; i++)
	   {
		matdir.matnum = dirblk->matdir[i].matnum ;
		matdir.strtblk = dirblk->matdir[i].strtblk ;
		matdir.endblk = dirblk->matdir[i].endblk ;
		matdir.matstat = dirblk->matdir[i].matstat ;
		insert_mdir(matdir, dirlist) ;
	   }
	   blkno = dirblk->nextblk ;
	} while (blkno != MatFirstDirBlk) ;
	mptr->dirlist = dirlist ;
	return(OK) ;
}

free_matrix_data(data)

  MatrixData	*data ;

{
	if (data != NULL)
	{
	   if (data->data_ptr != NULL) free(data->data_ptr) ;
	   if (data->shptr != NULL) free(data->shptr) ;
	   free(data) ;
	}

}


matrix_convert_data()
{
	return OK ;	/* dummy for now */
}


#include	<sys/types.h>
#include	<sys/stat.h>

file_exists(filename)	/* subroutine to see if file exists or not */

  char *filename ;

{
  struct stat stbuf ;
  int	status ;
  Main_header mhead ;

	if (!is_acs(filename))
	{
	   if (stat(filename, &stbuf) == -1) return(FALSE) ;
	   return(TRUE) ;
	}
	else
	{
	   status  = rts_rmhd(filename, &mhead) ;
	   if (status == OK) return(TRUE) ;
	   else return(FALSE) ;
	}
}

is_acs(fname)

  char	*fname ;

{
 	if (strindex(fname, "/sd") == 0)
	    return(TRUE) ; 
	else
	   return(FALSE) ;
}

/* subroutine to return starting position of substring within string */
/* return index of t in s, -1 if none */

strindex(s, t)

  char	s[], t[] ;

{
  int	i, j, k ;

	for (i=0 ; s[i] !='\0' ; i++)
	{
	   for (j=i, k=0 ; t[k] != '\0' && s[j]==t[k] ; j++, k++) ;
	   if (k > 0 && t[k] == '\0') return i ;
	}
	return -1 ;
}

matrix_perror( s)
  char *s;
{
	fprintf( stderr, "%s: %s\n", s, mat_errors[mat_errno]);
	exit(-1);
}

#include <varargs.h>

static int item_size[]={ 0, 1, 2, 4, 4, 4, 2, 4 };

MatrixData *matrix_object_create( va_alist)
va_dcl
{

	va_list arp;
	MatrixData *object, *proto_object=NULL;
	MatrixObjectAttribute attrib;
	int nitems;
	Scan_subheader *ssh;
	Image_subheader *ish;
	Attn_subheader *ash;
	Norm_subheader *nsh;

	object = (MatrixData*) calloc( 1, sizeof(MatrixData));
	if (!object)
	{
	  mat_errno = MAT_MALLOC_ERR;
	  return NULL;
	}
	va_start( arp);
	object->mat_type = va_arg( arp, int);
	while (attrib = va_arg( arp, MatrixObjectAttribute))
	  switch( attrib)
	  {
		case MAT_XDIM:
			object->xdim = va_arg( arp, int);
			break;
		case MAT_YDIM:
			object->ydim = va_arg( arp, int);
			break;
		case MAT_ZDIM:
			object->zdim = va_arg( arp, int);
			break;
		case MAT_DATA_TYPE:
			object->data_type = va_arg( arp, int);
			break;
		case MAT_SCALE_FACTOR:
			object->scale_factor = va_arg( arp, double);
			break;
		case MAT_PIXEL_SIZE:
			object->pixel_size = va_arg( arp, double);
			break;
		case MAT_Y_SIZE:
			object->y_size = va_arg( arp, double);
			break;
		case MAT_Z_SIZE:
			object->z_size = va_arg( arp, double);
			break;
		case MAT_DATA_MAX:
			object->data_max = va_arg( arp, double);
			break;
		case MAT_DATA_MIN:
			object->data_min = va_arg( arp, double);
			break;
		case MAT_PROTO:
			proto_object = va_arg( arp, MatrixData*);
			copy_proto_object( object, proto_object);
			break;
		default:
			mat_errno = MAT_BAD_ATTRIBUTE;
			return NULL;
	  }
	nitems = object->xdim*object->ydim;
	if (object->zdim>0) nitems *= object->zdim;
	object->data_size = item_size[(int)object->data_type]*nitems;
	object->data_ptr = (caddr_t) calloc( 1, object->data_size);
	if (!object->data_ptr)
	{
		mat_errno = MAT_MALLOC_ERR;
		return NULL;
	}
	if (!object->shptr) switch( object->mat_type)
	{
	  case SCAN_DATA:
		/*
		ssh = (Scan_subheader*)(object->shptr)
		*/
	        ssh    = (Scan_subheader*) calloc( 1, sizeof(Scan_subheader));
		ssh->data_type = object->data_type;
		ssh->dimension_1 = object->xdim;
		ssh->dimension_2 = object->ydim;
		ssh->sample_distance = object->pixel_size;
		ssh->scale_factor = object->scale_factor;
		break;
	  case IMAGE_DATA:
		/*
		ish = (Image_subheader*)(object->shptr)
		*/
		ish    = (Image_subheader*) calloc( 1, sizeof(Image_subheader));
		ish->data_type = object->data_type;
		ish->num_dimensions = 2;
		ish->dimension_1 = object->xdim;
		ish->dimension_2 = object->ydim;
		ish->pixel_size = object->pixel_size;
		ish->quant_scale = object->scale_factor;
		break;
	  case ATTN_DATA:
		/*
		ash = (Attn_subheader*)(object->shptr)
		*/
		ash    = (Attn_subheader*) calloc( 1, sizeof(Attn_subheader));
		ash->data_type = object->data_type;
		ash->dimension_1 = object->xdim;
		ash->dimension_2 = object->ydim;
		ash->sample_distance = object->pixel_size;
		ash->scale_factor = object->scale_factor;
		break;
	  case NORM_DATA:
		/*
		nsh = (Norm_subheader*)(object->shptr)
		*/
		nsh    = (Norm_subheader*) calloc( 1, sizeof(Norm_subheader));
		nsh->data_type = object->data_type;
		nsh->dimension_1 = object->xdim;
		nsh->dimension_2 = object->ydim;
		nsh->scale_factor = object->scale_factor;
		break;
	  default:
		mat_errno = MAT_UNKNOWN_FILE_TYPE;
		return NULL;
	}
	return object;
}

copy_proto_object( new, old)
  MatrixData *new, *old;
{
	int sh_size;

	if (new->mat_type == old->mat_type)
	{
	  switch (new->mat_type)
	  {
		case SCAN_DATA:
			sh_size = sizeof(Scan_subheader);
			break;
		case IMAGE_DATA:
			sh_size = sizeof(Image_subheader);
			break;
		case ATTN_DATA:
			sh_size = sizeof(Attn_subheader);
			break;
		case NORM_DATA:
			sh_size = sizeof(Norm_subheader);
			break;
	  }
	  new->shptr = (caddr_t) malloc( sh_size);
	  bcopy( old->shptr, new->shptr, sh_size);
	}
	new->xdim = old->xdim;
	new->ydim = old->ydim;
	new->zdim = old->zdim;
	new->data_type = old->data_type;
	new->scale_factor = old->scale_factor;
	new->pixel_size = old->pixel_size;
	new->y_size = old->y_size;
	new->z_size = old->z_size;
	new->data_min = old->data_min;
	new->data_max = old->data_max;
}

Main_header *matrix_init_main_header( fname, ftype, mh_proto)
  char *fname;
  MatrixFileType ftype;
  Main_header *mh_proto;
{
	Main_header *mhptr;

	mhptr = (Main_header*) calloc( 1, sizeof(Main_header));
	if (mh_proto)
	  bcopy( mh_proto, mhptr, sizeof(Main_header));
	mhptr->file_type = ftype;
	strncpy( mhptr->original_file_name, fname, 20);
	return mhptr;
}

matrix_free( matrix)
  MatrixData *matrix;
{
	if (matrix->shptr) free( matrix->shptr);
	if (matrix->data_ptr) free( matrix->data_ptr);
	free( matrix);
}

convert_float_scan( scan, fdata)
  MatrixData *scan;
  float *fdata;
{
	int i, nvals, tot;
	float fmax, scale;
	short int *sdata;
	Scan_subheader *ssh;

	if (scan->data_ptr)
	  free(scan->data_ptr);
	nvals = scan->xdim*scan->ydim;
	sdata = (short int*) malloc( nvals*sizeof(short int));
	scan->data_ptr = (caddr_t) sdata;
	scan->data_size = nvals*sizeof(short int);
	fmax = *fdata;
	for (i=0; i<nvals; i++)
	  if (fdata[i] > fmax) fmax = fdata[i];
	scale = 1.0;
	if (fmax > 0.0) scale = 32767./fmax;
	tot = 0;
	for (i=0; i<nvals; i++)
	{
	  sdata[i] = scale*fdata[i];
	  tot += sdata[i];
	}
	scan->scale_factor = 1.0/scale;
	ssh = (Scan_subheader*) scan->shptr;
	ssh->scan_min = 0;
	ssh->scan_max = fmax*scale;
	ssh->dimension_1 = scan->xdim;
	ssh->dimension_2 = scan->ydim;
	ssh->net_trues = tot;
	ssh->scale_factor = 1.0/scale;
        ssh->sample_distance = scan->pixel_size;
}
