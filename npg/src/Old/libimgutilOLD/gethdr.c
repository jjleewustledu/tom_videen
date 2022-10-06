/*	$Id: gethdr.c,v 2.1 1999/12/13 17:04:17 tom Exp $
	$Log: gethdr.c,v $
 * Revision 2.1  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 2.0  1996/04/24  23:07:41  tom
 * new library release
 *
 * Revision 1.1  1996/03/12  23:00:34  tom
 * Initial revision
 * */
/* ===============================================================================
 *	Module:			gethdr.c
 *	Date:			Apr-92
 *	Author:			Tom Yang
 *	Description:	Read an 'hdr' file into HEADER_DATA.
 * ===============================================================================
 */
#ifndef lint
static char		rcsid[] = "$Header: /home/npggw/tom/src/libimgutilOLD/RCS/gethdr.c,v 2.1 1999/12/13 17:04:17 tom Exp $";
#endif 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>
#include <libimage.h>
#include <imgutil.h>

int      gethdr (filename, header)
	char           *filename;
	HEADER_DATA    *header;
{
	FILE           *hdrfp;
	OS32_HEADER_DATA *os32_header;

	if ((hdrfp = fopen (filename, "rb")) == (FILE *) NULL)
		return (ERROR);
	
	os32_header = (OS32_HEADER_DATA *) malloc (sizeof (OS32_HEADER_DATA));

	fread (os32_header, sizeof (OS32_HEADER_DATA), 1, hdrfp);

	memcpy (header, os32_header, sizeof (HEADER_DATA));

	header->totalcnts = pe2ieee (os32_header->totalcnts);
	header->scancnts = pe2ieee (os32_header->scancnts);
	header->scanst = pe2ieee (os32_header->scanst);
	header->scanlen = pe2ieee (os32_header->scanlen);
	header->framelen = pe2ieee (os32_header->framelen);
	header->tau = pe2ieee (os32_header->tau);
	header->pettcnv1 = pe2ieee (os32_header->pettcnv1);
	header->aflow1 = pe2ieee (os32_header->aflow1);
	header->bflow1 = pe2ieee (os32_header->bflow1);
	header->bvfactor1 = pe2ieee (os32_header->bvfactor1);
	header->aoxygen1 = pe2ieee (os32_header->aoxygen1);
	header->boxygen1 = pe2ieee (os32_header->boxygen1);
	header->awater1 = pe2ieee (os32_header->awater1);
	header->bwater1 = pe2ieee (os32_header->bwater1);
	header->o2cnts1 = pe2ieee (os32_header->o2cnts1);
	header->oxycont1 = pe2ieee (os32_header->oxycont1);
	header->decay_factor1 = pe2ieee (os32_header->decay_factor1);
	header->pettcnv2 = pe2ieee (os32_header->pettcnv2);
	header->aflow2 = pe2ieee (os32_header->aflow2);
	header->bflow2 = pe2ieee (os32_header->bflow2);
	header->bvfactor2 = pe2ieee (os32_header->bvfactor2);
	header->aoxygen2 = pe2ieee (os32_header->aoxygen2);
	header->boxygen2 = pe2ieee (os32_header->boxygen2);
	header->awater2 = pe2ieee (os32_header->awater2);
	header->bwater2 = pe2ieee (os32_header->bwater2);
	header->o2cnts2 = pe2ieee (os32_header->o2cnts2);
	header->oxycont2 = pe2ieee (os32_header->oxycont2);
	header->decay_factor2 = pe2ieee (os32_header->decay_factor2);
	header->pieslope = pe2ieee (os32_header->pieslope);
	header->efactor = pe2ieee (os32_header->efactor);

	fclose (hdrfp);
	free (os32_header);
	return (OK);
}
