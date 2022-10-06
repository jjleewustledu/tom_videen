FUNCTION GETANALYZE, img, msg
   COMMON IMAGE_UTILS
   
   fname = img.fname
   IF (strlen(fname) EQ 0) THEN BEGIN
      msg =  'No file entered'
      RETURN, -1
   ENDIF 
   frm = img.f
   k = rstrpos(fname,'.img')
   hname = fname
   strput, hname,'.hdr', k

   openr,lun, hname, ERROR=err, /GET_LUN
   IF (err NE 0) THEN BEGIN
      msg = 'Cannot open '+fname
      RETURN, -1
   ENDIF
   hdr = ana
   readu, lun, hdr, TRANSFER_COUNT=count
   close, lun
   free_lun, lun
   
   msg = 'Error reading '+fname
   IF (count NE 1) THEN RETURN, -1
   IF (hdr.dime.dim[0] LT 3) THEN RETURN, -1
   
   CASE hdr.dime.bitpix OF
      8: BEGIN 
         img.ntype = type(0B)
         nbytes = 1
      ENDCASE
      16: BEGIN
         img.ntype = type(0)
         nbytes = 2
      ENDCASE 
      32: BEGIN
         img.ntype = type(0.0)
         nbytes = 4
      ENDCASE 
      ELSE: BEGIN
         msg = 'Unrecognized number format: bits/pix = '+str(hdr.dim.bitpix)+' in '+fname
         close, lun
         free_lun, lun
         RETURN, -1
      ENDELSE 
   ENDCASE
  
   dim = lonarr(4)
   FOR k=0,3 DO dim[k] = hdr.dime.dim[k+1]
   
   fp = img.fp
   lp = img.lp
   IF (lp GT dim[2]) THEN lp = dim[2]
   IF (fp GT lp OR fp LT 1) THEN BEGIN
      msg = 'Cannot read plane '+string(fp)+' from ' + fname
      RETURN, -1
   ENDIF 
   dim2 = long(lp-fp+1)
   fpln = dim[2]-lp
   slice = long(fpln)*dim[0]*dim[1]*nbytes
   ff = img.ff
   lf = img.lf
   IF (lf GT dim[3]) THEN lf = dim[3]
   IF (ff GT lf) THEN BEGIN
      msg = 'Cannot read frame '+string(ff)+' from ' + fname
      RETURN, -1
   ENDIF 
   dim3 = long(ff-lf+1)
   img.dim[0] = dim[0]
   img.dim[1] = dim[1]
   img.dim[2] = dim2
   img.dim[3] = dim3
   
   CASE img.ntype OF
      1: data = bytarr(dim[0],dim[1],dim2,dim3)
      2: data = intarr(dim[0],dim[1],dim2,dim3)
      3: data = lonarr(dim[0],dim[1],dim2,dim3)
      4: data = fltarr(dim[0],dim[1],dim2,dim3)
      ELSE: BEGIN
         img.fname = ''
         msg = 'ILLEGAL data type for ' + fname
         RETURN, -1
      ENDELSE
   ENDCASE

   IF (img.data NE ptr_new()) THEN ptr_free, img.data
   openr,lun,fname,/get_lun
   
   FOR k=ff-1, lf-1 DO BEGIN 
      offset = long(k)*dim[0]*dim[1]*dim[2] + slice
      CASE img.ntype OF
         1: temp = assoc(lun, bytarr(dim[0],dim[1],dim2),offset)
         2: temp = assoc(lun, intarr(dim[0],dim[1],dim2),offset)
         3: temp = assoc(lun, lonarr(dim[0],dim[1],dim2),offset)
         4: temp = assoc(lun, fltarr(dim[0],dim[1],dim2),offset)
      ENDCASE
      data[*,*,*,k] = temp[0]
      nn = where(finite(data) EQ 0, count)
      IF (count GT 0) THEN data[nn] = 0
   ENDFOR 
   IF (lp GT fp) THEN data = reverse(data,3)
   data = reverse(data,2)
   data = reverse(data,1)
   dptr = ptr_new(data)
   close,lun
   free_lun,lun
   
   img.fp = fp
   img.lp = lp
   img.ff = ff
   img.lf = lf
   img.itype = ANALYZE
   img.order = 1
   img.pdim[0] = hdr.dime.pixdim[1]
   img.pdim[1] = hdr.dime.pixdim[2]
   img.pdim[2] = hdr.dime.pixdim[3]
   img.org[0] = 0
   img.org[1] = 0
   img.org[2] = 0
   img.scale =  1.0
   img.gcal = 1.0
   img.ical = 1.0
   FOR k=0,8 DO img.mcal[k] = 0.0
   img.start = 0
   img.len = 1
   img.min = hdr.dime.glmin
   img.max = hdr.dime.glmax
   img.data = dptr
   RETURN, 0
END
