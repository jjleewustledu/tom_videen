;       $Revision:   1.2  $ $Date:   10/15/93 18:41:46  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	WMATS7	
;
; PURPOSE:	
; 	Write subheader and data into the file represented by unit.		
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:		
; 	result = wmats7 (unit,ftype,sh,data,mats,dir,dirinfo,
; 				SH_ONLY=sh_only)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION wmats7, unit, ftype, sh, data, mats, dir, dirinfo, SH_ONLY=sh_only

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 7) THEN BEGIN
      set_ecat_error, 2, "WMATS7", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF
   
   dtype = sh(0).data_type
   s = size(data(*, *, 0))
   scale = 0
   
   nblks = data_size(dtype, s(n_elements(s)-1))+1L
   IF ((!ECAT.file_types(ftype) EQ "IMG") OR  $
       (!ECAT.file_types(ftype) EQ "SCN")) THEN $
      scale = dscale(data)   $
   ELSE    $
      scale = replicate(1.0, n_elements(mats))

   FOR i=0, n_elements(mats)-1 DO BEGIN
      ind = where(dir(0, *) EQ mats(i), cnt)
      IF (cnt EQ 0) THEN BEGIN
         nextblk = nextfree(unit, mats(i), nblks, dir, dirinfo)
         IF (nextblk EQ !ECAT.ERROR) THEN BEGIN
            set_ecat_error, 0, "WMATS"
            RETURN, !ECAT.ERROR
         ENDIF
         off = (nextblk-1)*!ECAT.BLKSIZE
      ENDIF ELSE BEGIN
         off = dir(4, ind)
      ENDELSE
      IF (NOT keyword_set(SH_ONLY)) THEN BEGIN
         IF (!ECAT.file_types(ftype) EQ "IMG") THEN BEGIN
            imin = 0
            sh(i).image_max = max(data(*, *, i), min=imin)
            sh(i).image_min = imin
         ENDIF
         IF (!ECAT.file_types(ftype) EQ "SCN") THEN BEGIN
            smin = 0
            sh(i).scan_max = max(data(*, *, i), min=smin)
            sh(i).scan_min = smin
         ENDIF
      ENDIF
      stat = wsubheader7(unit, ftype, off, sh(i), SCALE=scale(i))
      IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
      IF (NOT keyword_set(SH_ONLY)) THEN BEGIN
         stat = wdata(unit, ftype, dtype, off+!ECAT.BLKSIZE, data(*, *, i))
         IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
      ENDIF
   ENDFOR

   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "WMATS7", UNIT=unit
   RETURN, !ECAT.ERROR

END
