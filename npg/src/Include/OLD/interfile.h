#ifndef interfile_h
#define interfile_h

static char rcsid_interfile [] = "$Header: /export/home/npggw/tom/include/RCS/interfile.h,v 1.1 1995/10/30 18:25:01 tom Exp $";

#include "matrix7.h"
#ifdef __alpha__
extern int is_interfile(const char*);
extern int interfile_open(MatrixFile*);
#else
extern int is_interfile();
extern int interfile_open();
#endif
#endif
