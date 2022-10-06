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
      xsize = fact * uv.img[0].dim[x0] * dsp[0].zoom
   ENDIF ELSE stype = 0

   ;; Format 3 Images
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

   display_type = dsp[0].set
   smoothed = dsp[2].set

   ;; Max/Min
   CASE display_type OF
      3: BEGIN                  ; Image 2 over Image 1
         dsp[2].max = dsp[1].max
         dsp[2].min = 0
      ENDCASE
      ELSE: BEGIN	
         dsp[2].max = dsp[0].max * percent / 100
         dsp[2].min = -dsp[2].max
      ENDCASE
   ENDCASE

   img0 = VIDIF_DISPLAY_SCALE (img0, 0, xsize, ysize, sample, stype)
   img1 = VIDIF_DISPLAY_SCALE (img1, 1, xsize, ysize, sample, stype)
   img2 = VIDIF_DISPLAY_SCALE (img2, 2, xsize, ysize, sample, stype)

   ;; Color Bars
   len = ysize*9/10
   width = xsize/20
   offset = ysize*1/20
   cbar0 = bytscl(indgen(len), TOP=(dsp[0].nc-1)) ; Color Bar 0
   cbar2 = bytscl(indgen(len), TOP=(dsp[2].nc-1)) ; Color Bar 2

   FOR j=1, width DO FOR k=1, len DO img0[xsize-j, ysize-k-offset] = cbar0[k-1]
   widget_control,uv.wid.max[0],set_value=dsp[0].amax
   widget_control, uv.wid.win[0], draw_xsize=xsize, draw_ysize=ysize
   wset, dsp[0].id
   tv, img0

   FOR j=1, width DO FOR k=1, len DO img1[xsize-j, ysize-k-offset] = cbar0[k-1]
   widget_control,uv.wid.max[1],set_value=dsp[1].amax
   widget_control, uv.wid.win[1], draw_xsize=xsize, draw_ysize=ysize
   wset, dsp[1].id
   tv, img1
   
   ;; Image specification
   CASE display_type OF
      0: BEGIN                  ; Difference Image Alone
         tmp2 = img2
      ENDCASE
      1: BEGIN                  ; Difference Image over Image 1
         tmp1 = img0
         tmp2 = img2
      ENDCASE
      2: BEGIN                  ; Difference Image over Image 2
         tmp1 = img1
         tmp2 = img2
      ENDCASE
      3: BEGIN                  ; Image 2 over Image 1
         tmp1 = img0
         tmp2 = img1
      ENDCASE
   ENDCASE

   IF (smoothed EQ 1) THEN tmp2 = smooth(tmp2, 3)
   img2 = VIDIF_DISPLAY_SCALE (tmp2, 2, xsize, ysize, sample, stype)

   ;; Combine with Color Bar, Threshold, Cutoff
   CASE display_type OF
      0: BEGIN                  ; Difference Image Alone
         FOR j=1, width DO FOR k=1, len DO tmp2[xsize-j, ysize-k-offset] = cbar2[k-1]
         tmp1 = dsp[2].bot
      ENDCASE
      1: BEGIN                  ; Difference Image over Image 1
         FOR j=1, width DO FOR k=1, len DO BEGIN
            tmp1[xsize-j, ysize-k-offset] = 0
            tmp2[xsize-j, ysize-k-offset] = cbar2[k-1]
         ENDFOR
         uthresh = (0.5 + (0.005 * dsp[0].cut)) * dsp[0].nc
         lthresh = (0.5 - (0.005 * dsp[0].cut)) * dsp[0].nc
         idx1 = where((tmp2 LT uthresh) AND (tmp2 GT lthresh), count1)
         idx2 = where((tmp2 GE uthresh) OR (tmp2 LE lthresh), count2)
         IF (count1 GT 0) THEN tmp2[idx1] = 0
         IF (count2 GT 0) THEN tmp1[idx2] = dsp[2].bot
      ENDCASE
      2: BEGIN                  ; Difference Image over Image 2
         FOR j=1, width DO FOR k=1, len DO BEGIN
            tmp1[xsize-j, ysize-k-offset] = 0
            tmp2[xsize-j, ysize-k-offset] = cbar2[k-1]
         ENDFOR
         uthresh = (0.5 + (0.005 * dsp[0].cut)) * dsp[0].nc
         lthresh = (0.5 - (0.005 * dsp[0].cut)) * dsp[0].nc
         idx1 = where((tmp2 LT uthresh) AND (tmp2 GT lthresh), count1)
         idx2 = where((tmp2 GE uthresh) OR (tmp2 LE lthresh), count2)
         IF (count1 GT 0) THEN tmp2[idx1] = 0
         IF (count2 GT 0) THEN tmp1[idx2] = dsp[2].bot
      ENDCASE
      3: BEGIN                  ; Image 2 over Image 1
         FOR j=1, width DO FOR k=1, len DO BEGIN
            tmp1[xsize-j, ysize-k-offset] = 0
            tmp2[xsize-j, ysize-k-offset] = cbar0[k-1]
         ENDFOR
         cutoff =  0.01 * dsp[0].cut * dsp[1].nc
         idx1 = where(tmp2 LT cutoff, count1)
         idx2 = where(tmp2 GE cutoff, count2)
         IF (count1 GT 0) THEN tmp2[idx1] = 0
         IF (count2 GT 0) THEN tmp1[idx2] = dsp[2].bot
      ENDCASE
   ENDCASE

   tmp3 = tmp1 + tmp2
   widget_control,uv.wid.max[2],set_value=dsp[2].amax
   widget_control, uv.wid.win[2], draw_xsize=xsize, draw_ysize=ysize
   wset, dsp[2].id
   tv, tmp3
END
