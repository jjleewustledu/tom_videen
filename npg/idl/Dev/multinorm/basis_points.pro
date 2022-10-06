;+
; NAME:
;	BASIS_POINTS
;
; PURPOSE:
;	Determines basis points for fit in a 3D image.
;	Locates up to 25 points per plane: 16 on perimeter, 9 internal.
;	A point is included if 25 of 25 neighboring points are non-zero (tissue)
;		and the point is >= 25 voxels from all other points in the same plane.
;
; MODIFICATION HISTORY:
;	Sep 2002 - Tom Videen
;-

FUNCTION BASIS_POINTS, data, nx, ny, nz

   nb = 25L * nz
   basis = lonarr(nb,3)
   n = 0L
   rad = 1
   thresh = (2*rad+1)*(2*rad+1)*3
   separation = 20

;  For every 6th plane

   FOR k = 1, nz-2, 10 DO BEGIN
      IF max(data[*,*,k]) GT 0 THEN BEGIN
         n0 = n

;	Centroid
         idx = where(data[*,*,k] GT 0, count)
         IF (count ge 2) THEN BEGIN
            sum = total(data[*,*,k])
            row = fltarr(nx)
            FOR i=0,nx-1 DO row[i]=total(data[i,*,k]*float(i))
            col = fltarr(ny)
            FOR j=0,ny-1 DO col[j]=total(data[*,j,k]*float(j))
            x0 = total(row)/sum
            y0 = total(col)/sum
         ENDIF

;	Horizontal 4 points
         j = fix(y0)
         idx = where(data[*,j,k] GT 0, count)
         IF (count ge 2) THEN BEGIN
            x1 = min(idx)
            x4 = max(idx)

            next = 1
            i = x1 > rad
            WHILE (next AND i LT x4) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN i = i+1 $
               ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  x1 = i
                  y1 = j
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE

            next = 1
            i = x4 < (nx-rad-1)
            WHILE (next AND i GT x1) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN i = i-1 $
               ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  x4 = i
                  y4 = j
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE

            x2 = x1 + 0.25*(x4-x1)
            x3 = x1 + 0.75*(x4-x1)

            next = 1
            i = x2 > rad
            WHILE (next AND i LT x3) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN i = i+1 $
               ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE

            next = 1
            i = x3 > rad
            WHILE (next AND i LT x4) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN i = i+1 $
               ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE
         ENDIF                  ; end Horizontal

;	Vertical 5 points
         i = fix(x0)
         idx = where(data[i,*,k] GT 0, count)
         IF (count ge 2) THEN BEGIN
            y1 = min(idx)
            y5 = max(idx)

            next = 1
            j = y1
            WHILE (next AND j LT y5) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN j = j+1 $
               ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  x1 = i
                  y1 = j
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE

            next = 1
            j = y5
            WHILE (next AND j GT y1) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN j = j-1 $
               ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  x5 = i
                  y5 = j
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE

            y2 = y1+0.25*(y5-y1)
            y3 = y1+0.5*(y5-y1)
            y4 = y1+0.75*(y5-y1)

            next = 1
            j = y2
            WHILE (next AND j LT y3) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN j = j+1 $
               ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE

            next = 1
            j = y3
            WHILE (next AND j LT y4) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN j = j+1 $
               ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE

            next = 1
            j = y4
            WHILE (next AND j LT y5) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN j = j+1 $
               ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE
         ENDIF                  ; end Vertical

;	First Diagonal 4 points (down-right)
         i = fix(x0)
         j = fix(y0)
         x1 = -1
         x4 = -1
         FOR m=0, x0 DO BEGIN
            IF (j-m GE 0) THEN $
               IF (data[i-m,j-m,k] GT 0) THEN x1 = i-m & y1 = j-m
         ENDFOR
         FOR m=0, (nx-x0) DO BEGIN
            IF (j+m LT ny) THEN $
               IF (data[i+m,j+m,k] GT 0) THEN x4 = i+m & y4 = j+m
         ENDFOR

         IF (x1 GE 0 and x1 NE x4) THEN BEGIN
            next = 1
            i = x1 > rad
            j = y1 > rad
            WHILE (next AND i LT x4) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN BEGIN
                  i = i+1
                  j = j+1
               ENDIF ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  x1 = i
                  y1 = j
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE

            next = 1
            i = x4 < (nx-rad-1)
            j = y4 < (ny-rad-1)
            WHILE (next AND i GT x1) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN BEGIN
                  i = i-1
                  j = j-1
               ENDIF ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  x4 = i
                  y4 = j
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE

            x2 = x1 + 0.25*(x4-x1)
            x3 = x1 + 0.75*(x4-x1)
            y2 = y1 + 0.25*(y4-y1)
            y3 = y1 + 0.75*(y4-y1)

            next = 1
            i = x2 > rad
            j = y2 > rad
            WHILE (next AND i LT x3) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN BEGIN
                  i = i+1
                  j = j+1
               ENDIF ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE

            next = 1
            i = x3 > rad
            j = y3 > rad
            WHILE (next AND i LT x4) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN BEGIN
                  i = i+1
                  j = j+1
               ENDIF ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE
         ENDIF                  ; end First Diagonal

;	Second Diagonal 4 points (up-right)
         i = fix(x0)
         j = fix(y0)
         x1 = -1
         x4 = -1
         FOR m=0, x0 DO BEGIN
            IF (j+m LT ny) THEN $
               IF (data[i-m,j+m,k] GT 0) THEN x1 = i-m & y1 = j+m
         ENDFOR
         FOR m=0, (nx-x0) DO BEGIN
            IF (j+m GE 0) THEN $
               IF (data[i+m,j-m,k] GT 0) THEN x4 = i+m & y4 = j-m
         ENDFOR

         IF (x1 GE 0 and x1 NE x4) THEN BEGIN
            next = 1
            i = x1 > rad
            j = y1 < (ny-rad-1)
            WHILE (next AND i LT x4) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN BEGIN
                  i = i+1
                  j = j-1
               ENDIF ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  x1 = i
                  y1 = j
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE

            next = 1
            i = x4 < (nx-rad-1)
            j = y4 > rad
            WHILE (next AND i GT x1) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN BEGIN
                  i = i-1
                  j = j+1
               ENDIF ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  x4 = i
                  y4 = j
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE

            x2 = x1 + 0.25*(x4-x1)
            x3 = x1 + 0.75*(x4-x1)
            y2 = y1 - 0.25*(y1-y4)
            y3 = y1 - 0.75*(y1-y4)

            next = 1
            i = x2 > rad
            j = y2 < (ny-rad-1)
            WHILE (next AND i LT x3) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN BEGIN
                  i = i+1
                  j = j-1
               ENDIF ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE

            next = 1
            i = x3 > rad
            j = y3 < (ny-rad-1)
            WHILE (next AND i LT x4) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN BEGIN
                  i = i+1
                  j = j-1
               ENDIF ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE
         ENDIF                  ; end Second Diagonal

;	First Steep Diagonal 2 points (down-right; near vertical)
         i = fix(x0)
         j = fix(y0)
         x1 = -1
         x2 = -1
         FOR m=0, x0 DO BEGIN
            IF (j-2*m GE 0) THEN $
               IF (data[i-m,j-2*m,k] GT 0) THEN x1 = i-m & y1 = j-2*m
         ENDFOR
         FOR m=0, (nx-x0) DO BEGIN
            IF (j+2*m LT ny) THEN $
               IF (data[i+m,j+2*m,k] GT 0) THEN x2 = i+m & y2 = j+2*m
         ENDFOR

         IF (x1 GE 0 and x1 NE x2) THEN BEGIN
            next = 1
            i = x1 > rad
            j = y1 > rad
            WHILE (next AND i LT x2) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN BEGIN
                  i = i+1
                  j = j+2
               ENDIF ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  x1 = i
                  y1 = j
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE

            next = 1
            i = x2 < (nx-rad-1)
            j = y2 < (ny-rad-1)
            WHILE (next AND i GT x1) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN BEGIN
                  i = i-1
                  j = j-2
               ENDIF ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE
         ENDIF                  ; end First Steep Diagonal

;	Second Steep Diagonal 2 points (up-right; near vertical)
         i = fix(x0)
         j = fix(y0)
         x1 = -1
         x2 = -1
         FOR m=0, x0 DO BEGIN
            IF (j+2*m LT ny) THEN $
               IF (data[i-m,j+2*m,k] GT 0) THEN x1 = i-m & y1 = j+2*m
         ENDFOR
         FOR m=0, (nx-x0) DO BEGIN
            IF (j-2*m GE 0) THEN $
               IF (data[i+m,j-2*m,k] GT 0) THEN x2 = i+m & y2 = j-2*m
         ENDFOR

         IF (x1 GE 0 and x1 NE x2) THEN BEGIN
            next = 1
            i = x1 > rad
            j = y1 < ny-rad-1
            WHILE (next AND i LT x2) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN BEGIN
                  i = i+1
                  j = j-2
               ENDIF ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  x1 = i
                  y1 = j
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE

            next = 1
            i = x2 < (nx-rad-1)
            j = y2 > rad
            WHILE (next AND i GT x1) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN BEGIN
                  i = i-1
                  j = j+2
               ENDIF ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE
         ENDIF                  ; end First Steep Diagonal

;	First Flat Diagonal 2 points (down-right; near horizontal)
         i = fix(x0)
         j = fix(y0)
         x1 = -1
         x2 = -1
         FOR m=0, x0/2 DO BEGIN
            IF (j-m GE 0) THEN $
               IF (data[i-2*m,j-m,k] GT 0) THEN x1 = i-2*m & y1 = j-m
         ENDFOR
         FOR m=0, (nx-x0)/2 DO BEGIN
            IF (j+m LT ny) THEN $
               IF (data[i+2*m,j+m,k] GT 0) THEN x2 = i+2*m & y2 = j+m
         ENDFOR

         IF (x1 GE 0 and x1 NE x2) THEN BEGIN
            next = 1
            i = x1 > rad
            j = y1 < (ny-rad-1)
            WHILE (next AND i LT x2) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN BEGIN
                  i = i+2
                  j = j+1
               ENDIF ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  x1 = i
                  y1 = j
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE

            next = 1
            i = x2 < (nx-rad-1)
            j = y2 > rad
            WHILE (next AND i GT x1) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN BEGIN
                  i = i-2
                  j = j+1
               ENDIF ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE
         ENDIF                  ; end Second Flat Diagonal

;	Second Flat Diagonal 2 points (up-right; near horizontal)
         i = fix(x0)
         j = fix(y0)
         x1 = -1
         x2 = -1
         FOR m=0, x0/2 DO BEGIN
            IF (j+m LT ny) THEN $
               IF (data[i-2*m,j+m,k] GT 0) THEN x1 = i-2*m & y1 = j+m
         ENDFOR
         FOR m=0, (nx-x0)/2 DO BEGIN
            IF (j-m GE 0) THEN $
               IF (data[i+2*m,j-m,k] GT 0) THEN x2 = i+2*m & y2 = j-m
         ENDFOR

         IF (x1 GE 0 and x1 NE x2) THEN BEGIN
            next = 1
            i = x1 > rad
            j = y1 < (ny-rad-1)
            WHILE (next AND i LT x2) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN BEGIN
                  i = i+2
                  j = j-1
               ENDIF ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  x1 = i
                  y1 = j
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE

            next = 1
            i = x2 < (nx-rad-1)
            j = y2 > rad
            WHILE (next AND i GT x1) DO BEGIN
               idx = where(data[i-rad:i+rad,j-rad:j+rad,k-1:k+1] GT 0, count)
               IF count lt thresh THEN BEGIN
                  i = i-2
                  j = j+1
               ENDIF ELSE BEGIN
                  basis[n,*] = [i,j,k]
                  next = 0
                  IF (distance_check(basis, n0, n-1, n, separation) GT 0) THEN n = n+1
               ENDELSE
            ENDWHILE
         ENDIF                  ; end Second Flat Diagonal
      ENDIF
   ENDFOR
   newbasis = basis[0:n-1,*]
   RETURN, newbasis
END
