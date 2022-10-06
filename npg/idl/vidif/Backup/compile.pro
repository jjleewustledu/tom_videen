@IMAGE_UTILS.h
@VIDI.h
libpath = getenv('NPG_DIR')
restore, libpath+'/vdisplay__define.sav'
restore, libpath+'/getframe.sav'
restore, libpath+'/gethdr.sav'
restore, libpath+'/getfiletype.sav'
restore, libpath+'/getecat6.sav'
restore, libpath+'/getanalyze.sav'
restore, libpath+'/getinterfile.sav'
restore, libpath+'/putinterfile.sav'
restore, libpath+'/setintersection.sav'
.compile vidif
resolve_all
save, /routines, filename='vidif.sav'
exit
