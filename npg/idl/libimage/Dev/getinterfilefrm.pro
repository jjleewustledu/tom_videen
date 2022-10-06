;; Read decay-corrected counts from frame img.ff into img.data
;; img.h.sh7.decay_corr_fctr = factor by which data were decay-corrected

FUNCTION GETINTERFILEFRM, img, msg
   COMMON IMAGE_UTILS
   
   fname = img.fname
   IF (strlen(fname) EQ 0) THEN BEGIN
      msg =  'No file entered'
      RETURN, -1
   ENDIF 

   i = rstrpos(fname,'.')
   IF (i GT 0) THEN BEGIN
      hname = strmid(fname,0,i)+'    '
      strput, hname,'.ifh', i
   ENDIF ELSE hname = fname + '.ifh'
   openr,lun, hname, ERROR=err, /GET_LUN
   IF (err NE 0) THEN BEGIN
      msg = 'Cannot open '+hname
      RETURN, -1
   ENDIF

   str = ''
   readf, lun, str
   IF ((strpos(str,'INTERFILE') NE 0) AND (strpos(str,'!INTERFILE') NE 0)) THEN BEGIN
      msg = 'Cannot read as Interfile: '+fname
      close, lun
      free_lun, lun
      RETURN, -1
   ENDIF

   frm = img.ff

   ifh = {IFheader}
   IF (img.data NE ptr_new()) THEN ptr_free, img.data
   
   img.itype = INTERFILE
   img.dim[0] = 1
   img.dim[1] = 1
   img.dim[2] = 1
   img.dim[3] = 1
   img.scale =  1.0
   revx = 0
   revy = 0
   revz = 0
   k = rstrpos(fname,'/')
   IF (k GT 0) THEN path = strmid(fname,0,k+1) ELSE path = ''

   WHILE (NOT eof(lun)) DO BEGIN
      readf,lun,str
      p = 0
      WHILE (P GE 0) DO BEGIN
         p = strpos(str,'!')
         IF (P GE 0) THEN strput, str, ' ', p
      ENDWHILE
      k = strpos(str,':=')
      key = strtrim(strmid(str,0,k),2)
      value = strtrim(strmid(str,k+2,256),2)
      CASE key OF
         'name of data file': BEGIN
            k = rstrpos(value,'/')
            IF (k LT 0) THEN fname = path+value $
            ELSE fname = value
         END
         'number format': BEGIN
            CASE value OF
               'byte': img.ntype = type(0B)
               'signed integer': img.ntype = type(0)
               'long integer': img.ntype = type(0L)
               'float': img.ntype = type(0.0)
               'double': img.ntype = type(0.0D)
               ELSE: img.ntype = type(0)
            ENDCASE
            ifh.mh.number_format = img.ntype
         END
         'number of bytes per pixel': BEGIN
            nbytes = value
            CASE value OF
               '1': img.ntype = type(0B)
               '2': img.ntype = type(0)
               '4': 
               '8': 
               ELSE: BEGIN
                  msg = 'Unrecognized number format '+fname
                  close, lun
                  free_lun, lun
                  RETURN, -1
               ENDELSE 
            ENDCASE
            ifh.mh.nbytes = img.value
         END
         'matrix size [1]': BEGIN
            img.dim[0] = fix(value)
            ifh.mh.dim1 = img.dim[0]
          END
         'matrix size [2]': BEGIN
            img.dim[1] = fix(value)
            ifh.mh.dim2 = img.dim[1]
          END
         'matrix size [3]': BEGIN
            img.dim[2] = fix(value)
            ifh.mh.dim3 = img.dim[2]
          END
         'matrix size [4]': BEGIN
            img.dim[3] = fix(value)
            ifh.mh.dim4 = img.dim[3]
          END
         'scaling factor (mm/pixel) [1]': BEGIN
            img.pdim[0] = float(value)
            ifh.mh.voxel_size_1 = img.pdim[0]
          END
         'scaling factor (mm/pixel) [2]': BEGIN
            img.pdim[1] = float(value)
            ifh.mh.voxel_size_2 = img.pdim[1]
          END
         'scaling factor (mm/pixel) [3]': BEGIN
            img.pdim[2] = float(value)
            ifh.mh.voxel_size_3 = img.pdim[2]
          END
         'matrix initial element [1]': BEGIN
            IF (strpos(value,'right') EQ 0) THEN revx = 1
            ifh.mh.initial_1 = value
          END
         'matrix initial element [2]': BEGIN
            IF (strpos(value,'posterior') EQ 0) THEN revy = 1
            ifh.mh.initial_2 = value
          END
         'matrix initial element [3]': BEGIN
            IF (strpos(value,'inferior') EQ 0) THEN revz = 1
            ifh.mh.initial_3 = value
          END
         'atlas origin [1]': img.org[0] = float(value)
         'atlas origin [2]': img.org[1] = float(value)
         'atlas origin [3]': img.org[2] = float(value)
         'scale_factor': img.scale = float(value)
         ELSE:
      ENDCASE
   ENDWHILE
   close,lun
   
   dim = lonarr(4)
   FOR k=0,3 DO dim[k] = img.dim[k]

   img.p  = 1
   img.fp = 1
   img.lp = dim[2]
   img.f  = frm
   img.ff = frm
   img.lf = frm

   fp = img.fp
   lp = img.lp
   ff = img.ff
   lf = img.lf

   slice = dim[0]*dim[1]*fix(nbytes)
   
   CASE img.ntype OF
      1: data = bytarr(dim[0],dim[1],dim[2])
      2: data = intarr(dim[0],dim[1],dim[2])
      3: data = lonarr(dim[0],dim[1],dim[2])
      4: data = fltarr(dim[0],dim[1],dim[2])
      5: data = dblarr(dim[0],dim[1],dim[2])
      ELSE: BEGIN
         img.fname = ''
         msg = 'ILLEGAL data type for ' + fname
         RETURN, -1
      ENDELSE
   ENDCASE

   IF (img.data NE ptr_new()) THEN ptr_free, img.data
   openr,lun,fname

   offset = long(frm-1)*dim[0]*dim[1]*dim[2]*fix(nbytes) + slice
   CASE img.ntype OF
      1: temp = assoc(lun, bytarr(dim[0],dim[1],dim2), offset)
      2: temp = assoc(lun, intarr(dim[0],dim[1],dim2), offset)
      3: temp = assoc(lun, lonarr(dim[0],dim[1],dim2), offset)
      4: temp = assoc(lun, fltarr(dim[0],dim[1],dim2), offset)
      5: temp = assoc(lun, dblarr(dim[0],dim[1],dim2), offset)
   ENDCASE
   data[*,*,*] = temp[0]
   nn = where(finite(data) EQ 0, count)
   IF (count GT 0) THEN data[nn] = 0

   IF (revz AND (lp NE fp)) THEN data = reverse(data,3)
   IF (revy) THEN data = reverse(data,2)
   IF (revx) THEN data = reverse(data,1)
   IF (img.scale NE 1.0) THEN data = img.scale * data
   dptr = ptr_new(data)
   close,lun
   free_lun,lun
   
   IF (revx EQ 0 AND revy EQ 0 AND revz EQ 0) THEN img.order = 0 $
   ELSE IF (revx EQ 1 AND revy EQ 1 AND revz EQ 1) THEN img.order = 1 $
   ELSE IF (revx EQ 1 AND revy EQ 0 AND revz EQ 0) THEN img.order = 2 $
   ELSE img.order = -1
   
   img.gcal = 1.0
   img.ical = 1.0
   FOR k=0,8 DO img.mcal[k] = 0.0
   img.start = 0
   img.len = 0
   img.min = min(data)
   img.max = max(data)
   img.data = dptr
   RETURN, 0
END
