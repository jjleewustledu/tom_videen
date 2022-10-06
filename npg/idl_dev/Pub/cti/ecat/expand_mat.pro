;       $Revision:   1.1  $ $Date:   10/15/93 18:17:14  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:		
;	EXPAND_MAT
;
; PURPOSE:
;	Handles the case where a "*" is input for frames, planes, gates 
;	or beds.			
;
; CATEGORY:	
;	CTI Matrix Utility		
;
; CALLING SEQUENCE:		
;	expand_mat,f,p,g,d,b,dvals
;
; INPUTS:	
;	dvals - matrix specification		
;
; OUTPUTS:
;	f -
;	p - 
;	g - 
;	d - 
;	b -			
;
; HISTORY NOTES:		
;-

PRO	expand_mat, f, p, g, d, b, dvals

   on_error, 2
   IF (n_params() NE 6) THEN BEGIN
      set_ecat_error, 2, "EXPAND"
      dvals = !ECAT.ERROR
      RETURN
   ENDIF
   
   maxf = dvals(0)
   maxp = dvals(1)
   maxg = dvals(2)
   maxd = dvals(3)
   maxb = dvals(4)
   sf = string(f)
   sp = string(p)
   sg = string(g)
   sd = string(d)
   sb = string(b)
   ind = where(sf EQ "*", cnt) & IF (cnt GT 0) THEN f = lindgen(maxf)+1
   ind = where(sp EQ "*", cnt) & IF (cnt GT 0) THEN p = lindgen(maxp)+1
   ind = where(sg EQ "*", cnt) & IF (cnt GT 0) THEN g = lindgen(maxg)+1
   ind = where(sd EQ "*", cnt) & IF (cnt GT 0) THEN d = lindgen(maxd)
   ind = where(sb EQ "*", cnt) & IF (cnt GT 0) THEN b = lindgen(maxb)
END
