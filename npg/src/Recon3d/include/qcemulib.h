/*$Id: qcemulib.h,v 1.2 1995/01/30 20:09:29 ty7777 Exp $*/
/*$Log: qcemulib.h,v $
 * Revision 1.2  1995/01/30  20:09:29  ty7777
 * Added program version.
 **/

#include  <math.h>

typedef struct {float r, i;} complex;

#define   cmov(a,b)     {(a).r = (b).r; (a).i = (b).i;}

#define   cadd(a,b,c)   {(a).r = (b).r + (c).r; (a).i = (b).i + (c).i;}

#define   csub(a,b,c)   {(a).r = (b).r - (c).r; (a).i = (b).i - (c).i;}

#define   cmul(a,b,c)   {complex tt; \
		tt.r = (float)((b).r * (c).r) - (float)((b).i * (c).i); \
		tt.i = (float)((b).r * (c).i) + (float)((b).i * (c).r); \
			(a).r = tt.r; (a).i = tt.i;}

#define   conjg(a,b)    {(a).r = (b).r; (a).i = -(b).i;}
#define   rcmul(a,b,c)  {(a).r = b * (c).r; (a).i = b * (c).i;}

static char rcsid_qcemulib [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/qcemulib.h,v 1.2 1995/01/30 20:09:29 ty7777 Exp $";

/*********************************************************************/
