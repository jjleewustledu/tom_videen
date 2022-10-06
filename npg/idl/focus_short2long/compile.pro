libpath = getenv('NPG_DIR')
.compile focus_short2long
resolve_all
save, /routines, filename='focus_short2long.sav'
exit
