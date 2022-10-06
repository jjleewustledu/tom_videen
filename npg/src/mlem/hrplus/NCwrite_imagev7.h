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
