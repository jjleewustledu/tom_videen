/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : Feb. 1998
  Modifications :
* endtopic History

******************************************************************************/
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "matrix.h"
#include "matrix_extra.h"
#include "NChrplus.h"

/* ====================================================================== */

MatrixData     *
read_a_slice(
	     MatrixFile * inputfile,
	     MatrixData ** inputvolume,
	     int is3d,
	     int frame,
	     int plane,
	     int gate,
	     int data,
	     int bed)

{
  int             matnum;
  MatrixData     *inmatrix;
  if (is3d)
  {
    if (*inputvolume == NULL)
    {
      matnum = mat_numcod(frame, 1, gate, data, bed);
      *inputvolume = matrix_read(inputfile, matnum, MAT_SUB_HEADER);
      if (!*inputvolume)
      {
	printf("Unable to read the volume\n");
	exit(0);
      }
    }
    inmatrix = matrix_read_slice(inputfile, *inputvolume, plane - 1, 0);
    if (!inmatrix)
    {
      printf("Unable to read the slice from the volume");
      exit(0);
    }
  }
  else
  {
    matnum = mat_numcod(frame, plane, gate, data, bed);
    inmatrix = matrix_read(inputfile, matnum, UnknownMatDataType);
    if (!inmatrix)
    {
      printf("Unable to read the slice from the image file\n");
      exit(0);
    }
  }
  return (inmatrix);

}
/* ====================================================================== */
void 
scale_image(
	    float *img,
	    int ncols,
	    int nrows,
	    float *scalefactor,
	    short *simg,
	    float *imagemin,
	    float *imagemax,
	    int version)

{
  int             i;
  float          *p_img;
  short          *p_simg;

  p_img = img;

  /* If not version 70, scale this slice independently ************************************************** */
  if (version < 70)
  {
    *imagemax = *p_img;
    *imagemin = *p_img;
  }

  for (i = 0; i < ncols * nrows; i++)
  {
    if (*p_img  > *imagemax)
      *imagemax = *p_img;
    if (*p_img < *imagemin)
      *imagemin = *p_img;
    p_img++;
  }

  /* If not version 70, compute a scale factor, otherwise, use the one from
   * the original study. ************************************** */
  if (version < 70)
  {
    if ((-*imagemin) > *imagemax)
      *scalefactor = (-*imagemin) / 32760.0;
    else
    {
      if (*imagemax > 0)
	*scalefactor = *imagemax / 32760.0;
      else
	*scalefactor = 1.0;
    }
  }

  p_simg = simg;
  p_img = img;
  for (i = 0; i < ncols * nrows; i++)
  {
    *p_simg++ = (short) (*p_img++ / *scalefactor + 0.5);
  }


}

/* ====================================================================== */
main(argc, argv)
  int             argc;
  char           *argv[];

{
  char            infilename[150];
  char            outfilename_cor[150];
  char            outfilename_sag[150];
  int             showhelp = 0;
  int             frame = 1;
  int             firstplane = 1;
  int             gate = 1;
  int             data = 0;
  int             bed = 0;
  int             i;
  int             j;
  int             k;
  int             col;
  int             n_img = 24;
  int             sum_cor = 3;
  int             sum_sag = 4;
  int             start_cor = 15;
  int             start_sag = 20;
  int             version = 6;
  int             verbose = 0;
  int             is3d;
  int             ncols = -1;
  int             nrows = -1;
  int             nplanes = -1;
  int             matnum_out;
  float           imagemin;
  float           imagemax;
  float           scalefactor;
  MatrixFile     *inputfile = NULL;
  MatrixData     *inputvolume = NULL;
  MatrixData     *inmatrix;
  MatrixFile     *outputfile_cor = NULL;
  MatrixFile     *outputfile_sag = NULL;
  MatrixData     *outmatrix;
  Image_subheader *input_sub;
  Image_subheader *output_sub;
  Main_header     proto;
  float         **inimages;
  int             n_inimgs;
  float          *workimg_cor;
  float          *workimg_sag;
  short          *shortdata = NULL;


  /* Get the arguments ****************** */
  strcpy(infilename, "");
  strcpy(outfilename_cor, "");
  strcpy(outfilename_sag, "");
  i = 1;
  while (i < argc)
  {
    if (strcmp(argv[i], "-i") == 0)
      strcpy(infilename, argv[i++ + 1]);
    if (strcmp(argv[i], "-oc") == 0)
      strcpy(outfilename_cor, argv[i++ + 1]);
    if (strcmp(argv[i], "-os") == 0)
      strcpy(outfilename_sag, argv[i++ + 1]);
    if (strcmp(argv[i], "-ni") == 0)
      sscanf(argv[i++ + 1], "%d", &n_img);
    if (strcmp(argv[i], "-nc") == 0)
      sscanf(argv[i++ + 1], "%d", &sum_cor);
    if (strcmp(argv[i], "-sc") == 0)
      sscanf(argv[i++ + 1], "%d", &start_cor);
    if (strcmp(argv[i], "-ns") == 0)
      sscanf(argv[i++ + 1], "%d", &sum_sag);
    if (strcmp(argv[i], "-ss") == 0)
      sscanf(argv[i++ + 1], "%d", &start_sag);
    if (strcmp(argv[i], "-V") == 0)
      sscanf(argv[i++ + 1], "%d", &version);
    if (strcmp(argv[i], "-h") == 0)
      showhelp = 1;
    if (strcmp(argv[i], "-v") == 0)
      verbose = 1;
    i++;
  }


  showhelp = showhelp || (strcmp(infilename, "") == 0);
  if (showhelp)
  {
    printf("Syntax: program -i inputfile -oc coronale_file -os sagitale_file\n");
    printf("         [-ni aantal_images] [-sc start_rij_cor] [-nc someer_n_cor]\n");
    printf("                             [-ss start_rij_sag] [-ns someer_n_sag]\n");

    printf("                             [-V versie] [-v]\n");
    printf("inputfile:  input file (PetVolume version 7.0\n");
    printf("outputfile_cor: output file coronaal\n");
    printf("versie:     versie van de output files: 6 or 70\n");
    printf("-v:         verbose\n");
    printf("Defaults : \n");
    printf("           -ni 24 \n");
    printf("           -nc  3 \n");
    printf("           -sc 15 \n");
    printf("           -ns  4 \n");
    printf("           -ss 20 \n");
    printf("           -V   6 \n");
    printf("-h:         show this help and stop\n");
    exit(0);
  }



  /* Open the input file ******************** */
  inputfile = matrix_open(infilename, MAT_READ_ONLY, PetImage);
  if (!inputfile)
  {
    is3d = 1;
    inputfile = matrix_open(infilename, MAT_READ_ONLY, PetVolume);
    if (!inputfile)
    {
      printf("Unable to open the file %s as an image or a volume\n",
	     infilename);
      exit(0);
    }
  }
  else
    is3d = 0;

  /* Read a slice to get the subheader. *********************************** */
  inmatrix = read_a_slice(inputfile, &inputvolume, is3d, frame, firstplane,
			  gate, data, bed);
  input_sub = (Image_subheader *) inmatrix->shptr;
  ncols = input_sub->x_dimension;
  nrows = input_sub->y_dimension;
  nplanes = input_sub->z_dimension;
  if (!is3d)
    nplanes = inputfile->mhptr->num_planes;
  if (verbose)
    printf("Volume size is %d x %d x %d\n",
	   ncols, nrows, nplanes);




  /* Read the first images. Images that don't exist are set to NULL ************************************** */
  inimages = (float **) calloc(nplanes, sizeof(float *));

  if (verbose)
    printf("Reading first images...\n");
  for (i = 1; i <= nplanes; i++)
  {
    if (verbose)
      printf("..%s, plane %d\n", infilename, i);
    inmatrix = read_a_slice(inputfile, &inputvolume, is3d, frame, i,
			    gate, data, bed);
    inimages[i] = NCread_mat_float(inmatrix, 1.0);
  }


  /* Check if the output file exists, if not, create it. If it does exist:
   * quit! *************************************************** */
  memcpy(&proto, inputfile->mhptr, sizeof(Main_header));
  if (version == 0)
  {
    if (is3d)
      version = 70;
    else
      version = 6;
  }
  proto.sw_version = version;
  if (is3d)
    proto.file_type = PetVolume;
  else
    proto.file_type = PetImage;
  proto.num_planes = n_img;
  
  if (verbose)
    printf("Opening %s\n", outfilename_cor);
  outputfile_cor = matrix_create(outfilename_cor, MAT_OPEN_EXISTING, &proto);
  if (!outputfile_cor)
  {
    printf("Error creating or opening image file %s\n", outfilename_cor);
    exit(0);
  }
  if (verbose)
    printf("Opening %s\n", outfilename_sag);
  outputfile_sag = matrix_create(outfilename_sag, MAT_OPEN_EXISTING, &proto);
  if (!outputfile_sag)
  {
    printf("Error creating or opening image file %s\n", outfilename_sag);
    exit(0);
  }

  /* Make the matrixdata structure. ****************************** */
  if (version == 70)
    matnum_out = mat_numcod(frame, 1, gate, data, bed);
  else
    matnum_out = mat_numcod(frame, 1, gate, data, bed);
  
  outmatrix = (MatrixData *) calloc(1, sizeof(MatrixData));
    
  memcpy(outmatrix, inmatrix, sizeof(MatrixData)); 
  output_sub = (Image_subheader *) calloc(1, sizeof(Image_subheader));
  memcpy(output_sub, input_sub, sizeof(Image_subheader));
  if (version == 70)
    outmatrix->mat_type = PetVolume;
  else
    outmatrix->mat_type = PetImage;
  scalefactor = output_sub->scale_factor;

  outmatrix->matfile = outputfile_cor;
  outmatrix->shptr = (caddr_t) output_sub;
  outmatrix->data_ptr = (caddr_t) NULL;
  outmatrix->xdim = ncols;
  outmatrix->ydim = nplanes;
  if (version < 70)
    outmatrix->zdim = 1;
  else
    outmatrix->zdim = nplanes;
  outmatrix->data_size = ncols*nplanes*2;  

  ncols = ncols;
  workimg_cor = (float *) calloc(ncols * nplanes, sizeof(float));
  workimg_sag = (float *) calloc(ncols * nplanes, sizeof(float));

  imagemin = 1e9;
  imagemax = 0;

  for (i = 1; i <= n_img; i++)
  {
    for (j = 1; j <= nplanes; j++)
    {
      for (col = 0; col < ncols; col++)
      {
	*(workimg_cor + ncols * (j - 1) + col) = 0;
	*(workimg_sag + ncols * (j - 1) + col) = 0;
        if ((col > 0) && (col < (ncols -1)))
        {
	for (k = 1; k <= sum_cor; k++)
	{
/*	  *(workimg_cor + ncols * (j - 1) + col) += *(inimages[j] + (start_cor-1)*ncols + (i-1)*sum_cor*ncols + (k - 1) * ncols + col) * *(inimages[j] + (start_cor-1)*ncols + (i-1)*sum_cor*ncols + (k - 1) * ncols + col); */
	  *(workimg_cor + ncols * (j - 1) + col) += *(inimages[j] + (start_cor-1)*ncols + (i-1)*sum_cor*ncols + (k - 1) * ncols + col); 
	}

	for (k = 1; k <= sum_sag; k++)
	{
/*	  *(workimg_sag + ncols * (j - 1) + col) += *(inimages[j] + (col - 1)*ncols + (start_sag-1) + (i-1)*sum_sag + (k-1)) * *(inimages[j] + (col - 1)*ncols + (start_sag-1) + (i-1)*sum_sag + (k-1)) ; */
	  *(workimg_sag + ncols * (j - 1) + col) += *(inimages[j] + (col - 1)*ncols + (start_sag-1) + (i-1)*sum_sag + (k-1)); 
	}
 
	*(workimg_cor + ncols * (j - 1) + col) /= sum_cor;
	*(workimg_sag + ncols * (j - 1) + col) /= sum_sag;
        }
      }
    }

    /* Scale and find min and max cor*************************** */
    switch (output_sub->data_type)
    {
    case SunShort:
    case VAX_Ix2:
      if (shortdata == NULL)
	shortdata = (short *) calloc(ncols * nplanes, sizeof(short));
      scale_image(workimg_cor, ncols, nplanes, &scalefactor, shortdata, &imagemin, &imagemax, version);
      outmatrix->data_min = (short) (imagemin / scalefactor + 0.5);
      outmatrix->data_max = (short) (imagemax / scalefactor + 0.5);
      outmatrix->data_ptr = (caddr_t) shortdata;
      output_sub->scale_factor = scalefactor;
      break;

    case IeeeFloat:
      outmatrix->data_ptr = (caddr_t) workimg_cor;
      break;

    default:
      printf("Datatype %d not supported.\n", output_sub->data_type);
      exit(0);
    }

    /* Write matrix to file cor********************* */
    if (version < 70)
    {
      output_sub->z_dimension = 1;
      output_sub->x_dimension = ncols;
      output_sub->y_dimension = nplanes;
      output_sub->image_min = outmatrix->data_min;
      output_sub->image_max = outmatrix->data_max;
    }
    else
    {
      output_sub->z_dimension = n_img;
      output_sub->x_dimension = ncols;
      output_sub->y_dimension = nplanes;

    }
    if (verbose)
      printf("Writing plane %d to %s\n", i, outfilename_cor);

    if (!NCmatrix_write_plane(outputfile_cor, outmatrix, i, frame, gate,
			      bed))
    {
      printf("Unable to write the plane\n");
    }

    /* Scale and find min and max sag*************************** */
    switch (output_sub->data_type)
    {
    case SunShort:
    case VAX_Ix2:
      if (shortdata == NULL)
	shortdata = (short *) calloc(ncols * nplanes, sizeof(short));
      scale_image(workimg_sag, ncols, nplanes, &scalefactor, shortdata, &imagemin, &imagemax, version);
      outmatrix->data_min = (short) (imagemin / scalefactor + 0.5);
      outmatrix->data_max = (short) (imagemax / scalefactor + 0.5);
      outmatrix->data_ptr = (caddr_t) shortdata;
      output_sub->scale_factor = scalefactor;
      break;

    case IeeeFloat:
      outmatrix->data_ptr = (caddr_t) workimg_sag;
      break;

    default:
      printf("Datatype %d not supported.\n", output_sub->data_type);
      exit(0);
    }

    /* Write matrix to file ********************* */
    if (version < 70)
    {
      output_sub->z_dimension = 1;
      output_sub->x_dimension = ncols;
      output_sub->y_dimension = nplanes;
      output_sub->image_min = outmatrix->data_min;
      output_sub->image_max = outmatrix->data_max;
    }
    else
    {
      output_sub->z_dimension = n_img;
      output_sub->x_dimension = ncols;
      output_sub->y_dimension = nplanes;
    }
    if (verbose)
      printf("Writing plane %d to %s\n", i, outfilename_sag);

    if (!NCmatrix_write_plane(outputfile_sag, outmatrix, i, frame, gate,
			      bed))
    {
      printf("Unable to write the plane\n");
    }

  }
}











