/*$Id: mat_dir_lookup.c,v 1.2 1995/01/30 17:04:02 ty7777 Exp $*/
/*$Log: mat_dir_lookup.c,v $
 * Revision 1.2  1995/01/30  17:04:02  ty7777
 * Added program version.
 **/

/* @(#)mat_dir_lookup.c	1.1 11/19/90 */

#include "matrix.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/mat_dir_lookup.c,v 1.2 1995/01/30 17:04:02 ty7777 Exp $";

mat_dir_lookup( dir, matnum, entry)
  Mat_dir dir;
  int matnum;
  struct MatDir *entry;
{
	int i;

	for (i=0; i<dir->nmats; i++)
	{
	  if (matnum != dir->entry[i].matnum) continue;
	  entry->matnum = dir->entry[i].matnum;
	  entry->strtblk = dir->entry[i].strtblk;
	  entry->endblk = dir->entry[i].endblk;
	  entry->matstat = dir->entry[i].matstat;
	  return entry->strtblk;
	}
	return -1;
}
