/* pt6mirimage_2.00mm.c */

/* 1991 August 01  GCJ started port to HP */

/* 128 x 128 version */
/* pt6mirimage_128.c */

/* 90-03-29 GCJ major modification to concatenate 7 files, */
/* and pad to be read by a record-oriented dinosaur.       */
/* modified 12 Oct 89 Gerald C. Johns */


/* accept a sieved floating-point image from the input file, */
/* and produce an INTEGER*2 fixed-point image for return */
/* to MALLINCKRODT */
/*

The program pt6mirimage_128 accepts seven reconstructed
images and scales and converts the seven images into one
output file in a format ready for display at the
Mallinckrodt Institute of Radiology.

Usage:

pt6mirimage_128 scale image1 image2 image3 image4 image5\
image6 image7 outputfilename


The command line parameter  scale   must be in floating-point
format, such as  1.0 or 2.237, etc.

The input images are in floating point format
(suitably byte swapped if pt6mirimage_128 is run on a SUN)
preceded by a 256 byte header.

The expected size of the input image files is
(256 + ((128 * 128) * 4) = 65792) bytes.

The output file will have seven concatenated images with the data
portion converted into Perkin-Elmer INTEGER*2 format.
Each image will have a 256 byte header, followed by
((128 * 128) * 2) = 32768 bytes of image, followed by
ZERO bytes of zeros.  Each image will thus consist of 129 records
of 256 bytes each.  ((129 * 256) = 33024)
The total file will be 129 * 7 = 903 records or 231168 bytes.

*/


/* mirimage.c */

#include <stdio.h>
#define TWO_00_MM
#include "pt6include.h"
#include <math.h>

#define SLICES 7
/* this is the number of slices per study */
#define FIRSTINDEX 2
/* FIRSTINDEX is the index in argv[] of the first image filename */
#define PADS 0
/* padding bytes expressed as a short,  0/2 = 0   */
#define INTEGER2MAX 32767

#define TRUE -1
#define FALSE 0

short int2 ();

FILE *fin;
FILE *fout;

int verbose = FALSE;
char inheader [nbhead];
char outheader [nbhead];
float in_data [numxyfin];
short out_data [numxyfin + PADS];

/* Nuclide names */
char o15 [11] = "O15       ";
char n13 [11] = "N13       ";
char c11 [11] = "C11       ";
char ga68 [11] = "GA68      ";
char f18 [11] = "F18       ";
char space10 [11] = "          ";

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
        fprintf(stderr,"Usage:  pt6mirimage_128 scale image1 image2 image3\\\n");
        fprintf(stderr,"\timage4 image5 image6 image7 outputfilename\n");
        exit(0);
    }


    if (( nscanned = sscanf( argv[1], "%f", &scale )) != 1 )
    {
        fprintf(stderr,"The first argument must be a floating point number,\n");
        fprintf(stderr,"such as 1.0 or 2.327 not %s\n",argv[1]);
        exit(0);
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
            exit (-1);
        }
        fread(inheader,sizeof(char),nbhead,fin);

        sliceno = int2 ( inheader+34 );  /* slice number in header */
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
        cvtheader ( inheader, outheader );  /* convert to super PETT header */
        fwrite(outheader, sizeof(char), nbhead, fout);
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


#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <string.h>


cvtheader ( inh, outh )
char *inh;
char *outh;
{
    int i;
    char *cp;
    char *icp;
    short *isp;
    short *osp;
    long *olp;
    long *ilp;
    struct tm *timeof;
    long clock;
    float p6decay;


    icp = inh;

    cp = outh;
    for ( i = 0; i < nbhead; i++ )
        *cp++ = '\0';

    strncpy ( outh, "PETT VI   ", 10 ); /* Device Name */
    isp = (short*) (inh+30);
    osp = (short*) (outh+10);
    *osp = *isp;  /* copy the resolution, high or low */

    strncpy ( outh+12, "    ", 4 ); /* Device Model  HEAD or BODY */

    isp = (short*) (inh+74);
    olp = (long*) (outh+16);
    *olp = *isp;  /* type of file  2000 EM, 2010 FBP */

    strncpy ( outh+20, inh+24, 6 );  /* Date of Scan */

    strncpy ( outh+26, inh+244, 12 );  /* location of device */

    strncpy ( outh+38, space10, 10); /* A8 to A10 field stretch */
    strncpy ( outh+38, inh+16, 8 );  /* Subject ID */

    strncpy ( outh+48, space10, 10); /* A6 to A10 field stretch */
    strncpy ( outh+48, inh+2, 6 );  /* Run Name */

    osp = (short *) (outh + 58);  /* scan time in seconds */
    isp = (short *) (inh + 42);   /* frame time */
    if ( *isp == (short) 0 )
        isp = (short *) (inh + 14);  /* non-dynamic scan time */
    *osp = *isp;

    osp = (short *) (outh + 64);
    *osp = (short) 7;  /* number of slices in file */


    /* convert from PETT VI decay constant for isotope, to */
    /* Super PETT II isotope name.  */

    ilp = (long *) (inh+36);
    pe2ieee ( ilp, &p6decay );
    cp = space10;
    if ( (p6decay >= 5.5e-3) && (p6decay <= 5.7e-3 ) )
        cp = o15;
    else if ( (p6decay >= 1.1e-3) && (p6decay <= 1.2e-3) )
        cp = n13;
    else if ( (p6decay >= 5.5e-4) && (p6decay <= 5.7e-4) )
        cp = c11;
    else if ( (p6decay >= 1.6e-4) && (p6decay <= 1.7e-4) )
        cp = ga68;
    else if ( (p6decay >= 1.0e-4) && (p6decay <= 1.1e-4) )
        cp = f18;
    else
    {
        fprintf(stderr,"ERROR  pt6mirimage_128: Unknown Nuclide decay of %10.6f\n",
            p6decay);
/*
        exit(-1);
*/
    }
    strncpy ( outh+66, cp, 10 ); /* Nuclide */

    strncpy ( outh+76, inh+48, 10 ); /* compound name */

    osp = (short *) (outh+86);
    isp = (short *) (inh+34);
    *osp = *isp;  /* slice number  1-7 */

    olp = (long *) (outh + 100);
    *olp = (long) 0x41100000; /* scale factor for image display */
    /* Perkin-Elmer floating-point 1.0000 */

    olp = (long *) (outh+104);
    ilp = (long *) (inh+100);
    *olp = *ilp;  /* total usable counts, this slice */
/* there is some question about this quantity. */
/* we may want to total the counts from the reconstructed */
/* image. */



    strncpy ( outh+108, inh+80, 12 ); /* reconstruction filter */


    clock = time ((time_t *)0);
    timeof = localtime ( &clock );
    timeof->tm_mon ++;
    sprintf(outh+120,"%02d%02d%02d",timeof->tm_mon,timeof->tm_mday,timeof->tm_year);
    /* date this file was generated */

    olp = (long *) (outh+148);
    ilp = (long *) (inh+100);
    *olp = *ilp;  /* slice total trues + scatter */

    olp = (long *) (outh+152);
    ilp = (long *) (inh+104);
    *olp = *ilp;  /* slice total accidentals */

    cp = outh + 176;
    for ( i = 0; i < 80; i++ )
        *cp++ = ' ';  /* blank out the comment area */



}

pe2ieee ( ilp, fp )
long *ilp;  /* Perkin-Elmer floating point number */
float *fp;  /* IEEE SUN-4 floating point number */
{
    /* this code is lifted from the dgp program petomass */
    int j;
    float   c;
    union   d_type {
        int idata;
        char    cdata[4];
        short   sdata1,sdata2;
    } datain;
    double  value,mantissa,pow();
    int iexp;

    datain.idata = *ilp;
    if (datain.idata != 0) {
        j = datain.idata & 0x00ffffff;
        if ((datain.idata & 0x80000000) != 0)
            j = -j;
        iexp = (datain.idata >> 24) & 0x7f;
        value = (double)j * pow((double)16, (double)(iexp - 0x46));
        c = (float)value;
    }
    else c = 0.;

    *fp = c;

}
