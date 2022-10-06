/*$Id: xvrmp.c,v 1.2 1995/01/30 21:56:22 ty7777 Exp $*/
/*$Log: xvrmp.c,v $
 * Revision 1.2  1995/01/30  21:56:22  ty7777
 * Added program version.
 **/

#include "qcemulib.h"

static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/xvrmp.c,v 1.2 1995/01/30 21:56:22 ty7777 Exp $";

#ifdef  EMUVERSION
void  emu_xvrmp_ (y,a,b,c,ny)
#else
     void  xvrmp_ (y,a,b,c,ny)
#endif /* EMUVERSION */
     
     float   y[], *a, *b, *c;
     long    *ny;
{
  long  i;
  float temp1,temp2,temp3 ;

/*   note that the reason the emulation is so lengthy is that it 
     had to go thru the same steps as the primitives to avoid roundoff
     error in the chip */

	temp1 = *c + *c ;
	temp2 = *c + temp1 ; 
	temp3 = *c + temp2 ; 
	y[0] = *b ;
	y[1] = *b + *c ;
	y[2] = *b + temp1;
	y[3] = *b + temp2;
	for (i=4; i < *ny - 4 ; i=i+4)
	{
	   y[i] = y[i-4] + temp3;
	   y[i+1] = y[i-3] + temp3;
	   y[i+2] = y[i-2] + temp3;
	   y[i+3] = y[i-1] + temp3;
	};
/*     handle remainder */
	for (/* i is set */; i < *ny; i++ )
	   y[i] = y[i-4] + temp3 ;
	*a = (y[*ny-1] + *c) ;
}

/*********************************************************************/

