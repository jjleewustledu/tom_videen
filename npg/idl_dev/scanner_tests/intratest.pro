; Intraplane Uniformity Test
; Uses output file from roi tool

PRO intratest
   str = 'unifwob_f2to7_3cm.cpt'
   nplanes = 31
   nrois = 5
   openr,lu,str,/get_lun
   roi=fltarr(nrois,nplanes)
   
   FOR r=0,nrois-1 DO BEGIN
     FOR i=1,2 DO readf, lu, str
     FOR p=0,nplanes-1 DO BEGIN
       readf,lu,plane,val
       roi(r,p) = val
     ENDFOR
     if (r lt nrois-1) then readf, lu, str
   ENDFOR
   close,lu
   
   str = ''
   read, prompt='Output filename? ', str
   openw,lu,str
   name=strarr(5)
   name(0)='center'
   name(1)='top   '
   name(2)='bottom'
   name(3)='left  '
   name(4)='right '
   printf,lu,'Pl    Max         Min      %diff   Max     Min'
   FOR p=0,nplanes-1 DO BEGIN
     a=max(roi(*,p))
     b=min(roi(*,p))
     ai = where((roi(*,p) eq a), count)
     bi = where((roi(*,p) eq b), count)
     dif = 100*((a-b)/b)
     err = ''
     IF (dif GT 8) THEN err = '*'
     printf,lu,format='(i2,f12.8,f12.8,f6.2,3x,a6,2x,a6,2x,a1)',$
        p+1, a, b, 100*((a-b)/b), name(ai),name(bi),err
  ENDFOR
  free_lun,lu
END

