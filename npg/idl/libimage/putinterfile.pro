FUNCTION PUTINTERFILE, img, name, msg, APPEND=append, EXTRA=extra
   ;; Writes one image frame to a file using Interfile format.
   ;; Reads unused keys from a previous Interfile header and writes
   ;; them to the new header in the appropriate blocking (if the original
   ;; was blocked) or to the end of the new header.
   ;; Blocking:
   ;;   File Identification (INTERFILE := )
   ;;   General Data (GENERAL DATA := )
   ;;   General Image Data (GENERAL IMAGE DATA := )
   ;;   Study Data (PET STUDY := )
   ;; 
   ;; Parameters:
   ;;	img = image structure of image to be saved
   ;;	name = file name to save
   ;;	msg = returns error message, if any
   ;; Keyword APPEND -> used to add image frame to end of file
   ;;	and increment number of frames in header
   ;; Keyword EXTRA -> used to add info to end of interfile header

   COMMON IMAGE_UTILS
   
   widget_control,/hourglass
   IF (strlen(name) EQ 0) THEN BEGIN
      msg =  'No filename defined'
      RETURN, -1
   ENDIF 
   IF (img.data EQ ptr_new()) THEN BEGIN
      msg =  'No data defined'
      RETURN, -1
   ENDIF 
   
   dim = lonarr(4)
   FOR k=0,3 DO dim[k] = img.dim[k]
   pdim = fltarr(4)
   FOR k=0,3 DO pdim[k] = img.pdim[k]
   dim[3] = dim[3] > 1
   
   CASE img.ntype OF
      type(0): BEGIN
         stype = 'signed integer'
         sbytes = '2'
      ENDCASE 
      type(0L): BEGIN
         stype = 'signed integer'
         sbytes = '4'
      ENDCASE 
      type(0.0): BEGIN 
         stype = 'float'
         sbytes = '4'
      ENDCASE 
      type(0.0D): BEGIN 
         stype = 'double'
         sbytes = '8'
      ENDCASE 
      ELSE: BEGIN
         msg =  'Subroutine does not handle this number type: '+string(img.ntype)
         exit
      ENDCASE
   ENDCASE
   
   k = rstrpos(name,'.')
   IF (k GT 0) THEN name = strmid(name,0,k)
   hname = name + '.ifh'
   fname = name + '.img'
   
   IF NOT KEYWORD_SET(APPEND) THEN append = 0 $
   ELSE BEGIN 
      openr, lu1, hname, ERR=err, /GET_LUN
      IF (err NE 0) THEN append = 0 $ ; if file doesn't exist, create a new one
      ELSE BEGIN 
         msg = ''
         str = ''
         WHILE (NOT eof(lu1)) DO BEGIN
            readf,lu1,str
            k = strpos(str,':=')
            key = strtrim(strmid(str,0,k),2)
            value = strtrim(strmid(str,k+2,256),2)
            CASE key OF
               'number format': IF (value NE stype) THEN $
                  msg = 'Existing file has a different number format'
               'number of bytes per pixel': IF (value NE sbytes) THEN $
                  msg = 'Existing file has a different number of bytes per pixel'
               'matrix size [1]': IF (long(value) NE dim[0]) THEN $
                  msg = 'Existing file has a different matrix size'
               'matrix size [2]': IF (long(value) NE dim[1]) THEN $
                  msg = 'Existing file has a different matrix size'
               'matrix size [3]': IF (long(value) NE dim[2]) THEN $
                  msg = 'Existing file has a different matrix size'
               'matrix size [4]': dim[3] = long(value)+dim[3]
               'scaling factor (mm/pixel) [1]': IF (value NE strtrim(string(pdim[0]),2)) THEN $
                  msg = 'Existing file has a different pixel scaling factor'
               'scaling factor (mm/pixel) [2]': IF (value NE strtrim(string(pdim[1]),2)) THEN $
                  msg = 'Existing file has a different pixel scaling factor'
               'scaling factor (mm/pixel) [3]': IF (value NE strtrim(string(pdim[2]),2)) THEN $
                  msg = 'Existing file has a different pixel scaling factor'
               'matrix initial element [1]': IF (value NE 'right') THEN $
                  msg = 'Existing file has a different orientation'
               'matrix initial element [2]': IF (value NE 'posterior') THEN $
                  msg = 'Existing file has a different orientation'
               'matrix initial element [3]': IF (value NE 'inferior') THEN $
                  msg = 'Existing file has a different orientation'
               ELSE:
            ENDCASE
         ENDWHILE
         close,lu1
         free_lun, lu1
         IF (msg NE '') THEN RETURN, -1
      ENDELSE 	
   ENDELSE 

   ;; New ifh (lu1)

   openw,lu1, hname, ERROR=err, /GET_LUN
   IF (err NE 0) THEN BEGIN
      msg = 'Cannot open '+hname
      RETURN, -1
   ENDIF
   
   ;; New img (lu2)

   openw,lu2, fname, ERROR=err, APPEND=append, /GET_LUN
   IF (err NE 0) THEN BEGIN
      msg = 'Cannot open '+fname
      close, lu1
      free_lun, lu1
      RETURN, -1
   ENDIF
   
   ;; Existing ifh for previous keys (lu3)

   str = ''
   k = rstrpos(img.fname,'.')
   oldname = ''
   IF (k GT 0) THEN oldname = strmid(img.fname,0,k)+'.ifh'
   IF (oldname EQ hname) THEN BEGIN
      oldname = hname+'tmp'
      cmd = 'cp ' + hname + ' ' + oldname
      spawn,cmd,unit=lun
      tmpfile = 1
   ENDIF ELSE tmpfile = 0
   existinghdr = 0
   openr,lu3, oldname, ERROR=err, /GET_LUN
   IF (err NE 0) THEN existinghdr = 0 $
   ELSE BEGIN
      readf, lu3, str
      IF (strpos(str,'INTERFILE') GE 0) THEN existinghdr = 1 $
      ELSE BEGIN
         existinghdr = 0
         close, lu3
         free_lun, lu3
      ENDELSE
   ENDELSE 
   
   ;; Format the image data amd write it (lu2)
   
   CASE img.ntype OF
      type(0): data = fix(*img.data)
      type(0L): data = long(*img.data)
      type(0.0): data = float(*img.data)
      type(0.0D): data = double(*img.data)
   ENDCASE
   order = img.order
   IF (order EQ 1) THEN BEGIN 
      data = reverse(data,1)
      data = reverse(data,2)
      IF (dim[2] GT 1) THEN data = reverse(data,3)
   ;ENDIF ELSE IF (order NE 0) THEN BEGIN
      ;msg =  'Subroutine does not handle this image order: '+string(order)
      ;RETURN, -1
   ENDIF 
   writeu,lu2,data
   close,lu2
   free_lun,lu2

   name = fname
   k = rstrpos(name,'/')
   IF (k GT 0) THEN name = strmid(name,k+1)
   maxval = MAX(data, MIN=minval)
   IF (maxval LT 100000000.0 AND maxval GE 1000.0) THEN $
      print,maxval,minval,format='("Max =",f12.3,"  Min = ",f12.3)' $
   ELSE IF (maxval GE 0.1) THEN $
      print,maxval,minval,format='("Max =",f12.6,"  Min = ",f12.6)' $
   ELSE print,maxval,minval,format='("Max =",e10.4,"  Min = ",e10.4)'
   
   ;; File Identification

   printf, lu1, 'INTERFILE :='
   printf, lu1, 'version of keys    := 3.3'
   printf, lu1, 'date of keys       := 1992:01:01'
   printf, lu1, 'conversion program := putinterfile.pro'
   printf, lu1, 'program date       := 2001:12:20'
   printf, lu1, 'conversion date    := ' +systime(0)
   IF (existinghdr) THEN BEGIN
      more =  1
      WHILE (more AND NOT eof(lu3)) DO BEGIN
         str = ''
         readf,lu3,str
         IF (strpos(str,'GENERAL DATA') GE 0) THEN more = 0 $
         ELSE IF (strpos(str,';') NE 0) THEN BEGIN
            p = 0
            WHILE (P GE 0) DO BEGIN
               p = strpos(str,'!')
               IF (P GE 0) THEN strput, str, ' ', p
            ENDWHILE
            k = strpos(str,':=')
            key = strtrim(strmid(str,0,k),2)
            value = strtrim(strmid(str,k+2,256),2)
            CASE key OF
               'version of keys':
               'date of keys':
               'conversion program':
               'program date':
               'conversion date':
               'original institution':
               'data starting block':
               'name of data file':
               'type of data':
               'total number of images':
               'imagedata byte order':
               'number of dimensions':
               'number of slices':
               'matrix size [1]':
               'matrix size [2]':
               'matrix size [3]':
               'matrix size [4]':
               'number format':
               'number of bytes per pixel':
               'scaling factor (mm/pixel) [1]':
               'scaling factor (mm/pixel) [2]':
               'scaling factor (mm/pixel) [3]':
               'matrix initial element [1]':
               'matrix initial element [2]':
               'matrix initial element [3]':
               'maximum pixel count':
               'minimum pixel count':
               'END OF INTERFILE':
               ELSE: printf, lu1, str
            ENDCASE
         ENDIF
      ENDWHILE
   ENDIF
   printf, lu1, ';'

   ;; General Data

   printf, lu1, 'GENERAL DATA :='
   printf, lu1, 'original institution := Washington University'
   printf, lu1, 'data starting block  := 0'
   printf, lu1, 'name of data file  := '+strtrim(name,2)
   IF (existinghdr) THEN BEGIN
      more =  1
      WHILE (more AND NOT eof(lu3)) DO BEGIN
         str = ''
         readf,lu3,str
         IF (strpos(str,'IMAGE DATA') GE 0) THEN more = 0 $
         ELSE IF (strpos(str,';') NE 0) THEN BEGIN
            p = 0
            WHILE (P GE 0) DO BEGIN
               p = strpos(str,'!')
               IF (P GE 0) THEN strput, str, ' ', p
            ENDWHILE
            k = strpos(str,':=')
            key = strtrim(strmid(str,0,k),2)
            value = strtrim(strmid(str,k+2,256),2)
            CASE key OF
               'original institution':
               'data starting block':
               'name of data file':
               'type of data':
               'total number of images':
               'imagedata byte order':
               'number of dimensions':
               'number of slices':
               'matrix size [1]':
               'matrix size [2]':
               'matrix size [3]':
               'matrix size [4]':
               'number format':
               'number of bytes per pixel':
               'scaling factor (mm/pixel) [1]':
               'scaling factor (mm/pixel) [2]':
               'scaling factor (mm/pixel) [3]':
               'matrix initial element [1]':
               'matrix initial element [2]':
               'matrix initial element [3]':
               'maximum pixel count':
               'minimum pixel count':
               ELSE: printf, lu1, str
            ENDCASE
         ENDIF
      ENDWHILE
   ENDIF
   printf, lu1, ';'

   ;; General Image Data

   printf, lu1, 'GENERAL IMAGE DATA :='
   printf, lu1, 'type of data := tomographic'
   printf, lu1, 'total number of images := '+strtrim(string(dim[2]),2)
   printf, lu1, 'imagedata byte order   := bigendian'
   IF (existinghdr) THEN BEGIN
      more =  1
      WHILE (more AND NOT eof(lu3)) DO BEGIN
         str = ''
         readf,lu3,str
         IF (strpos(str,'STUDY') GE 0) THEN more = 0 $
         ELSE IF (strpos(str,';') NE 0) THEN BEGIN
            p = 0
            WHILE (P GE 0) DO BEGIN
               p = strpos(str,'!')
               IF (P GE 0) THEN strput, str, ' ', p
            ENDWHILE
            k = strpos(str,':=')
            key = strtrim(strmid(str,0,k),2)
            value = strtrim(strmid(str,k+2,256),2)
            CASE key OF
               'type of data':
               'total number of images':
               'imagedata byte order':
               'number of dimensions':
               'number of slices':
               'matrix size [1]':
               'matrix size [2]':
               'matrix size [3]':
               'matrix size [4]':
               'number format':
               'number of bytes per pixel':
               'scaling factor (mm/pixel) [1]':
               'scaling factor (mm/pixel) [2]':
               'scaling factor (mm/pixel) [3]':
               'matrix initial element [1]':
               'matrix initial element [2]':
               'matrix initial element [3]':
               'maximum pixel count':
               'minimum pixel count':
               ELSE: printf, lu1, str
            ENDCASE
         ENDIF
      ENDWHILE
   ENDIF
   printf, lu1, ';'

   ;; Study Data

   IF (strpos(str,'STUDY') GE 0) THEN printf, lu1, str
   printf, lu1, 'number of dimensions   := 3'
   printf, lu1, 'number of slices := '+strtrim(string(dim[2]),2)
   printf, lu1, 'matrix size [1]    := '+strtrim(string(dim[0]),2)
   printf, lu1, 'matrix size [2]    := '+strtrim(string(dim[1]),2)
   printf, lu1, 'matrix size [3]    := '+strtrim(string(dim[2]),2)
   printf, lu1, 'matrix size [4]    := '+strtrim(string(dim[3]),2)
   printf, lu1, 'number format      := '+stype
   printf, lu1, 'number of bytes per pixel  := '+sbytes
   printf, lu1, 'scaling factor (mm/pixel) [1]  := '+strtrim(string(pdim[0]),2)
   printf, lu1, 'scaling factor (mm/pixel) [2]  := '+strtrim(string(pdim[1]),2)
   printf, lu1, 'scaling factor (mm/pixel) [3]  := '+strtrim(string(pdim[2]),2)
   IF (order EQ 0) THEN BEGIN 
      printf, lu1, 'matrix initial element [1] := left'
      printf, lu1, 'matrix initial element [2] := anterior'
      printf, lu1, 'matrix initial element [3] := superior'
   ENDIF ELSE IF (order EQ 1) THEN BEGIN 
      printf, lu1, 'matrix initial element [1] := right'
      printf, lu1, 'matrix initial element [2] := posterior'
      printf, lu1, 'matrix initial element [3] := inferior'
   ENDIF  
   printf, lu1, 'maximum pixel count := '+strtrim(string(maxval),2)
   printf, lu1, 'minimum pixel count := '+strtrim(string(minval),2)
   IF (existinghdr) THEN BEGIN
      more =  1
      WHILE (more AND NOT eof(lu3)) DO BEGIN
         str = ''
         readf,lu3,str
         IF (strpos(str,'END OF INTERFILE') GE 0) THEN more = 0 $
         ELSE IF (strpos(str,';') NE 0) THEN BEGIN
            p = 0
            WHILE (P GE 0) DO BEGIN
               p = strpos(str,'!')
               IF (P GE 0) THEN strput, str, ' ', p
            ENDWHILE
            k = strpos(str,':=')
            key = strtrim(strmid(str,0,k),2)
            value = strtrim(strmid(str,k+2,256),2)
            CASE key OF
               'number of dimensions':
               'number of slices':
               'matrix size [1]':
               'matrix size [2]':
               'matrix size [3]':
               'matrix size [4]':
               'number format':
               'number of bytes per pixel':
               'scaling factor (mm/pixel) [1]':
               'scaling factor (mm/pixel) [2]':
               'scaling factor (mm/pixel) [3]':
               'matrix initial element [1]':
               'matrix initial element [2]':
               'matrix initial element [3]':
               'maximum pixel count':
               'minimum pixel count':
               ELSE: printf, lu1, str
            ENDCASE
         ENDIF
      ENDWHILE
      close, lu3
      free_lun,lu3
   ENDIF
   IF KEYWORD_SET(EXTRA) THEN BEGIN
      nstr = (size(extra))[1] 
      IF (nstr GT 0) THEN FOR k=0,nstr-1 DO printf, lu1, extra[k]
   ENDIF
   printf, lu1, 'END OF INTERFILE :='
   close,lu1
   free_lun,lu1
   name = fname
   msg = 'File saved '+fname
   IF (tmpfile EQ 1) THEN BEGIN
      cmd = 'rm ' + hname + 'tmp'
      spawn,cmd,result
   ENDIF 
   RETURN, 0
END
