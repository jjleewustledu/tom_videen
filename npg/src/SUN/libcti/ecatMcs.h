/*$Id: ecatMcs.h,v 1.4 1999/12/13 22:02:19 tom Exp $*/
/*$Log: ecatMcs.h,v $
 *Revision 1.4  1999/12/13 22:02:19  tom
 *partially cleaned up with lint
 *
 * Revision 1.3  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.2  1994/03/11  20:01:31  ty7777
 * Added rcs headers.
 *
 * Revision 1.1  1994/03/11  19:37:06  ty7777
 * Initial revision
 **/

#ifndef ECATMCS_INCLUDED
#define ECATMCS_INCLUDED

/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <rpc/types.h>

#define MCS_SERVER 600000033
#define MCS_SERVER_VERSION 1
#define MOVE_AXIS 0
#define ROTATE_AXIS 1
#define REPORT_POSITION 2
#define REPORT_SPEED 3

#ifndef lint
static char     rcsid_ecatMcs[] = "$Header: /home/npggw/tom/src/libcti/RCS/ecatMcs.h,v 1.4 1999/12/13 22:02:19 tom Exp $";
#endif

enum MCS_FUNCTIONS {
	MOVE_HORIZONTAL_BED = 1,
	MOVE_WOBBLE = 1 + 1,
	MOVE_VERTICAL_BED = 1 + 2,
	MOVE_ROTATING_SOURCE = 1 + 3,
	ROTATE_SOURCE = 1 + 4,
	ROTATE_WOBBLE = 1 + 5,
	MOVE_TR_SOURCE = 1 + 6,
	REPORT_MOTIONS = 1 + 7,
	MCS_INIT = 1 + 8,
	MCS_POFF = 1 + 9,
	MCS_SERIAL = 1 + 10,
	MOTION_ZEROES = 1 + 11,
	MCS_UPDATE = 1 + 12,
	MCS_RING = 1 + 13,
	MCS_PON = 1 + 14,
	MOVE_SEPTA = 1 + 15
};

typedef enum MCS_FUNCTIONS MCS_FUNCTIONS;

bool_t          xdr_MCS_FUNCTIONS ();

enum MCS_MOTIONS {
	GANTRY_TILT = 0,
	GANTRY_ROTATION = 1,
	HORIZONTAL_BED = 2,
	VERTICAL_BED = 3,
	TR_SOURCE = 4,
	WOBBLE = 5,
	ROTATING_SOURCE = 7,
	LASER_OFFSET = 7 + 1,
	RETRACTIBLE_SEPTA = 7 + 2
};

typedef enum MCS_MOTIONS MCS_MOTIONS;

bool_t          xdr_MCS_MOTIONS ();

struct MCS_STATUS {
	int             ps_status;
	int             rs_status;
	int             wob_status;
	int             bed_status;
	int             tilt_status;
	int             rot_status;
	int             ps_encoder;
	int             wob_encoder;
	float           hb_encoder;
	int             vb_encoder;
	int             tilt_encoder;
	int             rot_encoder;
	int             mcs_inp;
	int             ps_speed;
	int             wob_speed;
	float           bed_ref_encoder;
	int             septa_status;
};

typedef struct MCS_STATUS MCS_STATUS;

bool_t          xdr_MCS_STATUS ();

struct MCS_ZEROES {
	int             axis;
	int             value;
};

typedef struct MCS_ZEROES MCS_ZEROES;

bool_t          xdr_MCS_ZEROES ();

#endif						 /* ECATMCS_INCLUDED */
