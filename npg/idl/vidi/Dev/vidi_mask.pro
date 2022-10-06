;+
; NAME:
;	VIDI_MASK
;
; PURPOSE:
;	Widget to interactively create an image mask.
; CATEGORY:
;	Graphics.
; CALLING SEQUENCE:
;	Result = VIDI_MASK(uv, inum)
; RESTRICTIONS:
;	This is a MODAL widget.  No other widget applications will be
;	responsive while this widget is in use.
;
; MODIFICATION HISTORY:
;	Tom Videen, April 2007.
;-

PRO VIDI_MASK, uv, inum
   COMMON VIDI
   COMMON IMAGE_UTILS
   
   imgnum = inum < 1
   img = uv.img[imgnum]
   draw = uv.wid.win[inum]
   group = uv.wid.base
   zoom = dsp[imgnum].zoom
   rtype = img.proc
   IF (rtype GT 2) THEN rtype = 0
   frame = img.f - img.ff
   imgsize = img.dim[0]*img.dim[1]*img.dim[2]
   imax = max(*img.data)
   scantime = img.len * 1000
   image = (*img.data)
   
   base = WIDGET_BASE(title='Mask', group_leader=group, /COL)   
   base1 = WIDGET_BASE(base, /ROW)
   select1 = ['1\Magnify','0\ 1 ','0\ 2 ','0\ 3 ','0\ 4 ']
   magnify = CW_PDMENU(base1, select1)
   up = WIDGET_BUTTON(base1, value='<- Up  ')
   down = WIDGET_BUTTON(base1, value=' Down ->')
   create = WIDGET_BUTTON(base1, value='Create Mask')
   mask = WIDGET_BUTTON(base1, value='Apply Mask')
   exit = WIDGET_BUTTON(base1, value='Exit')
   preview = cw_bgroup(base1, ['Preview Mask'], /NONEXCLUSIVE)

   base2 = WIDGET_BASE(base,/ROW)
   fillvalue = CW_FIELD(base2, title='Fill Value', value=uv.wid.editval, $
                  xsize=3, /INTEGER, /RETURN_EVENTS)
   threshold = CW_FIELD(base2, title='Threshold %', value=uv.wid.rthresh, $
                  xsize=3, /INTEGER, /RETURN_EVENTS)
   CASE rtype OF 
      0: str = 'None'
      1: str = 'CBF'
      2: str = 'CBV'
   ENDCASE
   process = CW_FIELD(base2, title='Processing',value=str, xsize=4)
   plane = img.p
   fp = img.fp
   lp = img.lp
   top = uv.wid.topslc
   bot = uv.wid.botslc
   top = top < bot
   bot = bot > top
   topslc = CW_FIELD(base2, title='Slice Range: Top', value=top, xsize=3, /INTEGER, /RETURN_EVENTS)
   botslc = CW_FIELD(base2, title='Bottom', value=bot, xsize=3, /INTEGER, /RETURN_EVENTS)

   base3 = WIDGET_BASE(base, /COL)
   slider = WIDGET_SLIDER(base3, value=plane, xsize=700, min=fp, max=lp, title='Slice')

   msg = WIDGET_TEXT(base, YSIZE=1)

   WIDGET_CONTROL, draw, GET_VALUE=win
   WSET, win
   WINDOW, /PIXMAP, /FREE, xs = !d.x_size, ys=!d.y_size ; Save window
   backing = !d.window
   DEVICE, copy = [0,0, !d.x_size, !d.y_size, 0, 0, win]

   s = {MASK_STATE, $        ; Structure containing state
        base: base, $           ; Main base widget
        win:  win, $            ; draw widget window #
        msg : msg, $            ; Message window
        button: 0, $            ; button state
        zoom : zoom $           ; zoom factor
       }

   WIDGET_CONTROL, base, /REALIZE, xoffset=0, yoffset=0
   WSHOW, win

   set = 0
   refresh = 1
   mask_type = 1
   connect_type = 1
   seed[0,0] = 0
   seed[1,0] = 0
   vmin = -32767

   WHILE 1 DO BEGIN             ; Event Loop   
      bot = uv.wid.botslc
      top = uv.wid.topslc
      plane = img.p
	  seed[2,0] = plane
      IF (refresh) THEN VIDI_DISPLAY, uv
      imax = max((*img.data)[*,*,top-1:bot-1])
      slcmax = VOI_PROCESS (img.proc, imax, img.mtype, img.mcal, $
         0, uv.cbf[imgnum].mcal, 0, uv.cbv[imgnum].mcal, scantime)
      vmax = slcmax * uv.wid.rthresh / 100.
      IF (set) THEN VIDI_OUTLINE, s, imgnum, uv, vmin, vmax, indx, 0
      str = strcompress(string(slcmax),/remove_all)
      wset, dsp[imgnum].id
      xyouts,s.zoom*3,s.zoom*3,str,charsize=s.zoom,charthick=1,/device,color=label
      refresh = 1

      ev = WIDGET_EVENT([base, draw])

      CASE ev.id OF
         draw:   
         fillvalue:   uv.wid.editval = ev.value
         threshold:   uv.wid.rthresh = ev.value

         topslc:   BEGIN
			top = ev.value
   			top = top > fp
   			top = top < lp
   			bot = bot > top
			uv.wid.topslc = top
			uv.wid.botslc = bot
            WIDGET_CONTROL, topslc, set_value=top
            WIDGET_CONTROL, botslc, set_value=bot
         ENDCASE

         botslc:   BEGIN
			bot = ev.value
   			bot = bot > fp
   			bot = bot < lp
   			top = top < bot
			uv.wid.topslc = top
			uv.wid.botslc = bot
            WIDGET_CONTROL, topslc, set_value=top
            WIDGET_CONTROL, botslc, set_value=bot
         ENDCASE

         create: BEGIN
            FOR slice = top, bot DO BEGIN
			   seed[2,0] = slice
               uv.img[imgnum].p = slice
               ;imax = max((*img.data)[*,*,slice-1])
               ;slcmax = VOI_PROCESS (img.proc, imax, img.mtype, img.mcal, $
               ;   0, uv.cbf[imgnum].mcal, 0, uv.cbv[imgnum].mcal, scantime)
               ;vmax = slcmax * uv.wid.rthresh / 100.
               VIDI_OUTLINE, s, imgnum, uv, vmin, vmax, indx, 1
               WIDGET_CONTROL, slider, set_value=slice
            ENDFOR
            maskimg = (*img.data)
            uv.img[imgnum].p = plane
         ENDCASE

         mask: BEGIN
            image = (*img.data)
            pix = where (maskimg LE 0, count)
            print,'mask ',count
            IF (count GT 0) THEN image(pix) = 0
            (*img.data) = image
         ENDCASE

         magnify: BEGIN
            dsp[0].zoom = ev.value
            dsp[1].zoom = ev.value
            dsp[2].zoom = ev.value
            s.zoom = ev.value
         ENDCASE

         up: BEGIN
            IF (img.p GT fp) THEN BEGIN
               img.p = img.p-1
               uv.img[0].p = img.p
               uv.img[1].p = img.p
               WIDGET_CONTROL, slider, set_value=img.p
            ENDIF
         ENDCASE

         down: BEGIN
            IF (img.p LT lp) THEN BEGIN
               img.p = img.p+1
               uv.img[0].p = img.p
               uv.img[1].p = img.p
               WIDGET_CONTROL, slider, set_value=img.p
            ENDIF
         ENDCASE

         slider: BEGIN
            WIDGET_CONTROL, slider, get_value=slice
            img.p = slice
            uv.img[0].p = img.p
            uv.img[1].p = img.p
            WIDGET_CONTROL, slider, set_value=img.p
         ENDCASE

         exit: BEGIN
            uv.img[0].p = img.p
            uv.img[1].p = img.p
   			uv.wid.topslc = top
   			uv.wid.botslc = bot
            (*img.data) = image
			widget_control, ev.top, set_uvalue=uv
            WIDGET_CONTROL, base, /DESTROY
            RETURN
         ENDCASE 

         preview: BEGIN
            IF (set EQ 0) THEN set = 1 $
            ELSE set = 0
         ENDCASE 

      ENDCASE 
   ENDWHILE			; End Event Loop
END   
