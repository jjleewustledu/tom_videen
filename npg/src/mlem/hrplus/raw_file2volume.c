#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "matrix.h"
#include "matrix_extra.h"
#include "NChrplus.h"

/* ====================================================================== */
float*
  NCread_mat_float(MatrixData *matrix, 
                     float      scale)
  {
  float   *image;
  float   *imgptr;
  int      nrcols, nrrows, i;
  char    *byteptr;
  short   *shortptr;
  int     *longptr;
  float   *floatptr;
  float    globalscale;
  float    meanval;
 
  nrcols = matrix->xdim;
  nrrows = matrix->ydim;

  globalscale = matrix->scale_factor * scale;
 
  switch (matrix->data_type)
    {
    case ByteData:
    /* printf("bytes\n"); */
      image    = (float *) calloc(nrcols*nrrows, sizeof(float));
      imgptr = image;
      byteptr = (char *) matrix->data_ptr;
      for (i = 0; i < nrcols*nrrows; i++)
        *imgptr++ = *byteptr++ * globalscale;
      break;
 
    case VAX_Ix4:
    case SunLong:
    /* printf("long words\n"); */
      image    = (float *) calloc(nrcols*nrrows, sizeof(float));
      imgptr = image;
      longptr = (int *) matrix->data_ptr;
      for (i = 0; i < nrcols*nrrows; i++)
        *imgptr++ = *longptr++ * globalscale;
      break;
 
    case VAX_Rx4:
    case IeeeFloat:
      if (globalscale == 1.0)
        {
        /* printf("unscaled floats\n"); */
        image = (float *) matrix->data_ptr;
        }
      else
        {
        /* printf("scaled floats\n"); */
        image    = (float *) calloc(nrcols*nrrows, sizeof(float));
        imgptr = image;
        floatptr = (float *)  matrix->data_ptr;
        for (i = 0; i < nrcols*nrrows; i++)
          *imgptr++ = *floatptr++ * globalscale;
        }
      break;
    case 6:
    /* printf("bytes\n"); */
      image    = (float *) calloc(nrcols*nrrows, sizeof(float));
      imgptr = image;
      shortptr = (short *) matrix->data_ptr;
      for (i = 0; i < nrcols*nrrows; i++)
        {
        *imgptr++ = (float) *shortptr++ * globalscale;
        }
      break;
 
    default:
      printf("Unexpected datatype, giving up (sorry)\n");
      exit(0);
    }
 
  return(image);
  }





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
    matnum = mat_numcod(frame, 1, gate, data, bed);
    *inputvolume = matrix_read(inputfile, matnum, MAT_SUB_HEADER);
    if (!*inputvolume)
    {
      printf("Unable to read the volume\n");
      exit(0);
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
scale_image_xx(
	    float *img,
	    int ncols,
	    int nrows,
	    float *scalefactor,
	    short *simg)

{
  int             i;
  float          *p_img;
  short          *p_simg;

  p_img = img;

  p_simg = simg;

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
  float          *inimages[10];
  char            input1[150];
  char            outfilename[150];
  int             showhelp = 0;
  int             i;
  int             j;
  int             k;
  int             verbose = 0;
  int             is3d;
  int             ncols = -1;
  int             nrows = -1;
  int             nplanes = -1;
  int             new_planes;
  int             matnum_out;
  float           scalefactor;
  float           x_size;
  float           z_size;
  MatrixFile     *inputfile1 = NULL;
  MatrixData     *inputvolume = NULL;
  MatrixData     *inmatrix;
  MatrixFile     *outputfile = NULL;
  MatrixData     *outmatrix;
  Image_subheader *input_sub1;
  Image_subheader *output_sub;
  Main_header     proto1;

  float          *workimg;
  short          *shortdata = NULL;

  int             start;
  int             end;
  float           start_fl;
  float           end_fl;
  float           value;


  /* Get the arguments ****************** */
  strcpy(outfilename, "");
  strcpy(input1, "");
  i = 1;
  while (i < argc)
  {
    if (strcmp(argv[i], "-o") == 0)
      strcpy(outfilename, argv[i++ + 1]);
    if (strcmp(argv[i], "-i") == 0)
      strcpy(input1, argv[i++ + 1]);
    if (strcmp(argv[i], "-h") == 0)
      showhelp = 1;
    if (strcmp(argv[i], "-v") == 0)
      verbose = 1;
    i++;
  }

  if (showhelp)
  {
    printf("Syntax: program -r raw_file -o output_file -i header_file\n");
    printf(" -x x_resolutie -z z_resolutie [-v]\n");
    printf("inputfile:  inputfile (PetVolume version 7.0\n");
    printf("outputfile: \n");
    printf("-v:         verbose\n");
    printf("-h:         show this help and stop\n");
    exit(0);
  }



  /* Open the input file ******************** */
  is3d = 1;
  inputfile1 = matrix_open(input1, MAT_READ_ONLY, PetVolume);
  if (!input1)
  {
    printf("Unable to open the file %s as an image or a volume\n", input1);
    exit(0);
  }
  inmatrix = (MatrixData *) calloc(1, sizeof(MatrixData));

  /* Read a slice to get the subheader. *********************************** */
  inmatrix = read_a_slice(inputfile1, &inputvolume, is3d, 1, 1,
			  1, 0, 0);
  memcpy(&proto1, inputfile1->mhptr, sizeof(Main_header));

  input_sub1 = (Image_subheader *) inmatrix->shptr;
  ncols = input_sub1->x_dimension;
  nrows = input_sub1->y_dimension;
  x_size = input_sub1->x_pixel_size;
  z_size = input_sub1->z_pixel_size;
  nplanes = input_sub1->z_dimension;
  scalefactor = input_sub1->scale_factor;  



  new_planes = (float) nplanes*z_size/x_size;

  if (verbose)
    printf("Volume size is %d x %d x %d\n", ncols, nrows, new_planes);


  /* Check if the output file exists, if not, create it. If it does exist:
   * quit! *************************************************** */
  memcpy(&proto1, inputfile1->mhptr, sizeof(Main_header));

  proto1.sw_version = 70;
  proto1.file_type = PetVolume;
  proto1.num_planes = new_planes;
  proto1.plane_separation = x_size;

  
  if (verbose)
    printf("Opening %s\n", outfilename);
  outputfile = matrix_create(outfilename, MAT_OPEN_EXISTING, &proto1);
  if (!outputfile)
  {
    printf("Error creating or opening image file %s\n", outfilename);
    exit(0);
  }

  /* Make the matrixdata structure. ****************************** */
  matnum_out = mat_numcod(1, 1, 1, 0, 0);
  
  outmatrix = (MatrixData *) calloc(1, sizeof(MatrixData));
    
  memcpy(outmatrix, inmatrix, sizeof(MatrixData)); 
  output_sub = (Image_subheader *) calloc(1, sizeof(Image_subheader));
  memcpy(output_sub, input_sub1, sizeof(Image_subheader));
  outmatrix->mat_type = PetVolume;


  outmatrix->matfile = outputfile;
  outmatrix->shptr = (caddr_t) output_sub;
  outmatrix->data_ptr = (caddr_t) NULL;
  outmatrix->xdim = ncols;
  outmatrix->ydim = nrows;
  outmatrix->zdim = new_planes;
  outmatrix->data_size = ncols*nrows*2;  

  workimg = (float *) calloc(ncols * nrows, sizeof(float));


  for (i = 1; i <= new_planes; i++)
  {
    start = (int) floor((i-1)*x_size/z_size);
    end   = (float) floor(i*x_size/z_size);
    start_fl = (float) (i-1)*x_size/z_size;
    end_fl   = (float) i*x_size/z_size;

    for (j = 0; j <= (end-start); j++)
    {
      inmatrix = read_a_slice(inputfile1,&inputvolume,is3d,1,start+j+1,1,0,0);
      inimages[j]  = NCread_mat_float(inmatrix, 1.0);

    }
    for (j = 0; j < ncols*nrows; j++)
    {
      value = 0.;
      for (k = start; k <= end; k++)
      {
        if (k == start)
          value += (((float)(k+1)-start_fl)*z_size/x_size) * *(inimages[0]+j);
        if ((k > start) && (k < end))
          value += (z_size/x_size)  * *(inimages[k-start]+j);
        if (k == end)
          value += ((end_fl-(float) k)*z_size/x_size) * *(inimages[k-start]+j);
      }
      *(workimg + j) = value;
    }

    /* Scale and find min and max cor*************************** */
      if (shortdata == NULL)
	shortdata = (short *) calloc(ncols * nrows, sizeof(short));
      scale_image_xx(workimg, ncols, nrows, &scalefactor, shortdata);
      outmatrix->z_size = x_size;
      outmatrix->data_ptr = (caddr_t) shortdata;
      output_sub->z_pixel_size = x_size;


    output_sub->z_dimension = new_planes;
    output_sub->x_dimension = ncols;
    output_sub->y_dimension = nrows;

    if (verbose)
      printf("Writing plane %d to %s\n", i, outfilename);

    if (!NCmatrix_write_plane(outputfile, outmatrix, i, 1, 1, 0))
    {
      printf("Unable to write the plane\n");
    }
  }
}















