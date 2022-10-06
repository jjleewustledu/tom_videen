/*  $Id: if.h,v 1.1 1995/11/30 18:21:37 tom Exp $
    $Log: if.h,v $
 * Revision 1.1  1995/11/30  18:21:37  tom
 * Initial revision
 *
 */
/* =============================================================================
 *  Module:         if.h
 *  Date:           Nov-95
 *  Author:         John Ollinger
 *  Description:    Routines for Interfile image and sinogram file I/O (libimg.a)
 * =============================================================================
 */
#ifndef	lint
static char     if_rcsid[] = "$Header: /home/npggw/tom/include/RCS/if.h,v 1.1 1995/11/30 18:21:37 tom Exp $";
#endif

#ifndef if_dot_h_defined
#define if_dot_h_defined

/* File types. */
#define UNKNOWN	 0
#define IMAGE    1
#define SINOGRAM 2

/* Number types. */
#define BYTE_IF   1
#define SHORT_IF  2
#define FLOAT_IF  3

typedef struct if_main_header {
    char   *data_file;
    int    file_type;
    int    number_format;
    char   *patient_id;
    char   *study_date;
    int	   ndim;
    int    dim1;
    int    dim2;
    int    dim3;
    int    dim4;
    int	   nbed;
    int    orig_system;
    float  voxel_size_1;
    float  voxel_size_2;
    float  voxel_size_3;
    float  scale_factor;
    float bed_elevation;
    float init_bed_pos;
    float bed_off[16];
    }
MainHeader_if;

typedef struct if_sub_header {
    int      dim1;		/* Assumed same as in main header. */
    int      dim2;		/* Assumed same as in main header. */
    int      dim3;		/* Assumed same as in main header. */
    int	     ndim;		/* Assumed same as in main header. */
    float    voxel_size_1;
    float    voxel_size_2;
    float    voxel_size_3;	/* Assumed same as in main header. */
    int	     number_format;	/* Assumed same as in main header. */
    float    storage_sf;
    float    deadtime_corr;
    int      trues;
    int      randoms;
    int      multiples;
    int      starting_time;	/* Assumed same as in frame header. */
    int      duration;		/* Assumed same as in frame header. */
    char     *comment;		/* User defined comment field. */
    }
SubHeader_if;

typedef struct if_frame_header {
    int      starting_time;
    int      duration
    }
FrameHeader_if;
    

typedef struct if_header {
    char     		*filnam;
    MainHeader_if 	*mh;
    FrameHeader_if 	**fh;
    SubHeader_if 	***sh;
    }
IFheader;

#endif if_dot_h_defined
