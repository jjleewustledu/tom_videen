PRO FID_TILT, uv, zoom
   COMMON FID
   COMMON IMAGE_UTILS
   
   group = uv.wid.base
   base = WIDGET_BASE(title='Set Image Tilt', group_leader=group, row=2)   
   base1 = widget_base(base, row=1)
   s5up = WIDGET_BUTTON(base1, value=' Up 5 Slices ')
   s5down = WIDGET_BUTTON(base1, value=' Down 5 Slices ')  
   up = WIDGET_BUTTON(base1, value=' Up ')
   down = WIDGET_BUTTON(base1, value=' Down ')  
   downleft = WIDGET_BUTTON(base1, value=' Down Left ')  
   downright = WIDGET_BUTTON(base1, value=' Down Right ')  
   ant = WIDGET_BUTTON(base1, value=' Anterior ')
   post = WIDGET_BUTTON(base1, value=' Posterior ')
   mpln = WIDGET_BUTTON(base1, value=' Mid-Commissural Plane ')  
   exit = WIDGET_BUTTON(base1, value=' Exit ')  
   msg = widget_text(base, xsize=91)

   widget_control, uv.wid.win[1], GET_VALUE=win1
   draw = uv.wid.win[0]
   img = uv.img[1]
   xdim = img.dim[0]
   fp = 1
   lp = img.dim[1]
   pt1 = 11
   pt2 = 12
   IF (locus[2,pt1] GT 0) THEN BEGIN
      xp1 = locus[0,pt1]
      yp1 = locus[1,pt1]
      zp1 = locus[2,pt1]
      xp2 = locus[0,pt2]
      yp2 = locus[1,pt2]
      zp2 = locus[2,pt2]
   ENDIF ELSE BEGIN
      xp1 = 1.
      xp2 = xdim
      yp1 = img.f
      yp2 = img.f
      zp1 = img.p
      zp2 = img.p
   ENDELSE
   
   widget_control, base, /REALIZE, xoffset=0, yoffset=0
   WSET, win1
   WSHOW, win1
   FID_LINE, uv, xp1, yp1, zp1, xp2, yp2, zp2
   
   catch, error_status
   WHILE 1 DO BEGIN             ; Internal event loop   
      str = string(format='(A,F8.1,A,F8.1,F8.1)',"Pln: ",yp1, "     Z: ",zp1, zp2)
      widget_control, msg, set_value=str
      ev = WIDGET_EVENT([base, draw])

      CASE ev.id OF
         
         up: BEGIN
            IF (zp1 GT fp AND zp2 GT fp) THEN BEGIN 
               zp1 = zp1-1.0
               zp2 = zp2-1.0
               FID_LINE, uv, xp1, yp1, zp1, xp2, yp2, zp2
               widget_control, msg, set_value='AC and PC must be defined'
            ENDIF
         ENDCASE 
         
         down: BEGIN
            IF (zp1 LT lp AND zp2 LT lp) THEN BEGIN 
               zp1 = zp1+1.0
               zp2 = zp2+1.0
               FID_LINE, uv, xp1, yp1, zp1, xp2, yp2, zp2
            ENDIF
         ENDCASE 

         downleft: BEGIN
            IF (zp1 LT lp) THEN BEGIN 
               zp1 = zp1+1.0
               FID_LINE, uv, xp1, yp1, zp1, xp2, yp2, zp2
            ENDIF
         ENDCASE 

         downright: BEGIN
            IF (zp2 LT lp) THEN BEGIN 
               zp2 = zp2+1.0
               FID_LINE, uv, xp1, yp1, zp1, xp2, yp2, zp2
            ENDIF
         ENDCASE 

         ant: BEGIN
            IF (yp1 GT ff) THEN BEGIN 
               yp1 = yp1-1.0
               yp2 = yp1
               uv.img[0].f = fix(yp1+0.5)
               uv.img[1].f = uv.img[0].f
               widget_control, uv.wid.slider2, set_value=uv.img[0].f
               FID_LINE, uv, xp1, yp1, zp1, xp2, yp2, zp2
            ENDIF
         ENDCASE 
         
         post: BEGIN
            IF (yp1 LT lf) THEN BEGIN 
               yp1 = yp1+1.0
               yp2 = yp1
               uv.img[0].f = fix(yp1+0.5)
               uv.img[1].f = uv.img[0].f
               widget_control, uv.wid.slider2, set_value=uv.img[0].f
               FID_LINE, uv, xp1, yp1, zp1, xp2, yp2, zp2
            ENDIF
         ENDCASE 

         s5up: BEGIN
            IF (zp1-5 GT fp AND zp2-5 GT fp) THEN BEGIN 
               zp1 = zp1-5.0
               zp2 = zp2-5.0
               FID_LINE, uv, xp1, yp1, zp1, xp2, yp2, zp2
            ENDIF
         ENDCASE 
         
         s5down: BEGIN
            IF (zp1+5 LT lp AND zp2+5 LT lp) THEN BEGIN 
               zp1 = zp1+5.0
               zp2 = zp2+5.0
               FID_LINE, uv, xp1, yp1, zp1, xp2, yp2, zp2
            ENDIF
         ENDCASE 

         mpln: BEGIN
            IF (locus[1,0] GT 0 AND locus[1,1] GT 0) THEN BEGIN
                                ; round to nearest 1/2 pixel
               yp1 = ROUND((2*locus[1,0] + 2*locus[1,1])/2)
               yp1 = yp1/2.0
               yp2 = yp1
               uv.img[0].f = yp1
               uv.img[1].f = uv.img[0].f
               widget_control, uv.wid.slider1, set_value=uv.img[0].f
               FID_LINE, uv, xp1, yp1, zp1, xp2, yp2, zp2
            ENDIF ELSE widget_control, msg, set_value='AC and PC must be defined'
         ENDCASE

         exit:  BEGIN
            locus[0,pt1] = xp1
            locus[1,pt1] = yp1
            locus[2,pt1] = zp1
            locus[0,pt2] = xp2
            locus[1,pt2] = yp2
            locus[2,pt2] = zp2
            widget_control, base, /DESTROY
            RETURN
         ENDCASE 
         
      ENDCASE 
   ENDWHILE
END
