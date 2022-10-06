/*$Id: xbp.c,v 1.2 1995/01/30 20:16:01 ty7777 Exp $*/
/*$Log: xbp.c,v $
 * Revision 1.2  1995/01/30  20:16:01  ty7777
 * Added program version.
 **/

/*  emulation of xbp  --  backprojection  */
#include "qcemulib.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/xbp.c,v 1.2 1995/01/30 20:16:01 ty7777 Exp $";

#ifdef  EMUVERSION
	void  emu_xbp_ ( image,   ra,   proj,   nproj,  size)
#else
	void      xbp_ ( image,   ra,   proj,   nproj,  size)
#endif /* EMUVERSION */
 

  float                  image[], ra[], proj[];
  long                                         *nproj, *size;

                            /*  nproj not used; here for compatibility
                                    with microcode version, only      */
{
	long x, y, j;		/* indices = x,y;   j = int part of r */
	float r0, rdx, rdy;	/* three elements of ra[] */
	float r, w, temp;	/* w = frac part of r; temp = float(j) */
        float one = 1.0;
	void xsfl4_();

	r0  = ra[0];
	rdx = ra[1];
	rdy = ra[2];

	for (y=0; y<*size; y++)
	{
	    r = r0;
	    for (x=0; x<*size; x++)
	    {
		j = (long) r;
                temp = (float) j;
		w = r-temp;
		*image = *image + (one-w)*proj[j] + w*proj[j+1];
                image++; 
		r += rdx;
	    }
	    r0 += rdy;
	}
}


/*********************************************************************/

