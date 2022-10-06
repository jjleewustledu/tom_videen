#ifndef __NCmake_empty_imagev7_module
#define __NCmake_empty_imagev7_module 1

extern 
int NCmake_empty_imagev7(  
  MatrixFile      *imgfile,
  Image_subheader *img_sub,
  int              matnum 
  )
;

#endif /* #ifndef __NCmake_empty_imagev7_module */
#ifndef __NCwrite_imagev7_module
#define __NCwrite_imagev7_module 1

extern 
int NCwrite_imagev7(       
  MatrixFile      *imgfile,
  Image_subheader *img_sub,  
  void            *data,     
  int              matnum,   
  int              plane     
  )
;

#endif /* #ifndef __NCwrite_imagev7_module */
#ifndef __NCreadnormv7_module
#define __NCreadnormv7_module 1

extern 
float*  NCreadnormv7(
  char*      sino_file,
  char*      norm_file,
  int        plane,
  int        frame,
  int        gate,
  int        bed,
  int        verbose,
  int        corr_intrinsic,
  int        corr_geometric,
  int        corr_deadtime)
;

#endif /* #ifndef __NCreadnormv7_module */
#ifndef __NCmatrix_write_plane_module
#define __NCmatrix_write_plane_module 1

extern 
int NCmatrix_write_plane(
  MatrixFile *imgfile,
  MatrixData *imgmatrix,
  int         plane, 
  int         frame,
  int         gate,
  int         bed)
;

#endif /* #ifndef __NCmatrix_write_plane_module */
#ifndef __NCread_mat_float_module
#define __NCread_mat_float_module 1

extern 
float* NCread_mat_float(MatrixData *matrix, 
                     float      scale)
;

#endif /* #ifndef __NCread_mat_float_module */
#ifndef __NCwrite_scanv7_module
#define __NCwrite_scanv7_module 1

extern 
int NCwrite_scanv7(
  MatrixFile       *sinofile,    
  Scan3D_subheader *scan3d_sub,  
  short            *data,        
  int               matnum,      
  int               plane,       
  int               segment      
  )
;

#endif /* #ifndef __NCwrite_scanv7_module */
#ifndef __NCmake_empty_scanv7_module
#define __NCmake_empty_scanv7_module 1


#endif /* #ifndef __NCmake_empty_scanv7_module */
#ifndef __NCmake_empty_attenv7_module
#define __NCmake_empty_attenv7_module 1

extern 
int NCmake_empty_attenv7(
  MatrixFile       *attenfile, 
  Attn_subheader   *atten_sub,
  int               matnum
  )
;

#endif /* #ifndef __NCmake_empty_attenv7_module */
#ifndef __NCwrite_attenv7_module
#define __NCwrite_attenv7_module 1

extern 
int NCwrite_attenv7(       
  MatrixFile      *attenfile,
  Attn_subheader  *atten_sub,  
  float           *data,   
  int              matnum,  
  int              plane,
  int              segment
  )
;

#endif /* #ifndef __NCwrite_attenv7_module */
