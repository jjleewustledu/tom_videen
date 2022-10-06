FUNCTION GETFILETYPE, fname
   COMMON IMAGE_UTILS

   filetype = 0

   ;; Check INTERFILE first

   i = rstrpos(fname,'.')
   IF (i GT 0) THEN BEGIN
      hname = strmid(fname,0,i)+'    '
      strput, hname,'.ifh', i
   ENDIF ELSE hname = fname + '.ifh'
   openr, lu, hname, /GET_LUN, ERROR=err
   IF (err EQ 0) THEN BEGIN
      str = ''
      readf, lu, str
      close, lu
      free_lun, lu
      IF ((strpos(str,'INTERFILE') EQ 0) OR $
          (strpos(str,'!INTERFILE') EQ 0)) THEN BEGIN
         filetype = INTERFILE
         RETURN, filetype
      ENDIF
   ENDIF

   ;; Check PET file types (ECAT7, ECAT6 or PETT6)

   openr, lu, fname, /GET_LUN, ERROR=err
   IF (err NE 0) THEN RETURN, -2
   
   test = {ver:bytarr(8), data:intarr(24)}
   readu, lu, test, TRANSFER_COUNT=count
   close,lu
   free_lun, lu
   IF (count NE 24) THEN RETURN, -1
   
   ver0 = string(test.ver(0:1))
   ver1 = string(test.ver(0:6))
   datatype = fix(test.data(21))
   system = fix(test.data(22))
   sw_version = fix(test.data(20))
   byteorder, datatype, /SSWAP
   IF (datatype EQ 2 OR datatype EQ 6) THEN BEGIN
      byteorder, system, /SSWAP
      byteorder, sw_version, /SSWAP
   ENDIF
   

   IF (filetype EQ 0) THEN BEGIN
      IF (ver1 EQ 'MATRIX7') THEN filetype = MATRIX_REV7 $
      ELSE IF (((system EQ 953) OR (system EQ 951) OR (system EQ 921) OR (system EQ 961) OR $
                (system EQ 962) OR (system EQ 6) OR (system EQ 1) OR (system EQ 0)) AND $
               ((sw_version EQ 64) OR (sw_version EQ 65))) THEN filetype = MATRIX_REV6 $
      ELSE IF ((system EQ 9110) AND (sw_version EQ 6)) THEN filetype = MATRIX_REV6 $
      ELSE IF (ver0 EQ '**') THEN filetype = PETT6
   ENDIF

   ;; Check if ANALYZE format
   
   IF (filetype EQ 0) THEN BEGIN
      i = rstrpos(fname,'.img')
      IF (i LT 0) THEN BEGIN
         i = strpos(fname,'.hdr')
         IF (i GT 0) THEN BEGIN
            hname = fname
            strput, fname, '.img', i
         ENDIF
      ENDIF ELSE BEGIN
         hname = fname
         strput, hname,'.hdr', i
      ENDELSE
      openr, lu, hname, /GET_LUN, ERROR=err
      IF (err NE 0) THEN RETURN, -2 $
      ELSE BEGIN
         hdr = ana
         readu, lu, hdr, TRANSFER_COUNT=count
         close, lu
         free_lun,lu
         IF ((count EQ 1) AND (hdr.hk.sizeof_hdr EQ 348)) THEN filetype = ANALYZE 
      ENDELSE
   END

   RETURN, filetype
END 

