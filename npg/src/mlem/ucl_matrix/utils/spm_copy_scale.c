#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "analyze.h"			 /* dsr */


/*	----------------------------
 *	Function:			cti2analyze
 *	----------------------------
 */
static char *version = "1.9";
static char *program_date = "1996:09:07";
static void usage() {
	fprintf (stderr,
		"Usage:\tspm_copy_scale [-i source_img | -s scale_factor] -o dest_img\n");
	exit (1);
}

main (argc, argv)
	int			 argc;
	char		   *argv[];
{

	struct dsr	  hdr;			 /* header for ANALYZE */
	FILE		   *fd_hdr;			 /* file for ANALYZE hdr */

	char		   *source_img=NULL;		 /* input image filename */
	char		   *dest_img=NULL;		 	/* destination image filename */
	char		   source_hdr[256];
	char		   dest_hdr[256];	/* output Analyze image filename */
	char			tmp[80];
	char			*p;
	int c;
	float	scale_factor = -1;
	extern char *optarg;

/*
 *	Get command line arguments and initialize filenames:
 *	---------------------------------------------------
 */


     while ((c = getopt (argc, argv, "i:o:s:")) != EOF) {
        switch (c) {
        case 'i' :
			source_img = optarg;
            break;
        case 'o' :
            dest_img = optarg;
			break;
        case 's' :
            sscanf(optarg,"%g",&scale_factor);
        }
    }
	if (source_img==NULL && scale_factor <= 0.0)  usage();
	if (dest_img==NULL) usage();

	if (scale_factor <= 0.0) {
		if ( (p = strrchr(source_img,'.')) != NULL) *p = '\0';
		sprintf(source_hdr,"%s.hdr",source_img);
		if ((fd_hdr = fopen (source_hdr, "r")) == NULL) {
			perror(source_hdr);
			exit (1);
		}
	
	/*
 	* read Analyze hdr file
 	*/
		if ((fread (&hdr, sizeof (struct dsr), 1, fd_hdr)) != 1) {
			printf ("Error reading to: %s\n", source_hdr);
			exit (1);
		}
		fclose(fd_hdr);

		if (ntohs(1) != 1) {
			swab(&hdr.dime.funused1,tmp,sizeof(float));
			swaw(tmp,&hdr.dime.funused1,sizeof(float)/2);
		}
		scale_factor = hdr.dime.funused1;
	}

/*
 * modify destination Analyze hdr file
 */
	if ( (p = strrchr(dest_img,'.')) != NULL) *p = '\0';
	sprintf(dest_hdr,"%s.hdr",dest_img);
	if ((fd_hdr = fopen (dest_hdr, "r")) == NULL) {
		perror(dest_hdr);
		exit (1);
	}

/*
 * read Analyze hdr file
 */
	if ((fread (&hdr, sizeof (struct dsr), 1, fd_hdr)) != 1) {
		printf ("Error reading to: %s\n", dest_hdr);
		exit (1);
	}
	fclose(fd_hdr);

	hdr.dime.funused1 = scale_factor;
	if (ntohs(1) != 1) {
		swab(&hdr.dime.funused1,tmp,sizeof(float));
		swaw(tmp,&hdr.dime.funused1,sizeof(float)/2);
	}
	
	if ((fd_hdr = fopen (dest_hdr, "w")) == NULL) {
		perror(dest_hdr);
		exit (1);
	}

	if ((fwrite (&hdr, sizeof (struct dsr), 1, fd_hdr)) != 1) {
		printf ("Error writing to: %s\n", dest_hdr);
		exit (1);
	}
	fclose(fd_hdr);
}
