;+
; NAME:
;       SELECT_READ_WRITE
;
; PURPOSE:
;       Procedure to:
; 	Select a dataset
; 	Open and read a user selected set of data
; 	Filter the data
; 	Display the data in a window
; 	Write the filtered data to a new file
; 	Close the file
; 	Free the data
;
; CALLING SEQUENCE:
;       SELECT_READ_WRITE
;
; ARGUMENTS:
; None.
;
; KEYWORDS:
; None.
;-

; HISTORY NOTES:
;
;-

PRO select_read_write
   ;; select a dataset
   stat = select_dataset(fname)
   IF (stat EQ 0) THEN BEGIN

      ;; open the file selected
      stat = df_open(fname, dfptr)
      IF (stat EQ 0) THEN BEGIN

         ;; get the matrix specification
         matspec = get_mat_selector(dfptr=dfptr)
         IF (matspec NE '') THEN BEGIN

            ;; read the specified matrix
            stat = df_read(dfptr, matspec, data)
            IF (stat EQ 0) THEN BEGIN

               handle_value, data.data_ptr, dp, /no_copy

               ;; filter/display the data
               window, 0
               sz = size(dp)
               IF (sz(0) EQ 3) THEN BEGIN
                  FOR i=0, sz(3)-1 DO BEGIN
                     dp(0, 0, i) = sobel(dp(*, *, i))
                     ctvscl, dp(*, *, i), i mod 20
                  ENDFOR
               ENDIF
               handle_value, data.data_ptr, dp, /no_copy, /set

               ;; write the data to a file
               stat = df_open('/capp/image/demo/test1.img', dfptrOut, /new, dtype=7, parent=data)
               IF (stat EQ 0) THEN BEGIN
                  stat = df_write(dfptrOut, data)
                  stat = df_close(dfptrOut)
                  stat = df_free_data (dfptrOut)
               ENDIF

               ;; free the data
               stat = df_free_data(data)
            ENDIF
         ENDIF

         ;; close the file
         stat = df_close(dfptr)

         ;; free the data file pointer
         stat = df_free_data(data)
      ENDIF
   ENDIF
END
