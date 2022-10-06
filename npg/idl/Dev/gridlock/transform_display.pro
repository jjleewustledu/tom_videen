FUNCTION TRANSFORM_DISPLAY_SCALE, image, type, i, xsize, ysize, sample, stype
   COMMON GRIDLOCK
   
   IF (dsp[i].type EQ 1) THEN BEGIN ; Manual scaling
      mini = dsp[i].min
      maxi = dsp[i].max
   ENDIF ELSE BEGIN             ; Auto scaling
      IF (dsp[i].range EQ 0) THEN BEGIN
         mini = 0
         maxi = max(image)
      ENDIF ELSE IF (dsp[i].range EQ 1) THEN BEGIN
         mini = 0
         maxi = -min(image)
      ENDIF ELSE BEGIN
         mini = min(image)
         maxi = max(image)
         IF (-mini GT maxi) THEN maxi = -mini ELSE mini = -maxi
      ENDELSE
   ENDELSE
   dsp[i].amin = mini
   dsp[i].amax = maxi
   nc = dsp[i].nc
   IF (stype EQ 0) THEN BEGIN
      IF (dsp[i].range EQ 1) THEN $
         temp1 = bytscl(rebin(-(image), xsize, ysize, SAMPLE=sample), $
                        MIN=mini, MAX=maxi, TOP=(nc-1)) $
      ELSE temp1 = bytscl(rebin(image, xsize, ysize, SAMPLE=sample), $
                          MIN=mini, MAX=maxi, TOP=(nc-1))
   ENDIF ELSE BEGIN 
      IF (dsp[i].range EQ 1) THEN $
         temp1 = bytscl(congrid(-(image), xsize, ysize, /INTERP), $
                        MIN=mini, MAX=maxi, TOP=(nc-1)) $
      ELSE temp1 = bytscl(congrid(image, xsize, ysize, /INTERP), $
                          MIN=mini, MAX=maxi, TOP=(nc-1))
   ENDELSE
   RETURN, temp1
END
      
PRO TRANSFORM_DISPLAY, uv, SAMPLE=sample
   ;; Scale and display image i
   ;; uv = the info structure for this display
   ;; sample = use nearest neighbor sampling for rebin
	;; img[0] = reference
	;; img[1] = study
	;; img[2] = reference transformed to study

   COMMON IMAGE_UTILS
   COMMON GRIDLOCK
   
   IF (NOT (keyword_set(sample))) THEN SAMPLE=0
   FOR i= 0, 3 DO BEGIN
      IF (uv.img[i].data NE ptr_new()) THEN BEGIN
         x0 = 0
         y0 = 1
         stype = 1
         slice = uv.img[i].p - uv.img[i].fp
         image = reform((*uv.img[i].data)[*,*,slice])
         xsize = uv.img[i].dim[x0] * dsp[i].zoom
         ysize = uv.img[i].dim[y0] * dsp[i].zoom
         IF (uv.img[i].pdim[y0] GT uv.img[i].pdim[x0]) THEN BEGIN 
            fact = (uv.img[i].pdim[y0])/(uv.img[i].pdim[x0])
            ysize = fact * uv.img[i].dim[y0] * dsp[i].zoom
         ENDIF ELSE IF (uv.img[i].pdim[0] GT uv.img[i].pdim[y0]) THEN BEGIN 
            fact = (uv.img[i].pdim[x0])/(uv.img[i].pdim[y0])
            xsize = fact * uv.img[i].dim[x0] * dsp[i].zoom
         ENDIF ELSE stype = 0

         temp1 = TRANSFORM_DISPLAY_SCALE (image, uv.img[i].itype, i, xsize, ysize, sample, stype)
         temp1 = temp1 + dsp[i].bot
         widget_control, uv.wid.win[i], draw_xsize=xsize, draw_ysize=ysize
         wset, dsp[i].id
         tv, temp1
      ENDIF 
   ENDFOR
   
END
