/*$Id: ct2analyze.c,v 1.2 1996/12/12 19:39:26 jane Exp $*/
/*$Log: ct2analyze.c,v $
 * Revision 1.2  1996/12/12  19:39:26  jane
 * Added ability to correct for gantry tilt:
 * 1. image reorientation (anterior/posterior shift)
 * 2. correct zpix (slice separation) in header
 * Added calculation of table increment from table positions
 * Modified code to speed execution (e.g., fewer malloc's)
 **/
/*  ===============================================================================
 *  File:           ct2analyze.c
 *  Date:           03-April-96
 *  Author:         Jane Dunford-Shore
 *  Description:    Convert Siemens Somatom Plus CT images into Analyze format.
 *                  Three files are created:
 *                      1. Analyze header file   (.hdr)
 *                      2. Analyze image file    (.img)
 *                      3. Interfile header file (.ifh)
 *  ===============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "ANALYZE.h"
#include "errhand.h"
#include "sspct.h"
#include "stringfns.h"

#define MAXSTR    256           /* maximum filename length */

#define TP_START 3159           /* byte position of table pos. in header */
#define TP_LEN      9           /* length of table pos. (in bytes) in header */

/* ---------------------
 * Function:    get_usage
 * Synopsis:    char *get_usage()
 * Description: Define command-line syntax string for this application
 * Purpose:     Make command-line syntax for application easy to locate and
 *              change within source code
 * Arguments:   None
 * Modifies:    No external variables modified
 * Returns:     Pointer to command syntax string
 * Note:        Max. length of command syntax string is determined by constant
 *              MAXLEN_CMDSYN, currently (5/8/96) defined as 1000 in errhand.h
 * Example:
 *              Example 1: err_handler(CMD_SYNTAX, get_usage());
 *              Example 2: strcpy(string, get_usage());
 * ---------------------
 */
 
static char *get_usage() { 

    static char cmd_syntax[MAXLEN_CMDSYN];

    strcpy(cmd_syntax, "Usage: ct2analyze firstfile# lastfile# outfile_prefix [-n]\n");
    strcat(cmd_syntax, "Note:  Correction for gantry tilt is default; -n disables reorientation.\n");
    strcat(cmd_syntax, "Examples: ct2analyze 1 42 p4098\n");
    strcat(cmd_syntax, "          ct2analyze 00001 00042 p3664 -n\n"); 

    return cmd_syntax;
}


/* ---------------------
 * Function:    swaptwo
 * Synopsis:    unsigned short swaptwo (unsigned short num)
 * Description: Swap the two bytes of a 16-bit word
 * ---------------------
 */

unsigned short swaptwo (unsigned short num) {

     /* Swap the bytes */
     num = ((num & 0xff00) >> 8) | ((num & 0x00ff) << 8);
     return num;

}


/* ---------------------
 * Function:    main
 * Synopsis:    void main (int argc, char *argv[])
 * Description: Convert Siemens Somatom Plus CT images into Analyze format.
 *              Three files are created:
 *              1. An Analyze format header file (.hdr)
 *              2. An analyze image file (.img)
 *              3. An Interfile format header file (.ifh)
 * ---------------------
 */
 
void main (int argc, char *argv[]) {

 /*-----------------------*
  * Start of Declarations *
  *-----------------------*/

 /* Command line options/parameters and associated variables */

    char       prefix[MAXSTR]; /* first portion of output filenames */
    int        first = 0;      /* first file number (slice) specified */
    int        last  = 0;      /* last file number (slice) specified */
    int        num_slices;     /* number of slices specified */
    int        slice;	       /* slice of interest */

 /* Input file and related buffers */

    char            infile[MAXSTR];	  /* filename for input file (.ima) */
    FILE            *fp_input     = NULL; /* input file (*.ima) */
    FILE            *fp_input2    = NULL; /* second *.ima - for table increment calc. */
    unsigned short  **pp_databuf1 = NULL; /* original pixel data */
    short           **pp_databuf2 = NULL; /* byte-swapped pixel data cast to short */
    short           *p_databuf3   = NULL; /* byte-swapped, interpolated pixel data */
    short           *p_databuf4   = NULL; /* pixels reordered (image reoriented) */

 /* Output files and related structures */

    DSR        *pstr_anal;	    /* Analyze header structure; defined in ANALYZE.h */
    SSPCT_HDR  *pstr_sspct = NULL;  /* SSPLUS header structure; defined in sspct.h */
    SSPCT_INIT str_hdr_init;        /* header values that get_sspct_head() needs */
    char       anal_img[MAXSTR];    /* filename for Analyze image file */
    char       anal_hdr[MAXSTR];    /* filename for Analyze header file */
    char       if_hdr[MAXSTR];	    /* filename for Interfile header file */
    FILE       *fp_img = NULL;	    /* Analyze image    (.img) */
    FILE       *fp_hdr = NULL;	    /* Analyze header   (.hdr) */
    FILE       *fp_if = NULL;	    /* Interfile header (.ifh) */

 /* Pixel positions and values */

    short      global_max;   /* maximum pixel value */
    short      global_min;   /* minimum pixel value */
    short      pixelval;     /* value of pixel of interest */
    int        pixelno;      /* original position of pixel in pixel array */
    int        lastpixno;    /* array index of last pixel in array */
    int        newpixno;     /* new position of pixel in array (image reoriented) */
    int        column;       /* position of pixel on x axis (which column) */       
    int        row;          /* position of pixel on y axis (which row) */

 /* Correcting for gantry tilt (reorienting image) */

    BOOL       reorient = TRUE;  /* whether or not to reorient */

    double     pixelsize;    /* x and y dimensions of voxels */
    double     row_interval; /* row interval by which slices are offset */
    double     srow_offset;  /* row offset (whole number plus fraction) */
    double     swrow_offset; /* whole number part of row offset */
    double     sfrow_offset; /* fractional part of row offset */

    int        tpix_offset;  /* target pixel offset (in pixels from beg. of data file) */
    int        spix1_offset; /* offset of first source row in pixels */
    int        tpix_no;      /* target pixel number */
    int        spix1_no;     /* first source pixel number */
    int        spix2_no;     /* second source pixel number */
    double     spix1_contrib;  /* contribution of first source pixel to weighted ave. */
    double     spix2_contrib;  /* contribution of second source pixel to weighted ave. */

    BOOL       bdexceed;     /* image boundary exceeded; end loop (TRUE or FALSE) */

    int        gt_deg;       /* gantry tilt in degrees */
    double     gt_rad;       /* gantry tilt in radians */
 
 /* Calculating table increment (slice separation; zpix) */

    char       *string;      /* temporary buffer */
    int        tabpos1;	     /* table position in first file */
    int        tabpos2;	     /* table position in second file */
    int        tabincr;      /* table increment between slices */

 /*---------------------*
  * End of Declarations *
  *---------------------*/

/*
 * Initialize error handler
 */

    err_handler_init(PROGNAME, argv[0]);
    err_handler_init(CMD_SYNTAX, get_usage());

/*
 *  Get command-line parameters
 */

    if (argc < 4 || argc > 5) {
        err_handler(CMD_SYNTAX_ERR, "");
    }

    /* First two parameters should contain only digits */
    if (!isalldigits(argv[1])) {
        err_handler(CMD_SYNTAX_ERR, "");
    }
    else {
        first = atoi(argv[1]);
    }   
  
    if (!isalldigits(argv[2])) {
        err_handler(CMD_SYNTAX_ERR, "");
    }
    else {
        last = atoi(argv[2]);
    }   

    /* Make sure file numbers are not reversed. Note: It is OK for file 
     * numbers to be identical. Only one file is examined in that case.
     */
    if (first > last) {
        err_handler(CMD_SYNTAX_ERR, "");
    }
    num_slices = last - first + 1;

    /* Get output file name prefix */
    strcpy(prefix, argv[3]);
    
    /* Correct for gantry tilt? */
    if (argc == 5) {
        if ( strcmp(argv[4], "-n") ) {
            err_handler(CMD_SYNTAX_ERR, "");
        }
        else {
            reorient = FALSE;
        }
    }

/*
 * Initialize output header and data file pointers
 */
    
    /* Create output file names */
    sprintf(anal_img, "%s.img", prefix);
    sprintf(anal_hdr, "%s.hdr", prefix);
    sprintf(if_hdr, "%s.ifh", prefix);

    if ((fp_hdr = fopen(anal_hdr, "w")) == NULL) {
        err_handler(FILE_OPEN_ERR, anal_hdr);
    }

    if ((fp_img = fopen(anal_img, "w")) == NULL) {
        err_handler(FILE_OPEN_ERR, anal_img);
    }

    if ((fp_if = fopen(if_hdr, "w")) == NULL) {
        err_handler(FILE_OPEN_ERR, if_hdr);
    }

/* 
 * Allocate space for pixel data buffers -- unsigned short (original) and short
 */

    if ((pp_databuf1 = (unsigned short **)malloc(num_slices * sizeof(unsigned short *))) == NULL) {
        err_handler(MEM_ALLOC_ERR, "");
    }

    if ((pp_databuf2 = (short **)malloc(num_slices * sizeof(short *))) == NULL) {
        err_handler(MEM_ALLOC_ERR, "");
    }

    for (slice = 0; slice < num_slices; slice++) {
        if ((pp_databuf1[slice] = (unsigned short *)malloc(LEN_DATA)) == NULL) {
                err_handler(MEM_ALLOC_ERR, "");    
        }
        if ((pp_databuf2[slice] = (short *)malloc(LEN_DATA)) == NULL) {
                err_handler(MEM_ALLOC_ERR, "");    
        }
    }

    if (reorient == TRUE) {
        if ((p_databuf3 = (short *)malloc(LEN_DATA)) == NULL) {
            err_handler(MEM_ALLOC_ERR, "");
        }
    }

    if ((p_databuf4 = (short *)malloc(LEN_DATA)) == NULL) {
                err_handler(MEM_ALLOC_ERR, "");    
    }

/*
 *  Read pixel data from input file(s). 
 */
   
    for (slice = first; slice <= last; slice++) {
        
        /* Get full file name */
        sprintf(infile, "%05d.ima", slice);
        fprintf(stdout, "Reading pixel data in %s\n", infile);     

        /* Open input file */
        if ((fp_input = fopen(infile,"r")) == NULL) {
            err_handler(FILE_OPEN_ERR, infile);
        }

        /* Read pixel data for each slice -- for concatenation later */
        fseek(fp_input, LEN_HEADER, SEEK_SET);
        if (fread(pp_databuf1[slice - first], LEN_DATA, 1, fp_input) != 1) {
            err_handler(FILE_READ_ERR, "");
        }

        /* Close input file */
        if (fp_input != NULL) {
            fclose(fp_input);
            fp_input = NULL;
        }
    }    

/*
 * Convert pixel values from unsigned short, little endian, to signed short,
 * big endian, and get max and min pixel values for whole set of images.
 */

    /* Initialize min and max to first pixel value */
    global_min = (short)swaptwo(pp_databuf1[0][0]);
    global_max = (short)swaptwo(pp_databuf1[0][0]);
    lastpixno  = (COLUMNS * ROWS) - 1;

    fprintf(stdout, "\nByte-swapping pixel data\n"); 

    for (slice = first; slice <= last; slice++) {

        for (pixelno = 0; pixelno <= lastpixno; pixelno++){

	    /* Get pixelval as unsigned short. */
	    /* Swap bytes and convert to signed short */  
            pixelval = (short)swaptwo(pp_databuf1[slice - first][pixelno]);

            /* Second pixel data array (signed short) will now hold pixel values */
	    pp_databuf2[slice - first][pixelno] = pixelval;
            
            if (pixelval < global_min) {
                global_min = pixelval;
            }
            if (pixelval > global_max) {
                global_max = pixelval;
            }
        }

        /* Original data buffer for this slice is no longer needed */
        if (pp_databuf1[slice - first] != NULL) {
            free(pp_databuf1[slice - first]);
            pp_databuf1[slice - first] = NULL;
        }

    }

    fprintf(stdout, "Pixel min: %d\nPixel max: %d\n", global_min, global_max);

    /* Free pointer to all original pixel data buffers */
    if (pp_databuf1 != NULL) {
        free(pp_databuf1);
        pp_databuf1 = NULL;
    }

/* 
 * Extract header info. and place in structures
 */

    fprintf(stdout, "\nCreating header files:\n");
    
    /* Allocate space for SSPLUS header structure defined in sspct.h */
    if ((pstr_sspct = (SSPCT_HDR *)malloc(sizeof(SSPCT_HDR))) == NULL) {
        err_handler(MEM_ALLOC_ERR, "");
    }

    /* Allocate space for Analyze header structure defined in ANALYZE.h */
    if ((pstr_anal = (DSR *)malloc(sizeof(DSR))) == NULL) {
        err_handler(MEM_ALLOC_ERR, "");
    }

    /* Open first specified file - for extracting header values */
    sprintf(infile, "%05d.ima", first);
    if ((fp_input = fopen(infile,"r")) == NULL) {
            err_handler(FILE_OPEN_ERR, infile);
    }
 
    /* Open next sequential file (*.ima) if one exists, in order
     * to get table position from header. 
     */
    sprintf(infile, "%05d.ima", first + 1);
    if ((fp_input2 = fopen(infile,"r")) == NULL) {
        if (num_slices > 1) {   /* second file specified, but couldn't open it */
            err_handler(FILE_OPEN_ERR, infile);
        }
        else { /* second file not specified (and doesn't happen to be present) */
	    printf("Table increment cannot be calculated because only one \n");
            printf("file specified (two are needed for calculation).\n");
            printf("Table increment set equal to slice width (usually the case).\n");
            tabincr = 0; /* tabincr (zpix) will be set to slice width in get_sspct_hdr() */
        }    
    }
    else {
        /* Calculate table increment (dist. table moves between slices).
        * Correction for gantry tilt occurs within get_sspct_hdr(), not here.
        */
        string = get_bin_str(fp_input, TP_START, TP_LEN);
        tabpos1 = atoi(string);
        string = get_bin_str(fp_input2, TP_START, TP_LEN);
        tabpos2 = atoi(string);
        tabincr = abs(tabpos1 - tabpos2);
    }

    /* Provide values that cannot currently be extracted from header */
    str_hdr_init.num_slices = num_slices;
    strcpy(str_hdr_init.prefix, prefix);
    strcpy(str_hdr_init.infile, infile);
    str_hdr_init.global_min = global_min;
    str_hdr_init.global_max = global_max;
    str_hdr_init.table_incr = (float)tabincr;

    /* Extract info. from header and place in SSPCT_HDR structure. */
    /* Fill Analyze header structure, and create Interfile header. */
    get_sspct_head(fp_input, str_hdr_init, pstr_sspct, pstr_anal, fp_if, reorient); 
    fprintf(stdout, "%s created\n", if_hdr);

    /* Close input file */
    if (fp_input != NULL) {
        fclose(fp_input);
        fp_input = NULL;
    }  

    /* Close Interfile header file */
    if (fp_if != NULL) {
        fclose(fp_if);
        fp_if = NULL;
    } 

/* 
 * Create Analyze header file (.hdr)
 */

    if ((fwrite(pstr_anal, sizeof(DSR), 1, fp_hdr)) != 1) {
	err_handler(FILE_WRITE_ERR, anal_img);
    }
    else {
        fprintf(stdout, "%s created\n", anal_hdr);
    }

    if (fp_hdr != NULL) {
        fclose(fp_hdr);
        fp_hdr = NULL;
    }
 
/*
 * Create Analyze data file (.img)
 */

    fprintf(stdout, "\nCreating image data file:\n");

    if (reorient == TRUE) { 
        /* Calculate interval, in rows (floating point), by which pixels are shifted.
         * Actual shift for each slice (mult. of interval) is calculated in slice loop below.
         */ 
        gt_deg = atoi(pstr_sspct->scan.gantry_tilt);        /* gantry tilt in degrees */
        gt_rad = ((double)gt_deg / (double)180.0) * M_PI;   /* gantry tilt in radians */
        pixelsize =  pstr_sspct->scan.xpix;                 /* pixel size (x and y)   */
        row_interval = -((double)tabincr * sin(gt_rad) / pixelsize); 
    }

    for (slice = 0; slice < num_slices; slice++) {

       /* Correct for gantry tilt 
        */
        if (reorient == TRUE) {

           /* Calculate pixel shift for this slice 
            * in rows (whole and fractional) and in whole-row pixels. 
            */
            srow_offset = row_interval * slice;        /* row offset (whole no. plus fraction) */
            if (srow_offset > 0) {
                swrow_offset = floor(srow_offset);             /* whole no. part of row offset */
            }
            else {
                swrow_offset = ceil(srow_offset);
            }
            sfrow_offset = srow_offset - swrow_offset;        /* fractional part of row offset */
            spix1_offset = swrow_offset * COLUMNS;     /* offset of first source row in pixels */

           /* Initialize pixel array to all zeros so that unfilled rows will be zeros. */
            memset(p_databuf3, 0, LEN_DATA);

           /* Shift pixel values in this slice by appropriate number of rows by setting each 
            * "target" pixel to weighted ave. of two source pixel values - e.g., if row offset 
            * for this slice is 2.7 rows, source pixels are in corresponding columns 2 and 3 rows 
            * away from target pixel (+2 and +3 if gantry tilt is neg.; -2 and -3 otherwise).
            */
            row = ( (gt_deg < 0) ? 0 : (ROWS-1) );      /* loop forward through rows if gt is neg. */
            bdexceed = FALSE;
            while (row >= 0 && row <= (ROWS-1) && bdexceed == FALSE) {  
                tpix_offset = row * COLUMNS;
                for (column = 0; column < COLUMNS; column++) {
                    tpix_no = tpix_offset + column;
                    spix1_no = tpix_no + spix1_offset; 
                    spix2_no = spix1_no + ((gt_deg < 0) ? +COLUMNS : -COLUMNS);
                   
                    /* set target pixel to weighted ave. of source pixels */
                    spix1_contrib = (double)pp_databuf2[slice][spix1_no] * (1 - sfrow_offset) ;
                    spix2_contrib = (double)pp_databuf2[slice][spix2_no] * sfrow_offset;
                    p_databuf3[tpix_no] = (short)(spix1_contrib + spix2_contrib); 
                }
                (gt_deg < 0) ? row++ : row--; 

                /* stop if contributing pixel is outside image boundary */            
                if ( ((double)row + srow_offset) < 0 || ((double)row + srow_offset) > ROWS-1 ) {
                    bdexceed = TRUE;
                }
            } 
        }

        newpixno = 0;
        memset(p_databuf4, 0, LEN_DATA);

       /* Rearrange pixels for this slice. Row order is reversed. */
        for (row = ROWS-1; row >= 0; row-- ) {
            tpix_offset = row * COLUMNS;
	    for (column = 0; column < COLUMNS; column++, newpixno++) {
	        if (reorient == TRUE) {
                    p_databuf4[newpixno] = p_databuf3[tpix_offset + column]; 
                }
                else {
                    p_databuf4[newpixno] = pp_databuf2[slice][tpix_offset + column]; 
                }
            }
        }

        /* Write pixel data to output file */
        if ((fwrite(p_databuf4, LEN_DATA, 1, fp_img)) != 1) {
            err_handler(FILE_WRITE_ERR, anal_img);
        }

    }   /* end for (slice = 0; slice < num_slices; slice++) */

    fprintf(stdout, "%s created\n", anal_img);
 
/* 
 * Clean up and exit
 */

    for (slice = 0; slice < num_slices; slice++) {
        if (pp_databuf2[slice] != NULL) {
            free(pp_databuf2[slice]);
            pp_databuf2[slice] = NULL;
        }
    }

    if (pp_databuf2 != NULL) {
        free(pp_databuf2);
    }
 
    if (p_databuf3 != NULL) {
        free(p_databuf3);
    }

    if (p_databuf4 != NULL) {
        free(p_databuf4);
    }

    if (fp_img != NULL) {
        fclose(fp_img);
    }

    fprintf(stdout, "\nct2analyze completed successfully\n");
    exit(0);

}

