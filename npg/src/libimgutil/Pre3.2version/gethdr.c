/*	$Id: gethdr.c,v 3.1 2003/08/21 15:58:11 tom Exp $
	$Log: gethdr.c,v $
 * Revision 3.1  2003/08/21  15:58:11  tom
 * eliminate error message if hdr file does not exist
 *
 * Revision 3.0  2003/08/21  15:16:40  tom
 * 21Aug2003
 *
	Revision 2.1  1999/12/13 17:04:17  tom
	tov revision

 * Revision 2.0  1996/04/24  23:07:41  tom
 * new library release
 *
 * Revision 1.1  1996/03/12  23:00:34  tom
 * Initial revision
 * */
/* ===============================================================================
 *	Module:			gethdr.c
 *	Date:			Mar 2003
 *	Author:			Tom Videen
 *	Description:	Read an hdr file into HEADER_DATA.
 * ===============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/gethdr.c,v 3.1 2003/08/21 15:58:11 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>
#include <imgutil.h>
#include <endian.h>

int             gethdr (fname, hdr)
	char           *fname;
	HEADER_DATA    *hdr;
{
	FILE           *fp;
	int				i, j;
	long            nbytes = 256;

	union temp_short {
		short           s[128];
		char            c[256];
	}               tmp;

	union temp_long {
		long            l;
		short           s[2];
	}               conv;


	fp = fopen (fname, "rb");
	if (fp == NULL)
		return (ERROR);
	if (fread (tmp.s, 1, 256, fp) != 256) {
		(void) fprintf (stderr, "ERROR [gethdr]: reading %s\n", fname);
		(void) fclose (fp);
		return (ERROR);
	}
	(void) fclose (fp);

	/* Strings   */
	for (i = 0; i < 14; i++)
		hdr->scanner[i] = tmp.c[i];
	for (i = 0; i < 8; i++)
		hdr->scanname[i] = tmp.c[14 + i];
	for (i = 0; i < 8; i++)
		hdr->scandate[i] = tmp.c[22 + i];
	for (i = 0; i < 10; i++)
		hdr->compound[i] = tmp.c[34 + i];
	for (i = 0; i < 12; i++)
		hdr->filter[i] = tmp.c[44 + i];
	for (i = 0; i < 8; i++)
		hdr->procdate[i] = tmp.c[60 + i];
	for (i = 0; i < 4; i++)
		hdr->initials[i] = tmp.c[68 + i];
	for (i = 0; i < 8; i++)
		hdr->piename[i] = tmp.c[76 + i];

	/* Shorts    */
	/* CONSTCOND  */
	if (__BYTE_ORDER == __LITTLE_ENDIAN)
		swap_byte (tmp.c, nbytes);

	hdr->slices = tmp.s[15];
	hdr->scantime = tmp.s[16];
	hdr->rcontype = tmp.s[28];
	hdr->resolution = tmp.s[29];
	hdr->ntype = tmp.s[36];
	hdr->unused_1 = tmp.s[37];
	for (i = 0; i < 26; i++)
		hdr->unused_2[i] = tmp.s[102 + i];

	/* Longs and Floats	*/
	/* CONSTCOND */
	if (__BYTE_ORDER == __LITTLE_ENDIAN)
		j = 1;
	else
		j = 0;

	conv.s[0 + j] = tmp.s[42];
	conv.s[1 - j] = tmp.s[43];
	hdr->totalcnts = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[44];
	conv.s[1 - j] = tmp.s[45];
	hdr->scancnts = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[46];
	conv.s[1 - j] = tmp.s[47];
	hdr->scanst = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[48];
	conv.s[1 - j] = tmp.s[49];
	hdr->scanlen = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[50];
	conv.s[1 - j] = tmp.s[51];
	hdr->framelen = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[52];
	conv.s[1 - j] = tmp.s[53];
	hdr->tau = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[54];
	conv.s[1 - j] = tmp.s[55];
	hdr->pettcnv1 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[56];
	conv.s[1 - j] = tmp.s[57];
	hdr->aflow1 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[58];
	conv.s[1 - j] = tmp.s[59];
	hdr->bflow1 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[60];
	conv.s[1 - j] = tmp.s[61];
	hdr->bvfactor1 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[62];
	conv.s[1 - j] = tmp.s[63];
	hdr->aoxygen1 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[64];
	conv.s[1 - j] = tmp.s[65];
	hdr->boxygen1 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[66];
	conv.s[1 - j] = tmp.s[67];
	hdr->awater1 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[68];
	conv.s[1 - j] = tmp.s[69];
	hdr->bwater1 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[70];
	conv.s[1 - j] = tmp.s[71];
	hdr->o2cnts1 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[72];
	conv.s[1 - j] = tmp.s[73];
	hdr->oxycont1 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[74];
	conv.s[1 - j] = tmp.s[75];
	hdr->decay_factor1 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[76];
	conv.s[1 - j] = tmp.s[77];
	hdr->pettcnv2 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[78];
	conv.s[1 - j] = tmp.s[79];
	hdr->aflow2 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[80];
	conv.s[1 - j] = tmp.s[81];
	hdr->bflow2 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[82];
	conv.s[1 - j] = tmp.s[83];
	hdr->bvfactor2 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[84];
	conv.s[1 - j] = tmp.s[85];
	hdr->aoxygen2 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[86];
	conv.s[1 - j] = tmp.s[87];
	hdr->boxygen2 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[88];
	conv.s[1 - j] = tmp.s[89];
	hdr->awater2 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[90];
	conv.s[1 - j] = tmp.s[91];
	hdr->bwater2 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[92];
	conv.s[1 - j] = tmp.s[93];
	hdr->o2cnts2 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[94];
	conv.s[1 - j] = tmp.s[95];
	hdr->oxycont2 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[96];
	conv.s[1 - j] = tmp.s[97];
	hdr->decay_factor2 = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[98];
	conv.s[1 - j] = tmp.s[99];
	hdr->pieslope = pe2ieee (conv.l);

	conv.s[0 + j] = tmp.s[100];
	conv.s[1 - j] = tmp.s[101];
	hdr->efactor = pe2ieee (conv.l);

	return (OK);
}
