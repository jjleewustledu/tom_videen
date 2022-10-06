PRO VIDI_VIEWER_EVENT, ev
   COMMON IMAGE_UTILS
   COMMON VIDI
   
   catch, error_status
   IF (error_status NE 0) THEN BEGIN
      ok = widget_message(!err_string)
      RETURN
   END
   
   done = 0
   widget_control, ev.top, get_uvalue=vw
   
   CASE ev.id OF
      
      vw.stype: BEGIN
         vdsp.set = ev.value
      END
      
      vw.zoom: BEGIN
         vdsp.zoom = ev.value
      END
      
      vw.nx: BEGIN
         vdsp.nx = ev.value
      END
      
      vw.ny: BEGIN
         vdsp.ny = ev.value
      END
      
      vw.start: BEGIN
         vdsp.start = ev.value
      END
      
      vw.space: BEGIN
         vdsp.space = ev.value
      END

      vw.zooms: BEGIN
         vdsp.zoom = ev.value
         widget_control, vw.zoom, set_value=vdsp.zoom
      ENDCASE
      
      vw.nxs: BEGIN
         vdsp.nx = ev.value
         widget_control, vw.nx, set_value=vdsp.nx
      END
      
      vw.nys: BEGIN
         vdsp.ny = ev.value
         widget_control, vw.ny, set_value=vdsp.ny
      END
      
      vw.starts: BEGIN
         vdsp.start = ev.value
         widget_control, vw.start, set_value=vdsp.start
      END
      
      vw.spaces: BEGIN
         vdsp.space = ev.value
         widget_control, vw.space, set_value=vdsp.space
      END

      vw.display: BEGIN
         i = vdsp.set
         IF NOT ptr_valid(vw.img[i].data) THEN message, 'No Image Data' $
         ELSE BEGIN
            vdsp.id    = 9
            vdsp.init  = 0
            vdsp.xsize = 128
            vdsp.ysize = 128
            vdsp.min   = dsp[i].min
            vdsp.max   = dsp[i].max
            vdsp.cut   = dsp[i].cut
            vdsp.type  = dsp[i].type
            vdsp.range = dsp[i].range
            vdsp.bot   = dsp[i].bot
            vdsp.nc    = dsp[i].nc
            vdsp.table = dsp[i].table
            vdsp.voi   = dsp[i].voi
            vidi_viewer_display, vw.tlb, vw.img[i], vw.cbf[i], vw.cbv[i]
         ENDELSE
      END
      
      vw.next: BEGIN
         i = vdsp.set
         nfrm = vw.img[i].lf - vw.img[i].ff + 1
         npln = vw.img[i].lp - vw.img[i].fp + 1
         ndsp = vdsp.nx * vdsp.ny * vdsp.space
         incr = npln < ndsp
         maxs = (nfrm-1)*npln
         IF NOT ptr_valid(vw.img[i].data) THEN message, 'No Image Data' $
         ELSE BEGIN
            IF (fmode LT 2) THEN vdsp.start = vdsp.start + incr $
            ELSE vdsp.start = (vdsp.start + npln) < maxs
            widget_control, vw.start, set_value=vdsp.start
            vdsp.id    = 9
            vdsp.init  = 0
            vdsp.xsize = 128
            vdsp.ysize = 128
            vdsp.min   = dsp[i].min
            vdsp.max   = dsp[i].max
            vdsp.cut   = dsp[i].cut
            vdsp.type  = dsp[i].type
            vdsp.range = dsp[i].range
            vdsp.bot   = dsp[i].bot
            vdsp.nc    = dsp[i].nc
            vdsp.table = dsp[i].table
            vdsp.voi   = dsp[i].voi
            vidi_viewer_display, vw.tlb, vw.img[i], vw.cbf[i], vw.cbv[i]
         ENDELSE
      END
      
      vw.sdone: BEGIN
         widget_control, ev.top, /destroy
         done = 1
      END
      
   ENDCASE
   IF (done EQ 1) THEN RETURN
END

PRO VIDI_VIEWER, uv
   COMMON VIDI
   COMMON IMAGE_UTILS
   
   tlb = uv.wid.base
   img = [uv.img[0], uv.img[1]]
   cbf = [uv.cbf[0], uv.cbf[1]]
   cbv = [uv.cbv[0], uv.cbv[1]]
   sbase = widget_base(title='Viewing Options', group_leader=tlb, /COLUMN)
   sbase1 = widget_base(sbase, /ROW)
   stype = cw_bgroup(sbase1, ['Image 1','Image 2'], /ROW, /EXCLUSIVE, set_value=vdsp.set)

   select1 = ['1\Magnify','0\ 1 ','0\ 2 ','0\ 3 ','0\ 4 ']
   zooms = cw_pdmenu(sbase1, select1)
   IF (atlas[vdsp.set] EQ 1) THEN vdsp.zoom = max([2, vdsp.zoom])
   scrnsize = get_screen_size()
   xmax = scrnsize[0]
   ymax = scrnsize[1]
   if (xmax GT 2000) then xmax = 1900
   zoom = vdsp.zoom
   CASE orient OF
      0: BEGIN
         xdim = img[0].dim[0]
         ydim = img[0].dim[1]
         pxdim = img[0].pdim[0]
         pydim = img[0].pdim[1]
         planes = img[0].dim[2]
      ENDCASE
      1: BEGIN
         xdim = img[0].dim[1]
         ydim = img[0].dim[2]
         pxdim = img[0].pdim[1]
         pydim = img[0].pdim[2]
         planes = img[0].dim[1]
      ENDCASE
      2: BEGIN
         xdim = img[0].dim[0]
         ydim = img[0].dim[2]
         pxdim = img[0].pdim[0]
         pydim = img[0].pdim[2]
         planes = img[0].dim[0]
      ENDCASE
   ENDCASE
   fact = 1.
   yoffset = 75

   ; Set nx, ny for screensize

   IF (vdsp.nx EQ 0 OR vdsp.ny EQ 0) THEN BEGIN
      IF (pydim GT pxdim) THEN BEGIN
         fact = pydim/pxdim
         vdsp.nx = xmax/(zoom*xdim)
         vdsp.ny = (ymax-yoffset)/(zoom*ydim*fact)
      ENDIF ELSE IF (pxdim GT pydim) THEN BEGIN
         fact = pxdim/pydim
         vdsp.nx = xmax/(zoom*xdim*fact)
         vdsp.ny = (ymax-yoffset)/(zoom*ydim)
      ENDIF ELSE BEGIN
         vdsp.nx = xmax/(zoom*xdim)
         vdsp.ny = (ymax-yoffset)/(zoom*ydim)
      ENDELSE
   ENDIF
   IF (planes LT vdsp.nx*vdsp.ny) THEN vdsp.ny = (planes-1)/vdsp.nx + 1
   zoom = cw_field(sbase1, title=' ', /INTEGER, xsize=3, value=vdsp.zoom, /RETURN_EVENTS)

   select2 = ['1\Columns','0\ 1 ','0\ 2 ','0\ 3 ','0\ 4 ','0\ 5 ','0\ 6 ','0\ 7 ','0\ 8 ', $
            '0\ 9 ', '0\ 10 ', '0\ 11 ', '0\ 12 ']
   nxs = cw_pdmenu(sbase1, select2)
   nx = cw_field(sbase1, title=' ', /INTEGER, xsize=3, value=vdsp.nx, /RETURN_EVENTS)

   select3 = ['1\Rows','0\ 1 ','0\ 2 ','0\ 3 ','0\ 4 ','0\ 5 ','0\ 6 ','0\ 7 ']
   nys = cw_pdmenu(sbase1, select3)
   ny = cw_field(sbase1, title=' ', /INTEGER, xsize=3, value=vdsp.ny, /RETURN_EVENTS)

   select4 = ['1\Start','0\ 1 ','0\ 2 ','0\ 3 ','0\ 4 ','0\ 5 ','0\ 6 ','0\ 7 ','0\ 8 ', $
            '0\ 9 ', '0\ 10 ', '0\ 11 ', '0\ 12 ', '0\ 13 ', '0\ 14 ', '0\ 15 ', '0\ 16 ', $
            '0\ 17 ', '0\ 18 ', '0\ 19 ', '0\ 20 ', '0\ 21 ', '0\ 22 ', '0\ 23 ', '0\ 24 ', $
            '0\ 25 ', '0\ 26 ', '0\ 27 ', '0\ 28 ', '0\ 29 ', '0\ 30 ', '0\ 31 ', '0\ 32 ', $
            '0\ 33 ', '0\ 34 ', '0\ 35 ', '0\ 36 ', '0\ 37 ', '0\ 38 ', '0\ 39 ', '0\ 40 ']
   starts = cw_pdmenu(sbase1, select4)
   start = cw_field(sbase1, title=' ', /INTEGER, xsize=3, value=vdsp.start, /RETURN_EVENTS)

   select5 = ['1\Spacing','0\ 1 ','0\ 2 ','0\ 3 ','0\ 4 ','0\ 5 ','0\ 6 ','0\ 7 ','0\ 8 ', $
            '0\ 9 ', '0\ 10 ']
   spaces = cw_pdmenu(sbase1, select5)
   space = cw_field(sbase1, title=' ', /INTEGER, xsize=3, value=vdsp.space, /RETURN_EVENTS)

   display = widget_button(sbase1, value='Display')
   next = widget_button(sbase1, value='Next')
   sdone = widget_button(sbase1, value='Exit')
   
   vw = {VIEWER,		$
         sbase    : sbase,	$ ; viewer menu id
         tlb      : tlb,	$
         img      : img,	$
         cbf      : cbf,	$
         cbv      : cbv,	$
         stype    : stype,	$ 
         zoom     : zoom,	$ 
         nx       : nx,		$ 
         ny       : ny,		$ 
         start    : start,	$ 
         space    : space,	$ 
         zooms    : zooms,	$ 
         nxs      : nxs,		$ 
         nys      : nys,		$ 
         starts   : starts,	$ 
         spaces   : spaces,	$ 
         display  : display,	$ 
         next     : next,	$ 
         sdone    : sdone	$ 
        }
   
   widget_control, sbase, set_uvalue=vw
   widget_control, sbase, /realize, xoffset=0, yoffset=0
   xmanager, "VIDI_VIEWER", sbase, EVENT_HANDLER='VIDI_VIEWER_EVENT'
   RETURN
END 
