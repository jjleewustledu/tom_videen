c $Log: integrat.f,v $
c Revision 1.2  1992/11/02  16:02:53  tom
c Added RCS identifies
c
c Revision 1.3  1992/10/19  15:56:10  tom
c modified RCSHEADER length
c
c Revision 1.2  1992/10/16  22:32:51  tom
c added RCSHEADER
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   INTEGRAT.FTN
C
C  Author:       Tom O. Videen
C  Date:         14-May-87
C  Written For:  METPROC
C
C  Intent:
C     Calculate the integral from XFIRST to XLAST over the
C     array of points (X,Y) using the Trapezoidal Rule.
C
C  Assume that function begins at 0,0.
C  Assume that at least one X exists after XLAST.
C
C  Called by:  METPROC, BLOODFLW, BLOODVOL, INTCONV, O2UTIL
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE INTEGRAT(X,Y,XFIRST,XLAST,INTEGRAL)
C
      INTEGER*4 I,J
      REAL      X(400),Y(400),XFIRST,XLAST,INTEGRAL,
     &          XDIFF,YFIRST,YLAST,YMEAN
			CHARACTER*256 RCSHEADER
C
			RCSHEADER = "$Header: /home/petsun2/tom/src/fastproc/src/RCS/integrat.f,v 1.2 1992/11/02 16:02:53 tom Exp $"
C
      INTEGRAL = 0.
C
C  First Point    (executed exactly once)
C
      IF (XFIRST.LT.X(1)) THEN
        YFIRST = Y(1)*XFIRST/X(1)
        IF (XLAST.LT.X(1)) THEN
          YLAST = Y(1)*XLAST/X(1)
          YMEAN = 0.5*(YFIRST+YLAST)
          XDIFF = XLAST-XFIRST
          INTEGRAL = XDIFF*YMEAN
          RETURN
        ELSE
          YMEAN = 0.5*(YFIRST+Y(1))
          XDIFF = X(1)-XFIRST
          INTEGRAL = XDIFF*YMEAN
          I = 1
          J = 2
          GO TO 200
        END IF
      END IF
      I = 1
      J = 2
C
C  Case: X(J) < XFIRST
C
100   IF (X(J).LT.XFIRST) THEN
        I = I+1
        J = J+1
        GO TO 100
      END IF
C
C  Case: X(I) < XFIRST <= X(J)  (executed exactly once)
C
      YFIRST = Y(I) + (Y(J)-Y(I))*(XFIRST-X(I))/(X(J)-X(I))
      YMEAN = 0.5*(YFIRST+Y(J))
      XDIFF = X(J)-XFIRST
      INTEGRAL = INTEGRAL + XDIFF*YMEAN
      I = I+1
      J = J+1
C
C  Case: XFIRST <= X(I) .and.  X(J) < XLAST
C
200   IF (X(J).LT.XLAST) THEN
        XDIFF = X(J)-X(I)
        YMEAN = 0.5*(Y(J)+Y(I))
        INTEGRAL = INTEGRAL + XDIFF*YMEAN
        I = I+1
        J = J+1
        GO TO 200
      END IF
C
C  Case: XLAST <= X(J)
C
      YLAST = Y(I) + (Y(J)-Y(I))*(XLAST-X(I))/(X(J)-X(I))
      YMEAN = 0.5*(Y(I)+YLAST)
      XDIFF = XLAST-X(I)
      INTEGRAL = INTEGRAL + XDIFF*YMEAN
C
      RETURN
      END
