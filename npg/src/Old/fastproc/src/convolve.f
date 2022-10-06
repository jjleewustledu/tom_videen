c $Log: convolve.f,v $
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
C  Subroutine:   CONVOLVE.FTN
C
C  Author:       Tom O. Videen
C  Date:         14-May-87
C  Written For:  METPROC
C
C  Intent:
C     Compute convolution integral, CONV(T), for T = 1 to INUM of:
C
C          IACTIV(I) * EXP(-K*(ITIME(T)-ITIME(I)) * dt
C
C     for I = 1 to T
C
C
C
C
C
C     NOTE:  ITIME is in units of seconds
C            IACTIV is in units of well counts/(ml*sec)
C
C     K = (Flow/LAMBDA)+Decay Constant must be passed in UNITS OF 1/SEC
C
C  The integration is computed using Simpson's rule.
C     Each time interval is divided in half. The midpoint value
C     for IACTIV is obtained from linear interpolation, while the
C     value EXP(-K*TDIFF) is computed.
C
C     This computation is accurate as long as K*TDIFF is small.
C
C  Variable passed:
C     K = rate constant ((flow/lambda) + decay const) UNITS OF 1/SEC)
C
C  Variables:
C     AMEAN = mean activity for the interval (IACTIV(I) in eqn above);
C     TMEAN = mean time for the interval;
C     TDIFF = ITIME(T) - ITIME(I)    (T-t  see equation above);
C     TDELTA = ITIME(I)-ITIME(I-1)   (dt in equation above);
C     AMEAN1,TMEAN1,TDELTA1 = values for first time interval;
C
C  Common block variables:
C     INUM = no. of data points
C     ITIME = interpolated time
C     IACTIV = interpolated activity
C     CONV(I) = integral from 0 to ITIME(I)
C
C  Called by:  BLOODFLW, INTCONV
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE CONVOLVE(K)
C
      INCLUDE 'metproc.inc'
C
      INTEGER*4 I,J
      REAL      K,AMEAN,TMEAN,TDELTA,AMEAN1,TMEAN1,TDELTA1,
     &          TDIFF0,TDIFF1,TDIFF2,Y0,Y1,Y2
			CHARACTER*256 RCSHEADER
C
			RCSHEADER = "$Header: /home/petsun2/tom/src/fastproc/src/RCS/convolve.f,v 1.2 1992/11/02 16:02:53 tom Exp $"
C
      AMEAN1 = 0.5*IACTIV(1)
      TMEAN1 = 0.5*ITIME(1)
      TDELTA1 = TMEAN1
      TDIFF1 = TMEAN1
      Y1 = AMEAN1*EXP(-K*TDIFF1)
      Y2 = IACTIV(1)
      CONV(1) = (TDELTA1/3.)*(4*Y1 + Y2)
C
      DO 200 I = 2,INUM
        TDIFF1 = ITIME(I)-TMEAN1
        TDIFF2 = ITIME(I)-ITIME(1)
        Y1 = AMEAN1*EXP(-K*TDIFF1)
        Y2 = IACTIV(1)*EXP(-K*TDIFF2)
        CONV(I) = (TDELTA1/3.)*(4*Y1+Y2)
        DO 100 J=2,I
          AMEAN = 0.5*(IACTIV(J)+IACTIV(J-1))
          TMEAN = 0.5*(ITIME(J)+ITIME(J-1))
          TDELTA = 0.5*(ITIME(J)-ITIME(J-1))
          TDIFF0 = ITIME(I)-ITIME(J-1)
          TDIFF1 = ITIME(I)-TMEAN
          TDIFF2 = ITIME(I)-ITIME(J)
          Y0 = IACTIV(J-1)*EXP(-K*TDIFF0)
          Y1 = AMEAN*EXP(-K*TDIFF1)
          Y2 = IACTIV(J)*EXP(-K*TDIFF2)
          CONV(I) = CONV(I) + (TDELTA/3.)*(Y0 + 4.*Y1 + Y2)
100     CONTINUE
200   CONTINUE
C
      RETURN
      END
