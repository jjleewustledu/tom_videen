;       $Revision:   1.1  $ $Date:   10/15/93 18:41:32  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:		
; 	WECAT7
;
; PURPOSE:	
; 	Writes the main header, subheaders, and data into a file.		
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:		
; 	result = wecat7 (file,f,p,g,d,b,sh,data,MH=mh,NEW=new, $
; 				ONLY_MH=only_mh,ONLY_SH=only_sh)
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION wecat7, file, f, p, g, d, b, sh, data, MH=mh, NEW=new, ONLY_MH=only_mh, ONLY_SH=only_sh

   on_error, 2
   on_ioerror, io_error

   newkey = keyword_set(NEW)
   mhkey = keyword_set(MH)      ; check FOR write of main header only
   onlymhkey = keyword_set(ONLY_MH)
   
   IF (n_params() NE 8) THEN BEGIN
      IF (onlymhkey) THEN BEGIN
                                ; must specify filename
         IF (n_params() NE 1) THEN BEGIN
            set_ecat_error, 1, "WECAT7", FILE=file
            RETURN, !ECAT.ERROR
         ENDIF ELSE BEGIN
            IF (mhkey) THEN BEGIN
               ;; write the main header only. If NEW was specified
               ;; create a new file, write the main header AND 
               ;; initialize the directory block. 
               IF (newkey) THEN BEGIN
                  openw, unit, file, /get_lun
                  IF (wmh7(unit, mh) EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
                  IF (mknewdirblk7(unit, dir, dirinfo, /NEW) EQ !ECAT.ERROR) THEN $
                     RETURN, !ECAT.ERROR
                  free_lun, unit
                  RETURN, !ECAT.OK
               ENDIF ELSE BEGIN ; open the existing file AND write the main header
                  openu, unit, file, /get_lun
                  IF (wmh7(unit, mh) EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
                  free_lun, unit
                  RETURN, !ECAT.OK
               ENDELSE
            ENDIF ELSE BEGIN
               set_ecat_error, 1, "WECAT7", FILE=file
               RETURN, !ECAT.ERROR
            ENDELSE
         ENDELSE
      ENDIF ELSE BEGIN
         set_ecat_error, 1, "WECAT7", FILE=file
         RETURN, !ECAT.ERROR
      ENDELSE
   ENDIF

   shonlykey = keyword_set(SH_ONLY)
; IF ((NOT mhkey) AND newkey) THEN BEGIN
;    set_ecat_error,1,"WECAT",FILE=file
;    RETURN,!ECAT.ERROR
; ENDIF
   IF (newkey AND shonlykey) THEN BEGIN
      set_ecat_error, 1, "WECAT7", FILE=file
      RETURN, !ECAT.ERROR
   ENDIF

   IF (newkey) THEN BEGIN
      openw, unit, file, /get_lun
      IF (wmh7(unit, mh) EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
      IF (mknewdirblk7(unit, dir, dirinfo, /NEW) EQ !ECAT.ERROR) THEN $
         RETURN, !ECAT.ERROR
   ENDIF ELSE BEGIN
      openu, unit, file, /get_lun
      IF (mhkey EQ 0) THEN stat = rmh7(unit, mh)

      IF (rdir7(unit, dir, dirinfo) EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
      IF (mhkey) THEN    $
         IF (wmh7(unit, mh) EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
   ENDELSE
   
   IF (NOT keyword_set(ONLY_MH)) THEN BEGIN
      dvals = [n_elements(f), n_elements(p), n_elements(g), $
               n_elements(d), n_elements(b)]
      expand_mat, f, p, g, d, b, dvals
      IF (dvals(0) EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
      IF (newkey) THEN   $
         mats = mkmats(f, p, g, d, b, dir, /NEW)  $
      ELSE    $
         mats = mkmats(f, p, g, d, b, dir, /NEW)
      IF (mats(0) EQ !ECAT.ERROR) THEN BEGIN
         set_ecat_error, 0, "WECAT", UNIT=unit
         RETURN, !ECAT.ERROR
      ENDIF
      mats = mats(0, *)
      IF (n_elements(mats) NE n_elements(sh)) THEN BEGIN
         set_ecat_error, 9, "WECAT7", UNIT=unit
         RETURN, !ECAT.ERROR
      ENDIF

      ndatablks = 0
      scales = 0
      IF (NOT shonlykey) THEN BEGIN
         s = size(data)
         IF ((s(0) LT 2) OR (s(0) GT 3)) THEN BEGIN
            set_ecat_error, 10, "WECAT7", UNIT=unit
            RETURN, !ECAT.ERROR
         ENDIF
         IF (s(0) EQ 2) THEN BEGIN
            IF (n_elements(mats) NE 1) THEN BEGIN    
               set_ecat_error, 11, "WECAT7", UNIT=unit
               RETURN, !ECAT.ERROR
            ENDIF
         ENDIF ELSE BEGIN
            IF (n_elements(mats) NE s(3)) THEN BEGIN
               set_ecat_error, 11, "WECAT7", UNIT=unit
               RETURN, !ECAT.ERROR
            ENDIF
         ENDELSE
         IF (newkey) THEN BEGIN
            ftype = mh.file_type
         ENDIF ELSE BEGIN
            m = matval(dir(0, 0))
            IF (m(0) EQ !ECAT.ERROR) THEN BEGIN
               set_ecat_error, 12, "WECAT7", UNIT=unit
               RETURN, !ECAT.ERROR
            ENDIF
                                ;stat = recat7(file, m(0), m(1), m(2), m(3), m(4), d, /NO_DATA)
            stat = recat7(file, m(0), m(1), m(2), m(3), m(4), d, /MAIN)
            IF (stat EQ !ECAT.ERROR) THEN BEGIN
               set_ecat_error, 0, "WECAT7", UNIT=unit
               RETURN, !ECAT.ERROR
            ENDIF
            ;; this info is NOT available when adding matrices one at a time
            ;; IF (s(1)*s(2) NE d.xdim*d.ydim) THEN BEGIN
            ;;  set_ecat_error, 13, "WECAT7", UNIT=unit
            ;;  RETURN, !ECAT.ERROR
            ;; ENDIF
            ftype = d.mh.file_type
         ENDELSE
         stat = wmats7(unit, ftype, sh, data, mats(0, *), dir, dirinfo, SH_ONLY=shonlykey)
         IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR

      ENDIF
   ENDIF

   free_lun, unit
   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "WECAT7", UNIT=unit
   RETURN, !ECAT.ERROR
   
END

