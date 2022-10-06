; Interplane Uniformity Test
; Uses output file from Siemens Countrate Test (crtest.lis)
; If this file is corrupted (e.g. NaN's) then correct with editor

PRO uniftest
   str = 'scat2d_970408_oldrec.cpt'
   nrois = 2
   nplanes = 47
   openr,lu,str,/get_lun
   roi = fltarr(nrois,nplanes)
   
   FOR f=0,nrois-1 DO BEGIN
      FOR i=1,2 DO readf, lu, str
      FOR p=0,nplanes-1 DO BEGIN
         readf,lu,plane,val
         roi(f,p) = val
      ENDFOR
      IF (f EQ 0) THEN readf, lu, str
   ENDFOR
   close,lu

   planes = indgen(47)
   f = 1
   nroi = roi(f,*)/(moment(roi(f,*)))(0)
   name = "961 2D Uniformity from Scatter Phantom (970408)"
   plot,planes,nroi,xrange=[0,50],yrange=[0.9,1.1],psym=-1,$
      xtitle='Plane', ytitle='Normalized Mean Counts',charsize=1.5, $
      charthick=2,xthick=2,ythick=2, title=name
   oplot,[0,50],[.98,.98],linestyle=2
   oplot,[0,50],[1.02,1.02],linestyle=2
   read,prompt='Save? ',str
   IF ((strpos(str,'Y') GE 0) OR (strpos(str,'y') GE 0)) THEN BEGIN
      read,prompt='Filename? ',str
      set_plot,'ps'
      device, filename=str,xsize=7,ysize=6,xoffset=0.8,yoffset=3,/inches
      plot,planes,nroi,xrange=[0,50],yrange=[0.9,1.1],psym=-1,$
         xtitle='Plane', ytitle='Normalized Mean Counts',charsize=1.5, $
         charthick=2,xthick=2,ythick=2, title=name
      oplot,[0,50],[.98,.98],linestyle=2
      oplot,[0,50],[1.02,1.02],linestyle=2
      device,/close
   ENDIF
   set_plot,'X'
END


   
   
