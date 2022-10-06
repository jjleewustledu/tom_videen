PRO plotpvc
    
   str = " "
   name = " "
   voi = " "
   dmag = 1.2
   cmag = 1.0
   cy = -27
   dy = -30
   ay = -33
   by = -36
   xd = [-dmag, 0, dmag, 0, -dmag]
   yd = [0, dmag, 0, -dmag, 0]
   rd = findgen(32) * (!pi*2/32.)
   y = fltarr(6,4)
   openr,1,'Analysis_table'
   FOR i=1,11 DO readf,1,str
   
   FOR i=1,5 DO BEGIN
      readf,1,str
      readf,1,name
      readf,1,voi
      readf,1,str
      readf,1,y
      print,voi

      x = y(0,*)
      a = 100.*y(1,*)/y(5,*) - 100.
      b = 100.*y(2,*)/y(5,*) - 100.
      c = 100.*y(3,*)/y(5,*) - 100.
      d = 100.*y(4,*)/y(5,*) - 100.
      
      set_plot,'X'
      usersym, xd,yd,/fill
      plot,x,a,yr=[-40,10],psym=-8, linestyle=0
      oplot,[1,3],[ay, ay],psym=-8, linestyle=0
      usersym, cmag*cos(rd), cmag*sin(rd), /fill
      oplot,x,b,psym=-8, linestyle=0
      oplot,[1,3],[by, by],psym=-8, linestyle=0
      usersym, xd,yd
      oplot,x,c,psym=-8, linestyle=5
      oplot,[1,3],[cy, cy],psym=-8, linestyle=0
      usersym, cmag*cos(rd), cmag*sin(rd)
      oplot,x,d,psym=-8, linestyle=5
      oplot,[1,3],[dy, dy],psym=-8, linestyle=0
      oplot,[0,20],[0,0],linestyle=0
      xyouts,3.5,ay,'Without Clot, Uncorrected',charsize=1.0
      xyouts,3.5,by,'With Clot, Uncorrected',charsize=1.0
      xyouts,3.5,cy,'Without Clot, Corrected',charsize=1.0
      xyouts,3.5,dy,'With Clot, Corrected',charsize=1.0

      set_plot,'ps'
      fname = name+'.ps'
      device, filename=fname,xsize=7,ysize=6,xoffset=0.8,yoffset=3,/inches
      usersym, xd,yd,/fill
      plot,x,a,yr=[-40,10],psym=-8, linestyle=0, $     
         charsize=1.4,charthick=2,xthick=2,ythick=2, $
         xtitle="PET Resolution (fwhm)",ytitle="Percent Error", title=voi, font=2
      oplot,[1,3],[ay, ay],psym=-8, linestyle=0
      usersym, cmag*cos(rd), cmag*sin(rd), /fill
      oplot,x,b,psym=-8, linestyle=0
      oplot,[1,3],[by, by],psym=-8, linestyle=0
      usersym, xd,yd
      oplot,x,c,psym=-8, linestyle=5
      oplot,[1,3],[cy, cy],psym=-8, linestyle=0
      usersym, cmag*cos(rd), cmag*sin(rd)
      oplot,x,d,psym=-8, linestyle=5
      oplot,[1,3],[dy, dy],psym=-8, linestyle=0
      oplot,[0,20],[0,0],linestyle=0
      xyouts,3.5,ay-0.5,'Without Clot, Uncorrected',charsize=1.0,font=2
      xyouts,3.5,by-0.5,'With Clot, Uncorrected',charsize=1.0,font=2
      xyouts,3.5,cy-0.5,'Without Clot, Corrected',charsize=1.0,font=2
      xyouts,3.5,dy-0.5,'With Clot, Corrected',charsize=1.0,font=2

      device,/close
      
   ENDFOR 
   close,1

END 
