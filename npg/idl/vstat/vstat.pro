PRO VSTAT
@IMAGE_UTILS.h

   on_error, 2
   infile = ''
   outfile = ''
   voifile = ''
   name = ''
   isotope = ''
   msg = ''
   pie = 0.0

   infile = getenv('INFILE')
   outfile = getenv('OUTFILE')
   voifile = getenv('VOIFILE')
   name = getenv('VOINAME')
   isotope = getenv('ISOTOPE')
   pie = getenv('PIESLOPE')

   CASE isotope OF
      'F18':  tau = 0.00010529D
      'f18':  tau = 0.00010529D
      'GA68': tau = 0.0001691D
      'ga68': tau = 0.0001691D
      'C11':  tau = 0.0005668D
      'c11':  tau = 0.0005668D
      'O15':  tau = 0.005668D
      'o15':  tau = 0.005668D
      'none':  tau = 0.D
      'test':  tau = 0.D
      ELSE: BEGIN
         PRINT, 'ERROR [vstat]: unrecognized isotope; use f18, c11, o15, ga68 or none'
         RETURN
      ENDELSE
   ENDCASE

   voi = ptr_new({VOI})
   (*voi).id = 'VOI'
   IF (VOI_READ(voi, voifile, msg) NE 0) THEN BEGIN
      PRINT,'ERROR [vstat]: reading ',voifile
      RETURN
   ENDIF
   roi = VOI_LOCATE(voi, name)
   IF (NOT ptr_valid(roi)) THEN BEGIN
      PRINT,'ERROR [vstat]: cannot locate ',name,' in ',voifile
      RETURN
   ENDIF
   IF ((*roi).num LE 0) THEN BEGIN
      PRINT,'ERROR [vstat]: reading ',name,' in ',voifile
      RETURN
   ENDIF

   img = {VIMAGE}
   img.fname = infile
   img.ff = 1
   img.lf = 1
   img.fp = 1
   img.lp = 1

   IF (getnumframes(img, msg) NE 0) THEN BEGIN
      PRINT,'ERROR [vstat]: reading ',infile
      RETURN
   ENDIF
   ff = img.ff
   lf = img.lf
   nfrm = lf-ff+1
   nidx = lf+1

   img.ff = ff
   img.lf = ff
   img.fp = 1
   img.lp = 1
   IF (getsingleframe(img, msg) NE 0) THEN BEGIN
      PRINT,'ERROR [vstat]: reading ',infile
      RETURN
   ENDIF

   IF (pie EQ 0 AND nfrm GT 1) THEN BEGIN 
      isotope = 'none'
      tau = 0.D
   ENDIF

   pixels = VOI_COORDS(voi, name, img.dim, msg)
   IF ((size(pixels))[2] LE 0) THEN BEGIN
      PRINT,'ERROR [vstat]: VOI ',roi,' has no defined pixels'
      RETURN
   ENDIF
   pixels = pixels[sort(pixels)]
   pixels = pixels[uniq(pixels)]
   n = n_elements(pixels)
   IF (n LE 0) THEN BEGIN
      PRINT,'ERROR [vstat]: VOI ',roi,' has no defined pixels'
      RETURN
   ENDIF

   zerotime = float(img.start[ff])/1000.
   print, format = '("First image frame (#",I1,") began at ",F9.3," sec")', ff, zerotime
   read,zerotime,prompt='Delay (sec) of first frame from injection?  '
   start0 = zerotime - float(img.start[ff])/1000. 

   print, '---------------------------------------------'
   print, 'vstat 2008:08:05  ',systime(0)
   print, format = '("Image File:  ",A,3X,I0,"x",I0,"x",I0,"   ",I0," frames")', infile, img.dim[0],img.dim[1],img.dim[2],nfrm
   IF (isotope NE 'none') THEN BEGIN 
      print, format = '("Isotope:     ",A,"   decay constant =",F11.8," sec")', isotope, tau
      print, format = '("Pieslope:  ",F8.4)', pie
   ENDIF
   print, format = '("VOI:  ",A,3X,I0," voxels  (",A,")")', name, n, voifile

   openw,lu1, outfile, ERROR=err, /GET_LUN
   IF (err NE 0) THEN BEGIN
      PRINT,'ERROR [vstat]: Cannot open ',outfile
      RETURN
   ENDIF
   IF (isotope EQ 'test') THEN BEGIN 
      printf, lu1, format = '(A8,A14,A13,A15)', 'Filename','Start','Duration','PETcnts/frame'
   ENDIF ELSE IF (pie EQ 0) THEN BEGIN 
      printf, lu1, format = '("30  no decay correction   VOI: ",A," (",A,")")', name, infile
      printf, lu1, format = '("1 3   Start  Duration        Mean")'
   ENDIF ELSE IF (isotope EQ 'none') THEN BEGIN 
      printf, lu1, format = '("30  no decay correction   VOI: ",A," (",A,")")', name, infile
      printf, lu1, 'vstat 2008:08:11  ',systime(0)
      printf, lu1, format = '("Image File:  ",A,3X,I0,"x",I0,"x",I0,"   ",I0," frames")', infile, img.dim[0:2],nfrm
      printf, lu1, format = '("Isotope:     ",A,"     decay constant =",F11.8," sec")', isotope, tau
      printf, lu1, format = '("Pieslope: ",F9.4,"  no decay correction")', pie
      printf, lu1, format = '("VOI:  ",A,3X,I0," voxels  (",A,")")', name, n, voifile
      IF (pie EQ 1) THEN BEGIN
         printf, lu1, format = '(A9,A20,A9,A11,A17)', 'Mid_Time','PETcnts/sec','Start','Duration','Decay_Cor_Factor'
      ENDIF ELSE BEGIN
         printf, lu1, format = '(A9,A15,A14,A11,A17)', 'Mid_Time','Wellcnts','Start','Duration','Decay_Cor_Factor'
      ENDELSE
   ENDIF ELSE BEGIN
      printf, lu1, 'vstat 2008:08:11  ',systime(0)
      printf, lu1, format = '("Image File:  ",A,3X,I0,"x",I0,"x",I0,"   ",I0," frames")', infile, img.dim[0:2],nfrm
      printf, lu1, format = '("Isotope:     ",A,"     decay constant =",F11.8," sec")', isotope, tau
      printf, lu1, format = '("Pieslope: ",F9.4,"  decay-corrected")', pie
      printf, lu1, format = '("VOI:  ",A,3X,I0," voxels  (",A,")")', name, n, voifile
      printf, lu1, format = '(A9,A15,A14,A11,A13)', 'Mean_Time','Wellcnts','Start','Duration','Mean_Decay'
   ENDELSE
   t0 = fltarr(nidx)
   tm = fltarr(nidx)
   cnts = fltarr(nidx)
   mean = fltarr(nidx)
   print, format = '("frm   decay_cor")'

   ;; getsingleframe reads decay-corrected counts/frame from Matrix 6 and Matrix 7 files
   ;; for uncorrected counts, multiply by EXP(-tau*len) -- if isotope is specified

   FOR k = ff, lf DO BEGIN 
      img.ff = k
      img.lf = k

      IF (getsingleframe(img, msg) NE 0) THEN BEGIN
         PRINT,'ERROR [vstat]: reading ',infile,' frame ',k
         RETURN
      ENDIF
      IF (img.itype EQ MATRIX_REV7) THEN decay_cor = img.h.sh7.decay_corr_fctr $
      ELSE IF (img.itype EQ MATRIX_REV6) THEN decay_cor = img.h.sh6.decay_corr_fctr
      print, format = '(I2, F12.4)', k, decay_cor

      t0[k] = float(img.start[k])/1000. + start0
      len = float(img.len[k])/1000.
      val = (*img.data)[pixels]
      IF (len GT 0) THEN BEGIN
         IF (isotope EQ 'test') THEN BEGIN 
            tm[k] = t0[k] + len/2.
            mean[k] = (moment(val))[0]
            printf, lu1, format = '(A10, F12.0,F10.0,F18.10)', infile, t0[k], len, mean[k]
         ENDIF ELSE IF (pie EQ 0) THEN BEGIN
            cnts[k] = (moment(val))[0]
            printf, lu1, format = '(F12.0,F10.0,F18.6)', t0[k], len, cnts[k]
         ENDIF ELSE IF (isotope EQ 'none') THEN BEGIN
         	scl = 1/len
            tm[k] = t0[k] + len/2.
            cnts[k] = (moment(val))[0]
            mean[k] = scl * cnts[k] * pie
            printf, lu1, format = '(F9.2,F17.6,F12.0,F10.0,F12.4)', tm[k], mean[k], t0[k], len, decay_cor
         ENDIF ELSE BEGIN
			;; Note: all units are per pixel
			;; pie = (wellcnts/cc/sec)/(PETcnts/min) 
			;; let: WELLcnts = (wellcnts/cc/sec)
			;; then: pie = WELLcnts*min/PETcnts
			;; from image: meanPET = PETcnts/frame
			;; using: len=sec/frame;  60=sec/min
			;; want WELLcnts = wellcnts/cc/sec 
			;; therefore: WELLcnts = (PETcnts/frame)*(pie)*(sec/min)/(sec/frame)
			;;   = (PETcnts/frame)*(WELLcnts*min/PETcnts)*(sec/min)*(frame/sec)= meanPET*60*pie/len
         	scl = 60*pie/len
            tmid = (-1.0/tau)*ALOG(0.5*(EXP(-tau*len)+1.0))
            tm[k] = t0[k] + tmid
            cnts[k] = (moment(val))[0]
            mean[k] = scl * cnts[k] * EXP(tau*tm[k])
            printf, lu1, format = '(F9.2,F17.6,F12.0,F10.0,F12.4)', tm[k], mean[k], t0[k], len, EXP(tau*tm[k])
         ENDELSE
      ENDIF
   ENDFOR 
   close,lu1
   free_lun,lu1
   IF (nfrm EQ 1) THEN RETURN
   IF (pie LE 0) THEN RETURN

   ymin = min(mean)
   ymax = max(mean)
   xlabel = 'Time (sec)'
   IF (isotope EQ 'none') THEN ylabel = 'Uncorrected Well Cnts/cc/sec' $
   ELSE ylabel = 'Decay-Corrected Counts/Sec'

   plot, tm[ff:lf], mean[ff:lf], charsize=1.4,charthick=1,xthick=2,ythick=2, $
      xtitle=xlabel, ytitle=ylabel, yrange=[ymin,ymax]
   a = ''
   read,a,prompt='Continue '

END 
