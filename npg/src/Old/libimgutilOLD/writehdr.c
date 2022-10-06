/* $Log: writehdr.c,v $
 * Revision 2.3  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 2.2  1999/07/07  15:19:36  tom
 * get scaling constants from imgutil.h
 *
 * Revision 2.1  1996/09/03  18:23:46  tom
 * set ntype properly
 *
 * Revision 2.0  1996/04/24  23:07:41  tom
 * new library release
 *
 * Revision 1.1  1996/03/12  23:00:34  tom
 * Initial revision
 *
 * Revision 1.4  1995/11/10  18:48:23  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.3  1994/10/03  16:17:13  tom
 * standard format with indent
 *
 * Revision 1.2  1994/05/18  15:53:30  tom
 * changed $Id: writehdr.c,v 2.3 1999/12/13 17:04:17 tom Exp $ to $Header: /home/npggw/tom/src/libimgutilOLD/RCS/writehdr.c,v 2.3 1999/12/13 17:04:17 tom Exp $
 * */
/*	========================================================================
 *	Module:			writehdr.c
 *	Date:			27-Sep-93
 *	Author:			Tom Videen
 *	Description:	Write an hdr file
 *
 *	Input:
 *		header  -   hdr structure
 *		outfile -   name of output file
 *		userid  -   user's id  
 *		today   -   date
 *		processing - type of processing
 *		nslices -   number of slces
 *	 
 *	Output:
 *		creates the file "outfile"
 *	 
 *	Algorithm:
 *	1)  Open the outfile.
 *	2)  Copy the hdr structure in "header" to a OS32-hdr structure:
 *		All real numbers must be converted from IEEE format to OS32 format.
 *	3)  Add or change the following to the header:
 *			outfile -   name of output file
 *			userid  -   user's id  
 *			today   -   date
 *			ntype   -   code number for type of processing
 *			nslices -   number of slces
 *	4)  Write the new structure to the output file and close.
 *	
 *	========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutilOLD/RCS/writehdr.c,v 2.3 1999/12/13 17:04:17 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>
#include <libimage.h>
#include <imgutil.h>

int	writehdr (header, outfile, userid, today, processing, nslices)
	HEADER_DATA    *header;
	char           *outfile;
	char           *userid;
	char           *today;
	int             processing;
	short           nslices;
{
	char            filename[255];
	char           *ext;
	float           zero = 0.;
	float           factor;
	FILE           *hdrfp;
	OS32_HEADER_DATA *os32_header;

	strcpy (filename, outfile);
	ext = strrchr (filename, '.');
	strcpy (ext, ".hdr");

	if ((hdrfp = fopen (filename, "w")) == (FILE *) NULL) {
		printf (" Error: Cannot open header file %s\n", filename);
		return (ERROR);
	}
	os32_header = (OS32_HEADER_DATA *) malloc (sizeof (HEADER_DATA));
	memcpy (os32_header, header, sizeof (OS32_HEADER_DATA));

	os32_header->slices = nslices;
	strncpy (os32_header->initials, userid, sizeof (os32_header->initials));
	strncpy (os32_header->procdate, today, sizeof (os32_header->procdate));

	os32_header->totalcnts = ieee2pe (&header->totalcnts);
	os32_header->scancnts = ieee2pe (&header->scancnts);
	os32_header->scanst = ieee2pe (&header->scanst);
	os32_header->scanlen = ieee2pe (&header->scanlen);
	os32_header->framelen = ieee2pe (&header->framelen);
	os32_header->tau = ieee2pe (&header->tau);
	os32_header->pettcnv1 = ieee2pe (&header->pettcnv1);
	os32_header->aflow1 = ieee2pe (&header->aflow1);
	os32_header->bflow1 = ieee2pe (&header->bflow1);
	os32_header->bvfactor1 = ieee2pe (&header->bvfactor1);
	os32_header->aoxygen1 = ieee2pe (&header->aoxygen1);
	os32_header->boxygen1 = ieee2pe (&header->boxygen1);
	os32_header->awater1 = ieee2pe (&header->awater1);
	os32_header->bwater1 = ieee2pe (&header->bwater1);
	os32_header->o2cnts1 = ieee2pe (&header->o2cnts1);
	os32_header->oxycont1 = ieee2pe (&header->oxycont1);
	os32_header->decay_factor1 = ieee2pe (&header->decay_factor1);
	os32_header->pettcnv2 = ieee2pe (&header->pettcnv2);
	os32_header->aflow2 = ieee2pe (&header->aflow2);
	os32_header->bflow2 = ieee2pe (&header->bflow2);
	os32_header->bvfactor2 = ieee2pe (&header->bvfactor2);
	os32_header->aoxygen2 = ieee2pe (&header->aoxygen2);
	os32_header->boxygen2 = ieee2pe (&header->boxygen2);
	os32_header->awater2 = ieee2pe (&header->awater2);
	os32_header->bwater2 = ieee2pe (&header->bwater2);
	os32_header->o2cnts2 = ieee2pe (&header->o2cnts2);
	os32_header->oxycont2 = ieee2pe (&header->oxycont2);
	os32_header->decay_factor2 = ieee2pe (&header->decay_factor2);
	os32_header->pieslope = ieee2pe (&header->pieslope);
	os32_header->efactor = ieee2pe (&header->efactor);

	if (processing == PROC_OEF) {
		os32_header->ntype = 11;
		factor = 1/SCALE_OEF;
		os32_header->aoxygen1 = ieee2pe (&factor);
		os32_header->boxygen1 = ieee2pe (&zero);
		os32_header->awater1 = ieee2pe (&zero);
		os32_header->bwater1 = ieee2pe (&zero);
		os32_header->o2cnts1 = ieee2pe (&zero);
		os32_header->oxycont1 = ieee2pe (&zero);
		os32_header->pettcnv1 = ieee2pe (&header->pettcnv1);

	} else if (processing == PROC_CMRO2) {
		os32_header->ntype = 12;
		factor = 1/SCALE_CMRO2;
		os32_header->aoxygen1 = ieee2pe (&factor);
		os32_header->boxygen1 = ieee2pe (&zero);
		os32_header->awater1 = ieee2pe (&zero);
		os32_header->bwater1 = ieee2pe (&zero);
		os32_header->o2cnts1 = ieee2pe (&zero);
		os32_header->oxycont1 = ieee2pe (&zero);
		os32_header->pettcnv1 = ieee2pe (&header->pettcnv1);

	} else if (processing == PROC_CBF) {
		os32_header->ntype = 2;
		factor = 1/SCALE_CBF;
		os32_header->aflow1 = ieee2pe (&zero);
		os32_header->bflow1 = ieee2pe (&factor);
		os32_header->pettcnv1 = ieee2pe (&header->pettcnv1);

	} else if (processing == PROC_CBV) {
		os32_header->ntype = 3;
		factor = 1/SCALE_CBV;
		os32_header->bvfactor1 = ieee2pe (&factor);
		os32_header->pettcnv1 = ieee2pe (&header->pettcnv1);
	}
	fwrite (os32_header, sizeof (HEADER_DATA), 1, hdrfp);

	fclose (hdrfp);
	free (os32_header);
	return (OK);
}
