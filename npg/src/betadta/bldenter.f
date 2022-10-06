C$Header: /home/npggw/tom/src/betadta/RCS/bldenter.f,v 1.1 1993/12/02 22:22:34 tom Exp $
C$Log: bldenter.f,v $
c Revision 1.1  1993/12/02  22:22:34  tom
c Initial revision
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   bldenter.f
C
C  Author:       Tom O. Videen
C  Date:         27-Oct-86
C  Written for:  BLOOD.FTN
C
C  Intent:
C     This subroutine get blood curve points from the user.
C
C  History:
C     Modified 14-Jan-88 by TOV so decay correction is computed by
C        including the average decay during the well counting period.
C        This is now always computed through a single subroutine BLDDECOR.
C			Modified 02-Dec-93 by TOV for betadta.
C
C  Common Variables whose values are set in this subroutine:
C     COUNTS - counts per COUNTIME seconds from well counter;
C     DRYWEIGHT - dry weight of syringe;
C     WETWEIGHT - weight of syringe with blood sample;
C     COUNTIME - seconds summed in well counter (usually 10);
C     TIMEDRAW - time blood sample was taken (MIN.SEC);
C     TIMECNT - time blood sample was counted (MIN.SEC);
C     CORCNTS - decay corrected counts/(ml*sec);
C        (corrected to time of injection)
C     BLOODDEN = density of blood (g/ml);
C
C  Uses Function:
C     SECS in BLDSECS
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE BLDENTER(LAMBDA, LASTTIME, CORCNTS)
C
			REAL*4			LAMBDA
      REAL*4       SC1
      REAL*4       SC2
      REAL*4       BLOODDEN
      REAL*4       DRYWEIGHT
      REAL*4       WETWEIGHT
			REAL*4			 WEIGHT
      REAL*4       SECS
      REAL*4       COUNTIME
			REAL*4			 LASTTIME
      REAL*4       TIMEDRAW
      REAL*4       TIMESECS
      REAL*4       TIMECNT
			REAL*4			 TIMECOUNTED
      REAL*4       CORCNTS
      REAL*4       CORRECTN
      REAL*4       COUNTS1

      INTEGER*4    USERIN    ! log unit assigned for terminal input!
      INTEGER*4    USEROUT   ! log unit for terminal output!
			INTEGER*4		 COUNTS

      CHARACTER*80 Q
      CHARACTER*1  BELL
			CHARACTER*256	RCSHEADER

      COMMON /USRIO/ USERIN,USEROUT

      DATA SC1, SC2 /1.026, -0.0522/		! new caps, 3cc syringes
			DATA BLOODDEN /1.05/
			RCSHEADER = "$Id"
C
      BELL = CHAR(7)
      DONE = .FALSE.

      Q = 'Dry syringe weight (grams)'
      CALL GETREAL (Q, DRYWEIGHT, 0.0, 100.0)

      Q = 'Wet syringe weight (grams)'
      CALL GETREAL (Q, WETWEIGHT, 0.0, 100.0)

      WEIGHT   = WETWEIGHT - DRYWEIGHT
      IF (WEIGHT .EQ. 0.) THEN
        RETURN
      END IF

10    Q = 'Time Sampled (MIN.SEC)'
      CALL GETREAL (Q, TIMEDRAW, 0., 300.)
      TIMESECS = SECS (TIMEDRAW)
			IF (TIMESECS .LT. LASTTIME) THEN
				WRITE(USEROUT,*) "Sample Time must be >= ", LASTTIME,BELL
				GO TO 10
			END IF
			Q = "Time Sampled in SECONDS"
			CALL GETREAL(Q,TIMESECS,0.,6000.)

20    Q = 'Time Counted (MIN.SEC)'
      CALL GETREAL (Q, TIMECNT, 1., 6000.0)
      TIMECOUNTED = SECS (TIMECNT)
			IF (TIMECOUNTED .LT. TIMESECS) THEN
				WRITE(USEROUT,*) "Count Time must be >= ", TIMESECS,BELL
				GO TO 20
			END IF
			Q = "Time Counted in SECONDS"
			CALL GETREAL(Q,TIMECOUNTED,0.,6000.)

      Q = 'Well count period (seconds)'
			COUNTIME = 10.
      CALL GETREAL (Q, COUNTIME, 1.0, 1000.0)

      Q = 'Number of counts'
			CALL GETINT (Q, COUNTS, 0, 999999)
C
C		Decay correct counts
C
      COUNTS1 = FLOAT(COUNTS)*LAMBDA/(1.-EXP(-LAMBDA*COUNTIME))
      CORCNTS = BLOODDEN*COUNTS1*EXP(LAMBDA*TIMECOUNTED)/WEIGHT

      CORRECTN = SC1 + SC2*WEIGHT
      IF (CORRECTN.GT.0.) THEN
        CORCNTS = CORCNTS/CORRECTN
      ELSE
        WRITE(USEROUT,*)'*** TOO MUCH BLOOD IN SYRINGE ***'
        WRITE(USEROUT,*)'*** Number of Counts Estimated ***',BELL
        CORCNTS = CORCNTS*10.
      END IF

			LASTTIME = TIMESECS

      RETURN
      END
