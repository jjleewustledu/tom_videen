FUNCTION GETECAT6, img, msg
   COMMON IMAGE_UTILS
   
   fname = img.fname
   IF (strlen(fname) EQ 0) THEN BEGIN
      msg =  'No file entered'
      RETURN, -1
   ENDIF 

   str = ''
   dim = intarr(3)
   ffrm = img.ff
   lfrm = img.lf
   fpln = img.fp
   lpln = img.lp
   mh6 = {M6header}
   sh6 = {Subheader6_img}
   M6header_init, mh6, sh6
   IF (img.data NE ptr_new()) THEN ptr_free, img.data
   
   cmd = '/usr/local/npg/bin/dynimage_read ' $
      + fname + string(ffrm) + string(lfrm) + string(fpln) + string(lpln)
   spawn,cmd,unit=lun
   k = 0
   str = ''
   readf,lun,str,format='(A)'
   reads,str,k,format='(I0)'
   IF (k EQ -1) THEN BEGIN 
      msg = 'Read Error: ' + str
      RETURN, -1
   ENDIF
   
   firstfrm = 0L
   lastfrm = 0L
   firstpln = 0L
   lastpln = 0L
   k0 = 0L
   k1 = 0L
   k2 = 0L
   readf,lun,str,format='(A)'
   reads,str, ffrm, lfrm, format='(2I0)'
   readf,lun,str,format='(A)'
   reads,str, fpln, lpln, format='(2I0)'
   readf,lun,str,format='(A)'
   reads,str,k0,k1,k2,format='(3I0)'
   dim[0] = k0
   dim[1] = k1
   dim[2] = k2
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

   readu,lun,mh6
   readu,lun,sh6
   dptr = ptr_new(fltarr(dim[0],dim[1],planes,frames),/allocate)
   readu,lun,*dptr
   IF (frames EQ 1) THEN BEGIN
      img.start[ffrm] = sh6.frame_start_time
      img.len[ffrm] = sh6.frame_duration
      img.min = sh6.image_min
      img.max = sh6.image_max
   ENDIF ELSE BEGIN
      start = 0.0
      len = 0.0
      FOR k=ffrm, lfrm DO BEGIN 
         readf,lun,str,format='(A)'
         reads, str, start, len, format='(2I0)'
         img.start[k] = start
         img.len[k] = len
      ENDFOR 
      img.min = min(*dptr)
      img.max = max(*dptr)
   ENDELSE 
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
   img.dim[2] = planes
   img.dim[3] = frames
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
