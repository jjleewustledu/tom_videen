PRO VIDIDISP1_EVENT, ev
   COMMON IMAGE_UTILS
   COMMON VIDI
   widget_control, ev.top, get_uvalue = top
   widget_control, top, get_uvalue = uv
   i = 0
   x = ev.x / dsp[i].zoom + 1
   CASE orient OF
      0:BEGIN
         IF (uv.img[i].pdim[0] GT uv.img[i].pdim[1]) THEN BEGIN
            fact = (uv.img[i].pdim[0]) / (uv.img[i].pdim[1])
            y = (ev.y + 1) / (fact * dsp[i].zoom)
            y = uv.img[i].dim[1] - y + 1
         ENDIF ELSE IF (uv.img[i].pdim[0] LT uv.img[i].pdim[1]) THEN BEGIN
            fact = (uv.img[i].pdim[1]) / (uv.img[i].pdim[0])
            y = (ev.y + 1) / (fact * dsp[i].zoom)
            y = uv.img[i].dim[1] - y + 1
         ENDIF ELSE BEGIN
            y = ev.y / dsp[i].zoom
            y = uv.img[i].dim[1] - y
         ENDELSE
      widget_control, uv.wid.msg, set_value = string (x, y, format = '("X = ",i,"  Y = ",i)') END
      1:BEGIN
         fact = (uv.img[i].pdim[2]) / (uv.img[i].pdim[0])
         y = (ev.y + 1) / (fact * dsp[i].zoom)
         y = uv.img[i].dim[2] - y + 1
      widget_control, uv.wid.msg, set_value = string (x, y, format = '("X = ",i,"  Z = ",i)') END
      2:BEGIN
         fact = (uv.img[i].pdim[2]) / (uv.img[i].pdim[1])
         y = (ev.y + 1) / (fact * dsp[i].zoom)
         y = uv.img[i].dim[2] - y + 1
      widget_control, uv.wid.msg, set_value = string (x, y, format = '("Y = ",i,"  Z = ",i)') END
   ENDCASE
END

PRO VIDIDISP2_EVENT, ev
   COMMON IMAGE_UTILS
   COMMON VIDI
   widget_control, ev.top, get_uvalue = top
   widget_control, top, get_uvalue = uv
   i = 1
   x = ev.x / dsp[i].zoom + 1
   CASE orient OF
      0:BEGIN
         IF (uv.img[i].pdim[0] GT uv.img[i].pdim[1])
         THEN            BEGIN
            fact = (uv.img[i].pdim[0]) / (uv.img[i].pdim[1])
            y = (ev.y + 1) / (fact * dsp[i].zoom)
            y = uv.img[i].dim[1] - y + 1
         ENDIF ELSE IF (uv.img[i].pdim[0] LT uv.img[i].pdim[1]) THEN BEGIN
            fact = (uv.img[i].pdim[1]) / (uv.img[i].pdim[0])
            y = (ev.y + 1) / (fact * dsp[i].zoom)
            y = uv.img[i].dim[1] - y + 1
         ENDIF ELSE BEGIN
            y = ev.y / dsp[i].zoom
            y = uv.img[i].dim[1] - y
         ENDELSE
      widget_control, uv.wid.msg, set_value = string (x, y, format = '("X = ",i,"  Y = ",i)') END
      1:BEGIN
         fact = (uv.img[i].pdim[2]) / (uv.img[i].pdim[0])
         y = (ev.y + 1) / (fact * dsp[i].zoom)
         y = uv.img[i].dim[2] - y + 1
      widget_control, uv.wid.msg, set_value = string (x, y, format = '("X = ",i,"  Z = ",i)') END
      2:BEGIN
         fact = (uv.img[i].pdim[2]) / (uv.img[i].pdim[1])
         y = (ev.y + 1) / (fact * dsp[i].zoom)
         y = uv.img[i].dim[2] - y + 1
      widget_control, uv.wid.msg, set_value = string (x, y, format = '("Y = ",i,"  Z = ",i)') END
   ENDCASE
END

PRO VIDIDISP3_EVENT, ev
   COMMON IMAGE_UTILS
   COMMON VIDI
   widget_control, ev.top, get_uvalue = top
   widget_control, top, get_uvalue = uv
   i = 2
   x = ev.x / dsp[i].zoom + 1
   CASE orient OF
      0:BEGIN
         IF (uv.img[0].pdim[0] GT uv.img[0].pdim[1])
         THEN            BEGIN
            fact = (uv.img[0].pdim[0]) / (uv.img[0].pdim[1])
            y = (ev.y + 1) / (fact * dsp[i].zoom)
            y = uv.img[0].dim[1] - y + 1
         ENDIF ELSE IF (uv.img[0].pdim[0] LT uv.img[0].pdim[1]) THEN BEGIN
            fact = (uv.img[0].pdim[1]) / (uv.img[0].pdim[0])
            y = (ev.y + 1) / (fact * dsp[i].zoom)
            y = uv.img[0].dim[1] - y + 1
         ENDIF ELSE BEGIN
            y = ev.y / dsp[i].zoom
            y = uv.img[0].dim[1] - y
         ENDELSE
      widget_control, uv.wid.msg, set_value = string (x, y, format = '("X = ",i,"  Y = ",i)') END
      1:BEGIN
         fact = (uv.img[0].pdim[2]) / (uv.img[0].pdim[0])
         y = (ev.y + 1) / (fact * dsp[i].zoom)
         y = uv.img[0].dim[2] - y + 1
      widget_control, uv.wid.msg, set_value = string (x, y, format = '("X = ",i,"  Z = ",i)') END
      2:BEGIN
         fact = (uv.img[0].pdim[2]) / (uv.img[0].pdim[1])
         y = (ev.y + 1) / (fact * dsp[i].zoom)
         y = uv.img[0].dim[2] - y + 1
      widget_control, uv.wid.msg, set_value = string (x, y, format = '("Y = ",i,"  Z = ",i)') END
   ENDCASE
END

PRO VIDI_EVENT, ev
   COMMON IMAGE_UTILS
   COMMON VIDI

   catch, error_status
   IF (error_status NE 0)
   THEN            BEGIN
      ok = widget_message (!err_string)
      RETURN
   END
   setuv = 0
   newrange = 0
   redisplay = 0
   xloc2 = 100
   yloc2 = 70
   widget_control, ev.top, get_uvalue = uv
   widget_control, uv.wid.msg, set_value = ''

   nimg = 1                     ;
   max index for   images
   nwin = 2                     ; max index for display windows
   CASE ev.id OF

      uv.wid.file:BEGIN

         IF (ev.value LT 7)
         THEN            BEGIN
            i = 0
            rtype = ev.value - 1
         ENDIF ELSE BEGIN
            i = 1
            rtype = ev.value - 7
         ENDELSE

         CASE rtype OF

            1:BEGIN             ;
               First           Frame, All Planes
               uv.img[i].ff = 1
               uv.img[i].lf = 1
               uv.img[i].fp = 1
               uv.img[i].lp = 512
               fmode = 0
            ENDCASE

            2:BEGIN             ;
               One             Frame, All Planes
               temp = ev.top
               planeselect = widget_base (group_leader = uv.wid.base)
               text = cw_field (planeselect, title = 'Frame: ', value = uv.img[i].ff, $
                                xsize = 4, /INTEGER, /RETURN_EVENTS)
               widget_control, planeselect, /realize, xoffset = xloc2, yoffset = yloc2
               ev = WIDGET_EVENT (planeselect) widget_control, planeselect, /DESTROY
               uv.img[i].ff = ev.value
               uv.img[i].lf = ev.value
               uv.img[i].fp = 1
               uv.img[i].lp = 512
               fmode = 0
               ev.top = temp
            ENDCASE

            3:BEGIN             ;
               One             Frame, Select Planes
               temp = ev.top
               planeselect = widget_base (group_leader = uv.wid.base)
               text = cw_field (planeselect, title = 'Frame: ', value = uv.img[i].ff, $
                                xsize = 4, /INTEGER, /RETURN_EVENTS)
               widget_control, planeselect, /realize, xoffset = xloc2, yoffset = yloc2
               ev = WIDGET_EVENT (planeselect) widget_control, planeselect, /DESTROY
               uv.img[i].ff = ev.value
               uv.img[i].lf = ev.value
               planeselect = widget_base (group_leader = uv.wid.base) text = cw_field (planeselect, title = 'First Plane: ', value = uv.img[i].ff, $
               xsize = 4, /INTEGER, /RETURN_EVENTS)
               widget_control, planeselect, /realize, xoffset = xloc2, yoffset = yloc2
               ev = WIDGET_EVENT (planeselect) widget_control, planeselect, /DESTROY
               uv.img[i].fp = ev.value
               planeselect = widget_base (group_leader = uv.wid.base) text = cw_field (planeselect, title = 'Last Plane: ', value = uv.img[i].ff, $
                                                                                       xsize = 4, /INTEGER, /RETURN_EVENTS)
               widget_control, planeselect, /realize, xoffset = xloc2, yoffset = yloc2
               ev = WIDGET_EVENT (planeselect) widget_control, planeselect, /DESTROY
               uv.img[i].lp = ev.value
               fmode = 0
               ev.top = temp
            ENDCASE

            4:BEGIN             ;
               All             Frames, One Plane
               temp = ev.top
               uv.img[i].ff = 1
               uv.img[i].lf = 256
               if (uv.img[i].fp LE 0)
               then uv.img[i].fp = 1
               planeselect = widget_base (group_leader = uv.wid.base)
               text = cw_field (planeselect, title = 'Plane: ', value = uv.img[i].fp, xsize = 4, $
                                / INTEGER, /RETURN_EVENTS)
               widget_control, planeselect, /realize, xoffset = xloc2, yoffset = yloc2
               ev = WIDGET_EVENT (planeselect)
               widget_control, planeselect, /DESTROY
               uv.img[i].fp = ev.value
               uv.img[i].lp = ev.value
               fmode = 1
               ev.top = temp
               orient = 0       ;
               transverse orientation only for multiple frames
            ENDCASE

            5:BEGIN             ; All Frames, All Planes
               temp = ev.top
               uv.img[i]
               .ff = 1
               uv.img[i].lf = 256
               uv.img[i].fp = 1
               uv.img[i].lp = 512
               fmode = 2
               ev.top = temp
               orient = 0       ;
               transverse orientation only for multiple frames
            ENDCASE

         ENDCASE

         fname = ''
         IF (default_path EQ 0)
         THEN BEGIN
            IF (strlen (uv.img[i].fname) GT 0) THEN fname = uv.img[i].fname $
            ELSE IF (strlen (uv.img[0].fname) GT 0) THEN fname = uv.img[0].fname $
            ELSE IF (strlen (uv.img[1].fname) GT 0) THEN fname = uv.img[1].fname
         ENDIF ELSE IF (default_path EQ 1) THEN BEGIN
            IF (strlen (uv.img[0].fname) GT 0) THEN fname = uv.img[0].fname
         ENDIF ELSE IF (default_path EQ 2) THEN BEGIN
            IF (strlen (uv.img[1].fname) GT 0) THEN fname = uv.img[1].fname
         ENDIF ELSE BEGIN
            fname = local_path + "*"
         ENDELSE

         IF (strlen (fname) GT 0) THEN BEGIN
            l1 = rstrpos (fname, '/')
            l2 = rstrpos (fname, '.')
            IF (l2 GT 0) THEN BEGIN
               str1 = string (replicate (32 b, (l2 - l1 - 2)))
               str1 = '*' + str1
               strput,         fname, str1, l1 + 1
               fname = strcompress (fname, /remove_all)
            ENDIF ELSE      fname = '*'
         ENDIF ELSE fname = '*'

         fname = dialog_pickfile (FILE = uv.img[i].fname, FILTER = fname, /READ)
         IF (strlen (fname) EQ 0) THEN BEGIN
            widget_control, uv.wid.msg, set_value = 'No file entered'
         ENDIF ELSE BEGIN
            image = uv.img[i]
            image.fname = fname
            msg = ''
            IF (getframe (image, msg) NE 0)
            THEN            BEGIN
               widget_control, uv.wid.msg, set_value = msg
               uv.img[i].fname = ''
               RETURN
            ENDIF
            tmp = uv.img[i].p
            uv.img[i] = image
            IF (orient EQ 0) THEN BEGIN ;
               keep same plane if in range
               uv.img[i]
               .               p = tmp > uv.img[i].fp
               uv.img[i].p = tmp < uv.img[i].lp
            ENDIF ELSE uv.img[i].p = tmp
            widget_control, uv.wid.msg, set_value = 'File ' + uv.img[i].fname + ' found'
            widget_control, uv.wid.fname[i], set_value = uv.img[i].fname
            uv.img[i].proc = 0
            setuv = 1
            newrange = 1
            IF (dsp[i].set) THEN redisplay = 1
         ENDELSE

      ENDCASE

      ;;----------------------------------------------------------------
      uv.wid.fname[0]:BEGIN     ;
         same as         last
         i = 0
         fname = ev.value
         IF (strlen (fname) EQ 0) THEN BEGIN
            widget_control, uv.wid.msg, set_value = 'No file entered'
         ENDIF ELSE BEGIN
            uv.img[i].fp = 1
            uv.img[i].lp = 512
            image = uv.img[i]
            image.fname = fname
            msg = ''
            IF (getframe (image, msg) NE 0)
            THEN            BEGIN
               widget_control, uv.wid.msg, set_value = msg
               uv.img[i].fname = ''
               RETURN
            ENDIF
            tmp = uv.img[i].p
            uv.img[i] = image
            IF (orient EQ 0) THEN BEGIN ;
               keep same plane if in range
               uv.img[i]
               .               p = tmp > uv.img[i].fp
               uv.img[i].p = tmp < uv.img[i].lp
            ENDIF ELSE uv.img[i].p = tmp
            widget_control, uv.wid.msg, set_value = 'File ' + uv.img[i].fname + ' found'
            widget_control, uv.wid.fname[i], set_value = uv.img[i].fname
            uv.img[i].proc = 0
            setuv = 1
            newrange = 1
            IF (dsp[i].set) THEN redisplay = 1
         ENDELSE
      ENDCASE

      ;;----------------------------------------------------------------
      uv.wid.fname[1]:BEGIN     ;
         same as         last
         i = 1
         fname = ev.value
         IF (strlen (fname) EQ 0) THEN BEGIN
            widget_control, uv.wid.msg, set_value = 'No file entered'
         ENDIF ELSE BEGIN
            uv.img[i].fp = 1
            uv.img[i].lp = 512
            image = uv.img[i]
            image.fname = fname
            msg = ''
            IF (getframe (image, msg) NE 0)
            THEN            BEGIN
               widget_control, uv.wid.msg, set_value = msg
               uv.img[i].fname = ''
               RETURN
            ENDIF
            tmp = uv.img[i].p
            uv.img[i] = image
            IF (orient EQ 0) THEN BEGIN ;
               keep same plane if in range
               uv.img[i]
               .               p = tmp > uv.img[i].fp
               uv.img[i].p = tmp < uv.img[i].lp
            ENDIF ELSE uv.img[i].p = tmp
            widget_control, uv.wid.msg, set_value = 'File ' + uv.img[i].fname + ' found'
            widget_control, uv.wid.fname[i], set_value = uv.img[i].fname
            uv.img[i].proc = 0
            setuv = 1
            newrange = 1
            IF (dsp[i].set) THEN redisplay = 1
         ENDELSE
      ENDCASE

      ;;----------------------------------------------------------------
      uv.wid.viewer:VIDI_VIEWER, uv

      ;;----------------------------------------------------------------
      uv.wid.orient:BEGIN
         CASE ev.value OF
            'Transverse':orient = 0
            'Coronal':BEGIN
               IF (uv.img[0].dim[2] GT 1)
               THEN            orient = 1 $
               ELSE BEGIN
                  msg = 'Transverse orientation only for multiple planes'
                  widget_control, uv.wid.msg, set_value = msg
               ENDELSE
            ENDCASE
            'Sagittal':BEGIN
               IF (uv.img[0].dim[2] GT 1) THEN orient = 2 $
               ELSE BEGIN
                  msg = 'Transverse orientation only for multiple planes'
                  widget_control, uv.wid.msg, set_value = msg
               ENDELSE
            ENDCASE
         ENDCASE
         setuv = 1
         newrange = 1
         redisplay = 1
      ENDCASE

      ;;----------------------------------------------------------------
      uv.wid.zoom:BEGIN
         FOR i = 0, nwin DO dsp[i].zoom = ev.value
         redisplay = 1
      ENDCASE

      ;;
      -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
      uv.wid.color:BEGIN
         CASE ev.value OF
            1:IF (dsp[0].nc EQ 0)
            THEN            widget_control, uv.wid.msg, set_value = 'NO IMAGE displayed' $
            ELSE xloadct,   group = uv.wid.base, ncolors = dsp[0].nc, bottom = dsp[0].bot, $
               file = "/usr/local/npg/idl/color_tables/videen.tbl"

            2:IF (dsp[1].nc EQ 0) THEN widget_control, uv.wid.msg, set_value = 'NO IMAGE displayed' $
            ELSE xloadct, group = uv.wid.base, ncolors = dsp[1].nc, bottom = dsp[1].bot, $
               file = "/usr/local/npg/idl/color_tables/videen.tbl"
         ENDCASE
      ENDCASE

      ;;
      -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
      uv.wid.roi:BEGIN
         otemp = orient
         inum = ev.value - 1
         IF (orient GT 0)
         THEN            BEGIN
            msg = 'VOIs require TRANSVERSE slices'
            widget_control, uv.wid.msg, set_value = msg
         ENDIF ELSE BEGIN
            IF (uv.wid.win[inum] GT 0) THEN BEGIN
               VIDI_DEFVOI,    uv, inum, otemp, dsp[inum].zoom
               setuv = 1
            ENDIF ELSE widget_control, uv.wid.msg, set_value = 'NO IMAGE displayed for VOI'
         ENDELSE

                                ;
      ENDIF ELSE CASE ev.value OF
                                ;
         1:BEGIN
;
            IF (uv.wid.win[0] GT 0)
            THEN            BEGIN
                                ;
               tempthresh = uv.wid.vthresh
;
               VIDI_DEFVOI, uv.wid.voi, uv.img[0], uv.cbf[0], uv.cbv[0], otemp, tempthresh, $
;
               uv.wid.win[0], uv.wid.base, zoom = dsp[0].zoom
;
               uv.wid.vthresh = tempthresh
;
               setuv = 1
;
            ENDIF ELSE      widget_control, uv.wid.msg, set_value = 'NO IMAGE displayed for VOI'
                                ;
         ENDCASE
;
         2:BEGIN
;
            IF (uv.wid.win[1] GT 0)
            THEN            BEGIN
                                ;
               tempthresh = uv.wid.vthresh
;
               VIDI_DEFVOI, uv.wid.voi, uv.img[1], uv.cbf[1], uv.cbv[1], otemp, tempthresh, $
;
               uv.wid.win[1], uv.wid.base, zoom = dsp[1].zoom
;
               uv.wid.vthresh = tempthresh
;
               setuv = 1
;
            ENDIF ELSE      widget_control, uv.wid.msg, set_value = 'NO IMAGE displayed for VOI'
                                ;
         ENDCASE
;
      ENDCASE
   ENDCASE

   ;;
   -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
   uv.wid.scale:BEGIN
      VIDI_SCALE, uv.wid.base
      redisplay = 1
   ENDCASE

   ;;
   -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
   uv.wid.process:BEGIN
      IF (ev.value LT 9)
      THEN            BEGIN
         i = 0
         rtype = ev.value - 2
      ENDIF ELSE BEGIN
         i = 1
         rtype = ev.value - 10
      ENDELSE
      image = uv.img[i]
      oldtype = image.proc
      image.proc = rtype
      IF ((oldtype EQ 3 AND image.proc EQ 4) OR (oldtype EQ 4 AND image.proc EQ 3)) THEN BEGIN
         widget_control, uv.wid.msg, set_value = 'Using existing metabolic factors'
         uv.img[i].proc = rtype
      ENDIF ELSE BEGIN
         IF (gethdr (image, msg) NE 0)
         THEN            BEGIN
            widget_control, uv.wid.msg, set_value = msg
            uv.img[i].proc = 0
         ENDIF ELSE BEGIN
            widget_control, uv.wid.msg, set_value = 'Metabolic factors read'
            uv.img[i] = image
         ENDELSE

         IF (rtype EQ 3 OR rtype EQ 4 OR rtype EQ 6) THEN BEGIN ;
            Oxygen and MTT models require CBF image
            uv.cbf[i].ff = 1
            uv.cbf[i].lf = 1
            uv.cbf[i].fp = 1
            uv.cbf[i].lp = 512
            uv.cbf[i].proc = 1
            fmode = 0
            fname = ''
            fname = uv.img[i].fname
            IF (strlen (fname) GT 0) THEN BEGIN
               l1 = rstrpos (fname, '/') IF (l1 GT 0) THEN strput, fname, '*', l1 + 1
               IF (rtype EQ 3 OR rtype EQ 4)
               THEN            BEGIN
                  l1 = rstrpos (fname, 'oo')
                  IF (l1 GT 0) THEN strput, fname, 'ho', l1
               ENDIF ELSE BEGIN
                  l1 = rstrpos (fname, 'oc') IF (l1 GT 0)
                  THEN            strput, fname, 'ho', l1
               ENDELSE
            ENDIF ELSE fname = '*ho*v'
            fname = dialog_pickfile (FILE = uv.cbf[i].fname, FILTER = fname, /READ, $
                                     TITLE = 'Select HO (CBF) Image') IF (strlen (fname) EQ 0) THEN BEGIN
               widget_control, uv.wid.msg, set_value = 'NO FILE entered'
               uv.img[i].proc = 0
               rtype = 0
            ENDIF ELSE BEGIN
               image = uv.cbf[i]
               image.fname = fname
               image.fname = fname
               msg = ''
               IF (getframe (image, msg) NE 0)
               THEN            BEGIN
                  widget_control, uv.wid.msg, set_value = msg
                  uv.cbf[i].fname = ''
                  RETURN
               ENDIF
               uv.cbf[i] = image
               widget_control, uv.wid.msg, set_value = 'File ' + uv.cbf[i].fname + ' found'
               image = uv.cbf[i]
               IF (gethdr (image, msg) NE 0) THEN BEGIN
                  widget_control, uv.wid.msg, set_value = msg
                  uv.img[i].proc = 0
                  rtype = 0
               ENDIF ELSE widget_control, uv.wid.msg, set_value = 'Metabolic factors read'
               uv.cbf[i] = image
            ENDELSE
         ENDIF

         IF (rtype EQ 3 OR rtype EQ 4)
         THEN            BEGIN  ;
            Oxygen models require CBV image
            uv.cbv[i].ff = 1
            uv.cbv[i].lf = 1
            uv.cbv[i].fp = 1
            uv.cbv[i].lp = 512
            uv.cbv[i].proc = 2
            fmode = 0
            fname = ''
            fname = uv.cbf[i].fname
            IF (strlen (fname) GT 0) THEN BEGIN
               l1 = rstrpos (fname, '/') IF (l1 GT 0) THEN strput, fname, '*', l1 + 1
               l1 = rstrpos (fname, 'ho') IF (l1 GT 0)
               THEN            strput, fname, 'oc', l1
            ENDIF ELSE fname = '*oc*v'
            fname = dialog_pickfile (FILE = uv.cbv[i].fname, FILTER = fname, /READ, $
                                     TITLE = 'Select CO (CBV) Image') IF (strlen (fname) EQ 0) THEN BEGIN
               widget_control, uv.wid.msg, set_value = 'NO FILE entered'
               uv.img[i].proc = 0
               rtype = 0
            ENDIF ELSE BEGIN
               image = uv.cbv[i]
               image.fname = fname
               image.fname = fname
               msg = ''
               IF (getframe (image, msg) NE 0)
               THEN            BEGIN
                  widget_control, uv.wid.msg, set_value = msg
                  uv.cbv[i].fname = ''
                  RETURN
               ENDIF
               uv.cbv[i] = image
               widget_control, uv.wid.msg, set_value = 'File ' + uv.cbv[i].fname + ' found'
               image = uv.cbv[i]
               IF (gethdr (image, msg) NE 0) THEN BEGIN
                  widget_control, uv.wid.msg, set_value = msg
                  uv.img[i].proc = 0
                  rtype = 0
               ENDIF ELSE widget_control, uv.wid.msg, set_value = 'Metabolic factors read'
               uv.cbv[i] = image
            ENDELSE
         ENDIF
      ENDELSE

      setuv = 1
      IF (dsp[i].set)
      THEN            redisplay = 1
   ENDCASE

   ;;
   -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
   uv.wid.edit:BEGIN
      i = ev.value - 1
      IF (uv.wid.win[i] GT 0)
      THEN            BEGIN
         VIDI_EDIT, uv, i, dsp[i].zoom
         setuv = 1
      ENDIF ELSE widget_control, uv.wid.msg, set_value = 'NO IMAGE displayed for Edit'
   ENDCASE

   ;;
   -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
   uv.wid.segment:BEGIN
      inum = ev.value - 1
      IF (uv.wid.win[inum] GT 0)
      THEN            VIDI_SEGMENT, uv, inum $
      ELSE widget_control, uv.wid.msg, set_value = 'NO IMAGE displayed for VOI'
      setuv = 1
      newrange = 1
      redisplay = 1
   ENDCASE

   ;;
   -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
   uv.wid.save:BEGIN
      i = ev.value - 1
      IF (uv.wid.win[i] GT 0)
      THEN            BEGIN
         temp = ev.top
         fname = uv.img[i].fname
         savefile = widget_base (group_leader = uv.wid.base)
         text = cw_field (savefile, title = 'Filename: ', value = fname, $
                          xsize = 80, /STRING, /RETURN_EVENTS)
         widget_control, savefile, xoffset = 0, yoffset = 0, /realize
         ev = WIDGET_EVENT (savefile) widget_control, savefile, /DESTROY
         fname = ev.value
         ev.top = temp
         IF (strtrim (fname EQ ''))
         THEN            BEGIN
            msg = 'NO FILENAME entered'
            result = dialog_message (msg, title = 'Image was NOT SAVED!', /error)
         ENDIF ELSE BEGIN
            msg = ' '
            stat = putinterfile (uv.img[i], fname, msg) uv.img[i].fname = fname
            widget_control, uv.wid.msg, set_value = msg
            widget_control, uv.wid.fname[i], set_value = uv.img[i].fname
            setuv = 1
         ENDELSE
      ENDIF ELSE widget_control, uv.wid.msg, set_value = 'NO IMAGE to save'
   ENDCASE

   ;;
   -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
   uv.wid.tiff:BEGIN
      i = ev.value - 1
      IF (uv.wid.win[i] GT 0)
      THEN            BEGIN
         temp = ev.top
         fname = ''
         savefile = widget_base (group_leader = uv.wid.base)
         text = cw_field (savefile, title = 'TIF Filename: ', value = fname, $
                          xsize = 80, /STRING, /RETURN_EVENTS)
         widget_control, savefile, xoffset = 50, yoffset = 0, /realize
         ev = WIDGET_EVENT (savefile) widget_control, savefile, /DESTROY
         fname = ev.value
         ev.top = temp
         IF (strtrim (fname EQ ''))
         THEN            BEGIN
            msg = 'NO FILENAME entered'
            result = dialog_message (msg, title = 'Image was NOT SAVED!', /error)
         ENDIF ELSE BEGIN
            k = rstrpos (fname, '.') IF (k GT 0) THEN fname = strmid (fname, 0, k) fname = fname + '.tif'
            wset,           dsp[i].id
            a = tvrd () tvlct, r, g, b, /get
            write_tiff, fname, a, compression = 1, red = r, green = g, blue = b
            widget_control, uv.wid.msg, set_value = 'Window saved in TIFF format:  ' + fname
         ENDELSE
      ENDIF ELSE widget_control, uv.wid.msg, set_value = 'NO IMAGE to save'
   ENDCASE

   ;;
   -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
   uv.wid.peaks:BEGIN
      IF (orient GT 0)
      THEN            BEGIN
         msg = 'Peaks displayed on TRANSVERSE slices'
         widget_control, uv.wid.msg, set_value = msg
      ENDIF ELSE BEGIN
         otemp = orient
         VIDI_PEAKS,     uv, otemp, dsp[0].zoom
         setuv = 1
      ENDELSE
   ENDCASE

   ;;
   -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
   uv.wid.exit:BEGIN
      FOR i = 0, nimg DO if (uv.img[i].data NE ptr_new ())
      THEN            ptr_free, uv.img[i].data
      IF ((loglun NE 0) AND (size (loglun))[1] NE 0)
      THEN            BEGIN
         status = FSTAT (loglun)
         IF (status.open GT 0) THEN close, loglun
         IF (status.size EQ 0)
         THEN            BEGIN
            command = 'rm ' + status.name
            spawn,          command
         ENDIF ELSE print, 'Statistics saved in ', status.name
      ENDIF
      widget_control, ev.top, /destroy
   ENDCASE

   ;;
   -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
   uv.wid.up:BEGIN
      CASE fmode OF
         0:BEGIN
            FOR i = 0, nimg DO IF (uv.img[i].p GT uv.img[i].fp)
            THEN            uv.img[i].p = uv.img[i].p - 1
            widget_control, uv.wid.slider, set_value = uv.img[0].p
         ENDCASE
         1:BEGIN
            FOR i = 0,      nimg DO IF (uv.img[i].f GT uv.img[i].ff)
            THEN            uv.img[i].f = uv.img[i].f - 1
            widget_control, uv.wid.slider, set_value = uv.img[0].f
         ENDCASE
         2:BEGIN
            FOR i = 0,      nimg DO BEGIN
               IF (uv.img[i].p GT uv.img[i].fp)
               THEN            BEGIN
                  uv.img[i].p = uv.img[i].p - 1
               ENDIF ELSE IF (uv.img[i].f GT uv.img[i].ff) THEN BEGIN
                  uv.img[i].f = uv.img[i].f - 1
                  uv.img[i].p = uv.img[i].lp
               ENDIF
            ENDFOR
            planes = uv.img[0].lp - uv.img[0].fp + 1
            slice = (uv.img[0].f - uv.img[0].ff) * planes + uv.img[0].p - uv.img[0].fp + 1
            widget_control, uv.wid.slider, set_value = slice
         ENDCASE
      ENDCASE
      setuv = 1
      redisplay = 1
   ENDCASE

   ;;
   -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
   uv.wid.mid:BEGIN
      FOR i = 0, nimg DO uv.img[i].p = (lp + fp) / 2
      widget_control, uv.wid.slider, set_value = uv.img[0].p
      setuv = 1
      redisplay = 1
   ENDCASE

   ;;
   -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
   uv.wid.down:BEGIN
      p = uv.img[0].p
      CASE fmode OF
         0:BEGIN
            FOR i = 0, nimg DO IF (uv.img[i].p LT lp)
            THEN            uv.img[i].p = uv.img[i].p + 1
            widget_control, uv.wid.slider, set_value = uv.img[0].p
         ENDCASE
         1:BEGIN
            FOR i = 0,      nimg DO IF (uv.img[i].f LT uv.img[i].lf)
            THEN            uv.img[i].f = uv.img[i].f + 1
            widget_control, uv.wid.slider, set_value = uv.img[0].f
         ENDCASE
         2:BEGIN
            FOR i = 0,      nimg DO BEGIN
               IF (uv.img[i].p LT uv.img[i].lp)
               THEN            BEGIN
                  uv.img[i].p = uv.img[i].p + 1
               ENDIF ELSE IF (uv.img[i].f LT uv.img[i].lf) THEN BEGIN
                  uv.img[i].f = uv.img[i].f + 1
                  uv.img[i].p = uv.img[i].fp
               ENDIF
            ENDFOR
            planes = uv.img[0].lp - uv.img[0].fp + 1
            slice = (uv.img[0].f - uv.img[0].ff) * planes + uv.img[0].p - uv.img[0].fp + 1
            widget_control, uv.wid.slider, set_value = slice
         ENDCASE
      ENDCASE
      setuv = 1
      redisplay = 1
   ENDCASE

   ;;
   -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
   uv.wid.slider:BEGIN
      widget_control, uv.wid.slider, get_value = slice
      CASE fmode OF
         0:FOR i = 0, nimg DO IF (slice LE lp)
         THEN            uv.img[i].p = slice
         1:FOR i = 0,    nimg DO IF (slice LE uv.img[i].lf)
         THEN            uv.img[i].f = slice
         2:BEGIN
            planes = uv.img[0].lp - uv.img[0].fp + 1
            frames = uv.img[0].lf - uv.img[0].ff + 1
            FOR i = 0,      nimg DO BEGIN
               loc = slice - 1
               f = loc / planes
               p = loc - f * planes
               uv.img[i].f = uv.img[i].ff + f
               uv.img[i].p = uv.img[i].fp + p
            ENDFOR
         ENDCASE
      ENDCASE
      setuv = 1
      redisplay = 1
   ENDCASE

   ;;
   -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
   uv.wid.display:BEGIN
      CASE ev.value OF
         0:IF (dsp[0].set EQ 0)
         THEN            dsp[0].set = 1 $
         ELSE dsp[0].set = 0
         1:IF (dsp[1].set EQ 0) THEN dsp[1].set = 1 $
         ELSE dsp[1].set = 0
         2:IF (dsp[2].set EQ 0) THEN dsp[2].set = 1 $
         ELSE dsp[2].set = 0
      END
      newrange = 1
      redisplay = 1
   ENDCASE

   ;;
   -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
   uv.wid.defpath:BEGIN
      CASE ev.value OF
         0:default_path = 0
         1:default_path = 1
         2:default_path = 2
         3:default_path = 3
      END
   ENDCASE

   ;;
   -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
   uv.wid.atlas:begin
      CASE ev.value OF
         0:IF (atlas[0] EQ 0)
         THEN            atlas[0] = 1 $
         ELSE atlas[0] = 0
         1:IF (atlas[1] EQ 0) THEN atlas[1] = 1 $
         ELSE atlas[1] = 0
         2:IF (atlas[2] EQ 0) THEN atlas[2] = 1 $
         ELSE atlas[2] = 0
         3:IF (atlas[3] EQ 0) THEN atlas[3] = 1 $
         ELSE atlas[3] = 0
      END
      redisplay = 1
   ENDCASE

   ;;
   -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
   uv.wid.atltype:BEGIN
      CASE ev.value OF
         0:atlas_type = 0
         1:atlas_type = 1
      END
   ENDCASE

   ;;
   -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
   uv.wid.lut1:BEGIN
      CASE ev.value OF
         1:BEGIN
            loadct, 36, ncolors = dsp[0].nc, bottom = dsp[0].bot, $
               file = "/usr/local/npg/idl/color_tables/videen.tbl"
            dsp[0].range = 0
         ENDCASE
         2:BEGIN
            loadct, 0, ncolors = dsp[0].nc, bottom = dsp[0].bot, $
               file = "/usr/local/npg/idl/color_tables/videen.tbl"
            dsp[0].range = 0
         ENDCASE
         3:BEGIN
            loadct, 3, ncolors = dsp[0].nc, bottom = dsp[0].bot, $
               file = "/usr/local/npg/idl/color_tables/videen.tbl"
            dsp[0].range = 0
         ENDCASE
         4:BEGIN
            loadct, 37, ncolors = dsp[0].nc, bottom = dsp[0].bot, $
               file = "/usr/local/npg/idl/color_tables/videen.tbl"
            dsp[0].range = 2
         ENDCASE
         5:BEGIN
            loadct, 10, ncolors = dsp[0].nc, bottom = dsp[0].bot, $
               file = "/usr/local/npg/idl/color_tables/videen.tbl"
            dsp[0].range = 0
         ENDCASE
      ENDCASE
      redisplay = 1
   ENDCASE
   ;;
   -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -

   uv.wid.lut2:BEGIN
      CASE ev.value OF
         1:BEGIN
            loadct, 36, ncolors = dsp[1].nc, bottom = dsp[1].bot, $
               file = "/usr/local/npg/idl/color_tables/videen.tbl"
            dsp[1].range = 0
         ENDCASE
         2:BEGIN
            loadct, 0, ncolors = dsp[1].nc, bottom = dsp[1].bot, $
               file = "/usr/local/npg/idl/color_tables/videen.tbl"
            dsp[1].range = 0
         ENDCASE
         3:BEGIN
            loadct, 3, ncolors = dsp[1].nc, bottom = dsp[1].bot, $
               file = "/usr/local/npg/idl/color_tables/videen.tbl"
            dsp[1].range = 0
         ENDCASE
         4:BEGIN
            loadct, 37, ncolors = dsp[1].nc, bottom = dsp[1].bot, $
               file = "/usr/local/npg/idl/color_tables/videen.tbl"
            dsp[1].range = 2
         ENDCASE
         5:BEGIN
            loadct, 10, ncolors = dsp[1].nc, bottom = dsp[1].bot, $
               file = "/usr/local/npg/idl/color_tables/videen.tbl"
            dsp[1].range = 0
         ENDCASE
      ENDCASE
      redisplay = 1
   ENDCASE

   ;;
   -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -

   uv.wid.atlasfile:BEGIN
      i = 0
      uv.img[i].ff = 1
      uv.img[i].lf = 1
      uv.img[i].fp = 1
      uv.img[i].lp = 512
      image = uv.img[i]
      CASE            ev.value OF
         1:image.fname = "/usr/local/npg/lib/t88/meanpet.t88"
         2:image.fname = "/usr/local/npg/lib/t88/meanmr.t88"
      ENDCASE
      msg = ''
      IF (getframe (image, msg) NE 0) THEN BEGIN
         widget_control, uv.wid.msg, set_value = msg
         uv.img[i].fname = ''
         RETURN
      ENDIF
      tmp = uv.img[i].p
      uv.img[i] = image
      IF (orient EQ 0)
      THEN            BEGIN     ;
         keep same plane if in range
         uv.img[i]
         .               p = tmp > uv.img[i].fp
         uv.img[i].p = tmp < uv.img[i].lp
      ENDIF ELSE uv.img[i].p = tmp
      widget_control, uv.wid.msg, set_value = 'File ' + uv.img[i].fname + ' found'
      widget_control, uv.wid.fname[i], set_value = uv.img[i].fname
      uv.img[i].proc = 0
      setuv = 1
      newrange = 1
      IF (dsp[i].set) THEN redisplay = 1
   ENDCASE
   ;;
   -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -

ELSE:

ENDCASE

;;
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -

IF (newrange)
	THEN            BEGIN

	                IF ((dsp[0].set) AND (strlen (uv.img[0].fname) GT 0) AND $
			                    (dsp[1].set) AND (strlen (uv.img[1].fname) GT 0))
	THEN            set = 3 $
	ELSE IF ((dsp[1].set) AND (strlen (uv.img[1].fname) GT 0)) THEN set = 2 $
ELSE IF ((dsp[0].set) AND (strlen (uv.img[0].fname) GT 0)) THEN set = 1 $
ELSE set = 0

CASE set OF
1:BEGIN
ff = uv.img[0].ff
lf = uv.img[0].lf
i = 0
ENDCASE
2:BEGIN
ff = uv.img[1].ff
lf = uv.img[1].lf
i = 1
ENDCASE
3:BEGIN
ff = uv.img[0].ff
lf = uv.img[0].lf
i = 0
ENDCASE
ELSE:
ENDCASE

CASE orient OF
0:BEGIN
IF (set EQ 3) THEN BEGIN
fp = uv.img[0].fp > uv.img[1].fp
lp = uv.img[0].lp < uv.img[1].lp
ENDIF ELSE IF (set EQ 2) THEN BEGIN
fp = uv.img[1].fp
lp = uv.img[1].lp
ENDIF ELSE IF (set EQ 1) THEN BEGIN
fp = uv.img[0].fp
lp = uv.img[0].lp
ENDIF
ENDCASE
1:BEGIN;
coronal
fp = 1
IF (set EQ 3)
	THEN            lp = uv.img[0].dim[1] < uv.img[1].dim[1] $
	ELSE IF (set GT 0) THEN lp = uv.img[i].dim[1]
ENDCASE
2:BEGIN;
sagittal
fp = 1
IF (set EQ 3)
	THEN            lp = uv.img[0].dim[0] < uv.img[1].dim[0] $
	ELSE IF (set GT 0) THEN lp = uv.img[i].dim[0]
ENDCASE
ENDCASE

IF (set GT 0) THEN BEGIN
CASE fmode OF
0:BEGIN
widget_control, uv.wid.slider, set_slider_min = fp
                IF (uv.img[i].lp GT uv.img[i].fp + 1) THEN $
                widget_control, uv.wid.slider, set_slider_max = lp $
                ELSE widget_control, uv.wid.slider, set_slider_max = fp + 1
                IF (uv.img[i].p GT lp)
	THEN FOR        k = 0, 1 DO uv.img[k].p = lp
	IF (uv.img[i].p LT fp) THEN FOR k = 0, 1 DO uv.img[k].p = fp
                widget_control, uv.wid.slider, set_value = uv.img[i].p
                FOR k = 0, 1 DO BEGIN
                IF (dsp[k].set)
	THEN            BEGIN
	                IF (uv.img[k].f GT uv.img[k].lf)
	THEN            uv.img[k].f = uv.img[k].lf
	IF (uv.img[k].f LT uv.img[k].ff) THEN uv.img[k].f = uv.img[k].ff
ENDIF
ENDFOR
ENDCASE

1:BEGIN
widget_control, uv.wid.slider, set_slider_min = ff
                IF (uv.img[i].lf GT uv.img[i].ff + 1) THEN $
                widget_control, uv.wid.slider, set_slider_max = lf $
                ELSE widget_control, uv.wid.slider, set_slider_max = ff + 1
                IF (uv.img[i].f GT uv.img[i].lf)
	THEN            uv.img[i].f = lf
	IF (uv.img[i].f LT uv.img[i].ff) THEN uv.img[i].f = ff
uv.img[i].p = uv.img[i].fp
widget_control, uv.wid.slider, set_value = uv.img[i].f
                ENDCASE

                2:BEGIN
                CASE set OF
                1:BEGIN
                planes = uv.img[0].dim[2]
                frames = uv.img[0].dim[3]
                ENDCASE
                2:BEGIN
                planes = uv.img[1].dim[2]
                frames = uv.img[1].dim[3]
                ENDCASE
                3:BEGIN
                planes = uv.img[0].dim[2] < uv.img[1].dim[2]
                frames = uv.img[0].dim[3] < uv.img[1].dim[3]
                ENDCASE
                ENDCASE
                widget_control, uv.wid.slider, set_slider_min = 1
                widget_control, uv.wid.slider, set_slider_max = planes * frames
                IF (uv.img[i].f GT lf)
	THEN FOR        k = 0, 1 DO uv.img[k].f = lf
	IF (uv.img[i].f LT ff) THEN FOR k = 0, 1 DO uv.img[k].f = ff
                IF (uv.img[i].p GT lp)
	THEN FOR        k = 0, 1 DO uv.img[k].p = lp
	IF (uv.img[i].p LT fp) THEN FOR k = 0, 1 DO uv.img[k].p = fp
                loc = (uv.img[i].f - ff) * planes + uv.img[i].p
                widget_control, uv.wid.slider, set_value = loc
                ENDCASE
                ENDCASE
                ENDIF
                ENDIF
               ;;
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -

IF (redisplay)
	THEN            BEGIN
	                FOR i = 0, nwin DO BEGIN
	                IF (dsp[i].set)
	THEN            BEGIN
	                IF (i LT 2)
	THEN            BEGIN
	                IF (strlen (uv.img[i].fname) EQ 0)
	THEN            BEGIN
	                widget_control, uv.wid.msg, set_value = 'No file has been entered for #' + string (i + 1)
redisplay = 0
ENDIF
ENDIF ELSE BEGIN
IF ((strlen (uv.img[0].fname) EQ 0) OR (strlen (uv.img[1].fname) EQ 0)) THEN BEGIN
widget_control, uv.wid.msg, set_value = 'Two image files must be entered'
                redisplay = 0
                ENDIF
                ENDELSE
                widget_control, uv.wid.win[i], bad_id = status
                IF (status NE 0)
	then            dsp[i].init = 0

	IF (redisplay AND (dsp[i].init EQ 0)) THEN BEGIN
dispbase = widget_base (TITLE = 'Image ' + strcompress (string (i + 1)), $
			group_leader = uv.wid.base, uvalue = ev.top)
win = widget_draw (dispbase, xsize = dsp[i].xsize, ysize = dsp[i].ysize, /button, /motion)
! p.background = 0
scrnsize = get_screen_size () yloc = scrnsize[1] / 2
IF (yloc GT 340) THEN yloc = 340
CASE i OF
0:xloc = 0
1:xloc = scrnsize[0] / 3
2:xloc = 2 * scrnsize[0] / 3
ENDCASE
widget_control, dispbase, /realize, xoffset = xloc, yoffset = yloc
                widget_control, win, get_value = windowIndex
                wset, windowIndex
                xmanager, "VIDIDISP" + strcompress (string (i + 1), /remove_all), dispbase
                uv.wid.win[i] = win
                dsp[i].id = windowIndex
                dsp[i].init = 1
                IF (dsp[i].nc EQ 0)
	THEN            BEGIN
	               ;;
print, "lut ", !d.n_colors
nc = (!d.n_colors - 1) / 2
dsp[0].bot = 0
dsp[0].nc = nc
dsp[1].bot = nc
dsp[1].nc = nc
dsp[2].nc = nc
ENDIF
IF (i EQ 0)
	THEN            loadct, 36, ncolors = dsp[0].nc, bottom = dsp[0].bot, $
	                file = "/usr/local/npg/idl/color_tables/videen.tbl" $
	ELSE IF (i EQ 1) THEN loadct, 36, ncolors = dsp[1].nc, bottom = dsp[1].bot, $
                file = "/usr/local/npg/idl/color_tables/videen.tbl"
                setuv = 1
                ENDIF
                ENDIF
                ENDFOR
                IF (redisplay)
	THEN            BEGIN
	                VIDI_DISPLAY, uv
	                VIDI_ATLAS, uv
	                ENDIF
	                ENDIF

	                IF (setuv)
	THEN            widget_control, ev.top, set_uvalue = uv
	END


	PRO VIDI
	@ IMAGE_UTILS.h
	@ VIDI.h

        on_error,       2
        logfile = ''
        logfile = getenv ('LOGFILE')
        local_path = getenv ('PWD') IF (strlen (logfile) GT 0) THEN BEGIN
           openw,          loglun, logfile, ERROR = err, /GET_LUN
           IF (err NE 0)
           THEN            BEGIN
              print, 'Cannot open logfile: ', logfile
              loglun = 0
           ENDIF
        ENDIF

        base = widget_base (TITLE = 'VIDI', /COLUMN)
        base1 = widget_base (base, row = 1) select0 =['1\Get Files', '1\Image 1', '0\First Frame, All Planes', $
                                                      '0\One Frame, All Planes', '0\One Frame, Select Planes', $
                                                      '0\All Frames, One Plane', '2\All Frames, All Planes', $
                                                      '1\Image2', '0\First Frame, All Planes', $
                                                      '0\One Frame, All Planes', '0\One Frame, Select Planes', $
                                                      '0\All Frames, One Plane', '0\All Frames, All Planes']
        file = cw_pdmenu (base1, select0);;
        select2 =['1\ViewAll', $
                  ;;
        '1\Image1', '0\by 1 ', '0\   2 ', '0\   3 ', '0\   4 ', $
           ;;
        '0\   5 ', '0\   6', '0\   7', '0\   8', '0\   9', '2\  10', $
           ;;
        '1\Image2', '0\by 1 ', '0\   2 ', '0\   3 ', '0\   4 ', $
           ;;
        '0\   5 ', '0\   6', '0\   7', '0\   8', '0\   9', '0\  10'];;
        viewall = cw_pdmenu (base1, select2)
        viewer = widget_button (base1, value = 'Viewer')
        select3 =['1\Orientation', '0\Transverse', '0\Coronal', '0\Sagittal']
        orient = cw_pdmenu (base1, select3, /RETURN_NAME)
        select4 =['1\Magnify', '0\ 1 ', '0\ 2 ', '0\ 3 ', '0\ 4 ', '0\ 5 ', '0\ 6 ', '0\ 7 ', '0\ 8 ', $
                  '0\ 9 ', '0\ 10 ', '0\ 11 ', '0\ 12 ', '0\ 13 ', '0\ 14 ', '0\ 15 ', '0\ 16 ']
        zoom = cw_pdmenu (base1, select4)
        select5 =['1\Color', '0\Image 1', '0\Image 2']
        color = cw_pdmenu (base1, select5)
        select6 =['1\VOI', '0\Image 1', '0\Image 2']
        roi = cw_pdmenu (base1, select6)
        scale = widget_button (base1, value = 'Scale')
        select7 =['1\Processing', '1\Image 1', '0\None', '0\CBF', '0\CBV', '0\OEF', '0\CMRO2', '0\CMRGlu', $
                  '2\MTT', '1\Image 2', '0\None', '0\CBF', '0\CBV', '0\OEF', '0\CMRO2', '0\CMRGlu', $
                  '0\MTT']
        process = cw_pdmenu (base1, select7)
        select8 =['1\Edit', '0\Image 1', '0\Image 2', '0\Image 2 in 3']
        edit = cw_pdmenu (base1, select8)
        select9 =['1\Segmentation', '0\Image 1', '0\Image 2', '0\Image 2 in 3']
        segment = cw_pdmenu (base1, select9)
        select10 =['1\Save Image', '0\Image 1 to Interfile', '0\Image 2 to Interfile']
        save = cw_pdmenu (base1, select10)
        select11 =['1\Save Window', '0\Image 1 to TIF', '0\Image 2 to TIF', '0\Image 3 to TIF']
        tiff = cw_pdmenu (base1, select11)
        peaks = widget_button (base1, value = 'Peaks')
        exit = widget_button (base1, value = 'Exit')
        base2 = widget_base (base, row = 1)
        base2a = widget_base (base2, row = 2)
        fname = lonarr (2)
        fname[0] = cw_field (base2a, title = 'Image 1', value = '', xsize = 85, /RETURN_EVENTS)
        fname[1] = cw_field (base2a, title = 'Image 2', value = '', xsize = 85, /RETURN_EVENTS)
        base2d = widget_base (base2, row = 2)
        max = lonarr (2)
        max[0] = cw_field (base2d, title = 'Max', /noedit, value = 0, xsize = 10, ysize = 1)
        max[1] = cw_field (base2d, title = 'Max', /noedit, value = 0, xsize = 10, ysize = 1)
        slice = 1
        slider = widget_slider (base, value = slice, min = 1, max = 47, xsize = 1000)
        base2e = widget_base (base, column = 2)
        display = cw_bgroup (base2e,['Image 1', 'Image 2', 'Composite (2 over 1)'], /NONEXCLUSIVE, /ROW)
        base2f = widget_base (base2e, row = 1)
        up = widget_button (base2f, value = '<- Up  ')
        mid = widget_button (base2f, value = ' Middle ')
        down = widget_button (base2f, value = ' Down ->')
        defpath = cw_bgroup (base2f,['Previous', 'Image 1', 'Image 2', 'Local  (Default Path)'], $
                             / ROW, /EXCLUSIVE, set_value = default_path)
        base2g = widget_base (base, row = 1)
        atl = cw_bgroup (base2g,['Atlas 1', 'Atlas 2', 'Composite Atlas', 'Label    '], $
                         / ROW, /NONEXCLUSIVE, set_value = atlas)
        atltype = cw_bgroup (base2g,['Simple or', 'Detailed Talairach 1988'], /ROW, /EXCLUSIVE, set_value = atlas_type)
        select14 =['1\Color 1', '0\Rainbow', '0\Gray', '0\Hot Metal', '0\Red/Green', '0\BGRY']
        select15 =['1\Color 2', '0\Rainbow', '0\Gray', '0\Hot Metal', '0\Red/Green', '0\BGRY']
        lut1 = cw_pdmenu (base2g, select14)
        lut2 = cw_pdmenu (base2g, select15)
        select16 =['1\Atlas 1', '0\PET', '0\MRI']
        atlasfile = cw_pdmenu (base2g, select16)
        msg = widget_text (base)
        voi = ptr_new (
        {
        VOI
        })

        (*voi).id = 'VOI'

        wid = {
        WIDGETS, $
           base:		base, $ ;
           top level       base
        file:file, $            ;
           file selection button id
        viewer:viewer, $        ;
           display all slices button id
        orient:orient, $        ;
           image orientation selection button id
        zoom:zoom, $            ;
           magnification selection button id
        color:color, $          ;
           color selection button id
        voicolor:6, $           ;
           voi draw        color (3 = white ;
        6 = XOR)
        scale:		scale, $ ;
           scaling menu selection button id
        process:process, $      ;
           type of processing selection button id
        roi:roi, $              ;
           ROI             menu
        edit:edit, $            ;
           edit            menu
        editval:0, $            ;
           value to fill in edit operations
        rsign:2, $              ;
           relation to rthresh determining replacement
        rthresh:0, $            ;
           threshold for   determining replacement
        rplanes:0, $            ; which planes to replace
           rerase:1, $          ; dimension of eraser (in image pixels)
           rslab:0, $           ;
           number of planes above and / or below to replace
        rmode:0, $              ;
           replacement     mode (inside / outside)
        segment:segment, $      ;
           segmentation    menu
        save:save, $            ;
           save            menu
        tiff:tiff, $            ;
           save window as tiff menu
        peaks:peaks, $          ;
           peaks menu selection button id
        up:up, $                ;
           up              button
        mid:mid, $              ;
           mid             button
        down:down, $            ;
           down            button
        defpath:defpath, $      ;
           default path selection
atlas:
        atl, $                  ;
           atlas           selection
        atltype:atltype, $      ;
           atlas           selection
        lut1:lut1, $            ;
           quick color     table
        lut2:lut2, $
           atlasfile:atlasfile, $ ;
           quick selection of atlas file for image 1
        exit:exit, $            ; exit selection button id
           fname:fname, $       ; filename entry field id
           max:max, $           ; maximum value displayed
           slider:slider, $     ; slice selection slider id
           display:display, $   ; image number selection buttons id
           msg:msg, $           ; message field id
           win:lonarr (3)
        ,$                      ;
           draw window widget id
        vthresh:[0L, 0L], $     ;
           low and high thresholds for VOI definition
        voi:voi $               ; pointer to VOI linked list
           }

        dsp[0]
        .               cut = 50 ;
        default cutoff for combined
        display
        FOR i = 0, 2 DO dsp[i].set = 0
        FOR i = 0, 2 DO dsp[i].init = 0
        FOR i = 0, 2 DO dsp[i].zoom = 1
        FOR i = 0, 1 DO dsp[i].bot = 0
        FOR i = 0, 1 DO dsp[i].nc = 0
        dsp[0].set = 1
        orient = 0L
        fmode = 0L

        vdsp.set = 0
        vdsp.init = 0
        vdsp.zoom = 1
        vdsp.nx = 8
        vdsp.ny = 6
        vdsp.start = 1
        vdsp.space = 1

        img = {
        VIMAGE
        }

        uv = {
wid:
        wid, img: [img, img], cbf: [img, img], cbv:[img, img]
        }
        FOR             i = 0, 1 DO BEGIN
           uv.img[i].f = 1
           uv.img[i].p = 1
           uv.img[i].ff = 1
           uv.img[i].lf = 1
           uv.img[i].fp = 1
           uv.img[i].lp = 512
        ENDFOR

        widget_control, display, set_value = dsp[0].set
        widget_control, base, set_uvalue = uv
        widget_control, base, /realize, xoffset = 0, yoffset = 0
        xmanager,       "VIDI", base, EVENT_HANDLER = 'VIDI_EVENT'

     END
