/*$Id: stringfns.h,v 2.0 2004/02/12 20:30:50 tom Exp $*/
/*$Log: stringfns.h,v $
 * Revision 2.0  2004/02/12  20:30:50  tom
 * sync Feb 2004
 *
 * Revision 1.2  1999/12/14  15:39:54  tom
 * add _STDC_ condition
 *
 * Revision 1.1  1998/08/26  19:49:36  tom
 * Initial revision
 *
 * Revision 1.1  1996/06/10  08:41:52  jane
 * Initial revision
 **/
/*
 * File:   stringfns.h
 * Author: Jane Dunford-Shore
 * Date:   28-April-96
 * Description: include file for libstringfns, library of
 *     in-house string functions
 */

extern char *get_bin_str();     

#if defined(__STDC__)
extern int isalldigits(char *string);
#else
extern int isalldigits();
#endif  /* __STDC__ */

