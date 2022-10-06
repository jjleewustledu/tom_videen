;+
; NAME:
;       SELECT_READ
;
; PURPOSE:
;       Procedure to:
; 	Select a dataset
; 	Open and read a user selected set of data
; 	Display the data in a window
; 	Close the file
; 	Free the data
;
; CALLING SEQUENCE:
;       SELECT_READ
;
; ARGUMENTS:
; 	None.
;
; KEYWORDS:
; 	None.
;-

PRO select_read

   stat = select_dataset(fname)
   IF (stat EQ 0) THEN BEGIN
      stat = df_open(fname, dfptr)
      IF (stat EQ 0) THEN BEGIN
         matspec = get_mat_selector(dfptr=dfptr)n
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
