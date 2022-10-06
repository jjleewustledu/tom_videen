c $Log: bloodflw.f,v $
c Revision 2.1  1993/07/15  20:25:17  tom
c Changed RCSHEADER to ID
c
c Revision 2.0  1993/07/12  23:13:24  tom
c Handles ECAT image format
c
c Revision 1.3  1992/10/19  15:56:10  tom
c modified RCSHEADER length
c
c Revision 1.2  1992/10/16  22:32:51  tom
c added RCSHEADER
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   BLOODFLW.FTN
C  Author:       Tom O. Videen
C  Date:         02-Jun-87
C  Written For:  METPROC
C  History:
C     Modified 07-Dec-88 by TOV:
C      1) Counts written to user (with flows) is now PETT counts (i.e., the
C         pie slope (PETTCONV) is now included);
C      2) User may now select maximum flow used to fit flow.
C     Fixed for UNIX 15-Jan-92 TOV
C       FORMAT E14 changed to E14.7
C
C  Intent:
C     Decay is treated explicitly in this model.
C     Compute estimated blood flow for 9 points (10,20,...,90 ml/(min*100 g))
C     and fit a quadratic equation these points (passing through 0,0).
C     The variables AFLOW and BFLOW are the coeficients of the equation:
C
C               CBF = AFLOW*PETT*PETT + BFLOW*PETT
C
C     where CBF is in units of ml of blood/(min*100g of brain)
C        and PETT is PET Counts/(pixel * scan period)
C
C     Note: the actual fit is done with:
C         CBF = AFLOW*P*P + BFLOW*P   where P = PETT * PETTCONV
C     and then the parameters AFLOW and BFLOW are adjusted by:
C         AFLOW = AFLOW*PETTCONV*PETTCONV
C         BFLOW = BFLOW*PETTCONV
C     to eliminate PETTCONV from the from the computed counts.
C
C  The blood flow algorithm is based on a one-compartmental model:
C     Herscovitch, Markham & Raichle (1983)
C        J. Nuclear Medicine, 24:782.
C     Raichle, Martin, Herscovitch, Mintun & Markham (1983)
C        J. Nuclear Medicine, 24:790.
C     Herscovitch (submitted - 1987)
C     Videen, Perlmutter, Herscovitch & Raichle (JCBFM - 1987)
C
C  Common Block Variables:
C     AFLOW,BFLOW = see equation above;
C     NUMPNTS = no. of points;
C     TIME = time (sec)
C     ACTIVITY = non-decay-corrected activity (well counts)/(ml*sec)
C     INUM = number of values in ITIME and IACTIV;
C     ITIME = interpolated blood curve times (twice as many times);
C     IACTIV = interpolated blood curve activities (twice as many);
C        non-decay-corrected;
C     CONV = convolution integral of IACTIV * EXP(-A*ITIME)
C     LAMBDA = partition coefficient (ml/ml);
C     TAU = decay constant of isotope (1/sec)
C     SCANST = initial time
C     SCANEND = final time
C     SCANLEN = length of scan
C     BRAINDEN = density of brain (g/ml)
C
C  Variables:
C     FLOW = blood flow in ml/(min*100g);
C     FLOWSEC = blood flow in ml/(sec*g);
C     COUNTS = PETT tissue counts estimated from true blood flow;
C     INTEGRAL = integral of instantaneous count rate over time
C        of scan;
C     SUMX2,SUMX3,SUMX4,SUMX2Y,SUMXY = sums used in regression fit;
C
C  Uses Subroutines:
C     CONVOLVE
C     INTEGRAT
C     GETINT
C
C  Called by:  METPROC
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE BLOODFLW

      INCLUDE 'metproc.inc'

      REAL*4       COUNTS,INTEGRAL,SUMX2,SUMX3,SUMX4,SUMX2Y,
     &             SUMXY,ACTIV1,ACTIV2,DTIME
      INTEGER*4    MAXFLOW,MINFLOW,INC,I,J
      CHARACTER*80 Q
      CHARACTER*256 RCSHEADER

      RCSHEADER = "$Id: bloodflw.f,v 2.1 1993/07/15 20:25:17 tom Exp tom $"
      SUMX2 = 0.
      SUMX3 = 0.
      SUMX4 = 0.
      SUMX2Y= 0.
      SUMXY = 0.

      MINFLOW = 10
      MAXFLOW = 90
      INC = 10
      WRITE(USEROUT,*)
      Q = 'Maximum flow to use for curve fit (ml/100g/min)'
      CALL GETINT(Q,MAXFLOW,10,400)
      IF (MAXFLOW.LE.50) THEN
        MINFLOW = 5
        INC = 5
      END IF

C  Check if last 2 points contain whole blood and plasma counts:
C  If last two points of blood curve have the same time, then
C  the last point is counts in plasma fraction of blood at the same time
C  as the next to last point, which is the total counts in whole blood.
C  (these counts are derived from a single blood sample at the end of scan)
C  The last point is not integrated in the blood curve.

      IF (TIME(NUMPNTS-1) .EQ. TIME(NUMPNTS)) THEN
        I = NUMPNTS
        WHOLEBLOOD = ACTIVITY(I-1)*EXP(TIME(I)*TAU)
        PLASMA = ACTIVITY(I)*EXP(TIME(I)*TAU)
        HOB2P = WHOLEBLOOD/PLASMA
        NUMPNTS = NUMPNTS-1
      ELSE
        HOB2P = 0
      ENDIF

C  Interpolate ACTIVITY curve
C  (double no. of points to reduce TDIFF in CONVOLVE)

      ITIME(1)  = 0.5*TIME(1)
      IACTIV(1) = 0.5*ACTIVITY(1)
      TIME(NUMPNTS+1) = 0.
      ACTIVITY(NUMPNTS+1) = 0.
      J = 2
      DO 100 I=1,NUMPNTS
        ITIME(J)  = TIME(I)
        IACTIV(J) = ACTIVITY(I)
        J = J+1
        DTIME = 0.5*(TIME(I+1)-TIME(I))
        ITIME(J) = TIME(I) + DTIME
        ACTIV1 = ACTIVITY(I)*EXP(-DTIME*TAU)
        ACTIV2 = ACTIVITY(I+1)*EXP(DTIME*TAU)
        IACTIV(J) = 0.5*(ACTIV1+ACTIV2)
C       ITIME(J) =  0.5*(TIME(I)+TIME(I+1))
C       IACTIV(J) = 0.5*(ACTIVITY(I)+ACTIVITY(I+1))
        J = J+1
100   CONTINUE
      INUM = 2*NUMPNTS

C  FLOW is divided by 6000 to convert from ml/(min*100 g)
C    to FLOWSEC in ml/(sec*g) and then multiplied by BRAINDEN
C    to convert to ml/(sec*ml)
C  LAMBDA is in ml/ml;
C  BRAINDEN is in g/ml;
C  ITIME is in sec and IACTIV is in counts/(sec*ml));
C  Thus, CONVOLVE works on units of seconds and ml;
C  COUNTS is divided by 1000 to prevent an overflow error;
C    this is compensated for by dividing AFLOW 1000*1000 and BFLOW by 1000;

      DO 200 I=MINFLOW,MAXFLOW,INC
        FLOW = FLOAT(I)
        FLOWSEC = BRAINDEN*FLOW/6000.
        CALL CONVOLVE((FLOWSEC/LAMBDA)+TAU)
        CALL INTEGRAT(ITIME,CONV,SCANST,SCANEND,INTEGRAL)
        COUNTS = FLOWSEC*INTEGRAL
        COUNTS = COUNTS/1000.
        SUMX4 = SUMX4 + COUNTS**4
        SUMX3 = SUMX3 + COUNTS**3
        SUMX2 = SUMX2 + COUNTS**2
        SUMXY = SUMXY + COUNTS*FLOW
        SUMX2Y = SUMX2Y + COUNTS*COUNTS*FLOW
        WRITE(USEROUT,8100) FLOW, COUNTS*1000.0/PETTCONV(1)
200   CONTINUE
8100  FORMAT(1X,'FLOW = ',F4.0,' ml/(min*100g)  PETT COUNTS =',F15.3)

C  Compute the coefficients of the best fitting 2nd order polynomial

      BFLOW(1) = (SUMX3*SUMX2Y-SUMXY*SUMX4)/(SUMX3*SUMX3-SUMX2*SUMX4)
      AFLOW(1) = (SUMX2Y-BFLOW(1)*SUMX3)/SUMX4
      AFLOW(1) = AFLOW(1)/(1000.*1000.)
      BFLOW(1) = BFLOW(1)/1000.

C  Include PETT Conversion Factor to Well Counts;
C  PETTCONV = PIESLOPE*60   (well counts*pixel*min)/(PETT*ml*sec)

      AFLOW(1) = AFLOW(1)*PETTCONV(1)*PETTCONV(1)
      BFLOW(1) = BFLOW(1)*PETTCONV(1)

C  Write values to user

      WRITE(USEROUT,8200) 'AFLOW ',AFLOW(1)
      WRITE(USEROUT,8200) 'BFLOW ',BFLOW(1)
      WRITE(USEROUT,8300) 'SUMX4 ',SUMX4
      WRITE(USEROUT,8300) 'SUMX3 ',SUMX3
      WRITE(USEROUT,8300) 'SUMX2 ',SUMX2
      WRITE(USEROUT,8300) 'SUMX2Y',SUMX2Y
      WRITE(USEROUT,8300) 'SUMXY ',SUMXY
      WRITE(USEROUT,*) 'Decay Constant = ',TAU
      WRITE(USEROUT,8400) 'Halflife = ',ALOG(2.0)/TAU
8200  FORMAT(' ',A6,' =',E14.7)
8300  FORMAT(' ',A6,' = ',F20.0)
8400  FORMAT(' ',A11,F12.2)

      RETURN
      END
