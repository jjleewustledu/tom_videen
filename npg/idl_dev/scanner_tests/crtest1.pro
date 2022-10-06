; Interplane Uniformity Test
; Uses output file from Siemens Countrate Test (crtest.lis)
; If this file is corrupted (e.g. NaN's) then correct with editor

PRO crtest1
   str = ''
   read, prompt='Countrate list filename? ', str
   read, prompt='Number of frames? ', nframes
   read, prompt='Number of planes? ', nplanes
;   str = 'crtest.fix'
;   nframes = 48
;   nplanes = 31
   openr,lu,str,/get_lun
   activ = fltarr(nplanes,nframes)
   roi = fltarr(nplanes,nframes)
   
   FOR p=0,nplanes-1 DO BEGIN
      str = ''
      WHILE (strpos(str,'Plane') LE 0) DO readf,lu,str
      FOR i=1,5 DO readf, lu, str
      FOR f=0,nframes-1 DO BEGIN
         readf,lu,str
         reads,strmid(str,40,70),act,x,y,val
         activ(p,f) = act
         roi(p,f) = val
      ENDFOR
   ENDFOR
   close,lu
   free_lun,lu
;  FOR f=0,nframes-1 DO $
;    print,f+1,activ(0,f),(moment(roi(*,f)))(0)
   planes = indgen(31)
   f = 0
   WHILE (f GE 0 AND f LT nframes) DO BEGIN
      print,'Frame=',f,'  Activity=',activ(0,f),' uCi/cc'
      nroi = roi(*,f)/(moment(roi(*,f)))(0)
      name = "953B Uniformity: "+ string(activ(0,f)) + ' uCi/cc'
      plot,planes,nroi,xrange=[0,30],yrange=[0.9,1.1],psym=-1,$
         xtitle='Plane', ytitle='Normalized Mean Counts',charsize=1.5, $
         charthick=2,xthick=2,ythick=2, title=name
      oplot,[0,30],[.98,.98],linestyle=2
      oplot,[0,30],[1.02,1.02],linestyle=2
      read,prompt='Frame number? ',f
      f = f-1
   ENDWHILE
   read,prompt='Postscript File? ',str
   IF ((strpos(str,'Y') GE 0) OR (strpos(str,'y') GE 0)) THEN BEGIN
      read,prompt='Filename? ',str
      read,prompt='Frame number? ',f
      set_plot,'ps'
      name = "953B Uniformity: "+ string(activ(0,f)) + ' uCi/cc'
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


   
   
