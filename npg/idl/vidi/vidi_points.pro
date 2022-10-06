;+
; NAME:
;	VIDI_POINTS_DRAW
;
; PURPOSE:
;	Draws a point [x,y,z] at the appropriate 2D location in the displayed image.
;
; CALLING SEQUENCE:
;	Result = VIDI_POINTS_DRAW(s, np)
;
; MODIFICATION HISTORY:
;	Tom Videen, Sep 2002.
;-

PRO VIDI_POINTS_DRAW, s, np
   COMMON VIDI
   COMMON IMAGE_UTILS
   
   IF ((npt LE 0) OR (np LE 0) OR (np GT npt)) THEN RETURN
   WSET, s.win

   xfact = 1.
   yfact = 1.
   CASE orient OF
      0: BEGIN
         xsiz = s.img.pdim[0]
         ysiz = s.img.pdim[1]
         ydim = s.img.dim[1]
         x = ptlist[0,np-1]
         y = ydim - ptlist[1,np-1]
         z = ptlist[2,np-1]
      ENDCASE
      1: BEGIN
         xsiz = s.img.pdim[0]
         ysiz = s.img.pdim[2]
         ydim = s.img.dim[2]
         x = ptlist[0,np-1]
         y = ydim - ptlist[2,np-1]
         z = ptlist[1,np-1]
      ENDCASE
      2: BEGIN
         xsiz = s.img.pdim[1]
         ysiz = s.img.pdim[2]
         ydim = s.img.dim[2]
         x = ptlist[1,np-1]
         y = ydim - ptlist[2,np-1]
         z = ptlist[0,np-1]
      ENDCASE
   ENDCASE
   IF (ysiz GT xsiz) THEN yfact = ysiz/xsiz $
   ELSE IF (xsiz GT ysiz) THEN xfact = xsiz/ysiz
   xy = [[x-1,y-1],[x-1,y+1],[x+1,y+1],[x+1,y-1],[x-1,y-1]]

   !x.s = [0, s.zoom/(float(!d.x_size) / xfact)]
   !y.s = [0, s.zoom/(float(!d.y_size) / yfact)]
   plots, xy
   
END   

;+
; NAME:
;	VIDI_POINTS_EVENT
;
; PURPOSE:
;	Shows coordinates of a point [x,y,z] and saves it to a list if button pressed
;
; Called from the VIDI_POINTS event loop.
; CALLING SEQUENCE:
;	VIDI_POINTS_EVENT, ev, s
;
; Parameters:
;	ev = event structure
;	s = state structure
;
; MODIFICATION HISTORY:
;	Tom Videen, Sep 2002.
;-

PRO VIDI_POINTS_EVENT, ev, s
   COMMON VIDI
   COMMON IMAGE_UTILS
   
   s.button = s.button OR ev.press XOR ev.release ; New button state   

   xfact = 1.
   yfact = 1.
   CASE orient OF
      0: BEGIN
         xsiz = s.img.pdim[0]
         ysiz = s.img.pdim[1]
         ydim = s.img.dim[1]
      ENDCASE
      1: BEGIN
         xsiz = s.img.pdim[0]
         ysiz = s.img.pdim[2]
         ydim = s.img.dim[2]
      ENDCASE
      2: BEGIN
         xsiz = s.img.pdim[1]
         ysiz = s.img.pdim[2]
         ydim = s.img.dim[2]
      ENDCASE
   ENDCASE
   IF (ysiz GT xsiz) THEN yfact = ysiz/xsiz $
   ELSE IF (xsiz GT ysiz) THEN xfact = xsiz/ysiz
   xc = ev.x / (s.zoom * xfact) + 1
   yc = ydim - ((ev.y + 1) / (s.zoom * yfact) - 1)
   CASE orient OF
      0: BEGIN
         x = xc
         y = yc
         z = s.img.p
      ENDCASE
      1: BEGIN
         x = xc
         y = s.img.p
         z = yc
      ENDCASE
      2: BEGIN
         x = s.img.p
         y = xc
         z = yc
      ENDCASE
   ENDCASE

   str = string(x, y, z, format='("X,Y,Z:",i6,i6,i6)')
   widget_control, s.pos_w, SET_VALUE=str
   
   IF (s.button EQ 1) THEN BEGIN ; Create
      IF (npt LT (size(ptlist))[2]) THEN BEGIN
         ptlist[*,npt] = [x,y,z]
         npt = npt+1
         VIDI_POINTS_DRAW, s, npt
      ENDIF 
   ENDIF 
   RETURN
END   
   
PRO VIDI_POINTS, uv, inum, zoom
   COMMON VIDI
   COMMON IMAGE_UTILS
   
   img = uv.img[inum]
   draw = uv.wid.win[inum]
   group = uv.wid.base

   CASE orient OF
      0: BEGIN                  ; transverse
         fp = img.fp
         lp = img.lp
      ENDCASE
      1: BEGIN                  ; coronal
         fp = 1
         lp = img.dim[1]
      ENDCASE
      2: BEGIN                  ; sagittal
         fp = 1
         lp = img.dim[0]
      ENDCASE
   ENDCASE

   base = WIDGET_BASE(title='Select Image Points', group_leader=group, /ROW)   
   base1 = WIDGET_BASE(base,/COLUMN)   
   base2 = WIDGET_BASE(base1, /ROW)
   select2 = ['1\Show','0\Points Defined on This Plane','0\All Points from All Planes']
   temp2 = cw_pdmenu(base2, select2, /RETURN_NAME)
   select3 = ['1\Delete','0\Last Point', '0\All Points']
   temp3 = CW_PDMENU(base2, select3, /RETURN_NAME)
   temp4 = CW_BGROUP(base2, /ROW, /NO_RELEASE, /RETURN_NAME, ['Read List', 'Write List', 'Exit'])
   
   base3 =  WIDGET_BASE(base1,/ROW)
   pos_w = WIDGET_TEXT(base3, YSIZE=1, XSIZE=18, /FRAME, VALUE='X,Y,Z:   0   0   0')
   up = WIDGET_BUTTON(base3, value=' Up ')
   down = WIDGET_BUTTON(base3, value=' Down ')  

   base5 = WIDGET_BASE(base1,/ROW)
   msg = WIDGET_TEXT(base5, YSIZE=3, XSIZE=35)

   base6 = WIDGET_BASE(base1,/ROW)
   count = cw_field(base1, title='Number of Defined Points', /noedit, value=npt, xsize=5, ysize=1)

   widget_control, draw, GET_VALUE=win
   WSET, win
   
   s = {POINT_STATE, $          ; Structure containing state
        base: base, $           ; Main base widget
        pos_w : pos_w, $        ; Position text widget
        msg : msg, $            ; Message window
        win:  win, $            ; draw widget window #
        button: 0, $            ; button state
        zoom : zoom, $          ; zoom factor
        orient : orient, $      ; orientation
        img : img $             ; pointer to image data
       }
   
   widget_control, base, /REALIZE, xoffset=0, yoffset=0
   WSHOW, win
   msg = string(format='(A,/,A,/,A)', "Use the left mouse button to add points.", $
                "Read List reads coordinates from a SPED file.", $
                "Write List saves to a SPED file.")
   widget_control, s.msg, set_value=msg

   p = s.img.p
   CASE orient OF
      0: pln = 2
      1: pln = 1
      2: pln = 0
   ENDCASE
   imgname = img.fname
   loc = RSTRPOS (imgname, '/')
   IF (loc GT 0) THEN imgname = STRMID(imgname,loc+1)
   
   catch, error_status
   IF (error_status NE 0) THEN $
      widget_control, s.msg, set_value=!err_string

   WHILE 1 DO BEGIN             ; Internal event loop   
      ev = WIDGET_EVENT([base, draw])
      CASE ev.id OF
         
         draw: VIDI_POINTS_EVENT, ev, s
         
         up: BEGIN
            p = s.img.p
            IF (p GT fp) THEN BEGIN
               p = p-1
               s.img.p = p
               s.img.p = p
               uv.img[0].p = p
               uv.img[1].p = p
               CASE orient OF
                  0: str = string(0,0,p, format='("X,Y,Z:",i6,i6,i6)')
                  1: str = string(0,p,0, format='("X,Y,Z:",i6,i6,i6)')
                  2: str = string(p,0,0, format='("X,Y,Z:",i6,i6,i6)')
               ENDCASE
               widget_control, s.pos_w, SET_VALUE=str
               widget_control, uv.wid.slider, set_value=p
               VIDI_DISPLAY, uv
            ENDIF 
         ENDCASE 
         
         down: BEGIN
            p = s.img.p
            IF (p LT lp) THEN BEGIN
               p = p+1
               s.img.p = p
               s.img.p = p
               uv.img[0].p = p
               uv.img[1].p = p
               CASE orient OF
                  0: str = string(0,0,p, format='("X,Y,Z:",i6,i6,i6)')
                  1: str = string(0,p,0, format='("X,Y,Z:",i6,i6,i6)')
                  2: str = string(p,0,0, format='("X,Y,Z:",i6,i6,i6)')
               ENDCASE
               widget_control, s.pos_w, SET_VALUE=str
               widget_control, uv.wid.slider, set_value=p
               VIDI_DISPLAY, uv
            ENDIF 
         ENDCASE 

         ELSE: CASE ev.value OF
            
            'Read List': BEGIN
               fname = dialog_pickfile(FILE='*',FILTER='*.pts', /read)
               IF (strlen(fname) EQ 0) THEN BEGIN
                  widget_control, s.msg, set_value='No file entered'
               ENDIF ELSE BEGIN
                  openr, listlun, fname, ERROR=err, /GET_LUN
                  IF (err NE 0) THEN BEGIN
                     widget_control, s.msg, set_value='Cannot open file '+fname
                     listlun = 0
                  ENDIF ELSE BEGIN
                     str = ''
                     coord = intarr(3)
                     readf, listlun, str
                     readf, listlun, npt
                     FOR n=1,npt DO BEGIN
                        readf, listlun, coord
                        ptlist[*,n-1] = coord
                     ENDFOR
                     close, listlun
                     free_lun, listlun
                  ENDELSE
               ENDELSE
            ENDCASE
            
            'Write List': BEGIN
               fname = dialog_pickfile(FILTER='*.pts', /WRITE)
               IF (strlen(fname) EQ 0) THEN BEGIN
                  widget_control, s.msg, set_value='No file entered'
               ENDIF ELSE BEGIN
                  k = rstrpos(fname,'.')
                  IF (k GT 0) THEN fname = strmid(fname,0,k)
                  fname = fname + '.pts'
                  openw, listlun, fname, ERROR=err, /GET_LUN
                  IF (err NE 0) THEN BEGIN
                     widget_control, s.msg, set_value='Cannot open file '+fname
                     listlun = 0
                  ENDIF ELSE BEGIN
                     printf, listlun, "Points defined on ",imgname
                     printf, listlun, npt
                     FOR n=1,npt DO printf, listlun, ptlist[*,n-1]
                     close, listlun
                     free_lun, listlun
                     msg = string(format='(A,/,A)', "Points saved to", fname)
                     widget_control, s.msg, set_value=msg
                  ENDELSE 
               ENDELSE 
            ENDCASE 
            
            'Points Defined on This Plane': IF npt GT 0 THEN FOR np=1,npt DO $
               IF ptlist[pln,np-1] EQ p THEN VIDI_POINTS_DRAW, s, np
            
            'All Points from All Planes': IF npt GT 0 THEN FOR np=1,npt DO VIDI_POINTS_DRAW, s, np
            
            'Last Point': IF npt GT 0 THEN npt = npt-1
            
            'All Points': npt = 0
            
            'Exit':  BEGIN
               widget_control, base, /DESTROY
               RETURN
            ENDCASE 
            
         ENDCASE   
      ENDCASE 
      widget_control, count, SET_VALUE=npt
   ENDWHILE
END   
