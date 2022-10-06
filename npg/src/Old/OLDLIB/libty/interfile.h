/*$Id: interfile.h,v 1.1 1995/10/16 17:20:53 ty7777 Exp $*/
/*$Log: interfile.h,v $
 * Revision 1.1  1995/10/16  17:20:53  ty7777
 * Initial revision
 **/

#ifndef interfile_h
#define interfile_h

#include "matrix.h"
#ifdef __alpha__
extern int is_interfile(const char*);
extern int interfile_open(MatrixFile*);
#else
extern int is_interfile();
extern int interfile_open();
#endif
#endif
