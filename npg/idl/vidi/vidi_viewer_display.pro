PRO VIDI_VIEWER_DISPLAY, tlb, img, cbf, cbv
   
   COMMON IMAGE_UTILS
   COMMON VIDI
   
   trans = [0,0,1,1,1,2,2,3,3,3,4,4,5,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13, $
      14,15,15,16,17,17,18,18,19,19,20,20,21,21,22,22,23,23,24,24,25,25]
 
   cor = intarr(84)
   cor[0:32]  = [0,0,0,1,1,2,2,2,3,3,4,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13,14,14]
   cor[33:58] = [15,16,16,17,17,18,18,19,19,20,20,21,21,22,22,23,23,24,24,25,25,26,26,26,27,27]
   cor[59:83] = [28,28,28,29,29,30,30,30,31,31,32,32,32,33,33,34,34,34,35,35,36,36,36,37,37]
 
   sag = [16,16,15,15,14,14,13,13,12,12,11,11,10,10,9,9,8,8,7,7,6,5,5,4,4,3,3,2,2,1,0,0]

   i = vdsp.set

   IF NOT ptr_valid(img.data) THEN BEGIN
      message, 'No Data for Image '+string(i+1)
      RETURN
   ENDIF
   zoom = vdsp.zoom
   ncol = vdsp.nx
   nrow = vdsp.ny
   first = vdsp.start - 1
   dim = img.dim
   pdim = img.pdim
   porg = img.org
   proc = img.proc
   datlas = 0

   IF (atlas[i] EQ 1) THEN BEGIN
      IF (pdim[0] EQ 2. AND pdim[1] EQ 2. AND pdim[2] EQ 2. AND porg[2] NE 0.) THEN BEGIN
         datlas = 1
         CASE zoom OF
            2: mag = '_025'
            4: mag = '_050'
            8: mag = '_100'
            ELSE: datlas=0
         ENDCASE
         IF (datlas EQ 1) THEN BEGIN
            CASE atlas_type OF
               0: atype = ''
               1: atype = '_d'
            ENDCASE
            atlpath = '/usr/local/npg/lib/view25d/'
            CASE orient OF
               0: atlname = atlpath+'Talairach_transverse'+atype+mag
               1: atlname = atlpath+'Talairach_coronal'+atype+mag
               2: atlname = atlpath+'Talairach_sagittal'+atype+mag
            ENDCASE
            msg = ''
            adim = [0L,0L,0L]
            aorg = [0.,0.,0.]
            IF (GETATLHDR(atlname+'.ifh', adim, aorg, msg) LT 0) THEN BEGIN
               message, 'msg'
               datlas = 0
            ENDIF ELSE BEGIN
               openr,lun, atlname+'.img', /GET_LUN
               atlimg = ASSOC(lun, intarr(adim[0],adim[1]))
            ENDELSE
         ENDIF
      ENDIF
   ENDIF
   
   CASE orient OF
      0: BEGIN
         x0 = 0
         y0 = 1
         planes = dim[2]
      END
      1: BEGIN
         x0 = 0
         y0 = 2
         planes = dim[1]
      END
      2: BEGIN
         x0 = 1
         y0 = 2
         planes = dim[0]
      END
   ENDCASE
   stype = 1
   xsize = dim[x0] * zoom
   ysize = dim[y0] * zoom
   IF (pdim[y0] GT pdim[x0]) THEN BEGIN
      fact = pdim[y0]/pdim[x0]
      ysize = fact * ysize
   ENDIF ELSE IF (pdim[0] GT pdim[y0]) THEN BEGIN
      fact = pdim[x0]/pdim[y0]
      xsize = fact * xsize
   ENDIF ELSE stype = 0

   frames = dim[3]
   IF (frames EQ 0) THEN frames = 1
   IF (fmode LT 2) THEN BEGIN
      first = first < (planes-1)
      last = first + ncol*nrow*vdsp.space - 1
      last = last < (planes-1)
      frm1 = 0
      frm2 = frames-1
      mini = min(*img.data)
      maxi = max(*img.data)
   ENDIF ELSE BEGIN
      first = img.fp - 1
      last = first + ncol*nrow*vdsp.space - 1
      last = last < (img.lp-1)
      frm1 = vdsp.start / planes
      frm2 = frm1
      mini = min((*img.data)[*,*,*,frm1])
      maxi = max((*img.data)[*,*,*,frm1])
   ENDELSE 
   xscrn = ncol*xsize
   yscrn = nrow*ysize
   nc = vdsp.nc
   
   ;; Determine scaling
   
   IF (vdsp.type EQ 1) THEN BEGIN ; Manual scaling
      mini = vdsp.min
      maxi = vdsp.max
   ENDIF ELSE BEGIN             ; Auto scaling
      IF (vdsp.range EQ 0) THEN mini = 0 $
      ELSE IF (vdsp.range EQ 1) THEN BEGIN
         maxi = -mini
         mini = 0
      ENDIF ELSE IF (-mini GT maxi) THEN maxi = -mini
   ENDELSE
   vdsp.amin = mini
   vdsp.amax = maxi
   
   ;; Scale and Display each slice
   
   viewbase = widget_base(TITLE='Viewer', group_leader=tlb)
   win = widget_draw(viewbase, xsize=xscrn, ysize=yscrn)
   !p.background = 0
   widget_control, viewbase, /realize, xoffset=0, yoffset=75
   widget_control, win, get_value=windowIndex
   wset, windowIndex
   xmanager, "VIDIVIEW"+strcompress(string(windowIndex),/remove_all), viewbase
   
   loc = 0
   FOR f=frm1, frm2 DO BEGIN
      FOR k=first, last, vdsp.space DO BEGIN
         CASE orient OF
            0: image = reform((*img.data)[*,*,k,f])
            1: image = reform((*img.data)[*,k,*,f])
            2: image = reform((*img.data)[k,*,*,f])
         ENDCASE
         IF (proc EQ 3 OR proc EQ 4) THEN BEGIN
            CASE orient OF
               0: cbf0 = reform((*cbf.data)[*,*,k,f])
               1: cbf0 = reform((*cbf.data)[*,k,*,f])
               2: cbf0 = reform((*cbf.data)[k,*,*,f])
            ENDCASE
            CASE orient OF
               0: cbv0 = reform((*cbv.data)[*,*,k,f])
               1: cbv0 = reform((*cbv.data)[*,k,*,f])
               2: cbv0 = reform((*cbv.data)[k,*,*,f])
            ENDCASE
         ENDIF ELSE BEGIN
            cbf0 = 0
            cbv0 = 0
         ENDELSE
         scantime = img.len * 1000
         temp1 = VOI_PROCESS (proc, image, img.mtype, img.mcal, cbf0, cbf.mcal, cbv0, cbv.mcal, scantime)
 
         ;; Determine scaling for processed images
   
         IF (vdsp.type NE 1 AND proc GT 0) THEN BEGIN 
            mini = min(temp1)
            maxi = max(temp1)
            IF (vdsp.range EQ 0) THEN mini = 0 $
            ELSE IF (vdsp.range EQ 1) THEN BEGIN
               maxi = -mini
               mini = 0
            ENDIF ELSE IF (-mini GT maxi) THEN maxi = -mini
         ENDIF
   
         IF (stype EQ 0) THEN BEGIN
            IF (vdsp.range EQ 1) THEN $
               temp1 = bytscl(rebin(-(temp1), xsize, ysize), MIN=mini, MAX=maxi, TOP=(nc-1)) $
            ELSE temp1 = bytscl(rebin(temp1, xsize, ysize),  MIN=mini, MAX=maxi, TOP=(nc-1))
         ENDIF ELSE BEGIN 
            IF (vdsp.range EQ 1) THEN $
               temp1 = bytscl(congrid(-(temp1), xsize, ysize, /INTERP), $
                              MIN=mini, MAX=maxi, TOP=(nc-1)) $
            ELSE temp1 = bytscl(congrid(temp1, xsize, ysize, /INTERP), $
                                MIN=mini, MAX=maxi, TOP=(nc-1))
         ENDELSE
         temp1 = temp1 + vdsp.bot
         tv, temp1, loc
         yloc = loc/ncol
         xloc = (loc-yloc*ncol) MOD ncol
         yloc = nrow - (yloc + 1)
         xl = (xloc+1)*xsize-(30*zoom)
         yl = yloc*ysize+10
         IF (datlas EQ 0) THEN BEGIN
            IF (planes EQ 1) THEN val = strtrim(string(f+1),2) $
            ELSE val = strtrim(string(k+1),2)
            xyouts,xl,yl,val,charsize=zoom, charthick=min([zoom,2]),/device,color=label
         ENDIF ELSE BEGIN
            ap = -1			; Determine atlas slice to display
            CASE orient OF
               0: BEGIN
                  aloc = k - porg[2]
                  ax = fix(aloc) + 33
                  xctr = zoom*(dim[0]-porg[0])
                  yctr = zoom*(dim[1]-porg[1])
                  IF ((ax GE 0) AND (ax LT (size(trans))[1])) THEN ap = trans[ax]
               END
               1: BEGIN
                  aloc = k - porg[1]
                  ax = fix(aloc) + 33
                  xctr = zoom*(dim[0]-porg[0])
                  yctr = zoom*(dim[2]-porg[2])
                  IF ((ax GE 0) AND (ax LT (size(cor))[1])) THEN ap = cor[ax]
               END
               2: BEGIN
                  aloc = porg[0] - k
                  ax = fix(abs(aloc))
                  xctr = zoom*(dim[1]-porg[1])
                  yctr = zoom*(dim[2]-porg[2])
                  IF ((ax GE 0) AND (ax LT (size(sag))[1])) THEN ap = sag[ax]
               END
            ENDCASE
            IF (ap GE 0) THEN BEGIN
               ;; Extract the subarray corresponding to the atlas, add the atlas, and write it back
               x0 = fix(xctr - adim[0] + aorg[0]) + xsize*xloc
               y0 = fix(yctr - adim[1] + aorg[1]) + ysize*yloc
               x1 = x0 + adim[0] - 1 
               y1 = y0 + adim[1] - 1
               x2 = fix(xctr - adim[0] + aorg[0])
               y2 = fix(ysize - yctr - aorg[1])
               IF ((x0 GE 0) AND (y0 GE 0) AND (x1 LT xscrn) AND (y1 LT yscrn)) THEN BEGIN
                  tmp = extrac(temp1,x2,y2,adim[0],adim[1])
                  indx = where(atlimg[ap] GT 0)
                  tmp(indx) = label
                  tv,tmp,x0,y0
               ENDIF
            ENDIF
            val = strcompress(string(fix(-2*aloc)),/remove_all)
            xyouts,xl,yl,val,charsize=2, charthick=2,/device,color=label
         ENDELSE
         loc = loc+1
      ENDFOR 
   ENDFOR 
   IF (fmode EQ 2) THEN BEGIN
      xl = 10
      yl = nrow*ysize-25
      val = 'Frame '+strcompress(string(fix(frm1+1)),/remove_all)
      xyouts,xl,yl,val,charsize=2, charthick=2,/device,color=label
   ENDIF
   IF (datlas EQ 1) THEN BEGIN
      close,lun
      free_lun,lun
   ENDIF
   
END
