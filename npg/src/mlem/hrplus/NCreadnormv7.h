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
