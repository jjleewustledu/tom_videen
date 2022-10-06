/* module NCgauss */

/*
 general information: 
 */

/******************************************************************************

* topic History
  Author(s)     : Jan Baetens, based on Frederik Maes' C++ program
  Date          : October 1997
  Modifications :
* endtopic History

******************************************************************************/

#define __NCgauss_module 1   /*NOINC*/


/*enddeclars*/
#include <math.h>


/*-------------------------------------------------------------------------*/
/*local*/
void generate_filter(
                     float sigma,    /* The sigma of the filter
                                        in 1 dimension (in pixels ) */
                     float n_sigmas, /* Request a number of sigma's wide */
                     int   *size,    /* Returned size of the filter */
                     float **filter  /* Filter elements, created here */
                     )
  {

  float     exp_scale, scale  ;
  int       i ;

  /* Determine filter size ( one side of the filter, since it is 
   * symmetrical, this is sufficient )
   */
  *size = ( int )( n_sigmas * sigma + 1 + 0.5 ) ;
  /* Allocate memory for filter */
  *filter = ( float * )calloc( *size, sizeof( float ) ) ;

  /* Fill in */
  /* Central element */
  ( *filter )[0] = 1.0;

  /* Pathological case */
  /* if size == 0: identity filter 1 */
  if (*size == 0) return;

  /* Create filter */
  exp_scale = 1.0 / ( 2.0 * sigma * sigma ) ; 
  for ( i = 1; i <= *size; i++ )
    {
    ( *filter )[i] = exp( -( float )( i * i ) * exp_scale ) ;
    }
  scale = 1.0 / ( sigma * sqrt( 2 * M_PI ) ) ;

  /* Rescale filter */
  for ( i = 0; i <= *size; i++ )
    {                
    ( *filter )[i] *= scale;
    }

    
  return;
  }

/*local*/
void filter_symmetric(
                      float *in,
                      float *out,
                      int   inc_el,  /* Pointer increment for 1 element */
                      int   inc_line, /* Pointer increment for 1 "row"   */
                      int   n_el,     /* Number of elements/line */
                      int   n_line,   /* Number of "lines"
                                        (rows, cols or planes) */
                      float *filter,
                      int   size,
                      int   copy_border, 
                      int   mirror_border
                      )
  {
  float *inptr1, *inptr2, *inptr3, *inptr4, *outptr1, *outptr2;
  float *fltrptr, *bufferline ;
  float leftval, rightval ;
  int row, el, i, borderwidth ;
  int left, right ;

  /* Allocate a buffer the size of a line. This will contain a copy of
   * the line that has to be convolved to prevent having to allocate
   * an image the size of the input image
   */
  bufferline = ( float *)calloc( n_el, sizeof( float ) ) ;
  /*size = ( size < n_el ) ? size:n_el ;*/
  
  inptr1 = in;
  outptr1 = out;
  for (row = 0; row < n_line; row++)      /* all rows */
    {        
    /* Copy the input line to the bufferline */
    for (el = 0; el < n_el;  *( bufferline + el ) = *( inptr1 + el * inc_el ),
         el++) ;
    
    /* Do the convolution on the "inside" of the image */
    if ( n_el > ( 2 * size + 1 ) )
      {
      outptr2 = outptr1 + size * inc_el;
      inptr2 = bufferline + size ;
      for (el = size; el < n_el-size;  el++)      /* all elements in row */
        {
        fltrptr = filter;
        
        *outptr2 = *inptr2 * (*fltrptr);  /* central point */
        
        inptr3 = inptr2;
        inptr4 = inptr2;
        for (i=1; i<=size; i++)
          {
          fltrptr++;
          inptr3 -= 1;
          inptr4 += 1;
          *outptr2 += (*inptr3 + *inptr4) * (*fltrptr);
          }
        
        inptr2 += 1;
        outptr2 += inc_el;
        }
      }
    if ( ( !copy_border ) && mirror_border )
      {
      /* mirroring of the data at the border */
      outptr2 = outptr1;
      inptr2 = bufferline ;
      for (el = 0; el < size; el++)
        {
        fltrptr = filter;
        
        *outptr2 = *inptr2 * (*fltrptr);  /* central point */
        
        inptr3 = inptr2;
        inptr4 = inptr2;
        for (i=1; i<=el; i++)
          {
          fltrptr++;
          inptr3 -= 1;
          inptr4 += 1;
          *outptr2 += (*inptr3 + *inptr4) * (*fltrptr);
          }
        for (i=el+1; i<=size; i++)
          {
          fltrptr++;
          inptr4 += 1;
          *outptr2 += (*inptr3 + *inptr4) * (*fltrptr);
          inptr3 += 1;
          }
        
        inptr2 += 1;
        outptr2 += inc_el;
        }
      
      inptr2 = bufferline + ( n_el - size ) ;
      outptr2 = outptr1 + (n_el - size) * inc_el;
      
      for (el = n_el-size; el < n_el; el++)
        {
        fltrptr = filter;
        
        *outptr2 = *inptr2 * (*fltrptr);  /* central point */
        
        inptr3 = inptr2;
        inptr4 = inptr2;
        for (i=1; i< n_el-el; i++)
          {
          fltrptr++;
          inptr3 -= 1;
          inptr4 += 1;          
          *outptr2 += (*inptr3 + *inptr4) * (*fltrptr);
          }
        for (i=n_el-el; i<=size; i++)
          {
          fltrptr++;
          inptr3 -= 1;
          *outptr2 += (*inptr3 + *inptr4) * (*fltrptr);
          inptr4 -= 1;
          }
        
        inptr2 += 1;
        outptr2 += inc_el;
        }
      
      inptr1 += inc_line;
      outptr1 += inc_line;
      }
    else
    if ( ( !copy_border ) && ( !mirror_border ) )
      {
      /* no mirroring of the data at the border */
      /* data outside the image are set to zero */
      
      outptr2 = outptr1;
      inptr2 = bufferline;
        
      borderwidth = ( size < n_el ) ? size : n_el ;
      /*printf( "borderwidth = %d\n", borderwidth ) ;*/
      for ( el = 0; el < borderwidth; el++ )
        {
        fltrptr = filter;

        *outptr2 = *inptr2 * (*fltrptr);  /* central point */
         
        inptr3 = inptr2;
        inptr4 = inptr2;
        left = right = 0 ;
        for ( i = 1; i <= size; i++ )
          {
          fltrptr++ ;
          left++ ;
          right++ ;
          leftval = ( ( el - left ) >= 0 ) ? *( inptr2 - left ): 0.0 ;
          rightval = ( ( el + right ) < n_el ) ? *( inptr2 + right ): 0.0 ;
          /*
             printf( "left = %d, right = %d, leftval =%.2f rightval=%.2f\n",
             left, right, leftval, rightval ) ;
             */
          *outptr2 += ( leftval + rightval ) * ( *fltrptr ) ;
          }
/*
        for (i=1; i<=el; i++)
          {
          fltrptr++;
          inptr3 -= 1;
          inptr4 += 1;
          *outptr2 += (*inptr3 + *inptr4) * (*fltrptr);
          }
        for (i=el+1; i<=borderwidth; i++)
          {
          fltrptr++;
          inptr4 += 1;
          *outptr2 += (*inptr4) * (*fltrptr);
          inptr3 += 1;
          }
*/
        inptr2 += 1;
        outptr2 += inc_el;
        }
      
      /* Do the next step only if the border is smaller than the
       * width of the image (n_el)
       * If the border is larger, these elements have been filtered by the
       * previous step
       */
      if ( borderwidth < n_el ) 
        {
        inptr2 = bufferline + (n_el - size);
        outptr2 = outptr1 + (n_el - size) * inc_el;
        for (el = n_el-borderwidth; el < n_el; el++)
          {
          fltrptr = filter;
          
          *outptr2 = *inptr2 * (*fltrptr);  /* central point */
          
          inptr3 = inptr2;
          inptr4 = inptr2;
          left = right = 0 ;
          for ( i = 1; i <= size; i++ )
            {
            fltrptr++ ;
            left++ ;
            right++ ;
            leftval = ( ( el - left ) >= 0 ) ? *( inptr2 - left ): 0.0 ;
            rightval = ( ( el + right ) < n_el ) ? *( inptr2 + right ): 0.0 ;
            *outptr2 += ( leftval + rightval ) * ( *fltrptr ) ;
            }
/*
          for (i=1; i< n_el-el; i++)
            {
            fltrptr++;
            inptr3 -= 1;
            inptr4 += 1;          
            *outptr2 += (*inptr3 + *inptr4) * (*fltrptr);
            }
          for (i=n_el-el; i<=size; i++)
            {
            fltrptr++;
            inptr3 -= 1;
            *outptr2 += (*inptr3) * (*fltrptr);
            inptr4 -= 1;
            }
*/
          inptr2 += 1;
          outptr2 += inc_el;
          }
        }
      }

    else
    if ( copy_border )
      {
      inptr1 = in;
      outptr1 = out;
      inptr2 = bufferline;
      outptr2 = outptr1;

      for (el = 0; el < size; el++)
        {
        *outptr2 = *inptr2;
        inptr2 += 1;
        outptr2 += 1;
        }
      
      inptr2 = bufferline + (n_el - size);
      outptr2 = outptr1 + (n_el - size) * inc_el;
      for (el = n_el-size; el < n_el; el++)
        {
        *outptr2 = *inptr2;
        inptr2 += 1;
        outptr2 += 1;
        }
      }
    
    inptr1 += inc_line;
    outptr1 += inc_line;
    }
    
  free( bufferline ) ;

  return;
  }

/*global*/
void NCgauss( 
             /* Image description */
             float *in,        /* Input image */
             float *out,       /* Output image (has to exist already) */
             int   nrrows,     /* Number of rows */
             int   nrcols,     /* Number of columns */
             int   nrplanes,   /* Number of planes */
             float pixelsize,  /* in mm/pixel */
             
             /* Filter description */
             float fwhm_mm,      /* FWHM in mm */
             int   n_sigmas,     /* Nr of sigma's (in 1 direction) */
             
             /* Filter application description */
             int   copy_border,  /* Do nothing to the border but copy the 
                                    original border */
             int   mirror_border,/* if 1, mirror border before filtering,
                                    if 0, assume 0 outside the image */
             int   axis          /* Dimension to which to apply the filter
                                    0 = X; 1 = Y; 2 = Z */
             )
   
  {
  float      sigma_p, sigma_mm ;
  int        filtersize ;
  float      *filter ;
  int        n_el, n_lines, inc_el, inc_line ;
  int        z ;

  /* Calculate the sigma in pixels */
  sigma_mm = fwhm_mm / sqrt( 8.0 * log( 2.0 ) ) ;
  sigma_p  = sigma_mm / pixelsize ;

  /* Generate the filter */
  generate_filter( sigma_p, n_sigmas, &filtersize, &filter ) ;

  /* Calculate the increments to the memory pointers */
  switch ( axis )
    {
    case 0: /* Filter along the X-axis */
      inc_el  = 1 ;
      inc_line = nrcols ;
      n_el = nrcols ;
      n_lines = nrrows ;
      for ( z = 0; z < nrplanes; z++ )
        filter_symmetric( in + nrrows * nrcols * z,
                         out + nrrows * nrcols * z,
                         inc_el, inc_line, n_el, n_lines, 
                         filter, filtersize, copy_border, mirror_border ) ;
      break ;
    case 1: /* Filter along the Y-axis */
      inc_el  = nrcols ;
      inc_line = 1 ;
      n_el = nrrows ;
      n_lines = nrcols ;
      for ( z = 0; z < nrplanes; z++ )
        filter_symmetric( in + nrrows * nrcols * z,
                         out + nrrows * nrcols * z,
                         inc_el, inc_line, n_el, n_lines, 
                         filter, filtersize, copy_border, mirror_border ) ;
      
      break ;
    case 2: /* Filter along the Z-axis */
      inc_el  = nrcols * nrrows ;
      inc_line = 1 ;
      n_el = nrplanes ;
      n_lines = nrrows * nrcols ;
      filter_symmetric( in, out, inc_el, inc_line, n_el, n_lines, 
                       filter, filtersize, copy_border, mirror_border ) ;
      break ;
      }

  /* Free unnecessary memory */
  free( filter )  ;

  /* Return */
  return ;
  }
