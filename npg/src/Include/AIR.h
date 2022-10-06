/*	$Log: AIR.h,v $
 * Revision 1.1  1996/03/05  22:43:07  tom
 * Initial revision
 *
	$Id: AIR.h,v 1.1 1996/03/05 22:43:07 tom Exp $
 */
/* AIR.h --header file for Automated Image Registration Subroutines */
/* Copyright 1993 Roger P. Woods, M.D. */
/* Modified 8/5/93 */

#ifndef AIR_INCLUDED
#define AIR_INCLUDED

#ifndef lint
static char rcsid_air [] = "$Header: /usr/local/npg/include/RCS/AIR.h,v 1.1 1996/03/05 22:43:07 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* The following information is site specific */

#define VERBOSITY 0      /*0 turns off nonessential screen printing*/


#define MAGIC_Z1 0		/*Number of planes in uninterpolated PET file*/
#define MAGIC_Z2 0		/*Number of planes in cubic voxel interpolation of PET file*/
				/*Apply zoom.c to a file to find out the interpolated number of planes*/

#define MAGIC_XYSIZE 2.608	/*Standard PET pixel size for use by fixheader*/
#define MAGIC_ZSIZE 3.375	/*Standard interplane distance for use by fixheader*/


#define PIX_SIZE_ERR .01	/* x y size anisotropy must be less than this value*/


#define PIXEL_MAX_SIZE 10	/* warning issued if voxel size exceeds this*/
#define PIXEL_MIN_SIZE .1	/* warning issued if voxel size less than this*/

#define EDITED 5.0		/* warning issued if MRI contains less than this percentage of zero pixel values*/
				/* suggesting that it has not been edited--value of 0.0 inactivates this feature*/

#define OLD_ANALYZE 1		/* if 1, warning will be issued whenever*/
				/* a file is loaded that has voxels sizes of */
				/* 1.000 x 1.000 x1.000 suggesting that*/
				/* file was created by a version of ANALYZE	*/
				/* that sometimes fails to preserve voxel sizes		*/
				/* (particularly when a file has been edited*/
				/* set to 0 (zero) to inactivate*/


#define OUTBITS 16		/* Options are 8 and 16 */
				/* Controls internal representation of data */
				/* Controls bits/pixel of output data*/
				/* Data with a different number of bits/pixel can be input*/
				/* but will be converted to this number of bits/pixel*/
				/* using bit shifting to increase the number*/
				/* and using bit shifting, possibly combined with rescaling*/
				/* to decrease the number of bits/pixel.*/
				/* (Whether or not to rescale is dictated by the header*/
				/* global maximum					*/


#define REP16 3			/* This variable is only relevant when OUTPUTBITS==16 	*/
				/* However, its value must always be defined		*/
				/* If 1, 16 bit data will be written to disk as		*/
				/* 	unsigned short ints (NOT ANALYZE compatible) 	*/
				/*	header minimum will be 0, maximum 65535		*/
				/* If 2, 16 bit data will be written to disk as		*/
				/* 	short ints w/o using negative values		*/
				/*	this effectively reduces storage to 15 bits	*/
				/* 	and reduction is done via bit shifting		*/
				/*	header minimum will be 0, maximum 32767		*/
				/* If 3, 16 bit data will be written to disk as		*/
				/* 	short ints using negative values		*/
				/*	an actual value of zero will be mapped to	*/
				/* 	-32768 in short int representation		*/
				/*	header minimum will be -32767 			*/
				/*	header maximum will be 32767 			*/


/* The remaining information should not generally require modification */

#if(OUTBITS==8)

typedef unsigned char my_pixels;
#define MAX_POSS_VALUE 255

#elif(OUTBITS==16)

typedef unsigned short int my_pixels;
#define MAX_POSS_VALUE 65535	/* This is true regardless of the value of REP16 above*/
				/* 16-bit data is always represented internally is*/
				/* unsigned short ints, even when it is written to*/
				/* output files in other formats*/

#endif


#define HDR_SUFF ".hdr"		/* This suffix will be used in trying to open an image header*/
#define IMG_SUFF ".img"		/* This suffix will be used in trying to open an image file*/

#define LOGFILE "\0"		/* Use "\0" if .img files don't have corresponding .log files*/
#define NORM_SUFF "\0"	/* Use "\0" if .log files containing ASCII weighting factors are not available*/

/* The following information relates to online registration which is not supported in this release*/


#define Z_ERROR 0
#define Y_ERROR 5.7             /*true rotation center of scanner is Y_ERROR mm below center of recontructed images*/
#define X_ERROR 1.4             /*true rotation center of scanner is X_ERROR mm to the left of center of reconstr images*/

/* These are the internal structs*/
/* The external struct (for image headers as stored on disk)*/
/* is stored in a separate file*/

struct key_info{
        int bits;
        int x_dim;
        int y_dim;
        int z_dim;
        double x_size;
        double y_size;
        double z_size;
};

struct air{
        double  		e[4][3];
        char    		s_file[128];
        struct key_info		s;
        char    		r_file[128];
        struct key_info 	r;
        char    		comment[128];
	unsigned long int	s_hash;
	unsigned long int	r_hash;
	char			reserved[120];
};

#endif /* AIR_INCLUDED */
