;       $Revision:   1.1  $ $Date:   10/15/93 18:13:26  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
;	DIFFVAL	
;
; PURPOSE:			
;	DIFFVAL returns an array with the total number of different frames, 
;	planes, gates, data, and beds for the directory given. DIFFVAL will 
;	return the total number of different values in any vector, or in each 
;	column of an array.
;
; CATEGORY:			
;	CTI Matrix read utility.
;
; CALLING SEQUENCE:		
;	Stat =  DIFFVAL (Dir, NOT_DIR=Not_Dir)
;
; INPUTS:
;	Dir - matrix directory entry, unless the not_dir keyword is set.
;				
;
; KEYWORD PARAMETERS:
;	Not_Dir - this keyword indicates that the dir parameter is a vector
;		to be searched.		
;
; OUTPUTS:	
;	Stat - 	the total number of different frames, planes, gates, data, 
;		and beds for the directory given OR the total number of 
;		different values in any vector, or in each column of an array.	
;
;-

FUNCTION DIFFVAL, Dir, NOT_DIR = Not_Dir

   on_error, 2
   IF (n_params() NE 1) THEN BEGIN
      set_ecat_error, 2, "DIFFVAL"
      RETURN, !ECAT.ERROR
   ENDIF

   IF (NOT keyword_set(NOT_DIR)) THEN BEGIN
      ind = where(dir(0, *) GT 0)
      IF (ind(0) LT 0) THEN ind = 0
      matnums = dir(0, ind)
      fpgdb = matval(matnums)
      fpgdb = reform(fpgdb, 5, n_elements(fpgdb)/5)
      s = size(fpgdb)
      count = s(1)
   ENDIF ELSE 	$
    fpgdb = dir
   IF (keyword_set(NOT_DIR)) THEN BEGIN
      s = size(fpgdb)
      IF (s(0) EQ 0) THEN RETURN, fpgdb
      IF (s(0) EQ 1) THEN BEGIN		$
       IF (s(1) EQ 1) THEN RETURN, fpgdb	$
      ELSE count = 1 & fpgdb = transpose(fpgdb)
      ENDIF ELSE count = s(1)
   ENDIF
   dvals = lonarr(count)
   FOR j = 0, count-1 DO BEGIN
      n = fpgdb(j, *)
      n = n(sort(n))
      count = 1
      FOR i = 0, n_elements(n)-2 DO BEGIN
         IF (n(i) NE n(i+1)) THEN count = count +1
      ENDFOR
      dvals(j) = count
   ENDFOR
   RETURN, dvals
END
