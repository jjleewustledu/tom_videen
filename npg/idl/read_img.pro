; $Id: $
;
; Copyright (c) 1997, Washington University.  All rights reserved.
;	Unauthorized reproduction prohibited.
;+
; NAME:
;	READ_IMG	
;
; PURPOSE:
;	Open, read and display an image.	
;
; CATEGORY:
;	Capp image processing.
;
; CALLING SEQUENCE:
;	ROUTINE_NAME, Parameter1, Parameter2, Foobar
;	Status = READ_IMG (Fname)
;
; INPUTS:
;	Fname: filename
;
; OUTPUTS:
;
; COMMON BLOCKS:
;	None.
;
; EXAMPLE:
;	READ_IMG (fname)
;
; MODIFICATION HISTORY:
; 	Written by:	Tom Videen, Feb 1997.
;-

PRO read_img, fname

   IF (fname NE '') THEN BEGIN
      stat = df_open(fname, dfptr)
      IF (stat EQ 0) THEN BEGIN
         matspec = get_mat_selector(dfptr=dfptr)
         IF (matspec NE '') THEN BEGIN
            stat = df_read(dfptr, matspec, data)
            IF (stat EQ 0) THEN BEGIN
               window, 0
               handle_value, data.data_ptr, dp, /no_copy
               sz = size(dp)
               IF (sz(0) EQ 3) THEN BEGIN
                  FOR i=0, sz(3)-1 DO BEGIN
                     ctvscl, dp(*, *, i), i mod 20
                  ENDFOR
               ENDIF
               handle_value, data.data_ptr, dp, /no_copy, /set
               stat = df_free_data(data)
            ENDIF
         ENDIF
         stat = df_close(dfptr)
         stat = df_free_data(data)
      ENDIF
   ENDIF
END
