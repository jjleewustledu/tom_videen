C $Log$
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   O2STEADY.FTN
C  Author:       Tom O. Videen
C  Date:         May 2001
C  Written For:  METPROC
C  Intent:
C     The only factors which can be computed without the CBF and CBV
C     values are the ratio of whole blood to plasma counts (for O2)
C     and the plasma concentration of oxygen-15 (assumed to be all water).
C     Everything else gets computed in procSSOxygen (libimgutil.h).
C
C  References:
C     Lammertsma & Jones (1983) JCBFM, 3:416-424
C     The model uses equations C.7, 13, and 14.
C     Herscovitch notes from Feb 1984
C
C  Common Block Variables:
C     OOB2P = whole blood counts/plasma counts (following O2 inhalation)
C     OOPLASMA = plasma well counts/ml/sec
C
C  Called by:  METPROC
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE O2STEADY

      INCLUDE 'metproc.inc'

      REAL         CORCNTS(400), WHOLEBLOOD, PLASMA
      INTEGER*4    I

      CHARACTER*256 RCSHEADER
      RCSHEADER = "$Id: o2util.f,v 2.2 2000/11/14 21:53:01 tom Exp $"

      DO 100 I=1,NUMPNTS
        CORCNTS(I) = ACTIVITY(I)*EXP(TIME(I)*TAU)
100   CONTINUE

C  Last two points of blood curve are used to separate O2 and H2O fractions:
C  The last point is counts in plasma fraction of blood at the same time
C  as the next to last point, which is the total counts in whole blood.
C  (these counts are derived from a single blood sample at the end of scan)
C  The last point is not integrated in the blood curve, but the next to
C  last point is.

      IF (TIME(NUMPNTS-1).NE.TIME(NUMPNTS)) THEN
        WRITE(USEROUT,*) "*** THE LAST 2 BLOOD POINTS HAVE DIFFERENT TIMES"
        WRITE(USEROUT,*) "*** No Whole Blood/Plasma samples"
        RETURN
      ENDIF
 
      WHOLEBLOOD = CORCNTS(NUMPNTS-1)
      PLASMA = CORCNTS(NUMPNTS)
      OOB2P = WHOLEBLOOD/PLASMA
      OOPLASMA = PLASMA
      AOXYGEN(1) = 0.
      BOXYGEN(1) = 0.
      AWATER(1) = 0.
      BWATER(1) = 0.

      RETURN
      END
