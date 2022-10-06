PRO GRIDLOCK_EVENT, ev
   COMMON IMAGE_UTILS
   COMMON GRIDLOCK
   
   catch, error_status
   IF (error_status NE 0) THEN BEGIN
      ok = widget_message(!err_string)
      RETURN
   END
   widget_control, ev.top, get_uvalue=uv
   ref = float(reform((*uv.img[0].data)[*,*]))
   study = float(reform((*uv.img[1].data)[*,*]))
   new = float(reform((*uv.img[2].data)[*,*]))
   nx = (size(ref))[1]
   ny = (size(ref))[2]
   zoom = dsp[0].zoom
   xpdim = uv.img[0].pdim[0]
   ypdim = uv.img[0].pdim[1]
   IF (xpdim GT ypdim) THEN BEGIN
      xfact = xpdim/ypdim
   ENDIF ELSE BEGIN
      xfact = 1.
   ENDELSE
   ref_id = dsp[0].id
   std_id = dsp[1].id

   CASE ev.id OF

      uv.wid.addpts: BEGIN
         define_coords,uv
         str = string(format='("Number of Matched Grid Points = ",i)', npt)
         widget_control, uv.wid.msg, set_value=str
      ENDCASE 

      uv.wid.delpts: BEGIN
         delete_coords,uv
         str = string(format='("Number of Matched Grid Points = ",i)', npt)
         widget_control, uv.wid.msg, set_value=str
      ENDCASE 

      uv.wid.refresh: BEGIN
         show_coords,uv
      ENDCASE 

      uv.wid.warp: BEGIN
         IF (npt LT 4) THEN BEGIN
            str = "Need at least 4 Matched Points to Warp"
            widget_control, uv.wid.msg, set_value=str
         ENDIF ELSE BEGIN
            xin = xref[0:npt-1]
            yin = ny-yref[0:npt-1]
            xout = xstd[0:npt-1]
            yout = ny-ystd[0:npt-1]
            new = warp_tri(xin,yin,xout,yout,study,/quintic)
            dif = ref-new
            (*uv.img[2].data) = new
            (*uv.img[3].data) = dif
            print,format='("Mean Square Error =",F14.2)',mean(dif*dif)
            show_coords,uv
         ENDELSE
      ENDCASE 

      uv.wid.savimg: BEGIN
         temp = ev.top
         fname = ''
         savefile = widget_base(group_leader=uv.wid.base)
         text = cw_field(savefile, title='Filename: ', value=fname, $
                         xsize=80, /STRING, /RETURN_EVENTS)
         widget_control, savefile, xoffset=0, yoffset=0, /realize
         ev = WIDGET_EVENT(savefile)
         widget_control, savefile, /DESTROY
         fname = ev.value
         ev.top = temp
         IF (strtrim(fname EQ '')) THEN BEGIN
            msg = 'NO FILENAME entered'
            result=dialog_message(msg, title='Image was NOT SAVED!', /error)
         ENDIF ELSE BEGIN
            msg = ' '
            stat = putinterfile(uv.img[2], fname, msg)
            widget_control, uv.wid.msg, set_value=msg
         ENDELSE
      ENDCASE 

      uv.wid.exit: BEGIN
         FOR i=0, 3 DO IF (uv.img[i].data NE ptr_new()) THEN ptr_free, uv.img[i].data
         widget_control, ev.top, /destroy
         exit
      ENDCASE 

      ELSE:
   ENDCASE
END


PRO GRIDLOCK
@IMAGE_UTILS.h
@GRIDLOCK.h

   on_error, 2

   ;;file1 = 'e1_g2.img'
   ;;file2 = 'e2_g2.img'
   ;;file1 = 'p5121ho1_p24n.img'
   ;;file2 = 'p5133ho1_p24n.img'
   file1 = 'p2808_seg.img'
   file2 = 'p2817_segb.img'
   ;;file1 = getenv('FILE1')
   ;;file2 = getenv('FILE2')
   fname = [file1,file2,file2,file2]
   dsp1 = 'Reference'
   dsp2 = 'Study'
   dsp3 = 'Warped Study'
   dsp4 = 'Reference-Warped Study'
   dname = [dsp1,dsp2,dsp3,dsp4]

   base = widget_base (TITLE='GRIDLOCK', /COLUMN)
   base1 = widget_base(base, row=1)
   addpts  = widget_button(base1, value=' Add  ')
   delpts  = widget_button(base1, value='Delete')
   refresh = widget_button(base1, value='Refresh')
   warp    = widget_button(base1, value=' Warp ')
   savimg  = widget_button(base1, value='Save Image')
   exit    = widget_button(base1, value=' Exit ')
   msg = WIDGET_TEXT(base1, xsize=70, ysize=1)
   
   wid = {WIDGETS,		$
          base     : base,		$ ; top level base
          win      : lonarr(4),	$ ; draw window widget id
          addpts   : addpts,	$ ; add coordinate pairs
          delpts   : delpts,	$ ; delete coordinate pairs
          refresh  : refresh,	$ ; save coordinate pairs
          warp     : warp,		$ ; warp to coordinates
          savimg   : savimg,	$ ; save warped image
          exit     : exit,		$ ; exit selection button id
          msg      : msg		$ ; message field
         }

   img = {VIMAGE}
   uv = {wid:wid, img:[img,img,img,img]}
   FOR i= 0, 3 DO BEGIN
      uv.img[i].f = 1
      uv.img[i].p = 1
      uv.img[i].fp = 1
      uv.img[i].lp = 1
      uv.img[i].ff = 1
      uv.img[i].lf = 1
      uv.img[i].fname = fname[i]
      image = uv.img[i]
      str = ''
      IF (getframe(image, str) NE 0) THEN BEGIN
         print, str,i+1
         uv.img[i].fname = ''
         RETURN
      ENDIF
      uv.img[i] = image
   ENDFOR
   ref = float(reform((*uv.img[0].data)[*,*]))
   study = float(reform((*uv.img[1].data)[*,*]))
   dif = ref-study
   (*uv.img[3].data) = dif

   zoom = 2
   xdim = uv.img[0].dim[0]
   ydim = uv.img[0].dim[1]
   xpdim = uv.img[0].pdim[0]
   ypdim = uv.img[0].pdim[1]
   xsize = xdim * zoom
   ysize = ydim * zoom
   IF (xpdim GT ypdim) THEN BEGIN
      xfact = xpdim/ypdim
   ENDIF ELSE BEGIN
      xfact = 1.
   ENDELSE
   dsp[0].set = 1
   dsp[0].cut =  50
   IF (!d.n_colors GE 160) THEN nc=80 $
   ELSE IF (!d.n_colors GE 80) THEN nc=40 $
   ELSE nc=(!d.n_colors)/2
   FOR i= 0, 3 DO BEGIN
      dsp[i].set = 1
      dsp[i].init = 1
      dsp[i].type = 0
      dsp[i].zoom = zoom
      dsp[i].xsize = xsize
      dsp[i].ysize = ysize
      dsp[i].nc = nc
      dsp[i].type = 1
      dsp[i].max = 800
   ENDFOR
   dsp[3].type = 1
   dsp[3].bot = nc
   dsp[3].range = 2
   dsp[3].min = -1000
   dsp[3].max = 1000

   !p.background = 0
   loadct, 36, ncolors=dsp[0].nc, bottom=dsp[0].bot, file=lut
   loadct, 37, ncolors=dsp[3].nc, bottom=dsp[3].bot, file=lut

   widget_control, base, set_uvalue=uv
   widget_control, base, /realize, xoffset=0, yoffset=ysize+30

   yloc = 0
   FOR i= 0, 3 DO BEGIN
      dispbase = widget_base(TITLE=dname[i], group_leader=uv.wid.base, uvalue=i)
      win = widget_draw(dispbase, xsize=dsp[i].xsize, ysize=dsp[i].ysize, /button, /motion)
      uv.wid.win[i] = win
      xloc = i*(xfact*xsize + 10) < i*325
      widget_control, dispbase, /realize, xoffset=xloc, yoffset=yloc
      widget_control, win, get_value=windowIndex
      wset, windowIndex
      dsp[i].id = windowIndex
   ENDFOR
   widget_control, base, set_uvalue=uv
   transform_display, uv
   print,format='("Mean Square Error =",F14.2)',mean(dif*dif)
   str = string(format='("Points: ",i)', npt)
   widget_control, uv.wid.msg, set_value=str
   xmanager, "GRIDLOCK", base, EVENT_HANDLER='GRIDLOCK_EVENT'

END 
