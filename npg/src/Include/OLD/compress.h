/*$Id: compress.h,v 1.1 1995/11/14 17:26:27 tom Exp $*/
/*$Log: compress.h,v $
 * Revision 1.1  1995/11/14  17:26:27  tom
 * Initial revision
 *
 * Revision 1.1  1995/10/16  17:46:24  ty7777
 * Initial revision
 **/

#ifndef compress_h
#define compress_h

#if defined(__cplusplus)
extern "C" int compress(char* data, int size, int type, char* z_data);
extern "C" int uncompress(char* z_data, int size, char* data, int type);
#else
extern int compress();
extern int uncompress();
#endif /* __cplusplus */

#endif /* compress_h */
