FUNCTION GETHDR, img, msg
   
   COMMON IMAGE_UTILS
   
   msg = ''
   fname = img.fname
   IF (img.proc EQ 0) THEN BEGIN
      FOR k=0,9 DO img.mcal = 0.0
      msg =  'No hdrfile needed for no processing'
      RETURN, -1
   ENDIF 
   
   IF (strlen(fname) EQ 0) THEN BEGIN
      msg =  'No file entered'
      RETURN, -1
   ENDIF
   
   l1 = rstrpos(fname,'/') > 0
   l2 = strlen(fname)
   stat = 1
   k = l1
   WHILE (stat EQ 1 AND k LT l2) DO BEGIN ; first number of subject ID
      k = k+1
      char = strmid(fname,k,1)
      IF (char GE '0' AND char LE '9') THEN stat = 0
   ENDWHILE 
   l3 = k
   stat = 1
   WHILE (stat EQ 1 AND k LT l2) DO BEGIN ; last number of subject ID
      k = k+1
      char = strmid(fname,k,1)
      IF (char LT '0' OR char GT '9') THEN stat = 0
   ENDWHILE 
   stat = 1
   WHILE (stat EQ 1 AND k LT l2) DO BEGIN ; first number of scan number
      k = k+1
      char = strmid(fname,k,1)
      IF (char GE '0' AND char LE '9') THEN stat = 0
   ENDWHILE 
   l4 = k-l1-2
   str1 = '*'+strmid(fname,l3,l4+1)+'*hdr'+string(replicate(32b,(l2-l1-l4)))
   fname = strmid(fname,0,l1+1)+str1
   fname = strcompress(fname, /remove_all)
   
   fname = dialog_pickfile(FILE=fname, FILTER=fname, /READ, TITLE='Select HDR file')
   IF (strlen(fname) EQ 0) THEN BEGIN
      msg = 'No file entered'
      RETURN, -1
   ENDIF
   
   cmd = '/usr/local/npg/bin/hdr_read '+ fname
   spawn,cmd,unit=lun
   stat = 0
   readf,lun,stat
   img.mtype = stat
   IF (stat LT 0) THEN BEGIN
      msg = 'Unrecognized hdr file type'
      RETURN, -1
   ENDIF 
   x = 0.0
   FOR k=0,9 DO BEGIN
      readf,lun,x
      img.mcal[k] = x
   ENDFOR 
   close,lun
   free_lun,lun
   RETURN, 0
END
