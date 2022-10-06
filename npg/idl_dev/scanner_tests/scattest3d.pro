; Scatter ROI Test
; Uses output file from Siemens ROI program (.cpt)

PRO scattest3d
   str = ''
;   read, prompt='Scatter ROI filename? ', str
;   read, prompt='Number of planes? ', nplanes
;   read, prompt='Cold ROI ID? ', roi1
;   read, prompt='Background ROI ID? ',roi2

   str = 'scat3dnew.dat'
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
   FOR p=0,nplanes-1 DO BEGIN
      readf,lu,plane,val
      back(plane-1) = val
   ENDFOR
   close,lu
   openw,lu,'scat3dnew.calc'
   printf,lu,'Plane    Cold        Hot        %Error  Excessive'
   FOR p=0,nplanes-1 DO BEGIN
      pct(p) = 100*cold(p)/back(p)
      str = ''
      IF (abs(pct(p)) GT 5.0) THEN str = '*'
      IF (abs(pct(p)) GT 10.0) THEN str = '**'
      IF (abs(pct(p)) GT 15.0) THEN str = '***'
      printf,lu,p+1,cold(p),back(p),pct(p),str,format='(I4,F14.8,F12.8,F8.2,3X,A)'
   ENDFOR
   printf,lu,'---------------------------------------'
   meancold = (moment(cold))(0)
   meanback = (moment(back))(0)
   diff = 100*meancold/meanback
   str = ''
   IF (abs(diff) GT 2.0) THEN str = '*'
   IF (abs(diff) GT 4.0) THEN str = '**'
   IF (abs(diff) GT 6.0) THEN str = '***'
   IF (abs(diff) GT 8.0) THEN str = '****
   printf,lu,meancold,meanback,diff,str,format='("Mean",F14.8,F12.8,F8.2,3X,A)'
   free_lun,lu
END


   
   
