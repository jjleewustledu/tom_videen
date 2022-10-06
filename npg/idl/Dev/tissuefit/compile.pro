libpath = getenv('NPG_DIR')
.compile tissuefit
resolve_all
save, /routines, filename='tissuefit.sav'
exit
