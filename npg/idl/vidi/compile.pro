@IMAGE_UTILS.h
@VIDI.h
libpath = getenv('NPG_DIR')
restore, libpath+'/vdisplay__define.sav'
restore, libpath+'/voi__define.sav'
restore, libpath+'/voi_process.sav'
restore, libpath+'/getframe.sav'
restore, libpath+'/gethdr.sav'
restore, libpath+'/getfiletype.sav'
restore, libpath+'/getecat6.sav'
restore, libpath+'/getanalyze.sav'
restore, libpath+'/getinterfile.sav'
restore, libpath+'/putinterfile.sav'
restore, libpath+'/setintersection.sav'
.compile vidi
resolve_all
save, /routines, filename='vidi.sav'
exit
