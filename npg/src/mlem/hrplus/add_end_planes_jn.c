/******************************************************************************

* topic History
  Author(s)     : S Vleugels
  Date          : Jan. 1999
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
read_the_slice(
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
  int             test;
 
  p_img = img;
 
  p_simg = simg;
 
  for (i = 0; i < ncols * nrows; i++)
  {
    *(p_simg + i) = (short) (*(p_img + i) / *scalefactor);
    test = (int) (*(p_img + i) / *scalefactor);
    if (test > 32767) 
      printf("%d\n", test);

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
  int             bed_to_read;
  int             plane = 1;
  int             i;
  int             j;
  int             n_img;
  int             version = 0;
  int             verbose = 0;
  int             is3d;
  int             ncols = -1;
  int             nrows = -1;
  int             nplanes = -1;
  int             nbeds = -1;
  int             overlap = 5;
  int             new_planes;
  int             last_plane;
  float           x_size;
  float           y_size;
  float           z_size;
  float           x_dim;
  float           y_dim;
  float           z_dim;
  float           scalefactor;
  float           imagemin;
  float           imagemax;
  int             imgmin;
  int             imgmax;

  MatrixFile     *inputfile = NULL;
  MatrixFile     *outputfile = NULL;
  MatrixData     *inputvolume = NULL;
  MatrixData     *inmatrix;
  MatrixData     *outmatrix;
  Image_subheader *input_sub;
  Image_subheader *output_sub;
  Main_header     proto1;
  float         **inimages;
  float           *workimg;
  float          *tmpoverlap;
  short          *shortdata = NULL;

  int             reversebed = 2; /* 0 = no reverse, 1 = reverse, 
                                     2 = als vroeger */


  /* Get the arguments ****************** */
  strcpy(infilename, "");
  strcpy(outfilename, "");

  i = 1;
  while (i < argc)
  {
    if (strcmp(argv[i], "-i") == 0)
      strcpy(infilename, argv[i++ + 1]);
    if (strcmp(argv[i], "-rb") == 0)
      sscanf(argv[i++ + 1], "%d", &reversebed);
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
    printf("Syntax: program -i inputfile -o output_file [-rb #][-v] [-h]\n");

    printf("inputfile:  input file (PetVolume version 7.0\n");
    printf("outputfile: output file raw data\n");
    printf("-v:         verbose\n");
    printf("-h:         show this help and stop\n");
    printf("-rb:        0 = no reverse, 1 = reverse, 2 = als vroeger (default)\n");
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
  inmatrix = read_the_slice(inputfile, &inputvolume, is3d, frame, firstplane,
			  gate, data, bed);
  input_sub = (Image_subheader *) inmatrix->shptr;
  ncols = input_sub->x_dimension;
  nrows = input_sub->y_dimension;
  nplanes = input_sub->z_dimension;
  x_size = input_sub->x_pixel_size;
  z_size = input_sub->z_pixel_size;
  nbeds = inputfile->mhptr->num_bed_pos;
  new_planes = nplanes + nbeds*(nplanes - overlap); 

  inimages = (float **) calloc(nplanes, sizeof(float *));

  imagemax = 0.;
  imagemin = 0.;
  scalefactor = input_sub->scale_factor;
  /* find global max, min) */
  for (bed = 0; bed <= nbeds; bed++)
  {
    for (i = 1; i <= nplanes; i++)
    {
      inmatrix = read_the_slice(inputfile, &inputvolume, is3d, frame, i,
			    gate, data, bed);
      inimages[0] = NCread_mat_float(inmatrix, 1.0);
      for (j = 0; j < ncols*nrows; j++)
      {
        if ( *(inimages[0] + j) > imagemax)
           imagemax = *(inimages[0] + j);
        if (*(inimages[0] + j) < imagemin)
           imagemin = *(inimages[0] + j);
      }
    }
  }
  
  if (imagemax > 0.)
    scalefactor = imagemax/32767;
  else
    scalefactor = 1.0;


  if (verbose)
    printf("Volume size is %d x %d x %d\n",
	   ncols, nrows, nplanes);

  /* Check if the output file exists, if not, create it. If it does exist:
   * quit! *************************************************** */
  memcpy(&proto1, inputfile->mhptr, sizeof(Main_header));
 
  proto1.sw_version = 70;
  proto1.file_type = PetVolume;
  proto1.num_planes = new_planes;
  proto1.num_bed_pos = 0;
  
  if (verbose)
    printf("Opening %s\n", outfilename);
  outputfile = matrix_create(outfilename, MAT_OPEN_EXISTING, &proto1);
  if (!outputfile)
  {
    printf("Error creating or opening image file %s\n", outfilename);
    exit(0);
  }
 
  /* Make the matrixdata structure. ****************************** */
  
  outmatrix = (MatrixData *) calloc(1, sizeof(MatrixData));
    
  memcpy(outmatrix, inmatrix, sizeof(MatrixData)); 
  output_sub = (Image_subheader *) calloc(1, sizeof(Image_subheader));
  memcpy(output_sub, input_sub, sizeof(Image_subheader));
  outmatrix->mat_type = PetVolume;
 
 
  outmatrix->matfile = outputfile;
  outmatrix->shptr = (caddr_t) output_sub;
  outmatrix->data_ptr = (caddr_t) NULL;
  outmatrix->xdim = ncols;
  outmatrix->ydim = nrows;
  outmatrix->zdim = new_planes;
  outmatrix->data_size = ncols*nrows*2;  
  outmatrix->data_min = 32767*imagemin/imagemax;
  outmatrix->data_max = 32767;
  output_sub->scale_factor = scalefactor;
  output_sub->z_dimension = new_planes;
  output_sub->x_dimension = ncols;
  output_sub->y_dimension = nrows;
  output_sub->image_max = 32767;
  output_sub->image_min = (short) 32767*imagemin/imagemax;




  /* Read the first images. Images that don't exist are set to NULL ************************************** */
  tmpoverlap = (float *) calloc(nrows*ncols*5 , sizeof(float *));
  workimg = (float *) calloc(ncols * nrows, sizeof(float));

  if (verbose)
    printf("Reading first images...\n");
  for (bed = 0; bed <= nbeds; bed++)
  {
    for (i = 1; i <= nplanes; i++)
    {
      if (verbose)
        printf("..%s, plane %d\n", infilename, i);
      if (reversebed == 2)
      {
        bed_to_read = nbeds - bed;
        if (((inputfile->mhptr->patient_orientation == FeetFirstProne) ||
             (inputfile->mhptr->patient_orientation == FeetFirstSupine) ||
             (inputfile->mhptr->patient_orientation == FeetFirstRight) ||
             (inputfile->mhptr->patient_orientation == FeetFirstLeft)) &&
            (proto1.bed_offset[0] < 0.))
          bed_to_read = bed;
        if (((inputfile->mhptr->patient_orientation == HeadFirstProne) ||
             (inputfile->mhptr->patient_orientation == HeadFirstSupine) ||
             (inputfile->mhptr->patient_orientation == HeadFirstRight) ||
             (inputfile->mhptr->patient_orientation == HeadFirstLeft)) &&
            (proto1.bed_offset[0] > 0.))
          bed_to_read = bed;
      }
      else
        if (reversebed == 0) bed_to_read = bed;
        else                 bed_to_read = nbeds - bed;

      inmatrix = read_the_slice(inputfile, &inputvolume, is3d, frame, i,
			    gate, data, bed_to_read);
      inimages[i-1] = NCread_mat_float(inmatrix, 1.0);
    }
    if (bed > 0)
    {
      for ( i = 0; i < overlap; i++)
      {
        for (j = 0; j < ncols*nrows; j++)
        {
          *(inimages[i] + j) = *(inimages[i] + j)*(i+1)/6 + *(tmpoverlap + i*ncols*nrows + j)*(overlap-i)/6;
        }
      }

    }

    if (bed < nbeds)
    {
      last_plane = nplanes - overlap;

      for (i = 0; i < overlap; i++)
      {
        for (j = 0; j < ncols*nrows; j++)
        {
          *(tmpoverlap + i*ncols*nrows + j) = *(inimages[i+last_plane] + j);
        }
      }
    }
    else
    {
      last_plane  = nplanes;
    }
    for (i = 1; i <= last_plane; i++)
    {
      for (j = 0; j < ncols*nrows; j++)
      {
        *(workimg + j) = *(inimages[i-1] + j);
      }
      switch (output_sub->data_type)
      {
      case SunShort:
      case VAX_Ix2:
        if (shortdata == NULL)
          shortdata = (short *) calloc(ncols * nrows, sizeof(short));
        scale_image_xx(workimg, ncols, nrows, &scalefactor, shortdata);
/*        outmatrix->data_min = (short) 32760*imagemin/imagemax;
        outmatrix->data_max = 32760;    */
        outmatrix->data_ptr = (caddr_t) shortdata;
/*        output_sub->scale_factor = scalefactor;*/
        break;
 
      case IeeeFloat:
        outmatrix->data_ptr = (caddr_t) workimg;
        break;
 
      default:
        printf("Datatype %d not supported.\n", output_sub->data_type);
        exit(0);
      }
 
/*      output_sub->z_dimension = new_planes;
      output_sub->x_dimension = ncols;
      output_sub->y_dimension = nrows;
      output_sub->image_max = 32760;
      output_sub->image_min = (short) 32760*imagemin/imagemax; */

      if (verbose) 
        printf("Writing plane %d to %s\n", plane, outfilename);
 
      if (!NCmatrix_write_plane(outputfile, outmatrix, plane, 1, 1,
                              0))
      {
        printf("Unable to write the plane\n");
      }
      plane++;
    }
  }
}












