; Interplane Uniformity Test
; Uses output file from Siemens Countrate Test (crtest.lis)
; If this file is corrupted (e.g. NaN's) then correct with editor

PRO uniftest
   str = 'unifwob.dat'
   nframes = 6
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
   activ = fltarr(nframes)
   activ(0) = 5.62
   activ(1) = 2.63
   activ(2) = 1.23
   activ(3) = 0.58
   activ(4) = 0.27
   activ(5) = 0.13
   f = 0
   WHILE (f GE 0 AND f LE nframes) DO BEGIN
      print,'Frame=',f+1,'  Activity=',activ(f),' uCi/cc'
      nroi = roi(*,f)/(moment(roi(*,f)))(0)
      name = "953B Uniformity: "+ string(activ(f)) + ' uCi/cc'
      plot,planes,nroi,xrange=[0,30],yrange=[0.9,1.1],psym=-1,$
         xtitle='Plane', ytitle='Normalized Mean Counts',charsize=1.5, $
         charthick=2,xthick=2,ythick=2, title=name
      oplot,[0,30],[.98,.98],linestyle=2
      oplot,[0,30],[1.02,1.02],linestyle=2
      read,prompt='Frame number? ',f
      f = f-1
   ENDWHILE
LABEL:
   read,prompt='Postscript File? ',str
   IF ((strpos(str,'Y') GE 0) OR (strpos(str,'y') GE 0)) THEN BEGIN
      read,prompt='Filename? ',str
      read,prompt='Frame number? ',f
      set_plot,'ps'
      nroi = roi(*,f)/(moment(roi(*,f)))(0)
      name = "953B Uniformity: "+ string(activ(f)) + ' uCi/cc'
      device, filename=str,xsize=7,ysize=6,xoffset=0.8,yoffset=3,/inches
      plot,planes,nroi,xrange=[0,30],yrange=[0.9,1.1],psym=-1,$
         xtitle='Plane', ytitle='Normalized Mean Counts',charsize=1.5, $
         charthick=2,xthick=2,ythick=2, title=name
      oplot,[0,30],[.98,.98],linestyle=2
      oplot,[0,30],[1.02,1.02],linestyle=2
      device,/close
	  goto, label
   ENDIF
   set_plot,'X'
END


   
   
