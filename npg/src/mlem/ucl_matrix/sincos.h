#if !defined(__sun) || defined(__SVR4)       /* solaris 2.x*/

#ifndef sincos_h
#define sincos_h
#include <math.h>

static void sincos(theta, sintheta, costheta)
double theta, *sintheta, *costheta;
{
	*sintheta = sin(theta);
	*costheta = cos(theta);
}
#endif /* sincos_h */

#endif /* sun */
