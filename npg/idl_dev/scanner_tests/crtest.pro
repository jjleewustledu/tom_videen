; Interplane Uniformity Test
; Uses output file from Siemens Countrate Test (crtest.lis)
; If this file is corrupted (e.g. NaN's) then correct with editor

PRO crtest
   str = 'crtest_4.lis'
   nframes = 30
   nplanes = 31
   openr,lu,str,/get_lun
   activ = fltarr(nplanes,nframes)
   roi = fltarr(nplanes,nframes)
   cor = fltarr(nplanes,nframes)
   
   FOR p=0,nplanes-1 DO BEGIN
      str = ''
      WHILE (strpos(str,'Plane') LE 0) DO readf,lu,str
      FOR i=1,5 DO readf, lu, str
      FOR f=0,nframes-1 DO BEGIN
         readf,lu,str
         reads,strmid(str,40,88),act,x,y,val,cval
         activ(p,f) = act
         roi(p,f) = val
         cor(p,f) = cval
      ENDFOR
   ENDFOR
   close,lu
   free_lun,lu
   planes = indgen(31)
   f = 5
   WHILE (f GT 0 AND f LT nframes) DO BEGIN
      print,'Frame=',f+1,'  Activity=',activ(0,f),' uCi/cc'
      nroi = cor(*,f)
      name = "953B Uniformity: "+ string(activ(0,f)) + ' uCi/cc'
      plot,planes,nroi,xrange=[0,30],psym=-1, yrange=[85,115],$
         xtitle='Plane', $
         charthick=2,xthick=2,ythick=2, title=name
      oplot,[0,30],[95,95],linestyle=2
      oplot,[0,30],[105,105],linestyle=2
      read,prompt='Frame number? ',f
   ENDWHILE
LABEL:
   read,prompt='Postscript File? ',str
   IF ((strpos(str,'Y') GE 0) OR (strpos(str,'y') GE 0)) THEN BEGIN
      read,prompt='Filename? ',str
      read,prompt='Frame number? ',f
      nroi = cor(*,f)/100
      set_plot,'ps'
      name = "953B Uniformity: "+ string(activ(0,f)) + ' uCi/cc'
      device, filename=str,xsize=7,ysize=6,xoffset=0.8,yoffset=3,/inches
      plot,planes,nroi,xrange=[0,30],yrange=[0.9,1.1],psym=-1,$
         xtitle='Plane', ytitle='Normalized Mean Counts',charsize=1.5, $
         charthick=2,xthick=2,ythick=2, title=name
      oplot,[0,30],[.98,.98],linestyle=2
      oplot,[0,30],[1.02,1.02],linestyle=2
      device,/close
      goto, LABEL
   ENDIF
   set_plot,'X'
END


   
   
