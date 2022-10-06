;+
; NAME:
;	FID_DRAW
;
; PURPOSE:
;	Draws a point [xp,yp,zp] at the appropriate 2D location in the displayed image.
;
; CALLING SEQUENCE:
;	Result = FID_DRAW(s)
;
; MODIFICATION HISTORY:
;	Tom Videen, Sep 2005.
;-

PRO FID_DRAW, s
   COMMON FID
   COMMON IMAGE_UTILS
   
   str = string(xp, yp, zp, format='("X,Y,Z:",F8.1,F8.1,F8.1)')
   widget_control, s.pos_w, SET_VALUE=str

   x = (xp - 1)*s.zoom
   y = (s.img.dim[1] + 0.75 - yp)*s.zoom
   z = (s.img.dim[2] + 0.75 - zp)*s.zoom
   IF (cursor EQ 1) THEN BEGIN
      zlen = 10*s.zoom
      xy1 = [[x-20,y],[x+20,y]]
      xy2 = [[x,y-50],[x,y+50]]
      xz1 = [[x-20,z],[x+20,z]]
      xz2 = [[x,z-zlen],[x,z+zlen]]

      WSET, s.win0
      !x.s = [0, 1/float(!d.x_size)]
      !y.s = [0, 1/float(!d.y_size)]
      plots, xy1, color=dsp[0].nc
      plots, xy2, color=dsp[0].nc
      
      WSET, s.win1
      !x.s = [0, 1/float(!d.x_size)]
      !y.s = [0, 1/float(!d.y_size)]
      plots, xz1, color=dsp[0].nc
      plots, xz2, color=dsp[0].nc
   ENDIF ELSE IF (cursor EQ 2) THEN BEGIN
      n = 32
      a = findgen(n+1) * (2 * !pi/(n-1))
      xy = transpose([[x + radius * cos(a)], [y + radius * sin(a)]])
      WSET, s.win0
      !x.s = [0, 1/float(!d.x_size)]
      !y.s = [0, 1/float(!d.y_size)]
      plots, xy, color=dsp[0].nc

      xz = transpose([[x + radius * cos(a)], [z + radius * sin(a)]])
      WSET, s.win1
      !x.s = [0, 1/float(!d.x_size)]
      !y.s = [0, 1/float(!d.y_size)]
      plots, xz, color=dsp[0].nc
   ENDIF
END   

;+
; NAME:
;	FID_POINTS_EVENT
;
; PURPOSE:
;	Shows coordinates of a point [x,y,z] and saves it to a list if button pressed
;
; Called from the FID_POINTS event loop.
; CALLING SEQUENCE:
;	FID_POINTS_EVENT, ev, s
;
; Parameters:
;	ev = event structure
;	s = state structure
;
; MODIFICATION HISTORY:
;	Tom Videen, Sep 2005.
;-

PRO FID_POINTS_EVENT, ev, s	;; alway on transverse slice
   COMMON FID
   COMMON IMAGE_UTILS
   
   s.button = s.button OR ev.press XOR ev.release ; New button state   

   IF (s.button EQ 1) THEN BEGIN
      xp = ev.x/s.zoom + 1
      yp = s.img.dim[1] - ev.y/s.zoom
      FID_DRAW, s
   ENDIF 
   RETURN
END   
   
PRO FID_POINTS, uv, zoom
   COMMON FID
   COMMON IMAGE_UTILS
   
   group = uv.wid.base
   base = WIDGET_BASE(title='Select Image Points', group_leader=group, /ROW)   
   base1 = WIDGET_BASE(base,row=4)   
   base2 = WIDGET_BASE(base1,/ROW)
   up = WIDGET_BUTTON(base2, value=' Up ')
   down = WIDGET_BUTTON(base2, value=' Down ')  
   left = WIDGET_BUTTON(base2, value=' Left ')
   right = WIDGET_BUTTON(base2, value=' Right ')  
   ant = WIDGET_BUTTON(base2, value=' Anterior ')
   post = WIDGET_BUTTON(base2, value=' Posterior ')
   pos_w = WIDGET_TEXT(base2, YSIZE=1, XSIZE=25, /FRAME, VALUE='X,Y,Z:   0   0   0')
   record = WIDGET_BUTTON(base2, value=' Record ')
   listing = WIDGET_BUTTON(base2, value=' List ')
   base3 = WIDGET_BASE(base1,/ROW)
   fid = ['AC','PC','OX','OTL','OTR','BEL','BER','PTL','PTR','RNL','RNR','IPF']
   pnum = CW_BGROUP(base3, fid, LABEL_LEFT='Fiducial:', /ROW, /EXCLUSIVE, /NO_RELEASE, $
                    set_value=uv.wid.pnum)
   base4 = WIDGET_BASE(base1,/ROW)
   s5up = WIDGET_BUTTON(base4, value=' Up 5 Slices ')
   s5down = WIDGET_BUTTON(base4, value=' Down 5 Slices ')  
   cpln = WIDGET_BUTTON(base4, value=' Commissural Plane ')  
   mpln = WIDGET_BUTTON(base4, value=' Mid-Commissural Plane ')  
   msg = WIDGET_TEXT(base, YSIZE=12, XSIZE=35)
   curs = CW_BGROUP(base4, LABEL_LEFT='Cursor:',['Off','Cross','Circle'], /ROW, /EXCLUSIVE, /NO_RELEASE, $
                   /RETURN_INDEX, set_value=cursor)
   base5 = WIDGET_BASE(base1,/ROW)

   fname0 = uv.img[0].fname
   loc = rstrpos(fname0,'/')
   IF (loc GT 0) THEN fname0 = strmid(fname0,loc+1)
   name0 = WIDGET_TEXT(base5, value=fname0, xsize=35)

   fname1 = uv.img[1].fname
   loc = rstrpos(fname1,'/')
   IF (loc GT 0) THEN fname1 = strmid(fname1,loc+1)
   name1 = WIDGET_TEXT(base5, value=fname1, xsize=35)

   select1 = ['1\Radius','0\ 1 ','0\ 2 ','0\ 3 ','0\ 4 ','0\ 5 ','0\ 6 ','0\ 7 ','0\ 8 ', '0\ 9 ', '0\ 10 ']
   rad = cw_pdmenu(base5, select1)

   exit = WIDGET_BUTTON(base, value=' Exit ')  

   widget_control, uv.wid.win[0], GET_VALUE=win0
   widget_control, uv.wid.win[1], GET_VALUE=win1
   draw = uv.wid.win[0]
   WSET, win0
   img = uv.img[0]
   fp = 1
   lp = img.dim[2]
   xdim = img.dim[0]
   xp = 0.
   yp = 0.
   zp = 0.
   yp = img.f
   zp = img.p
   
   s = {POINT_STATE, $          ; Structure containing state
        base: base, $           ; Main base widget
        pos_w : pos_w, $        ; Position text widget
        msg : msg, $            ; Message window
        win0:  win0, $          ; draw window 1 (tra)
        win1:  win1, $          ; draw window 2 (cor)
        button: 0, $            ; button state
        zoom : zoom, $          ; zoom factor
        rad : rad, $            ; radius of circular cursor
        img : img $             ; pointer to image data
       }

   widget_control, base, /REALIZE, xoffset=0, yoffset=0
   WSHOW, win0
   msg = string(format='(A,/,A)', "Use left mouse button to add a point.", $
                "Button keys on left to shift it.")
   widget_control, s.msg, set_value=msg

   p = s.img.p
   imgname = img.fname
   loc = RSTRPOS (imgname, '/')
   IF (loc GT 0) THEN imgname = STRMID(imgname,loc+1)
   
   catch, error_status
   IF (error_status NE 0) THEN $
      widget_control, s.msg, set_value=!err_string

   WHILE 1 DO BEGIN             ; Internal event loop   
      IF (uv.wid.pnum GE 3 AND uv.wid.pnum LE 6) THEN BEGIN
         IF (locus[1,0] GT 0 AND locus[1,1] GT 0) THEN BEGIN
                                ; round to nearest 1/2 pixel
            yp = ROUND((2*locus[1,0] + 2*locus[1,1])/2)
            yp = yp/2.0
            uv.img[0].f = yp
            uv.img[1].f = uv.img[0].f
            s.img.f = uv.img[0].f
            widget_control, uv.wid.slider1, set_value=uv.img[0].f
            FID_DISPLAY, uv
            FID_DRAW, s
         ENDIF ELSE widget_control, s.msg, set_value='AC and PC must be defined'
      ENDIF
      IF (uv.wid.pnum GE 5 AND uv.wid.pnum LE 8) THEN BEGIN
         IF (locus[2,0] GT 0) THEN BEGIN
            zp = locus[2,0]
            uv.img[0].p = zp
            uv.img[1].p = uv.img[0].p
            s.img.p = uv.img[0].p
            widget_control, uv.wid.slider1, set_value=uv.img[0].p
            FID_DISPLAY, uv
            FID_DRAW, s
         ENDIF ELSE widget_control, s.msg, set_value='AC is not defined'
      ENDIF

      ev = WIDGET_EVENT([base, draw])

      CASE ev.id OF
         
         draw: BEGIN
            FID_POINTS_EVENT, ev, s
            IF (s.img.f NE yp AND yp GT 0) THEN BEGIN 
               uv.img[0].f = yp
               uv.img[1].f = yp
               widget_control, uv.wid.slider2, set_value=uv.img[0].f
               FID_DISPLAY, uv
               FID_DRAW, s
            ENDIF
         ENDCASE 
         
         up: BEGIN
            IF (zp GT fp) THEN BEGIN 
               zp = zp-0.5
               uv.img[0].p = fix(zp+0.5)
               uv.img[1].p = uv.img[0].p
               widget_control, uv.wid.slider1, set_value=uv.img[0].p
               FID_DISPLAY, uv
               FID_DRAW, s
            ENDIF
         ENDCASE 
         
         down: BEGIN
            IF (zp LT lp) THEN BEGIN 
               zp = zp+0.5
               uv.img[0].p = fix(zp+0.5)
               uv.img[1].p = uv.img[0].p
               widget_control, uv.wid.slider1, set_value=uv.img[0].p
               FID_DISPLAY, uv
               FID_DRAW, s
            ENDIF
         ENDCASE 

         left:  BEGIN
            IF (xp GT 1) THEN BEGIN
               xp = xp-0.5
               FID_DISPLAY, uv
               FID_DRAW, s
            ENDIF 
         ENDCASE 

         right:  BEGIN
            IF (xp LT xdim) THEN BEGIN
               xp = xp+0.5
               FID_DISPLAY, uv
               FID_DRAW, s
            ENDIF 
         ENDCASE 

         ant: BEGIN
            IF (yp GT ff) THEN BEGIN 
               yp = yp-0.5
               uv.img[0].f = fix(yp+0.5)
               uv.img[1].f = uv.img[0].f
               widget_control, uv.wid.slider2, set_value=uv.img[0].f
               FID_DISPLAY, uv
               FID_DRAW, s
            ENDIF
         ENDCASE 
         
         post: BEGIN
            IF (yp LT lf) THEN BEGIN 
               yp = yp+0.5
               uv.img[0].f = fix(yp+0.5)
               uv.img[1].f = uv.img[0].f
               widget_control, uv.wid.slider2, set_value=uv.img[0].f
               FID_DISPLAY, uv
               FID_DRAW, s
            ENDIF
         ENDCASE 

         pnum:   BEGIN
            uv.wid.pnum = ev.value
            pt = uv.wid.pnum 
			IF (pt EQ 11) THEN pt = 13
            CASE pt OF
               0: msg = "Anterior Commissure"
               1: msg = "Posterior Commissure"
               2: msg = "Optic Chiasm"
               3: msg = "Optic Tract Left"
               4: msg = "Optic Tract Right"
               5: msg = "Brain Edge Left"
               6: msg = "Brain Edge Right"
               7: msg = "Putamen Left"
               8: msg = "Putamen Right"
               9: msg = "Red Nucleus Left"
               10: msg = "Red Nucleus Right"
               11: msg = "LVT"
               12: msg = "RVT"
               13: msg = "Interpeduncular Fissure"
            ENDCASE
            IF (locus[0,pt] GT 0) THEN BEGIN 
               msg = string(format='(A,/,F8.1,F8.1,F8.1)',msg,locus[*,pt])
               xp = locus[0,pt]
               yp = locus[1,pt]
               zp = locus[2,pt]
               uv.img[0].f = yp
               uv.img[1].f = yp
               uv.img[0].p = fix(zp+0.5)
               uv.img[1].p = zp
               s.img.p = uv.img[0].p
               widget_control, uv.wid.slider1, set_value=zp
               widget_control, uv.wid.slider2, set_value=yp
               FID_DISPLAY, uv
               FID_DRAW, s
            ENDIF
            widget_control, s.msg, set_value=msg
         ENDCASE
         
         record: BEGIN
            pt = uv.wid.pnum
			IF (pt EQ 11) THEN pt = 13
            locus[0,pt] = xp
            locus[1,pt] = yp
            locus[2,pt] = zp
            list = strarr(12)
            FOR n=0, 10 DO list[n] = string(format='(A,F8.1,F8.1,F8.1)',fid[n],locus[*,n])
            list[11] = string(format='(A,F8.1,F8.1,F8.1)',fid[11],locus[*,13])
            widget_control, s.msg, set_value=list
            saved = 0
         ENDCASE 

         listing: BEGIN
            list = strarr(12)
            FOR n=0, 10 DO list[n] = string(format='(A,F8.1,F8.1,F8.1)',fid[n],locus[*,n])
            list[11] = string(format='(A,F8.1,F8.1,F8.1)',fid[11],locus[*,13])
            widget_control, s.msg, set_value=list
         ENDCASE 

         s5up: BEGIN
            IF (zp-5 GT fp) THEN BEGIN 
               zp = zp-5
               uv.img[0].p = fix(zp+0.5)
               uv.img[1].p = uv.img[0].p
               s.img.p = uv.img[0].p
               widget_control, uv.wid.slider1, set_value=uv.img[0].p
               FID_DISPLAY, uv
               FID_DRAW, s
            ENDIF
         ENDCASE 
         
         s5down: BEGIN
            IF (zp+5 LT lp) THEN BEGIN 
               zp = zp+5
               uv.img[0].p = fix(zp+0.5)
               uv.img[1].p = uv.img[0].p
               s.img.p = uv.img[0].p
               widget_control, uv.wid.slider1, set_value=uv.img[0].p
               FID_DISPLAY, uv
               FID_DRAW, s
            ENDIF
         ENDCASE 

         cpln: BEGIN
            IF (locus[2,0] GT 0) THEN BEGIN
               zp = locus[2,0]
               uv.img[0].p = zp
               uv.img[1].p = uv.img[0].p
               s.img.p = uv.img[0].p
               widget_control, uv.wid.slider1, set_value=uv.img[0].p
               FID_DISPLAY, uv
               FID_DRAW, s
            ENDIF ELSE widget_control, s.msg, set_value='AC is not defined'
         ENDCASE 

         mpln: BEGIN
            IF (locus[1,0] GT 0 AND locus[1,1] GT 0) THEN BEGIN
                                ; round to nearest 1/2 pixel
               yp = ROUND((2*locus[1,0] + 2*locus[1,1])/2)
               yp = yp/2.0
               uv.img[0].f = yp
               uv.img[1].f = uv.img[0].f
               s.img.f = uv.img[0].f
               widget_control, uv.wid.slider1, set_value=uv.img[0].f
               FID_DISPLAY, uv
               FID_DRAW, s
            ENDIF ELSE widget_control, s.msg, set_value='AC and PC must be defined'
         ENDCASE 

         curs: BEGIN
            cursor = ev.value
            pt = uv.wid.pnum 
			IF (pt EQ 11) THEN pt = 13
            IF (cursor GT 0 AND locus[0,pt] GT 0) THEN BEGIN 
               xp = locus[0,pt]
               yp = locus[1,pt]
               zp = locus[2,pt]
               uv.img[0].f = yp
               uv.img[1].f = yp
               uv.img[0].p = fix(zp+0.5)
               uv.img[1].p = zp
               s.img.p = uv.img[0].p
               widget_control, uv.wid.slider1, set_value=zp
               widget_control, uv.wid.slider2, set_value=yp
            ENDIF
            FID_DISPLAY, uv
            FID_DRAW, s
         ENDCASE 

         rad: BEGIN
            radius = 3 * ev.value
            FID_DISPLAY, uv
            FID_DRAW, s
         ENDCASE 

         exit:  BEGIN
            widget_control, base, /DESTROY
            RETURN
         ENDCASE 
         
      ENDCASE 
   ENDWHILE
END
