FUNCTION GETECAT6FRM, img, msg
   COMMON IMAGE_UTILS
   
   fname = img.fname
   IF (strlen(fname) EQ 0) THEN BEGIN
      msg =  'No file entered'
      RETURN, -1
   ENDIF 

   str = ''
   dim = lonarr(3)
   ffrm = img.ff
   img.p  = 1
   img.fp = 1
   img.f  = ffrm
   img.ff = ffrm
   img.lf = ffrm
   frames = 1

   mh6 = {M6header}
   sh6 = {Subheader6_img}
   M6header_init, mh6, sh6
   IF (img.data NE ptr_new()) THEN ptr_free, img.data
   
   cmd = '/usr/local/npg/bin/dynimage_frm ' + fname + string(ffrm)
   spawn,cmd,unit=lun
   k = 0
   str = ''
   readf,lun,k,str,format='(I0,A)'
   IF (k EQ -1) THEN BEGIN 
      msg = 'Read Error: ' + str
      RETURN, -1
   ENDIF
   
   readf,lun,dim,format='(3I0)'
   img.lp = dim[2]
   planes = dim[2]
   
   readu,lun,mh6
   readu,lun,sh6
   
   dptr = ptr_new(fltarr(dim[0],dim[1],planes,frames),/allocate)
   readu,lun,*dptr
   img.start[ffrm] = sh6.frame_start_time
   img.len[ffrm] = sh6.frame_duration
   img.min = sh6.image_min
   img.max = sh6.image_max
   close,lun
   free_lun,lun
   
   sh6.data_type = 5            ; IEEE_R4
   img.h.mh6 = mh6
   img.h.sh6 = sh6

   CASE mh6.data_type OF
      1: img.ntype = type(0B)
      2: img.ntype = type(0)
      5: img.ntype = type(0.0)
      6: img.ntype = type(0)
   ENDCASE
   img.itype = MATRIX_REV6
   img.order = 1
   img.dim[0] = dim[0]
   img.dim[1] = dim[1]
   img.dim[2] = dim[2]
   img.dim[3] = 1
   img.pdim[0] = 10*sh6.pixel_size
   img.pdim[1] = 10*sh6.pixel_size
   IF (sh6.slice_width EQ 0) THEN img.pdim[2] = 3.375 $
   ELSE img.pdim[2] = 10*sh6.slice_width
   img.org[0] = 0.5 * float(dim[0]+1)
   img.org[1] = 0.5 * float(dim[1]+1)
   comment = mh6.study_description
   a = strpos(comment,'z0:') 
   IF (a GE 0) THEN BEGIN
      str = strmid(comment, a+4,3)
      img.org[2] = float(str)
   ENDIF ELSE img.org[2] = 0
   img.org[3] = 0
   img.scale =  1.0
   img.gcal = 1.0
   img.ical = 1.0
   FOR j=0,8 DO img.mcal[j] = 0.0
   img.data = dptr
   RETURN, 0
END
