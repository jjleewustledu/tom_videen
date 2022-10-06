
/*$Id: gethdr.c,v 1.5 1994/03/30 19:11:49 ty7777 Exp $*/
/*$Log: gethdr.c,v $
 * Revision 1.5  1994/03/30  19:11:49  ty7777
 * Delted the error message.
 *
 * Revision 1.4  1994/03/29  17:37:38  ty7777
 * Changed the error message.
 *
 * Revision 1.3  1993/11/03  17:57:11  ty7777
 * Added rcsid.
 *
 * Revision 1.2  1993/09/13  17:52:11  ty7777
 * Chage Error to Warning.
 *
 * Revision 1.1  1993/04/30  14:02:59  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		gethdr.c

  Function:	int gethdr (char *filename, HEADER_DATA *header). 

  Arguments:	
		filename,	PETT VI header file name, passed in.
		HEADER_DATA,	header containing all the fields of a PETT VI header file,
				returned.

  Description:	This function reads a PETT VI header file and fills the HEADER_DATA 
		structure.
  Return:	SUCCEED for successful; FAIL for error.

  History:	 Created by Tom Yang on 04/29/92. 
___________________________________________________________________________*/

#include <petutil/petutil.h>

static char rcsid [] = "$Header: /home/petsun4/ty7777/src/libnpg/RCS/gethdr.c,v 1.5 1994/03/30 19:11:49 ty7777 Exp $";

PUBLIC int gethdr (filename, header)
	char		*filename;
	HEADER_DATA	*header;
{
	FILE			*hdrfp;
	OS32_HEADER_DATA	*os32_header;

	if ((hdrfp = fopen (filename, "rb")) == (FILE *) NULL) 
	{
		return FAIL;
	}

	os32_header	= (OS32_HEADER_DATA *) malloc (sizeof (OS32_HEADER_DATA));

	fread (os32_header, sizeof (OS32_HEADER_DATA), 1, hdrfp); 

	bcopy (os32_header, header, sizeof (HEADER_DATA));

	header->totalcnts	= pe2ieee (os32_header->totalcnts);
	header->scancnts	= pe2ieee (os32_header->scancnts);
	header->scanst		= pe2ieee (os32_header->scanst);
	header->scanlen		= pe2ieee (os32_header->scanlen);
	header->framelen	= pe2ieee (os32_header->framelen);
	header->tau		= pe2ieee (os32_header->tau);
	header->pettcnv1	= pe2ieee (os32_header->pettcnv1);
	header->aflow1		= pe2ieee (os32_header->aflow1);
	header->bflow1		= pe2ieee (os32_header->bflow1);
	header->bvfactor1	= pe2ieee (os32_header->bvfactor1);
	header->aoxygen1	= pe2ieee (os32_header->aoxygen1);
	header->boxygen1	= pe2ieee (os32_header->boxygen1);
	header->awater1		= pe2ieee (os32_header->awater1);
	header->bwater1		= pe2ieee (os32_header->bwater1);
	header->o2cnts1		= pe2ieee (os32_header->o2cnts1);
	header->oxycont1	= pe2ieee (os32_header->oxycont1);
	header->decay_factor1	= pe2ieee (os32_header->decay_factor1);
	header->pettcnv2	= pe2ieee (os32_header->pettcnv2);
	header->aflow2		= pe2ieee (os32_header->aflow2);
	header->bflow2		= pe2ieee (os32_header->bflow2);
	header->bvfactor2	= pe2ieee (os32_header->bvfactor2);
	header->aoxygen2	= pe2ieee (os32_header->aoxygen2);
	header->boxygen2	= pe2ieee (os32_header->boxygen2);
	header->awater2		= pe2ieee (os32_header->awater2);
	header->bwater2		= pe2ieee (os32_header->bwater2);
	header->o2cnts2		= pe2ieee (os32_header->o2cnts2);
	header->oxycont2	= pe2ieee (os32_header->oxycont2);
	header->decay_factor2	= pe2ieee (os32_header->decay_factor2);
	header->pieslope	= pe2ieee (os32_header->pieslope);
	header->efactor		= pe2ieee (os32_header->efactor);

	/* 
	 * Close the header file 
	 */
	fclose (hdrfp);
	free (os32_header);

	return SUCCEED;
}
