PRO intraplot2
   str = 'unif_970410_f2to6_2rois.cpt'
   nplanes = 31
   openr,lu,str,/get_lun
   minv = fltarr(nplanes)
   maxv = fltarr(nplanes)
   dif = fltarr(nplanes)
   
   FOR i=1,2 DO readf, lu, str
   FOR p=0,nplanes-1 DO BEGIN
      readf,lu,plane,val
      minv(p)=val
   ENDFOR
   FOR i=1,3 DO readf, lu, str
   FOR p=0,nplanes-1 DO BEGIN
      readf,lu,plane,val
      maxv(p)=val
      dif(p) = 100*(maxv(p)-minv(p))/minv(p)
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


   
   
