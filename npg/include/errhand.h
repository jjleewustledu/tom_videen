/*$Id: errhand.h,v 2.0 2004/02/12 20:30:50 tom Exp $*/
/*$Log: errhand.h,v $
 * Revision 2.0  2004/02/12  20:30:50  tom
 * sync Feb 2004
 *
 * Revision 1.2  1999/12/14  15:37:44  tom
 * tov revision
 *
 * Revision 1.1  1998/08/26  19:49:23  tom
 * Initial revision
 *
 * Revision 1.1  1996/06/10  08:51:06  jane
 * Initial revision
 **/
/*  ===========================================================================
 *  File:        errhand.h
 *  Date:        April-96
 *  Author:      Jane Dunford-Shore
 *  Description: Include file for liberrhand
 *  ===========================================================================
 */

#ifndef ERRHAND_H
#define ERRHAND_H

typedef enum {
    CMD_SYNTAX_ERR,
    MEM_ALLOC_ERR,
    FILE_CREATE_ERR,
    FILE_OPEN_ERR,
    FILE_READ_ERR,
    FILE_WRITE_ERR,
    CUSTOM
} ERRTYPE; 

/* Tags for variables that err_handler_init() can initialize */
typedef enum {
    PROGNAME,
    CMD_SYNTAX    
} ERRVAR;
  
#define ERRSTRLEN 500          /* length of output error string */ 
#define MAXSTR 256             /* max. length of program name */
#define MAXLEN_CMDSYN 1000     /* max. length of command syntax */


#if defined(__STDC__)
extern void err_handler_init (ERRVAR errvar, void *value);
extern void err_handler(ERRTYPE errortype, const char *string);
#else
extern void err_handler_init ();
extern void err_handler();
#endif  /* __STDC__ */

#endif	/* define ERRHAND_H */
