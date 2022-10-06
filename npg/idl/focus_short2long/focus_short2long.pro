PRO FOCUS_SHORT2LONG
   
   infile = getenv('INSCN')
   IF (strlen(infile) EQ 0) THEN BEGIN
      print, 'No input file entered'
      RETURN
   ENDIF 
   outfile = getenv('OUTSCN')
   IF (strlen(outfile) EQ 0) THEN BEGIN
      print,  'No output file entered'
      RETURN
   ENDIF 
   nfrm = fix(getenv('NFRAMES'))
   IF (nfrm LE 0) THEN BEGIN
      print,  'Frames must be > 0'
      RETURN
   ENDIF 

   openr,luin, infile, ERROR=err, /GET_LUN
   IF (err NE 0) THEN BEGIN
      print, 'Cannot open '+infile
      RETURN
   ENDIF
   openw,luout, outfile, ERROR=err, /GET_LUN
   IF (err NE 0) THEN BEGIN
      print, 'Cannot open '+outfile
      RETURN
   ENDIF

   nbytes = 227453184L			; This is the size of a 3D sinogram (288x252x95 with 17 segments)
   len = nbytes/2
   data = intarr(len)
   ldata = lonarr(len)
   FOR n=1, nfrm DO BEGIN 
      print, 'Frame ',n
      readu, luin, data 
      data = swap_endian(data)
      ldata = long(data)
      ldata = swap_endian(ldata)
      writeu, luout, ldata
   ENDFOR 

   close,luin
   close,luout
   free_lun,luin
   free_lun,luout
END
