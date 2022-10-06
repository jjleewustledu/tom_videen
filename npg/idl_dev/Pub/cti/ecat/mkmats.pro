;       $Revision:   1.1  $ $Date:   10/15/93 18:18:12  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:		
;	MKMATS
;
; PURPOSE:	
;	Returns the matrix numbers corresponding to frame, plane, gate,
;	data, and bed. 	
;
; CATEGORY:			
;
; CALLING SEQUENCE:		
;	result =  mkmats (f,p,g,d,b,dir,NEW=new)
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION   mkmats, f, p, g, d, b, dir, NEW = new

   on_error, 2
   IF (n_params() NE 6) THEN BEGIN
      set_ecat_error, 2, "MKMATS"
      RETURN, [!ECAT.ERROR]
   ENDIF
   
   nf = n_elements(f)
   np = n_elements(p)
   ng = n_elements(g)
   nd = n_elements(d)
   nb = n_elements(b)
   n  = nf*np*ng*nd*nb
   IF (n LE 0) THEN BEGIN
      set_ecat_error, 3, "MKMATS"
      RETURN, !ECAT.ERROR
   END

   mats = lonarr(5, n)

; FILL FRAME INDICES

   ind = replicate(f(0), np)
   FOR i=1,nf-1 DO ind = [ind,replicate(f(i), np)]
   lim = ng*nb*nd & ctr = 0 & inc = nf*np
   FOR i = 1, lim DO BEGIN
      mats(0, ctr:ctr+inc-1) = ind
      ctr = ctr+inc
   END

; FILL PLANE INDICES

   lim = nf*ng*nb*nd
   ind = p
   FOR i=2, lim DO ind = [ind, p]
   mats(1, *) = ind

; FILL GATE INDICES

   lim = np*nf
   ind = replicate(g(0), lim)
   FOR i=1,ng-1 DO ind = [ind, replicate(g(i),lim)]
   lim = nb*nd & ctr = 0 & inc = np*nf*ng
   FOR i = 1, lim DO BEGIN
      mats(2, ctr:ctr+inc-1) = ind
      ctr = ctr + inc
   END

; FILL DATA INDICES

   lim = np*nf*ng*nb
   ind = replicate(d(0), lim) 
   FOR i=1,nd-1 DO ind = [ind, replicate(d(i),lim)]
   mats(3, *) = ind

; FILL BED INDICES

   lim = np*nf*ng
   ind = replicate(b(0), lim)
   FOR i=1,nb-1 DO ind = [ind, replicate(b(i),lim)]
   lim = nd & ctr = 0 & inc = np*nf*ng*nb
   FOR i = 1, lim DO BEGIN
      mats(4, ctr:ctr+inc-1) = ind
      ctr = ctr + inc
   END

; REMOVE MISSING MATRICES

   matnums = matnum(mats(0, *), mats(1, *), mats(2, *), mats(3, *), mats(4, *))
   IF (keyword_set(NEW)) THEN BEGIN
      list = lonarr(7, n_elements(matnums))
      FOR i = 0, n_elements(matnums)-1 DO BEGIN
         list(*,i) = [matnums(i), 0L, mats(*,i)]
      ENDFOR
   ENDIF ELSE BEGIN
      matchnums, dir, matnums, mats
      if (mats(0) EQ -1) THEN RETURN, !ECAT.ERROR
      list = lonarr(7, n_elements(matnums))
      FOR i = 0, n_elements(matnums)-1 DO BEGIN
         off = dir(4, where(dir(0, *) EQ matnums(i)))
         list(*,i) = [matnums(i), off, mats(*,i)]
      ENDFOR
   ENDELSE
   RETURN, list
END
