outfile = getenv('OUTFILE')
.compile $INFILE
resolve_all
save, /routines, filename=outfile
exit
