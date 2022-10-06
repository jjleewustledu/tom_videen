/*$Id: sincos.h,v 1.1 1995/10/16 17:26:57 ty7777 Exp $*/
/*$Log: sincos.h,v $
 * Revision 1.1  1995/10/16  17:26:57  ty7777
 * Initial revision
 **/

#ifndef sun

#ifndef sincos_h
#define sincos_h
#include <math.h>

static void sincos(theta, sintheta, costheta)
double theta, *sintheta, *costheta;
{
	double tmp = sin(theta);
	*sintheta = tmp;
	*costheta = sqrt(1 - tmp * tmp);
}
#endif /* sincos_h */

#endif /* sun */
