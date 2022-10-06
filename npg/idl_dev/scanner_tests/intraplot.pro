PRO intraplot
   str = 'unifwob_f2to7_intraall.dat'
   nplanes = 31
   openr,lu,str,/get_lun
   dif = fltarr(nplanes)
   
   readf, lu, str
   FOR p=0,nplanes-1 DO BEGIN
         readf,lu,plane,max,min,val
         dif(p)=val
   ENDFOR
   close,lu
   planes = indgen(31)
   name = "953B Intraplane Uniformity"
   plot,planes,dif,xrange=[0,30],yrange=[0.0,20.],psym=-1,$
      xtitle='Plane', ytitle='(Max-Min)/Min %',charsize=1.5, $
      charthick=2,xthick=2,ythick=2, title=name
   oplot,[0,30],[8.,8.],linestyle=2

   read,prompt='Postscript File? ',str
   IF ((strpos(str,'Y') GE 0) OR (strpos(str,'y') GE 0)) THEN BEGIN
      read,prompt='Filename? ',str
      set_plot,'ps'
      name = "953B Intraplane Uniformity"
      device, filename=str,xsize=7,ysize=6,xoffset=0.8,yoffset=3,/inches
      plot,planes,dif,xrange=[0,30],yrange=[0.0,20.],psym=-1,$
         xtitle='Plane', ytitle='(Max-Min)/Min %',charsize=1.5, $
         charthick=2,xthick=2,ythick=2, title=name
      oplot,[0,30],[8.,8.],linestyle=2
      device,/close
   ENDIF
   set_plot,'X'
END


   
   
