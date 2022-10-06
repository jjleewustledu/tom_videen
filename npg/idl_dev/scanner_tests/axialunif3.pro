PRO axialunif3
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

   ;;set_plot,'X'	
   !p.multi = [0,1,3]
   loadct,0
   aa = findgen(16) * (!pi*2/16.)
   usersym, cos(aa), sin(aa), /fill
   
   set_plot,'ps'
   device, filename="axialunif.ps",xsize=7,ysize=9,xoffset=0.8,yoffset=1,/inches

   f = 1
   nroi = roi(*,f)/(moment(roi(*,f)))(0)
   plot,planes,nroi,psym=-8,yr=[0.9,1.1],xr=[0,30],$
      charsize=1.5,xticks=3,xminor=5,charthick=2,xthick=2,ythick=2
   oplot,[0,30],[1.00,1.00],linestyle=0
   oplot,[0,30],[0.95,0.95],linestyle=2
   oplot,[0,30],[1.05,1.05],linestyle=2

   f = 2
   nroi = roi(*,f)/(moment(roi(*,f)))(0)
   plot,planes,nroi,psym=-8,yr=[0.9,1.1],xr=[0,30],$
      charsize=1.5,xticks=3,xminor=5,charthick=2,xthick=2,ythick=2
   oplot,[0,30],[1.00,1.00],linestyle=0
   oplot,[0,30],[0.95,0.95],linestyle=2
   oplot,[0,30],[1.05,1.05],linestyle=2
   
   f = 5
   nroi = roi(*,f)/(moment(roi(*,f)))(0)
   plot,planes,nroi,psym=-8,yr=[0.9,1.1],xr=[0,30],$
      charsize=1.5,xticks=3,xminor=5,charthick=2,xthick=2,ythick=2
   oplot,[0,30],[1.00,1.00],linestyle=0
   oplot,[0,30],[0.95,0.95],linestyle=2
   oplot,[0,30],[1.05,1.05],linestyle=2
   
   xyouts,0.53,1.0,"953B 2-D Axial Uniformity vs. Count Rate",align=0.5,charsize=2,/normal,font=2
   xyouts,0.53,-0.02,"Plane",align=0.5,charsize=1.5,/normal,font=2
   xyouts,0.02,0.84,'1.2-2.6 uCi/cc',align=0.5,charsize=1.3,orientation = 90,/normal,font=2
   xyouts,0.02,0.51,'0.6-1.2 uCi/cc',align=0.5,charsize=1.3,orientation = 90,/normal,font=2
   xyouts,0.02,0.18,'< 0.1 uCi/cc',align=0.5,charsize=1.3,orientation = 90,/normal,font=2
   device,/close
   free_lun,lu
   set_plot,'X'
END





