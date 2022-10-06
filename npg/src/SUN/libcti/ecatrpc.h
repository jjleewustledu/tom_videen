/*$Id: ecatrpc.h,v 1.4 1999/12/13 22:02:19 tom Exp $*/
/*$Log: ecatrpc.h,v $
 *Revision 1.4  1999/12/13 22:02:19  tom
 *partially cleaned up with lint
 *
 * Revision 1.3  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.2  1994/03/11  19:49:09  ty7777
 * Added rcs headers.
 **/

#ifndef ECATRPC_INCLUDED
#define ECATRPC_INCLUDED

/*    @(#)ecatrpc.h	2.3 10/30/91    */
#define AUTOQUEPROGRAM 600000035
#define AUTOQUEVERSION 1

/*
#define MaxPlanes 	2 * defaults_get_integer ("/Ecat/EcatNumDirect", 16, 0) - 1
#define EcatModel 	defaults_get_integer ("/Ecat/EcatModel", 951, 0)
#define RtsType 	defaults_get_string ("/Ecat/EcatRTSCpu", "Mizar", 0)
*/
#define MaxPlanes 	32
#define EcatModel 	951
#define RtsType 	"Mizar"

#include "ecatRts.h"
#include "ecatMcs.h"
#include "ecatRecon.h"
#include "ecatAcs.h"
#include "ecatRfa.h"
#include "ecatArc.h"
#include "reconRpc.h"

#ifndef lint
static char     rcsid_ecatrpc[] = "$Header: /home/npggw/tom/src/libcti/RCS/ecatrpc.h,v 1.4 1999/12/13 22:02:19 tom Exp $";
#endif

#endif						 /* ECATRPC_INCLUDED */
