PRO MCAVOI_PIC
@IMAGE_UTILS.h

   on_error, 2
   scan1 = getenv('SCAN1')
   scan2 = getenv('SCAN2')
   scan3 = getenv('SCAN3')
   vname = getenv('VOI')
   tif   = getenv('TIF')
   scanname = [scan1, scan2, scan3]

;; Read first image to get dimensions

   msg = ''
   img = {VIMAGE}
   img.ff = 1
   img.lf = 1
   img.fp = 1
   img.lp = 38
   img.proc = 0
   img.fname = scanname[0]
   IF (getframe(img, msg) NE 0) THEN BEGIN
      print, msg
      RETURN
   ENDIF

;; Read VOI

   openr,lun, vname, ERROR=err, /GET_LUN
   IF (err NE 0) THEN BEGIN
      print, 'Cannot open '+vname
      RETURN
   ENDIF
   peaks = {PEAK_STR, $
            id: '', $
            x:  0, $
            y:  0, $
            z:  0 $
           }

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

   FOR n=0,npeaks-1 DO BEGIN
      id = peak[n].id
      x = peak[n].x
      y = peak[n].y
      z = peak[n].z
   ENDFOR

;; Display Initialization
   
   ipln = [21,24,26,31,33,37]
   zloc = ["Z=34","Z=28","Z=24","Z=12","Z=10","Z=2"]
   zoom = 2
   nrow = 3
   ncol = 6
   fname_offset = 60
   voi_name_offset = 30
   z_loc_offset = voi_name_offset + 50
   img_offset = voi_name_offset + fname_offset
   xshift = 52
   xsize = img.dim[0] * zoom
   xstep = xsize-xshift
   ysize = img.dim[1] * zoom
   ystep = ysize
   xscrn = ncol*xsize - (ncol-1)*xshift
   yscrn = nrow*ysize + img_offset 
   
   viewbase = widget_base(TITLE='VOI Display', group_leader=tlb)
   win = widget_draw(viewbase, xsize=xscrn, ysize=yscrn)
   !p.background = 0
   widget_control, viewbase, /realize, xoffset=0, yoffset=0
   widget_control, win, get_value=windowIndex
   
;; Load color table + yellow + white 

   nc = 20
   loadct, 36, ncolors=nc, bottom=0, file="/usr/local/npg/idl/color_tables/videen.tbl"
   red    = rotate([255,200,0],4)
   yellow = rotate([255,255,0],4)
   white  = rotate([255,255,255],4)
   tvlct, red, nc
   tvlct, white, nc+1
   
;; Display 6 PET Planes for 3 images

   FOR n=0, 2 DO BEGIN
      img.fname = scanname[n]
      IF (getframe(img, msg) NE 0) THEN BEGIN
         print, msg
         RETURN
      ENDIF
      FOR k=0, 5 DO BEGIN
         image = reform((*img.data)[*,*,ipln[k]])
         IF (n NE 0) THEN maxi=100 ELSE maxi = max(image)
         temp = bytscl(rebin(image, xsize, ysize), MIN=0, MAX=maxi, TOP=(nc-1))
         x0 = xstep*k
         y0 = (nrow-n-1)*ystep + img_offset 
         tv, temp, x0, y0
      ENDFOR 
      IF (n NE 0) THEN str = img.fname + '    Max = 1.00' $
      ELSE str = img.fname + '    Max = ' + strtrim(string(max(*img.data)),2)
      ytxt = (nrow-n)*ystep + fname_offset
      xyouts, 10, ytxt, str, charsize=2, charthick=2,/device,color=nc+1
   ENDFOR 
   
   ytxt = z_loc_offset
   FOR k=0, 5 DO BEGIN
      xtxt = k*xstep + 100
      xyouts, xtxt, ytxt, zloc[k], charsize=2, charthick=2,/device,color=nc+1
   ENDFOR 

;; Display VOIs 
   
   npts = 32
   radius = 5
   a = findgen(npts+1) * (2 * !pi/(npts-1))
   FOR k=0, npeaks-1 DO BEGIN
      id = peak[k].id
      x  = peak[k].x
      y  = peak[k].y
      z  = peak[k].z
      CASE z of
         21: dsp = 0
         24: dsp = 1
         26: dsp = 2
         31: dsp = 3
         33: dsp = 4
         37: dsp = 5
      ENDCASE
      xy = zoom * transpose([[x + radius * cos(a)], [y + radius * sin(a)]])
      xy[0,*] = xy[0,*] + xstep*dsp
      xy[1,*] = ysize - xy[1,*] + img_offset 
      plots, xy, /device, color=nc+1
      xy[1,*] = xy[1,*] + ysize 
      plots, xy, /device, color=0
      xy[1,*] = xy[1,*] + ysize 
      plots, xy, /device, color=0

      nam = strmid(id, 6, 3)
      IF (strpos(nam,'cs') EQ 0) THEN BEGIN
         ytxt = voi_name_offset+10
         xtxt = xstep*dsp + 120
      ENDIF ELSE IF (strpos(nam,'gsm') EQ 0) THEN BEGIN
         ytxt = voi_name_offset-10
         xtxt = xstep*dsp + 110
      ENDIF ELSE BEGIN
         ytxt = voi_name_offset 
         xtxt = xstep*dsp + 120
      ENDELSE
      xyouts, xtxt, ytxt, nam, charsize=2, charthick=2,/device,color=nc+1
   ENDFOR

PROMPT:
   ch = ''
   read,prompt='S = Save as TIF; Return= Exit ',ch
   IF (ch EQ 'S' OR ch EQ 's') THEN BEGIN
      a = tvrd() 
      rgb = lonarr(3,(size(a))[1],(size(a))[2])
      rgb[0,*,*] = tvrd(0,0,(size(a))[1],(size(a))[2],1)
      rgb[1,*,*] = tvrd(0,0,(size(a))[1],(size(a))[2],2)
      rgb[2,*,*] = tvrd(0,0,(size(a))[1],(size(a))[2],3)
      write_tiff,tif,rgb,compression=1
      print, 'Window saved in TIF format:  '+tif
      goto, PROMPT
   ENDIF

END 
