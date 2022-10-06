PRO VIDI_OUTLINE, s, i, uv, bot, top, indx, remove
   ;; s   = SEGMENT_STATE structure (from vidi_segment.pro)
   ;; i   = index to image
   ;; uv
   ;; bot, top = lower and upper thresholds
   ;; indx  = indices to masked points
   ;; remove  = zero pixels in image

   COMMON VIDI
   COMMON IMAGE_UTILS
   
   editval = uv.wid.editval
   img = uv.img[i]
   IF (img.data NE ptr_new()) THEN BEGIN
      slice = img.p - img.fp
      frame = img.f - img.ff
      CASE orient OF
         0: BEGIN
            x0 = 0
            y0 = 1
            z0 = 2
            image = reform((*img.data)[*,*,slice,frame])
         END
         1: BEGIN
            x0 = 0
            y0 = 2
            z0 = 1
            image = reform((*img.data)[*,slice,*,frame])
         END
         2: BEGIN
            x0 = 1
            y0 = 2
            z0 = 0
            image = reform((*img.data)[slice,*,*,frame])
         END
      ENDCASE
      xsize = img.dim[x0] * dsp[i].zoom
      ysize = img.dim[y0] * dsp[i].zoom
      stype = 1
      IF (img.pdim[y0] GT img.pdim[x0]) THEN BEGIN 
         fact = (img.pdim[y0])/(img.pdim[x0])
         ysize = fact * img.dim[y0] * dsp[i].zoom
      ENDIF ELSE IF (img.pdim[0] GT img.pdim[y0]) THEN BEGIN 
         fact = (img.pdim[x0])/(img.pdim[y0])
         xsize = fact * img.dim[x0] * dsp[i].zoom
      ENDIF ELSE stype = 0
      voxvol = img.pdim[0]*img.pdim[1]*img.pdim[2]
      npix_img = img.dim[0]*img.dim[1]*img.dim[2]

      widget_control, /hourglass
      count = 0
      CASE connect_type OF
         0: BEGIN
            CASE mask_type OF
               1: indx = where (image GE bot AND image LE top, count)
               2: indx = where (image LE bot OR image GE top, count)
               ELSE: count=0
            ENDCASE
            str = 'Pixels Identified for Removal (this plane) = '+string(count)
            ;;widget_control, s.msg, set_value=str
            IF (count GT 0) THEN BEGIN
               image(indx) = editval
               IF (remove) THEN BEGIN
                  CASE orient OF
                     0: (*img.data)[*,*,slice,frame] = image
                     1: (*img.data)[*,slice,*,frame] = image
                     2: (*img.data)[slice,*,*,frame] = image
                  ENDCASE
               ENDIF
            ENDIF
         ENDCASE
         1: BEGIN
            IF (mask_type EQ 0) THEN BEGIN
               count = 0
            ENDIF ELSE BEGIN
               testval = image[seed[x0,0], seed[y0,0]]
               IF ((testval LT bot) OR (testval GT top)) THEN BEGIN
                  count = 0
                  str = 'Seed pixel value is not within thresholds: '+string(testval)
                  widget_control, s.msg, set_value=str
               ENDIF ELSE BEGIN
                  indx = SEARCH2D (image, seed[x0,0], seed[y0,0], bot, top)
                  IF (nseeds GT 0) THEN FOR sn = 1, nseeds DO BEGIN
                     testval = image[seed[x0,sn], seed[y0,sn]]
                     IF ((testval GE bot) AND (testval LE top)) THEN BEGIN
                        indx1 = SEARCH2D (image, seed[x0,sn], seed[y0,sn], bot, top)
                        indx = [indx, indx1]
                     ENDIF
                  ENDFOR
                  count = n_elements (indx)
                  IF (mask_type EQ 2 AND count GT 0) THEN BEGIN
                     val = max(image) + 1
                     nimg = image 
                     nimg[indx] = val
                     indx = where(nimg NE val)
                  ENDIF
                  count = n_elements (indx)
                  IF (count EQ 1) THEN IF (indx[0] LT 0) THEN count = 0
                  str = 'Pixels filled on this plane = '+string(count)
                  widget_control, s.msg, set_value=str
               ENDELSE
            ENDELSE
            IF (count GT 0) THEN BEGIN
               image(indx) = editval
               IF (remove) THEN BEGIN
                  CASE orient OF
                     0: (*img.data)[*,*,slice,frame] = image
                     1: (*img.data)[*,slice,*,frame] = image
                     2: (*img.data)[slice,*,*,frame] = image
                  ENDCASE
               ENDIF
            ENDIF
         ENDCASE
         2: BEGIN
            image = (*img.data)[*,*,*,frame]
            IF (remove) THEN BEGIN
               count = n_elements (indx)
               str = 'Removing pixels from entire volume... '+string(count)
               widget_control, s.msg, set_value=str
               IF (count GT 0) THEN BEGIN
                  image(indx) = editval
                  (*img.data)[*,*,*,frame] = image
               ENDIF
            ENDIF ELSE BEGIN
               IF (mask_type EQ 0) THEN BEGIN
                  count = 0
               ENDIF ELSE BEGIN
                  str = '3D-connected pixel search... WARNING: Moving cursor into Image Windows before Remove cancels 3D'
                  widget_control, s.msg, set_value=str
                  testval = image[seed[0,0], seed[1,0], seed[2,0]]
                  IF (testval LT bot OR testval GT top) THEN BEGIN
                     count = 0
                  ENDIF ELSE BEGIN
                     indx = SEARCH3D (image, seed[0,0], seed[1,0], seed[2,0], bot, top)
                     IF (nseeds GT 0) THEN FOR sn = 1, nseeds DO BEGIN
                        testval = image[seed[x0,sn], seed[y0,sn]]
                        IF ((testval GE bot) AND (testval LE top)) THEN BEGIN
                           indx1 = SEARCH3D (image, seed[0,sn], seed[1,sn], seed[2,sn],bot, top)
                           indx = [indx, indx1]
                        ENDIF
                     ENDFOR
                     count = n_elements (indx)
                     IF (mask_type EQ 2 AND count GT 0) THEN BEGIN
                        val = max(image)+1
                        nimg = image 
                        nimg[indx] = val
                        indx = where(nimg NE val)
                     ENDIF
                     count = n_elements (indx)
                     IF (count EQ 1) THEN IF (indx[0] LT 0) THEN count = 0
                  ENDELSE
               ENDELSE
               npix_diff = count - npix_removed
               vspared = 0.001 * voxvol*(npix_img - count)
               str = 'Pixels Identified for Removal = '+strcompress(string(count)) $
                  + '     Last ='+strcompress(string(npix_removed)) $
                  + '  Diff ='+strcompress(string(npix_diff)) $
                  + '     Volume Spared ='+strcompress(string(vspared))+ 'cc'
               npix_removed = count
               widget_control, s.msg, set_value=str
               IF (count GT 0) THEN image(indx) = editval
            ENDELSE
            CASE orient OF
               0: image = reform(image[*,*,slice])
               1: image = reform(image[*,slice,*])
               2: image = reform(image[slice,*,*])
            ENDCASE
         ENDCASE
      ENDCASE
   ENDIF 
   temp2 = VIDI_DISPLAY_SCALE (image, img.itype, i, xsize, ysize, 0, stype)
   temp2 = temp2 + dsp[i].bot
   wset, dsp[i].id
   tv, temp2

   IF ((i EQ 1) AND dsp[2].set) THEN BEGIN
      img0 = uv.img[0]
      CASE orient OF
         0:  image0 = reform((*img0.data)[*,*,slice,frame])
         1:  image0 = reform((*img0.data)[*,slice,*,frame])
         2:  image0 = reform((*img0.data)[slice,*,*,frame])
      ENDCASE
      temp1 = VIDI_DISPLAY_SCALE (image0, img0.itype, 0, xsize, ysize, sample, stype)

      IF (segshow_type EQ 0) THEN BEGIN
         temp2 = VIDI_DISPLAY_SCALE (image, img.itype, i, xsize, ysize, 0, stype)
      ENDIF ELSE BEGIN
         CASE orient OF
            0:  image1 = reform((*img.data)[*,*,slice,frame])
            1:  image1 = reform((*img.data)[*,slice,*,frame])
            2:  image1 = reform((*img.data)[slice,*,*,frame])
         ENDCASE
         indx2 = where (image1 EQ image, count1)
         IF (count1 GT 0) THEN image0(indx2) = 0
         temp2 = VIDI_DISPLAY_SCALE (image0, img.itype, i, xsize, ysize, 0, stype)
      ENDELSE
      
      IF (dsp[1].range EQ 2) THEN BEGIN
         uthresh =  (0.5 + (0.005 * dsp[0].cut)) * dsp[1].nc
         lthresh =  (0.5 - (0.005 * dsp[0].cut)) * dsp[1].nc
         idx1 = where((temp2 LT uthresh) AND (temp2 GT lthresh), count1)
         idx2 = where((temp2 GE uthresh) OR (temp2 LE lthresh), count2)
      ENDIF ELSE BEGIN
         thresh =  0.01 * dsp[0].cut * dsp[1].nc
         idx1 = where(temp2 LT thresh, count1)
         idx2 = where(temp2 GE thresh, count2)
      ENDELSE
      IF (count1 GT 0) THEN temp2[idx1] = 0 ; temp = temp1
      IF (count2 GT 0) THEN temp1[idx2] = dsp[0].nc ; temp = nc + temp2
      temp = temp1 + temp2      ; combine
      wset, dsp[2].id
      tv, temp
   ENDIF
   
END
