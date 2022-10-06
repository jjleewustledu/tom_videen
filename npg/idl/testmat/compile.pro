@IMAGE_UTILS.h
libpath = getenv('NPG_DIR')
outfile = getenv('OUTFILE')
.compile air__define
.compile analyzehdr__define
.compile $INFILE
resolve_all
save, /routines, filename=outfile
exit
