@IMAGE_UTILS.h
libpath = getenv('NPG_DIR')
restore, libpath+'/voi__define.sav'
restore, libpath+'/voi_process.sav'
restore, libpath+'/getframe.sav'
restore, libpath+'/gethdr.sav'
restore, libpath+'/getfiletype.sav'
restore, libpath+'/putinterfile.sav'
.compile pcimages

resolve_all
save, /routines, filename='pcimages.sav'
exit
