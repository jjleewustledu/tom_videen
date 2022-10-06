/******************************************************************************

* topic History
  Author(s)     : J Nuyts
  Date          : Jan. 1998
  Modifications :
* endtopic History

******************************************************************************/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "matrix.h"
#include "matrix_extra.h"
#include "NChrplus.h"

/*======================================================================*/
int main (argc, argv)
	int             argc;
	char           *argv[];

{
	char            filename[150];
	int             i;
	int             retmin = 0;
	int             retmax = 0;
	int             showhelp = 0;
	int             frame = 1;
	int             plane = 1;
	int             gate = 1;
	int             data = 0;
	int             bed = 0;
	int             is3d;
	int             matnum;
	int             npixels;
	MatrixFile     *inputfile = NULL;
	MatrixData     *inputvolume;
	MatrixData     *inmatrix;
	Image_subheader *input_sub;
	float          *img;
	float          *p_img;
	float           maximum, minimum;

/* Get the arguments
********************/
	strcpy (filename, "");
	i = 1;
	while (i < argc) {
		if (strcmp (argv[i], "-i") == 0)
			strcpy (filename, argv[i++ + 1]);
		if (strcmp (argv[i], "-min") == 0)
			retmin = 1;
		if (strcmp (argv[i], "-max") == 0)
			retmax = 1;
		if (strcmp (argv[i], "-h") == 0)
			showhelp = 1;
		if (strcmp (argv[i], "-ms") == 0)
			sscanf (argv[i++ + 1], "%d,%d,%d,%d,%d", &frame, &plane, &gate,
				&data, &bed);
		i++;
	}

	if ((retmin == 0) && (retmax == 0)) {
		retmin = 1;
		retmax = 1;
	}
	showhelp = showhelp || (strcmp (filename, "") == 0);
	if (showhelp) {
		printf ("Syntax: programname -i filename [-min] [-max] [-ms f,p,g,d,b] [-h]\n");
		printf (" -i   matrix filename (image version 6 or volume version 70)\n");
		printf (" -min:   return minimum\n");
		printf (" -max:   return maximum\n");
		printf ("      if neither -min nor -max is set, both min and max are returned\n");
		printf (" -ms  frame, plane, gate, data, bed: matrix spec. (default: 1,1,1,0,0)\n");
		printf (" -h:     show help (and perform no action)\n");
		exit (0);
	}
/* Open the file
****************/
/* Open the input file as a PetImage or a PetVolume
****************************************/
	inputfile = matrix_open (filename, MAT_READ_ONLY, PetImage);
	if (!inputfile) {
		is3d = 1;
		inputfile = matrix_open (filename, MAT_READ_ONLY, PetVolume);
		if (!inputfile) {
			printf ("Unable to open the file %s as an image or a volume\n",
				filename);
			exit (0);
		}
	} else
		is3d = 0;

/* Read the slice
****************/
	if (is3d) {
		matnum = mat_numcod (frame, 1, gate, data, bed);
		inputvolume = matrix_read (inputfile, matnum, MAT_SUB_HEADER);
		if (!inputvolume) {
			printf ("Unable to read the volume\n");
			exit (0);
		}
		inmatrix = matrix_read_slice (inputfile, inputvolume, plane - 1, 0);
		if (!inmatrix) {
			printf ("Unable to read the slice from the volume");
			exit (0);
		}
	} else {
		matnum = mat_numcod (frame, plane, gate, data, bed);
		inmatrix = matrix_read (inputfile, matnum, UnknownMatDataType);
		if (!inmatrix) {
			printf ("Unable to read the slice from the image file\n");
			exit (0);
		}
	}

/* Get the min and/or max
*************************/
	input_sub = (Image_subheader *) inmatrix->shptr;
	img = NCread_mat_float (inmatrix, 1.0);
	npixels = input_sub->x_dimension * input_sub->y_dimension;

	if (retmin) {
		p_img = img;
		minimum = *p_img;
		for (i = 0; i < npixels; i++) {
			if (*p_img < minimum)
				minimum = *p_img;
			p_img++;
		}
	}
	if (retmax) {
		p_img = img;
		maximum = *p_img;
		for (i = 0; i < npixels; i++) {
			if (*p_img > maximum)
				maximum = *p_img;
			p_img++;
		}
	}
	if (retmin && retmax)
		printf ("%1.15f, %1.15f\n", minimum, maximum);
	else {
		if (retmin)
			printf ("%1.15f\n", minimum);
		else
			printf ("%1.15f\n", maximum);
	}
}
