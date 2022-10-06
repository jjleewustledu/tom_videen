@IMAGE_UTILS.h
@FID.h
libpath = getenv('NPG_DIR')
restore, libpath+'/getframe.sav'
restore, libpath+'/gethdr.sav'
restore, libpath+'/getfiletype.sav'
restore, libpath+'/getinterfile.sav'
restore, libpath+'/putinterfile.sav'
restore, libpath+'/intersect.sav'
restore, libpath+'/runlength.sav'
restore, libpath+'/setintersection.sav'
.compile fiducial
resolve_all
save, /routines, filename='fiducial.sav'
exit
