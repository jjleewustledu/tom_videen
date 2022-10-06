/*  $Id: numcode7.c,v 1.2 1999/12/13 17:45:50 tom Exp $
    $Log: numcode7.c,v $
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		numcode7
 *  Date:           Nov-95
 *  Author:         John Ollinger
 *	Description:	Encode a matrix number from frame, slice and phase numbers.
 *	History:
 *		Modified Mar-96	TOV	Changed name and order of parameters to CTI standard.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/numcode7.c,v 1.2 1999/12/13 17:45:50 tom Exp $";
#endif

int             numcode7 (frame, plane, gate, data, bed)
	int             frame, plane, gate, data, bed;
{
	int             matnum8data16bed64gate1024plane512frame;
	int             loPlane, loData;
	int             hiPlane, hiData;

	hiPlane = (plane & 0x300);
	loPlane = (plane & 0xFF);
	loData = (data & 0x3);
	hiData = (data & 0x4);
	matnum8data16bed64gate1024plane512frame = ((frame & 0x1FF) | ((bed & 0xF) << 12)
						   | ((loPlane << 16) | (hiPlane << 1)) | ((gate & 0x3F) << 24) | ((loData << 30) | (hiData << 9)));

	return (matnum8data16bed64gate1024plane512frame);
}
