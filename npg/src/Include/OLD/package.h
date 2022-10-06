/*$Id: package.h,v 1.2 1995/12/22 21:34:05 tom Exp $*/
/*$Log: package.h,v $
 * Revision 1.2  1995/12/22  21:34:05  tom
 * lint
 *
 * Revision 1.1  1995/10/30  18:20:37  tom
 * Initial revision
 *
 * Revision 1.3  1994/03/03  20:55:54  ty7777
 * Replace id with header.
 *
 * Revision 1.2  1993/11/11  17:58:40  ty7777
 * Change : to ;.
 *
 * Revision 1.1  1993/11/11  17:51:00  ty7777
 * Initial revision
 **/

#ifndef PACKAGE_INCLUDED
#define PACKAGE_INCLUDED

#ifndef lint
static char rcsid_package [] = "$Header: /home/npggw/tom/include/RCS/package.h,v 1.2 1995/12/22 21:34:05 tom Exp $";
#endif

#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "fcntl.h"
#include "varargs.h"
#include "errno.h"
#include "string.h"

/* Version Info */

#define PACKAGE_VERSION 1.1

/* Standard Types */
typedef char      		BOOLEAN;	/* boolean variables */
typedef unsigned short int	WORD;		/* 16 bit unsigned type */
typedef unsigned long int	LONGWORD;	/* 32 bit unsigned type */
typedef char			*STRING;	/* character pointer */

/* Scope control psuedo-keywords */
#define PUBLIC			extern 	/* available outside declared module */
#define PRIVATE			static	/* specific to declared module */

/* Standard constants */
#define TRUE 			1	/* for use with booleans and ifs */
#define FALSE 			0
#define SUCCEED 		0	/* procedure return tests */
#define FAIL 			-1
#define EOL			'\n'	/* end of line character */
#define EOS			'\0'	/* end of string character */
#define EOP			'\14'	/* end of page character (FF) */
#define MAXLINE			512	/* maximum number of chars in a line */

/* Standard Macros */

#define HIBYTE(x)	(((x) >> 8) & 0xff)  	/* hi byte of WORD*/
#define LOBYTE(x)	(((x) & 0xff))  	/* lo byte of WORD */
#define HIWORD(x)	(((x) >> 16) & 0xffffL)	/* hi word of LONGWORD */
#define LOWORD(x)	(((x) & 0xffffL)) 	/* lo word of LONGWORD */

#define pkg_min(A,B)	((A) < (B) ? (A) : (B))
#define pkg_max(A,B)	((A) > (B) ? (A) : (B))
#define pkg_abs(A)	((A) >= 0 ? (A) : -(A))

/* Error Routines */

/* Message Label Types */

#define PKG_STREAM	stderr

typedef enum {
	PKG_ERROR=0,
	PKG_WARNING=1,
	PKG_MESSAGE=2,
	PKG_BLANK=3
} PKG_LABEL;

static char	*pkg_message_labels[] = {
		"ERROR:    ",
		"WARNING:  ",
		"MESSAGE:  ",
		"          " };

PUBLIC void	pkg_message(/* va_alist */);
PUBLIC char	*pkg_malloc ();
PUBLIC void	pkg_read ();

/* Prototypes 

	pkg_message(type,program,function,call,format [,arg] ...)

	int	type; 		// label type (i.e.,PKG_ERROR, etc.);
	char	*program;	// program name 
	char	*function;	// function name
	char	*call;		// system call name (if any) will label perror
	char	*format;	// format string (like in printf)

		arg....		// arguments corresponding to format string
				// again like printf function

*/

#endif /* PACKAGE_INCLUDED */
