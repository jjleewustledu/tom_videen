;+
; NAME:
;	VIDI_PEAKS
;
; PURPOSE:
;	Widget to read and display voi coordinates.
;	Reads and writes voistat voi files.
; CATEGORY:
;	Regions of interest, graphics.
; CALLING SEQUENCE:
;	Result = VIDI_PEAKS(uv, orient, zoom)
; RESTRICTIONS:
;	This is a MODAL widget.  No other widget applications will be
;	responsive while this widget is in use.
;
; MODIFICATION HISTORY:
;	Tom Videen, Sept 1999.
;-

PRO VIDI_PEAKS_DRAW, s
; Draw outline around voi peak
; Image coordinates are tranposed to screen coordinates before
; drawing.
   
   FOR k=0,2 DO BEGIN
      IF (s.win[k] GT 0) THEN BEGIN 
         WSET, s.win[k]
         xsave = !x.s & ysave = !y.s ; Set scaling to pixel coords
         !x.s = [0, s.zoom/float(!d.x_size)]
         !y.s = [0, s.zoom/float(!d.y_size)]
         ydim = s.img.dim[1]
         DEVICE, SET_GRAPHICS_FUNCTION=s.color	; XOR
         n = 32                 ; #points to define the circle
         a = findgen(n+1) * (2 * !pi/(n-1))
         x = s.x
         y = s.y
         r = s.r
         xy = transpose([[x + r * cos(a)], [y + r * sin(a)]])
         xy = VOI_TRANSPOSE(xy, ydim, /DISPLAY)
         plots, xy
         !x.s = xsave & !y.s = ysave
         DEVICE, SET_GRAPHICS_FUNCTION=3 ; Copy mode   
      ENDIF 
   ENDFOR 
END   
  
PRO VIDI_PEAKS, uv, orient, zoom
   COMMON VIDI
   
   win = [0L,0L,0L]
   FOR k=2,0,-1 DO BEGIN 
      draw = uv.wid.win[k]
      IF (draw GT 0) THEN BEGIN 
         WIDGET_CONTROL, draw, GET_VALUE=tmp
         IF (tmp GT 0) THEN BEGIN 
            win[k] = tmp
            IF (k LT 2) THEN img = uv.img[k]
         ENDIF 
      ENDIF ELSE  win[k] = 0
   ENDFOR 
   group = uv.wid.base   
   
   base = WIDGET_BASE(title='Peak Identification', group_leader=group, /ROW)   
   base1 = WIDGET_BASE(base,/COLUMN)   
   base2 = WIDGET_BASE(base1, /ROW)
   temp = CW_BGROUP(base2, /COLUMN, /NO_RELEASE, /RETURN_NAME,  $
                    ['Exit', 'Read', 'Draw', 'Next']  )
   select1 = ['1\Radius','0\ 1 ','0\ 2 ','0\ 3 ','0\ 4 ','0\ 5 ','0\ 6 ','0\ 7 ','0\ 8 ', $
              '0\ 9 ', '0\ 10 ']
   radius = CW_PDMENU(base1, select1)
   color = 6
   cval = cw_bgroup(base1, ['XOR','White'],/ROW, /EXCLUSIVE, set_value=0)

   base3 = WIDGET_BASE(base,/COLUMN)
   msg = WIDGET_TEXT(base3, YSIZE=1)
   list = WIDGET_LIST(base3, XSIZE=30, YSIZE=10)
   
   peaks = {PEAK_STR, $
            id:	'', $
            x:	0, $
            y:	0, $
            z:	0 $
           }
   
   s = {PEAK_STATE, $           ; Structure containing state
        base: base, $           ; Main base widget
        msg : msg, $            ; Message window
        win:  win, $            ; draw widget window #
        zoom : zoom, $          ; zoom factor
        img : img, $		; pointer to image data
        id  : '', $		; current VOI id
        x : 0.0, $              ; x coorinate
        y : 0.0, $              ; y coordinate
        z : 0.0, $              ; z coordinate
        r : 5, $                ; radius in pixels
        color : color $         ; color (XOR or White)
       }
   
   WIDGET_CONTROL, base, /REALIZE, xoffset=445, yoffset=0
   widget_control, s.msg, set_value='Plots points from a voi/peaks file'

   peaknum = 0
   WHILE 1 DO BEGIN             ; Internal event loop   
      ev = WIDGET_EVENT(base)
      CASE ev.id OF
         
         list: BEGIN
            n = ev.index
            peaknum = n
            s.id = peak[n].id
            IF (orient EQ 0) THEN BEGIN
               s.x = peak[n].x
               s.y = peak[n].y
               s.z = peak[n].z
            ENDIF ELSE IF (orient EQ 1) THEN BEGIN
               s.x = peak[n].x
               s.y = peak[n].z
               s.z = peak[n].y
            ENDIF ELSE BEGIN
               s.x = peak[n].y
               s.y = peak[n].z
               s.z = peak[n].x
            ENDELSE
            widget_control, s.msg, set_value=s.id
         ENDCASE 
         
         radius: s.r = ev.value

         cval: BEGIN
            IF (ev.value EQ 0) THEN s.color = 6 $
            ELSE s.color = 3
         ENDCASE

         
         ELSE: CASE ev.value OF
            
            'Draw': BEGIN
               IF (s.id EQ '') THEN BEGIN
                  widget_control, s.msg, set_value='No voi has been identified'
               ENDIF ELSE BEGIN
                  s.img.p = s.z
                  uv.img[0].p = s.z
                  uv.img[1].p = s.z
                  widget_control, uv.wid.slider, set_value=s.img.p
                  VIDI_DISPLAY, uv
                  VIDI_PEAKS_DRAW, s
               ENDELSE
            ENDCASE
            
            'Next': BEGIN
               peaknum = peaknum + 1
               IF (peaknum GE npeaks) THEN peaknum = 0
               n = peaknum
               s.id = peak[n].id
               IF (s.id EQ '') THEN BEGIN
                  widget_control, s.msg, set_value='No voi has been identified'
               ENDIF ELSE BEGIN
                  IF (orient EQ 0) THEN BEGIN
                     s.x = peak[n].x
                     s.y = peak[n].y
                     s.z = peak[n].z
                  ENDIF ELSE IF (orient EQ 1) THEN BEGIN
                     s.x = peak[n].x
                     s.y = peak[n].z
                     s.z = peak[n].y
                  ENDIF ELSE BEGIN
                     s.x = peak[n].y
                     s.y = peak[n].z
                     s.z = peak[n].x
                  ENDELSE
                  widget_control, s.msg, set_value=s.id
                  s.img.p = s.z
                  uv.img[0].p = s.z
                  uv.img[1].p = s.z
                  widget_control, uv.wid.slider, set_value=s.img.p
                  VIDI_DISPLAY, uv
                  VIDI_PEAKS_DRAW, s
               ENDELSE
            ENDCASE
            
            'Read': BEGIN
               fname = dialog_pickfile(FILE=img.fname, FILTER='*.voi', /READ)
               IF (strlen(fname) EQ 0) THEN BEGIN
                  widget_control, s.msg, set_value='No file entered'
               ENDIF ELSE BEGIN 
                  openr,lun, fname, ERROR=err, /GET_LUN
                  IF (err NE 0) THEN widget_control, s.msg, set_value='Cannot open '+fname $
                  ELSE BEGIN 
                     str = ''
                     npeaks =  0L
                     id = ''
                     x = 0.0
                     y = 0.0
                     z = 0.0
                     listing = ['']
                     readf, lun, str
                     readf, lun, str
                     readf, lun, npeaks
                     FOR n=0,npeaks-1 DO BEGIN
                        readf, lun, str
                        IF (min('' EQ listing) EQ 1) THEN BEGIN
                           listing = [str]
                           peak = [peaks]
                        ENDIF ELSE BEGIN 
                           listing = [listing,str]
                           peak = [peak, peaks]
                        ENDELSE 
                        k = strpos(str,' ')
                        id = strmid(str,0,k)
                        str = strmid(str,k)
                        reads, str, x, y, z
                        peak[n].id = id
                        peak[n].x = x
                        peak[n].y = y 
                        peak[n].z = z
                     ENDFOR 
                     close, lun
                     free_lun, lun
                     widget_control,list, set_value=listing
                     widget_control, s.msg, set_value='Peaks from '+fname 
                     widget_control, list, /SHOW
                  ENDELSE 
               ENDELSE 
            ENDCASE 
            
            'Exit':  BEGIN
               WIDGET_CONTROL, base, /DESTROY
               RETURN
            ENDCASE 
            
         ENDCASE   
      ENDCASE 
   ENDWHILE
END   
