;       $Revision:   1.1  $ $Date:   10/15/93 18:17:56  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
;	MATCHNUMS	
;
; PURPOSE:
;	Change matnums and mats so that only matrices actually
;	in the directory dir are represented in matnums and mats.			
;
; CATEGORY:
;	CTI Matrix utility			
;
; CALLING SEQUENCE:	
;	matchnums, dir, matnums, mats	
;
; INPUTS:
;	dir -
;	matnums -
;	mats - 			
;
; OUTPUTS:	
;	matnums -
;	mats - 		
;
; HISTORY NOTES:		
;-

PRO matchnums, dir, matnums, mats

   on_error, 2
   IF (n_params() NE 3) THEN BEGIN
      set_ecat_error, 2, "MATCHNUMS"
      RETURN
   ENDIF
   FOR i = 0, n_elements(matnums)-1 DO BEGIN
      ind = where(matnums(i) EQ dir(0, *))
      IF (ind(0) EQ -1) THEN BEGIN
         matnums(i) = 0
         mats(*, i) = 0
      ENDIF
   ENDFOR
   ind = where(matnums GT 0)
   IF (ind(0) EQ -1) THEN BEGIN
      matnums = !ECAT.ERROR
      mats = !ECAT.ERROR
      set_ecat_error, 14, "MATCHNUMS"
      RETURN
   ENDIF
   matnums = matnums(where(matnums GT 0))
   ind = where((mats(0, *) GT 0) GT 0)
   IF (ind(0) EQ -1) THEN RETURN
   mats = mats(*, ind)
END
