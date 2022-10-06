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
