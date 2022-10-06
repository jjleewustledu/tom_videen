; NAME:
;	VIDI_DEFVOI
;
; PURPOSE:
;	Widget to define, manipulate and compute values of
;	a region of interest within a widget drawable. 
;	Reads and writes VOI files.
; CATEGORY:
;	Regions of interest, graphics.
; CALLING SEQUENCE:
;	Result = VIDI_DEFVOI(uv, inum, orient)
; RESTRICTIONS:
;	This is a MODAL widget.  No other widget applications will be
;	responsive while this widget is in use.
;
; MODIFICATION HISTORY:
;	Tom Videen, Dec 1997.
;	Add Threshold option. TOV, May 1998.
;	Add Limit option. TOV, June 1998.
;-


PRO VIDI_DEFVOI_DRAW, s, i, FILL=fill
; Draw outline (or fill if FILL is set) of ROI
;	or the ith segment of a polygon if i < 0.
; Uses XOR or solid color drawing mode (as specified by s.color).
; Image coordinates are tranposed to screen coordinates before drawing.
; Parameters:
;	s = state structure
;	i = segment number
   
   n = (*s.roi).num
   IF (n LT 1) THEN RETURN
   
   WSET, s.win
   
   xsave = !x.s & ysave = !y.s  ; Set scaling to pixel coords
   xfact = 1.
   yfact = 1.
   IF (s.img.pdim[1] GT s.img.pdim[0]) THEN BEGIN
      yfact = (s.img.pdim[1])/(s.img.pdim[0])
   ENDIF ELSE IF (s.img.pdim[0] GT s.img.pdim[1]) THEN BEGIN
      xfact = (s.img.pdim[0])/(s.img.pdim[1])
   ENDIF

   !x.s = [0, s.zoom/(float(!d.x_size) / xfact)]
   !y.s = [0, s.zoom/(float(!d.y_size) / yfact)]
   ydim = s.img.dim[1]
   
   IF  KEYWORD_SET(FILL) THEN DEVICE, SET_GRAPHICS_FUNCTION=s.color $
   ELSE DEVICE, SET_GRAPHICS_FUNCTION=6
   
   CASE (*s.roi).type OF
      
      0: BEGIN                  ; Circle
         n = 32                 ; #points to define the circle
         a = findgen(n+1) * (2 * !pi/(n-1))
         x = (*(*s.roi).loc)[0]
         y = (*(*s.roi).loc)[1]
         r = (*s.roi).r
         xy = transpose([[x + r * cos(a)], [y + r * sin(a)]])
         xy = VOI_TRANSPOSE(xy, ydim, /DISPLAY)
         plots, xy
        IF KEYWORD_SET(FILL) THEN polyfill, xy
      ENDCASE
      
      1: BEGIN                  ; Rectangle
         x1 = (*(*s.roi).loc)[0,0]
         y1 = (*(*s.roi).loc)[1,0]
         x2 = (*(*s.roi).loc)[0,1]
         y2 = (*(*s.roi).loc)[1,1]
         xy = [[x1,y1],[x1,y2],[x2,y2],[x2,y1],[x1,y1]]
         xy = VOI_TRANSPOSE(xy, ydim, /DISPLAY)
         plots, xy
         IF KEYWORD_SET(FILL) THEN polyfill, xy
      ENDCASE
      
      2: IF (n GE 2) THEN BEGIN ; Trace
         xy = (*(*s.roi).loc)[*,0:n-1]
         xy = VOI_TRANSPOSE(xy, ydim, /DISPLAY)
         IF KEYWORD_SET(FILL) THEN polyfill, xy $
         ELSE BEGIN
            DEVICE, SET_GRAPHICS_FUNCTION=3
            IF (i GE 0) THEN plots, xy[*, i:i+1] $ ; One segment
            ELSE plots, xy      ; All of it
         ENDELSE 
      ENDIF
      
      3: BEGIN                  ; Thresholded ROI = filled polygon
         xy = [(*(*s.roi).loc)[*,0:n-1]]
         xy = VOI_TRANSPOSE(xy, ydim)
         print,(size(xy))[2], ' points'
         FOR k=0,n-1 DO BEGIN
            imgpoint = [[xy[0,k],xy[1,k]], [xy[0,k]-1,xy[1,k]], $
                        [xy[0,k]-1,xy[1,k]+1], [xy[0,k],xy[1,k]+1]]
            polyfill, imgpoint
         ENDFOR 
      ENDCASE
      
   ENDCASE
   
   !x.s = xsave & !y.s = ysave
   DEVICE, SET_GRAPHICS_FUNCTION=3 ; Copy mode   
END   

  
PRO VIDI_DEFVOI_EVENT, ev, s
; Called from the VIDI_DEFVOI event loop.
; Parameters:
;	ev = event structure
;	s = state structure
   
   s.button = s.button OR ev.press XOR ev.release ; New button state   
   n = (*s.roi).num

   xfact = 1.
   yfact = 1.
   IF (s.img.pdim[1] GT s.img.pdim[0]) THEN BEGIN
      yfact = (s.img.pdim[1])/(s.img.pdim[0])
   ENDIF ELSE IF (s.img.pdim[0] GT s.img.pdim[1]) THEN BEGIN
      xfact = (s.img.pdim[0])/(s.img.pdim[1])
   ENDIF
   xzoom = xfact * s.zoom
   yzoom = yfact * s.zoom

   x = (ev.x - s.offset[0]) / xzoom ; Screen coordinates (0,0 is lower left)
   y = (ev.y - s.offset[1]) / yzoom
   xdim = s.img.dim[0]
   ydim = s.img.dim[1]
   xy = [fix(x),fix(y)]
   xy = VOI_TRANSPOSE(xy, ydim) ; Image coordinates (0,0 is upper left)
   WIDGET_CONTROL, s.pos_w, SET_VALUE=string(xy+1, format='("Position: ",i,", ",i)')

   IF (xy[0] LT 0) OR (xy[1] LT 0) OR $ ; Check if cursor is within image
      (xy[0] GE xdim) OR (xy[1] GE ydim) THEN RETURN
   IF ev.press NE 0 THEN s.drag = [x,y] ; Start of drag operation
   
   IF (s.button EQ 1) THEN BEGIN ; Create
      
      CASE (*s.roi).type OF
         
         0: BEGIN               ; Circle
            IF (n GT 0) THEN VIDI_DEFVOI_DRAW, s, -1 ; Erase
            s.saved = 0
            t = s.drag
            (*s.roi).num = 1
            (*s.roi).r = sqrt((float(x)-t[0])^2 + (float(y) - t[1])^2)
            IF NOT ptr_valid((*s.roi).loc) THEN (*s.roi).loc = ptr_new(lonarr(2))
            *(*s.roi).loc = VOI_TRANSPOSE(t, ydim, /DISPLAY)
            VIDI_DEFVOI_DRAW, s, -1
         ENDCASE
         
         1: BEGIN               ; Rectangle
            IF (n GT 0) THEN VIDI_DEFVOI_DRAW, s, -1 ; Erase
            s.saved = 0
            IF NOT ptr_valid((*s.roi).loc) THEN (*s.roi).loc = ptr_new(lonarr(2,2))
            t = s.drag
            (*s.roi).type = 1
            (*s.roi).num = 2
            (*(*s.roi).loc)[*,0] = xy
            (*(*s.roi).loc)[*,1] = VOI_TRANSPOSE(t, ydim, /DISPLAY)
            VIDI_DEFVOI_DRAW, s, -1
         ENDCASE
         
         2: BEGIN               ; Trace
            s.saved = 0
            IF NOT ptr_valid((*s.roi).loc) THEN BEGIN
               (*s.roi).type = 2
               (*s.roi).num = 0
               (*s.roi).loc = ptr_new(lonarr(2,100))
               n = 0
            ENDIF
            IF (n GT 0) THEN $  ; Check for duplicates
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
               IF ((*s.roi).type EQ 2 AND n GE 2) THEN VIDI_DEFVOI_DRAW, s, n-2
            ENDIF ELSE BEGIN    ; make certain trace is 4-connected
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
         ENDCASE
         
         3: BEGIN               ; Thresholded ROI
            IF (s.saved) THEN s.saved = 0 $
            ELSE BEGIN
               IF (n GE 2) THEN VIDI_DEFVOI_DRAW, s, -1 ; Erase roi   
               IF ((*s.roi).loc NE ptr_new()) THEN ptr_free, (*s.roi).loc
            ENDELSE
            s.saved = 0
            
            IF NOT ptr_valid((*s.roi).loc) THEN BEGIN
               (*s.roi).type = 3
               (*s.roi).num = 0
               (*s.roi).loc = ptr_new(lonarr(2,100))
               n = 0
            ENDIF
            pln = long(s.img.p-s.img.fp)
            plane_offset = pln*(ydim*xdim)
            pix = long(plane_offset) + long(xdim)*long(xy[1]) + long(xy[0])
            contrapix = long(plane_offset) + long(xdim)*long(xy[1]) + long(xdim-xy[0]-1)
            seedval = (*s.img.data)[pix]
            contraval = (*s.img.data)[contrapix]
            IF (seedval GE s.bot AND seedval LE s.top) THEN BEGIN 
           		IF (s.contra EQ 0) THEN BEGIN 
               		(*(*s.roi).loc)[*,0] = xy
               		(*s.roi).num = 1
            	ENDIF ELSE IF (seedval GE s.conrat*contraval) THEN BEGIN 
               		(*(*s.roi).loc)[*,0] = xy
               		(*s.roi).num = 1
            	ENDIF ELSE (*s.roi).num = 0
           	ENDIF ELSE (*s.roi).num = 0

            limited = 0
            lim = VOI_LOCATE(s.voi, 'LIMIT')
            IF ptr_valid(lim) THEN BEGIN
               limited = 1
               xlp = (*(*lim).loc)[0,*]
               ylp = (*(*lim).loc)[1,*]
               lim = (*lim).next
               WHILE (lim NE ptr_new()) DO BEGIN
                  IF ((*lim).id EQ 'LIMIT') THEN BEGIN 
                     IF ((*lim).num GT 0) THEN BEGIN 
                        xlp1 = (*(*lim).loc)[0,*]
                        ylp1 = (*(*lim).loc)[1,*]
                        xlp = [xlp, xlp1]
                        ylp = [ylp, ylp1]
                     ENDIF 
                  ENDIF 
                  lim = (*lim).next
               ENDWHILE
            ENDIF 
            
            minx = 0
            maxx = xdim-1
            miny = 0
            maxy = ydim-1
            n = (*s.roi).num    ; total number of points
            k = n-1             ; current point
            IF (k EQ 0) THEN widget_control, /hourglass
            WHILE (k LT n AND n GT 0) DO BEGIN
               IF ((n+11) GE N_ELEMENTS(*(*s.roi).loc)/2) THEN BEGIN ; Extend array
                  msg = string(format='("Voxels: ",i)', n)
                  widget_control, s.msg, set_value=msg
                  progressEvent = widget_event(s.base, /NoWait)
                  IF progressEvent.id EQ s.cancel THEN BEGIN
                     widget_control, s.msg, set_value='Thresholding Canceled!'
                     ptr_free, (*s.roi).loc
                     (*s.roi).num = 0
                     goto, EndThreshold
                  ENDIF
                  new = ptr_new(lonarr(2,n+100))
                  (*new)[*,0:n-1] = (*(*s.roi).loc)[*,0:n-1]
                  ptr_free, (*s.roi).loc
                  (*s.roi).loc = new
               ENDIF
               xyp = (*(*s.roi).loc)[*,k] ; current point
               xmin = xyp[0]-1 > minx
               xmax = xyp[0]+1 < maxx
               ymin = xyp[1]-1 > miny
               ymax = xyp[1]+1 < maxy
               FOR xt=xmin,xmax DO BEGIN ; examine 8-connected points to current point, k
                  FOR yt=ymin,ymax DO BEGIN
                     xlocs = where(xt EQ (*(*s.roi).loc)[0,0:n]) ; compare with all defined points
                     ylocs = where (yt EQ (*(*s.roi).loc)[1,0:n])
                     IF (max(setintersection(xlocs,ylocs)) EQ -1) THEN BEGIN ; if it is a new point
                        excluded = 0
                        IF (limited) THEN BEGIN
                           xlocs = where(xt EQ xlp)
                           ylocs = where (yt EQ ylp)
                           IF (max(setintersection(xlocs,ylocs)) ne -1) THEN excluded = 1
                        ENDIF
                        IF (NOT excluded) THEN BEGIN ; and this point is not excluded
                           pix = plane_offset + xdim*yt + xt
                           val = (*s.img.data)[pix]
                           IF (val GE s.bot AND val LE s.top) THEN BEGIN
            					IF (s.contra EQ 0) THEN BEGIN 
                              		(*(*s.roi).loc)[*,n] = [xt,yt]
                              		n = n + 1
                              		(*s.roi).num = n
            					ENDIF ELSE BEGIN 
                           			contrapix = plane_offset + xdim*yt + (xdim-xt-1)
                           			contraval = (*s.img.data)[contrapix]
            						IF (val GE s.conrat*contraval) THEN BEGIN 
                              			(*(*s.roi).loc)[*,n] = [xt,yt]
                              			n = n + 1
                              			(*s.roi).num = n
           							ENDIF
            					ENDELSE
           					ENDIF
                        ENDIF 
                     ENDIF 
                  ENDFOR
               ENDFOR
               k = k+1
            ENDWHILE
            k = 0
            n = (*s.roi).num
           	IF (s.contra EQ 0) THEN BEGIN 
            	msg = string(format='("Voxels: ",i,/,"Seed: [",I3,",",I3,",",I3,"] =",F10.2)', n, xy+1,pln+1,seedval)
			ENDIF ELSE BEGIN
                cx = xdim-xy[0]-1
            	msg = string(format=$
   				'("Voxels: ",i,/,"Seed: [",I3,",",I3,",",I3,"] =",F10.2,/,"Contra: [",I3,",",I3,",",I3,"]=",F10.2)',$
					n, xy+1,pln+1,seedval,cx,xy[1]+1,pln+1,contraval)
			ENDELSE
            widget_control, s.msg, set_value=msg
            VIDI_DEFVOI_DRAW, s, n-1
			EndThreshold:
         ENDCASE
         
      ENDCASE
      
   ENDIF ELSE IF (s.button EQ 2 AND (NOT s.saved OR ptr_valid(s.last))) THEN BEGIN ; Move
      
      CASE (*s.roi).type OF
         
         0: BEGIN               ; Circle
            IF (s.saved) THEN BEGIN
               s.saved = 0
               (*s.roi).id = s.id
               (*s.roi).r = (*s.last).r
               (*s.roi).num = 1 
               (*s.roi).loc = ptr_new(lonarr(2))
            ENDIF ELSE VIDI_DEFVOI_DRAW, s, -1 ; Remove old
            *(*s.roi).loc = xy
            VIDI_DEFVOI_DRAW, s, -1
         ENDCASE 
         
         1: BEGIN               ; Rectangle
            IF (s.saved) THEN BEGIN
               s.saved = 0
               (*s.roi).id = s.id
               (*s.roi).num = 2
               (*s.roi).loc = ptr_new(lonarr(2,2))
               xy0 = (*(*s.last).loc)[*,1] - (*(*s.last).loc)[*,0]
            ENDIF ELSE BEGIN 
               VIDI_DEFVOI_DRAW, s, -1 ; Remove old
               xy0 = (*(*s.roi).loc)[*,1] - (*(*s.roi).loc)[*,0]
            ENDELSE 
            (*(*s.roi).loc)[*,0] = xy
            (*(*s.roi).loc)[*,1] = xy + xy0
            VIDI_DEFVOI_DRAW, s, -1
         ENDCASE 

         2: BEGIN               ; Trace
            IF (s.saved) THEN BEGIN
               s.saved = 0
               (*s.roi).id = s.id
               n = (*s.last).num
               (*s.roi).num = n
               pdim = 100 > n
               (*s.roi).loc = ptr_new(lonarr(2,pdim))
               FOR i=0,n-1 DO (*(*s.roi).loc)[*,i] = (*(*s.last).loc)[*,i]
            ENDIF ELSE VIDI_DEFVOI_DRAW, s, -1 ; Remove old
            xy0 = xy - (*(*s.roi).loc)[*,0]
            (*(*s.roi).loc)[*,0] = xy
            FOR i=1,n DO (*(*s.roi).loc)[*,i] = (*(*s.roi).loc)[*,i] + xy0
            VIDI_DEFVOI_DRAW, s, -1
         ENDCASE 
         
      ENDCASE
   ENDIF 
   RETURN
END   
   

PRO VIDI_DEFVOI, uv, inum, orient, zoom
   COMMON VIDI
   
   voi = uv.wid.voi
   color = uv.wid.voicolor
   img = uv.img[inum]
   cbf = uv.cbf[inum]
   cbv = uv.cbv[inum]
   draw = uv.wid.win[inum]
   group = uv.wid.base
   proc = img.proc

   bot = uv.wid.vthresh[0]
   top = uv.wid.vthresh[1]
;XXX
   bot = 300
   top = 2000
   imax = max(*img.data, min=imin)
   minlim = long(imin-0.9)
   maxlim = long(imax+0.9)
   IF (maxlim EQ minlim) THEN BEGIN
      minlim = 0
      maxlim = 1
   ENDIF
   bot = bot > minlim
   bot = bot < maxlim
   top = top > minlim
   top = top < maxlim
   conrat = 1.5
   contra = 0

   base = WIDGET_BASE(title='Region of Interest', group_leader=group, /ROW)   
   base1 = WIDGET_BASE(base,/COLUMN)   
   base1a = WIDGET_BASE(base1, /ROW)
   temp1 = CW_BGROUP(base1a, /ROW, /NO_RELEASE, /RETURN_NAME,  $
                     ['Read', 'Write', 'Refresh', 'Exit'])
   
   IF (color EQ 3) THEN cval = 1 ELSE cval = 0
   coltype = CW_BGROUP(base1a, /ROW, /EXCLUSIVE, /NO_RELEASE, /RETURN_NAME, $
                       ['XOR','White'], SET_VALUE=cval)
   
   base2 =  WIDGET_BASE(base1,/ROW)
   select3 = ['1\Draw','0\Only if Defined on this Slice','0\Regardless']
   temp3 = cw_pdmenu(base2, select3, /RETURN_NAME)

   
   select4 = ['1\Statistics', $
              '0\Using Planes in which the ROI Components were Defined', $
              '0\ALL Unique ROI Components using the Current Plane', $
              '0\ONLY ROI Components which were Defined on Current Plane']
   temp4 = CW_PDMENU(base2, select4, /RETURN_NAME)
   
   select5 = ['1\Plot','0\Across Planes','0\Across Frames']
   temp5 = CW_PDMENU(base2, select5, /RETURN_NAME)
   
   select6 = ['1\Delete','0\Last Component Entered on This Slice', $
              '0\All Components on This Slice','0\All Components on All Slices']
   temp6 = CW_PDMENU(base2, select6, /RETURN_NAME)

   select7 = ['1\Create','0\Contralateral ROI on This Slice', $
              '0\Contralateral ROI on All Slices']
   temp7 = CW_PDMENU(base2, select7, /RETURN_NAME)
   
   base3 = WIDGET_BASE(base1,/ROW)
   temp6 = CW_BGROUP(base3, /ROW, /NO_RELEASE, /RETURN_NAME, ['Save'])
   name = CW_FIELD(base3, title='as', value='', xsize=36, /RETURN_EVENTS)

   dtype = CW_BGROUP(base1, /ROW, /EXCLUSIVE, /NO_REL, /RETURN_NAME, $
                     ['Circle', 'Rectangle', 'Trace', 'Threshold','Limit'], SET_VALUE=0)
   
   pos_w = WIDGET_TEXT(base1, YSIZE=1, XSIZE=18, /FRAME, $
                       VALUE='Position:    0,    0')
   
   base4 = WIDGET_BASE(base1, /ROW)
   plane = CW_FIELD(base4, title='Plane', /NOEDIT, value=img.p, xsize=3, ysize=1)
   up = WIDGET_BUTTON(base4, value='<- Up  ')
   down = WIDGET_BUTTON(base4, value=' Down ->')  
   cancel = WIDGET_BUTTON(base4, value=' Cancel Thresholding ')
   base9 = WIDGET_BASE(base1, /ROW)
   ratio = CW_FIELD(base9, title='Contra Thresh', value=conrat, xsize=8, /FLOAT, /RETURN_EVENTS)
   rtype = CW_BGROUP(base9, /ROW, /EXCLUSIVE, /NO_REL, /RETURN_NAME, $
                     ['Ignore', 'Use'], SET_VALUE=0)

   base5 = WIDGET_BASE(base,/COLUMN)
   msg = WIDGET_TEXT(base5, YSIZE=3)
   
   list = WIDGET_LIST(base5, XSIZE=45, YSIZE=7)
   
   base6 = WIDGET_BASE(base5, /COLUMN)
   base7 = WIDGET_BASE(base6,/ROW)
   loval = CW_FIELD(base7, title='Lower Thresh', value=bot, $
                    xsize=8, /INTEGER, /RETURN_EVENTS)
   lolimit = WIDGET_SLIDER(base7, value=bot, min=minlim, max=maxlim, xsize=440)

   base8 = WIDGET_BASE(base6,/ROW)
   hival = CW_FIELD(base8, title='Upper Thresh', value=top, $
                    xsize=8, /INTEGER, /RETURN_EVENTS)
   hilimit = WIDGET_SLIDER(base8, value=top, min=minlim, max=maxlim, xsize=440)
   
   WIDGET_CONTROL, draw, GET_VALUE=win
   WSET, win
   
   IF N_ELEMENTS(image_size) LE 0 THEN image_size = [!d.x_size, !d.y_size] / [zoom, zoom]
   IF N_ELEMENTS(offset) LE 0 THEN offset = [0,0]   
   p  = offset + image_size /2   
   TVCRS, p[0], p[1], /DEVICE
   WINDOW, /PIXMAP, /FREE, xs = !d.x_size, ys=!d.y_size ; Save window
   backing = !d.window
   DEVICE, copy = [0,0, !d.x_size, !d.y_size, 0, 0, win]
   
   roi = VOI_APPEND(voi)        ; position pointer to free element at end of list
   
   s = {ROI_STATE, $            ; Structure containing state
        base: base, $           ; Main base widget
        name: name, $           ; ROI name field
        pos_w : pos_w, $        ; Position text widget
        msg : msg, $            ; Message window
        win:  win, $            ; draw widget window #
        button: 0, $            ; button state
        cancel: cancel, $       ; cancel button
        drag: [0,0], $          ; Beginning of drag motion
        saved: 1, $             ; 1=roi saved
        image_size : long(image_size), $ ; Image array size
        offset: fix(offset), $  ; offset of array within window
        zoom : zoom, $          ; zoom factor
        color : color, $        ; 3 -> color=white; 6 -> color=XOR
        contra : contra, $      ; use contraleral ratio threshold for ROIs
        conrat : conrat, $      ; contraleral ratio threshold for ROIs
        bot : bot, $            ; low threshold range for ROIs
        top : top, $            ; high threshold range for ROIs
        imin : imin, $          ; image minimum
        imax : imax, $          ; image maximum
        img : img, $            ; pointer to image data
        id  : '', $             ; current VOI id
        voi : voi, $            ; linked list
        roi : roi, $            ; pointer to current ROI in linked list
        last : ptr_new() $      ; pointer to last ROI entered in linked list
       }
   
   WIDGET_CONTROL, base, /REALIZE, xoffset=0, yoffset=0
   WSHOW, win
   voilist = VOI_LIST(voi)
   listing = VOI_LISTING(voi,voilist,img.dim[2])
   widget_control,list, set_value=listing
   widget_control,dtype, set_value=(*s.roi).type

   newslice = 1
   p = s.img.p
   imgname = img.fname
   loc = RSTRPOS (imgname, '/')
   IF (loc GT 0) THEN imgname = STRMID(imgname,loc+1)
   
   WHILE 1 DO BEGIN             ; Internal event loop   
      ev = WIDGET_EVENT([base, draw])
      n = (*s.roi).num
      CASE ev.id OF
         
         draw: VIDI_DEFVOI_EVENT, ev, s
         
         s.name: s.id = ev.value
         
         ratio:    BEGIN
            conrat = ev.value
            s.conrat = conrat
            WIDGET_CONTROL, ratio, SET_VALUE=conrat
         ENDCASE

         loval:    BEGIN
            bot = ev.value
            top = bot > top
            s.bot = bot
            s.top = top
            WIDGET_CONTROL, loval, SET_VALUE=bot
            WIDGET_CONTROL, hival, SET_VALUE=top
            WIDGET_CONTROL, lolimit, SET_VALUE=bot
            WIDGET_CONTROL, hilimit, SET_VALUE=top
         ENDCASE

         hival:    BEGIN
            top = ev.value
            bot = bot < top
            s.bot = bot
            s.top = top
            WIDGET_CONTROL, loval, SET_VALUE=bot
            WIDGET_CONTROL, hival, SET_VALUE=top
            WIDGET_CONTROL, lolimit, SET_VALUE=bot
            WIDGET_CONTROL, hilimit, SET_VALUE=top
         ENDCASE

         lolimit: BEGIN
            bot = ev.value
            top = bot > top
            s.bot = bot
            s.top = top
            WIDGET_CONTROL, loval, SET_VALUE=bot
            WIDGET_CONTROL, hival, SET_VALUE=top
            WIDGET_CONTROL, lolimit, SET_VALUE=bot
            WIDGET_CONTROL, hilimit, SET_VALUE=top
         ENDCASE 
         
         hilimit: BEGIN
            top = ev.value
            bot = bot < top
            s.bot = bot
            s.top = top
            WIDGET_CONTROL, loval, SET_VALUE=bot
            WIDGET_CONTROL, hival, SET_VALUE=top
            WIDGET_CONTROL, lolimit, SET_VALUE=bot
            WIDGET_CONTROL, hilimit, SET_VALUE=top
         ENDCASE 
         
         list: BEGIN
            s.id = voilist[ev.index]
            widget_control,name,set_value=s.id
            IF (s.saved AND ptr_valid(s.last) AND n NE 0) THEN $
               VIDI_DEFVOI_DRAW, s, -1 ; Erase roi
            s.last = VOI_LOCATE(voi, s.id)
            s.saved = 1
            (*s.roi).type = (*s.last).type
            IF (s.id EQ 'LIMIT') THEN widget_control,dtype, set_value=4
         ENDCASE 
         
         up: BEGIN
            p = s.img.p
            IF (p GT img.fp) THEN BEGIN
               p = p-1
               s.img.p = p
               s.img.p = p
               uv.img[0].p = p
               uv.img[1].p = p
               widget_control, uv.wid.slider, set_value=p
               widget_control, plane, set_value=p
               VIDI_DISPLAY, uv
            ENDIF 
         ENDCASE 
         
         down: BEGIN
            p = s.img.p
            IF (p LT img.lp) THEN BEGIN
               p = p+1
               s.img.p = p
               s.img.p = p
               uv.img[0].p = p
               uv.img[1].p = p
               widget_control, uv.wid.slider, set_value=p
               widget_control, plane, set_value=p
               VIDI_DISPLAY, uv
            ENDIF 
         ENDCASE 

         cancel: BEGIN
         ENDCASE 
         
         ELSE: CASE ev.value OF
            
            'XOR' : s.color = 6
            
            'White' : s.color = 3
            
            'Refresh': BEGIN 
               IF (s.saved) THEN s.saved = 0
               IF ((*s.roi).loc NE ptr_new()) THEN ptr_free, (*s.roi).loc
               (*s.roi).num = 0   
               VIDI_DISPLAY,uv
            ENDCASE 

            'Ignore' :  s.contra = 0

            'Use' :  s.contra = 1
            
            'Circle' :  BEGIN   
               IF (s.saved) THEN s.saved = 0 $
               ELSE BEGIN 
                  IF (n NE 0) THEN BEGIN
                     IF ((*s.roi).type EQ 2) THEN VIDI_DISPLAY,uv $
                     ELSE  VIDI_DEFVOI_DRAW, s, -1 ; Erase roi   
                     IF ((*s.roi).loc NE ptr_new()) THEN ptr_free, (*s.roi).loc
                  ENDIF 
               ENDELSE
               (*s.roi).num = 0   
               (*s.roi).type = 0
               IF (s.id EQ 'LIMIT') THEN BEGIN
                  s.id = ''
                  widget_control,name,set_value=s.id
               ENDIF 
               msg = string(format='(A,/,A)', "Depress and drag Left Button to define the circle.", $
                            "Depress the Middle Button to move the last or current VOI.")
               widget_control, s.msg, set_value=msg
            ENDCASE
            
            'Rectangle' :   BEGIN   
               IF (s.saved) THEN s.saved = 0 $
               ELSE BEGIN
                  IF (n NE 0) THEN BEGIN
                     IF ((*s.roi).type EQ 2) THEN VIDI_DISPLAY,uv $
                     ELSE  VIDI_DEFVOI_DRAW, s, -1 ; Erase roi   
                     IF ((*s.roi).loc NE ptr_new()) THEN ptr_free, (*s.roi).loc
                  ENDIF 
               ENDELSE
               (*s.roi).num = 0   
               (*s.roi).type = 1
               IF (s.id EQ 'LIMIT') THEN BEGIN
                  s.id = ''
                  widget_control,name,set_value=s.id
               ENDIF 
               msg = string(format='(A,/,A)', "Depress and drag Left Button to define the rectangle.", $
                            "Depress the Middle Button to move the last or current VOI.")
               widget_control, s.msg, set_value=msg
            ENDCASE
            
            'Trace':  BEGIN   
               IF (s.saved) THEN s.saved = 0 $
               ELSE BEGIN
                  IF (n NE 0) THEN BEGIN
                     IF ((*s.roi).type EQ 2) THEN VIDI_DISPLAY,uv $
                     ELSE  VIDI_DEFVOI_DRAW, s, -1 ; Erase roi   
                     IF ((*s.roi).loc NE ptr_new()) THEN ptr_free, (*s.roi).loc
                  ENDIF 
               ENDELSE
               (*s.roi).num = 0   
               (*s.roi).type = 2
               IF (s.id EQ 'LIMIT') THEN BEGIN
                  s.id = ''
                  widget_control,name,set_value=s.id
               ENDIF 
               msg = string(format='(A,A,/,A)', "Depress Left Button and to draw the boundary.  ", $
                            "Release the button to complete.", $
                            "Depress the Middle Button to move the last or current VOI.")
               widget_control, s.msg, set_value=msg
            ENDCASE
            
            'Threshold':  BEGIN   
               IF (s.saved) THEN s.saved = 0 $
               ELSE BEGIN
                  IF (n NE 0) THEN BEGIN
                     IF ((*s.roi).type EQ 2) THEN VIDI_DISPLAY,uv $
                     ELSE  VIDI_DEFVOI_DRAW, s, -1 ; Erase roi   
                     IF ((*s.roi).loc NE ptr_new()) THEN ptr_free, (*s.roi).loc
                  ENDIF 
               ENDELSE
               (*s.roi).num = 0   
               (*s.roi).type = 3
               IF (s.id EQ 'LIMIT') THEN BEGIN
                  s.id = ''
                  widget_control,name,set_value=s.id
               ENDIF 
               msg = string(format='(A,/,A,/,A)', "Adjust Lower and Upper Thresholds.", $
                            "Then depress Left Button to select the seed point.", $
                            "(Set both thresholds to maximum to see the pixel of a seed point.)")
               widget_control, s.msg, set_value=msg
            ENDCASE

   			'Contralateral ROI on This Slice': BEGIN
            	s.saved = 0
               	orig_id = s.id
               	orig = VOI_LOCATE(s.voi, orig_id)
   				xdim = s.img.dim[0]
                check = 0
               	IF ptr_valid(orig) THEN BEGIN 
               		WHILE (orig NE ptr_new()) DO BEGIN
                  		IF ((*orig).id EQ s.id) THEN BEGIN
            				n = (*orig).num  
print,n,(*orig).z,s.img.p
                     		IF (((*orig).z EQ s.img.p) AND (n GT 0)) THEN BEGIN 
								check = 1
                  				(*voi).num = (*voi).num + 1
               					IF ((*s.roi).loc NE ptr_new()) THEN ptr_free, (*s.roi).loc
               					(*s.roi).loc = ptr_new(lonarr(2,n))
               					(*s.roi).id = 'contra_'+orig_id
            					(*s.roi).type = (*orig).type
                  				(*s.roi).orient = (*orig).orient
                  				(*s.roi).z = (*orig).z
               					(*s.roi).r = (*orig).r
                  				(*s.roi).num = (*orig).num
            					k = 0
           						WHILE (k LT n) DO BEGIN
              						xy = (*(*orig).loc)[*,k]
               						(*(*s.roi).loc)[*,k] = [xdim-xy[0]-1, xy[1]]
               						k = k+1
            					ENDWHILE
                     			VIDI_DEFVOI_DRAW, s, -1
                     			VIDI_DEFVOI_DRAW, s, -1, /fill
                  				s.last = VOI_SORT(s.voi, s.roi)
                  				s.roi = VOI_APPEND(s.voi)
                  				(*s.roi).type = (*s.last).type
                  				s.saved = 1
                  				IF (min('' EQ voilist) EQ 1) THEN voilist = [s.id] $
                  				ELSE IF (max(s.id EQ voilist) EQ 0) THEN voilist = [voilist,[s.id]]
                  				voilist = VOI_LIST(voi)
                  				listing = VOI_LISTING(voi,voilist,img.dim[2])
                  				widget_control,list, set_value=listing
                     		ENDIF 
                  		ENDIF
                  		orig = (*orig).next
               		ENDWHILE
                	IF (check) THEN widget_control, s.msg, set_value='ROI saved:   '+s.id $
                	ELSE widget_control, s.msg, set_value='ROI not defined on this plane'
               	ENDIF
                s.roi = VOI_EOL(voi)
            ENDCASE 
   			
            'Contralateral ROI on All Slices': BEGIN
            	s.saved = 0
               	orig_id = s.id
               	orig = VOI_LOCATE(s.voi, orig_id)
               	IF ptr_valid(orig) THEN BEGIN 
   					xdim = s.img.dim[0]
            		n = (*orig).num  
                  	IF (n GT 0) THEN BEGIN
                  		(*voi).num = (*voi).num + 1
               			IF ((*s.roi).loc NE ptr_new()) THEN ptr_free, (*s.roi).loc
               			(*s.roi).loc = ptr_new(lonarr(2,n))
               			(*s.roi).id = 'contra_'+orig_id
            			(*s.roi).type = (*orig).type
                  		(*s.roi).orient = (*orig).orient
                  		(*s.roi).z = (*orig).z
               			(*s.roi).r = (*orig).r
                  		(*s.roi).num = (*orig).num
            			k = 0
           				WHILE (k LT n) DO BEGIN
              				xy = (*(*orig).loc)[*,k]
               				(*(*s.roi).loc)[*,k] = [xdim-xy[0]-1, xy[1]]
               				k = k+1
            			ENDWHILE
                     	VIDI_DEFVOI_DRAW, s, -1
                     	VIDI_DEFVOI_DRAW, s, -1, /fill
                  		s.last = VOI_SORT(s.voi, s.roi)
                  		s.roi = VOI_APPEND(s.voi)
                  		(*s.roi).type = (*s.last).type
                  		s.saved = 1
                  		widget_control, s.msg, set_value='ROI saved:   '+s.id
                  		IF (min('' EQ voilist) EQ 1) THEN voilist = [s.id] $
                  		ELSE IF (max(s.id EQ voilist) EQ 0) THEN voilist = [voilist,[s.id]]
                  		voilist = VOI_LIST(voi)
                  		listing = VOI_LISTING(voi,voilist,img.dim[2])
                  		widget_control,list, set_value=listing
               		ENDIF
               	ENDIF
            ENDCASE 
            
            'Limit':  BEGIN   
               IF (s.saved) THEN s.saved = 0 $
               ELSE BEGIN
                  IF (n NE 0) THEN BEGIN
                     IF ((*s.roi).type EQ 2) THEN VIDI_DISPLAY,uv $
                     ELSE  VIDI_DEFVOI_DRAW, s, -1 ; Erase roi   
                     IF ((*s.roi).loc NE ptr_new()) THEN ptr_free, (*s.roi).loc
                  ENDIF 
               ENDELSE
               (*s.roi).num = 0   
               (*s.roi).type = 2
               s.id = 'LIMIT'
               widget_control,name,set_value=s.id
               msg = string(format='(A,/,A,/,A)',"Trace boundary limit while depressing Left Button.", $
                            "Then Save with the name LIMIT.", $
                            "Multiple saved limit segments are permissible and apply to ALL planes.")
               widget_control, s.msg, set_value=msg
            ENDCASE
            
            'Read': BEGIN
               fname = dialog_pickfile(FILE=img.fname, FILTER='*.voi', /read)
               IF (strlen(fname) EQ 0) THEN BEGIN
                  widget_control, s.msg, set_value='No file entered'
               ENDIF ELSE IF (VOI_READ(voi, fname, msg) NE 0) THEN BEGIN
                  widget_control, s.msg, set_value=msg 
               ENDIF ELSE BEGIN
                  voilist = VOI_LIST(voi)
                  listing = VOI_LISTING(voi,voilist,img.dim[2])
                  IF (listing[0] NE '') THEN BEGIN
                     widget_control,list, set_value=listing
                     s.roi = VOI_APPEND(s.voi)
                  ENDIF ELSE BEGIN
                     widget_control, s.msg, set_value= $
                        'No data.  VOI file is incorrect format or incompatible with current image.'
                     roi = voi
                     WHILE ptr_valid((*roi).next) DO BEGIN
                        next = (*roi).next
                        last = (*next).next
                        ptr_free, (*next).loc
                        ptr_free, next
                        (*roi).next = last
                     ENDWHILE
                     s.roi = voi
                  ENDELSE 
               ENDELSE 
               s.saved = 0
               (*s.roi).num = 0   
               (*s.roi).type = 0
               widget_control,dtype, set_value=(*s.roi).type
            ENDCASE
            
            'Write': BEGIN
               fname = dialog_pickfile(FILE=img.fname, FILTER='*.voi', /WRITE)
               IF (strlen(fname) EQ 0) THEN BEGIN
                  widget_control, s.msg, set_value='No file entered'
               ENDIF ELSE BEGIN
                  (*voi).type = 9802 ; version 2.0, 1998
                  IF (VOI_WRITE(voi, fname, msg) NE 0) THEN $
                     widget_control, s.msg, set_value=msg
               ENDELSE 
               widget_control,dtype, set_value=(*s.roi).type
            ENDCASE 
            
            'Only if Defined on this Slice': IF ((*voi).num GT 0) THEN BEGIN
               s.roi = VOI_LOCATE(voi, s.id)
               check = 0
               WHILE (s.roi NE ptr_new()) DO BEGIN
                  IF ((*s.roi).id EQ s.id) THEN BEGIN
                     IF (((*s.roi).z EQ s.img.p) AND ((*s.roi).num GT 0)) THEN BEGIN 
                        IF ((*s.roi).num GT 0) THEN BEGIN 
                           IF (s.id NE 'LIMIT') THEN VIDI_DEFVOI_DRAW, s, -1, /fill $
                           ELSE VIDI_DEFVOI_DRAW, s, -1
                        ENDIF
                     ENDIF ELSE check = 1
                  ENDIF
                  s.roi = (*s.roi).next
               ENDWHILE
               IF (check) THEN BEGIN
                  msg = string(format='(A,/,A)', "Only components on this slice are shown.", $
                               "Components exist for other slices.")
                  widget_control, s.msg, set_value=msg
               ENDIF
               s.roi = VOI_EOL(voi)
               widget_control,dtype, set_value=(*s.roi).type
            ENDIF 
            
            'Regardless': IF ((*voi).num GT 0) THEN BEGIN
               s.roi = VOI_LOCATE(voi, s.id)
               WHILE (s.roi NE ptr_new()) DO BEGIN
                  IF ((*s.roi).id EQ s.id) THEN BEGIN
                     IF ((*s.roi).num GT 0) THEN BEGIN 
                        IF (s.id NE 'LIMIT') THEN VIDI_DEFVOI_DRAW, s, -1, /fill $
                        ELSE VIDI_DEFVOI_DRAW, s, -1
                     ENDIF
                  ENDIF 
                  s.roi = (*s.roi).next
               ENDWHILE
               s.roi = VOI_EOL(voi)
               widget_control,dtype, set_value=(*s.roi).type
            ENDIF 
            
            ;; Statistics
            
            'Using Planes in which the ROI Components were Defined': BEGIN 
               s.roi = VOI_LOCATE(voi, s.id)
               msg = 'Pixels = 0' ; default if no data
               log = ''
               IF ptr_valid(s.roi) THEN BEGIN 
                  IF ((*s.roi).num GT 0) THEN BEGIN
                     pixels = VOI_COORDS(voi, s.id, img.dim, msg, OFFSET=img.fp)
                     IF ((size(pixels))[3] GT 0) THEN BEGIN
                        pixels = pixels[sort(pixels)]
                        pixels = pixels[uniq(pixels)]
                        msg = VIDI_PROCESS(pixels, img, cbf, cbv, log)
                     ENDIF
                  ENDIF
               ENDIF
               widget_control, s.msg, set_value=msg
               s.roi = VOI_EOL(voi)
               widget_control,dtype, set_value=(*s.roi).type
               IF (strlen(log) GT 0 AND loglun NE 0) THEN BEGIN
                  printf, loglun, imgname,s.id,log, format='(A," ",A," --- ",A)'
               ENDIF
            ENDCASE

            'ALL Unique ROI Components using the Current Plane': BEGIN 
               s.roi = VOI_LOCATE(voi, s.id)
               msg = 'Pixels = 0' ; default if no data
               log = ''
               IF ptr_valid(s.roi) THEN BEGIN 
                  IF ((*s.roi).num GT 0) THEN BEGIN
                     pixels = VOI_COORDS(voi, s.id, img.dim, msg, SLICE=s.img.p, OFFSET=img.fp)
                     IF ((size(pixels))[2] GT 1) THEN BEGIN
                        pixels = pixels[sort(pixels)]
                        pixels = pixels[uniq(pixels)]
                        msg = VIDI_PROCESS(pixels, img, cbf, cbv, log)
                     ENDIF
                  ENDIF
               ENDIF
               widget_control, s.msg, set_value=msg
               s.roi = VOI_EOL(voi)
               widget_control,dtype, set_value=(*s.roi).type
               IF (strlen(log) GT 0 AND loglun NE 0) THEN BEGIN
                  printf, loglun, imgname,s.id,s.img.p,log, format='(A," ",A," ",I3," ",A)'
               ENDIF
            ENDCASE
            
            'ONLY ROI Components which were Defined on Current Plane': BEGIN 
               s.roi = VOI_LOCATE(voi, s.id)
               msg = 'Pixels = 0' ; default if no data
               log = ''
               IF ptr_valid(s.roi) THEN BEGIN 
                  IF ((*s.roi).num GT 0) THEN BEGIN
                     pixels = VOI_COORDS(voi, s.id, img.dim, msg, SLICE=s.img.p, OFFSET=img.fp, /LIMIT)
                     IF ((size(pixels))[2] GT 1) THEN BEGIN
                        pixels = pixels[sort(pixels)]
                        pixels = pixels[uniq(pixels)]
                        msg = VIDI_PROCESS(pixels, img, cbf, cbv, log)
                     ENDIF
                  ENDIF
               ENDIF
               widget_control, s.msg, set_value=msg
               s.roi = VOI_EOL(voi)
               widget_control,dtype, set_value=(*s.roi).type
               IF (strlen(log) GT 0 AND loglun NE 0) THEN BEGIN
                  printf, loglun, imgname,s.id,s.img.p,log, format='(A," ",A," ",I3," ",A)'
               ENDIF
            ENDCASE
            
            'Across Planes': BEGIN
               s.roi = VOI_LOCATE(voi, s.id)
               msg = 'Pixels = 0' ; default if no data
               IF ptr_valid(s.roi) THEN BEGIN 
                  IF ((*s.roi).num GT 0) THEN BEGIN
                     pixels = VOI_COORDS(voi, s.id, img.dim, msg, SLICE=1)
                     IF ((size(pixels))[2] GT 1) THEN BEGIN
                        msg = ''
                        pixels = pixels[sort(pixels)]
                        pixels = pixels[uniq(pixels)]
                        n = n_elements(pixels)
                        IF (n GT 0) THEN BEGIN
                           slice = fltarr(img.lp-img.fp+1)
                           mean = fltarr(img.lp-img.fp+1)
                           FOR k=img.fp, img.lp DO BEGIN 
                              pix = pixels + long(k-img.fp)*img.dim[0]*img.dim[1]
                              val = (*img.data)[pix]
                              slice[k-img.fp] = k
                              IF (img.system EQ 220) THEN $
                                 mean[k-img.fp] = (moment(val))[0] $
                              ELSE IF (img.len[img.f] GT 0) THEN $
                                 mean[k-img.fp] = (moment(val))[0]/(img.len[img.f]/1000.) $
                              ELSE mean[k-img.fp] = (moment(val))[0]
                              print,slice[k-img.fp], mean[k-img.fp]
                           ENDFOR 
                           WINDOW, /free
                           ymin = min(mean)
                           ymax = max(mean)
                           xlabel = 'Slice'
                           PLOT, slice, mean, charsize=1.4,charthick=2,xthick=2,ythick=2, $
                              xtitle=xlabel, ytitle='Counts/Sec', yrange=[ymin,ymax]
                           WSET, s.win
                        ENDIF
                     ENDIF
                  ENDIF
               ENDIF
               widget_control, s.msg, set_value=msg
               s.roi = VOI_EOL(voi)
               widget_control,dtype, set_value=(*s.roi).type
            ENDCASE
            
            'Across Frames': BEGIN
               msg = 'Use the program "vstat" for this processing'
               widget_control, s.msg, set_value=msg

;               s.roi = VOI_LOCATE(voi, s.id)
;               msg = 'Pixels = 0' ; default if no data
;               IF ptr_valid(s.roi) THEN BEGIN 
;                  IF ((*s.roi).num GT 0) THEN BEGIN
;                     pixels = VOI_COORDS(voi, s.id, img.dim, msg, SLICE=1)
;                     print,'Pixels = ',n_elements(pixels)
;                     IF ((size(pixels))[2] GT 1) THEN BEGIN
;                        msg = ''
;                        pixels = pixels[sort(pixels)]
;                        pixels = pixels[uniq(pixels)]
;                        n = n_elements(pixels)
;                        print,'Pixels = ',n_elements(pixels)
;                        print,img.dim
;                        frmsize =  long(img.dim[0])*long(img.dim[1])*long(img.dim[2])
;                        print,'Framesize = ',frmsize
;                        IF (n GT 0) THEN BEGIN
;                           frame = fltarr(img.lf-img.ff+1)
;                           mean = fltarr(img.lf-img.ff+1)
;                           FOR k=img.ff, img.lf DO BEGIN 
;                              pix = pixels + long(k-img.ff)*frmsize
;                              val = (*img.data)[pix]
;                              frame[k-img.fp] = float(img.start[k])/1000.
;                              IF (img.len[img.f] GT 0) THEN $
;                                 mean[k-img.fp] = (moment(val))[0]/(img.len[k]/1000.) $
;                              ELSE mean[k-img.fp] = (moment(val))[0]
;                              mean[k-img.fp] = (moment(val))[0]
;                              print,frame[k-img.fp], mean[k-img.fp]
;                           ENDFOR 
;                           WINDOW, /free
;                           ymin = min(mean)
;                           ymax = max(mean)
;                           xlabel = 'Seconds'
;                           PLOT, frame, mean, charsize=1.4,charthick=2,xthick=2,ythick=2, $
;                              xtitle=xlabel, ytitle='Counts/Sec', yrange=[ymin,ymax]
;                           WSET, s.win
;                        ENDIF
;                     ENDIF
;                  ENDIF
;               ENDIF
;               widget_control, s.msg, set_value=msg
;               s.roi = VOI_EOL(voi)
;               widget_control,dtype, set_value=(*s.roi).type
            ENDCASE
            
            'Save': BEGIN
               IF (n_elements((*s.roi).loc) EQ 0) THEN BEGIN
                  msg = 'ROI contains no data'
                  result=dialog_message(msg, title='ROI was not saved!', /error)
               ENDIF ELSE IF (strtrim(s.id EQ '')) THEN BEGIN
                  msg = 'ROI has no ID'
                  result=dialog_message(msg, title='ROI was not saved!', /error)
               ENDIF ELSE BEGIN
                  newslice = 0
                  (*voi).num = (*voi).num + 1
                  (*s.roi).orient = orient
                  (*s.roi).id = s.id
                  (*s.roi).z = s.img.p
                  IF (s.id NE 'LIMIT') THEN BEGIN
                     VIDI_DEFVOI_DRAW, s, -1
                     VIDI_DEFVOI_DRAW, s, -1, /fill
                  ENDIF 
                  s.last = VOI_SORT(s.voi, s.roi)
                  s.roi = VOI_APPEND(s.voi)
                  (*s.roi).type = (*s.last).type
                  s.saved = 1
                  widget_control, s.msg, set_value='ROI saved:   '+s.id
                  IF (min('' EQ voilist) EQ 1) THEN voilist = [s.id] $
                  ELSE IF (max(s.id EQ voilist) EQ 0) THEN voilist = [voilist,[s.id]]
                  voilist = VOI_LIST(voi)
                  listing = VOI_LISTING(voi,voilist,img.dim[2])
                  widget_control,list, set_value=listing
               ENDELSE
            ENDCASE
            
            'Last Component Entered on This Slice': BEGIN    
               IF (newslice OR NOT ptr_valid(s.last)) THEN BEGIN
                  msg = 'There is no last ROI defined for this slice'
                  widget_control, s.msg, set_value=msg
               ENDIF ELSE IF ((*s.last).id EQ '') THEN BEGIN
                  msg = 'There is no last ROI defined for this slice'
                  widget_control, s.msg, set_value=msg
               ENDIF ELSE BEGIN
                  s.roi = s.last
                  IF ((*s.roi).type EQ 2) THEN VIDI_DISPLAY,uv $
                  ELSE VIDI_DEFVOI_DRAW, s, -1, /fill ; Erase roi
                  IF (VOI_DELETE(voi, (*s.last).id, slice=s.img.p, ptr=s.last) NE 0) THEN BEGIN
                     msg = 'ROI id not found on this slice'
                     widget_control, s.msg, set_value=msg
                  ENDIF ELSE BEGIN 
                     s.roi = VOI_EOL(voi)
                     s.last = VOI_LOCATE(voi,s.id)
                     IF (ptr_valid(s.last)) THEN s.saved = 1
                     widget_control,dtype, set_value=(*s.roi).type
                     msg = 'Last ROI component removed from slice '+strtrim(string(s.img.p),2)
                     widget_control, s.msg, set_value=msg
                     voilist = VOI_LIST(voi)
                     listing = VOI_LISTING(voi,voilist,img.dim[2])
                     widget_control,list, set_value=listing
                  ENDELSE 
               ENDELSE 
            ENDCASE
            
            'All Components on All Slices': BEGIN    
               IF (s.id EQ '') THEN BEGIN
                  msg = 'No ROI selected'
                  widget_control, s.msg, set_value=msg
               ENDIF ELSE BEGIN
                  IF (VOI_DELETE(voi, s.id) NE 0) THEN BEGIN
                     msg = 'ROI id not found'
                     widget_control, s.msg, set_value=msg
                  ENDIF ELSE BEGIN
                     msg = 'ROI removed from all slices'
                     widget_control, s.msg, set_value=msg
                     voilist = VOI_LIST(voi)
                     listing = VOI_LISTING(voi,voilist,img.dim[2])
                     widget_control,list, set_value=listing
                  ENDELSE
               ENDELSE
               s.roi = VOI_EOL(voi)
               widget_control,dtype, set_value=(*s.roi).type
            ENDCASE
            
            'All Components on This Slice': BEGIN    
               IF (s.id EQ '') THEN BEGIN
                  msg = 'No ROI selected'
                  widget_control, s.msg, set_value=msg
               ENDIF ELSE BEGIN
                  IF (VOI_DELETE(voi, s.id, slice=s.img.p) NE 0) THEN BEGIN
                     msg = 'ROI id not found on this slice'
                     widget_control, s.msg, set_value=msg
                  ENDIF ELSE BEGIN
                     msg = 'ROI removed from slice '+strtrim(string(s.img.p),2)
                     widget_control, s.msg, set_value=msg
                     voilist = VOI_LIST(voi)
                     listing = VOI_LISTING(voi,voilist,img.dim[2])
                     widget_control,list, set_value=listing
                  ENDELSE
               ENDELSE
               s.roi = VOI_EOL(voi)
               widget_control,dtype, set_value=(*s.roi).type
            ENDCASE
            
            'Exit':  BEGIN
               uv.wid.vthresh[0] = bot
               uv.wid.vthresh[1] = top
               uv.wid.voicolor = s.color
               WIDGET_CONTROL, base, /DESTROY
               RETURN
            ENDCASE 
            
         ENDCASE   
      ENDCASE 
   ENDWHILE
END   
