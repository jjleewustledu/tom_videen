; Scatter ROI Test
; Uses output file from Siemens ROI program (.cpt)

PRO scattest2d
   str = 'scat2d_970408_oldrec.cpt'
   nplanes = 47
   openr,lu,str,/get_lun
   cold = dblarr(nplanes)
   back = dblarr(nplanes)
   pct = dblarr(nplanes)
   
   readf,lu,str
   readf,lu,str
   FOR p=0,nplanes-1 DO BEGIN
      readf,lu,plane,val
      cold(plane-1) = val
   ENDFOR
   readf,lu,str
   readf,lu,str
   readf,lu,str
   FOR p=0,nplanes-1 DO BEGIN
      readf,lu,plane,val
      back(plane-1) = val
   ENDFOR
   close,lu
   openw,lu,'scat2d.calc'
   printf,lu,'Plane    Cold        Hot        %Error  Excessive'
   FOR p=0,nplanes-1 DO BEGIN
      pct(p) = 100*cold(p)/back(p)
      str = ''
      IF (abs(pct(p)) GT 2.0) THEN str = '*'
      IF (abs(pct(p)) GT 4.0) THEN str = '**'
      IF (abs(pct(p)) GT 6.0) THEN str = '***'
      printf,lu,p+1,cold(p),back(p),pct(p),str,format='(I4,F14.8,F12.8,F8.2,3X,A3)'
   ENDFOR
   printf,lu,'---------------------------------------'
   meancold = (moment(cold))(0)
   meanback = (moment(back))(0)
   diff = 100*meancold/meanback
   str = ''
   IF (abs(diff) GT 2.0) THEN str = '*  '
   IF (abs(diff) GT 4.0) THEN str = '** '
   IF (abs(diff) GT 6.0) THEN str = '***'
   printf,lu,meancold,meanback,diff,str,format='("Mean",F14.8,F12.8,F8.2,3X,A3)'
   free_lun,lu
END


   
   
