PRO DEFINE_COORDS, uv
   COMMON GRIDLOCK
   
   xdim = uv.img[0].dim[0]
   ydim = uv.img[0].dim[1]
   xpdim = uv.img[0].pdim[0]
   ypdim = uv.img[0].pdim[1]
   zoom = dsp[0].zoom
   device,SET_GRAPHICS_FUNCTION=3 ; plot points with white cross
   IF (xpdim GT ypdim) THEN BEGIN
      xfact = xpdim/ypdim
   ENDIF ELSE BEGIN
      xfact = 1.
   ENDELSE
   new = 1
   activ = 0

   WHILE 1 DO BEGIN             ; event loop   
      wset, dsp[activ].id
      wshow, dsp[activ].id
      activ_win = uv.wid.win[activ]
      IF (new) THEN BEGIN
         IF (activ EQ 0) THEN BEGIN
            str = "Left Button = reference point in window 0; Right Button = exit"
         ENDIF ELSE BEGIN
            str = "Left Button = matching point in window 1"
         ENDELSE
         widget_control, uv.wid.msg, set_value=str
         new = 0
      ENDIF
      ev = WIDGET_EVENT([uv.wid.base, activ_win])
      CASE ev.press OF

         1: BEGIN
            x = fix(ev.x/(xfact*zoom))
            y = fix(ev.y/ zoom)
            IF (x GE 0) AND (y GE 0) AND (x LT xdim) AND (y LT ydim) THEN BEGIN
               IF (activ EQ 0) THEN BEGIN
                  xref[npt] = x
                  yref[npt] = y
                  print,format='("Reference:",I4,I4)',x,y
                  activ = 1
               ENDIF ELSE BEGIN
                  xstd[npt] = x
                  ystd[npt] = y
                  npt = npt+1
                  print,format='("Study:",I8,I4)',x,y
                  activ = 0
                  str = string(format='("Points: ",i)', npt)
                  widget_control, uv.wid.msg, set_value=str
               ENDELSE
               x = x*zoom*xfact
               y = y*zoom
               xy = [[x,y+len],[x,y-len],[x,y],[x+len,y],[x-len,y]]
               plots, xy, /device
               new = 1
            ENDIF
         xy = VOI_TRANSPOSE(xy, ydim, /DISPLAY)
         ENDCASE 

         4: RETURN
         ELSE: 
      ENDCASE 
   ENDWHILE
END   
