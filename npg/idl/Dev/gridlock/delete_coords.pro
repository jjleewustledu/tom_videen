PRO DELETE_COORDS, uv
   COMMON GRIDLOCK
   
   xdim = uv.img[0].dim[0]
   ydim = uv.img[0].dim[1]
   xpdim = uv.img[0].pdim[0]
   ypdim = uv.img[0].pdim[1]
   zoom = dsp[0].zoom
   IF (xpdim GT ypdim) THEN BEGIN
      xfact = xpdim/ypdim
   ENDIF ELSE BEGIN
      xfact = 1.
   ENDELSE

   WHILE 1 DO BEGIN             ; event loop   
      wset, dsp[0].id
      wshow, dsp[0].id
      str = "Left Button = indicate point in window 0; Right Button = exit"
      widget_control, uv.wid.msg, set_value=str
      ev = WIDGET_EVENT([uv.wid.base, uv.wid.win[0]])
      CASE ev.press OF

         1: BEGIN
            x = fix(ev.x/(xfact*zoom))
            y = fix(ev.y/ zoom)
            IF (x LT 0) OR (y LT 0) OR (x GE xdim) OR (y GE ydim) THEN return
            IF (npt EQ 0) THEN BEGIN
               str = "No points defined"
               widget_control, uv.wid.msg, set_value=str
               return
            ENDIF ELSE IF (npt EQ 1) THEN BEGIN	; Remove single point from list
               xtmp1 = xref[npt-1]
               ytmp1 = yref[npt-1]
               xtmp2 = xstd[npt-1]
               ytmp2 = ystd[npt-1]
            ENDIF ELSE BEGIN	; Find nearest point in ref data set and remove it
               nearpt = 0
               neardist = (xref(nearpt)-x)^2. + (yref(nearpt)-y)^2.
               FOR n=1,npt-1 DO BEGIN
                  dist = (xref(n)-x)^2. + (yref(n)-y)^2.
                  IF (dist LT neardist) THEN BEGIN
                     nearpt = n
                     neardist = dist
                  ENDIF
               ENDFOR
               xtmp1 = xref[nearpt]
               ytmp1 = yref[nearpt]
               xtmp2 = xstd[nearpt]
               ytmp2 = ystd[nearpt]
               xref[nearpt] = xref[npt-1]	; last point is list
               yref[nearpt] = yref[npt-1]
               xstd[nearpt] = xstd[npt-1]
               ystd[nearpt] = ystd[npt-1]
            ENDELSE
            npt = npt-1
            print,format='("Nearest Point:",I4,I4)',xtmp1,ytmp1
            x = xtmp1*zoom*xfact
            y = ytmp1*zoom
            xy = [[x,y+len],[x,y-len],[x,y],[x+len,y],[x-len,y]]
            plots, xy, color=0, /device
            x = xtmp2*zoom*xfact
            y = ytmp2*zoom
            xy = [[x,y+len],[x,y-len],[x,y],[x+len,y],[x-len,y]]
            wset, dsp[1].id
            plots, xy, color=0, /device
         ENDCASE 

         4: RETURN
         ELSE: 
      ENDCASE 
   ENDWHILE
END   
