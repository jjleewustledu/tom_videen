/*  $Log: interfile.h,v $
 * Revision 2.1  2004/02/20  16:07:24  tom
 * *** empty log message ***
 *
 * Revision 2.0  2004/02/12  20:30:50  tom
 * sync Feb 2004
 *
 * Revision 1.4  2004/01/08  15:36:50  tom
 * *** empty log message ***
 *
 * Revision 1.3  2000/07/17  19:29:17  tom
 * revise for libimage routines
 *
 * Revision 1.2  1996/09/24  16:38:04  tom
 * *** empty log message ***
 *
 * Revision 1.1  1996/03/05  22:43:28  tom
 * Initial revision
 *
    $Id: interfile.h,v 2.1 2004/02/20 16:07:24 tom Exp $
 */
/* =============================================================================
 *  Module:         interfile.h
 *  Date:           Jul 2000
 *  Author:         Tom Videen
 *  Description:    Interfile header structure
 * =============================================================================
 */
#ifndef if_dot_h_defined
#define if_dot_h_defined

/*@unused@*/
#ifndef	lint
static char     if_rcsid[] = "$Header: /usr/local/npg/include/RCS/interfile.h,v 2.1 2004/02/20 16:07:24 tom Exp $";
#endif

typedef struct if_main_header {
	char           *modality;
	char           *sw_name;
	int           	sw_version;
	char           *orig_system;
	char           *institution;
	char           *data_file;
	char           *subject_id;
	char           *study_date;
	int             big_endian;
	int             number_format;
	int             nbytes;
	int             ndim;
	int             dim1;
	int             dim2;
	int             dim3;
	int             dim4;
	float           voxel_size_1;
	float           voxel_size_2;
	float           voxel_size_3;
	char            initial_1[10];
	char            initial_2[10];
	char            initial_3[10];
}
MainHeader_if;

typedef struct if_frame_header {
	float           start_time;
	float           duration;
	long			offset;
	float			scale_factor;
	float			deadtime_correction;
	float			decay_correction;
}
FrameHeader_if;

typedef struct if_header {
	MainHeader_if  *mh;
	FrameHeader_if *fh;
}
IFheader;

#endif	/* if_dot_h_defined */
