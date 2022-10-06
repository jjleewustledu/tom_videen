PRO VIDIFDISP1_EVENT, ev
   COMMON IMAGE_UTILS
   COMMON VIDI
   widget_control, ev.top, get_uvalue=top
   widget_control, top, get_uvalue=uv

   i = 0
   xfact = 1.
   yfact = 1.
   zoom = dsp[i].zoom
   CASE orient OF
      0: BEGIN
         xsiz = uv.img[i].pdim[0]
         ysiz = uv.img[i].pdim[1]
         ydim = uv.img[i].dim[1]
      ENDCASE
      1: BEGIN
         xsiz = uv.img[i].pdim[0]
         ysiz = uv.img[i].pdim[2]
         ydim = uv.img[i].dim[2]
      ENDCASE
      2: BEGIN
         xsiz = uv.img[i].pdim[1]
         ysiz = uv.img[i].pdim[2]
         ydim = uv.img[i].dim[2]
      ENDCASE
   ENDCASE
   IF (ysiz GT xsiz) THEN yfact = ysiz/xsiz $
   ELSE IF (xsiz GT ysiz) THEN xfact = xsiz/ysiz
   xc = ev.x / (zoom * xfact) + 1
   yc = ydim - ((ev.y + 1) / (zoom * yfact) - 1)
   CASE orient OF
      0: BEGIN
         x = xc
         y = yc
         z = uv.img[i].p
      ENDCASE
      1: BEGIN
         x = xc
         y = uv.img[i].p
         z = yc
      ENDCASE
      2: BEGIN
         x = uv.img[i].p
         y = xc
         z = yc
      ENDCASE
   ENDCASE
   str = string(x, y, z, format='("X =",I6,"  Y =",I6,"  Z =",I6)')
   widget_control, uv.wid.msg, set_value=str

   IF (ev.press) THEN BEGIN
      x = xc
      y = ydim-yc
      xy = [[x-2,y-2],[x-2,y+2],[x+2,y+2],[x+2,y-2],[x-2,y-2]]
      !x.s = [0, zoom/(float(!d.x_size) / xfact)]
      !y.s = [0, zoom/(float(!d.y_size) / yfact)]
      col = dsp[2].bot + 0.85*dsp[2].nc
      wset,dsp[0].id
      plots, xy, color=col
      wset,dsp[1].id
      plots, xy, color=col
      col = dsp[2].bot - 1
      wset,dsp[2].id
      plots, xy, color=col
   ENDIF
END   

PRO VIDIFDISP2_EVENT, ev
   COMMON IMAGE_UTILS
   COMMON VIDI
   widget_control, ev.top, get_uvalue=top
   widget_control, top, get_uvalue=uv
   i = 1
   xfact = 1.
   yfact = 1.
   zoom = dsp[i].zoom
   CASE orient OF
      0: BEGIN
         xsiz = uv.img[i].pdim[0]
         ysiz = uv.img[i].pdim[1]
         ydim = uv.img[i].dim[1]
      ENDCASE
      1: BEGIN
         xsiz = uv.img[i].pdim[0]
         ysiz = uv.img[i].pdim[2]
         ydim = uv.img[i].dim[2]
      ENDCASE
      2: BEGIN
         xsiz = uv.img[i].pdim[1]
         ysiz = uv.img[i].pdim[2]
         ydim = uv.img[i].dim[2]
      ENDCASE
   ENDCASE
   IF (ysiz GT xsiz) THEN yfact = ysiz/xsiz $
   ELSE IF (xsiz GT ysiz) THEN xfact = xsiz/ysiz
   xc = ev.x / (zoom * xfact) + 1
   yc = ydim - ((ev.y + 1) / (zoom * yfact) - 1)
   CASE orient OF
      0: BEGIN
         x = xc
         y = yc
         z = uv.img[i].p
      ENDCASE
      1: BEGIN
         x = xc
         y = uv.img[i].p
         z = yc
      ENDCASE
      2: BEGIN
         x = uv.img[i].p
         y = xc
         z = yc
      ENDCASE
   ENDCASE
   str = string(x, y, z, format='("X =",I6,"  Y =",I6,"  Z =",I6)')
   widget_control, uv.wid.msg, set_value=str

   IF (ev.press) THEN BEGIN
      x = xc
      y = ydim-yc
      xy = [[x-2,y-2],[x-2,y+2],[x+2,y+2],[x+2,y-2],[x-2,y-2]]
      !x.s = [0, zoom/(float(!d.x_size) / xfact)]
      !y.s = [0, zoom/(float(!d.y_size) / yfact)]
      col = dsp[2].bot + 0.85*dsp[2].nc
      wset,dsp[0].id
      plots, xy, color=col
      wset,dsp[1].id
      plots, xy, color=col
      col = dsp[2].bot - 1
      wset,dsp[2].id
      plots, xy, color=col
   ENDIF
END   

PRO VIDIFDISP3_EVENT, ev
   COMMON IMAGE_UTILS
   COMMON VIDI
   widget_control, ev.top, get_uvalue=top
   widget_control, top, get_uvalue=uv
   i = 0
   xfact = 1.
   yfact = 1.
   zoom = dsp[i].zoom
   CASE orient OF
      0: BEGIN
         xsiz = uv.img[i].pdim[0]
         ysiz = uv.img[i].pdim[1]
         ydim = uv.img[i].dim[1]
      ENDCASE
      1: BEGIN
         xsiz = uv.img[i].pdim[0]
         ysiz = uv.img[i].pdim[2]
         ydim = uv.img[i].dim[2]
      ENDCASE
      2: BEGIN
         xsiz = uv.img[i].pdim[1]
         ysiz = uv.img[i].pdim[2]
         ydim = uv.img[i].dim[2]
      ENDCASE
   ENDCASE
   IF (ysiz GT xsiz) THEN yfact = ysiz/xsiz $
   ELSE IF (xsiz GT ysiz) THEN xfact = xsiz/ysiz
   xc = ev.x / (zoom * xfact) + 1
   yc = ydim - ((ev.y + 1) / (zoom * yfact) - 1)
   CASE orient OF
      0: BEGIN
         x = xc
         y = yc
         z = uv.img[i].p
      ENDCASE
      1: BEGIN
         x = xc
         y = uv.img[i].p
         z = yc
      ENDCASE
      2: BEGIN
         x = uv.img[i].p
         y = xc
         z = yc
      ENDCASE
   ENDCASE
   str = string(x, y, z, format='("X =",I6,"  Y =",I6,"  Z =",I6)')
   widget_control, uv.wid.msg, set_value=str

   IF (ev.press) THEN BEGIN
      x = xc
      y = ydim-yc
      xy = [[x-2,y-2],[x-2,y+2],[x+2,y+2],[x+2,y-2],[x-2,y-2]]
      !x.s = [0, zoom/(float(!d.x_size) / xfact)]
      !y.s = [0, zoom/(float(!d.y_size) / yfact)]
      col = dsp[2].bot + 0.85*dsp[2].nc
      wset,dsp[0].id
      plots, xy, color=col
      wset,dsp[1].id
      plots, xy, color=col
      col = dsp[2].bot - 1
      wset,dsp[2].id
      plots, xy, color=col
   ENDIF
END   

PRO VIDIF_EVENT, ev
   COMMON IMAGE_UTILS
   COMMON VIDI
   
   catch, error_status
   IF (error_status NE 0) THEN BEGIN
      ok = widget_message(!err_string)
      RETURN
   END
   setuv = 0
   newdif = 0
   newrange = 0
   redisplay = 0
   widget_control, ev.top, get_uvalue=uv
   widget_control, uv.wid.msg, set_value=''
   
   nimg = 2                     ; max index for images
   nwin = 2                     ; max index for display windows
   CASE ev.id OF
      
      uv.wid.file: BEGIN
         
         i = ev.value -1
         
         BEGIN                  ; First Frame, All Planes
            uv.img[i].ff = 1
            uv.img[i].lf = 1
            uv.img[i].fp = 1
            uv.img[i].lp = 512
         ENDCASE
         
         fname = ''
         IF (strlen(uv.img[i].fname) GT 0) THEN fname = uv.img[i].fname $
         ELSE IF (strlen(uv.img[0].fname) GT 0) THEN fname = uv.img[0].fname $
         ELSE IF (strlen(uv.img[1].fname) GT 0) THEN fname = uv.img[1].fname

         IF (strlen(fname) GT 0) THEN BEGIN 
            l1 = rstrpos(fname,'/')
            l2 = rstrpos(fname,'.')
            IF (l2 GT 0) THEN BEGIN
               str1 = string(replicate(32b,(l2-l1-2)))
               str1 = '*'+str1
               strput,fname,str1,l1+1
               fname = strcompress(fname, /remove_all)
            ENDIF ELSE fname = '*'
         ENDIF ELSE fname = '*'
         
         fname = dialog_pickfile(FILE=uv.img[i].fname, FILTER=fname, /READ)
         IF (strlen(fname) EQ 0) THEN BEGIN
            widget_control, uv.wid.msg, set_value='No file entered'
         ENDIF ELSE BEGIN 
            image = uv.img[i]
            image.fname = fname
            msg = ''
            IF (getframe(image, msg) NE 0) THEN BEGIN
               widget_control, uv.wid.msg, set_value=msg
               uv.img[i].fname = ''
               RETURN
            ENDIF
            tmp = uv.img[i].p
            uv.img[i] = image
            IF (orient EQ 0) THEN BEGIN ; keep same plane if in range
               uv.img[i].p = tmp > uv.img[i].fp
               uv.img[i].p = tmp < uv.img[i].lp
            ENDIF ELSE uv.img[i].p = tmp
            widget_control, uv.wid.msg, set_value='File '+uv.img[i].fname+' found'
            widget_control, uv.wid.fname[i], set_value=uv.img[i].fname
            setuv = 1
            newdif = 1
            newrange = 1
            redisplay = 1
         ENDELSE 

      ENDCASE 

      ;; -------------------------------------------------------------------------------
      uv.wid.fname[0]: BEGIN	; same as last
         i = 0
         fname = ev.value
         IF (strlen(fname) EQ 0) THEN BEGIN
            widget_control, uv.wid.msg, set_value='No file entered'
         ENDIF ELSE BEGIN 
            uv.img[i].fp = 1
            uv.img[i].lp = 512
            image = uv.img[i]
            image.fname = fname
            msg = ''
            IF (getframe(image, msg) NE 0) THEN BEGIN
               widget_control, uv.wid.msg, set_value=msg
               uv.img[i].fname = ''
               RETURN
            ENDIF
            tmp = uv.img[i].p
            uv.img[i] = image
            IF (orient EQ 0) THEN BEGIN ; keep same plane if in range
               uv.img[i].p = tmp > uv.img[i].fp
               uv.img[i].p = tmp < uv.img[i].lp
            ENDIF ELSE uv.img[i].p = tmp
            widget_control, uv.wid.msg, set_value='File '+uv.img[i].fname+' found'
            widget_control, uv.wid.fname[i], set_value=uv.img[i].fname
            setuv = 1
            newdif = 1
            newrange = 1
            redisplay = 1
         ENDELSE 
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      
      uv.wid.fname[1]: BEGIN	; same as last
         i = 1
         fname = ev.value
         IF (strlen(fname) EQ 0) THEN BEGIN
            widget_control, uv.wid.msg, set_value='No file entered'
         ENDIF ELSE BEGIN 
            uv.img[i].fp = 1
            uv.img[i].lp = 512
            image = uv.img[i]
            image.fname = fname
            msg = ''
            IF (getframe(image, msg) NE 0) THEN BEGIN
               widget_control, uv.wid.msg, set_value=msg
               uv.img[i].fname = ''
               RETURN
            ENDIF
            tmp = uv.img[i].p
            uv.img[i] = image
            IF (orient EQ 0) THEN BEGIN ; keep same plane if in range
               uv.img[i].p = tmp > uv.img[i].fp
               uv.img[i].p = tmp < uv.img[i].lp
            ENDIF ELSE uv.img[i].p = tmp
            widget_control, uv.wid.msg, set_value='File '+uv.img[i].fname+' found'
            widget_control, uv.wid.fname[i], set_value=uv.img[i].fname
            setuv = 1
            newdif = 1
            newrange = 1
            redisplay = 1
         ENDELSE 
      ENDCASE 

      ;; -------------------------------------------------------------------------------
      uv.wid.orient: BEGIN
         CASE ev.value OF
            'Transverse': BEGIN
               orient = 0
               lp = uv.img[0].lp
            ENDCASE 
            'Coronal': BEGIN 
               IF (uv.img[0].dim[2] GT 1) THEN BEGIN
                  orient = 1
                  lp = uv.img[0].dim[1] 
               ENDIF ELSE BEGIN
                  msg = 'Transverse orientation only for multiple planes'
                  widget_control, uv.wid.msg, set_value=msg
               ENDELSE
            ENDCASE 
            'Sagittal': BEGIN
               IF (uv.img[0].dim[2] GT 1) THEN BEGIN
                  orient = 2
                  lp = uv.img[0].dim[0] 
               ENDIF ELSE BEGIN
                  msg = 'Transverse orientation only for multiple planes'
                  widget_control, uv.wid.msg, set_value=msg
               ENDELSE
            ENDCASE 
         ENDCASE
         setuv = 1
         newrange = 1
         redisplay = 1
         slice = (fp + lp)/2
         uv.img[0].p = slice
         uv.img[1].p = slice
         widget_control, uv.wid.slider, set_slider_max=lp
         widget_control, uv.wid.slider, set_value=slice
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.zoom: BEGIN
         FOR i=0,nwin DO dsp[i].zoom = ev.value
         redisplay = 1
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.scale: BEGIN
         tmp = ev.value
         IF (tmp LE 6) THEN maxval = 50*tmp $
         ELSE IF (tmp LE 13) THEN maxval =  300 + 100*(tmp-6) $
         ELSE IF (tmp LE 18) THEN maxval = 1000 + 200*(tmp-13) $
         ELSE IF (tmp LE 24) THEN maxval = 2000 + 500*(tmp-18) $
         ELSE IF (tmp LE 29) THEN maxval = 5000 + 1000*(tmp-24) $
         ELSE maxval = 10000 + 5000*(tmp-29)
         FOR i=0,1 DO dsp[i].max = maxval
         dsp[2].max = maxval * percent / 100
         dsp[2].min = -dsp[2].max
         redisplay = 1
         widget_control, uv.wid.msg, set_value='Difference image is scaled to constant percent of Image Max'
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.diff: BEGIN
         tmp = ev.value
         IF (tmp LE 5) THEN percent = 2*tmp $
         ELSE IF (tmp LE 13) THEN percent = 10 + 5*(tmp-5) $
         ELSE percent = 50 + 10*(tmp-13)
         dsp[2].max = dsp[0].max * percent / 100
         dsp[2].min = -dsp[2].max
         redisplay = 1
      ENDCASE 
      ;; -------------------------------------------------------------------------------

      uv.wid.cut: BEGIN
         dsp[0].cut = 10 * ev.value
         redisplay = 1
      ENDCASE 

      ;; -------------------------------------------------------------------------------
      uv.wid.smooth: BEGIN
         dsp[2].set = ev.value
         redisplay = 1
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.max[0]: BEGIN	
         dsp[0].max = ev.value
         redisplay = 1
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.max[1]: BEGIN	
         dsp[1].max = ev.value
         redisplay = 1
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.max[2]: BEGIN	
         dsp[2].max = ev.value
         dsp[2].min = -dsp[2].max
         redisplay = 1
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.exit: BEGIN
         FOR i=0, nimg DO if (uv.img[i].data NE ptr_new()) THEN ptr_free, uv.img[i].data
         widget_control, ev.top, /destroy
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.up: BEGIN
         FOR i=0, nimg DO IF (uv.img[i].p GT uv.img[i].fp) THEN uv.img[i].p = uv.img[i].p-1
         widget_control, uv.wid.slider, set_value=uv.img[0].p
         setuv = 1
         redisplay = 1
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.mid: BEGIN
         FOR i=0, nimg DO uv.img[i].p = (lp + fp)/2
         widget_control, uv.wid.slider, set_value=uv.img[0].p
         setuv = 1
         redisplay = 1
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.down: BEGIN
         p = uv.img[0].p
         FOR i=0, nimg DO IF (uv.img[i].p LT lp) THEN uv.img[i].p = uv.img[i].p+1
         widget_control, uv.wid.slider, set_value=uv.img[0].p
         setuv = 1
         redisplay = 1
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.slider: BEGIN
         widget_control, uv.wid.slider, get_value=slice
         FOR i=0, nimg DO IF (slice LE lp) THEN uv.img[i].p = slice
         setuv = 1
         redisplay = 1
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.display: BEGIN
         ;; set=0 -> Difference Alone (lut 37: red/green)
         ;; set=1 -> Difference on 1  (lut 37: red/green)
         ;; set=2 -> Difference on 2  (lut 37: red/green)
         ;; set=3 -> Image 2 on 1     (lut  3: hot metal)
         dsp[0].set = ev.value
         IF (dsp[0].set EQ 3) THEN BEGIN
            loadct, 3, ncolors=dsp[2].nc, bottom=dsp[2].bot, file=colortable
         ENDIF ELSE loadct, 37, ncolors=dsp[2].nc, bottom=dsp[2].bot, file=colortable
         newrange = 1
         redisplay = 1
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      
      ELSE:
      
   ENDCASE
   
   ;;-----------------------------------------------------------------------------------

   IF (newdif) THEN BEGIN
      IF (uv.img[2].data NE ptr_new()) THEN ptr_free, uv.img[2].data
      data = (*uv.img[1].data[*,*,*,*]) - (*uv.img[0].data[*,*,*,*])
      uv.img[2].data = ptr_new(data)
   ENDIF

   ;;-----------------------------------------------------------------------------------
   
   IF (newrange) THEN BEGIN
      
      ff = uv.img[0].ff
      lf = uv.img[0].lf
      i = 0

      CASE orient OF
         0: BEGIN
            fp = uv.img[0].fp
            lp = uv.img[0].lp
         ENDCASE
         1: BEGIN               ; coronal
            fp = 1
            lp = uv.img[i].dim[1]
         ENDCASE
         2: BEGIN               ; sagittal
            fp = 1
            lp = uv.img[i].dim[0]
         ENDCASE
      ENDCASE

      widget_control, uv.wid.slider, set_slider_min=fp
      IF (uv.img[i].lp GT uv.img[i].fp+1) THEN $
         widget_control, uv.wid.slider, set_slider_max=lp $
      ELSE widget_control, uv.wid.slider, set_slider_max=fp+1
      IF (uv.img[i].p GT lp) THEN FOR k=0,1 DO uv.img[k].p = lp
      IF (uv.img[i].p LT fp) THEN FOR k=0,1 DO uv.img[k].p = fp
      widget_control, uv.wid.slider, set_value=uv.img[i].p
      IF (uv.img[0].f GT uv.img[0].lf) THEN uv.img[0].f = uv.img[0].lf
      IF (uv.img[0].f LT uv.img[0].ff) THEN uv.img[0].f = uv.img[0].ff
   ENDIF 
   ;;-----------------------------------------------------------------------------------
   
   IF (redisplay) THEN BEGIN 
      FOR i= 0, nwin DO BEGIN
         IF (i LT 2) THEN BEGIN
            IF (strlen(uv.img[i].fname) EQ 0) THEN BEGIN 
               widget_control, uv.wid.msg, set_value='No file has been entered for #'+string(i+1) 
               redisplay = 0
            ENDIF 
         ENDIF ELSE BEGIN
            IF ((strlen(uv.img[0].fname) EQ 0) OR (strlen(uv.img[1].fname) EQ 0)) THEN BEGIN 
               widget_control, uv.wid.msg, set_value='Two image files must be entered'
               redisplay = 0
            ENDIF 
         ENDELSE
         widget_control, uv.wid.win[i], bad_id=status
         IF (status NE 0) then dsp[i].init = 0
         IF (dsp[i].init EQ 0) THEN BEGIN
            CASE i OF
               0: dispbase = widget_base(TITLE='Image 1',group_leader=uv.wid.base, uvalue=ev.top)
               1: dispbase = widget_base(TITLE='Image 2',group_leader=uv.wid.base, uvalue=ev.top)
               2: dispbase = widget_base(TITLE='Image 2 - Image 1',group_leader=uv.wid.base, uvalue=ev.top)
            ENDCASE
            win = widget_draw(dispbase, xsize=dsp[i].xsize, ysize=dsp[i].ysize, /button, /motion)
            !p.background = 0
            yloc = 285
            xstep = dsp[0].zoom*(uv.img[0].dim[0]) + 6
            CASE i OF
               0: xloc = 0
               1: xloc = xstep
               2: xloc = 2*xstep
            ENDCASE
            widget_control, dispbase, /realize, xoffset=xloc, yoffset=yloc
            widget_control, win, get_value=windowIndex
            wset, windowIndex
            xmanager, "VIDIFDISP"+strcompress(string(i+1),/remove_all), dispbase
            uv.wid.win[i] = win
            dsp[i].id = windowIndex
            dsp[i].init = 1
            setuv = 1
         ENDIF
      ENDFOR
      IF (redisplay) THEN VIDIF_DISPLAY, uv
   ENDIF 

   IF (setuv) THEN widget_control, ev.top, set_uvalue=uv
END


PRO VIDIF
@IMAGE_UTILS.h
@VIDI.h

   on_error, 2
   inputfile = strarr(2)
   inputlut = 0
   inputfile[0] = getenv('FNAME1')
   inputfile[1] = getenv('FNAME2')
   inputlut = getenv('LUT1')

   base = widget_base (TITLE='VIDI_DIF', /COLUMN)
   base1 = widget_base(base, row=1)
   
   select0 =  ['1\Get File', '0\Image 1', '0\Image 2']
   file = cw_pdmenu(base1, select0)

   select3 = ['1\Orientation','0\Transverse','0\Coronal','0\Sagittal']
   orient = cw_pdmenu(base1, select3, /RETURN_NAME)
   
   select4 = ['1\Magnify','0\ 1 ','0\ 2 ','0\ 3 ','0\ 4 ']
   zoom = cw_pdmenu(base1, select4)
   
   select5 = ['1\Image Max',  '0\   50 ','0\  100 ','0\  150 ','0\  200 ',$
              '0\  250 ','0\  300 ','0\  400 ','0\  500 ','0\  600 ',$
              '0\  700 ','0\  800 ','0\  900 ','0\ 1000 ','0\ 1200 ',$
              '0\ 1400 ','0\ 1600 ','0\ 1800 ','0\ 2000 ','0\ 2500 ',$
              '0\ 3000 ','0\ 3500 ','0\ 4000 ','0\ 4500 ','0\ 5000 ',$
              '0\ 6000 ','0\ 7000 ','0\ 8000 ','0\ 9000 ','0\10000 ',$
              '0\15000 ','0\20000 ','0\25000 ','0\30000 ']
   scale = cw_pdmenu(base1, select5)

   select6 = ['1\Difference Max','0\   2% ','0\   4% ','0\   6% ','0\   8% ','0\ 10% ', $
              '0\ 15% ','0\ 20% ','0\ 25% ','0\ 30% ','0\ 35% ','0\ 40% ','0\ 45%', $
              '0\ 50% ','0\ 60% ','0\ 70% ','0\ 80% ','0\ 90% ','0\100% ']
   diff = cw_pdmenu(base1, select6)
   
   select7 = ['1\Cutoff %','0\ 10% ', '0\ 20% ','0\ 30% ','0\ 40% ','0\ 50% ', $
              '0\ 60% ','0\ 70% ','0\ 80% ','0\ 90% ','0\100% ']
   cut = cw_pdmenu(base1, select7)
   
   exit = widget_button(base1, value='Exit')
   
   base2 = widget_base(base, row=1)
   base2a = widget_base(base2, row=2)
   fname = lonarr(3)
   fname[0] = cw_field(base2a, title='Image 1', value='', xsize=75, /RETURN_EVENTS)
   fname[1] = cw_field(base2a, title='Image 2', value='', xsize=75, /RETURN_EVENTS)
   
   base2d = widget_base(base2, row=2)
   max = lonarr(3)
   max[0] = cw_field(base2d, title='Max', value=0, xsize=10, ysize=1, /RETURN_EVENTS)
   max[1] = cw_field(base2d, title='Max', value=0, xsize=10, ysize=1, /RETURN_EVENTS)
   space = '               '
   max[2] = cw_field(base1, title=space+'Max Difference', value=0, xsize=10, ysize=1, /RETURN_EVENTS)

   slice = 1
   slider = widget_slider(base, value=slice, min=1, max=64, xsize=910)
   
   base2e = widget_base(base, column=3)
   display = cw_bgroup(base2e, ['Difference Alone', 'Diff on 1','Diff on 2','2 on 1'], $
                       /ROW, /EXCLUSIVE, set_value=0)
   base2f = widget_base(base2e, row=1)
   up = widget_button(base2f, value='<- Up  ')
   mid = widget_button(base2f, value=' Middle ')
   down = widget_button(base2f, value=' Down ->')  
   smooth = cw_bgroup(base2e, ['Difference Unfiltered','Smoothed'], $
                      /ROW, /EXCLUSIVE, set_value=0)

   msg = widget_text(base)
   
   wid = {WIDGETS,		$
          base     : base,	$ ; top level base
          file     : file,	$ ; file selection button id
          orient   : orient,    $ ; image orientation selection button id
          zoom     : zoom,	$ ; magnification selection button id
          scale    : scale,	$ ; scaling selection button id
          diff     : diff,	$ ; %diff scaling selection button id
          cut      : cut,	$ ; %cutoff selection button id
          smooth   : smooth,    $ ; smooth selection button id
          up       : up,	$ ; up button
          mid      : mid,	$ ; mid button
          down     : down,	$ ; down button
          exit     : exit,	$ ; exit selection button id
          fname    : fname,	$ ; filename entry field id
          max      : max,	$ ; maximum value displayed
          slider   : slider,    $ ; slice selection slider id
          display  : display,   $ ; image number selection buttons id
          msg      : msg,	$ ; message field id
          win      : lonarr(3) 	$ ; draw window widget id
         }

   dsp[0].cut =  50             ; default cutoff for combined display
   dsp[0].set = 0               ; difference image alone (not superimposed)
   dsp[2].set = 0               ; difference image unfiltered
   FOR i=0, 1 DO dsp[i].range = 0 ; positive
   FOR i=0, 1 DO dsp[i].min = 0
   dsp[2].range = 2             ; pos+neg
   orient = 0L

   img = {VIMAGE}
   uv = {wid:wid, img:[img,img,img]}
   FOR i= 0, 2 DO BEGIN
      uv.img[i].f = 1
      uv.img[i].p = 20
      uv.img[i].ff = 1
      uv.img[i].lf = 1
      uv.img[i].fp = 1
      uv.img[i].lp = 512
   ENDFOR

   FOR i= 0, 1 DO BEGIN
      image = uv.img[i]
      image.fname = inputfile[i]
      msg = ''
      IF (getframe(image, msg) NE 0) THEN BEGIN
         print,msg
         uv.img[i].fname = ''
      ENDIF ELSE BEGIN
         uv.img[i] = image
         print,'File '+uv.img[i].fname+' read'
         widget_control, uv.wid.fname[i], set_value=uv.img[i].fname
      ENDELSE
   ENDFOR
   maxval = max(*uv.img[0].data)
   FOR i=0, 1 DO dsp[i].max = maxval
   dsp[2].max = maxval * percent / 100
   dsp[2].min = -dsp[2].max

   scrnsize = get_screen_size()
   default_zoom = fix(scrnsize[0]/(3*uv.img[0].dim[0]))
   FOR i=0, 2 DO dsp[i].zoom = default_zoom

   FOR i=0,3 DO uv.img[2].dim[i] = uv.img[0].dim[i]
   uv.img[2].fname = "diff"

   data = (*uv.img[1].data[*,*,*,*]) - (*uv.img[0].data[*,*,*,*])
   uv.img[2].data = ptr_new(data)

   ff = uv.img[0].ff
   lf = uv.img[0].lf
   fp = uv.img[0].fp
   lp = uv.img[0].lp
   uv.img[0].p = slice
   uv.img[1].p = slice
   uv.img[2].p = slice
   widget_control, uv.wid.slider, set_slider_min=fp
   widget_control, uv.wid.slider, set_slider_max=lp
   widget_control, uv.wid.slider, set_value=slice

   nc = (!d.n_colors-1)/2
   dsp[0].bot = 0
   dsp[0].nc = nc
   dsp[1].bot = 0
   dsp[1].nc = nc
   dsp[2].bot = nc
   dsp[2].nc = nc
   loadct, inputlut, ncolors=dsp[0].nc, bottom=dsp[0].bot, file=colortable
   loadct, 37, ncolors=dsp[2].nc, bottom=dsp[2].bot, file=colortable

   widget_control, display, set_value=0
   widget_control, base, set_uvalue=uv
   widget_control, base, /realize, xoffset=0, yoffset=0
   widget_control, uv.wid.msg, set_value='Select Middle to Display All Images'
   xmanager, "VIDIF", base, EVENT_HANDLER='VIDIF_EVENT'
END 

