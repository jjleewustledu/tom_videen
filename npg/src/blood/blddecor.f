C$Header: /home/npggw/tom/src/blood/RCS/blddecor.f,v 2.1 2009/04/22 19:48:29 tom Exp $
C$Log: blddecor.f,v $
c Revision 2.1  2009/04/22  19:48:29  tom
c default scanner = 3
c
c Revision 2.0  2004/02/20  16:38:58  tom
c Feb 2004
c
c Revision 1.3  2001/06/06  17:59:32  tom
c add ntypes 7-9 for steady-state and steady-inhalation oxygen methods
c
c Revision 1.2  1995/05/10  16:13:20  tom
c default count time differs for different scanners
c
c Revision 1.1  1992/10/12  16:17:30  ty7777
c Initial revision
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   BLDDECOR.FTN
C  Author:       Tom O. Videen
C  Date:         14-Jan-88
C  Written for:  BLOOD.FTN
C
C  History:
C     Modified 06-Oct-88 by TOV to fix processing with no decay.
C        LAMBDA is not computed for ISOTOPE = 6.
C
C  Intent:
C     This subroutine calculates the decay-corrected counts,
C     CORCNTS for a single point in a blood curve.
C
C     Decay correction is computed from total activity within
C     the syringe counting period which is corrected to the counts/sec
C     at the beginning of the counting period which would produce this
C     total activity with constant decay (i.e, based on average decay).
C     This differs slightly from using the midpoint to correct counts
C     and is theoretically the most accurate estimate.
C
C  Variable Passed:
C     I = number of the blood point to correct (1-200);
C
C  Variables:
C     WEIGHT = weight of blood sample (grams);
C     CORRECTN = correction factor for well counts;
C        (involves volume of sample and position in well counter)
C     LAMBDA = decay constant (1/sec);
C     COUNTS1 = well counts decay corrected to start of counting period;
C        (based on average decay during the counting period)
C
C  Common Variables:
C     COUNTS - counts per COUNTIME seconds from well counter;
C     DRYWEIGHT - dry weight of syringe;
C     WETWEIGHT - weight of syringe with blood sample;
C     COUNTIME - seconds summed in well counter (usually 10 or 12);
C     TIMECNT - time blood sample was counted (MIN.SEC);
C     CORCNTS - decay corrected counts/(ml*sec);
C        (corrected to time of injection)
C     BLOODDEN = density of blood (g/ml);
C     HALFLIFE = halflife in seconds of ISOTOPE;
C     ISOTOPE = isotope (1-6);
C
C  Uses Function
C     BLDSECS - function name: SECS
C
C  Called by:
C     BLDADDPT
C     BLDALTER
C     BLDCALC
C     BLDENTER
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE BLDDECOR(I)

      INCLUDE 'blood.inc'

      INTEGER*4    I
      REAL*4       CORRECTN,WEIGHT,SECS,COUNTS1,LAMBDA
      REAL*4       FAC, X
      CHARACTER*256 RCSHEADER    ! RCS header information
 
      RCSHEADER = "$Header: /home/npggw/tom/src/blood/RCS/blddecor.f,v 2.1 2009/04/22 19:48:29 tom Exp $"
      WEIGHT   = WETWEIGHT(I)-DRYWEIGHT(I)

C
C Countrate Correction for well counter (see wellcounter_linearity_20070717_C11.xls)
C
      X = (0.001*COUNTS(I))*(12/COUNTIME(I))
C
C      FAC = 0.000005298*X*X + 0.0004575*X + 1.0
C Updated to the current parameters in wellcounter_linearity_20070717_C11.xls
C
      FAC = 0.000003971*X*X + 0.0004575*X + 1.0	
 	
      WRITE(USEROUT,*),"Well Counter Deadtime Correction Factor= ",FAC

      IF (ISOTOPE.EQ.6) THEN                        ! no decay
        COUNTS1 = FLOAT(FAC*COUNTS(I))/COUNTIME(I)
        CORCNTS(I)= BLOODDEN*COUNTS1/WEIGHT
      ELSE                                           ! decay-correct
        LAMBDA  = ALOG(2.0)/HALFLIFE(ISOTOPE)
        COUNTS1 = FLOAT(FAC*COUNTS(I))*LAMBDA/(1.-EXP(-LAMBDA*COUNTIME(I)))
        CORCNTS(I)= BLOODDEN*COUNTS1*EXP(LAMBDA*SECS(TIMECNT(I)))/WEIGHT
      END IF

      CORRECTN = SC1 + SC2*WEIGHT
      IF (CORRECTN.GT.0.) THEN
        CORCNTS(I) = CORCNTS(I)/CORRECTN
      ELSE
        WRITE(USEROUT,*)'*** TOO MUCH BLOOD IN SYRINGE ***'
        WRITE(USEROUT,*)'*** Number of Counts Estimated ***'
        CORCNTS(I) = CORCNTS(I)*FLOAT(DEFTIME)
      END IF

      RETURN
      END
