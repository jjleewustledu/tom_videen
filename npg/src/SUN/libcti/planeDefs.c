/*$Id: planeDefs.c,v 1.5 1999/12/13 22:02:19 tom Exp $*/
/*$Log: planeDefs.c,v $
 *Revision 1.5  1999/12/13 22:02:19  tom
 *partially cleaned up with lint
 *
 * Revision 1.4  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.3  1994/03/03  17:36:00  ty7777
 * Change from id to header.
 *
 * Revision 1.2  1993/09/13  19:49:24  ty7777
 * Introduce rcsid.
 *
 * Revision 1.1  1993/03/04  19:56:19  ty7777
 * Initial revision
 **/

/* @(#)planeDefs.c	2.3 4/24/92 */

/* planeDefs.c 	twjansto	09 jun 1990
 *
 * this file is used to define plane combinations for various
 * machine types and resolutions
 *
 * added gantry commands to kick ipcs into or out of all coincidence modes
 *				25 sep 1990
 */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/planeDefs.c,v 1.5 1999/12/13 22:02:19 tom Exp $";
#endif

#include <stdio.h>
#include <string.h>

#define Ok 0
#define RtsTimeOutError -1
#define IpcTimeOutError -2
#define EcatModel 951
#define RtsType "Mizar"
#define NumberOfRings 2

int             all_planes ()
{
	int             status, numPlanesDefined;
	int             setupIpcBoard (), allAcsPlanes (), allAcs6Planes (), all931Planes (),
	                all951Planes ();

	if (strcmp (RtsType, "ACS") == 0) {
		switch (EcatModel) {
		case 921:
		case 961:
			numPlanesDefined = allAcs6Planes (NumberOfRings);
			status = setupIpcBoard (EcatModel, NumberOfRings, "g5");
			if (status == Ok)
				status = numPlanesDefined;
			return (status);
			break;
		default:
			numPlanesDefined = allAcsPlanes (NumberOfRings);
			status = setupIpcBoard (EcatModel, NumberOfRings, "g3");
			if (status == Ok)
				status = numPlanesDefined;
			return (status);
			break;
		}
	} else {
		switch (EcatModel) {
		case 931:
			numPlanesDefined = all931Planes (NumberOfRings);
			status = setupIpcBoard (EcatModel, NumberOfRings, "g3");
			if (status == Ok)
				status = numPlanesDefined;
			return (status);
			break;
		case 951:
			numPlanesDefined = all951Planes (NumberOfRings);
			status = setupIpcBoard (EcatModel, NumberOfRings, "g3");
			if (status == Ok)
				status = numPlanesDefined;
			return (status);
			break;
		}
	}
}

int             allPlanes (numRings)
	int             numRings;
{
	int             all_planes ();

	return (all_planes ());
}


int             straightPlanes (numRings)
	int             numRings;
{
	int             status, setupIpcBoard (), straightAcsPlanes (), all931Planes (), straight951Planes ();
	int             numPlanesDefined, straightAcs6Planes ();

	if (strcmp (RtsType, "ACS") == 0) {
		switch (EcatModel) {
		case 921:
		case 961:
			numPlanesDefined = straightAcs6Planes (NumberOfRings);
			status = setupIpcBoard (EcatModel, NumberOfRings, "g0");
			if (status == Ok)
				status = numPlanesDefined;
			return (status);
			break;
		default:
			numPlanesDefined = straightAcsPlanes (NumberOfRings);
			status = setupIpcBoard (EcatModel, NumberOfRings, "g3");
			if (status == Ok)
				status = numPlanesDefined;
			return (status);
			break;
		}
	} else {
		switch (EcatModel) {
		case 931:
			numPlanesDefined = all931Planes (NumberOfRings);
			status = setupIpcBoard (EcatModel, NumberOfRings, "g3");
			if (status == Ok)
				status = numPlanesDefined;
			return (status);
			break;
		case 951:
			numPlanesDefined = straight951Planes (NumberOfRings);
			status = setupIpcBoard (EcatModel, NumberOfRings, "g3");
			if (status == Ok)
				status = numPlanesDefined;
			return (status);
			break;
		}
	}
}

int             realPlanes (numRings)
	int             numRings;
{
	int             status, numPlanesDefined;
	int             fourPlanesFor921 (), setupIpcBoard (), realAcsPlanes (), all931Planes (),
	                real951Planes ();

	if (strcmp (RtsType, "ACS") == 0) {
		switch (EcatModel) {
		case 921:
		case 961:
			numPlanesDefined = fourPlanesFor921 (numRings);
			status = setupIpcBoard (EcatModel, NumberOfRings, "g5");
			if (status == Ok)
				status = numPlanesDefined;
			return (status);
			break;
		default:
			return (realAcsPlanes (NumberOfRings));
		}
	} else {
		switch (EcatModel) {
		case 931:
			numPlanesDefined = all931Planes (NumberOfRings);
			status = setupIpcBoard (EcatModel, NumberOfRings, "g3");
			if (status == Ok)
				status = numPlanesDefined;
			return (status);
			break;
		case 951:
			numPlanesDefined = real951Planes (NumberOfRings);
			status = setupIpcBoard (EcatModel, NumberOfRings, "g3");
			if (status == Ok)
				status = numPlanesDefined;
			return (status);
			break;
		}
	}
}

int             directPlanes (numRings)
	int             numRings;
{
	int             status, numPlanesDefined, setupIpcBoard (), directAcsPlanes (), all931Planes (),
	                direct951Planes ();

	if (strcmp (RtsType, "ACS") == 0)
		return (directAcsPlanes (NumberOfRings));
	else {
		switch (EcatModel) {
		case 931:
			numPlanesDefined = all931Planes (NumberOfRings);
			status = setupIpcBoard (EcatModel, NumberOfRings, "g3");
			if (status == Ok)
				status = numPlanesDefined;
			return (status);
			break;
		case 951:
			numPlanesDefined = direct951Planes (NumberOfRings);
			status = setupIpcBoard (EcatModel, NumberOfRings, "g3");
			if (status == Ok)
				status = numPlanesDefined;
			return (status);
			break;
		}
	}
}

int             planes931 (numberOfRings)
	int             numberOfRings;
{
	int             status, numPlanesDefined;
	int             setupIpcBoard (), lowEffHighResolutionAcsPlanes (), lowEffHighResolutionAcs6Planes ();

	if (!numberOfRings)
		numberOfRings = NumberOfRings;
	if (strcmp (RtsType, "ACS") == 0) {
		switch (EcatModel) {
		case 921:
		case 961:
			numPlanesDefined = lowEffHighResolutionAcs6Planes (numberOfRings);
			status = setupIpcBoard (EcatModel, NumberOfRings, "g2");
			if (status == Ok)
				status = numPlanesDefined;
			return (status);
			break;
		default:
			numPlanesDefined = lowEffHighResolutionAcsPlanes (numberOfRings);
			status = setupIpcBoard (EcatModel, NumberOfRings, "g2");
			if (status == Ok)
				status = numPlanesDefined;
			return (status);
			break;
		}
	}
}

int             all951Planes (numberOfRings)
	int             numberOfRings;
{
	rts_dpln (0, 0, -1, -1, -1, -1, -1);
	rts_dpln (1, 16, -1, -1, -1, -1, -1);
	rts_dpln (2, 1, 31, -1, -1, -1, -1);
	rts_dpln (3, 17, 45, -1, -1, -1, -1);
	rts_dpln (4, 2, 32, -1, -1, -1, -1);
	rts_dpln (5, 18, 46, -1, -1, -1, -1);
	rts_dpln (6, 3, 33, -1, -1, -1, -1);
	rts_dpln (7, 19, 47, -1, -1, -1, -1);
	rts_dpln (8, 4, 34, -1, -1, -1, -1);
	rts_dpln (9, 20, 48, -1, -1, -1, -1);
	rts_dpln (10, 5, 35, -1, -1, -1, -1);
	rts_dpln (11, 21, 49, -1, -1, -1, -1);
	rts_dpln (12, 6, 36, -1, -1, -1, -1);
	if (numberOfRings == 1) {
		rts_dpln (13, 22, -1, -1, -1, -1, -1);
		rts_dpln (14, 7, -1, -1, -1, -1, -1);
		return (15);
	} else if (numberOfRings == 2) {
		rts_dpln (13, 22, 50, -1, -1, -1, -1);
		rts_dpln (14, 7, 37, -1, -1, -1, -1);
		rts_dpln (15, 23, 51, -1, -1, -1, -1);
		rts_dpln (16, 8, 38, -1, -1, -1, -1);
		rts_dpln (17, 24, 52, -1, -1, -1, -1);
		rts_dpln (18, 9, 39, -1, -1, -1, -1);
		rts_dpln (19, 25, 53, -1, -1, -1, -1);
		rts_dpln (20, 10, 40, -1, -1, -1, -1);
		rts_dpln (21, 26, 54, -1, -1, -1, -1);
		rts_dpln (22, 11, 41, -1, -1, -1, -1);
		rts_dpln (23, 27, 55, -1, -1, -1, -1);
		rts_dpln (24, 12, 42, -1, -1, -1, -1);
		rts_dpln (25, 28, 56, -1, -1, -1, -1);
		rts_dpln (26, 13, 43, -1, -1, -1, -1);
		rts_dpln (27, 29, 57, -1, -1, -1, -1);
		rts_dpln (28, 14, 44, -1, -1, -1, -1);
		rts_dpln (29, 30, -1, -1, -1, -1, -1);
		rts_dpln (30, 15, -1, -1, -1, -1, -1);
		return (31);
	}
}

int             straight951Planes (numberOfRings)
	int             numberOfRings;
{
	rts_dpln (0, 0, -1, -1, -1, -1, -1);
	rts_dpln (1, 1, -1, -1, -1, -1, -1);
	rts_dpln (2, 2, -1, -1, -1, -1, -1);
	rts_dpln (3, 3, -1, -1, -1, -1, -1);
	rts_dpln (4, 4, -1, -1, -1, -1, -1);
	rts_dpln (5, 5, -1, -1, -1, -1, -1);
	rts_dpln (6, 6, -1, -1, -1, -1, -1);
	rts_dpln (7, 7, -1, -1, -1, -1, -1);
	if (numberOfRings == 1)
		return (8);
	else if (numberOfRings == 2) {
		rts_dpln (8, 8, -1, -1, -1, -1, -1);
		rts_dpln (9, 9, -1, -1, -1, -1, -1);
		rts_dpln (10, 10, -1, -1, -1, -1, -1);
		rts_dpln (11, 11, -1, -1, -1, -1, -1);
		rts_dpln (12, 12, -1, -1, -1, -1, -1);
		rts_dpln (13, 13, -1, -1, -1, -1, -1);
		rts_dpln (14, 14, -1, -1, -1, -1, -1);
		rts_dpln (15, 15, -1, -1, -1, -1, -1);
		return (16);
	}
}

int             real951Planes (numberOfRings)
	int             numberOfRings;
{
	rts_dpln (0, 0, -1, -1, -1, -1, -1);
	rts_dpln (1, 16, -1, -1, -1, -1, -1);
	rts_dpln (2, 1, -1, -1, -1, -1, -1);
	rts_dpln (3, 17, -1, -1, -1, -1, -1);
	rts_dpln (4, 2, -1, -1, -1, -1, -1);
	rts_dpln (5, 18, -1, -1, -1, -1, -1);
	rts_dpln (6, 3, -1, -1, -1, -1, -1);
	rts_dpln (7, 19, -1, -1, -1, -1, -1);
	rts_dpln (8, 4, -1, -1, -1, -1, -1);
	rts_dpln (9, 20, -1, -1, -1, -1, -1);
	rts_dpln (10, 5, -1, -1, -1, -1, -1);
	rts_dpln (11, 21, -1, -1, -1, -1, -1);
	rts_dpln (12, 6, -1, -1, -1, -1, -1);
	rts_dpln (13, 22, -1, -1, -1, -1, -1);
	rts_dpln (14, 7, -1, -1, -1, -1, -1);
	if (numberOfRings == 1)
		return (15);
	else if (numberOfRings == 2) {
		rts_dpln (15, 23, -1, -1, -1, -1, -1);
		rts_dpln (16, 8, -1, -1, -1, -1, -1);
		rts_dpln (17, 24, -1, -1, -1, -1, -1);
		rts_dpln (18, 9, -1, -1, -1, -1, -1);
		rts_dpln (19, 25, -1, -1, -1, -1, -1);
		rts_dpln (20, 10, -1, -1, -1, -1, -1);
		rts_dpln (21, 26, -1, -1, -1, -1, -1);
		rts_dpln (22, 11, -1, -1, -1, -1, -1);
		rts_dpln (23, 27, -1, -1, -1, -1, -1);
		rts_dpln (24, 12, -1, -1, -1, -1, -1);
		rts_dpln (25, 28, -1, -1, -1, -1, -1);
		rts_dpln (26, 13, -1, -1, -1, -1, -1);
		rts_dpln (27, 29, -1, -1, -1, -1, -1);
		rts_dpln (28, 14, -1, -1, -1, -1, -1);
		rts_dpln (29, 30, -1, -1, -1, -1, -1);
		rts_dpln (30, 15, -1, -1, -1, -1, -1);
		return (31);
	}
}

int             direct951Planes (numberOfRings)
	int             numberOfRings;
{
	rts_dpln (0, 0, -1, -1, -1, -1, -1);
	rts_dpln (1, 16, -1, -1, -1, -1, -1);
	rts_dpln (2, 1, 31, -1, -1, -1, -1);
	rts_dpln (3, 17, 45, -1, -1, -1, -1);
	rts_dpln (4, 2, 32, -1, -1, -1, -1);
	rts_dpln (5, 18, 46, -1, -1, -1, -1);
	rts_dpln (6, 3, 33, -1, -1, -1, -1);
	rts_dpln (7, 19, 47, -1, -1, -1, -1);
	rts_dpln (8, 4, 34, -1, -1, -1, -1);
	rts_dpln (9, 20, 48, -1, -1, -1, -1);
	rts_dpln (10, 5, 35, -1, -1, -1, -1);
	rts_dpln (11, 21, 49, -1, -1, -1, -1);
	rts_dpln (12, 6, 36, -1, -1, -1, -1);
	rts_dpln (13, 22, 50, -1, -1, -1, -1);
	rts_dpln (14, 7, 37, -1, -1, -1, -1);
	if (numberOfRings == 1)
		return (15);
	else if (numberOfRings == 2) {
		rts_dpln (15, 23, 51, -1, -1, -1, -1);
		rts_dpln (16, 8, 38, -1, -1, -1, -1);
		rts_dpln (17, 24, 52, -1, -1, -1, -1);
		rts_dpln (18, 9, 39, -1, -1, -1, -1);
		rts_dpln (19, 25, 53, -1, -1, -1, -1);
		rts_dpln (20, 10, 40, -1, -1, -1, -1);
		rts_dpln (21, 26, 54, -1, -1, -1, -1);
		rts_dpln (22, 11, 41, -1, -1, -1, -1);
		rts_dpln (23, 27, 55, -1, -1, -1, -1);
		rts_dpln (24, 12, 42, -1, -1, -1, -1);
		rts_dpln (25, 28, 56, -1, -1, -1, -1);
		rts_dpln (26, 13, 43, -1, -1, -1, -1);
		rts_dpln (27, 29, 57, -1, -1, -1, -1);
		rts_dpln (28, 14, 44, -1, -1, -1, -1);
		rts_dpln (29, 30, -1, -1, -1, -1, -1);
		rts_dpln (30, 15, -1, -1, -1, -1, -1);
		return (31);
	}
}

int             all931Planes (numberOfRings)
	int             numberOfRings;
{
	rts_dpln (0, 0, -1, -1, -1, -1, -1);
	rts_dpln (1, 1, 4, -1, -1, -1, -1);
	rts_dpln (2, 5, -1, -1, -1, -1, -1);
	rts_dpln (3, 6, 9, -1, -1, -1, -1);
	rts_dpln (4, 10, -1, -1, -1, -1, -1);
	rts_dpln (5, 11, 14, -1, -1, -1, -1);
	rts_dpln (6, 15, -1, -1, -1, -1, -1);
	if (numberOfRings == 1)
		return (7);
	else if (numberOfRings == 2) {
		rts_dpln (7, 19, 44, -1, -1, -1, -1);
		rts_dpln (8, 48, -1, -1, -1, -1, -1);
		rts_dpln (9, 49, 52, -1, -1, -1, -1);
		rts_dpln (10, 53, -1, -1, -1, -1, -1);
		rts_dpln (11, 54, 57, -1, -1, -1, -1);
		rts_dpln (12, 58, -1, -1, -1, -1, -1);
		rts_dpln (13, 59, 62, -1, -1, -1, -1);
		rts_dpln (14, 63, -1, -1, -1, -1, -1);
		return (15);
	}
}

int             allAcsPlanes (numberOfRings)
	int             numberOfRings;
{
	rts_dpln (0, 0, -1, -1, -1, -1, -1);
	rts_dpln (1, 1, 8, -1, -1, -1, -1);
	rts_dpln (2, 2, 9, 16, -1, -1, -1);
	rts_dpln (3, 3, 10, 17, 24, -1, -1);
	rts_dpln (4, 11, 18, 25, -1, -1, -1);
	rts_dpln (5, 12, 19, 26, 33, -1, -1);
	rts_dpln (6, 20, 27, 34, -1, -1, -1);
	rts_dpln (7, 21, 28, 35, 42, -1, -1);
	rts_dpln (8, 29, 36, 43, -1, -1, -1);
	rts_dpln (9, 30, 37, 44, 51, -1, -1);
	rts_dpln (10, 38, 45, 52, -1, -1, -1);
	rts_dpln (11, 39, 46, 53, 60, -1, -1);
	rts_dpln (12, 47, 54, 61, -1, -1, -1);
	if (numberOfRings == 1) {
		rts_dpln (13, 55, 62, -1, -1, -1, -1);
		rts_dpln (14, 63, -1, -1, -1, -1, -1);
		return (15);
	} else if (numberOfRings == 2) {
		rts_dpln (13, 55, 62, 69, 168, -1, -1);
		rts_dpln (14, 63, 70, 176, -1, -1, -1);
		rts_dpln (15, 71, 78, 177, 184, -1, -1);
		rts_dpln (16, 79, 185, 192, -1, -1, -1);
		rts_dpln (17, 87, 186, 193, 200, -1, -1);
		rts_dpln (18, 194, 201, 208, -1, -1, -1);
		rts_dpln (19, 195, 202, 209, 216, -1, -1);
		rts_dpln (20, 203, 210, 217, -1, -1, -1);
		rts_dpln (21, 204, 211, 218, 225, -1, -1);
		rts_dpln (22, 212, 219, 226, -1, -1, -1);
		rts_dpln (23, 213, 220, 227, 234, -1, -1);
		rts_dpln (24, 221, 228, 235, -1, -1, -1);
		rts_dpln (25, 222, 229, 236, 243, -1, -1);
		rts_dpln (26, 230, 237, 244, -1, -1, -1);
		rts_dpln (27, 231, 238, 245, 252, -1, -1);
		rts_dpln (28, 239, 246, 253, -1, -1, -1);
		rts_dpln (29, 247, 254, -1, -1, -1, -1);
		rts_dpln (30, 255, -1, -1, -1, -1, -1);
		return (31);
	}
}

int             allAcs6Planes (numberOfRings)
	int             numberOfRings;
{
	rts_dpln (0, 0, -1, -1, -1, -1, -1);
	rts_dpln (1, 1, 8, -1, -1, -1, -1);
	rts_dpln (2, 2, 9, 16, -1, -1, -1);
	rts_dpln (3, 3, 10, 17, 24, -1, -1);
	rts_dpln (4, 4, 11, 18, 25, 32, -1);
	rts_dpln (5, 5, 12, 19, 26, 33, 40);
	rts_dpln (6, 13, 20, 27, 34, 41, -1);
	rts_dpln (7, 14, 21, 28, 35, 42, 49);
	rts_dpln (8, 22, 29, 36, 43, 50, -1);
	rts_dpln (9, 23, 30, 37, 44, 51, 58);
	rts_dpln (10, 31, 38, 45, 52, 59, -1);
	rts_dpln (11, 39, 46, 53, 60, 67, 152);
	rts_dpln (12, 47, 54, 61, 68, 160, -1);
	if (numberOfRings == 1) {
		rts_dpln (13, 55, 62, -1, -1, -1, -1);
		rts_dpln (14, 63, -1, -1, -1, -1, -1);
		return (15);
	}
	rts_dpln (13, 55, 62, 69, 76, 161, 168);
	rts_dpln (14, 63, 70, 77, 169, 176, -1);
	rts_dpln (15, 71, 78, 85, 170, 177, 184);
	rts_dpln (16, 79, 86, 178, 185, 192, -1);
	rts_dpln (17, 87, 94, 179, 186, 193, 200);
	rts_dpln (18, 95, 187, 194, 201, 208, -1);
	rts_dpln (19, 103, 188, 195, 202, 209, 216);
	rts_dpln (20, 196, 203, 210, 217, 224, -1);
	rts_dpln (21, 197, 204, 211, 218, 225, 232);
	rts_dpln (22, 205, 212, 219, 226, 233, -1);
	rts_dpln (23, 206, 213, 220, 227, 234, 241);
	rts_dpln (24, 214, 221, 228, 235, 242, -1);
	rts_dpln (25, 215, 222, 229, 236, 243, 250);
	rts_dpln (26, 223, 230, 237, 244, 251, -1);
	if (numberOfRings == 2) {
		rts_dpln (27, 231, 238, 245, 252, -1, -1);
		rts_dpln (28, 239, 246, 253, -1, -1, -1);
		rts_dpln (29, 247, 254, -1, -1, -1, -1);
		rts_dpln (30, 255, -1, -1, -1, -1, -1);
		return (31);
	}
	rts_dpln (27, 231, 238, 245, 252, 387, 600);
	rts_dpln (28, 239, 246, 253, 388, 608, -1);
	rts_dpln (29, 247, 254, 389, 396, 609, 616);
	rts_dpln (30, 255, 390, 397, 617, 624, -1);
	rts_dpln (31, 391, 398, 405, 618, 625, 632);
	rts_dpln (32, 399, 406, 626, 633, 768, -1);
	rts_dpln (33, 407, 414, 627, 634, 769, 776);
	rts_dpln (34, 415, 635, 770, 777, 784, -1);
	rts_dpln (35, 423, 636, 771, 778, 785, 792);
	rts_dpln (36, 772, 779, 786, 793, 800, -1);
	rts_dpln (37, 773, 780, 787, 794, 801, 808);
	rts_dpln (38, 781, 788, 795, 802, 809, -1);
	rts_dpln (39, 782, 789, 796, 803, 810, 817);
	rts_dpln (40, 790, 797, 804, 811, 818, -1);
	rts_dpln (41, 791, 798, 805, 812, 819, 826);
	rts_dpln (42, 799, 806, 813, 820, 827, -1);
	rts_dpln (43, 807, 814, 821, 828, -1, -1);
	rts_dpln (44, 815, 822, 829, -1, -1, -1);
	rts_dpln (45, 823, 830, -1, -1, -1, -1);
	rts_dpln (46, 831, -1, -1, -1, -1, -1);
	if (numberOfRings == 3)
		return (47);
}

int             straightAcsPlanes (numberOfRings)
	int             numberOfRings;
{
	rts_dpln (0, 0, -1, -1, -1, -1, -1);
	rts_dpln (1, 9, -1, -1, -1, -1, -1);
	rts_dpln (2, 18, -1, -1, -1, -1, -1);
	rts_dpln (3, 27, -1, -1, -1, -1, -1);
	rts_dpln (4, 36, -1, -1, -1, -1, -1);
	rts_dpln (5, 45, -1, -1, -1, -1, -1);
	rts_dpln (6, 54, -1, -1, -1, -1, -1);
	rts_dpln (7, 63, -1, -1, -1, -1, -1);
	if (numberOfRings == 1)
		return (8);
	else if (numberOfRings == 2) {
		rts_dpln (8, 192, -1, -1, -1, -1, -1);
		rts_dpln (9, 201, -1, -1, -1, -1, -1);
		rts_dpln (10, 210, -1, -1, -1, -1, -1);
		rts_dpln (11, 219, -1, -1, -1, -1, -1);
		rts_dpln (12, 228, -1, -1, -1, -1, -1);
		rts_dpln (13, 237, -1, -1, -1, -1, -1);
		rts_dpln (14, 246, -1, -1, -1, -1, -1);
		rts_dpln (15, 255, -1, -1, -1, -1, -1);
		return (16);
	}
}

int             straightAcs6Planes (numberOfRings)
	int             numberOfRings;
{
	rts_dpln (0, 0, -1, -1, -1, -1, -1);
	rts_dpln (1, 9, -1, -1, -1, -1, -1);
	rts_dpln (2, 18, -1, -1, -1, -1, -1);
	rts_dpln (3, 27, -1, -1, -1, -1, -1);
	rts_dpln (4, 36, -1, -1, -1, -1, -1);
	rts_dpln (5, 45, -1, -1, -1, -1, -1);
	rts_dpln (6, 54, -1, -1, -1, -1, -1);
	rts_dpln (7, 63, -1, -1, -1, -1, -1);
	if (numberOfRings == 1)
		return (8);
	rts_dpln (8, 192, -1, -1, -1, -1, -1);
	rts_dpln (9, 201, -1, -1, -1, -1, -1);
	rts_dpln (10, 210, -1, -1, -1, -1, -1);
	rts_dpln (11, 219, -1, -1, -1, -1, -1);
	rts_dpln (12, 228, -1, -1, -1, -1, -1);
	rts_dpln (13, 237, -1, -1, -1, -1, -1);
	rts_dpln (14, 246, -1, -1, -1, -1, -1);
	rts_dpln (15, 255, -1, -1, -1, -1, -1);
	if (numberOfRings == 2)
		return (16);
	rts_dpln (16, 768, -1, -1, -1, -1, -1);
	rts_dpln (17, 777, -1, -1, -1, -1, -1);
	rts_dpln (18, 786, -1, -1, -1, -1, -1);
	rts_dpln (19, 795, -1, -1, -1, -1, -1);
	rts_dpln (20, 804, -1, -1, -1, -1, -1);
	rts_dpln (21, 813, -1, -1, -1, -1, -1);
	rts_dpln (22, 822, -1, -1, -1, -1, -1);
	rts_dpln (23, 831, -1, -1, -1, -1, -1);
	if (numberOfRings == 3)
		return (24);
}

int             lowEffHighResolutionAcsPlanes (numberOfRings)
	int             numberOfRings;
{
	rts_dpln (0, 0, -1, -1, -1, -1, -1);
	rts_dpln (1, 1, 8, -1, -1, -1, -1);
	rts_dpln (2, 9, -1, -1, -1, -1, -1);
	rts_dpln (3, 10, 17, -1, -1, -1, -1);
	rts_dpln (4, 18, -1, -1, -1, -1, -1);
	rts_dpln (5, 19, 26, -1, -1, -1, -1);
	rts_dpln (6, 27, -1, -1, -1, -1, -1);
	rts_dpln (7, 28, 35, -1, -1, -1, -1);
	rts_dpln (8, 36, -1, -1, -1, -1, -1);
	rts_dpln (9, 37, 44, -1, -1, -1, -1);
	rts_dpln (10, 45, -1, -1, -1, -1, -1);
	rts_dpln (11, 46, 53, -1, -1, -1, -1);
	rts_dpln (12, 54, -1, -1, -1, -1, -1);
	rts_dpln (13, 55, 62, -1, -1, -1, -1);
	rts_dpln (14, 63, -1, -1, -1, -1, -1);
	if (numberOfRings == 1)
		return (15);
	else if (numberOfRings == 2) {
		rts_dpln (15, 71, 184, -1, -1, -1, -1);
		rts_dpln (16, 192, -1, -1, -1, -1, -1);
		rts_dpln (17, 193, 200, -1, -1, -1, -1);
		rts_dpln (18, 201, -1, -1, -1, -1, -1);
		rts_dpln (19, 202, 209, -1, -1, -1, -1);
		rts_dpln (20, 210, -1, -1, -1, -1, -1);
		rts_dpln (21, 211, 218, -1, -1, -1, -1);
		rts_dpln (22, 219, -1, -1, -1, -1, -1);
		rts_dpln (23, 220, 227, -1, -1, -1, -1);
		rts_dpln (24, 228, -1, -1, -1, -1, -1);
		rts_dpln (25, 229, 236, -1, -1, -1, -1);
		rts_dpln (26, 237, -1, -1, -1, -1, -1);
		rts_dpln (27, 238, 245, -1, -1, -1, -1);
		rts_dpln (28, 246, -1, -1, -1, -1, -1);
		rts_dpln (29, 247, 254, -1, -1, -1, -1);
		rts_dpln (30, 255, -1, -1, -1, -1, -1);
		return (31);
	}
}

int             lowEffHighResolutionAcs6Planes (numberOfRings)
	int             numberOfRings;
{
	rts_dpln (0, 0, -1, -1, -1, -1, -1);
	rts_dpln (1, 1, 8, -1, -1, -1, -1);
	rts_dpln (2, 9, -1, -1, -1, -1, -1);
	rts_dpln (3, 10, 17, -1, -1, -1, -1);
	rts_dpln (4, 18, -1, -1, -1, -1, -1);
	rts_dpln (5, 19, 26, -1, -1, -1, -1);
	rts_dpln (6, 27, -1, -1, -1, -1, -1);
	rts_dpln (7, 28, 35, -1, -1, -1, -1);
	rts_dpln (8, 36, -1, -1, -1, -1, -1);
	rts_dpln (9, 37, 44, -1, -1, -1, -1);
	rts_dpln (10, 45, -1, -1, -1, -1, -1);
	rts_dpln (11, 46, 53, -1, -1, -1, -1);
	rts_dpln (12, 54, -1, -1, -1, -1, -1);
	rts_dpln (13, 55, 62, -1, -1, -1, -1);
	rts_dpln (14, 63, -1, -1, -1, -1, -1);
	if (numberOfRings == 1)
		return (15);
	rts_dpln (15, 71, 184, -1, -1, -1, -1);
	rts_dpln (16, 192, -1, -1, -1, -1, -1);
	rts_dpln (17, 193, 200, -1, -1, -1, -1);
	rts_dpln (18, 201, -1, -1, -1, -1, -1);
	rts_dpln (19, 202, 209, -1, -1, -1, -1);
	rts_dpln (20, 210, -1, -1, -1, -1, -1);
	rts_dpln (21, 211, 218, -1, -1, -1, -1);
	rts_dpln (22, 219, -1, -1, -1, -1, -1);
	rts_dpln (23, 220, 227, -1, -1, -1, -1);
	rts_dpln (24, 228, -1, -1, -1, -1, -1);
	rts_dpln (25, 229, 236, -1, -1, -1, -1);
	rts_dpln (26, 237, -1, -1, -1, -1, -1);
	rts_dpln (27, 238, 245, -1, -1, -1, -1);
	rts_dpln (28, 246, -1, -1, -1, -1, -1);
	rts_dpln (29, 247, 254, -1, -1, -1, -1);
	rts_dpln (30, 255, -1, -1, -1, -1, -1);
	if (numberOfRings == 2)
		return (31);
	rts_dpln (31, 391, 632, -1, -1, -1, -1);
	rts_dpln (32, 768, -1, -1, -1, -1, -1);
	rts_dpln (33, 769, 776, -1, -1, -1, -1);
	rts_dpln (34, 777, -1, -1, -1, -1, -1);
	rts_dpln (35, 778, 785, -1, -1, -1, -1);
	rts_dpln (36, 786, -1, -1, -1, -1, -1);
	rts_dpln (37, 787, 794, -1, -1, -1, -1);
	rts_dpln (38, 795, -1, -1, -1, -1, -1);
	rts_dpln (39, 796, 803, -1, -1, -1, -1);
	rts_dpln (40, 804, -1, -1, -1, -1, -1);
	rts_dpln (41, 805, 812, -1, -1, -1, -1);
	rts_dpln (42, 813, -1, -1, -1, -1, -1);
	rts_dpln (43, 814, 821, -1, -1, -1, -1);
	rts_dpln (44, 822, -1, -1, -1, -1, -1);
	rts_dpln (45, 823, 830, -1, -1, -1, -1);
	rts_dpln (46, 831, -1, -1, -1, -1, -1);
	if (numberOfRings == 3)
		return (47);
}

int             fourPlanesFor921 (numberOfRings)
	int             numberOfRings;
{
	rts_dpln (0, 0, -1, -1, -1, -1, -1);
	rts_dpln (1, 1, 8, -1, -1, -1, -1);
	rts_dpln (2, 2, 9, 16, -1, -1, -1);
	rts_dpln (3, 3, 17, 10, 24, -1, -1);
	rts_dpln (4, 18, 25, 11, -1, -1, -1);
	rts_dpln (5, 26, 19, 33, 12, -1, -1);
	rts_dpln (6, 27, 34, 20, -1, -1, -1);
	rts_dpln (7, 35, 28, 42, 21, -1, -1);
	rts_dpln (8, 36, 43, 29, -1, -1, -1);
	rts_dpln (9, 44, 37, 51, 30, -1, -1);
	rts_dpln (10, 45, 52, 38, -1, -1, -1);
	rts_dpln (11, 53, 46, 60, 39, -1, -1);
	rts_dpln (12, 54, 61, 47, -1, -1, -1);
	if (numberOfRings == 1) {
		rts_dpln (13, 55, 62, -1, -1, -1, -1);
		rts_dpln (14, 63, -1, -1, -1, -1, -1);
		return (15);
	}
	rts_dpln (13, 62, 55, 69, 168, -1, -1);
	rts_dpln (14, 63, 70, 176, -1, -1, -1);
	rts_dpln (15, 71, 184, 78, 177, -1, -1);
	rts_dpln (16, 192, 79, 185, -1, -1, -1);
	rts_dpln (17, 200, 193, 87, 186, -1, -1);
	rts_dpln (18, 201, 208, 194, -1, -1, -1);
	rts_dpln (19, 209, 202, 216, 195, -1, -1);
	rts_dpln (20, 210, 217, 203, -1, -1, -1);
	rts_dpln (21, 218, 211, 225, 204, -1, -1);
	rts_dpln (22, 219, 226, 212, -1, -1, -1);
	rts_dpln (23, 227, 220, 234, 213, -1, -1);
	rts_dpln (24, 228, 235, 221, -1, -1, -1);
	rts_dpln (25, 236, 229, 243, 222, -1, -1);
	rts_dpln (26, 237, 244, 230, -1, -1, -1);
	if (numberOfRings == 2) {
		rts_dpln (27, 231, 238, 245, 252, -1, -1);
		rts_dpln (28, 239, 246, 253, -1, -1, -1);
		rts_dpln (29, 247, 254, -1, -1, -1, -1);
		rts_dpln (30, 255, -1, -1, -1, -1, -1);
		return (31);
	}
	rts_dpln (27, 245, 238, 252, 231, -1, -1);
	rts_dpln (28, 246, 253, 239, -1, -1, -1);
	rts_dpln (29, 254, 247, 389, 616, -1, -1);
	rts_dpln (30, 255, 390, 624, -1, -1, -1);
	rts_dpln (31, 391, 632, 398, 625, -1, -1);
	rts_dpln (32, 768, 399, 633, -1, -1, -1);
	rts_dpln (33, 776, 769, 407, 634, -1, -1);
	rts_dpln (34, 777, 784, 770, -1, -1, -1);
	rts_dpln (35, 785, 778, 792, 771, -1, -1);
	rts_dpln (36, 786, 793, 779, -1, -1, -1);
	rts_dpln (37, 794, 787, 801, 780, -1, -1);
	rts_dpln (38, 795, 802, 788, -1, -1, -1);
	rts_dpln (39, 803, 796, 810, 789, -1, -1);
	rts_dpln (40, 804, 811, 797, -1, -1, -1);
	rts_dpln (41, 812, 805, 819, 798, -1, -1);
	rts_dpln (42, 813, 820, 806, -1, -1, -1);
	rts_dpln (43, 821, 814, 828, 807, -1, -1);
	rts_dpln (44, 822, 829, 815, -1, -1, -1);
	rts_dpln (45, 830, 823, -1, -1, -1, -1);
	rts_dpln (46, 831, -1, -1, -1, -1, -1);
	return (47);
}

int             realAcsPlanes ()
{
	return (all_planes ());
}

int             directAcsPlanes ()
{
	return (all_planes ());
}

int             setupIpcBoard (ecatModel, numberOfRings, ipcMode)
	int             ecatModel, numberOfRings;

	char           *ipcMode;
{
	int             i;

	char            command[8], resp[8];

	for (i = 64; i < 68; i++) {
		sprintf (command, "%2d%s", i, ipcMode);
		if (!rts_gantry (command, resp))
			return (RtsTimeOutError);
		if (!(strcmp (resp, "")) || !(strcmp (resp, "T")))
			return (IpcTimeOutError);
		if (numberOfRings == 1 || ecatModel == 921 || ecatModel == 961)
			break;
	}
	return (Ok);
}

int             acs_3d (maxPlanes)
{
	int             plane, logicalPlane;

	char            resp[32];

	if (EcatModel == 921 || EcatModel == 961) {
		for (plane = 0, logicalPlane = 0; plane < 832; plane++) {
			if ((plane >= 704 && plane <= 767) ||	/* not doable in 3 rings */
			    (plane >= 640 && plane <= 703) ||	/* not doable in 3 rings */
			    (plane >= 448 && plane <= 511) ||	/* not doable in 3 rings */
			    (plane >= 320 && plane <= 383))	/* not doable in 3 rings */
				continue;
			else
				rts_dpln (logicalPlane++, plane, -1, -1, -1, -1, -1);
		}
		if (!rts_gantry ("64g15", resp))
			return (RtsTimeOutError);
	} else {
		for (plane = 0; plane < maxPlanes; plane++)
			rts_dpln (plane, plane, -1, -1, -1, -1, -1);
		if (!rts_gantry ("64a", resp))
			return (RtsTimeOutError);
		printf ("Use defaults_get_integer\n");
/*
	if (defaults_get_integer ("/Ecat/EcatNumDirect", 16, 0) == 16)
*/
		if (16) {
			if (!rts_gantry ("65a", resp))
				return (RtsTimeOutError);
			if (!rts_gantry ("66a", resp))
				return (RtsTimeOutError);
			if (!rts_gantry ("67a", resp))
				return (RtsTimeOutError);
		}
	}
	return (maxPlanes);
}
