pro plotalign
   nm = 4                       ; number of measurements (mean, x, y, z)
   nc = 43                      ; number of comparisons

   x = fltarr(nm, nc)        ; [comparison, measurement]
   str = ''
   pchar = ''

new:
   read,prompt='File: ',str
   fname = str+'.dat'
   openr,1,fname
   readf,1,x
   close,1
   n = fltarr(nc)
   FOR j=0,nc-1 DO BEGIN
      FOR i=0,nc-1 DO n[j]=j+1
   ENDFOR
   xmax =  max(x, min=xmin)
   print,xmax,xmin
   read,prompt='Max: ',xmax
   xmin = -xmax
   
   set_plot,'X'
   loadct,0

save_ps:
   IF (pchar EQ 'p') THEN BEGIN
      set_plot,'ps'
      fname = str+'.ps'
      device,filename=fname,xsize=7,ysize=6,xoffset=0.8,yoffset=3,/inches
   ENDIF
   
   cmag = 1.0
   rd = findgen(32) * (!pi*2/32.)
   usersym, cmag*cos(rd), cmag*sin(rd), /fill

   plot,n[*],x[0,*],yr=[xmin,xmax],xr=[0,50], psym=-8, $
      charsize=1.4, charthick=2, xthick=2, ythick=2, $
      xtitle="Frame",ytitle="Difference (mm)", $
      title="Alignment Error ("+str+")"
   oplot,n[*],x[1,*],psym=-1,linestyle=1
   oplot,n[*],x[2,*],psym=-2,linestyle=1
   oplot,n[*],x[3,*],psym=-5,linestyle=1
   oplot,[0,50],[0,0],linestyle=0  
   
   inc = float(xmax)/8.0
   oplot,[3],[xmin+5*inc],psym=8
   oplot,[3],[xmin+4*inc],psym=1
   oplot,[3],[xmin+3*inc],psym=2
   oplot,[3],[xmin+2*inc],psym=5
   xyouts,4,xmin+5*inc,'Mean',charsize=1.5
   xyouts,4,xmin+4*inc,'X',charsize=1.5
   xyouts,4,xmin+3*inc,'Y',charsize=1.5
   xyouts,4,xmin+2*inc,'Z',charsize=1.5
   IF (pchar EQ 'p') THEN device,/close
   read,prompt='print, continue or exit [p,c,x]: ',pchar
   IF (pchar EQ 'c') THEN goto, new
   IF (pchar EQ 'p') THEN goto, save_ps
END 
