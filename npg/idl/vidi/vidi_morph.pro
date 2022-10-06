PRO VIDI_MORPH, s, img, type
   ;; s = SEGMENT_STATE structure
   ;; img  = image to erode/dilate
   ;; type:  1 = unconditional erosion
   ;;        2 = conditional erosion
   ;;        3 = unconditional dilation
   ;;        4 = conditional dilation
   ;;        5 = unconditional dilation
   ;;        6 = conditional dilation

   COMMON VIDI
   COMMON IMAGE_UTILS
   
   IF ((type EQ 2 OR type EQ 4 OR type EQ 6) AND (size(maskimg))[1] LE 1) THEN BEGIN
      str = 'No conditional image has been set'
   ENDIF ELSE IF (img.data EQ ptr_new()) THEN BEGIN
      str = 'No image data for erosion/dilation'
   ENDIF ELSE BEGIN
      widget_control, /hourglass
      frame = img.f - img.ff
      image = reform((*img.data)[*,*,*,frame])
      shape = intarr(3,3,3)
      shape = [[[0,0,0],[0,1,0],[0,0,0]],$
      			[[0,1,0],[1,1,1],[0,1,0]],$
      			[[0,0,0],[0,1,0],[0,0,0]]]
      indx = where (image GT 0)
      bimg = byte(image)
      bimg[indx] = 1
      IF (type LE 2) THEN BEGIN
         mask = erode(bimg, shape)
         IF (type EQ 1) THEN indx = where (mask EQ 0, count) $
         ELSE indx = where (mask EQ 0 AND maskimg EQ 0, count)
         IF (count GT 0) THEN image[indx]=0
         str = 'Pixels removed = '+string(count)
      ENDIF ELSE BEGIN
         mask = dilate(bimg, shape)
         indx = where (mask GT 0 AND image EQ 0, count)
         IF (count GT 0) THEN BEGIN
            IF (type EQ 3) THEN BEGIN
               val = mean (image[where (image GT 0)])
               image[indx]=val
               str = 'Pixels added = '+string(count)
            ENDIF ELSE BEGIN
               image[indx]=maskimg[indx]
               indx = where (maskimg[indx] GT 0, count)
               str = 'Pixels restored = '+string(count)
            ENDELSE
         ENDIF
      ENDELSE
      (*img.data)[*,*,*,frame] = image
   ENDELSE
   widget_control, s.msg, set_value=str
END
