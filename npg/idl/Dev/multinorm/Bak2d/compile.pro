@IMAGE_UTILS.h
libpath = getenv('NPG_DIR')
restore, libpath+'/getframe.sav'
restore, libpath+'/putinterfile.sav'
restore, libpath+'/type.sav'
outfile = getenv('OUTFILE')
.compile $INFILE
resolve_all
save, /routines, filename=outfile
exit
