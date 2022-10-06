; Scatter ROI Test
; Uses output file from Siemens ROI program (.cpt)

PRO scattest
   str = ''
   read, prompt='Scatter ROI filename? ', str
   read, prompt='Number of planes? ', nplanes
   read, prompt='Cold ROI ID? ', roi1
   read, prompt='Background ROI ID? ',roi2

;   str = 'scatawob1_roi.cpt'
;   nplanes = 31
;   roi1 = 4
;   roi2 = 6
   
   openr,lu,str,/get_lun
   cold = dblarr(nplanes)
   back = dblarr(nplanes)
   pct = dblarr(nplanes)
   
   WHILE (strpos(str,'Plane') NE 0) DO readf,lu,str
   readf,lu,str
   FOR p=0,2*nplanes-1 DO BEGIN
      str = ''
      readf,lu,plane,bed,id,val
      IF (id EQ roi1) THEN cold(plane-1) = val $
      ELSE back(plane-1) = val
   ENDFOR
   close,lu
   free_lun,lu
   print,'Plane    Cold        Hot        %Error  Excessive'
   FOR p=0,nplanes-1 DO BEGIN
      pct(p) = 100*cold(p)/back(p)
      str = ''
      IF (abs(pct(p)) GT 2.0) THEN str = '*'
      IF (abs(pct(p)) GT 4.0) THEN str = '**'
      IF (abs(pct(p)) GT 6.0) THEN str = '***'
      IF (abs(pct(p)) GT 8.0) THEN str = '****'
      print,p+1,cold(p),back(p),pct(p),str,format='(I4,F14.8,F12.8,F8.2,3X,A)'
   ENDFOR
   print,'---------------------------------------'
   meancold = (moment(cold))(0)
   meanback = (moment(back))(0)
   diff = 100*meancold/meanback
   str = ''
   IF (abs(diff) GT 2.0) THEN str = '*'
   IF (abs(diff) GT 4.0) THEN str = '**'
   IF (abs(diff) GT 6.0) THEN str = '***'
   IF (abs(diff) GT 8.0) THEN str = '****
   print,meancold,meanback,diff,str,format='("Mean",F14.8,F12.8,F8.2,3X,A)'
END


   
   
