/*$Id: plandefs.h,v 1.1 1995/10/16 17:24:18 ty7777 Exp $*/
/*$Log: plandefs.h,v $
 * Revision 1.1  1995/10/16  17:24:18  ty7777
 * Initial revision
 **/

/* prevent recursive definition */
#ifndef		plandefs_h
#define		plandefs_h

#ifdef __cplusplus
extern "C" int plandefs(int rmax,int span,int amax,int* vplane)
#else /* __cplusplus */
extern plandefs();
#endif /* __cplusplus */
#endif	/* 	plandefs_h */
