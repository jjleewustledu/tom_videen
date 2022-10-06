/* pt6intfloat.c */

/* modified 12 Oct 89 Gerald C. Johns */


/* this program reads PETT VI distance-angle data and */
/* swaps the bytes in the data portion to get transputer */
/* byte order.  The header is passed through untouched.  */
/* the data portion is initially INTEGER*4, and will be output */
/* as IEEE Floating Point.  */

/* byteswap.c */

#include <stdio.h>
#include "pt6include.h"

FILE *fin;
FILE *fout;

char header [nbhead];
long in_data [numdaorig];
float out_data [numdaorig];
float convert;  /* temp used while converting to float */


main ( argc, argv )
int argc;
char **argv;
{

    long i;
    long j;
    int c;
    long fourbyteint;  /* four bytes as an integer */
    long fsize;
    int readsize;
    long origsize;
    long *inptr;
    char *outptr;
    float *foutptr;
    long *cvptr;
    int slice;

    if ( argc < 4 )
    {
        fprintf(stderr,"Usage:  pt6intfloat fileinname fileoutname slice\n");
        exit(0);
    }

    slice = atoi ( argv[3] );
    if ( (slice < 1) || (slice > 7 ) )
    {
        fprintf(stderr,"ERROR.  Cannot process slice %d\n",slice);
        exit(0);
    }
    /*    printf("Extracting slice %d\n",slice); */
    if ( (fin = fopen( argv[1], "r" )) == NULL )
    {
        fprintf(stderr,"ERROR.  Cannot open %s for input\n",argv[1]);
    }

    if ( (fout = fopen( argv[2], "w" )) == NULL )
    {
        fprintf(stderr,"ERROR.  Cannot open %s for output\n",argv[2]);
    }

    fseek ( fin, (long) (slice-1) * slicerecord, 0 );
    fread(header,sizeof(char),nbhead,fin);
    /* read data portion of input file */
    if((readsize=fread(in_data,sizeof(long),numdaorig,fin)) != numdaorig)
        fprintf(stderr,"ERROR.  pt6intfloat: read size error\n");
    inptr = in_data;
    outptr = (char*) out_data;
    foutptr = out_data;
    cvptr = (long*) &convert;  /* used for type conversion */
    for ( i = 0; i < numdaorig; i++ )
    {
        convert = (float) *inptr++; /* convert INTEGER*4 to IEEE floating point */
        if ( convert < 0.0 )
        {
            fprintf(stderr,"ERROR:  Warning, negative data value\n");
            convert = 0.0;
        }
#ifdef TRANSPUTER
        fourbyteint = *cvptr;  /* load up a float as if it were a long */
        for ( j = 0; j < 4; j++ )
        {
            *outptr++ = fourbyteint & 0xff;
            fourbyteint >>= 8;
        }
#else
        *foutptr++ = convert;  /* store floating value in output */
#endif
    }

    fwrite(header, sizeof(char), nbhead, fout);
    /* write data portion of output file */
    fwrite( out_data, sizeof (float), numdaorig, fout);
}
