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
