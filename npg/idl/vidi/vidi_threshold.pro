PRO VIDI_THRESHOLD, s, image, bot, top
   ;; image = image
   ;; bot, top = lower and upper thresholds

   COMMON VIDI
   COMMON IMAGE_UTILS
   
   widget_control, /hourglass
   IF (mask_type EQ 0) THEN BEGIN
      str = 'Masking is not set to Inside or Outside; No Pixels Removed'
   ENDIF ELSE BEGIN
      CASE mask_type OF
         1: indx = where (image GE bot AND image LE top, count)
         2: indx = where (image LE bot OR image GE top, count)
      ENDCASE
      IF (count GT 0) THEN image[indx] = 0
      str = 'Pixels removed = '+string(count)
   ENDELSE
   widget_control, s.msg, set_value=str
END
