;+
; NAME:
;	BASIS_POINTS
;
; PURPOSE:
;	Deterines basis points for fit in a 3D image
;
; MODIFICATION HISTORY:
;	Sep 2002 - Tom Videen
;-

FUNCTION BASIS_POINTS, data, nx, ny, nz

;  Determine basis points for fit
;  and compute local averages at these points
help,data

   i = 0
   xmin = -1
   WHILE (i LE nx-1 AND xmin LT 0) DO BEGIN
      IF max(data[i,*,*]) GT 0 THEN xmin=i
      i = i+1
   ENDWHILE
   IF (xmin LT 0) THEN BEGIN
      xmin=0
      xmax=nx-1
   ENDIF ELSE BEGIN
      i = nx-1
      xmax = -1
      WHILE (i GE 0 AND xmax LT 0) DO BEGIN
         IF max(data[i,*,*]) GT 0 THEN xmax=i
         i = i-1
      ENDWHILE
   ENDELSE
   print,"x: ", xmin, xmax

   j = 0
   ymin = -1
   WHILE (j LE ny-1 AND ymin LT 0) DO BEGIN
      IF max(data[*,j,*]) GT 0 THEN ymin=j
      j = j+1
   ENDWHILE
   IF (ymin LT 0) THEN BEGIN
      ymin=0
      ymax=ny-1
   ENDIF ELSE BEGIN
      j = ny-1
      ymax = -1
      WHILE (j GE 0 AND ymax LT 0) DO BEGIN
         IF max(data[*,j,*]) GT 0 THEN ymax=j
         j = j-1
      ENDWHILE
   ENDELSE
   print,"y: ", ymin, ymax

   xinc = 20
   yinc = 20
   zinc = 1
   xtop = xmax+xinc-1 < nx-1
   ytop = ymax+yinc-1 < ny-1
   nbx = (xtop-xmin)/xinc + 1
   nby = (ytop-ymin)/yinc + 1
   nbz = nz/zinc + 1
   nb = nbx * nby * nbz
   print,"inc: ",xinc,yinc,zinc
   print,format='("Num points: ",i4,i4,i4,"  Total =",i5)',nbx,nby,nbz,nb
   basis = lonarr(nb,3)

   n = 0
   FOR k = 0,nz-1+zinc,zinc DO BEGIN
      km = k < (nz-1)
      FOR j = ymin,ytop,yinc DO BEGIN
         jm = j < ymax
         FOR i = xmin,xtop,xinc DO BEGIN
            im = i < xmax
            IF data[im,jm,km] EQ 0 THEN next=1 ELSE next=0
            WHILE (im LT xtop AND next) DO BEGIN
                im = im + 1
                IF data[im,jm,km] GT 0 THEN next=0
            ENDWHILE
            IF (next EQ 0) THEN BEGIN
                basis[n,*] = [im,jm,km]
                n = n+1
            ENDIF
         ENDFOR
      ENDFOR
   ENDFOR
   newbasis = basis[0:n-1,*]
   print,"Basis Points ",n
   RETURN, newbasis
END
