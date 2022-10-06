;+
; NAME:
;	XYPTNORM
;
; PURPOSE:
;	Normalizes axially a 3D image using all points within a mask image
;
; CATEGORY:
;	Image processing.
;
; INPUTS:
; 	INFILE:		Image to normalize.
; 	NRM:		Image of points to use in normalization.
;
; OUTPUT:
;	OUTFILE:	Normalized image.
;
; PROCEDURE:
;
; MODIFICATION HISTORY:
;	Feb 2008 - Tom Videen
;-

PRO XYPTNORM
@IMAGE_UTILS.h

   on_error,2
   infile = getenv('INFILE')
   nrmfile = getenv('NRMFILE')
   outfile = getenv('OUTFILE')

   IF (infile EQ '') THEN BEGIN 
      message,'No input file specified'
      exit
   ENDIF ELSE IF (nrmfile EQ '') THEN BEGIN 
      message,'No normalization file specified'
      exit
   ENDIF ELSE IF (outfile EQ '') THEN BEGIN 
      message,'No output file specified'
      exit
   ENDIF

;  Open output image

   str1 = outfile
   l1 = rstrpos(str1,'.')
   IF (l1 GT 0) THEN strput,str1,'log',l1+1 $
   ELSE logfile = logfile+'.log'
   logfile = strcompress(str1, /remove_all)
   get_lun,log_lun
   openw,log_lun,logfile
   printf,log_lun,'Output image ',outfile

;  Read input image

   img = {VIMAGE}
   img.fname = infile
   img.ff = 1
   img.lf = 1
   img.fp = 1
   img.lp = 300
   msg = ''
   print,'Original image ',infile
   printf,log_lun,'Original image ',infile
   stat =  getframe(img, msg)
   IF (stat NE 0) THEN BEGIN
      message, msg
      exit 
   ENDIF 
   idata = *img.data
   nx = img.dim[0]
   ny = img.dim[1]
   nz = img.dim[2]

;  Read normalizing image

   nrm = {VIMAGE}
   nrm.fname = nrmfile
   nrm.ff = 1
   nrm.lf = 1
   nrm.fp = 1
   nrm.lp = 300
   msg = ''
   print,'Normalizing image ',nrmfile
   printf,log_lun,'Normalizing image ',nrmfile
   stat =  getframe(nrm, msg)
   IF (stat NE 0) THEN BEGIN
      message, msg
      exit 
   ENDIF 
   ndata = *nrm.data
   IF (nx NE nrm.dim[0] OR ny NE nrm.dim[1] OR nz NE nrm.dim[2]) THEN
      print,'Dimension of '+nrmfile+' do not match '*infile
      exit
   ENDIF

;  -----------------------------------------------------------
;  Fit surface to plane 65 of nrm

   w = ndata
   idx = where (ndata GT 0, count) 
   if (count GT 0) w[idx] = 1
   result = mapfitw(ndata,1,w)

WINDOW, XSIZE = 1600, YSIZE = 800
!P.MULTI = [0, 2, 1]					;Set up side-by-side plots.
SURFACE, ndata, ZRANGE = [0, 20000], ZSTYLE = 1
SURFACE, result

   coef = poly_fit(zc,val,2,vfil)
   print,coef

   printf, val_lun, npt, 5
   fitval = fltarr(npt)
   FOR n=1,npt DO BEGIN
      z = zc[n-1]
      fitval[n-1] = poly(z,coef)
      printf,val_lun,xc[n-1],yc[n-1],zc[n-1],val[n-1],fitval[n-1]
   ENDFOR
   close, val_lun
   free_lun, val_lun

;  Multiply the fit scaler times the input image

   FOR z=0,nz-1 DO BEGIN
      scl = meanval/poly(z,coef)
      idata[*,*,z] = scl*idata[*,*,z]
      printf,log_lun,format='("pln",i4,"  Scl =",f7.4)',z, scl
   ENDFOR

;  -----------------------------------------------------------
;  Write normalized image

   print,'Output image ',outfile
   WRITE_IMAGE:
   *img.data = idata
   extra = strarr(5)
   extra[0] = 'program name    := xyptnorm'
   extra[1] = 'program version := 1.1'
   extra[2] = 'program date    := 2008:02:05'
   extra[3] = 'input file      := '+infile
   IF (putinterfile(img, outfile, msg, EXTRA=extra) NE 0) THEN BEGIN
      message, msg
      exit
   ENDIF 
   print,msg
   close,log_lun
   free_lun,log_lun
END
