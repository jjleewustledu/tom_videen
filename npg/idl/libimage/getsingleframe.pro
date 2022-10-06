FUNCTION GETSINGLEFRAME, img, msg
   COMMON IMAGE_UTILS
   
   fname = img.fname
   IF (strlen(fname) EQ 0) THEN BEGIN
      msg =  'No file entered'
      RETURN, -1
   ENDIF ELSE BEGIN 
      widget_control,/hourglass
      ftype = getfiletype(fname)
      ;IF (ftype GE 0) THEN print,fname,' = ',fileformat[ftype], ' format' 
      IF (ftype LT 0) THEN print,'Format returned as ',ftype
      CASE ftype of
         MATRIX_REV6: n = getecat6frm(img, msg)
         MATRIX_REV7: n = getecat7frm(img, msg)
         ;INTERFILE: n = getinterfilefrm(img, msg)
         ;PETT6: n = getpett6(img, msg)
         ;ANALYZE: n = getanalyze(img, msg)
         -2: BEGIN   
            msg = 'Cannot open ' + fname
            RETURN, -1
         ENDCASE
         ELSE: BEGIN
            msg = 'Fileformat not handled by getsinglefrm ' + fname
            RETURN, -1
         ENDELSE
      ENDCASE
      RETURN, n
   ENDELSE 
END
