/*  $Log: libimg.h,v $
 * Revision 2.0  2004/02/12  20:30:50  tom
 * sync Feb 2004
 *
 * Revision 1.1  1996/03/05  22:43:35  tom
 * Initial revision
 *
    $Id: libimg.h,v 2.0 2004/02/12 20:30:50 tom Exp $
 */
/* =============================================================================
 *  Module:         libimg.h
 *  Date:           Dec-95
 *  Author:         Tom Videen
 *  Description:    Prototypes for routines for image and sinogram file I/O
 *	Library:		libimg.a
 *	Functions in:	cti.c, cti6.c, cti7.c, interfile.c, pett6.c, comment_info.c
 * =============================================================================
 */
#ifndef libimg_dot_h_defined
#define libimg_dot_h_defined

#ifndef	lint
static char     libimg_rcsid[] = "$Header: /usr/local/npg/include/RCS/libimg.h,v 2.0 2004/02/12 20:30:50 tom Exp $";
#endif

#ifndef imagio_dot_h_defined
#include <imagio.h>
#endif

#ifndef cti7_dot_h_defined
#include <cti7.h>
#endif

#ifndef if_dot_h_defined
#include <interfile.h>
#endif

#ifndef pett6_dot_h_defined
#include <pett6.h>
#endif

#ifndef matrix_dot_h_defined
#include <matrix.h>
#endif

/* cti.c */

SubHeader      *getsbhdr ();
SubHeader      *ssh7_to_sh ();
SubHeader      *ish7_to_sh ();
SubHeader      *ssh6_to_sh ();
SubHeader      *ish6_to_sh ();
Scan3D_subheader7 *sh_to_ssh7 ();
Image_subheader7 *sh_to_ish7 ();
Scan_subheader *sh_to_ssh6 ();
Image_subheader *sh_to_ish6 ();
MainHeader     *getmhdr ();
MainHeader     *mh6_to_mh ();
MainHeader     *mh7_to_mh ();
void            mhnames6 ();
void            mhnames7 ();
Main_header    *mh_to_mh6 ();
Main_header7   *mh_to_mh7 ();
float          *rdimg ();
int             putsbhdr ();
int             putmhdr ();
int             wrimg ();
int             get_filetype ();
void            free_mh ();
void            free_sh ();
MainHeader     *make_mainheader ();
SubHeader      *make_subheader ();
float          *readimgfrm ();
int             writeimgfrm ();
float          *flipimg ();
float          *padplane ();
int             existimgfrm ();

/* cti6.c */

void           *getsbhdr6 ();
int             putsbhdr6 ();
int             putmhdr6 ();
Main_header    *getmhdr6 ();
float          *rdimg6 ();
int             wrimg6 ();
MatrixFile     *write_fp6 ();
MatrixFile     *getfp6 ();
void            close_fp6 ();
int             plnnum_f3d ();
int             ringnum ();
int             existfrm6 ();

/* cti7.c */

int             numcod ();
FILE           *filopn ();
void            close_fp7 ();
FILE           *filcre ();
int             getmatpos ();
int             rdrec ();
int             wrtrec ();
Main_header7   *read_mhdr ();
int             write_mhdr ();
Scan3D_subheader7 *read_scan_shdr ();
int             write_scan_shdr ();
Image_subheader7 *read_image_shdr ();
int             write_image_shdr ();
int             create_dir ();
int             credirent ();
float           get_sf ();
void            put_sf ();
FILE           *getfp7 ();
void           *getsbhdr7 ();
Main_header7   *getmhdr7 ();
float          *rdimg7 ();
int             putsbhdr7 ();
int             putmhdr7 ();
int             wrimg7 ();
FILE           *write_fp7 ();
int             existfrm7 ();

/* interfile.c */

IFheader       *gethdr_if ();
char           *getstr ();
SubHeader_if   *getsbhdr_if ();
int             putmhdr_if ();
int             write_key ();
int             putsbhdr_if ();
char           *header_file ();
MainHeader_if  *getmhdr_if ();
void            free_ifh ();
void            free_ifmh ();
float          *rdimg_if ();
int             wrimg_if ();
FILE           *write_fp ();
IFheader       *get_ifh ();
void            put_ifh ();
FILE           *getfp ();
void            close_if ();
SubHeader      *ifsh_to_sh ();
MainHeader     *ifmh_to_mh ();
SubHeader_if   *sh_to_ifsh ();
MainHeader_if  *mh_to_ifmh ();

/* pett6.c */

PETT6HDR       *getp6hdr ();
MainHeader     *p6h_to_mh ();
SubHeader      *p6h_to_sh ();
PETT6HDR       *mh_to_p6h ();
int             putp6hdr ();
float          *rdimgp6 ();
int             wrimgp6 ();
FILE           *fileopen ();
FILE           *get_fp ();
void            close_fp ();
int             existp6 ();

/* comment_info.c */

void           comment2info ();
void           info2comment ();

/* other */

float           pe2ieee ();
void			save_rcs ();

#endif	/* libimg_dot_h_defined */
