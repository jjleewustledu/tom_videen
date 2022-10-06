;       $Revision:   1.1  $ $Date:   10/15/93 18:42:04  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	WPM6	
;
; PURPOSE:	
; 	Write ecat6 polar map data file		
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:	
; 	result = wpm6 (file,f,p,g,d,b,sh,data,MH=mh,NEW=new,
; 			ONLY_MH=only_mh,ONLY_SH=only_sh	)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION wpm6, file, f, p, g, d, b, sh, data, MH=mh, NEW=new, ONLY_MH=only_mh, ONLY_SH=only_sh

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 8) THEN BEGIN
      set_ecat_error, 1, "WPM6", FILE=file
      RETURN, !ECAT.ERROR
   ENDIF

   mhkey = keyword_set(MH)
   newkey = keyword_set(NEW)
   shonlykey = keyword_set(SH_ONLY)
   IF ((NOT mhkey) AND newkey) THEN BEGIN
      set_ecat_error, 1, "WPM6", FILE=file
      RETURN, !ECAT.ERROR
   ENDIF
   IF (newkey AND shonlykey) THEN BEGIN
      set_ecat_error, 1, "WPM6", FILE=file
      RETURN, !ECAT.ERROR
   ENDIF

   IF (newkey) THEN BEGIN
      openw, unit, file, /get_lun
      IF (wmh6(unit, mh) EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
      IF (mknewdirblk6(unit, dir, dirinfo, /NEW) EQ !ECAT.ERROR) THEN $
         RETURN, !ECAT.ERROR
   ENDIF ELSE BEGIN
      openu, unit, file, /get_lun
      IF (rdir6(unit, dir, dirinfo) EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
      print, dir
      IF (mhkey) THEN    $
         IF (wmh6(unit, mh) EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
   ENDELSE
   
   IF (NOT keyword_set(ONLY_MH)) THEN BEGIN

      dvals = [n_elements(f), n_elements(p), n_elements(g), $
               n_elements(d), n_elements(b)]
      print, 'dvals: ', dvals
      print, f, p, g, d, b
      expand_mat, f, p, g, d, b, dvals
      print, 'called expand_mat' 
      print, f, p, g, d, b
      print, 'dvals: ', dvals 
      IF (dvals(0) EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
      IF (newkey) THEN   $
         mats = mkmats(f, p, g, d, b, dir, /NEW)  $
      ELSE $
         mats = mkmats(f, p, g, d, b, dir)
      print, 'mats:', mats
      
      IF (mats(0) EQ !ECAT.ERROR) THEN BEGIN
         set_ecat_error, 0, "WPM6", UNIT=unit
         RETURN, !ECAT.ERROR
      ENDIF
      mats = mats(0, *)
      IF (n_elements(mats) NE n_elements(sh)) THEN BEGIN
         set_ecat_error, 9, "WPM6", UNIT=unit
         RETURN, !ECAT.ERROR
      ENDIF

      ndatablks = 0
      scales = 0
      IF (NOT shonlykey) THEN BEGIN
         s = size(data)

         IF (newkey) THEN BEGIN
            ftype = mh.file.file_type
         ENDIF ELSE BEGIN
            m = matval(dir(0, 0))
            IF (m(0) EQ !ECAT.ERROR) THEN BEGIN
               set_ecat_error, 12, "WPM6", UNIT=unit
               RETURN, !ECAT.ERROR
            ENDIF
            stat = recat6(file, m(0), m(1), m(2), m(3), m(4), d, /NO_DATA)
            IF (stat EQ !ECAT.ERROR) THEN BEGIN
               set_ecat_error, 0, "WPM6", UNIT=unit
               RETURN, !ECAT.ERROR
            ENDIF
            
            ftype = d.mh.file.file_type
         ENDELSE
         print, mats
         stat = wmats6(unit, ftype, sh, data, mats(0, *), dir, dirinfo, SH_ONLY=shonlykey)
         IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
      ENDIF
   ENDIF

DONE:
   free_lun, unit
   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "WPM6", UNIT=unit
   RETURN, !ECAT.ERROR
   
END
