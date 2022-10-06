/*$Id: archive.h,v 1.2 1995/10/30 22:46:07 tom Exp $*/
/*$Log: archive.h,v $
 * Revision 1.2  1995/10/30  22:46:07  tom
 * remove reference to path petutil
 *
 * Revision 1.1  1995/10/30  18:21:49  tom
 * Initial revision
 *
 * Revision 1.2  1994/03/03  20:51:21  ty7777
 * Replace id with header.
 *
 * Revision 1.1  1993/11/11  17:46:38  ty7777
 * Initial revision
 **/

/* General definition and includes for the image archive software
   Creator: Fred Kuhns
   Date: 06/14/93
 */

#ifndef ARCHIVE_INCLUDED
#define ARCHIVE_INCLUDED

static char rcsid_archive [] = "$Header: /home/npggw/tom/inc/RCS/archive.h,v 1.2 1995/10/30 22:46:07 tom Exp $";

/* include files */
#include <petutil.h>

/* Global definitions, constants */

#define FALSE 0
#define TRUE 1
/* file access codes */
#define ERROR -1
#define OK 0
#define NWP 1  /* No Write Permission */
#define FEM 2  /* File Exists on Magnetic disk */
#define FEO 3  /* File Exists on Optical disk */
#define FDE 4  /* File Doesn't Exist */
/* various constants used for strings */
#define MAXPATH 1024
#define MAXFNAME 64
#define MAXNAME 32
#define VALID 1
#define INVALID -1
/* File Categories */
#define INDIVIDUAL 0
#define COMPOSITE  1
#define PHANTOM    2
/* Constant Strings */
#define Patient "patient"
#define Monkey "monkey"
/* File Types */
#define COMMON_DATA 0
#define PET_DATA    1
#define PET_IMAGE   2
#define MRI_DATA    3
#define MRI_IMAGE   4
#define ASCII       5
#define BINARY      6 
/* file system related define's */
/* UMASK == 000 000 011 -> rwx rwx r-- */
#define UMASK 0002

/* Structure Definitions */

struct ATTR_TYPE {
  char scanner[MAXNAME]; 
  char acq[MAXNAME];
  char recon[MAXNAME];
  char fltr[MAXNAME];
};

struct IND_TYPE {
  char subject[MAXNAME];      /* subject == patient or monkey */
  char id[MAXNAME];           /* subject id == [p|m]NNNN */
  char group[MAXNAME];        /* group == groupNN, NN = integer */
};

struct COMP_TYPE {
  char subject[MAXNAME];
  char protocol[MAXNAME];
  char group[MAXNAME];
};

struct PHAN_TYPE {
  char attributes[MAXFNAME];
};

union SPEC_TYPE {
  struct IND_TYPE  individual;
  struct COMP_TYPE composite;
  struct PHAN_TYPE phantom;
};

struct INFO_TYPE {
  int  cat;
  int  type;                  /* type == data, MRI or PETT */
  struct ATTR_TYPE attr;
  union SPEC_TYPE spec;
};

/* Structure describing Image file headers */
struct BASIC_HEADER {
  char Tracer[MAXNAME];  /* tracer string */
  char Id[MAXNAME];      /* Subject's Id string = [p|m]number */
  char Recon[MAXNAME];    /* Recon String used for ECAT headers */
  char Filter[MAXNAME];  /* filter string */
  int  Data_Type;        /* pett6 -> 1000, 1010, 1020, 2000, 2010
                            ecat  -> 3000 */
  int  Scanner;          /* pett6 == 6,
                            spetti = 7,
                            spettii == 8,
                            ecat == 10 */
  char Resolution[5];    /* in PETT6 headers -> low or high */
  int  Num_Slices;       /* used for determining image type */
  };

#endif /* ARCHIVE_INCLUDED */
