; Interplane Uniformity Test
; Uses output file from Siemens Countrate Test (crtest.lis)
; If this file is corrupted (e.g. NaN's) then correct with editor

PRO uniftest0
   str = 'unifwob_f2to7_15cm.cpt'
   nframes = 1
   nplanes = 31
   openr,lu,str,/get_lun
   roi = fltarr(nplanes,nframes)
   
   FOR f=0,nframes-1 DO BEGIN
     FOR i=1,2 DO readf, lu, str
       FOR p=0,nplanes-1 DO BEGIN
         readf,lu,plane,val
         roi(p,f) = val
      ENDFOR
   ENDFOR
   close,lu
   free_lun,lu
   planes = indgen(31)
   f = 0
      print,'Frame=',f+1,'  Activity < 2.63 uCi/cc'
      nroi = roi(*,f)/(moment(roi(*,f)))(0)
      name = "953B Uniformity: Frames 2-7 <2.63 uCi/cc"
      plot,planes,nroi,xrange=[0,30],yrange=[0.9,1.1],psym=-1,$
         xtitle='Plane', ytitle='Normalized Mean Counts',charsize=1.5, $
         charthick=2,xthick=2,ythick=2, title=name
      oplot,[0,30],[.98,.98],linestyle=2
      oplot,[0,30],[1.02,1.02],linestyle=2
   read,prompt='Postscript File? ',str
   IF ((strpos(str,'Y') GE 0) OR (strpos(str,'y') GE 0)) THEN BEGIN
      read,prompt='Filename? ',str
      set_plot,'ps'
      nroi = roi(*,f)/(moment(roi(*,f)))(0)
      name = "953B Uniformity: Frames 2-7 <2.63 uCi/cc"
      device, filename=str,xsize=7,ysize=6,xoffset=0.8,yoffset=3,/inches
      plot,planes,nroi,xrange=[0,30],yrange=[0.9,1.1],psym=-1,$
         xtitle='Plane', ytitle='Normalized Mean Counts',charsize=1.5, $
         charthick=2,xthick=2,ythick=2, title=name
      oplot,[0,30],[.98,.98],linestyle=2
      oplot,[0,30],[1.02,1.02],linestyle=2
      device,/close
   ENDIF
   set_plot,'X'
END


   
   
