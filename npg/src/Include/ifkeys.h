/*  $Log: ifkeys.h,v $
 * Revision 1.1  1996/04/04  14:05:36  tom
 * Initial revision
 *
    $Id: ifkeys.h,v 1.1 1996/04/04 14:05:36 tom Exp $
 */
/* =============================================================================
 *  Module:         ifkeys.h
 *  Date:           Nov-95
 *  Author:         John Ollinger
 *  Description:    Standard keys for Interfile header files.
 * =============================================================================
 */
#ifndef	lint
static char     if_rcsid[] = "$Header: /usr/local/npg/include/RCS/ifkeys.h,v 1.1 1996/04/04 14:05:36 tom Exp $";
#endif

#ifndef interfile_dot_h_defined
#include <interfile.h>
#endif

static char    *key[NUM_KEYS] =
	{"INTERFILE",
    "originating system",
    "number of dimensions",
    "program version",
    "matrix size [1]",
    "matrix size [2]",
    "matrix size [3]",
    "matrix size [4]",
    "scaling factor (mm/pixel) [1]",
    "scaling factor (mm/pixel) [2]",
    "scaling factor (mm/pixel) [3]",
    "number of bytes per pixel",
    "name of data file",
    "patient ID",
    "study date",
    "number format",
    "volume scale factor",
    "bed elevation",
    "number of bed positions",
    "initial bed position",
    "bed offset",
    "frame number",
    "frame starting time",
    "frame duration",
    "plane number",
    "storage scale factor",
    "number of trues",
    "number of randoms",
    "number of multiples",
    "deadtime correction factor",
	"user defined comment"};
