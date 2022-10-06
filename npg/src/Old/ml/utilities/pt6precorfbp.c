/* pt6precorfbp.c						  */
/*								  */
/*								  */
/* Modified from gensurvprob.c 17 Nov 89, Gerald C. Johns	  */
/*								  */
/* This program precorrects PETT VI data as it is done using the  */
/* standard software at Neurology.  The formula is:		  */
/*								  */
/* emission = (Emission - Randoms) * Blank Average * Trans Time   */
/*	      _________________________________________________,  */
/*                   Transmission * Blank Time                    */
/*								  */
/* where the Ring Factor has been already been included in the    */
/* transmission data:						  */
/*								  */
/* transmission = (Transmission - Randoms) / Ring Factor	  */
/*								  */
/* Modified 25 Oct 89 Gerald C. Johns				  */
/* Modified  1 Oct 90 David G. Politte				  */
/*								  */
/* This program reads the Blank Average and Blank Scan time from  */
/* the Normalization file (which is the size of a header).  The   */
/* Transmission scan time is read from the header of the	  */
/* Transmission data.						  */
/*								  */
/* The data are in INTEGER*4; the output is in IEEE floating	  */
/* point.							  */

#include <stdio.h>
#include "pt6include.h"

FILE *ftrans;  /* Transmission Data file */
FILE *fblin;   /* Norm header file	 */
FILE *femiss;  /* Emission Data file	 */
FILE *frndom;  /* Randoms Data file	 */
FILE *fout;
char bl_hdr [nbhead];
char tr_hdr [nbhead];
char em_hdr [nbhead];
char rm_hdr [nbhead];

long  tr_data [numdaorig];
long  em_data [numdaorig];
long  rm_data [numdaorig];
float cem_data [numdaorig];  /* corrected emission data */

long  *emptr;
long  *rmptr;
long  *trptr;
float *cemptr;


main ( argc, argv )
int argc;
char **argv;
{

    long  i;
    long  j;
    int   readsize;
    long  *inptr;
    char  *outptr;
    long  bl_scan_time;  /* blank scan time	     */
    short tr_scan_time;  /* transmission scan time   */
    long  bl_average;    /* blank average	     */
    float fblscant;
    float ftrscant;
    float fblav;
    float emission;
    float scale;
    int   slice;
    short hslice;
    short *hsp;

    /* Check that program was invoked correctly. */
    if ( argc < 7 )
    {
	fprintf(stderr,"ERROR in pt6precorfbp.\nUsage:  pt6precorfbp  normhead  transmission  emission  randoms  outfile  slice\n");
        exit(-1);
    }
    slice = atoi ( argv[6] );
    if ( (slice < 1) || (slice > 7 ) )
    {
	fprintf(stderr,"ERROR in pt6precorfbp.  Cannot process slice %d\n",slice);
        exit(-1);
    }

    /* Open all input files and output file. */
    if ( (fblin = fopen( argv[1], "r" )) == NULL )
    {
	fprintf(stderr,"ERROR in pt6precorfbp.  Cannot open %s for input\n",argv[1]);
    }
    if ( (ftrans = fopen( argv[2], "r" )) == NULL )
    {
        fprintf(stderr,"ERROR in pt6precorfbp.  Cannot open %s for input\n",argv[2]);
    }
    if ( (femiss = fopen( argv[3], "r" )) == NULL )
    {
        fprintf(stderr,"ERROR in pt6precorfbp.  Cannot open %s for input\n",argv[3]);
    }
    if ( (frndom = fopen( argv[4], "r" )) == NULL )
    {
        fprintf(stderr,"ERROR in pt6precorfbp.  Cannot open %s for input\n",argv[4]);
    }
    if ( (fout = fopen( argv[5], "w" )) == NULL )
    {
        fprintf(stderr,"ERROR in pt6precorfbp.  Cannot open %s for output\n",argv[5]);
    }

    /* Read Norm header. */
    if ((readsize=fread(bl_hdr,sizeof(char),nbhead,fblin)) != nbhead)
	fprintf(stderr,"ERROR in pt6precorfbp.  Error reading norm header.\n");

    /* Read Transmission file header and then transmission data. */
    fseek ( ftrans, (long) (slice-1) * slicerecord, 0 );
    if ((readsize=fread(tr_hdr,sizeof(char),nbhead,ftrans)) != nbhead)
	fprintf(stderr,"ERROR in pt6precorfbp.  Error reading transmission header.\n");
    if((readsize=fread(tr_data,sizeof(long),numdaorig,ftrans)) != numdaorig)
	fprintf(stderr,"ERROR in pt6precorfbp.  Read size error, transmission.\n");

    /* Read Emission file header and then emission data. */
    fseek ( femiss, (long) (slice-1) * slicerecord, 0 );
    if ((readsize=fread(em_hdr,sizeof(char),nbhead,femiss)) != nbhead)
	fprintf(stderr,"ERROR in pt6precorfbp.  Error reading emission header.\n");
    if((readsize=fread(em_data,sizeof(long),numdaorig,femiss)) != numdaorig)
	fprintf(stderr,"ERROR in pt6precorfbp.  Read size error, emission.\n");

    /* Read randoms (accidentals) file header and then randoms data. */
    fseek ( frndom, (long) (slice-1) * slicerecord, 0 );
    if ((readsize=fread(rm_hdr,sizeof(char),nbhead,frndom)) != nbhead)
	fprintf(stderr,"ERROR in pt6precorfbp.  Error readings randoms header.\n");
    if((readsize=fread(rm_data,sizeof(long),numdaorig,frndom)) != numdaorig)
	fprintf(stderr,"ERROR in pt6precorfbp.  Read size error, randoms.\n");


    /* Look into the headers to make sure that the slice number   */
    /* matches the requested slice.  The slice number is at C     */
    /* array offset 17 considering the header to be a collection  */
    /* of shorts.						  */
    /* Check transmission header. */
    hsp = (short *) tr_hdr;
    hslice = *(hsp + 17);
    if ( hslice != slice )
    {
        fprintf(stderr,"ERROR in pt6precorfbp.  Slice %d requested, Transmission Slice is %d\n",slice,hslice);
    }

    /* Check emission header. */
    hsp = (short *) em_hdr;
    hslice = *(hsp + 17);
    if ( hslice != slice )
    {
        fprintf(stderr,"ERROR in pt6precorfbp.  Slice %d requested, Emission Slice is %d\n",slice,hslice);
    }

    /* Check randoms header. */
    hsp = (short *) rm_hdr;
    hslice = *(hsp + 17);
    if ( hslice != slice )
    {
        fprintf(stderr,"ERROR in pt6precorfbp.  Slice %d requested, Randoms Slice is %d\n",slice,hslice);
    }

    /* Compute a scale factor from the data in the headers. */
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
    scale = ( fblav * ftrscant ) / fblscant;

    /* Main loop. */
    emptr  = em_data;  /* integer */
    rmptr  = rm_data;  /* integer */
    trptr  = tr_data;  /* integer */
    cemptr = cem_data; /* float   */

    for ( i = 0; i < numdaorig; i++ )
    {
	if ( *trptr ==0 ) /* division by zero imminent */
	{
	    emission = 0.0;
	    emptr++;
	    rmptr++;
	    trptr++; /* pass over this data point */
	}
	else
            emission = ( (float)(*emptr++ - *rmptr++) * scale ) / ( (float)( *trptr++ ));
        *cemptr++ = emission;
    }

    /* Write the header and output file. */
    fwrite(em_hdr,sizeof(char), nbhead, fout);
    fwrite( cem_data, sizeof (float), numdaorig, fout);

}
