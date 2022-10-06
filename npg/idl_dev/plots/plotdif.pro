pro plotdif
   nm = 4                       ; number of measurements (mean, x, y, z)
   nr = 5                       ; number of reference images (1,2,3,4,5,6)
   nc = nr-1                    ; number of comparisons (1-2, 1-3, 1-4, 1-5, 1-6)

   data = fltarr(nc, nm, nr)    ; [comparison, measurement, reference]
   openr,1,'ordered_dif.dat'
   readf,1,data
   close,1
   n = fltarr(nr,nc)            ; [reference, comparison]
   FOR j=0,nr-1 DO BEGIN
      FOR i=0,nc-1 DO n[j,i]=j+1 + i*0.02
   ENDFOR 
   ref = fltarr(nm, nc)          ; [measurement, comparison]
   FOR k=0,nc-1 DO BEGIN
      FOR j=0,nm-1 DO BEGIN 
         ref[j,k] = (data[k,j,k] + data[k,j,k+1])/2
      ENDFOR
   ENDFOR
   x = fltarr(nr,nc)            ; [reference, comparison]
   FOR k=0,nr-1 DO BEGIN
      FOR j=0,nc-1 DO BEGIN 
         x[k,j] = mean(data[j,*,k]-ref[*,j])
      ENDFOR
   ENDFOR
   xmax =  max(x, min=xmin)
   print,xmax,xmin
   if (xmax lt 0.6) then xmax = 0.6
   if (xmin gt -0.6) then xmin = -0.6

   set_plot,'X'	
   loadct,0
   ;;set_plot,'ps'
   ;;device,filename="p5355fdg_align.ps",xsize=7,ysize=6,xoffset=0.8,yoffset=3,/inches
   
   ;; for each of nr references, plot nc comparisons
   ;; n[reference,index] vs. x[comparison,reference]
   
   cmag = 1.0
   rd = findgen(32) * (!pi*2/32.)
   usersym, cmag*cos(rd), cmag*sin(rd), /fill

   plot,n[*,0],x[*,0],yr=[xmin,xmax],xr=[0,nr+1], psym=-8, $
      charsize=1.4, xticks=nr+1, xminor=1, charthick=2, xthick=2, ythick=2, $
      xtitle="Reference Scan",ytitle="Difference (mm)", $
      title="Difference in Alignment (p5355fdg)"
   oplot,n[*,1],x[*,1],psym=-1,linestyle=1
   oplot,n[*,2],x[*,2],psym=-2,linestyle=1
   oplot,n[*,3],x[*,3],psym=-5,linestyle=1
   oplot,[0,nr+1],[0,0],linestyle=0  
   
   oplot,[nr-0.5],[xmin+0.28],psym=8
   oplot,[nr-0.5],[xmin+0.22],psym=1
   oplot,[nr-0.5],[xmin+0.16],psym=2
   oplot,[nr-0.5],[xmin+0.10],psym=5
   xyouts,nr-0.3,xmin+0.28,'Mean',charsize=1.5
   xyouts,nr-0.3,xmin+0.22,'X',charsize=1.5
   xyouts,nr-0.3,xmin+0.16,'Y',charsize=1.5
   xyouts,nr-0.3,xmin+0.10,'Z',charsize=1.5
   ;;device,/close
END 
