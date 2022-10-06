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

   k = 0
   zmin = -1
   WHILE (k LE nz-1 AND zmin LT 0) DO BEGIN
      IF max(data[*,*,k]) GT 0 THEN zmin=k
      k = k+1
   ENDWHILE
   IF (zmin LT 0) THEN BEGIN
      zmin=0
      zmax=nz-1
   ENDIF ELSE BEGIN
      k = nz-1
      zmax = -1
      WHILE (k GE 0 AND zmax LT 0) DO BEGIN
         IF max(data[*,*,k]) GT 0 THEN zmax=k
         k = k-1
      ENDWHILE
   ENDELSE
   print,"z: ", zmin, zmax

   inc = 15L;
   nbx = nx/inc + 2
   nby = (ymax-ymin)/inc + 2
   nbz = (zmax-zmin)/inc + 2
   xtop= xmax+inc-1
   ytop= ymax+inc-1
   ztop= zmax+inc-1
   nb = nbx * nby * nbz
   print,format='("Num points: ",i4,i4,i4,"  Total =",i5)',nbx,nby,nbz,nb
   basis = lonarr(nb,3)

;  Sample edges thoroughly, then fill in center

   n = 0L
   FOR k = zmin,ztop,inc DO BEGIN
      km = k < (nz-1)
      FOR j = ymin,ytop,inc DO BEGIN
         jm = j < (ny-1)
         idx = where(data[*,jm,km] GT 0, count)
         IF (count ge 2) THEN BEGIN
            xlo = min(idx)
            xhi = max(idx)
            FOR i = xlo,xhi,inc DO BEGIN
               im = i
               IF data[im,jm,km] EQ 0 THEN next=0 ELSE next=1
               WHILE (im LT xhi AND next) DO BEGIN
                  im = im + 1
                  IF data[im,jm,km] GT 0 THEN next=0
               ENDWHILE
               IF (next EQ 0) THEN BEGIN
                  basis[n,*] = [im,jm,km]
                  n = n+1
               ENDIF
            ENDFOR
            IF (im NE xhi) THEN BEGIN
               basis[n,*] = [xhi,jm,km]
               n = n+1
            ENDIF
         ENDIF
      ENDFOR
   ENDFOR
   newbasis = basis[0:n-1,*]
   print,"Basis Points ",n
   RETURN, newbasis
END
