FUNCTION FID_DISPLAY_SCALE, image, xsize, ysize, maxi, type
   COMMON FID
   
   top = dsp[0].nc-2
   mini = dsp[0].min
   CASE type of			; 0=image, 1=line
      0: img0 = bytscl(rebin(image, xsize, ysize), MIN=mini, MAX=maxi, TOP=top)
      1: img0 = bytscl(congrid(image, xsize, ysize), MIN=0, MAX=1, TOP=top)
   ENDCASE
   RETURN, img0
END
      
PRO FID_DISPLAY, uv
   ;; Scale and display image 
   ;; uv = the info structure for this display
   COMMON IMAGE_UTILS
   COMMON FID
   
   ;; Display Transverse (img) in Window 1, Coronal (obj) in Window 2

   x0 = 0
   zoom = dsp[0].zoom
   img = uv.img[0]
   obj = uv.img[1]
   max0 = dsp[0].max
   max1 = dsp[1].max
   IF (img.data NE ptr_new()) THEN BEGIN
      y0 = 1
      xsize = img.dim[x0] * zoom
      pp = img.p - img.fp
      image = reform((*img.data)[*,*,pp,0])
      ysize = img.dim[y0] * zoom
      dsp[0].xsize = xsize
      dsp[0].ysize = ysize
      img0 = FID_DISPLAY_SCALE (image, xsize, ysize, max0, 0)
      IF (obj.data NE ptr_new()) THEN BEGIN
         line = intarr(img.dim[x0],img.dim[y0])
         line[*,*] = 0
         len = img.dim[x0]
         IF (full EQ 0) THEN BEGIN
            line[0:len/5,obj.f-1] = 1
            line[4*len/5:len-1,obj.f-1] = 1
         ENDIF ELSE line[*,obj.f-1] = 1
         line0 = reform(line[*,*])
         ln0 = FID_DISPLAY_SCALE (line0, xsize, ysize, max0, 1)
         idx = where(ln0 GT 0, cnt)
         IF (cnt GT 0) THEN img0[idx] = dsp[0].nc-1
      ENDIF
      widget_control, uv.wid.win[0], draw_xsize=xsize, draw_ysize=ysize
      wset, dsp[0].id
      tv, img0
   ENDIF

   IF (obj.data EQ ptr_new()) THEN return
   y0 = 2
   pp = obj.f - obj.ff
   object = reform((*obj.data)[*,pp,*,0])
   xsize = obj.dim[x0] * zoom
   ysize = obj.dim[y0] * zoom
   dsp[1].xsize = xsize
   dsp[1].ysize = ysize
   obj0 = FID_DISPLAY_SCALE (object, xsize, ysize, max1, 0)
   IF (img.data NE ptr_new()) THEN BEGIN
      line = intarr(obj.dim[x0],obj.dim[y0])
      line[*,*] = 0
      len = obj.dim[x0]
      IF (full EQ 0) THEN BEGIN
         line[0:len/5,img.p-1] = 1
         line[4*len/5:len-1,img.p-1] = 1
      ENDIF ELSE line[*,img.p-1] = 1
      line0 = reform(line[*,*])
      ln0 = FID_DISPLAY_SCALE (line0, xsize, ysize, max1, 1)
      idx = where(ln0 GT 0, cnt)
      IF (cnt GT 0) THEN obj0[idx] = dsp[0].nc-1
   ENDIF
   widget_control, uv.wid.win[1], draw_xsize=xsize, draw_ysize=ysize
   wset, dsp[1].id
   tv, obj0
END
