@IMAGE_UTILS.h
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
.compile mcavoi_pic
resolve_all
save, /routines, filename='mcavoi_pic.sav'
exit
