/* module NCdcoffset */

/*
 general information: 
 This module produces a 2D image and corresponding sinogram. The image
 consists of a 2D disk, filled with a constant value. The sinogram is the
 corresponding projection, as it would be measured by the PET (taking
 into account a sensitivity matrix).
 With this procedure, one can add a constant value to a sinogram, reconstruct
 it and subtract the corresponding image from the reconstruction afterwards.
 The result is an MLEM-like reconstruction which allows negative values.

 Advantage: for inconsistent sinograms (e.g. not corrected for
   attenuation), there is no really good reconstruction. FBP produces
   negative values, which seems meaningless. However, what FBP does is
   using "anti-photons" to explain the inconsistency in the
   sinogram. These negative values actually contain interesting
   information about the local variations in tracer distribution.
   MLEM cannot produce negative values, and as a result, it sometimes sets
   large regions to zero. All local tracer information in these regions
   is lost.

 Disadvantage:
   1. The algorithm is no longer an MLEM algorithm. However, if there
      is no attenuation correction, the acquisition model was wrong anyway.
   2. The background is not as nicely set to zero as in straightforward
      MLEM.
   3. Negative values appear. The provide information, but many people
      don't like them.

 Use: Only intended to be used for highly inconsistent sinograms. When
    proper normalization and attenuation are available, it is a VERY BAD
    idea to add DC-offsets!
*/

/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : may 1998
  Modifications :
* endtopic History

******************************************************************************/
#define __NCdcoffset_module 1
#include "stdio.h"  /*NOINC*/
#include "NCproj_bproj.h"  /*NOINC*/
#include "math.h"  /*NOINC*/

/*enddeclars*/

/*-------------------------------------------------------------------------*/
/*global*/

float* NCdcoffset(           /* returns the DC-image, in case caller wants
                              to subtract it after reconstructionc */
  float         *sinogram,   /* Sinogram to be addapted accordingly */
  p_ProjCoeff    projcoeff,  /* the interpolation coeff. (NCprojcoeffx)*/
  float         *sensit,     /* The sensitivity matrix or NULL */
  float          radius,     /* The radius of the disk set to value */
  float          value       /* The value used to fill the disk in image */
  )

{
float    *image;
float    *sinotmp;
float     center;
float     radkwad;
float    *p_img;
float    *p_sensit;
float    *p_sino;
float    *p_final;
int       col;
int       row;
int       projnum;
int       pixnum;

float     testval;

/* Allocate image and sinogram
******************************/
sinotmp = (float *) calloc(projcoeff->N * projcoeff->nrproj, sizeof(float));
image   = (float *) calloc(projcoeff->N * projcoeff->N,      sizeof(float));

/* Zero the sinogram
*******************/
p_sino = sinotmp;
for (pixnum = 0; pixnum < (projcoeff->N * projcoeff->nrproj); pixnum++)
  *p_sino++ = 0.0;

/* Fill in the disk with the constant value
*******************************************/
center  = (projcoeff->N - 1.0) / 2.0;
radkwad = radius * radius;
p_img   = image;
for (col = 0; col < projcoeff->N; col++)
  for (row = 0; row < projcoeff->N ; row++)
    {
       if (((col - center)*(col - center) + (row - center)*(row-center))
          < radkwad)
        *p_img++ = value;
      else
        *p_img++ = 0.0;
    }

/* Project this image into the sinogram
***************************************/
for (projnum = 0; projnum < projcoeff->nrproj; projnum++)
  {
  NCproj_forward(image, sinotmp, projnum, projcoeff);
  }

/* If sensit is available, multiply the projection with the
 * sensitivity matrix. Otherwise: simply copy.
 **************************************************/
p_sino   = sinotmp;
p_final  = sinogram;
if (sensit != NULL)
  {
  p_sensit = sensit;
  for (pixnum = 0; pixnum < (projcoeff->N * projcoeff->nrproj); pixnum++)
    *p_final++ += *p_sino++ * *p_sensit++;
  }
else
  {
  for (pixnum = 0; pixnum < (projcoeff->N * projcoeff->nrproj); pixnum++)
    *p_final++ += *p_sino++;
  
  }

free(sinotmp);
return(image);
}


