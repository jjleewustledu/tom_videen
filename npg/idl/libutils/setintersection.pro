FUNCTION SetIntersection, a, b
   minab = min(a, MAX=maxa) > min(b, MAX=maxb) ;Only need intersection of ranges
   maxab = maxa < maxb

   ;; If either set is empty, or their ranges don't intersect: result = NULL.
   IF maxab LT minab OR maxab LT 0 THEN return, -1
   r = where((histogram(a, MIN=minab, MAX=maxab) NE 0) AND  $
             (histogram(b, MIN=minab, MAX=maxab) NE 0), count)
   IF count EQ 0 THEN return, -1 ELSE return, r + minab
END 

