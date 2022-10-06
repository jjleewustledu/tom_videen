/* $Log: writehdr.c,v $
 * Revision 3.0  2003/08/21  15:16:40  tom
 * 21Aug2003
 *
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
 * changed $Id: writehdr.c,v 3.0 2003/08/21 15:16:40 tom Exp $ to $Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/writehdr.c,v 3.0 2003/08/21 15:16:40 tom Exp $
 * */
/*	========================================================================
 *	Module:			writehdr.c
 *	Date:			Mar 2003
 *	Author:			Tom Videen
 *	Description:	Write an hdr file
 *	Algorithm:
 *	1)  Copy the hdr structure to OS32_header structure (tmp).
 *		All real numbers must be converted from IEEE format to OS32 format.
 *	2)  Add or change the following to the header:
 *			outfile -   name of output file
 *			uid     -   user's id
 *			today   -   date
 *			ntype   -   code number for type of processing
 *			nslices -   number of slces
 *	3)  Write new structure to output file 
 *	========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/writehdr.c,v 3.0 2003/08/21 15:16:40 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>
#include <libimage.h>
#include <imgutil.h>
#include <endian.h>

int             writehdr (hdr, outfile, uid, today, processing, nslices)
	HEADER_DATA    *hdr;
	char           *outfile;
	char           *uid;
	char           *today;
	int             processing;
	short           nslices;
{
    union temp_short {
        short           s[128];
        char            c[256];
    }               tmp;

    union temp_long {
        long            l;
        short           s[2];
    }               conv;

    union temp_zero {
        long            l;
        short           s[2];
    }               zero;

	FILE           *fp;
	char            fname[MAXFNAME];
	char           *ext;
	float           factor;
	int				i, j;
    long            nbytes = 256;
	float           fzero = 0.;

	zero.l = ieee2pe (&fzero);

	(void) strcpy (fname, outfile);
	ext = strrchr (fname, '.');
	(void) strcpy (ext, ".hdr");

	if ((fp = fopen (fname, "w")) == (FILE *) NULL) {
		fprintf (stderr, "ERROR [writehdr]: opening %s\n", fname);
		return (ERROR);
	}

	/* Shorts	*/

	tmp.s[15] = nslices;
    tmp.s[16] = hdr->scantime;
    tmp.s[28] = hdr->rcontype;
    tmp.s[29] = hdr->resolution;
    tmp.s[36] = hdr->ntype;
    tmp.s[37] = hdr->unused_1;
    for (i = 0; i < 26; i++)
        tmp.s[102 + i] = hdr->unused_2[i];

    /* Longs and Floats */
    /* CONSTCOND */
    if (__BYTE_ORDER == __LITTLE_ENDIAN)
        j = 1;
    else
        j = 0;

	conv.l = ieee2pe (&hdr->totalcnts);
    tmp.s[42] = conv.s[0 + j];
    tmp.s[43] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->scancnts);
    tmp.s[44] = conv.s[0 + j];
    tmp.s[45] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->scanst);
    tmp.s[46] = conv.s[0 + j];
    tmp.s[47] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->scanlen);
    tmp.s[48] = conv.s[0 + j];
    tmp.s[49] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->framelen);
    tmp.s[50] = conv.s[0 + j];
    tmp.s[51] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->tau);
    tmp.s[52] = conv.s[0 + j];
    tmp.s[53] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->pettcnv1);
    tmp.s[54] = conv.s[0 + j];
    tmp.s[55] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->aflow1);
    tmp.s[56] = conv.s[0 + j];
    tmp.s[57] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->bflow1);
    tmp.s[58] = conv.s[0 + j];
    tmp.s[59] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->bvfactor1);
    tmp.s[60] = conv.s[0 + j];
    tmp.s[61] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->aoxygen1);
    tmp.s[62] = conv.s[0 + j];
    tmp.s[63] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->boxygen1);
    tmp.s[64] = conv.s[0 + j];
    tmp.s[65] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->awater1);
    tmp.s[66] = conv.s[0 + j];
    tmp.s[67] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->bwater1);
    tmp.s[68] = conv.s[0 + j];
    tmp.s[69] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->o2cnts1);
    tmp.s[70] = conv.s[0 + j];
    tmp.s[71] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->oxycont1);
    tmp.s[72] = conv.s[0 + j];
    tmp.s[73] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->decay_factor1);
    tmp.s[74] = conv.s[0 + j];
    tmp.s[75] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->pettcnv2);
    tmp.s[76] = conv.s[0 + j];
    tmp.s[77] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->aflow2);
    tmp.s[78] = conv.s[0 + j];
    tmp.s[79] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->bflow2);
    tmp.s[80] = conv.s[0 + j];
    tmp.s[81] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->bvfactor2);
    tmp.s[82] = conv.s[0 + j];
    tmp.s[83] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->aoxygen2);
    tmp.s[84] = conv.s[0 + j];
    tmp.s[85] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->boxygen2);
    tmp.s[86] = conv.s[0 + j];
    tmp.s[87] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->awater2);
    tmp.s[88] = conv.s[0 + j];
    tmp.s[89] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->bwater2);
    tmp.s[90] = conv.s[0 + j];
    tmp.s[91] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->o2cnts2);
    tmp.s[92] = conv.s[0 + j];
    tmp.s[93] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->oxycont2);
    tmp.s[94] = conv.s[0 + j];
    tmp.s[95] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->decay_factor2);
    tmp.s[96] = conv.s[0 + j];
    tmp.s[97] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->pieslope);
    tmp.s[98] = conv.s[0 + j];
    tmp.s[99] = conv.s[1 - j];

	conv.l = ieee2pe (&hdr->efactor);
    tmp.s[100] = conv.s[0 + j];
    tmp.s[101] = conv.s[1 - j];

	if (processing == PROC_OEF) {
		tmp.s[36] = 11;					/* ntype	*/
		factor = 1 / SCALE_OEF;
		conv.l = ieee2pe (&factor);
    	tmp.s[62] = conv.s[0 + j];		/* aoxygen1	*/
    	tmp.s[63] = conv.s[1 - j];
    	tmp.s[64] = zero.s[0 + j];		/* boxygen1	*/
    	tmp.s[65] = zero.s[1 - j];
    	tmp.s[66] = zero.s[0 + j];		/* awater1	*/
    	tmp.s[67] = zero.s[1 - j];
    	tmp.s[68] = zero.s[0 + j];		/* bwater1	*/
    	tmp.s[69] = zero.s[1 - j];
    	tmp.s[70] = zero.s[0 + j];		/* o2cnts1	*/
    	tmp.s[71] = zero.s[1 - j];
    	tmp.s[72] = zero.s[0 + j];		/* oxycont1	*/
    	tmp.s[73] = zero.s[1 - j];
		conv.l = ieee2pe (&hdr->pettcnv1);
    	tmp.s[54] = conv.s[0 + j];		/* pettcnv1	*/
    	tmp.s[55] = conv.s[1 - j];

	} else if (processing == PROC_CMRO2) {
		tmp.s[36] = 12;					/* ntype	*/
		factor = 1 / SCALE_CMRO2;
		conv.l = ieee2pe (&factor);
    	tmp.s[62] = conv.s[0 + j];		/* aoxygen1	*/
    	tmp.s[63] = conv.s[1 - j];
    	tmp.s[64] = zero.s[0 + j];		/* boxygen1	*/
    	tmp.s[65] = zero.s[1 - j];
    	tmp.s[66] = zero.s[0 + j];		/* awater1	*/
    	tmp.s[67] = zero.s[1 - j];
    	tmp.s[68] = zero.s[0 + j];		/* bwater1	*/
    	tmp.s[69] = zero.s[1 - j];
    	tmp.s[70] = zero.s[0 + j];		/* o2cnts1	*/
    	tmp.s[71] = zero.s[1 - j];
    	tmp.s[72] = zero.s[0 + j];		/* oxycont1	*/
    	tmp.s[73] = zero.s[1 - j];
		conv.l = ieee2pe (&hdr->pettcnv1);
    	tmp.s[54] = conv.s[0 + j];		/* pettcnv1	*/
    	tmp.s[55] = conv.s[1 - j];

	} else if (processing == PROC_CBF) {
		tmp.s[36] = 2;					/* ntype	*/
    	tmp.s[56] = zero.s[0 + j];		/* aflow1	*/
    	tmp.s[57] = zero.s[1 - j];
		factor = 1 / SCALE_CBF;
		conv.l = ieee2pe (&factor);
    	tmp.s[58] = conv.s[0 + j];		/* bflow1	*/
    	tmp.s[59] = conv.s[1 - j];
		conv.l = ieee2pe (&hdr->pettcnv1);
    	tmp.s[54] = conv.s[0 + j];		/* pettcnv1	*/
    	tmp.s[55] = conv.s[1 - j];

	} else if (processing == PROC_CBV) {
		tmp.s[36] = 3;					/* ntype	*/
		factor = 1 / SCALE_CBV;
		conv.l = ieee2pe (&factor);
    	tmp.s[60] = conv.s[0 + j];		/* bvfactor1	*/
    	tmp.s[61] = conv.s[1 - j];
		conv.l = ieee2pe (&hdr->pettcnv1);
    	tmp.s[54] = conv.s[0 + j];		/* pettcnv1	*/
    	tmp.s[55] = conv.s[1 - j];
	}

    /* CONSTCOND  */
    if (__BYTE_ORDER == __LITTLE_ENDIAN)
        swap_byte (tmp.c, nbytes);

	/* Strings	*/
    for (i = 0; i < 14; i++)
        tmp.c[i] = hdr->scanner[i];
    for (i = 0; i < 8; i++)
        tmp.c[14 + i] = hdr->scanname[i];
    for (i = 0; i < 8; i++)
        tmp.c[22 + i] = hdr->scandate[i];
    for (i = 0; i < 10; i++)
        tmp.c[34 + i] = hdr->compound[i];
    for (i = 0; i < 12; i++)
        tmp.c[44 + i] = hdr->filter[i];
    for (i = 0; i < 8; i++)
        tmp.c[60 + i] = today[i];
    for (i = 0; i < 4; i++)
        tmp.c[68 + i] = uid[i];
    for (i = 0; i < 8; i++)
        tmp.c[76 + i] = hdr->piename[i];

	if (fwrite (tmp.c, 1, nbytes, fp) != nbytes) {
		fprintf (stderr, "ERROR [writehdr]: writing %s\n", fname);
		return (ERROR);
	}
	(void) fclose (fp);
	return (OK);
}
