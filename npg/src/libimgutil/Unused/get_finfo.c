/*	$Id$
	$Log$ */
/* ===============================================================================
 *	Module:			get_finfo.c
 *	Date:			Dec-95
 *	Author:			Tom Yang
 *	Description:	Fills finfo using parameters defined in parameter_data.
 *					For use in conjunction with image archive.
 * ===============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header$";
#endif

#include <arc.h>
#include <imgutil.h>

int             get_finfo (parameter_data, finfo)
	PARAMETER_DATA  parameter_data;
	struct INFO_TYPE *finfo;
{
	finfo->cat = parameter_data.category;
	finfo->type = parameter_data.datatype;
	strcpy (finfo->supp.pet.scanner, parameter_data.scanner);
	strcpy (finfo->supp.pet.acq, parameter_data.acquisition);
	strcpy (finfo->supp.pet.recon, parameter_data.reconstruction);
	strcpy (finfo->supp.pet.fltr, parameter_data.recfilter);
	return (OK);
}
