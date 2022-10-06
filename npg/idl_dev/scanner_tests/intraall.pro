; Intraplane Uniformity Test
; Uses output file from roi tool

PRO intraall
LABEL:
   str = 'unifwob_f2to7_3cm.cpt'
   nframes = 5
   nplanes = 47
   nrois = 5
   openr,lu,str,/get_lun
   roi=fltarr(nframes,nrois,nplanes)
   
   FOR f=0,nframes-1 DO BEGIN
      FOR r=0,nrois-1 DO BEGIN
         FOR i=1,2 DO readf, lu, str
         FOR p=0,nplanes-1 DO BEGIN
            readf,lu,plane,val
            roi(f,r,p) = val
         ENDFOR
         if (r lt nrois-1) then readf, lu, str
      ENDFOR
   ENDFOR
   close,lu
   
   activ = strarr(6)
   activ(0) = '1.04 uCi/cc'
   activ(1) = '0.49 uCi/cc'
   activ(2) = '0.23 uCi/cc'
   activ(3) = '0.11 uCi/cc'
   activ(4) = '0.05 uCi/cc'
   activ(5) = '0.02 uCi/cc'

   openw,lu,'unif3dintra.dat'
   name=strarr(5)
   name(0)='center'
   name(1)='top   '
   name(2)='bottom'
   name(3)='left  '
   name(4)='right '
   printf,lu,'Pl    Max         Min      %diff   Max     Min'
   FOR f=0,nframes-1 DO BEGIN
      printf,lu,'Frame ',f+1,'   ',activ(f)
      FOR p=0,nplanes-1 DO BEGIN
         a=max(roi(f,*,p))
         b=min(roi(f,*,p))
         ai = where((roi(f,*,p) eq a), count)
         bi = where((roi(f,*,p) eq b), count)
         dif = 100*((a-b)/b)
         err = ''
         IF (dif GT 5) THEN err = '*  '
         IF (dif GT 10) THEN err = '** '
         IF (dif GT 15) THEN err = '***'
         printf,lu,format='(i2,f12.8,f12.8,f6.2,3x,a6,2x,a6,2x,a3)',$
            p+1, a, b, dif, name(ai),name(bi), err
      ENDFOR
   ENDFOR
   free_lun,lu
END


   
   
