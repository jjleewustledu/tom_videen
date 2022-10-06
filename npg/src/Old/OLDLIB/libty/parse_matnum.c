/*  $Id$
    $Log$
 */
/* =============================================================================
 *  Module:         parse_matnum.c
 *  Date:           2.1   2/24/92
 *  Author:         Will Tribbey (CTI)
 *  Description:    
 *					parse_matnum takes the input string buff
 *					and parses it into a list of integers, allocates
 *			        space for the list and returns the pointer to the list.
 *			
 *		input		buff is a string containing the a specification
 *					for a list of integers, num_in_header is the number of
 *					frames, planes, gates, beds in the main header, and
 *					bedflag indicates if the number of bed postions is to
 *					be parsed.
 *			
 *		output      On successful exit, n points to the number of
 *					integers in the array of integers pointed to by
 *					the returned value of parse_matnum. If an error
 *					occurs a NULL pointer is returned. The pointer
 *					returned by parse_matnum should be free'd
 *					after use.
 *			
 *		syntax		the syntax for buff is a string consisting of
 *					numbers separated by spaces and ranges of numbers
 *	                are indicated by using '-' or a '*' can be used to
 *					indicate that the header information is to be used.
 *			
 *		example		list = parse_matnum("1 2 3-10 11 15-20", main.num_frames, 0, &n)
 *
 *  History:
 *      Nov-95   Modified TOV
 * =============================================================================
 */

#include <matrix.h>

static char     rcsid[] = "$Header: /export/home/npggw/tom/src/libecat7/RCS/parse_matnum.c,v 1.1 1995/10/09 16:50:33 ty7777 Exp $";

int            *parse_matnum (buff, num_in_header, bedflag, n)
	char           *buff;
	int             num_in_header, bedflag, *n;
{
	char           *p;
	int             ctr = 0, num1, num2, *list, i, extract_num (), add, numcomp ();

/*
 * Check for NULL string
 */
	p = &buff[0];

	if (*p == '\0')
		return ((int *) 0);

/*
 * If entire range is to generated make the list and return
 */
	if (*p == '*') {
		if (bedflag) {
			ctr = num_in_header;
			list = (int *) malloc ((ctr + 1) * sizeof (int));
			if (!list)
				return ((int *) 0);
			for (i = 0; i < ctr; i++)
				list[i] = i;
			*n = ctr;
		} else {
			ctr = (num_in_header > 0) ? num_in_header : 1;
			list = (int *) malloc (ctr * sizeof (int));
			if (!list)
				return ((int *) 0);
			for (i = 0; i < ctr; i++)
				list[i] = i + 1;
			*n = ctr;
		}
		return (list);
	}

/*
 * Check syntax
 */
	while (*p != '\0') {
		if ((!isdigit (*p)) && (*p != ' ') && (*p != '-'))
			return ((int *) 0);
		p++;
	}

/*
 * Find length of list
 */
	p = &buff[0];

	while (*p != '\0') {
		if (isdigit (*p)) {
			num1 = extract_num (p, &add);
			ctr++;
			p += add;
		} else if (*p == '-') {
			p++;
			if (!isdigit (*p))
				return ((int *) 0);
			num2 = extract_num (p, &add);
			if (num2 <= num1)
				return ((int *) 0);
			ctr += num2 - num1;
			p += add;
			if ((*p != ' ') && (*p != '\0'))
				return ((int *) 0);
		} else
			p++;
	}

/*
 * Allocate memory for list
 */
	list = (int *) malloc (ctr * sizeof (int));
	if (!list)
		return ((int *) 0);

/*
 * Fill in list
 */
	p = &buff[0];
	*n = ctr;
	ctr = 0;

	while (*p != '\0') {
		if (isdigit (*p)) {
			num1 = extract_num (p, &add);
			list[ctr] = num1;
			ctr++;
			p += add;
		} else if (*p == '-') {
			p++;
			num2 = extract_num (p, &add);
			for (i = num1 + 1; i <= num2; i++) {
				list[ctr] = i;
				ctr++;
			}
			p += add;
		} else
			p++;
	}

/*
 * Sort list
 */
	qsort ((char *) list, *n, sizeof (int), numcomp);

	return (list);
}
