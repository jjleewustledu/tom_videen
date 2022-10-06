PRO COS_REPORT
@IMAGE_UTILS.h

   on_error, 2
   study = getenv('STUDY')
   scan = getenv('SCAN')
   oefmax = 100
   oefmax = getenv('OEFMAX')
   studyname = [study+'-ho'+scan+'.msk', study+'-oo'+scan+'.msk', study+'-oo'+scan+'ho'+scan+'.msk']
   desc = ['Oxygen-15 Water Blood Flow Image', $
           'Oxygen-15 Oxygen Metabolism Image', $
           'Count-Based Oxygen Extraction Image']

   device, decompose=0, retain=2
   device, get_visual_name=name, get_visual_depth=depth

   ;; Read first image to get dimensions

   msg = ''
   img = {VIMAGE}
   img.ff = 1
   img.lf = 1
   img.fp = 1
   img.lp = 38
   img.proc = 0
   img.fname = studyname[0]
   IF (getframe(img, msg) NE 0) THEN BEGIN
      print, msg
      RETURN
   ENDIF

   ;; Display Initialization
   
   ipln = [20,23,25,31,32,36]
   zoom = 2
   nrow = 3
   ncol = 6
   xshift = 52
   xsize = img.dim[0] * zoom
   xstep = xsize-xshift
   label_offset = 50
   xoffset = 10
   ysize = img.dim[1] * zoom
   ystep = ysize
   xscrn = ncol*xsize - (ncol-1)*xshift -xoffset
   yscrn = nrow*ysize + label_offset
   
   viewbase = widget_base(TITLE='VOI Display', group_leader=tlb)
   win = widget_draw(viewbase, xsize=xscrn, ysize=yscrn)
   !p.background = 0
   widget_control, viewbase, /realize, xoffset=0, yoffset=0
   widget_control, win, get_value=windowIndex
   
   ;; Load color table + black

   nc = 20
   ;loadct, 36, ncolors=nc, bottom=0, file='/usr/local/npg/idl/color_tables/videen.tbl'
   color44,r,g,b,nc
   tvlct,r,g,b
   red    = rotate([255,200,0],4)
   black  = rotate([0,0,0],4)
   tvlct, red, nc
   tvlct, black, nc+1
   
   ;; Display 6 PET Planes for 3 images

   maxoef = string(format='(F3.1)', oefmax/100.)
   maxlab = ['Max','Max',maxoef]
   minlab = ['0','0','0']
   FOR n=0, 2 DO BEGIN
      img.fname = studyname[n]
      IF (getframe(img, msg) NE 0) THEN BEGIN
         print, msg
         RETURN
      ENDIF
      y0 = (nrow-n-1)*ystep + label_offset
      FOR k=0, 5 DO BEGIN
         image = reform((*img.data)[*,*,ipln[k]])
         IF (n EQ 2) THEN maxi=oefmax ELSE maxi = max(image)
         temp = bytscl(rebin(image, xsize, ysize), MIN=0, MAX=maxi, TOP=(nc-1))
         x0 = xstep*k -xoffset
         tv, temp, x0, y0
      ENDFOR 

      ;; Color Bar
      cbar = rotate(findgen(nc),3)
      yn = ysize/20
      temp = rebin(cbar, 20, 20*yn, /sample)
      tv,temp,xscrn-25,y0

      str = maxlab[n]
      ytxt = y0 + ystep - 35
      xyouts, xscrn-30, ytxt, str, charsize=2, charthick=2,/device,color=nc+1, alignment=1.0
      str = minlab[n]
      ytxt = y0 + 10
      xyouts, xscrn-30, ytxt, str, charsize=2, charthick=2,/device,color=nc+1, alignment=1.0

      ;; Label
      ytxt = y0 + ystep - 25
      str = desc[n]+'  ('+study+')'
      xyouts, 300, ytxt, str, charsize=2, charthick=2,/device,color=nc+1
      xyouts, 50, y0+10, 'L', charsize=2, charthick=2,/device,color=nc+1
      xyouts, xstep-30, y0+10, 'R', charsize=2, charthick=2,/device,color=nc+1
   ENDFOR 

   ;; Label Slices

   zloc = ['Z=34','Z=28','Z=24','Z=12','Z=10','Z=2']
   ytxt = label_offset - 30
   FOR k=0, 5 DO BEGIN
      xtxt = k*xstep + 100
      xyouts, xtxt, ytxt, zloc[k], charsize=2, charthick=2,/device,color=nc+1
   ENDFOR

   ch = ''
   read,prompt='Save Image and Exit ',ch
   arr = tvrd()
   IF (depth EQ 24) THEN BEGIN
      xarr = (size(arr))[1]
      yarr = (size(arr))[2]
      rgb = lonarr(3,xarr,yarr)
      rgb[0,*,*] = tvrd(0,0,xarr,yarr,1)
      rgb[1,*,*] = tvrd(0,0,xarr,yarr,2)
      rgb[2,*,*] = tvrd(0,0,xarr,yarr,3)
      fname = study + '-report.tif'
      write_tiff,fname,rgb,compression=1
      print, 'Window saved in TIF format:  '+fname
   ENDIF ELSE BEGIN
      tvlct,r,g,b,/get
      fname = study + '-report.gif'
      write_gif, fname, rotate(tvrd(),7), r,g,b
      print, 'Window saved in GIF format:  '+fname
   ENDELSE

END 
