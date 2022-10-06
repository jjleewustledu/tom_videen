/*$Id: matnum_3d.c,v 1.2 1995/01/30 17:04:35 ty7777 Exp $*/
/*$Log: matnum_3d.c,v $
 * Revision 1.2  1995/01/30  17:04:35  ty7777
 * Added program version.
 **/

/* @(#)matnum_3d.c	1.1 1/31/91 */

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/matnum_3d.c,v 1.2 1995/01/30 17:04:35 ty7777 Exp $";

int matnum_3d( ring_a, ring_b)
  int ring_a, ring_b;
{
	int frame, plane;

	frame = 1+((ring_b&0x8)>>2)+((ring_a&0x8)>>3);
	plane = 1+((ring_a&0x7)<<3)+(ring_b&0x7);
	return mat_numcod( frame, plane, 1, 0, 0);
}
