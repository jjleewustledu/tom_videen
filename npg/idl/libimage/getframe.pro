FUNCTION GETFRAME, img, msg
   COMMON IMAGE_UTILS
   
   fname = img.fname
   IF (strlen(fname) EQ 0) THEN BEGIN
      msg =  'No file entered'
      RETURN, -1
   ENDIF ELSE BEGIN 
      widget_control,/hourglass
      ftype = getfiletype(fname)
      IF (ftype GE 0) THEN print,fname,' = ',fileformat[ftype], ' format' $
      ELSE print,'Format returned as ',ftype
      CASE ftype of
         MATRIX_REV6: n = getecat6(img, msg)
         MATRIX_REV7: n = getecat7(img, msg)
         INTERFILE: n = getinterfile(img, msg)
         PETT6: n = getpett6(img, msg)
         ANALYZE: n = getanalyze(img, msg)
         -2: BEGIN   
            msg = 'Cannot open ' + fname
            RETURN, -1
         ENDCASE
         ELSE: BEGIN
            msg = 'Unrecognized fileformat ' + fname
            RETURN, -1
         ENDELSE
      ENDCASE
      RETURN, n
   ENDELSE 
END
