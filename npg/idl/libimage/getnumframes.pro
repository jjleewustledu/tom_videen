FUNCTION GETNUMFRAMES, img, msg
   COMMON IMAGE_UTILS
   
   fname = img.fname
   IF (strlen(fname) EQ 0) THEN BEGIN
      msg =  'No file entered'
      RETURN, -1
   ENDIF
   cmd = '/usr/local/npg/bin/getnumframes ' + fname
   spawn,cmd,unit=lun
   k = 0
   str = ''
   readf,lun,k,str,format='(I0,A)'
   IF (k EQ -1) THEN BEGIN
      msg = 'Read Error: ' + str
      RETURN, -1
   ENDIF
   img.ff = k
   readf,lun,k,str,format='(I0,A)'
   IF (k EQ -1) THEN BEGIN
      msg = 'Read Error: ' + str
      RETURN, -1
   ENDIF
   img.lf = k
   readf,lun,k,str,format='(I0,A)'
   IF (k EQ -1) THEN BEGIN
      msg = 'Read Error: ' + str
      RETURN, -1
   ENDIF
   img.lp = k
   RETURN, 0
END
