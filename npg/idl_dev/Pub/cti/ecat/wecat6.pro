;       $Revision:   1.1  $ $Date:   10/15/93 18:41:26  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:		
; 	WECAT6
;
; PURPOSE:
; 	Write the main header, subheaders and data into the file.			
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:		
; 	result = wecat6 (file,f,p,g,d,b,sh,data,MH=mh,NEW=new, $
; 			ONLY_MH=only_mh,ONLY_SH=only_sh)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION wecat6, file, f, p, g, d, b, sh, data, MH=mh, NEW=new, ONLY_MH=only_mh, ONLY_SH=only_sh

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 8) THEN BEGIN
      set_ecat_error, 1, "WECAT6", FILE=file
      RETURN, !ECAT.ERROR
   ENDIF

   mhkey = keyword_set(MH)
   newkey = keyword_set(NEW)
   shonlykey = keyword_set(SH_ONLY)
   IF ((NOT mhkey) AND newkey) THEN BEGIN
      set_ecat_error, 1, "WECAT6", FILE=file
      RETURN, !ECAT.ERROR
   ENDIF
   IF (newkey AND shonlykey) THEN BEGIN
      set_ecat_error, 1, "WECAT6", FILE=file
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
      IF (mhkey) THEN    $
         IF (wmh6(unit, mh) EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
   ENDELSE
   
   IF (NOT keyword_set(ONLY_MH)) THEN BEGIN

      dvals = [n_elements(f), n_elements(p), n_elements(g), $
               n_elements(d), n_elements(b)]
      expand_mat, f, p, g, d, b, dvals
      IF (dvals(0) EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
      IF (newkey) THEN   $
         mats = mkmats(f, p, g, d, b, dir, /NEW)  $
      ELSE    $
         mats = mkmats(f, p, g, d, b, dir)
      IF (mats(0) EQ !ECAT.ERROR) THEN BEGIN
         set_ecat_error, 0, "WECAT6", UNIT=unit
         RETURN, !ECAT.ERROR
      ENDIF
      mats = mats(0, *)
      IF (n_elements(mats) NE n_elements(sh)) THEN BEGIN
         set_ecat_error, 9, "WECAT6", UNIT=unit
         RETURN, !ECAT.ERROR
      ENDIF

      ndatablks = 0
      scales = 0
      IF (NOT shonlykey) THEN BEGIN
         s = size(data)
         IF (s(0) EQ 1) THEN BEGIN
                                ; assume polar map data 
            ftype = 5
            stat = wsubheader6(unit, ftype, 1024, sh(0))
            IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
            IF (NOT keyword_set(SH_ONLY)) THEN BEGIN
               stat=wpmdata6(unit, sh(0), data)
            ENDIF 
            goto, done
         ENDIF
         
         IF ((s(0) LT 2) OR (s(0) GT 3)) THEN BEGIN
            set_ecat_error, 10, "WECAT6", UNIT=unit
            RETURN, !ECAT.ERROR
         ENDIF
         IF (s(0) EQ 2) THEN BEGIN
            IF (n_elements(mats) NE 1) THEN BEGIN    
               set_ecat_error, 11, "WECAT6", UNIT=unit
               RETURN, !ECAT.ERROR
            ENDIF
         ENDIF ELSE BEGIN
            IF (n_elements(mats) NE s(3)) THEN BEGIN
               set_ecat_error, 11, "WECAT6", UNIT=unit
               RETURN, !ECAT.ERROR
            ENDIF
         ENDELSE
         IF (newkey) THEN BEGIN
            ftype = mh.file.file_type
         ENDIF ELSE BEGIN
            m = matval(dir(0, 0))
            IF (m(0) EQ !ECAT.ERROR) THEN BEGIN
               set_ecat_error, 12, "WECAT6", UNIT=unit
               RETURN, !ECAT.ERROR
            ENDIF
            stat = recat6(file, m(0), m(1), m(2), m(3), m(4), d, /NO_DATA)
            IF (stat EQ !ECAT.ERROR) THEN BEGIN
               set_ecat_error, 0, "WECAT6", UNIT=unit
               RETURN, !ECAT.ERROR
            ENDIF
            IF ((s(1)*s(2) NE d.xdim*d.ydim) OR $
                (sh(0).data_type NE d.mh.file.data_type)) THEN BEGIN
               set_ecat_error, 13, "WECAT6", UNIT=unit
               RETURN, !ECAT.ERROR
            ENDIF
            ftype = d.mh.file.file_type
         ENDELSE
         stat = wmats6(unit, ftype, sh, data, mats(0, *), dir, dirinfo, SH_ONLY=shonlykey)
         IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR

      ENDIF
   ENDIF

DONE:
   free_lun, unit
   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "WECAT6", UNIT=unit
   RETURN, !ECAT.ERROR
   
END
