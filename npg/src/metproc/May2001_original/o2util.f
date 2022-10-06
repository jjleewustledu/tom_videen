c $Log: o2util.f,v $
c Revision 2.2  2000/11/14  21:53:01  tom
c increase array size to 400
c
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
C  Subroutine:   O2UTIL.FTN
C
C  Author:       Tom O. Videen
C  Date:         02-Jun-87
C  Written For:  METPROC
C
C  History:
C     Modified 24-May-88 by TOV to eliminate the need for a redundant
C        point in the blood curve.  Previously, this routine used the
C        last 2 points in the curve for computing the fraction of O15
C        in water but did not use the second to the last point as part
C        of the curve for total counts.  This revision uses the next
C        to last point of the curve as part of the curve for total counts
C        (i.e., NUMPNTS is reduced by 1 instead of 2).
C
C  Intent:
C     Decay is treated explicitly in this model.
C     Subroutine for computing the coefficients needed to compute
C     the rate of oxygen consumption (CMRO2) and the oxygen
C     extraction rate (E).
C
C  Notes:
C     These coefficients are used in SPIDERAM as follows:
C
C         PETTCONV*PETT1 - Water Counts - SLVFACTOR*CBV*O2CNTS*BRAINDEN/100.
C     E = ------------------------------------------------------------------
C              Oxygen Counts - 0.835*SLVFACTOR*CBV*O2CNTS*BRAINDEN/100.
C
C     CMRO2 = E*CBF*OXYCONT
C
C     where:
C        PETTCONV = PETT conversion factor = PIESLOPE*60;
C        PETT = PET counts/(pixel*scan)
C           PETT1 = counts for O2 scan
C           PETT2 = counts for H2O scan
C           PETT3 = counts for CO scan
C        Water Counts = counts due to recirculating water of metabolism;
C           = AWATER*CBF*CBF + BWATER*CBF
C           (well counts/(g*scan period))
C        Oxygen Counts = counts due to oxygen if 100% extraction of O2
C           = AOXYGEN*CBF*CBF + BOXYGEN*CBF
C           (well counts/(g*scan period))
C        O2CNTS = integral of blood O2 counts during scan
C           (well counts/(g*scan period))
C        0.835 = (postcapillary vol + 0.5*capillary vol)/CBV
C        SLVRATIO = ratio of cerebral small vessel hematocrit to
C           large vessel hematocrit = 0.85;
C        CBV = BVFACTOR*PETT3  (ml/g)
C        BRAINDEN - density of brain (g/ml)
C        CBF = AFLOW*PETT2*PETT2 + BFLOW*PETT2  (ml/(min*100g))
C
C  References:
C     Mintun, Raichle, Martin & Herscovitch (1984)
C        J. Nuclear Medicine, 25:177
C     Herscovitch, Mintun & Raichle, (1985)
C        J. Nuclear Medicine, 26:416
C     Videen, Perlmutter, Herscovitch & Raichle (JCBFM 1987)
C
C  Variables:
C     H2OACTIV = counts from O-15 water;
C     O2ACTIV = counts from O-15 O2;
C     ALLCNTS  = total counts in blood at FRACTIME;
C     WATERCNT = counts from water fraction at last time (FRACTIME);
C        fraction of counts in water and in O2;
C     MAXACTIV = maximum activity in blood curve;
C     HEM = hematocrit expressed as a fraction (instead of percent);
C     CORCNTS = decay-corrected counts; (ACTIVITY decay-corrected with TAU)
C        (needed to separate O2 and H2O counts)
C
C  Common Block Variables:
C     AWATER,BWATER = coefficients of eqn:
C         Counts = AWATER*CBF*CBF + BWATER*CBF
C         Counts= local tissue counts due to recirculating water of metabolism
C            well counts/(ml*scan period)
C         CBF = ml/(min*100 g)
C     AOXYGEN,BOXYGEN = coefficients of eqn:
C         Counts = AOXYGEN*CBF*CBF + BOXYGEN * CBF
C         Counts = local tissue counts if 100% extraction of arterial O2
C            well counts/(ml*scan period)
C         CBF = ml/(min*100 g)
C     O2CNTS = integral of blood oxygen counts from start of scan;
C     HEMATOCRIT = hematocrit (percent);
C     FRACTIME = time at which sample was taken for determining H2O fraction;
C     PEAKTIME = time at which MAXACTIV appeared;
C
C                   66 g water         g rbc
C                  -----------  * 1.10 -----
C                   100 g rbc           ml
C     RBCFACTOR = -------------------------------  = 0.766
C                   92 g water         g plasma
C                  -----------  * 1.03 --------
C                  100 g plasma           ml
C
C  Uses Subroutines:
C     INTCONV
C     INTEGRAT
C
C  TERMINAL.LIB
C     GETREAL
C
C  FORTRAN Runtime Library
C     ENCODE
C
C  Called by:  METPROC
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE O2UTIL
C
      INCLUDE 'metproc.inc'
C
      REAL         CORCNTS(400),H2OACTIV(400),O2ACTIV(400),WATERCNT,
     &             MAXACTIV,ALLCNTS,PLASMACNT,HEM
      INTEGER*4    I,K
      CHARACTER*1  BELL
      CHARACTER*6  NUM
      CHARACTER*80 Q
      LOGICAL*1    A
			CHARACTER*256 RCSHEADER
C
			RCSHEADER = "$Id: o2util.f,v 2.2 2000/11/14 21:53:01 tom Exp $"
C
      BELL = CHAR(7)
C
C  Decay-correct ACTIVITY to Time=0 and
C  Find time and value of the peak of the blood counts curve
C
      MAXACTIV = 0.
      PEAKTIME = 0.
      DO 100 I=1,NUMPNTS
        CORCNTS(I) = ACTIVITY(I)*EXP(TIME(I)*TAU)
        IF (CORCNTS(I).GT.MAXACTIV) THEN
          PEAKTIME = TIME(I)
          MAXACTIV = CORCNTS(I)
        END IF
100   CONTINUE
C
C  IF HEMATOCRIT > 0 then
C  Last two points of blood curve are used to separate O2 and H2O fractions:
C  The last point is counts in plasma fraction of blood at the same time
C  as the next to last point, which is the total counts in whole blood.
C  (these counts are derived from a single blood sample at the end of scan)
C  The last point is not integrated in the blood curve, but the next to
C  last point is.
C
      IF (HEMATOCRIT.GT.0.0 .AND.TIME(NUMPNTS-1).EQ.TIME(NUMPNTS)) THEN
        HEM = HEMATOCRIT/100.
        FRACTIME = TIME(NUMPNTS)
        ALLCNTS = CORCNTS(NUMPNTS-1)
        PLASMACNT = CORCNTS(NUMPNTS)
        WATERCNT = HEM*RBCFACTOR*PLASMACNT + (1.-HEM)*PLASMACNT
        H2OFRAC = WATERCNT/ALLCNTS
        NUMPNTS = NUMPNTS-1
C
C  ELSE get FRACTIME and H2OFRAC from USER
C  (The values of 50.0 seconds and 0.198 were obtained from 6 subjects)
C
      ELSE
200     WRITE(USEROUT,*) BELL
        IF (HEMATOCRIT.GT.0.0 .AND. TIME(NUMPNTS-1).NE.TIME(NUMPNTS))
     &    WRITE(USEROUT,8000)
        WRITE(USEROUT,8100)
        WRITE(USEROUT,8150) PEAKTIME
        WRITE(USEROUT,8160) TIME(NUMPNTS)
        Q = 'Time at which water fraction is estimated'
        FRACTIME = PEAKTIME + 50.0
        CALL GETREAL(Q,FRACTIME,1.0,1000.0)
        WRITE(NUM,8200) FRACTIME
        Q = 'Fraction of total counts due to water at '//
     &      NUM//' seconds'
        H2OFRAC = 0.198
        CALL GETREAL(Q,H2OFRAC,0.01,1.0)
        K = 0
        DO 300 I=1,NUMPNTS
          IF (TIME(I).GE.FRACTIME) THEN
            K = I-1
            ALLCNTS = CORCNTS(K) + (CORCNTS(K+1)-CORCNTS(K))*
     &         (FRACTIME-TIME(K))/(TIME(K+1)-TIME(K))
            WATERCNT = H2OFRAC*ALLCNTS
            GO TO 400
          END IF
300     CONTINUE
C
C  Specified FRACTIME is after the last blood time
C
        H2OFRAC = H2OFRAC*(TIME(NUMPNTS)-PEAKTIME)/(FRACTIME-PEAKTIME)
        FRACTIME = TIME(NUMPNTS)
        WATERCNT = H2OFRAC*CORCNTS(NUMPNTS)
        WRITE(USEROUT,8170) BELL
        WRITE(USEROUT,8171) FRACTIME
        WRITE(USEROUT,8172) FRACTIME,H2OFRAC
        Q = 'Use this water fraction'
        A = .TRUE.
        CALL YESNO(Q,A)
        IF (.NOT.A) GO TO 200
      END IF
C
8000  FORMAT(1X,'*** THE LAST 2 BLOOD POINTS HAVE DIFFERENT ',
     &    'SAMPLE TIMES')
8100  FORMAT(1X,'*** NO PLASMA SAMPLE WAS COUNTED')
8150  FORMAT(1X,'Peak Activity at ',F6.1,' seconds after injection')
8160  FORMAT(1X,'End of Blood Curve at ',F6.1,' seconds')
8170  FORMAT(1X,'The time you have chosen for estimating the water ',
     &   'fraction',A1)
8171  FORMAT(1X,'is after the last blood curve time,',F,'seconds.')
8172  FORMAT(1X,'The comparable fraction at ',F,'seconds is ',F)
8200  FORMAT(F6)
C
C  Separate H2O and O2 fractions (blood counts attributed to water
C     and oxygen, respectively);
C  H2O is approximated by a straight line with a value of zero at
C     the peak of the blood curve (with decay-corrected counts).
C
400   DO 500 I=1,NUMPNTS
        IF (TIME(I).LT.PEAKTIME) THEN
          H2OACTIV(I) = 0.
        ELSE
          H2OACTIV(I) = WATERCNT*(TIME(I)-PEAKTIME)/(FRACTIME-PEAKTIME)
        END IF
        O2ACTIV(I) = CORCNTS(I) - H2OACTIV(I)
500   CONTINUE
C
C  For each fraction, calculate a convolution integral
C  and express the result as a 2nd order equation of flow
C
      CALL INTCONV(H2OACTIV,AWATER(1),BWATER(1))
      CALL INTCONV(O2ACTIV,AOXYGEN(1),BOXYGEN(1))
C
C  Calculate the integral of the blood O2
C  Note: after calling INTCONV, the array IACTIV contains the
C     O2 activity with decay restored with twice as many points.
C     Corresponding times are in the array ITIME.
C
      CALL INTEGRAT(ITIME,IACTIV,SCANST,SCANEND,O2CNTS(1))
      WRITE(USEROUT,8300) O2CNTS(1)
8300  FORMAT(1X,'Integral of O2 Counts =',F15.0)
C
      RETURN
      END
