PRO VIDI_MORPH2D, s, img, type, ndil
   ;; s = SEGMENT_STATE structure
   ;; img  = image to erode/dilate
   ;; type:  1 = unconditional erosion
   ;;        2 = conditional erosion
   ;;        3 = unconditional dilation
   ;;        4 = conditional dilation
   ;; ndil = number of erosions/dilations

   COMMON VIDI
   COMMON IMAGE_UTILS
   
   IF ((type EQ 2 OR type EQ 4) AND (size(maskimg))[1] LE 1) THEN BEGIN
      str = 'No conditional image has been set'
   ENDIF ELSE IF (img.data EQ ptr_new()) THEN BEGIN
      str = 'No image data for erosion/dilation'
   ENDIF ELSE BEGIN
      widget_control, /hourglass
      frame = img.f - img.ff
      shape = intarr(3,3)
      sum = 0
      FOR pln = img.fp-1, img.lp-1 DO BEGIN
         image = reform((*img.data)[*,*,pln,frame])
         IF (type EQ 2 OR type EQ 4) THEN mskimg = reform(maskimg[*,*,pln])
         indx = where (image GT 0, cnt3)
         IF (indx[0] GT 0) THEN BEGIN
            FOR i = 1, ndil DO BEGIN
               IF (i MOD 2 GT 0) THEN BEGIN
                  shape = [[0,1,0],[1,1,1],[0,1,0]]
               ENDIF ELSE BEGIN
                  shape = [[1,1,1],[1,1,1],[1,1,1]]
               ENDELSE
               bimg = byte(image)
               bimg[indx] = 1
               CASE type OF
                  1: BEGIN
                     emask = erode(bimg, shape)
                     idx = where (emask EQ 0, count) 
                     IF (count GT 0) THEN BEGIN
                        image[idx] = 0
                        sum = sum + count
                     ENDIF
                  ENDCASE
                  2: BEGIN
                     emask = erode(bimg, shape)
                     idx = where (emask EQ 0 AND mskimg EQ 0, count)
                     IF (count GT 0) THEN BEGIN
                        image[idx] = 0
                        sum = sum + count
                     ENDIF
                  ENDCASE
                  3: BEGIN
                     dmask = dilate(bimg, shape)
                     idx = where (dmask GT 0 AND image EQ 0, count)
                     IF (count GT 0) THEN BEGIN
                        val = mean (image[where (image GT 0)])
                        image[idx] = val
                        sum = sum + count
                     ENDIF
                  ENDCASE
                  4: BEGIN
                     dmask = dilate(bimg, shape)
                     idx = where (dmask GT 0 AND image EQ 0 AND mskimg GT 0, count)
                     IF (count GT 0) THEN BEGIN
						idx1 = where (image GT 0,cnt4)
						IF (cnt4 GT 1) THEN BEGIN
                        	val = mean (image[idx1])
   						ENDIF ELSE BEGIN
							val = image[idx1]
						ENDELSE
                        image[idx] = val
                        sum = sum + count
                     ENDIF
                  ENDCASE
               ENDCASE
               indx = where (image GT 0, cnt3)
            ENDFOR
         ENDIF
         (*img.data)[*,*,pln,frame] = image
      ENDFOR
   ENDELSE
   CASE type OF
      1: str = 'Pixels removed = ' +string(sum)
      2: str = 'Pixels removed = ' +string(sum)
      3: str = 'Pixels added = '   +string(sum)
      4: str = 'Pixels restored = '+string(sum)
   ENDCASE
   widget_control, s.msg, set_value=str
END
