FUNCTION GETPETT6A, img, msg
   COMMON IMAGE_UTILS
   
   fname = img.fname
   IF (strlen(fname) EQ 0) THEN BEGIN
      msg =  'No file entered'
      RETURN, -1
   ENDIF
  
   ff = 0
   str=''
   dim = lonarr(3)
   ffrm = img.ff
   lfrm = img.lf
   fpln = img.fp
   lpln = img.lp
   p6 = {PETT6header}
   PETT6header_init, p6
   IF (img.data NE ptr_new()) THEN ptr_free, img.data
   
print,"getpett6a"
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
   frames = lfrm-ffrm+1
   planes = lpln-fpln+1

   readu,lun,p6
help,p6,/str
   dptr = ptr_new(fltarr(dim[0],dim[1],planes,frames),/allocate)
   readu,lun,*dptr
   IF (frames EQ 1) THEN BEGIN
      img.start[ffrm] = (ffrm-1)*long(p6.time_per_frame+1)
      img.len[ffrm] = p6.time_per_frame
      img.min = 0
      img.max = 0
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

   img.fname = fname
   img.h.p6 = p6
   img.ntype = type(0.0)
   img.itype = PETT6
   img.order = 1
   img.dim[0] = dim[0]
   img.dim[1] = dim[1]
   img.dim[2] = planes
   img.dim[3] = frames
   print,"slices =", p6.number_of_slices
   IF (p6.number_of_slices LE 7) THEN BEGIN
      img.pdim[0] = 2.86
      img.pdim[1] = 2.86
      img.pdim[2] = 14.4
   ENDIF ELSE IF (p6.number_of_slices EQ 14) THEN BEGIN
      img.pdim[0] = 2.86
      img.pdim[1] = 2.86
      img.pdim[2] = 7.2
   ENDIF ELSE BEGIN
      img.pdim[0] = 2.0
      img.pdim[1] = 2.0
      img.pdim[2] = 2.0
   ENDELSE
   img.org[0] = 0
   img.org[1] = 0
   img.org[2] = 0
   img.scale =  1.0
   img.gcal = 1.0
   img.ical = 1.0
   FOR j=0,8 DO img.mcal[j] = 0.0
   img.data = dptr
   RETURN, 0
END
