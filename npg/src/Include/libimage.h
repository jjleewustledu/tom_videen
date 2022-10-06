/*  $Id: libimage.h,v 1.1 1996/04/04 14:07:53 tom Exp $
 *	$Log: libimage.h,v $
 * Revision 1.1  1996/04/04  14:07:53  tom
 * Initial revision
 *
 */
/* =============================================================================
 *  Module:         libimage.h
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *  Description:    Prototypes for routines for image and sinogram file I/O
 *	Library:		libimage.a
 * =============================================================================
 */
#ifndef libimage_dot_h_defined
#define libimage_dot_h_defined

#ifndef lint
static char     libimage_rcsid[] = "$Header: /usr/local/npg/include/RCS/libimage.h,v 1.1 1996/04/04 14:07:53 tom Exp $";
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

void			close_imgfile ();
void            comment2info ();
int				credirent ();
int             existfrm6 ();
int             existfrm7 ();
int             existfrmif ();
int             existimgfrm ();
int             existp6 ();
int				fill_mh6 ();
int				fill_mh7 ();
int				fill_hp6 ();
int				fill_sh6 ();
int				fill_sh7 ();
float          *flipimg ();
void            free_imgfile ();
int       		get_hif ();
int             get_hp6 ();
int     	    get_mh ();
int    		    get_mh6 ();
int             get_mh7 ();
int             get_mhif ();
int       		get_sh ();
int 			get_sh6 ();
int 			get_sh7 ();
int   			get_shif ();
int             getfiletype ();
int             getmatpos ();
char           *getstr ();
char           *header_file ();
int				ieee2pe ();
void            info2comment ();
int       		make_hp6 ();
ImageFile      *make_imgfile ();
int     		make_mh ();
int    			make_mh6 ();
int   			make_mh7 ();
int  			make_mhif ();
int      		make_sh ();
int 			make_sh6 ();
int 			make_sh7 ();
int   			make_shif ();
void            mhnames6 ();
void            mhnames7 ();
int             numcode7 ();
int				open_imgfile ();
int				open_m6 ();
int				open_m7 ();
int				open_p6 ();
float          *padplane ();
float           pe2ieee ();
int             put_hp6 ();
int             put_mh ();
int             put_mh6 ();
int             put_mh7 ();
int             put_mhif ();
int             put_sh ();
int             put_sh6 ();
int             put_sh7 ();
int             put_shif ();
float          *rdimg ();
float          *rdimg6 ();
float          *rdimg7 ();
float          *rdimgif ();
float          *rdimgp6 ();
int             rdrec ();
int             save_rcs ();
int             scale_sh ();
int             scale_sh6 ();
int             scale_sh7 ();
int             scale_shif ();
int				update_mh ();
int				update_mh6 ();
int				update_mh7 ();
int				update_hp6 ();
int				update_sh ();
int				update_sh6 ();
int				update_sh7 ();
int     		use_mh6 ();
int		    	use_mh7 ();
int			    use_mhif ();
int			    use_mhp6 ();
int			    use_sh6 ();
int			    use_sh7 ();
int			    use_shif ();
int			    use_shp6 ();
int             wrimg6 ();
int             wrimg7 ();
int             wrimgif ();
int             wrimgp6 ();
int             wrtrec ();

#endif	/* libimage_dot_h_defined */
