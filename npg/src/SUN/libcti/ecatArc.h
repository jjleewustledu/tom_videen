/*$Id: ecatArc.h,v 1.3 1999/12/13 22:02:19 tom Exp $*/
/*$Log: ecatArc.h,v $
 *Revision 1.3  1999/12/13 22:02:19  tom
 *partially cleaned up with lint
 *
 * Revision 1.2  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.1  1994/03/11  19:34:19  ty7777
 * Initial revision
 **/

/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef ECATARC_INCLUDED
#define ECATARC_INCLUDED
#ifndef lint
static char     rcsid_ecatArc[] = "$Header: /home/npggw/tom/src/libcti/RCS/ecatArc.h,v 1.3 1999/12/13 22:02:19 tom Exp $";
#endif

#include <rpc/types.h>


/* server number and version */

#define ARC_SERVER 600000041
#define ARC_SERVER_VERSION 1

/* job type definitions */

/* ioctl functions for the acs scsi 8mm tape */

#define MTTSTRDY 1
#define MTREWIND 2
#define MTRDBLKLIMITS 3
#define MTWRTFMRK 4
#define MTFSF 5
#define MTBSF 6
#define MTERASE 7
#define MTLOAD 8
#define MTEJECT 9
#define MTLOCK 10
#define MTUNLOCK 11
#define MTEOM 12
#define MTBLKSF 0
#define MTBMODE 0
#define MTGETERROR 15
#define MTBLKSIZ 16
#define MTBLANKCHK 17

/* acs archive job type definitions */

#define ARCJ_CREATE 0
#define ARCJ_EXTRACT 1
#define ARCJ_VERIFY 2
#define ARCJ_LIST 3
#define ARCJ_SEARCH 4
#define ARCJ_DELETE 5
#define ARCJ_VOLLABEL 6

/* acs archive job notification complete */

#define ARC_FILE_DONE 0
#define ARC_STUDY_DONE 1
#define ARC_ERROR_NOTIFY 2

/* acs archive functions and tape control */


enum ARC_FUNCTIONS {
	DBUGARC = 1,
	LABEL = 1 + 1,
	CREATE = 1 + 2,
	EXTRACT = 1 + 3,
	VERIFY = 1 + 4,
	ARCLIST = 1 + 5,
	SEARCH = 1 + 6,
	QUEINFO = 1 + 7,
	DELETE = 1 + 8,
	STAT = 1 + 9,
	ACSMT = 1 + 10,
	GET_LABEL = 1 + 11
};

typedef enum ARC_FUNCTIONS ARC_FUNCTIONS;

bool_t          xdr_ARC_FUNCTIONS ();

struct DATE_args {
	int             seconds;
	int             minute;
	int             hour;
	int             day;
	int             month;
	int             year;
};

typedef struct DATE_args DATE_args;

bool_t          xdr_DATE_args ();

struct VOLLABEL_args {
	char           *vol_name;
	char           *date;
	char           *comments;
	bool_t          overwrite;
};

typedef struct VOLLABEL_args VOLLABEL_args;

bool_t          xdr_VOLLABEL_args ();

typedef char   *file_names;

bool_t          xdr_file_names ();

struct ARCJOB_args {
	int             jobtype;
	bool_t          verbose;
	char           *targetdev;
	char           *study_name;
	struct {
		u_int           arc_names_len;
		file_names     *arc_names_val;
	}               arc_names;
};

typedef struct ARCJOB_args ARCJOB_args;

bool_t          xdr_ARCJOB_args ();

struct ARCQUE_args {
	int             entry;
	int             jobnum;
	int             jobtype;
	int             jobstatus;
	int             nfiles;
	char           *study;
	struct {
		u_int           files_len;
		file_names     *files_val;
	}               files;
};

typedef struct ARCQUE_args ARCQUE_args;

bool_t          xdr_ARCQUE_args ();

struct ACSMT_args {
	int             mtcode;
	int             mtarg;
};

typedef struct ACSMT_args ACSMT_args;

bool_t          xdr_ACSMT_args ();

struct STAT_args {
	bool_t          vol_valid;
	bool_t          tape_valid;
	bool_t          tape_present;
	char           *vol_name;
	char           *date;
	char           *comments;
	u_long          tape_left;
};

typedef struct STAT_args STAT_args;

bool_t          xdr_STAT_args ();

#endif						 /* ECATARC_INCLUDED */
