/*$Id: rdraster.c,v 1.5 1995/08/02 14:59:23 ty7777 Exp tom $*/
/*$Log: rdraster.c,v $
 * Revision 1.5  1995/08/02  14:59:23  ty7777
 * Added processing for reading PC generated images.
 *
 * Revision 1.4  1995/06/27  16:44:06  ty7777
 * Added processing for HR961 data.
 *
 * Revision 1.3  1995/02/22  17:52:42  ty7777
 * Read both old and new sun raster file with source from the Mac scanner.
 *
 * Revision 1.2  1993/04/20  19:37:14  ty7777
 * Before writing getsxr.c
 *
 * Revision 1.1  1993/01/26  20:57:54  ty7777
 * Initial revision
 **/

#ifndef lint
static char     sccsid[] = "@(#)readraster.c	10/26/92  Copyright Neural Pet Group, Washington University 1992";

#endif

static char     rcsid[] = "$Id: rdraster.c,v 1.5 1995/08/02 14:59:23 ty7777 Exp tom $";

/*___________________________________________________________________________
  File:		readraster.c

  Function:	void ReadRaster ()

  Arguments:

  Description:	This function reads a Sun raster image and returns the image
		array and the number of rows and columns.

  History:	Created by Tom Yang on 10/23/92.
___________________________________________________________________________*/

#include <TY/xray.h>
#define ON_VALUE 100

PUBLIC void     ReadRaster (raster_file, cols, rows, tracing, header_only)
	char           *raster_file;
	int            *cols;
	int            *rows;
	short int      *tracing;
	BOOLEAN         header_only;
{
	FILE           *rasterfp;
	int             cx, cy, depth, pcols;
	int             max_value;
	int             threshold;
	register int    i, j;
	register int    size;
	struct rasterfile header;
	unsigned char  *char_buffer;
	unsigned char  *temp_colormap;
	unsigned int    width, height;

	rasterfp = fopen (raster_file, "rb");

	/*
	 * Read in the rasterfile. First the header.
	 */
	if (fread (&header, sizeof (header), 1, rasterfp) != 1)
		fprintf (stderr, "unable to read in raster file header\n");

	*cols = header.ras_width;
	*rows = header.ras_height;
	depth = header.ras_depth;

	if (*cols <= 0)
		fprintf (stderr, "invalid cols: %d", *cols);
	if (*rows <= 0)
		fprintf (stderr, "invalid rows: %d", *rows);

	if (header_only) {
		fclose (rasterfp);
		return;
	}

	/*
	 * If there is a color map, read it.
	 */
	if (header.ras_maplength != 0 && header.ras_maptype == RMT_EQUAL_RGB) {
		temp_colormap = (unsigned char *) malloc (header.ras_maplength);

		if (fread (temp_colormap, header.ras_maplength, 1, rasterfp) != 1)
			printf (stderr, "unable to read in rasterfile colormap\n");

		free (temp_colormap);
	}

	/*
	 * Read in the image data and convert all the 8-bit pixels to 16-bit..
	 */
	if (header.ras_type == RT_STANDARD && (depth == 8 || depth == 1)) {

		/*
		 * Read in image data.
		 */
		size = *cols * *rows;
		pcols = *cols;
		if (*cols % 2 != 0) {		 /* Sun raster file has even number of columns */
			size = (*cols + 1) * *rows;
			pcols = (*cols + 1);
		}
		char_buffer = (unsigned char *) malloc (size);
		if (fread (char_buffer, size, 1, rasterfp) != 1)
			printf (stderr, "unable to read in rasterfile data\n");

		/*
		 * Convert image data.
		 */
		max_value = 0;
		for (i = 0; i < size; i++) {
			if (char_buffer[i] > max_value)
				max_value = char_buffer[i];
		}
		threshold = ROUND (max_value * 0.5);

		for (j = 0; j < *rows; j++) {
			for (i = 0; i < *cols; i++) {
				if (char_buffer[j * pcols + i] < threshold) {
					tracing[j * *cols + i] = ON_VALUE;
				} else {
					tracing[j * *cols + i] = 0;
				}
			}
		}
		free (char_buffer);

		if (char_buffer[0] < threshold) {
			for (i = 0; i < size; i++) {
				tracing[i] = ON_VALUE - tracing[i];
			}
		}
	}
	fclose (rasterfp);
}
