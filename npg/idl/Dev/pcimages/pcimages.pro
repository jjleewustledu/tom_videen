PRO PCIMAGES
@IMAGE_UTILS.h

   on_error, 2
   list = ''
   outfile = ''
   mskfile = ''
   msg = ''

   list = getenv('INFILE')
   mskfile = getenv('MSKFILE')
   outfile = getenv('OUTFILE')

   ;; Read image list

   openr,lu1, list, ERROR=err, /GET_LUN
   IF (err NE 0) THEN BEGIN
      PRINT,'ERROR [pcimages]: Cannot open ',list
      RETURN
   ENDIF
   nimg = 0L
   fname = ''
   readf, lu1, nimg
   PRINT,list,' has ',nimg,' files'

   msk = {VIMAGE}
   msk.fname = mskfile
   msk.ff = 1
   msk.lf = 1
   msk.fp = 1
   msk.lp = 256
   IF (getframe(msk, msg) NE 0) THEN BEGIN
      PRINT,'ERROR [pcimages]: reading ',mskfile
      RETURN
   ENDIF
   print, format = '("Mask File:  ",A)', mskfile

   out = {VIMAGE}
   img = replicate({VIMAGE}, nimg)
   
   FOR i=0,nimg-1 DO BEGIN
      readf, lu1, fname
      out.fname = fname
      out.ff = 1
      out.lf = 1
      out.fp = 1
      out.lp = 256
      IF (getframe(out, msg) NE 0) THEN BEGIN
         PRINT,'ERROR [pcimages]: reading ',fname
         RETURN
      ENDIF
      img[i] = out
      print, format = '("Image File:  ",A)', fname
      img[i].data = ptr_new(*out.data)
   ENDFOR
   close,lu1
   free_lun,lu1

   out.fname = img[0].fname
   out.ff = 1
   out.lf = 1
   out.fp = 1
   out.lp = 256
   IF (getframe(out, msg) NE 0) THEN BEGIN
      PRINT,'ERROR [pcimages]: reading ',fname
      RETURN
   ENDIF

   len =  img[0].dim[0] * img[0].dim[1] * img[0].dim[2]

   ;; Get pixels comprising VOI

   i = 0
   data = reform(*img[i].data,len)
   mask = reform(*msk.data,len)
   pixels = where(mask GT 0 AND data NE 0, count)
   print, format = '("Mask:  ",A,3X,I0," voxels")', mskfile, count
   IF (count LE 0) THEN BEGIN
      PRINT,'ERROR [pcimages]: No pixels defined in ',mskfile
      RETURN
   ENDIF

   ndata = reform(*out.data,len)
   ndata[*] = 0
   ndata[pixels] = 1000*alog(data[pixels])
   *out.data = reform(ndata,img[0].dim)

   ;; Write output image

   extra = strarr(5)
   extra[0] = 'program name       := pcimages'
   extra[1] = 'program version    := 1.0'
   extra[2] = 'program date       := 2000:05:03'
   extra[3] = 'input file 1       := '+fname+'
   extra[4] = 'input file 2       := '+fname+'
   out.fname = outfile
   print,'Writing '+out.fname+'... '
   IF (PUTINTERFILE(out, outfile, type(0), msg, EXTRA=extra) NE 0) THEN message, msg
   print,'(ignore message -- "Floating illegal operand")'

END 
