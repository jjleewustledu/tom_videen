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

;  Read original and reference images

   img = {VIMAGE}
   img.fname = infile
   img.ff = 1
   img.lf = 1
   img.fp = 1
   img.lp = 300
   msg = ''
   print,'Reading original image'
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
   print,'Reading reference image'
   IF (GETFRAME(ref, msg) NE 0) THEN BEGIN
      message, msg
      exit 
   ENDIF 
   rdata = *ref.data

;  Compute ratio of ref/img

   nx = img.dim[0]
   ny = img.dim[1]
   nz = img.dim[2]
   ;fdata = fltarr(nx,ny,nz)
   fdata = fltarr(nx,ny)
   k = where (idata NE 0)
   IF (k[0] NE -1) THEN fdata[k] = float(rdata[k])/float(idata[k])
   k = where (idata EQ 0)
   IF (k[0] NE -1) THEN fdata[k] = 0.
   print,format='("In  =",f12.3,"   Ref =",f12.3, "   In/Ref  =",f6.3)',$
		mean(idata),mean(rdata),1./mean(fdata)

;  Determine basis points for fit
;  and compute local averages at these points

   ns = 4
   xinc = nx/ns
   yinc = ny/ns
   nb = (ns+1)^2
   ;basis = lonarr(nb,3)
   basis = lonarr(nb,2)
   n = 0
   FOR j = 0,ny-1+yinc,yinc DO BEGIN
      jm = j < (ny-1)
      FOR i = 0,nx-1+xinc,xinc DO BEGIN
         im = i < (nx-1)
         ;basis[n,*] = [i,j,0]
         basis[n,*] = [im,jm]
         n = n+1
      ENDFOR
   ENDFOR

;  Evaluate ratio at the basis points 

   ;val = fdata[basis[*,0],basis[*,1],basis[*,2]]
   val = fdata[basis[*,0],basis[*,1]]

;  Fit a multiquadratic curve to the basis points

   w = s3dfit(basis, val)
   IF (w[0] EQ -1) THEN BEGIN
      message, 'Matrix Inversion Error"
      exit
   ENDIF 

;  Compute the multiquadratic fit to all points in the ratio image, then
;  multiply the fit times the input image (to normlize it)


   const = 1.
   FOR k=0,nz-1 DO BEGIN
      FOR j=0,ny-1 DO BEGIN
         FOR i=0,nx-1 DO BEGIN
            ;f = s3deval([i,j,k], basis, w, const)
            ;idata[i,j,k] = f*idata[i,j,k]
            f = s3deval([i,j], basis, w, const)
            idata[i,j] = f*idata[i,j]
         ENDFOR
      ENDFOR
   ENDFOR

;  Write normalized image

   *img.data = idata
   extra = strarr(5)
   extra[0] = 'program name    := multinorm'
   extra[1] = 'program version := 1.1'
   extra[2] = 'program date    := 2002:09:05'
   extra[3] = 'input file      := '+infile
   IF (PUTINTERFILE(img, outfile, msg, EXTRA=extra) NE 0) THEN BEGIN
      message, msg
      exit
   ENDIF 
   print,format='("Out =",f12.3,"   Ref =",f12.3, "   Out/Ref =",f6.3)',$
		mean(idata),mean(rdata),mean(rdata)/mean(idata)

END
