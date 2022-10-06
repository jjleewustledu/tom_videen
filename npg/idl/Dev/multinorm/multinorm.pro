;+
; NAME:
;	MULTINORM
;
; PURPOSE:
;	Normalizes a 3D image using a multiquadric fit.
;
; CATEGORY:
;	Image processing.
;
; INPUTS:
; 	INFILE:		Image to normalize.
; 	REFFILE:	Reference image for normalization.
;
; OUTPUT:
;	OUTFILE:	Normalized image.
;
; PROCEDURE:
;	Ratio of Reffile to Infile computed at basis points.
;	Multiquadratic fit of this ratio at the basis points
; 	(using method of Wang, et al, SPIE 2001;4322:1411-1422).
;	Fit equation applied to all images points of 1 to normalize.
;
; MODIFICATION HISTORY:
;	Sep 2002 - Tom Videen
;-

PRO MULTINORM
@IMAGE_UTILS.h

   on_error,2
   infile = getenv('INFILE')
   reffile = getenv('REFFILE')
   outfile = getenv('OUTFILE')
   showpoints = getenv('SHOWPOINTS')
   rad = 1

   IF (infile EQ '') THEN BEGIN 
      message,'No input file specified'
      exit
   ENDIF ELSE IF (reffile EQ '') THEN BEGIN 
      message,'No reference file specified'
      exit
   ENDIF ELSE IF (outfile EQ '') THEN BEGIN 
      message,'No output file specified'
      exit
   ENDIF
   str1 = outfile
   l1 = rstrpos(str1,'.')
   IF (l1 GT 0) THEN strput,str1,'log',l1+1 $
   ELSE logfile = logfile+'.log'
   logfile = strcompress(str1, /remove_all)
   get_lun,log_lun
   openw,log_lun,logfile
   printf,log_lun,'Output image ',outfile

;  Read original and reference images

   img = {VIMAGE}
   img.fname = infile
   img.ff = 1
   img.lf = 1
   img.fp = 1
   img.lp = 300
   msg = ''
   print,'Original image ',infile
   printf,log_lun,'Original image ',infile
   IF (GETFRAME(img, msg) NE 0) THEN BEGIN
      message, msg
      exit 
   ENDIF 
   idata = *img.data

   ref = {VIMAGE}
   ref.fname = reffile
   ref.ff = 1
   ref.lf = 1
   ref.fp = 1
   ref.lp = 300
   msg = ''
   print,'Reference image ',reffile
   printf,log_lun,'Reference image ',reffile
   IF (GETFRAME(ref, msg) NE 0) THEN BEGIN
      message, msg
      exit 
   ENDIF 
   rdata = *ref.data

;  Compute ratio of ref/img

   nx = img.dim[0]
   ny = img.dim[1]
   nz = img.dim[2]
   indx = where(rdata GT 0)
   fdata = fltarr(nx,ny,nz)
   fdata[indx] = float(rdata[indx])/float(idata[indx])
   print,format='("In  =",f12.3,"   Ref =",f12.3, "   In/Ref  =",f6.3)',$
      mean(idata[indx]), mean(rdata[indx]), 1./mean(fdata[indx])

;  Determine basis points for fit

   time0 = systime(0,/seconds)
   print,format='(i6," sec:  basis_points")',0
   basis = basis_points(rdata, nx, ny, nz)
   nb = (size(basis))[1]
   print,"Basis Points ",nb
   printf,log_lun,"Basis Points ",nb

;  Evaluate ratio at the basis points 

   val = fltarr(nb)
   FOR n=0,nb-1 DO BEGIN
      x = basis[n,0]
      y = basis[n,1]
      z = basis[n,2]
      IF (showpoints EQ 1) THEN BEGIN
         idata[x-rad:x+rad, y-rad:y+rad, z-1:z+1] = 32767
      ENDIF ELSE BEGIN
         subr = rdata[x-rad:x+rad, y-rad:y+rad, z-1:z+1] 
         subi = idata[x-rad:x+rad, y-rad:y+rad, z-1:z+1] 
         idx = where(subr GT 0, count)
         if (count gt 0) then val[n] = total(subr[idx])/total(subi[idx])
         print, n, val[n], total(subr[idx]), total(subi[idx]), count
      ENDELSE
   ENDFOR
   IF (showpoints EQ 1) THEN goto,WRITE_IMAGE

;  Fit a multiquadratic curve to the basis points

   time1 = systime(0,/seconds)
   print,format='("+",i5," sec:  s3dfit")',time1-time0
   w = s3dfit(basis, val)
   IF (w[0] EQ -1) THEN BEGIN
      message, 'Matrix Inversion Error'
      exit
   ENDIF 

;  Compute the multiquadratic fit to all points in the ratio image, then
;  multiply the fit times the input image (to normlize it)

   time2 = systime(0,/seconds)
   print,format='("+",i5," sec:  s3deval")',time2-time1
   const = 1.
   tlast = systime(0,/seconds)
   FOR k=0,nz-1 DO BEGIN
      tnow = systime(0,/seconds)
      FOR j=0,ny-1 DO BEGIN
         FOR i=0,nx-1 DO BEGIN
            scl = s3deval([i,j,k], basis, w, const)
            IF ((j eq ny/2) AND (i eq nx/2)) THEN BEGIN
                print,format='("pln",i4,"  Scl =",f7.4,i8," sec")',k, scl, tnow-tlast
                printf,log_lun,format='("pln",i4,"  Scl =",f7.4,i8," sec")',k, scl, tnow-tlast
            ENDIF
            idata[i,j,k] = scl*idata[i,j,k]
         ENDFOR
      ENDFOR
      tlast=tnow
   ENDFOR

;  Write normalized image

   print,'Output image ',outfile
   time3 = systime(0,/seconds)
   print,format='("+",i5," sec:  write")',time3-time2
   WRITE_IMAGE:
   *img.data = idata
   extra = strarr(5)
   extra[0] = 'program name    := multinorm'
   extra[1] = 'program version := 1.1'
   extra[2] = 'program date    := 2002:09:05'
   extra[3] = 'input file      := '+infile
   IF (putinterfile(img, outfile, msg, EXTRA=extra) NE 0) THEN BEGIN
      message, msg
      exit
   ENDIF 
   print,msg
   fdata[indx] = float(rdata[indx])/float(idata[indx])
   print,format='("Out =",f12.3,"   Ref =",f12.3, "   Out/Ref =",f6.3)',$
      mean(idata[indx]), mean(rdata[indx]), 1./mean(fdata[indx])
   close,log_lun
   free_lun,log_lun
END
