/*$Id: FillHeadPett6.c,v 1.5 1995/11/10 16:27:32 tom Exp $*/
/*$Log: FillHeadPett6.c,v $
 * Revision 1.5  1995/11/10  16:27:32  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.4  1994/11/07  22:49:02  ty7777
 * Modified the comments.
 *
 * Revision 1.3  1993/11/03  18:14:57  ty7777
 * Added rcsid.
 **/

/*___________________________________________________________________________________
	File Name:	FillHeadPett6.c

	Function Name:	void FillHeaderPett6 (PETT6HDR *pett6_header)

	Arguments:	pett6_header:	A PETI6HDR structure. See the header
					file "petutil.h" for details. The 
					application programmer should allocate
					the structure before calling the function.

	Return Value:	void

	Description:	Fill a PETT6 header with default parameters.

	Calling:	none

	Author:		Tom Yang

	Date:		04/08/1992
___________________________________________________________________________________*/

#include <stdio.h>
#include <time.h>
#include <petutil.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libnpg/RCS/FillHeadPett6.c,v 1.5 1995/11/10 16:27:32 tom Exp $";

void FillHeaderPett6 (pett6_header)
PETT6HDR	*pett6_header;
{
	int		i;
	time_t		current_time, time1;
	struct tm	*tp;
	char		date [MAXLINE];

	/*
	 * "**" in standard files.
	 */
	for (i = 0; i < PETT6_FILE_FLAG_LEN; i++)
		pett6_header->file_flag [i]	= '*';
	pett6_header->wobble_rate	= 1;
	pett6_header->nwobble_pts	= 16;


	current_time	= time (&time1);
	tp		= localtime (&current_time); 
	if (strftime (date, MAXLINE, "%m%d%y", tp) > 0)
	{
		for (i = 0; i < PETT6_DATE_LEN; i++)
			pett6_header->date [i]	= date [i];
	}

	pett6_header->isotope_code	= 1;
	pett6_header->slice_number	= 1;
	pett6_header->unused_1 [0]	= 1;
	pett6_header->unused_1 [1]	= 1;
	pett6_header->number_of_slices	= 1;
	pett6_header->file_type		= 900;

	/*
	 * For PETT6 images, this number is always 6.
	 */
	pett6_header->pett_number	= PETT6_NUMBER;
}
