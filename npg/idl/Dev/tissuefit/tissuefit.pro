; $Id$
;
;+
; NAME:
;   TISSUEFIT
;
; PURPOSE:
;   Curve fitting
;
; CATEGORY:
;   Image processing.
;
; CALLING SEQUENCE:
;   tissuefit filename
;
; MODIFICATION HISTORY:
;   Written by: Tom Videen, Dec 2001
;-

PRO triple_gauss, x, a, y, pder
   IF (a[2] NE 0.) THEN z1 = (x - a[1])/a[2] $
   ELSE z1 = 10.
   ez1 = exp(-z1^2/2)
   IF (a[5] NE 0.) THEN z2 = (x - a[4])/a[5] $
   ELSE z2 = 10.
   ez2 = exp(-z2^2/2)
   IF (a[8] NE 0.) THEN z3 = (x - a[7])/a[8] $
   ELSE z3 = 10.
   ez3 = exp(-z3^2/2)

   y = a[0] * ez1 + a[3] * ez2 + a[6] * ez3
   IF n_params() EQ 2 THEN RETURN

   pder = fltarr(n_elements(x),9)
   pder[0,0] = ez1
   IF (a[2] NE 0.) THEN pder[0,1] = a[0] * ez1 * z1/a[2] $
   ELSE pder[*,1] = [replicate(1.0,n_elements(x))]
   pder[0,2] = pder[*,1]*z1
   pder[0,3] = ez2
   IF (a[5] NE 0.) THEN pder[0,4] = a[3] * ez2 * z2/a[5] $
   ELSE pder[*,4] = [replicate(1.0,n_elements(x))]
   pder[0,5] = pder[*,4]*z2
   pder[0,6] = ez3
   IF (a[8] NE 0.) THEN pder[0,7] = a[6] * ez3 * z3/a[8] $
   ELSE pder[*,7] = [replicate(1.0,n_elements(x))]
   pder[0,8] = pder[*,7]*z3
END

FUNCTION gaussfn, x, a, n
   z = (x - a[n+1])/a[n+2]
   ez = exp(-z^2/2)
   y = a[n] * ez 
   RETURN, y
END

PRO TISSUEFIT

   fname = getenv('FILENAME')
   str = ''
   nbin = 0
   ncol = 0
   openr,lu,fname,/get_lun
   readf,lu,str
   readf,lu,nbin,ncol
   print,str,nbin,ncol
   crv = fltarr(ncol,nbin)
   readf,lu,crv
   close,lu
   free_lun,lu
   x = crv[0,*] + 0.5
   y = crv[2,*]
   A = fltarr(9)
   A[0] = 400.
   A[1] = 180.
   A[2] = 60.

   A[3] = 1000.
   A[4] = 520.
   A[5] = 60.

   A[6] = 2000.
   A[7] = 670.
   A[8] = 60.

   ;print,'     Amplitude    Location       Width'
   ;print,a[0:2]
   ;print,a[3:5]
   ;print,a[6:8]

   x1 = rotate(x,4)
   y1 = rotate(y,4)
   weights = y1 > 1.
   ;plot,x,y
   ;triple_gauss, x1, a, yfit1
   ;oplot,x1,yfit1
   ;read,prompt='continue',str

   yfit = curvefit(x1, y1, weights, a, iter=num, itmax=500, FUNCTION_NAME='triple_gauss')
   print,'Iterations',num
   print,'     Amplitude    Location       Width'
   print,'     ---------    --------      -------'
   print,a[0:2]
   print,a[3:5]
   print,a[6:8]
   plot,x,y
   oplot,x1,yfit

   yfit = gaussfn(x1,a,0)
   oplot,x1,yfit
   yfit = gaussfn(x1,a,3)
   oplot,x1,yfit
   yfit = gaussfn(x1,a,6)
   oplot,x1,yfit

   print,'Intersections of normalized curves'

	cg1 = (a[1]*a[5] - a[2]*a[4])/(a[5] - a[2])
	cg2 = (a[1]*a[5] + a[2]*a[4])/(a[5] + a[2])
	print, '   CSF/Gray   = ',cg1 > cg2

	gw1 = (a[4]*a[8] - a[5]*a[7])/(a[8] - a[5])
	gw2 = (a[4]*a[8] + a[5]*a[7])/(a[8] + a[5])
	print, '   Gray/White = ',gw1 < gw2

   read,prompt='end',str
END 

