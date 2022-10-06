FUNCTION testpix, im, i, j
   ;; Test 4 directions at im[i,j]
   ;; return array set to 1 where direction is > 0
   
   test = intarr(4)
   test[0] = im[i+1, j] GT 0     ; east
   test[1] = im[i, j-1] GT 0     ; south
   test[2] = im[i-1, j] GT 0     ; west
   test[3] = im[i, j+1] GT 0     ; north
   RETURN, test
END

FUNCTION evalpix, im, i, j, direction
   ;; Get neighboring pixels values
   
   test = testpix(im, i, j)
   eval = (5 + direction - indgen(4)) MOD 4 + 1 ; evaluation mask
   eval = eval*test             ; blank out zero values
   minval = min(eval(WHERE(eval GT 0)))
   newdir = WHERE(eval EQ minval)
   RETURN, newdir[0]
END

PRO getpoly, im, i, j, x, y, XOFF=xoff, YOFF=yoff
   ;; Automatic generation of boundaries

   IF (n_elements(XOFF) EQ 0) THEN xoff = 0
   IF (n_elements(YOFF) EQ 0) THEN yoff = 0
   IF (im[i-xoff,j-yoff) GE 1) THEN stop,'Already on boundary.'

   WHILE (im[i-xoff,j-yoff] LT 1) DO j = j+1 ; search FOR boundary
   i0 = i-xoff                  ; save i,j pair
   j0 = j-yoff
   x = i0                       ; initialize x, y, direction
   y = j0
   dir = 0                      ; 0=E, 1=S, 2=W, 3=N
   im[i-xoff,j-yoff] = 2
   steps = 0L

   ;; Crawl along boundary: Test if pixel at right is boundary, 
   ;; if not, test pixel straight ahead, then pixel at left, finally move back
   REPEAT BEGIN
      IF (steps MOD 200 EQ 0) THEN tv,im
      res = evalpix(im, i-xoff, j-yoff, dir)
      CASE res OF
         0 :  i = i+1           ; east
         1 :  j = j-1           ; south
         2 :  i = i-1           ; west
         3 :  j = j+1           ; north
      ENDCASE 
      x = [x, i-xoff]
      y = [y, j-yoff]
      steps = steps+1
      dir = res
      im[i-xoff,j-yoff] = 2
   ENDREP UNTIL ((i-xoff EQ i0 AND j-yoff EQ j0) OR steps GE 20000)
   tv,im                        ; final display
   print,steps ,' used FOR boundary definition.'
   RETURN
END


PRO picklist,filename,SCALE=SCALE,start=start,XOFF=xoff,YOFF=yoff, exclude=exclude
   ;; Interactive picking of country coordinates with mouse

   IF (n_elements(xoff) EQ 0) THEN xoff = 0
   IF (n_elements(yoff) EQ 0) THEN yoff = 0
   IF (n_elements(exclude) EQ 0) THEN exclude = 0
   IF (n_elements(start) EQ 0) THEN start = 0L
   IF (n_elements(SCALE) EQ 0) THEN SCALE = 1
   ascale = float(SCALE)

   ;; Create two files interactively:
   ;; a country file with starting points FOR each country
   ;; a name file with country numbers and names

   openw,ilun,filename+'.dat',/get_lun
   printf,ilun,'COUNTRY   X   Y'
   openw,olun,filename+'.names',/get_lun

   ;; use CURSOR to pick points
   print,'left click : the last point defining a country or an exclusion'
   print,'middle click : not the last point ... (e.g .FOR islands)'
   print,'right click : no more input (does not produce an entry !)'
   print

   mx=0  &  my=0
   status = -1
   count = long(start)          ; country counter
   cname = ''
   read,cname,prompt='COUNTRY : '
   printf,olun,count,cname,format='(i4,1X,A)'

   WHILE (status NE 4) DO BEGIN
      print,count,cname,format='(i4,A)'
      cursor,mx,my,/down,/device
      status = !err
      IF (status LT 4) THEN $
         printf,ilun,count,mx/ascale+xoff,my/ascale+yoff,format='(i6,2f10.3)'
      IF (status EQ 1) THEN BEGIN
         count = count + 1
         read,cname,prompt='COUNTRY : '
         IF (cname EQ '') THEN BEGIN
            close,/all
            RETURN
         ENDIF
         printf,olun,count,cname,format='(i4,1X,A)'
      ENDIF
   ENDWHILE
   close,/all
   RETURN
END


;; ==============  COUNTRIES : MAIN PROGRAM =====================
;;   typical calling sequence:
;;        countries,continent="northamerica",start=1
;;        countries,continent="northamerica",/fillit
;;        countries,/merge

PRO countries,continent=continent,start=start,fillit=fillit,exclude=exclude, merge=merge

   ;; create region -> grid maps and country -> grid maps
   ;; NEW version : uses pre-stored GIF images FOR country boundaries

   SCALE = 8                    ; 2 makes it a 0.5x0.5 grid, 4 gives better resolution
                                ; SCALE must be identical to that used in CONTINENTS.PRO !!

   ;; for simplicity, the window size is the same for all continents, only
   ;; the offset shifts - always draw complete map into pixmap but THEN copy
   ;; only continent region

   XOFF = 0  &  YOFF = 0

   IF (n_elements(continent) EQ 0) THEN continent = "northamerica"
   continent = strlowcase(continent)

   ;; for merge option: define continent names as array and create loop
   ;; other calls willloop only once

   IF (keyword_set(merge)) THEN BEGIN
      cnnames = [ 'northamerica', 'southamerica', 'europe', 'asia', 'australia', 'africa' ]
      ic0 = 0
      ic1 = 5
      grandim = intarr(SCALE*360,SCALE*180) ; result image array
   ENDIF ELSE BEGIN
      ic0 = 0
      ic1 = 0
   ENDELSE

   FOR ic = ic0,ic1 DO BEGIN
      ;; IMPORTANT : the following offsets must be identical with those 
      ;; given in CONTINENTS.PRO !!!
      IF (keyword_set(merge)) THEN continent = cnnames(ic)

      CASE continent OF
         "northamerica" : BEGIN
            XOFF = 0
            YOFF = 90
         END
         "southamerica" : BEGIN
            XOFF = 45
            YOFF = 20
         END
         "africa"       : BEGIN
            XOFF = 120
            YOFF = 50
         END
         "europe"       : BEGIN
            XOFF = 90
            YOFF = 90
         END
         "asia"         : BEGIN
            XOFF = 225
            YOFF = 90
         END
         "australia"    : BEGIN
            XOFF = 225
            YOFF = 30
         END
         ELSE            : message,'invalid continent!'
      ENDCASE 

      XSIZE = 135
      YSIZE = 90                ; plot size in degrees (multiply by SCALE)

      ;; make sure that complete plotting area is used
      !p.position=[0,0,1,1]

      IF (not keyword_set(merge)) THEN GOTO,no_merge


      ;; ------   MERGING SECTION    -------------------------------------

      ;; read in image with filled continents
      read_gif,continent+'.gif',im

      ;; convert all non-white, non-country pixels to white
      ind = WHERE(im LT 3)
      IF (ind[0] GE 0) THEN im[ind] = 0

      print,ic,' : ',max(im)

      ;; add partial image to total image
      grandim(SCALE*XOFF:SCALE*(XOFF+XSIZE)-1,SCALE*YOFF:SCALE*(YOFF+YSIZE)-1) = $
         grandim(SCALE*XOFF:SCALE*(XOFF+XSIZE)-1,SCALE*YOFF:SCALE*(YOFF+YSIZE)-1) + $
         fix(im)
   ENDFOR                       ; ic loop for merge

   print,'grandim:',max(grandim)
   loadct,27,bottom=1           ; load new color table
   tvlct,r,g,b,/get

   IF (!D.n_colors LT 170) THEN print,'WARNING : not enough colors !'

   ;; write complete image as gif file
   write_gif,'world_countries_large.gif',byte(grandim),r,g,b

   ;; condense image by aggregating 4x4 pixels
   ;; this gives 0.5x0.5 degree resolution
   ;; NOTE: tried rebin before, but this leads to unpleasant white lines
   ;; therefore have to go through each 4x4 block

   tmp = intarr(720,360)
   rescale = fix(SCALE/2)

   print,'Condensing image ... please be patient!'

   FOR i=0,719 DO BEGIN
      FOR j=0,359 DO BEGIN
         block = grandim(i*rescale:(i+1)*rescale-1,j*rescale:(j+1)*rescale-1)
         code = 0
         IF (max(block) GT 0) THEN BEGIN ; found a country
                                ; determine country code with maximum portion
            u = block(uniq(block,sort(block)))
            u = u(WHERE(u GT 0))
            FOR n=0,n_elements(u)-1 DO BEGIN
               testind = WHERE(block EQ u[n], count)
               IF (count GT code) THEN code = u[n] 
            ENDFOR
         ENDIF                  ; else only water
         tmp[i,j] = code
      ENDFOR
      IF ((i MOD 36) EQ 0) THEN tv,tmp
   ENDFOR

   ;; shift by 10 degrees west in order to start at -180
   newim = [ tmp[700:719,*], tmp[0:699,*] ]

   ;; move all colors down 3 so that first country starts at 1
   ind = WHERE(newim GT 2)
   newim[ind] = newim[ind]-3
   print,max(newim)
   tv,newim

   RETURN

   ;;------------------------------------------------------------------ 

no_merge:
   ;; load continent
   read_gif,continent+'.empty.gif',im 
   ind = WHERE(im GT 0) 
   IF (ind[0] GE 0) THEN im[ind] = 1

   ;; open a display window and show map
   window,xsize=XSIZE*SCALE,ysize=YSIZE*SCALE
   curwin = !d.window

   tv,im
   IF (keyword_set(fillit)) THEN GOTO,fillit
   
   ;; create picklist
   IF (keyword_set(exclude)) THEN $
      picklist,strlowcase(continent)+'.exclude',scale=SCALE,XOFF=xoff,YOFF=yoff,/EXCLUDE $
   ELSE $
      picklist,strlowcase(continent)+'.countries',scale=SCALE,start=start,XOFF=xoff,YOFF=yoff
   RETURN
   
fillit:                      
   ;; draw black frame around image
   s = size(im)                 
   im[0:1,*] = 1
   im[s[1]-2:s[1]-1,*]=1
   im[*,0:1] = 1
   im[*,s[2]-2:s[2]-1] = 1
   readdata, strlowcase(continent)+'.countries.dat',data,cols=3, /noheader,skp1=1
   c_col = reform(data[0,*])
   iind = reform(data[1,*])
   jind = reform(data[2,*])

   IF (n_elements(start) EQ 0) THEN start = 0

   FOR i = 0,n_elements(iind)-1 DO BEGIN
      IF (c_col[i] GE start) THEN BEGIN 
         getpoly, im, fix(iind[i]*SCALE), fix(jind[i]*SCALE), x, y, $
            xoff=xoff*SCALE, yoff=yoff*SCALE
         tv,im                  ; and display manipulated image
         polyfill,x,y,/device,/fill,color=c_col[i]+3
         im = tvrd()            ; read image with filled continents from display
      ENDIF
   ENDFOR

   RETURN
END
