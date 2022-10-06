/* module NCproj_defs */

/*
 general information: 
 Type definitions.
 */

/******************************************************************************

* topic History
  Author(s)     :
  Date          :
  Modifications :
* endtopic History

******************************************************************************/
#define __NCproj_defs_module 1 /*NOINC*/
#define  SMALL     0.1    /* Too small value causes instability!
                             This relatively high value requires that the
                             initial image produces sinogram with sufficiently
                             high counts (count >> SMALL)*/
#define  VERYSMALL 1e-15


typedef struct ProjCoeff_struct
  {
  int     nrproj;    /* total number of angles */
  float   angle;     /* angle between projection lines and vertical axis */
  float   scale;     /* value between 0 and 1, downscaling of pixel size */
  int     cosscale;  /* 1 = scale is cos(angle), 0 = scale is sin(angle) */
  int     N;         /* number of pixels in the original projection */
  int     bigN;      /* number of pixels in the scaled projection */
                     /* bigN > N   */

  /* The arrays recindexX and recweightX together describe the interpolation
     coefficients for the central projection line. The weights for the other
     projection lines are the same. The indices of the other projection lines
     are obtained by adding an offset. Since shifted lines may partly fall
     out of the image, the coordinates of the first and last points that
     still belong to the image are stored as well */

  /* indices and weights of a projection line: N elements,
     the indices point to image coordinates. */
  int   *recindex1;  /* index to the first point for interpolation */
  int   *recindex2;  /*             second point for interpolation */
  float *recweight1; /* interpolation weight for first point */
  float *recweight2; /*                          second point */

  float *bigsino;    /* sinogram row with bigN pixels, provided as working
                          memory */

  /* First and last indices. It is possible that e.g. the leftmost point is
     in the image, while its right neighbor, needed for interpolation is not.
     Pixels outside the image are ignored (= assuming them equal to zero).
  */
  /* indices and offset for each projection line: there are bigN projection
     lines, so these arrays have bigN elements. They point to coordinates
     in recindex1 etc. */
  int *linefirst1; /* index to first interpolation point within image */
  int *linefirst2; /* index to the first other interpolation point */
  int *linelast1;  /* index to last interpolation point 1 */
  int *linelast2;  /* index to last interpolation point 2 */

  int *lineoffset; /* offset to be added to recindex1 and recindex2 */

  /* Interpolation coefficients and indices required to compute a sinogram
     line of regular size N, from a big sinogram line (with smaller pixels)
     of size bigN:  convert bigN ==> N. Thus: N elements in the arrays */
  int   *bigindex1;  /* index to left interpolation point */
  int   *bigindex2;  /* index to right interpolation point */
  float *bigweight1; /* weight for left interpolation point */
  float *bigweight2; /* weight for right interpolation point */

  /* Interpolation coefficients and indices required to compute a sinogram
     line of size bigN, from a regular sinogram line (size N).
     Convert N ==> bigN. Consequently: bigN elements in the arrays. */
  int   *smallindex1;  /* index to left interpolation point */
  int   *smallindex2;  /* index to right interpolation point */
  float *smallweight1; /* weight for left interpolation point */
  float *smallweight2; /* weight for right interpolation point */

  /* Working buffers: one the size of a sinogram, the other the size
   * of a reconstruction image. In addition, we also allocate an extra
   * sinogram which will be used to store the calculated (predicted)
   * sinogram, and an extra image to store the backprojection of the
   * attenuation coefficients. */
  float  *worksino;
  float  *calcsino;
  float  *workrecon;
  float  *backatten;
  int     backatten_ok;
  } s_ProjCoeff;

typedef s_ProjCoeff  *p_ProjCoeff;


typedef struct AbsPrior_struct
  {
  int     number;     /* Number of maxima in the absolute prior function */
  float   weight;     /* Weight of the prior */
  float  *mean;       /* The mean values of the modes (local Gaussian shape */
  float  *std;        /* The corresponding standard deviation, determining
                         the strength of the mode */
  float  *width;      /* The width of the mode (the range). This is a relative
                         number, determines where one mode ends and the next
                         one starts */
  int     allocated;  /* When set, the images gprior and g2prior are
                         allocated */
  float  *gprior;     /* Image for the gradient of the prior */
  float  *g2prior;    /* Image for the second derivative (step size) */
  } s_AbsPrior;

typedef s_AbsPrior *p_AbsPrior;


typedef struct MarkovPrior_struct
  {
  char     functiontype;   /* 'q' = quadratic, 'h' = Huber, 'g' = Geman */
  int      neighborhood;   /* 4 or 8 neighborhood */
  float    delta;
  float    weight;         /* Overall weight (hyper-parameters) */
  int      allocated;      /* When set, the image gprior has been allocated */
  float   *gprior;         /* image for the gradient of the prior */
  float    g2prior;        /* Value for estimate of second derivative */
  } s_MarkovPrior;

typedef s_MarkovPrior  *p_MarkovPrior;

/*enddeclars*/
