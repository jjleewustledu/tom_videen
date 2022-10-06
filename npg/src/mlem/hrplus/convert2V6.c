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
  char            outfilename[150];
  int             showhelp = 0;
  int             frame = 1;
  int             firstplane = 1;
  int             gate = 1;
  int             data = 0;
  int             bed = 0;
  int             i;
  int             col;
  int             row;
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
  MatrixFile     *outputfile = NULL;
  MatrixData     *outmatrix;
  Image_subheader *input_sub;
  Image_subheader *output_sub;
  Main_header     proto;
  float         **inimages;
  int             n_inimgs;
  float          *workimg;
  short          *shortdata = NULL;


  /* Get the arguments ****************** */
  strcpy(infilename, "");
  strcpy(outfilename, "");
  i = 1;
  while (i < argc)
  {
    if (strcmp(argv[i], "-i") == 0)
      strcpy(infilename, argv[i++ + 1]);
    if (strcmp(argv[i], "-o") == 0)
      strcpy(outfilename, argv[i++ + 1]);
    if (strcmp(argv[i], "-h") == 0)
      showhelp = 1;
    if (strcmp(argv[i], "-v") == 0)
      verbose = 1;
    i++;
  }


  showhelp = showhelp || (strcmp(infilename, "") == 0);
  if (showhelp)
  {
    printf("Syntax: program -i inputfile -o outputfile [-v] [-h]\n");
    printf("inputfile:  input file (PetVolume version 7.0\n");
    printf("outputfile: output file V6 format\n");
    printf("-v:         verbose\n");
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

  proto.sw_version = version;
  proto.file_type = PetImage;
  proto.num_planes = nplanes;
  
  if (verbose)
    printf("Opening %s\n", outfilename);
  outputfile = matrix_create(outfilename, MAT_OPEN_EXISTING, &proto);
  if (!outputfile)
  {
    printf("Error creating or opening image file %s\n", outfilename);
    exit(0);
  }


  /* Make the matrixdata structure. ****************************** */

  matnum_out = mat_numcod(frame, 1, gate, data, bed);
  
  outmatrix = (MatrixData *) calloc(1, sizeof(MatrixData));
    
  memcpy(outmatrix, inmatrix, sizeof(MatrixData)); 
  output_sub = (Image_subheader *) calloc(1, sizeof(Image_subheader));
  memcpy(output_sub, input_sub, sizeof(Image_subheader));
  outmatrix->mat_type = PetImage;
  scalefactor = output_sub->scale_factor;

  outmatrix->matfile = outputfile;
  outmatrix->shptr = (caddr_t) output_sub;
  outmatrix->data_ptr = (caddr_t) NULL;
  outmatrix->xdim = ncols;
  outmatrix->ydim = nrows;
  outmatrix->zdim = 1;
  outmatrix->data_size = ncols*nrows*2;  

  workimg = (float *) calloc(ncols * nrows, sizeof(float));

  imagemin = 1e9;
  imagemax = 0;

  for (i = 1; i <= nplanes; i++)
  {
    for (row = 0; row < nrows; row++)
    {
      for (col = 0; col < ncols; col++)
      {
	*(workimg + ncols * row + col) = *(inimages[i] + ncols * row + col);
      }
    }

    /* Scale and find min and max cor*************************** */
    if (shortdata == NULL)
      shortdata = (short *) calloc(ncols * nrows, sizeof(short));
    scale_image(workimg, ncols, nrows, &scalefactor, shortdata, &imagemin, &imagemax, version);
    outmatrix->data_min = (short) (imagemin / scalefactor + 0.5);
    outmatrix->data_max = (short) (imagemax / scalefactor + 0.5);
    outmatrix->data_ptr = (caddr_t) shortdata;

    output_sub->scale_factor = scalefactor;
    output_sub->z_dimension = 1;
    output_sub->x_dimension = ncols;
    output_sub->y_dimension = nrows;
    output_sub->image_min = outmatrix->data_min;
    output_sub->image_max = outmatrix->data_max;

    if (verbose)
      printf("Writing plane %d to %s\n", i, outfilename);

    if (!NCmatrix_write_plane(outputfile, outmatrix, i, frame, gate,
			      bed))
    {
      printf("Unable to write the plane\n");
    }
  }
}













