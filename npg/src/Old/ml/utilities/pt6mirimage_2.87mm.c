/* pt6mirimage_2.87mm.c */

/* 1991 August 01 GCJ started changes to port to HP */

/* pt6mirimage.c */

/* 90-03-29 GCJ major modification to concatenate 7 files, */
/* and pad to be read by a record-oriented dinosaur.       */
/* modified 12 Oct 89 Gerald C. Johns */


/* accept a sieved floating-point image from the input file, */
/* and produce an INTEGER*2 fixed-point image for return */
/* to MALLINCKRODT */
/*

The program pt6mirimage accepts seven reconstructed
images and scales and converts the seven images into one
output file in a format ready for display at the
Mallinckrodt Institute of Radiology.

Usage:

pt6mirimage_2.87mm scale image1 image2 image3 image4 image5\
image6 image7 outputfilename


The command line parameter  scale   must be in floating-point
format, such as  1.0 or 2.237, etc.

The input images are in floating point format
(suitably byte swapped if pt6mirimage is run on a SUN)
preceded by a 256 byte header.

The expected size of the input image files is
(256 + ((100 * 100) * 4) = 40256) bytes.

The output file will have seven concatenated images with the data
portion converted into Perkin-Elmer INTEGER*2 format.
Each image will have a 256 byte header, followed by
((100 * 100) * 2) = 20256 bytes of image, followed by
224 bytes of zeros.  Each image will thus consist of 80 records
of 256 bytes each.  ((80 * 256) = 20480)
The total file will be 80 * 7 = 560 records or 143360 bytes.

*/


/* mirimage.c */

#include <stdio.h>
#define TWO_87_MM
#include "pt6include.h"
#include <math.h>

#define SLICES 7
/* this is the number of slices per study */
#define FIRSTINDEX 2
/* FIRSTINDEX is the index in argv[] of the first image filename */
#define PADS 112
/* padding bytes expressed as a short,  224/2 = 112   */
#define INTEGER2MAX 32767

#define TRUE -1
#define FALSE 0

short int2 ();

FILE *fin;
FILE *fout;

int verbose = FALSE;
char header [nbhead];
float in_data [numxyfin];
short out_data [numxyfin + PADS];


main ( argc, argv )
int argc;
char **argv;
{

    long k;
    long i;
    long j;
    int byteno;
    int c;
    long fourbyteint;  /* four bytes as an integer */
    long fsize;
    int readsize;
    long origsize;
    float *inptr;
    float fmax;
    float fmin;
    float scale;
    float smax;
    float smin;
    float sbig;
    float newscale;
    int nscanned;
    float pixel;
    short *outptr;
    long *cvptr;
    short sliceno;  /* slice number extracted from header */
    short slice;    /* slice being processed */

    if ( argc < 10 )
    {
        fprintf(stderr,"Usage:  pt6mirimage scale image1 image2 image3\\\n");
        fprintf(stderr,"\timage4 image5 image6 image7 outputfilename\n");
        exit(0);
    }


    if (( nscanned = sscanf( argv[1], "%f", &scale )) != 1 )
    {
        fprintf(stderr,"The first argument must be a floating point number,\n");
        fprintf(stderr,"such as 1.0 or 2.327 not %s\n",argv[1]);
        exit(-1);
    }

    if ( verbose )
        fprintf(stdout,"The scale factor is %f\n",scale);

    if ( (fout = fopen( argv[9], "w" )) == NULL )
    {
        fprintf(stderr,"Cannot open %s for output\n",argv[9]);
        exit (-1);
    }

    for ( k = FIRSTINDEX, slice = 1; k < SLICES+FIRSTINDEX; k++, slice++ )
    {
        if ( (fin = fopen( argv[k], "r" )) == NULL )
        {
            fprintf(stderr,"Cannot open %s for input\n",argv[k]);
            exit(-1);
        }
        fread(header,sizeof(char),nbhead,fin);

        sliceno = int2 ( header+34 );  /* slice number in header */
        if ( slice != sliceno )
            fprintf(stderr,"Warning: %s header slice %d does not match %s slice %d\n",
                argv[k],sliceno,argv[9],slice);


        /* read data portion of input file */
        if((readsize=fread(in_data,sizeof(float),numxyfin,fin)) != numxyfin)
            printf("pt6mirimage: read size error\n");
        inptr = in_data;
        outptr =  out_data;
        fmax = *inptr;
        fmin = *inptr;
        for ( i = 0; i < numxyfin; i++ )
        {
            pixel = *inptr;
            if ( pixel > fmax )
                fmax = pixel;
            if ( pixel < fmin )
                fmin = pixel;
            *outptr++ = (short) ( *inptr++ * scale );
        }
        for ( i = 0; i < PADS; i++ )  /* pad with dinosaur fodder */
            *outptr++ = (short) 0;
        /* validity checking */

        smax = fabs( fmax * scale );
        smin = fabs( fmin * scale );
        if ( smax > smin )
        {
            sbig = smax;
            newscale = (float) INTEGER2MAX / fabs (fmax);
        }
        else
        {
            sbig = smin;
            newscale = (float) INTEGER2MAX / fabs (fmin);
        }

        if ( sbig > (float) INTEGER2MAX )
        {
            fprintf(stderr,"Scale factor must be less than or equal to %8.4f\n",newscale );
            exit(0);
        }

        if ( verbose )
        {
            fprintf(stdout,"Before scaling,\n");
            fprintf(stdout,"the maximum pixel value is %12.2f,\n",fmax);
            fprintf(stdout,"the minimum pixel value is %12.2f\n",fmin);
        }
        fwrite(header, sizeof(char), nbhead, fout);
        /* write data portion of output file */
        fwrite( out_data, sizeof (short), numxyfin + PADS, fout);
        fclose ( fin );
    }
    fclose ( fout );
}

short int2 ( cp )  /* convert two bytes to INTEGER*2 */
/* assume incoming bytes are in BIG-ENDIAN byte order */
char *cp;
{
    short result;

    result = ((*cp << 8) | (*(cp+1)) );

    return result;
}
