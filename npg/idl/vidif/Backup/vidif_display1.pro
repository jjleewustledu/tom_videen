FUNCTION VIDIF_DISPLAY_SCALE, image, i, xsize, ysize, sample, stype
   COMMON VIDI
   
   mini = dsp[i].min            ; Manual scaling
   maxi = dsp[i].max
   dsp[i].amin = mini
   dsp[i].amax = maxi
   nc = dsp[i].nc
   IF (stype EQ 0) THEN BEGIN
      IF (dsp[i].range EQ 1) THEN $
         tmp = bytscl(rebin(-(image), xsize, ysize, SAMPLE=sample), $
                        MIN=mini, MAX=maxi, TOP=(nc-1)) $
      ELSE tmp = bytscl(rebin(image, xsize, ysize, SAMPLE=sample), $
                          MIN=mini, MAX=maxi, TOP=(nc-1))
   ENDIF ELSE BEGIN 
      IF (dsp[i].range EQ 1) THEN $
         tmp = bytscl(congrid(-(image), xsize, ysize, /INTERP), $
                        MIN=mini, MAX=maxi, TOP=(nc-1)) $
      ELSE tmp = bytscl(congrid(image, xsize, ysize, /INTERP), $
                          MIN=mini, MAX=maxi, TOP=(nc-1))
   ENDELSE
   RETURN, tmp
END
      
PRO VIDIF_DISPLAY, uv
   ;; Scale and display 3 images
   ;; uv = the info structure for this display
   COMMON IMAGE_UTILS
   COMMON VIDI
   
   IF ((uv.img[0].dim[0] NE uv.img[1].dim[0]) OR $
       (uv.img[0].dim[1] NE uv.img[1].dim[1]) OR $
       (uv.img[0].dim[2] NE uv.img[1].dim[2])) THEN BEGIN
      widget_control, uv.wid.msg, set_value='Cannot display composite of different sized images'
      RETURN
   ENDIF
   CASE orient OF
      0: BEGIN
         x0 = 0
         y0 = 1
      END
      1: BEGIN
         x0 = 0
         y0 = 2
      END
      2: BEGIN
         x0 = 1
         y0 = 2
      END
   ENDCASE
   slice = uv.img[0].p - uv.img[0].fp
   frame = uv.img[0].f - uv.img[0].ff
   xsize = uv.img[0].dim[x0] * dsp[0].zoom
   ysize = uv.img[0].dim[y0] * dsp[0].zoom
   stype = 1
   IF (uv.img[0].pdim[y0] GT uv.img[0].pdim[x0]) THEN BEGIN 
      fact = (uv.img[0].pdim[y0])/(uv.img[0].pdim[x0])
      ysize = fact * uv.img[0].dim[y0] * dsp[0].zoom
   ENDIF ELSE IF (uv.img[0].pdim[0] GT uv.img[0].pdim[y0]) THEN BEGIN 
      fact = (uv.img[0].pdim[x0])/(uv.img[0].pdim[y0])
      xsize = fact * uv.img[0].dim[x0] * dsp[i].zoom
   ENDIF ELSE stype = 0

   ; Format 3 Images
   CASE orient OF
      0: BEGIN
         img0 = reform((*uv.img[0].data)[*,*,slice,frame])
         img1 = reform((*uv.img[1].data)[*,*,slice,frame])
         img2 = reform((*uv.img[2].data)[*,*,slice,frame])
      ENDCASE
      1: BEGIN
         img0 = reform((*uv.img[0].data)[*,slice,*,frame])
         img1 = reform((*uv.img[1].data)[*,slice,*,frame])
         img2 = reform((*uv.img[2].data)[*,slice,*,frame])
      ENDCASE
      2: BEGIN
         img0 = reform((*uv.img[0].data)[slice,*,*,frame])
         img1 = reform((*uv.img[1].data)[slice,*,*,frame])
         img2 = reform((*uv.img[2].data)[slice,*,*,frame])
      ENDCASE
   ENDCASE
   tmp0 = VIDIF_DISPLAY_SCALE (img0, 0, xsize, ysize, sample, stype)
   tmp1 = VIDIF_DISPLAY_SCALE (img1, 1, xsize, ysize, sample, stype)
   tmp2 = VIDIF_DISPLAY_SCALE (img2, 2, xsize, ysize, sample, stype)

   nc = dsp[0].nc         ; Color Bar
   wid = xsize/20
   len = ysize*9/10
   off = ysize*1/20
   cbar0 = bytscl(indgen(len), TOP=(nc-1))
   FOR j=1, wid DO FOR k=1, len DO temp[xsize-j, ysize-k-off] = cbar0[k-1]

   FOR i= 0, 1 DO BEGIN
         temp = temp + dsp[i].bot
         widget_control, uv.wid.win[i], draw_xsize=xsize, draw_ysize=ysize
         wset, dsp[i].id
         tv, temp
         widget_control,uv.wid.max[i],set_value=dsp[i].amax
      ENDELSE
   ENDFOR
   
   ;; dsp[0].set = 0 -> Display Difference Image Alone
   ;; dsp[0].set = 1 -> Display Difference Image over Image 1
   ;; dsp[0].set = 2 -> Display Difference Image over Image 2
   ;; dsp[0].set = 3 -> Display Image 2 over Image 1

   IF (dsp[0].set EQ 3) THEN BEGIN	; Image 2 on 1
      i = 1		; Image 2
      dsp[2].max = dsp[1].max
      dsp[2].min = 0
   ENDIF ELSE BEGIN	; Image 3 (difference, alone or on 1 or 2)
      i = 2		; Image 3 (difference)
      dsp[2].max = dsp[0].max * percent / 100
      dsp[2].min = -dsp[2].max
   ENDELSE

   IF (uv.img[i].data NE ptr_new() AND uv.img[i].data NE ptr_new()) THEN BEGIN
      CASE orient OF
         0: image = reform((*uv.img[i].data)[*,*,slice,frame])
         1: image = reform((*uv.img[i].data)[*,slice,*,frame])
         2: image = reform((*uv.img[i].data)[slice,*,*,frame])
      ENDCASE
      widget_control, uv.wid.win[2], draw_xsize=xsize, draw_ysize=ysize
      IF (dsp[2].set EQ 0 OR dsp[2].set EQ 3) THEN tmp2 = image $
      ELSE tmp2 = smooth(image, 3) 
      temp2 = VIDIF_DISPLAY_SCALE (tmp2, uv.img[i].itype, i, xsize, ysize, sample, stype)

      IF (dsp[0].set EQ 0) THEN BEGIN	; Difference Alone
         nc = dsp[i].nc         ; Color Bar
         wid = xsize/20
         len = ysize*9/10
         off = ysize*1/20
         cbar = bytscl(indgen(len), TOP=(nc-1))
         FOR j=1, wid DO FOR k=1, len DO temp2[xsize-j, ysize-k-off] = cbar[k-1]
         tmp2 = temp2 + dsp[2].bot
         widget_control,uv.wid.max[2],set_value=dsp[2].amax

      ENDIF ELSE IF (dsp[0].set EQ 3) THEN BEGIN	; Image 2 on 1 (h=0)
         CASE orient OF
            0:  image = reform((*uv.img[0].data)[*,*,slice,frame])
            1:  image = reform((*uv.img[0].data)[*,slice,*,frame])
            2:  image = reform((*uv.img[0].data)[slice,*,*,frame])
         ENDCASE
         temp1 = VIDIF_DISPLAY_SCALE (image, uv.img[0].itype, 0, xsize, ysize, sample, stype)

         CASE orient OF
            0:  image = reform((*uv.img[1].data)[*,*,slice,frame])
            1:  image = reform((*uv.img[1].data)[*,slice,*,frame])
            2:  image = reform((*uv.img[1].data)[slice,*,*,frame])
         ENDCASE
         temp2 = VIDIF_DISPLAY_SCALE (image, uv.img[1].itype, 1, xsize, ysize, sample, stype)

         nc = dsp[1].nc         ; Color Bar
         wid = xsize/20
         len = ysize*9/10
         off = ysize*1/20
         cbar = bytscl(indgen(len), TOP=(nc-1))
         FOR j=1, wid DO FOR k=1, len DO BEGIN
            temp1[xsize-j, ysize-k-off] = 0
            temp2[xsize-j, ysize-k-off] = cbar[k-1]
         ENDFOR

         cutoff =  0.01 * dsp[0].cut * dsp[1].nc
         idx1 = where(temp2 LT cutoff, count1)
         IF (count1 GT 0) THEN temp2[idx1] = 0
         idx2 = where(temp2 GE cutoff, count2)
         IF (count2 GT 0) THEN temp1[idx2] = dsp[2].bot
         temp = temp1 + temp2
         widget_control,uv.wid.max[2],set_value=dsp[2].max

      ENDIF ELSE BEGIN		; Difference on either Image 1 or 2 (h=0 or 1)
         h = dsp[0].set - 1
         CASE orient OF
            0:  image = reform((*uv.img[h].data)[*,*,slice,frame])
            1:  image = reform((*uv.img[h].data)[*,slice,*,frame])
            2:  image = reform((*uv.img[h].data)[slice,*,*,frame])
         ENDCASE
         temp1 = VIDIF_DISPLAY_SCALE (image, uv.img[h].itype, h, xsize, ysize, sample, stype)

         CASE orient OF
            0:  image = reform((*uv.img[2].data)[*,*,slice,frame])
            1:  image = reform((*uv.img[2].data)[*,slice,*,frame])
            2:  image = reform((*uv.img[2].data)[slice,*,*,frame])
         ENDCASE
         temp2 = VIDIF_DISPLAY_SCALE (image, uv.img[2].itype, 2, xsize, ysize, sample, stype)

         nc = dsp[i].nc         ; Color Bar
         wid = xsize/20
         len = ysize*9/10
         off = ysize*1/20
         cbar = bytscl(indgen(len), TOP=(nc-1))
         FOR j=1, wid DO FOR k=1, len DO BEGIN
            temp1[xsize-j, ysize-k-off] = 0
            temp2[xsize-j, ysize-k-off] = cbar[k-1]
         ENDFOR

         uthresh =  (0.5 + (0.005 * dsp[0].cut)) * dsp[0].nc
         lthresh =  (0.5 - (0.005 * dsp[0].cut)) * dsp[0].nc
         idx1 = where((temp1 LT uthresh) AND (temp1 GT lthresh), count1)
         IF (count1 GT 0) THEN temp1[idx1] = 0
         idx2 = where((temp1 GE uthresh) OR (temp1 LE lthresh), count2)
         IF (count2 GT 0) THEN temp2[idx2] = dsp[0].nc
         temp = temp1 + temp2
         widget_control,uv.wid.max[2],set_value=dsp[2].amax
      ENDELSE
      wset, dsp[2].id
      tv, temp
   ENDIF
END
