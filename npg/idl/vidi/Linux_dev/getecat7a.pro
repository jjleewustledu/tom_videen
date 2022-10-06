FUNCTION GETECAT7A, img, msg
   
   COMMON IMAGE_UTILS
   
   fname = img.fname
   IF (strlen(fname) EQ 0) THEN BEGIN
      msg =  'No file entered'
      RETURN, -1
   ENDIF 

   str=''
   dim = lonarr(3)
   ffrm = img.ff
   lfrm = img.lf
   fpln = img.fp
   lpln = img.lp
   mh7 = {M7header}
   sh7 = {Subheader7_img}
   M7header_init, mh7, sh7
   IF (img.data NE ptr_new()) THEN ptr_free, img.data
   
	print,"getecat7a"
   cmd = '/home/npggw/tom/src/dynimage_read/dynread ' $
      + fname + string(ffrm) + string(lfrm) + string(fpln) + string(lpln)
   spawn,cmd,unit=lun
   k = 0
   str = ''
   readf,lun,k,str,format='(I0,A)'
   IF (k EQ -1) THEN BEGIN 
      msg = 'Read Error: ' + str
      RETURN, -1
   ENDIF
   
   firstfrm = 0L
   lastfrm = 0L
   firstpln = 0L
   lastpln = 0L
   readf,lun, ffrm, lfrm, format='(2I0)'
   readf,lun, fpln, lpln, format='(2I0)'
   readf,lun,dim,format='(3I0)'
   img.p  = fpln
   img.fp = fpln
   img.lp = lpln
   img.f  = ffrm
   img.ff = ffrm
   img.lf = lfrm
   ff = img.ff
   lf = img.lf
   frames = lfrm-ffrm+1
   planes = lpln-fpln+1
   
   readu,lun,mh7
   readu,lun,sh7
   
   dptr = ptr_new(fltarr(dim[0],dim[1],planes,frames),/allocate)
   readu,lun,*dptr
help,sh7,/str
   IF (frames EQ 1) THEN BEGIN
      img.start[ffrm] = sh7.frame_start_time
      img.len[ffrm] = sh7.frame_duration
      img.min = sh7.image_min
      img.max = sh7.image_max
   ENDIF ELSE BEGIN
      start = 0.0
      len = 0.0
      FOR k=ffrm, lfrm DO BEGIN 
         readf, lun, start, len, format='(2I0)'
         img.start[k] = start
         img.len[k] = len
      ENDFOR 
      img.min = min(*dptr)
      img.max = max(*dptr)
   ENDELSE 
   close,lun
   free_lun,lun
   
   sh7.data_type = 2            ; float
   img.h.mh7 = mh7
   img.h.sh7 = sh7
   
   CASE sh7.data_type OF
      1: img.ntype = type(0B)
      2: img.ntype = type(0)
      5: img.ntype = type(0.0)
      6: img.ntype = type(0)
   ENDCASE
   img.itype = MATRIX_REV7
   img.order = 1
   img.dim[0] = dim[0]
   img.dim[1] = dim[1]
   img.dim[2] = planes
   img.dim[3] = frames
   img.pdim[0] = 10*sh7.x_pixel_size
   img.pdim[1] = 10*sh7.y_pixel_size
   img.pdim[2] = 10*sh7.z_pixel_size
   img.org[0] = 0.5 * float(dim[0]+1)
   img.org[1] = 0.5 * float(dim[1]+1)
   comment = mh7.study_description
   a = strpos(comment,'z0:')
   IF (a GE 0) THEN BEGIN
      str = strmid(comment, a+4,3)
      img.org[2] = float(str)
   ENDIF ELSE img.org[2] = 0
   img.org[3] = 0
   img.scale =  1.0
   img.gcal = 1.0
   img.ical = 1.0
   FOR k=0,8 DO img.mcal[k] = 0.0
   img.data = dptr
   RETURN, 0
END
