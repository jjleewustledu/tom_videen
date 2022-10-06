/*  $Id: libimage.h,v 2.0 2004/02/12 20:30:50 tom Exp tom $
 *	$Log: libimage.h,v $
 * Revision 2.0  2004/02/12  20:30:50  tom
 * sync Feb 2004
 *
 * Revision 1.12  2004/01/08  15:36:50  tom
 * *** empty log message ***
 *
 * Revision 1.11  2002/02/27  18:45:30  tom
 * add wrattn6
 *
 * Revision 1.10  2000/09/28  17:18:30  tom
 * get_sa
 *
 * Revision 1.9  2000/07/17  19:30:48  tom
 * *** empty log message ***
 *
 * Revision 1.8  2000/03/13  16:10:33  tom
 * add get_ss put_ss put_ss6 wrscan wrscan6
 *
 * Revision 1.7  2000/02/18  16:00:56  tom
 * add get_sa6 get_ss6 make_sa6 make_ss6 rdattn6 rdscan6 use_sa6 use_ss6
 *
 * Revision 1.6  2000/01/20  21:25:01  tom
 * add rdattn7 wrattn7 get_sa7 put_sa7 make_sa7 use_sa7
 *
 * Revision 1.5  1999/12/14  15:38:13  tom
 * add create_dir7
 *
 * Revision 1.4  1998/12/15  19:22:46  tom
 * add use_ss7
 *
 * Revision 1.3  1998/12/10  18:43:00  tom
 * *** empty log message ***
 *
 * Revision 1.2  1997/01/24  18:22:36  tom
 * add firstimgfrm
 *
 * Revision 1.1  1996/04/04  14:07:53  tom
 * Initial revision
 *
 */
/* =============================================================================
 *  Module:         libimage.h
 *  Date:           Mar 1996
 *  Author:         Tom Videen
 *  Description:    Prototypes for routines for image and sinogram file I/O
 *	Library:		libimage.a
 * =============================================================================
 */
#ifndef libimage_dot_h_defined
#define libimage_dot_h_defined

/*@unused@*/
#ifndef lint
static char     libimage_rcsid[] = "$Header: /usr/local/npg/include/RCS/libimage.h,v 2.0 2004/02/12 20:30:50 tom Exp tom $";
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

#ifndef cti6_dot_h_defined
#include <cti6.h>
#endif

extern void     close_imgfile ();
extern void     comment2info ();
extern int      create_dir7 ();
extern int      credirent ();
extern int      existfif ();
extern int      existfrm6 ();
extern int      existfrm7 ();
extern int      existimgfrm ();
extern int      existp6 ();
extern int      fill_mh6 ();
extern int      fill_mh7 ();
extern int      fill_hp6 ();
extern int      fill_sh6 ();
extern int      fill_sh7 ();
extern int      firstimgfrm ();
extern float   *flipimg ();
extern void     free_imgfile ();
extern int      get_ifh ();
extern int      get_imgh ();
extern int      get_hp6 ();
extern int      get_mh ();
extern int      get_mh6 ();
extern int      get_mh7 ();
extern int      get_mhif ();
extern int      get_sa ();
extern int      get_sa6 ();
extern int      get_sa7 ();
extern int      get_sh ();
extern int      get_sh6 ();
extern int      get_sh7 ();
extern int      get_ss ();
extern int      get_ss6 ();
extern int      get_ss7 ();
extern int      get_shif ();
extern int      getfiletype ();
extern int      getmatpos ();
extern char    *getstr ();
extern char    *header_file ();
extern int      ieee2pe ();
extern void     info2comment ();
extern int      make_hp6 ();
extern ImageFile *make_imgfile ();
extern int      make_mh ();
extern int      make_mh6 ();
extern int      make_mh7 ();
extern int      make_mhif ();
extern int      make_sa6 ();
extern int      make_sa7 ();
extern int      make_sh ();
extern int      make_sh6 ();
extern int      make_sh7 ();
extern int      make_ss6 ();
extern int      make_ss7 ();
extern int      make_shif ();
extern void     mhnames6 ();
extern void     mhnames7 ();
extern int      numcode7 ();
extern int      open_if ();
extern int      open_imgfile ();
extern int      open_m6 ();
extern int      open_m7 ();
extern int      open_p6 ();
extern float   *padplane ();
extern float    pe2ieee ();
extern int      put_hp6 ();
extern int      put_mh ();
extern int      put_mh6 ();
extern int      put_mh7 ();
extern int      put_mhif ();
extern int      put_sa7 ();
extern int      put_sh ();
extern int      put_sh6 ();
extern int      put_sh7 ();
extern int      put_ss ();
extern int      put_ss6 ();
extern int      put_ss7 ();
extern int      put_shif ();
extern int      read_fsr ();
extern float   *rdattn6 ();
extern float   *rdattn7 ();
extern float   *rdimg ();
extern float   *rdimg_decay ();
extern float   *rdimg_decaycor ();
extern float   *rdimg6 ();
extern float   *rdimg7 ();
extern float   *rdimg7_decay ();
extern float   *rdimg7_decaycor ();
extern float   *rdimgif ();
extern float   *rdimgp6 ();
extern short   *rdscan6 ();
extern short   *rdscan7 ();
extern int      rdrec ();
extern int      save_rcs ();
extern int      scale_sh ();
extern int      scale_sh6 ();
extern int      scale_sh7 ();
extern int      scale_shif ();
extern int      update_mh ();
extern int      update_mh6 ();
extern int      update_mh7 ();
extern int      update_hp6 ();
extern int      update_sh ();
extern int      update_sh6 ();
extern int      update_sh7 ();
extern int      use_mh6 ();
extern int      use_mh7 ();
extern int      use_mhif ();
extern int      use_mhp6 ();
extern int      use_sa6 ();
extern int      use_sa7 ();
extern int      use_sh6 ();
extern int      use_sh7 ();
extern int      use_shif ();
extern int      use_shp6 ();
extern int      use_ss6 ();
extern int      use_ss7 ();
extern int      wrattn6 ();
extern int      wrattn7 ();
extern int      wrimg6 ();
extern int      wrimg7 ();
extern int      wrimgif ();
extern int      wrimgp6 ();
extern int      wrscan ();
extern int      wrscan6 ();
extern int      wrscan7 ();
extern int      wrtrec ();

#endif						 /* libimage_dot_h_defined */
