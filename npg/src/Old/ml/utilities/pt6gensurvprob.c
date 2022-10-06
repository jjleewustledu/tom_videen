/* pt6gensurvprob.c */

/* modified 25 Oct 89 Gerald C. Johns */


/* This program reads two numbers from the Blank Average file */
/* to get the  Blank Average and the time for the Blank Average scan*/
/* Then the Transmission file is read, and the survival probability */
/* is calculated and stored in the output file. */
/* the data portion is initially INTEGER*4, and will be output */
/* as IEEE Floating Point.  */
/* the header for the transmission scan becomes the header for */
/* the survival probability file. */


#include <stdio.h>
#include "pt6include.h"

FILE *fin;
FILE *fblin;
FILE *fout;
char bl_hdr [nbhead];
char tr_hdr [nbhead];

long tr_data [numdaorig];
float sp_data [numdaorig];  /* survival probability data */


long * instore;
float * outstore;


main ( argc, argv )
int argc;
char **argv;
{

    long i;
    long j;
    int byteno;
    int c;
    long fourbyteint;  /* four bytes as an integer */
    long fsize;
    int readsize;
    long origsize;
    long *inptr;
    char *outptr;
    long bl_scan_time;  /* blank scan time */
    short tr_scan_time;  /* transmission scan time */
    long bl_average;    /* blank average */
    float fblscant;
    float ftrscant;
    float fblav;
    float survprob;
    float scale;
    int negcount;
    int slice;

    if ( argc < 5 )
    {
        fprintf(stderr,"Usage:  pt6gensurvprob blankfile transmissionfile fileoutname slice\n");
        exit(0);
    }
    slice = atoi ( argv[4] );
    if ( (slice < 1) || (slice > 7 ) )
    {
        fprintf(stderr,"Error, cannot process slice %d\n",slice);
        exit(0);
    }
       /* printf("Extracting slice %d\n",slice);  */

    if ( (fblin = fopen( argv[1], "r" )) == NULL )
    {
        fprintf(stderr,"ERROR.  Cannot open %s for input\n",argv[1]);
    }
    if ( (fin = fopen( argv[2], "r" )) == NULL )
    {
        fprintf(stderr,"ERROR.  Cannot open %s for input\n",argv[2]);
    }

    if ( (fout = fopen( argv[3], "w" )) == NULL )
    {
        fprintf(stderr,"ERROR.  Cannot open %s for output\n",argv[3]);
    }

    instore = tr_data;
    outstore = sp_data;
    if ((readsize=fread(bl_hdr,sizeof(char),nbhead,fblin)) != nbhead)
        fprintf(stderr,"ERROR.  Error reading blank header\n");
    fseek ( fin, (long) (slice-1) * slicerecord, 0 );
    if ((readsize=fread(tr_hdr,sizeof(char),nbhead,fin)) != nbhead)
        fprintf(stderr,"ERROR.  Error reading transmission header\n");
    /* read entire transmission file */
    if((readsize=fread(instore,sizeof(long),numdaorig,fin)) != numdaorig)
        fprintf(stderr,"ERROR.  read size error\n");

    /* Compute a scale factor from the data in the headers */
    bl_average = (long) ( ((bl_hdr[0]&0xff)<<24) |
        ((bl_hdr[1]&0xff)<<16) |
        ((bl_hdr[2]&0xff)<<8)  | (bl_hdr[3]&0xff));
    fblav = (float) bl_average;

    bl_scan_time = (long) ( ((bl_hdr[4]&0xff)<<24) |
        ((bl_hdr[5]&0xff)<<16) |
        ((bl_hdr[6]&0xff)<<8)  | (bl_hdr[7]&0xff));
    fblscant = (float) bl_scan_time;

    tr_scan_time = (short) (  ((tr_hdr[14]&0xff)<<8) | (tr_hdr[15]&0xff) );
    ftrscant = (float) tr_scan_time;
    scale = fblscant / ( fblav * ftrscant );
       /* printf("The Blank Average is %8.0f\n",fblav);  */
       /* printf("The blank scan time is %8.0f\n",fblscant);  */
       /* printf("transmission scan time is %6.0f seconds\n",ftrscant);  */
    for ( negcount=0, instore = tr_data, outstore = sp_data, i=0; i<numdaorig; i++ )
    {
        survprob = (float) (*instore++) * scale;  /* compute survival prob */
        if ( (survprob) < 0.0 )
        {
            survprob = 0.0;
            negcount++;
        }
        *outstore++ = survprob;
    }
    if ( negcount != 0 )
        fprintf(stderr, "ERROR.  There were %d negative data values, set to zero\n",negcount);

#ifdef TRANSPUTER
    byteno = 0;
    inptr = (long*) sp_data;
    outptr = (char*) sp_data;  /* in-place byte swap */
    for ( i = 0; i < numdaorig; i++ )
    {
        fourbyteint = *inptr++;
        for ( j = 0; j < 4; j++ )
        {
            *outptr++ = fourbyteint & 0xff;
            fourbyteint >>= 8;
        }
        byteno++;
    }
#endif

    /* write the output header */
    fwrite(tr_hdr,sizeof(char), nbhead, fout);
    /* write entire output file */
    fwrite( sp_data, sizeof (float), numdaorig, fout);
}
