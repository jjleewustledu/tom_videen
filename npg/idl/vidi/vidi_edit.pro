;+
; NAME:
;	VIDI_EDIT
; PURPOSE:
;	Widget to edit an image (by setting pixels in ROIs to 0)
; CATEGORY:
;	Image processing.
; CALLING SEQUENCE:
;	Result = VIDI_EDIT(uv,i,zoom)
; INPUTS:
;	Uv  = array of image structures
;	I = index to array
;	Zoom = zoom factor 
; OUTPUTS:
;	Modified image.
; RESTRICTIONS:
;	This is a MODAL widget.  No other widget applications will be
;	responsive while this widget is in use.
; MODIFICATION HISTORY:
;	Tom Videen, Feb 1999.
;-

PRO VIDI_EDIT_DRAW, s, i, xdim, ydim, xzoom, yzoom, FILL= fill
; Draw outline (or fill if FILL is set) of ROI
;	or the ith segment of a polygon if i < 0.
; Image coordinates are tranposed to screen coordinates before drawing.
; Fill is used to restore.
; Parameters:
;	s = state structure
;	i = segment number
   
   COMMON VIDI

   n = (*s.roi).num
   IF (n LT 1) THEN RETURN
   
   WSET, s.win
   DEVICE, SET_GRAPHICS_FUNCTION=3 ; source
   
   xsave = !x.s & ysave = !y.s  ; Set scaling to pixel coords
   !x.s = [0, xzoom/float(!d.x_size)]
   !y.s = [0, yzoom/float(!d.y_size)]
   
   IF (n GE 2) THEN BEGIN       ; Draw
      xy = (*(*s.roi).loc)[*,0:n-1]
      ;xy = VOI_TRANSPOSE(xy, ydim, /DISPLAY)
      xy = VOI_TRANSPOSE(xy, ydim)
      IF KEYWORD_SET(FILL) THEN polyfill, xy, color=label $
      ELSE IF (i GE 0) THEN plots,xy[*, i:i+1], color=label $ ; One segment
      ELSE plots, xy, color=label ; All of it
   ENDIF    
   !x.s = xsave & !y.s = ysave  ; Restore scaling
END   

  
PRO VIDI_EDIT_EVENT, ev, s, xdim, ydim, xzoom, yzoom
; Called from the VIDI_EDIT event loop.
; Parameters:
;	ev = event structure
;	s = state structure
   
   s.button = s.button OR ev.press XOR ev.release ; New button state   
   n = (*s.roi).num
   x = (ev.x - s.offset[0]) / xzoom ; Screen coordinates (0,0 is lower left)
   y = (ev.y - s.offset[1]) / yzoom
   ;xy = [fix(x)-1,fix(y)-1]
   xy = [fix(x),fix(y)]
   xy = VOI_TRANSPOSE(xy, ydim) ; Image coordinates (0,0 is upper left)
   WIDGET_CONTROL, s.pos_w, SET_VALUE=string(xy+1, format='("Cursor Position: ",i,", ",i)')

   IF (xy[0] LT 0) OR (xy[1] LT 0) OR $ ; Check if cursor is within image
      (xy[0] GT xdim) OR (xy[1] GT ydim) THEN RETURN
   IF ev.press NE 0 THEN s.drag = [x,y] ; Start of drag operation
   
   IF (s.button EQ 1) THEN BEGIN ; Create
      s.clear = 0
      IF NOT ptr_valid((*s.roi).loc) THEN BEGIN
         (*s.roi).type = 2
         (*s.roi).num = 0
         (*s.roi).loc = ptr_new(lonarr(2,100))
         n = 0
      ENDIF
      IF (n GT 0) THEN $        ; Check for duplicates
         IF(xy[0] EQ (*(*s.roi).loc)[0,n-1] AND xy[1] EQ (*(*s.roi).loc)[1,n-1]) THEN RETURN
      IF ((n+1) GE N_ELEMENTS(*(*s.roi).loc)/2) THEN BEGIN ; Extend array
         new = ptr_new(lonarr(2,2*(n+1)))
         (*new)[*,0:n-1] = (*(*s.roi).loc)[*,0:n-1]
         ptr_free, (*s.roi).loc
         (*s.roi).loc = new
      ENDIF
      IF (n EQ 0) THEN BEGIN 
         (*(*s.roi).loc)[*,n] = xy ; then add the point to the trace
         n = n + 1
         (*s.roi).num = n    
         IF ((*s.roi).type EQ 2 AND n GE 2) THEN VIDI_EDIT_DRAW, s, n-2, xdim, ydim, xzoom, yzoom
      ENDIF ELSE BEGIN          ; make certain trace is 4-connected
         lastx = (*(*s.roi).loc)[0,n-1]
         lasty = (*(*s.roi).loc)[1,n-1]
         dx = abs(xy[0]-lastx)
         dy = abs(xy[1]-lasty)
         IF (dx GT dy) THEN BEGIN
            dir = 0
            IF (dy GT 0) THEN slope = dx/dy ELSE slope = 1
         ENDIF ELSE BEGIN
            dir = 1
            IF (dx GT 0) THEN slope = dy/dx ELSE slope = 1
         ENDELSE
         WHILE (xy[0] NE lastx OR xy[1] NE lasty) DO BEGIN
            IF ((n+1) GE N_ELEMENTS(*(*s.roi).loc)/2) THEN BEGIN ; Extend array
               new = ptr_new(lonarr(2,2*(n+1)))
               (*new)[*,0:n-1] = (*(*s.roi).loc)[*,0:n-1]
               ptr_free, (*s.roi).loc
               (*s.roi).loc = new
            ENDIF
            IF (dir EQ 0) THEN BEGIN
               IF (xy[1] EQ lasty) THEN slope = 1 $
               ELSE slope = abs(xy[0]-lastx)/abs(xy[1]-lasty)
               FOR k=1,slope DO BEGIN
                  IF ((n+1) GE N_ELEMENTS(*(*s.roi).loc)/2) THEN BEGIN ; Extend array
                     new = ptr_new(lonarr(2,2*(n+1)))
                     (*new)[*,0:n-1] = (*(*s.roi).loc)[*,0:n-1]
                     ptr_free, (*s.roi).loc
                     (*s.roi).loc = new
                  ENDIF
                  IF (xy[0] NE lastx) THEN BEGIN
                     dx = xy[0]-lastx
                     nextx = lastx + dx/abs(dx)
                     (*(*s.roi).loc)[0,n] = nextx
                     (*(*s.roi).loc)[1,n] = lasty
                     lastx = nextx
                     n = n + 1
                     (*s.roi).num = n
                     IF ((*s.roi).type EQ 2 AND n GE 2) THEN VIDI_EDIT_DRAW, s, n-2, xdim, ydim, xzoom, yzoom
                  ENDIF
               ENDFOR
               IF (xy[1] NE lasty) THEN BEGIN
                  dy = xy[1]-lasty
                  nexty = lasty + dy/abs(dy)
                  (*(*s.roi).loc)[0,n] = lastx
                  (*(*s.roi).loc)[1,n] = nexty
                  lasty = nexty
                  n = n + 1
                  (*s.roi).num = n
                  IF ((*s.roi).type EQ 2 AND n GE 2) THEN VIDI_EDIT_DRAW, s, n-2, xdim, ydim, xzoom, yzoom
               ENDIF
            ENDIF ELSE BEGIN
               IF (xy[0] EQ lastx) THEN slope = 1 $
               ELSE slope = abs(xy[1]-lasty)/abs(xy[0]-lastx)
               FOR k=1,slope DO BEGIN
                  IF ((n+1) GE N_ELEMENTS(*(*s.roi).loc)/2) THEN BEGIN ; Extend array
                     new = ptr_new(lonarr(2,2*(n+1)))
                     (*new)[*,0:n-1] = (*(*s.roi).loc)[*,0:n-1]
                     ptr_free, (*s.roi).loc
                     (*s.roi).loc = new
                  ENDIF
                  IF (xy[1] NE lasty) THEN BEGIN
                     dy = xy[1]-lasty
                     nexty = lasty + dy/abs(dy)
                     (*(*s.roi).loc)[0,n] = lastx
                     (*(*s.roi).loc)[1,n] = nexty
                     lasty = nexty
                     n = n + 1
                     (*s.roi).num = n
                     IF ((*s.roi).type EQ 2 AND n GE 2) THEN VIDI_EDIT_DRAW, s, n-2, xdim, ydim, xzoom, yzoom
                  ENDIF
               ENDFOR
               IF (xy[0] NE lastx) THEN BEGIN
                  dx = xy[0]-lastx
                  nextx = lastx + dx/abs(dx)
                  (*(*s.roi).loc)[0,n] = nextx
                  (*(*s.roi).loc)[1,n] = lasty
                  lastx = nextx
                  n = n + 1
                  (*s.roi).num = n
                  IF ((*s.roi).type EQ 2 AND n GE 2) THEN VIDI_EDIT_DRAW, s, n-2, xdim, ydim, xzoom, yzoom
               ENDIF
            ENDELSE
         ENDWHILE 
      ENDELSE 
   ENDIF 
   RETURN
END   
   

PRO VIDI_EDIT, uv, i, zoom
   COMMON VIDI
   COMMON IMAGE_UTILS
   
   draw = uv.wid.win[i]
   i = i < 1                    ; if img[2] is selected, edit img[1] in win[2]
   img = uv.img[i]
   base = WIDGET_BASE(title='Image Edit', group_leader=uv.wid.base, /COLUMN)   
   
   base1 =  WIDGET_BASE(base,/ROW)
   val = CW_FIELD(base1, title='Fill Value', value=uv.wid.editval, $
                  xsize=8, /INTEGER, /RETURN_EVENTS)
   fill = WIDGET_BUTTON(base1, value='Fill')
   restore = WIDGET_BUTTON(base1, value='Restore')
   up = WIDGET_BUTTON(base1, value='<- Up  ')
   down = WIDGET_BUTTON(base1, value=' Down ->')  
   exit = WIDGET_BUTTON(base1, value='Exit')
   
   base2 = WIDGET_BASE(base,/ROW)
   replace = WIDGET_BUTTON(base2, value='Replace (this plane) Only If')
   rsign = CW_BGROUP(base2, ['>=','=','<='],/ROW, /EXCLUSIVE, set_value=uv.wid.rsign)
   rthresh = CW_FIELD(base2, title=' ', value=uv.wid.rthresh, $
                      xsize=8, /INTEGER, /RETURN_EVENTS)

   base3 = WIDGET_BASE(base,/ROW)
   rplanes = CW_BGROUP(base3, ['Current','All Above','All Below','All', $
                               'Slab Above', 'Slab Below', 'Slab Up&Down'], $
                       LABEL_LEFT='Remove from Planes:', /ROW, /EXCLUSIVE, set_value=uv.wid.rplanes)
   
   base4 = WIDGET_BASE(base,/ROW)
   rmode = CW_BGROUP(base4, ['Inside','Outside','Erase'], $
                     LABEL_LEFT='Removal Area:', /ROW, /EXCLUSIVE, set_value=uv.wid.rmode)
   rerase = CW_FIELD(base4, title='Eraser Size', value=uv.wid.rerase, $
                     xsize=4, /INTEGER, /RETURN_EVENTS)
   rslab = CW_FIELD(base4, title='Slab Planes', value=uv.wid.rslab, $
                    xsize=4, /INTEGER, /RETURN_EVENTS)

   pos_w = WIDGET_TEXT(base, YSIZE=1, XSIZE=45, /FRAME, $
                       VALUE='Cursor Position:    0,    0')
   msg = widget_text(base, YSIZE=3)
   
   voi = ptr_new({VOI})
   (*voi).id = 'VOI'
   roi = VOI_APPEND(voi)        ; position pointer to free element at end of list
   WIDGET_CONTROL, draw, GET_VALUE=win
   WSET, win
   
   image_size = [!d.x_size, !d.y_size] / [zoom, zoom]
   offset = [0,0]
   p  = offset + image_size /2
   TVCRS, p[0], p[1], /DEVICE
   WINDOW, /PIXMAP, /FREE, xs = !d.x_size, ys=!d.y_size ; Save window
   backing = !d.window
   DEVICE, copy = [0,0, !d.x_size, !d.y_size, 0, 0, win]
   
   s = {EDIT_STATE, $           ; Structure containing state
        base: base, $           ; Main base widget
        pos_w : pos_w, $        ; Position text widget
        msg : msg, $            ; Message window
        win:  win, $            ; draw widget window #
        button: 0, $            ; button state
        drag: [0,0], $          ; Beginning of drag motion
        clear: 1, $             ; 1=last roi has been erased
        image_size : long(image_size), $ ; Image array size
        offset: fix(offset), $  ; offset of array within window
        zoom : zoom, $          ; zoom factor
        img : img, $            ; pointer to image data
        voi : voi, $            ; linked list
        roi : roi, $            ; pointer to current ROI in linked list
        last : ptr_new() $      ; pointer to last ROI entered in linked list
       }
   
   WIDGET_CONTROL, base, /REALIZE, xoffset=0, yoffset=0
   WSHOW, win
   draw_msg = string(format='(A,/,A,/,A)', $
                     "Press left mouse button to trace or select sequential boundary points.", $
                     "Then use Fill to complete or Restore to cancel current outline.", $
                     "Restore will restore all points within the last tracing.")
   erase_msg = string(format='(A,/,A,/,A)', $
                      "Press left mouse button to erase pixels.", $
                      "Erasures are limited to current plane.", "Restore is inactivated!")
   IF (uv.wid.rmode EQ 2) THEN widget_control, s.msg, set_value=erase_msg $
   ELSE widget_control, s.msg, set_value=draw_msg

   CASE orient OF
      0: BEGIN
         x0 = 0
         y0 = 1
         fp = img.fp
         lp = img.lp
      END
      1: BEGIN
         x0 = 0
         y0 = 2
         fp = 1
         lp = img.dim[1]
      END
      2: BEGIN
         x0 = 1
         y0 = 2
         fp = 1
         lp = img.dim[0]
      END
   ENDCASE
   slice = img.p - img.fp
   frame = img.f - img.ff
   xdim = img.dim[x0]
   ydim = img.dim[y0]
   xzoom = float (zoom)
   yzoom = float (zoom)
   IF (img.pdim[y0] GT img.pdim[x0]) THEN BEGIN 
      yzoom = (img.pdim[y0])/(img.pdim[x0]) * zoom
   ENDIF ELSE IF (img.pdim[y0] LT img.pdim[x0]) THEN BEGIN 
      xzoom = (img.pdim[x0])/(img.pdim[y0]) * zoom
   ENDIF
   image0 = reform((*img.data)[*,*,*,frame])
   erase = 0

   WHILE 1 DO BEGIN             ; Internal event loop   
      CASE uv.wid.rplanes OF
         0: BEGIN
            pln1 = img.p
            pln2 = img.p
         ENDCASE
         1: BEGIN
            pln1 = fp
            pln2 = img.p
         ENDCASE
         2: BEGIN
            pln1 = img.p
            pln2 = lp
         ENDCASE
         3: BEGIN
            pln1 = fp 
            pln2 = lp
         ENDCASE
         4: BEGIN
            pln1 = img.p-uv.wid.rslab
            pln2 = img.p
            IF (pln1 LT fp) THEN pln1 = fp
         ENDCASE
         5: BEGIN
            pln1 = img.p
            pln2 = img.p+uv.wid.rslab
            IF (pln2 GT lp) THEN pln2 = lp
         ENDCASE
         6: BEGIN
            pln1 = img.p-uv.wid.rslab
            pln2 = img.p+uv.wid.rslab
            IF (pln1 LT fp) THEN pln1 = fp
            IF (pln2 GT lp) THEN pln2 = lp
         ENDCASE
      ENDCASE
      CASE orient OF
         0: image = reform((*img.data)[*,*,slice,frame])
         1: image = reform((*img.data)[*,slice,*,frame])
         2: image = reform((*img.data)[slice,*,*,frame])
      ENDCASE

      ev = WIDGET_EVENT([base, draw])
      n = (*s.roi).num
      IF ev.id EQ draw THEN BEGIN
         IF (uv.wid.rmode EQ 2) THEN BEGIN
            d0 = -(uv.wid.rerase)/2 ; eraser dimensions
            d1 = d0 + uv.wid.rerase-1
            IF (ev.press EQ 1) THEN erase = 1 $
            ELSE IF (ev.release EQ 1) THEN erase = 0
            IF (erase EQ 1) THEN BEGIN
               xp = (ev.x - s.offset[0]) / xzoom + 1 ; pixel coordinate
               yp = ydim - (ev.y - s.offset[1]) / yzoom
               xmin = 0 > xp+d0
               ymin = 0 > yp+d0
               xmax = xdim-1 < xp+d1
               ymax = ydim-1 < yp+d1
               image[xmin:xmax,ymin:ymax] = uv.wid.editval
               CASE orient OF
                  0: (*img.data)[*,*,slice,frame] = image
                  1: (*img.data)[*,slice,*,frame] = image
                  2: (*img.data)[slice,*,*,frame] = image
               ENDCASE
               VIDI_DISPLAY, uv
            ENDIF
         ENDIF ELSE BEGIN
            VIDI_EDIT_EVENT, ev, s, xdim, ydim, xzoom, yzoom
         ENDELSE
      ENDIF ELSE IF ev.id EQ val THEN uv.wid.editval = ev.value $
      ELSE CASE ev.id of 
         
         fill:  BEGIN   
            widget_control, /hourglass
            n = (*s.roi).num
            IF (n GE 2 AND s.clear EQ 0) THEN BEGIN 
               xy = (*(*s.roi).loc)[*,0:n-1]
               pts = polyfillv(xy[0,*],xy[1,*], xdim, ydim)
               IF ((size(pts))[0] GT 0) THEN BEGIN
                  IF (uv.wid.rmode EQ 1) THEN BEGIN
                     timage = image
                     xval = max(timage)+1
                     if (xval EQ -32768) then xval = 32767
                     timage[pts] = xval
                     pts = where (timage LT xval)
                  ENDIF
                  IF (pln1 EQ pln2) THEN msg = "Fill applied in plane "+strtrim(string(pln1),2) $
                  ELSE msg = "Fill applied in planes "+strtrim(string(pln1),2)+" to "+strtrim(string(pln2),2)
                  FOR pln=pln1-1,pln2-1 DO BEGIN
                     CASE orient OF
                        0: image = reform((*img.data)[*,*,pln,frame])
                        1: image = reform((*img.data)[*,pln,*,frame])
                        2: image = reform((*img.data)[pln,*,*,frame])
                     ENDCASE
                     IF ((size(pts))[0] GT 0) THEN image[pts] = uv.wid.editval
                     CASE orient OF
                        0: (*img.data)[*,*,pln,frame] = image
                        1: (*img.data)[*,pln,*,frame] = image
                        2: (*img.data)[pln,*,*,frame] = image
                     ENDCASE
                  ENDFOR
               ENDIF ELSE msg = "No pixels filled"
               VIDI_DISPLAY, uv
               s.clear = 1
               (*s.roi).num = 0  
            ENDIF ELSE msg = "No defined region to fill"
            widget_control, s.msg, set_value=msg
         ENDCASE
         
         restore:  BEGIN 
            n = (*s.roi).num
            IF (s.clear GT 0 AND n GT 0) THEN VIDI_EDIT_DRAW, s, -1, xdim, ydim, xzoom, yzoom, /fill
            msg = "Nothing to Restore"
            IF ((size(pts))[0] GT 0) THEN BEGIN
               IF (pln1 EQ pln2) THEN msg = "Restored last edit of plane "+strtrim(string(pln1),2) $
               ELSE msg = "Restored last edit of planes "+strtrim(string(pln1),2)+" to "+strtrim(string(pln2),2)
               FOR pln=pln1-1,pln2-1 DO BEGIN
                  CASE orient OF
                     0: image = reform((*img.data)[*,*,pln,frame])
                     1: image = reform((*img.data)[*,pln,*,frame])
                     2: image = reform((*img.data)[pln,*,*,frame])
                  ENDCASE
                  CASE orient OF
                     0: timage = reform(image0[*,*,pln])
                     1: timage = reform(image0[*,pln,*])
                     2: timage = reform(image0[pln,*,*])
                  ENDCASE
                  image[pts] = timage[pts]
                  CASE orient OF
                     0: (*img.data)[*,*,pln,frame] = image
                     1: (*img.data)[*,pln,*,frame] = image
                     2: (*img.data)[pln,*,*,frame] = image
                  ENDCASE
               ENDFOR
            ENDIF
            widget_control, s.msg, set_value=msg
            s.clear = 1
            (*s.roi).num = 0 
            VIDI_DISPLAY, uv
         ENDCASE
         
         up: BEGIN
            IF (img.p GT fp) THEN BEGIN
               img.p = img.p-1
               slice = img.p - img.fp
               uv.img[0].p = img.p
               uv.img[1].p = img.p
               widget_control, uv.wid.slider, set_value=img.p
               VIDI_DISPLAY, uv
            ENDIF 
         ENDCASE

         down: BEGIN
            IF (img.p LT lp) THEN BEGIN
               img.p = img.p+1
               slice = img.p - img.fp
               uv.img[0].p = img.p
               uv.img[1].p = img.p
               widget_control, uv.wid.slider, set_value=img.p
               VIDI_DISPLAY, uv
            ENDIF 
         ENDCASE 

         exit:  BEGIN
            WIDGET_CONTROL, base, /DESTROY
            RETURN
         ENDCASE 
         
         replace:  BEGIN   
            n = (*s.roi).num
            IF (n GE 2 AND s.clear EQ 0) THEN BEGIN 
               xy = (*(*s.roi).loc)[*,0:n-1]
               pts = polyfillv(xy[0,*],xy[1,*], xdim, ydim)
               IF ((size(pts))[0] GT 0) THEN BEGIN 
                  CASE uv.wid.rsign OF
                     0: ix = where (image[pts] GE uv.wid.rthresh)
                     1: ix = where (image[pts] EQ uv.wid.rthresh)
                     2: ix = where (image[pts] LE uv.wid.rthresh)
                  ENDCASE 
                  IF ((size(ix))[0] GT 0) THEN BEGIN 
                     npts = (size(ix))[1]
                     pts = pts[ix]
                     image[pts] = uv.wid.editval
                     msg = "Replaced "+strtrim(string(npts),2)+" pixels"
                  ENDIF ELSE msg = "No points replaced"
               ENDIF 
               CASE orient OF
                  0: (*img.data)[*,*,slice,frame] = image
                  1: (*img.data)[*,slice,*,frame] = image
                  2: (*img.data)[slice,*,*,frame] = image
               ENDCASE
               pln1 = img.p
               pln2 = img.p
               VIDI_DISPLAY, uv
               s.clear = 1
               (*s.roi).num = 0  
            ENDIF ELSE msg = "No defined region to Fill"
            widget_control, s.msg, set_value=msg
         ENDCASE
         
         rsign: uv.wid.rsign = ev.value

         rthresh: uv.wid.rthresh = ev.value

         rplanes: uv.wid.rplanes = ev.value

         rmode: BEGIN
            uv.wid.rmode = ev.value
            IF (uv.wid.rmode EQ 2) THEN widget_control, s.msg, set_value=erase_msg $
            ELSE widget_control, s.msg, set_value=draw_msg
         ENDCASE

         rerase: uv.wid.rerase = ev.value

         rslab: uv.wid.rslab = ev.value
         
         ELSE:
      ENDCASE   
   ENDWHILE
END   
