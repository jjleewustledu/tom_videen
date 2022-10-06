/*  $Id$
    $Log$
 */
/* =============================================================================
 *  Module:         numcomp.c
 *  Date:			2.1   2/24/92  Copyright 1989-1991 CTI PET Systems, Inc.
 *	Author:			Will Tribbey
 *  Description:    Comparison function for qsort used in make_list and parse_matnum.
 *					Returns the difference between the integer values pointed to by i and j.
 *  History:
 *      Nov-95   Modified TOV
 * =============================================================================
 */

static char     rcsid[] = "$Header$";

int             numcomp (i, j)
	int            *i, *j;

{
	return (*i - *j);
}
