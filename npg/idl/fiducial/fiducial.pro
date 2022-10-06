PRO FIDDISP1_EVENT, ev          ; Transverse
   COMMON IMAGE_UTILS
   COMMON FID
   widget_control, ev.top, get_uvalue=top
   widget_control, top, get_uvalue=uv
   x = ev.x / dsp[0].zoom + 1
   y = uv.img[0].dim[1] - (ev.y + 1)/ dsp[0].zoom + 1
   widget_control, uv.wid.msg, set_value=string(x, y, format='("X = ",i,"  Y = ",i)')
   IF (ev.press) THEN BEGIN
      uv.img[0].f = y
      uv.img[1].f = uv.img[0].f
      widget_control, uv.wid.slider2, set_value=uv.img[0].f
      widget_control, top, set_uvalue=uv
      FID_DISPLAY, uv
   ENDIF
END

PRO FIDDISP2_EVENT, ev          ; Coronal
   COMMON IMAGE_UTILS
   COMMON FID
   widget_control, ev.top, get_uvalue=top
   widget_control, top, get_uvalue=uv
   x = ev.x / dsp[0].zoom + 1
   z = uv.img[1].dim[2] - (ev.y + 1)/ dsp[0].zoom + 1
   widget_control, uv.wid.msg, set_value=string(x, z, format='("X = ",i,"  Z = ",i)')
   IF (ev.press) THEN BEGIN
      uv.img[0].p = z
      uv.img[1].p = uv.img[0].p
      widget_control, uv.wid.slider1, set_value=uv.img[0].p
      widget_control, top, set_uvalue=uv
      FID_DISPLAY, uv
   ENDIF
END

PRO FID_EVENT, ev
   COMMON IMAGE_UTILS
   COMMON FID
   
   catch, error_status
   IF (error_status NE 0) THEN BEGIN
      ok = widget_message(!err_string)
      RETURN
   END
   setuv = 0
   newrange = 0
   redisplay = 0
   xloc2 = 100
   yloc2 = 70
   widget_control, ev.top, get_uvalue=uv
   widget_control, uv.wid.msg, set_value=''
   
   i = 0
   
   CASE ev.id OF
      
      uv.wid.file: BEGIN
         k = ev.value - 1
         i = k < 1              ; set i to 0 or 1
         uv.img[i].ff = 1
         uv.img[i].lf = 256
         uv.img[i].fp = 1
         uv.img[i].lp = 512
         stat = 0

         CASE k OF
            0: BEGIN
               fname = 'rd*_tra_*.img'
               fname = dialog_pickfile(FILE=uv.img[i].fname, FILTER=fname, /READ)
               IF (strlen(fname) EQ 0) THEN BEGIN
                  widget_control, uv.wid.msg, set_value='No file entered'
                  stat = 1
               ENDIF
            ENDCASE 

            1: BEGIN
               fname = 'rd*_???_*.img'
               fname = dialog_pickfile(FILE=uv.img[i].fname, FILTER=fname, /READ)
               IF (strlen(fname) EQ 0) THEN BEGIN
                  widget_control, uv.wid.msg, set_value='No file entered'
                  stat = 1
               ENDIF
            ENDCASE 
         ENDCASE 

         IF (stat EQ 0) THEN BEGIN
            image = uv.img[i]
            image.fname = fname
            msg = ''
            IF (getframe(image, msg) NE 0) THEN BEGIN
               widget_control, uv.wid.msg, set_value=msg
               uv.img[i].fname = ''
               RETURN
            ENDIF
            IF (dsp[0].max LE 100) THEN BEGIN 
               dsp[0].max = 1500
               dsp[1].max = 1500
            ENDIF
            widget_control, uv.wid.slider4, set_value=fix(dsp[0].max)
            widget_control, uv.wid.slider5, set_value=fix(dsp[1].max)
            uv.img[i] = image
            uv.img[i].lf = uv.img[i].dim[1]
            widget_control, uv.wid.msg, set_value='File '+uv.img[i].fname+' found'
            widget_control, uv.wid.fname[i], set_value=uv.img[i].fname
            CASE i OF
               0: BEGIN
                  IF (uv.img[0].p EQ 1) THEN BEGIN
                     uv.img[0].p = 100
                     uv.img[1].p = uv.img[0].p
                     widget_control, uv.wid.slider1, set_value=uv.img[0].p
                  ENDIF
               ENDCASE
               1: BEGIN
                  IF (uv.img[1].f EQ 1) THEN BEGIN
                     uv.img[1].f = (uv.img[1].lf)/2
                     uv.img[0].f = uv.img[1].f
                     widget_control, uv.wid.slider2, set_value=uv.img[1].f
                  ENDIF
               ENDCASE
            END
            newrange = 1
            setuv = 1
            redisplay = 1
         ENDIF
      ENDCASE 

      ;; -------------------------------------------------------------------------------
      uv.wid.fname[0]: BEGIN	; same as last
         i = 0
         fname = ev.value
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
            uv.img[i] = image
            uv.img[i].lf = uv.img[i].dim[1]
            widget_control, uv.wid.msg, set_value='File '+uv.img[i].fname+' found'
            widget_control, uv.wid.fname[i], set_value=uv.img[i].fname
            setuv = 1
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
            image = uv.img[i]
            image.fname = fname
            msg = ''
            IF (getframe(image, msg) NE 0) THEN BEGIN
               widget_control, uv.wid.msg, set_value=msg
               uv.img[i].fname = ''
               RETURN
            ENDIF
            uv.img[i] = image
            uv.img[i].lf = uv.img[i].dim[1]
            widget_control, uv.wid.msg, set_value='File '+uv.img[i].fname+' found'
            widget_control, uv.wid.fname[i], set_value=uv.img[i].fname
            setuv = 1
            redisplay = 1
         ENDELSE 
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.zoom: BEGIN
         dsp[0].zoom = ev.value
         redisplay = 1
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.linelen: BEGIN
         full = ev.value-1
         redisplay = 1
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.tilt: BEGIN
         FID_TILT, uv, dsp[0].zoom
         setuv = 1
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.points: BEGIN
         FID_POINTS, uv, dsp[0].zoom
         setuv = 1
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.up: BEGIN
         IF (uv.img[0].p GT uv.img[0].fp) THEN uv.img[0].p = uv.img[0].p-1
         widget_control, uv.wid.slider1, set_value=uv.img[0].p
         setuv = 1
         redisplay = 1
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.down: BEGIN
         IF (uv.img[0].p LT lp) THEN uv.img[0].p = uv.img[0].p+1
         widget_control, uv.wid.slider1, set_value=uv.img[0].p
         setuv = 1
         redisplay = 1
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.ant: BEGIN
         IF (uv.img[0].f GT uv.img[0].ff) THEN uv.img[0].f = uv.img[0].f-1
         uv.img[1].f = uv.img[0].f
         widget_control, uv.wid.slider2, set_value=uv.img[0].f
         setuv = 1
         redisplay = 1
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.post: BEGIN
         IF (uv.img[0].f LT uv.img[0].lf) THEN uv.img[0].f = uv.img[0].f+1
         uv.img[1].f = uv.img[0].f
         widget_control, uv.wid.slider2, set_value=uv.img[0].f
         setuv = 1
         redisplay = 1
      ENDCASE 

      ;; -------------------------------------------------------------------------------
      uv.wid.save: BEGIN
         fname = uv.img[0].fname
         IF (strlen(fname) GT 0) THEN BEGIN 
            l1 = rstrpos(fname,'/')
            IF (l1 GT 0) THEN fname = strmid(fname,l1+1)
            l2 = strpos(fname,'_')
            IF (l2 GT 0) THEN fname = strmid(fname,0,l2+1)+uid+'.pts' $
            ELSE fname = '*'+uid+'.pts'
         ENDIF ELSE fname = '*'+uid+'.pts'
         fname = strcompress(fname, /remove_all)
         fname = dialog_pickfile(FILE=fname, FILTER=fname, /WRITE)
         IF (strlen(fname) EQ 0) THEN BEGIN
            widget_control, uv.wid.msg, set_value='No file entered'
         ENDIF ELSE BEGIN
            k = rstrpos(fname,'.')
            IF (k GT 0) THEN fname = strmid(fname,0,k)
            fname = fname + '.pts'
            openw, listlun, fname, ERROR=err, /GET_LUN
            IF (err NE 0) THEN BEGIN
               widget_control, uv.wid.msg, set_value='Cannot open file '+fname
               listlun = 0
            ENDIF ELSE BEGIN
               fid = ['AC ','PC ','OX ','OTL','OTR','BEL','BER','PTL','PTR','RNL','RNR','LVT','RVT','IPF']
               printf, listlun, "Fiducials defined on ",uv.img[0].fname," by ",uid," ",systime(0)
               FOR n=0, 13 DO printf, listlun, fid[n],locus[*,n], format='(A3,F8.1,F8.1,F8.1)'
               close, listlun
               free_lun, listlun
               saved = 1
               FOR n=0,2  DO FOR m=0,2 DO IF locus[m,n] EQ 0 THEN saved = 2
               FOR m=0,2 DO IF locus[m,13] EQ 0 THEN saved = 2
               IF (saved EQ 1) THEN msg = string(format='(A,/,A)', "Points saved to", fname) $
               ELSE msg = string(format='(A,/,A)', "Incomplete Points saved to", fname)
               widget_control, uv.wid.msg, set_value=msg
            ENDELSE
         ENDELSE
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.get: BEGIN
         fname = '*.pts'
         fname = dialog_pickfile(FILE=fname, FILTER=fname, /WRITE)
         IF (strlen(fname) EQ 0) THEN BEGIN
            widget_control, uv.wid.msg, set_value='No file entered'
         ENDIF ELSE BEGIN
            k = rstrpos(fname,'.')
            IF (k GT 0) THEN fname = strmid(fname,0,k)
            fname = fname + '.pts'
            openr, listlun, fname, ERROR=err, /GET_LUN
            IF (err NE 0) THEN BEGIN
               widget_control, uv.wid.msg, set_value='Cannot open file '+fname
               listlun = 0
            ENDIF ELSE BEGIN
               str = ''
               nam = ''
               coord = fltarr(3)
               readf, listlun, str
               readf, listlun, str
               n = 0
               locus[*,*] = 0.0
               WHILE (NOT eof(listlun)) DO BEGIN
                  readf, listlun, nam,coord, format='(A3,F8.1,F8.1,F8.1)'
                  IF (n LT 14) THEN locus[*,n] = coord
                  n = n+1
               ENDWHILE
               close, listlun
               free_lun, listlun
               msg = string(format='(A,/,A)', "Points read from", fname)
               widget_control, uv.wid.msg, set_value=msg
            ENDELSE
         ENDELSE
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.exit: BEGIN
         IF (saved EQ 0) THEN BEGIN
            stat = DIALOG_MESSAGE("Exit without saving points?",/QUESTION)
            IF (stat EQ 'Yes') THEN BEGIN
               FOR i=0, 1 DO if (uv.img[i].data NE ptr_new()) THEN ptr_free, uv.img[i].data
               widget_control, ev.top, /destroy
            ENDIF
         ENDIF ELSE IF (saved EQ 2) THEN BEGIN
            stat = DIALOG_MESSAGE("Exit with incomplete set of fiducial points?",/QUESTION)
            IF (stat EQ 'Yes') THEN BEGIN
               FOR i=0, 1 DO if (uv.img[i].data NE ptr_new()) THEN ptr_free, uv.img[i].data
               widget_control, ev.top, /destroy
            ENDIF
         ENDIF ELSE BEGIN
            FOR i=0, 1 DO if (uv.img[i].data NE ptr_new()) THEN ptr_free, uv.img[i].data
            widget_control, ev.top, /destroy
         ENDELSE
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.slider1: BEGIN
         widget_control, uv.wid.slider1, get_value=slice
         IF (slice LE lp) THEN uv.img[0].p = slice
         setuv = 1
         redisplay = 1
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.slider2: BEGIN
         widget_control, uv.wid.slider2, get_value=plane
         IF (plane LE uv.img[0].lf) THEN uv.img[0].f = plane
         uv.img[1].f = uv.img[0].f
         setuv = 1
         redisplay = 1
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.slider3: BEGIN
         widget_control, uv.wid.slider3, get_value=tmp
         dsp[0].min = tmp
         IF (dsp[0].max LT dsp[0].min) THEN BEGIN
            dsp[0].max = dsp[0].min
            widget_control, uv.wid.slider4, set_value=fix(dsp[0].max)
         ENDIF
         IF (dsp[1].max LT dsp[0].min) THEN BEGIN
            dsp[1].max = dsp[0].min
            widget_control, uv.wid.slider4, set_value=fix(dsp[1].max)
         ENDIF
         setuv = 1
         redisplay = 1
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.slider4: BEGIN
         widget_control, uv.wid.slider4, get_value=tmp
         dsp[0].max = tmp
         IF (dsp[0].min GT dsp[0].max) THEN BEGIN
            dsp[0].min = dsp[0].max
            widget_control, uv.wid.slider3, set_value=fix(dsp[0].min)
         ENDIF
         setuv = 1
         redisplay = 1
      ENDCASE 
      
      ;; -------------------------------------------------------------------------------
      uv.wid.slider5: BEGIN
         widget_control, uv.wid.slider5, get_value=tmp
         dsp[1].max = tmp
         IF (dsp[0].min GT dsp[1].max) THEN BEGIN
            dsp[0].min = dsp[1].max
            widget_control, uv.wid.slider3, set_value=fix(dsp[0].min)
         ENDIF
         setuv = 1
         redisplay = 1
      ENDCASE 
      
      ELSE:
      
   ENDCASE
   
   ;;-----------------------------------------------------------------------------------
   IF (newrange) THEN BEGIN
      fp = uv.img[0].fp
      lp = uv.img[0].lp
      ff = 1
      lf = uv.img[0].lf
      widget_control, uv.wid.slider1, set_slider_min=fp
      widget_control, uv.wid.slider1, set_slider_max=lp
      widget_control, uv.wid.slider2, set_slider_max=lf
      widget_control, uv.wid.slider1, set_value=uv.img[0].p
   ENDIF 

   ;;-----------------------------------------------------------------------------------
   IF (redisplay) THEN BEGIN 
      IF (strlen(uv.img[0].fname) EQ 0) THEN BEGIN 
         widget_control, uv.wid.msg, set_value='No file has been entered'
         redisplay = 0
      ENDIF 
      FOR i=0, 1 DO BEGIN
         IF (uv.img[i].data NE ptr_new()) THEN BEGIN
            widget_control, uv.wid.win[i], bad_id=status
            IF (status NE 0) then dsp[i].init = 0
            
            IF (redisplay AND (dsp[i].init EQ 0)) THEN BEGIN
               IF (i eq 0) THEN dispbase = widget_base(TITLE='Transverse',group_leader=uv.wid.base,uvalue=ev.top)$
               ELSE dispbase = widget_base(TITLE='Coronal',group_leader=uv.wid.base,uvalue=ev.top)
               win = widget_draw(dispbase, xsize=dsp[i].xsize, ysize=dsp[i].ysize, /button)
               !p.background = 0
               scrnsize = get_screen_size()
               yloc = scrnsize[1]/2
               IF (yloc GT 300) THEN yloc = 300
               CASE i OF
                  0: xloc = 0
                  1: xloc = scrnsize[0]/2
               ENDCASE
               IF (xloc GT 1000) THEN xloc = 960
               widget_control, dispbase, /realize, xoffset=xloc, yoffset=yloc
               widget_control, win, get_value=windowIndex
               wset, windowIndex
               xmanager, "FIDDISP"+strcompress(string(i+1),/remove_all), dispbase
               uv.wid.win[i] = win
               dsp[i].id = windowIndex
               dsp[i].init = 1
               setuv = 1
            ENDIF 
         ENDIF 
      ENDFOR
      FID_DISPLAY, uv
      wshow, dsp[0].id
   ENDIF 

   IF (setuv) THEN widget_control, ev.top, set_uvalue=uv
END


PRO FIDUCIAL
@IMAGE_UTILS.h
@FID.h

   on_error, 2
   uid = getenv('UID')
   dmax = FIX(getenv('DMAX'))

   base = widget_base (TITLE='FIDUCIAL', /COLUMN)
   base1 = widget_base(base, row=1)
   
   select1 = ['1\Get Image','0\Transverse','0\Coronal']
   file = cw_pdmenu(base1, select1)

   select2 = ['1\Magnify','0\ 1 ','0\ 2 ','0\ 3 ','0\ 4 ']
   zoom = cw_pdmenu(base1, select2)

   select3 = ['1\Line','0\ Half','0\ Full']
   linelen = cw_pdmenu(base1, select3)

   base2 = widget_base(base1,row=1,/align_right)
   up = widget_button(base2, value='<- Up     ')
   down = widget_button(base2, value=' Down ->')  

   ant = widget_button(base2, value='<- Anterior  ')
   post = widget_button(base2, value=' Posterior ->')  

   tilt = widget_button(base1, value='Tilt')
   get = widget_button(base1, value='Get Points')
   points = widget_button(base1, value='Points')
   save = widget_button(base1, value='Save Points')

   exit = widget_button(base1, value='Exit')
   
   base3 = widget_base(base,col=2)
   slice = 1
   slider1 = widget_slider(base3, value=slice, min=1, max=47, xsize=400,title='Transverse Plane')

   plane = 1
   slider2 = widget_slider(base3, value=plane, min=1, max=100, xsize=600,title='Coronal Plane')

   base4 = widget_base(base,col=3)
   slider3 = widget_slider(base4, value=dsp[0].min, min=0, max=dmax, xsize=300,title='Min Intensity')
   slider4 = widget_slider(base4, value=dsp[0].max, min=0, max=dmax, xsize=340, $
		title='Max Intensity Transverse')
   slider5 = widget_slider(base4, value=dsp[1].max, min=0, max=dmax, xsize=340, $
		title='Max Intensity Coronal')

   base5 = widget_base(base, row=1)
   fname = lonarr(2)
   fname[0] = cw_field(base5, title='Tra', value='', xsize=50, /RETURN_EVENTS)
   fname[1] = cw_field(base5, title='Cor', value='', xsize=50, /RETURN_EVENTS)

   pnum = 0
   msg = widget_text(base)
   

   wid = {WIDGETS,		$
          base     : base,	$ ; top level base
          file     : file,	$ ; file selection button id
          zoom     : zoom,	$ ; magnification selection button id
          linelen  : linelen,	$ ; line length
          tilt     : tilt,	$ ; head tilt
          points   : points,	$ ; line length
          up       : up,	$ ; up button
          down     : down,	$ ; down button
          ant      : ant,	$ ; ant button
          post     : post,	$ ; post button
          save     : save,	$ ; save fiducial points
          get      : get,	$ ; get fiducial points
          exit     : exit,	$ ; exit selection button id
          fname    : fname,	$ ; filename field for images
          slider1  : slider1,	$ ; slice selection slider id
          slider2  : slider2,	$ ; slice selection slider id
          slider3  : slider3,	$ ; min slider id
          slider4  : slider4,	$ ; max slider id
          slider5  : slider5,	$ ; max slider id
          pnum     : pnum,	$ ; fiducial point number
          msg      : msg,	$ ; message field id
          win      : lonarr(2)	$ ; draw window widget id
         }

   FOR i= 0, 1 DO dsp.init = 0
   FOR i= 0, 1 DO dsp.set = 1
   FOR i= 0, 1 DO dsp.zoom = 2
   FOR i=0, 1 DO dsp[i].range = 0
   dsp[0].nc = 256
   dsp[0].min = 0
   dsp[0].max = 0
   dsp[1].max = 0
   dsp[0].type = 1
   loadct, 0, ncolors=dsp[0].nc, bottom=0, file="/usr/local/npg/idl/color_tables/videen.tbl"
   tvlct,255,0,0,dsp[0].nc

   img = {VIMAGE}
   uv = {wid:wid, img:[img,img]}

   ;; Use existing structure by treating f, ff, lf as coronal planes
   FOR i=0,1 DO BEGIN
      uv.img[i].f = 1
      uv.img[i].p = 1
      uv.img[i].ff = 1
      uv.img[i].lf = 256
      uv.img[i].fp = 1
      uv.img[i].lp = 512
   ENDFOR
   
   widget_control, base, set_uvalue=uv
   widget_control, base, /realize, xoffset=0, yoffset=0
   xmanager, "FIDUCIAL", base, EVENT_HANDLER='FID_EVENT'

END 
