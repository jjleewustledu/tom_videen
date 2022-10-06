PRO SHOW_COORDS, uv
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
   ref_id = dsp[0].id
   std_id = dsp[1].id
   transform_display, uv

   wset, ref_id
   wshow, ref_id
   FOR n=0,npt-1 DO BEGIN
      x = xref[n]
      y = yref[n]
      x = x*zoom*xfact
      y = y*zoom
      xy = [[x,y+len],[x,y-len],[x,y],[x+len,y],[x-len,y]]
      plots, xy, /device
   ENDFOR
   wset, std_id
   wshow, std_id
   FOR n=0,npt-1 DO BEGIN
      x = xstd[n]
      y = ystd[n]
      x = x*zoom*xfact
      y = y*zoom
      xy = [[x,y+len],[x,y-len],[x,y],[x+len,y],[x-len,y]]
      plots, xy, /device
   ENDFOR

END   
