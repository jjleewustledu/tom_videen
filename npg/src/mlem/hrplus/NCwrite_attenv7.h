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
