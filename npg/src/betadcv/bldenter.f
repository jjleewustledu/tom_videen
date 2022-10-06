C$Header: /home/npggw/tom/src/betadcv/RCS/bldenter.f,v 2.4 2009/04/22 19:23:40 tom Exp $
C$Log: bldenter.f,v $
c Revision 2.4  2009/04/22  19:23:40  tom
c changed computation of well counter nonlinearity and moved it prior to decay correction
c
c Revision 2.3  2008/11/14  20:41:21  tom
c *** empty log message ***
c
c Revision 2.2  2007/04/06  21:18:44  tom
c enter total counts and background separately
c
c Revision 2.1  2007/03/20  22:33:36  tom
c change prompt in bldenter
c
c Revision 2.0  2004/02/13  19:45:46  tom
c Feb 2004
c
c Revision 1.3  2002/11/27  22:01:56  tom
c *** empty log message ***
c
c Revision 1.2  1998/07/27  17:51:43  tom
c automatically subtract 105 sec for added points to CO curve
c
c Revision 1.1  1995/10/03  18:37:03  tom
c Initial revision
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:	bldenter.f
C  Author:		Tom O. Videen
C  Date:		27-Oct-86
C  Written for:	BLOOD.FTN
C  Intent:		Get blood curve points from user.
C
C  History:
C     Modified 14-Jan-88 by TOV so decay correction is computed by
C	  including the average decay during the well counting period.
C	  This is now always computed through a single subroutine BLDDECOR.
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
C	  (corrected to time of injection)
C     BLOODDEN = density of blood (g/ml);
C
C  Uses Function:
C     SECS in BLDSECS
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

	SUBROUTINE BLDENTER(LAMBDA, LASTTIME, CORCNTS, SCANTYPE)

	REAL*4   LAMBDA
	REAL*4	 SC1
	REAL*4	 SC2
	REAL*4	 BLOODDEN
	REAL*4	 DRYWEIGHT
	REAL*4	 WETWEIGHT
	REAL*4	 WEIGHT
	REAL*4	 SECS
	REAL*4	 COUNTIME
	REAL*4	 LASTTIME
	REAL*4	 TIMEDRAW
	REAL*4	 TIMESECS
	REAL*4	 TIMECNT
	REAL*4	 TIMECOUNTED
	REAL*4	 CORCNTS
	REAL*4	 CORRECTN
	REAL*4	 COUNTS1
	REAL*4	 FAC, X

	INTEGER*4    SCANTYPE
	INTEGER*4    USERIN    ! log unit assigned for terminal input!
	INTEGER*4    USEROUT   ! log unit for terminal output!
	INTEGER*4	 COUNTS
	INTEGER*4	 TOTCNTS
	INTEGER*4	 BKGCNTS

	CHARACTER*80 Q
	CHARACTER*1  BELL
	CHARACTER*256	RCSHEADER
	LOGICAL*1	ANSWER

	COMMON /USRIO/ USERIN,USEROUT

	DATA SC1, SC2 /1.026, -0.0522/		! new caps, 3cc syringes
	DATA BLOODDEN /1.05/
	RCSHEADER = "$Id: bldenter.f,v 2.4 2009/04/22 19:23:40 tom Exp $"
	BELL = CHAR(7)

	Q = 'Well count period (seconds)'
	COUNTIME = 12.
	CALL GETREAL (Q, COUNTIME, 1.0, 1000.0)

10	Q = 'Dry syringe weight (grams)'
	CALL GETREAL (Q, DRYWEIGHT, 0.0, 100.0)

	Q = 'Wet syringe weight (grams)'
	CALL GETREAL (Q, WETWEIGHT, 0.0, 100.0)

	WEIGHT   = WETWEIGHT - DRYWEIGHT
	IF (WEIGHT .EQ. 0.) THEN
	  RETURN
	ELSE IF (WEIGHT .GT. 1.) THEN
	  WRITE(USEROUT,*) "WARNING: Syringe Volume > 1 cc"
	  Q = 'Are you sure'
	  ANSWER = .FALSE.
	  CALL YESNO(Q, ANSWER)
	  IF (.NOT.ANSWER) THEN
		GOTO 10
	  ENDIF
	END IF

	WRITE(USEROUT,*) " "
	WRITE(USEROUT,*) ">>> NOTE THE FORMAT FOR ENTERING TIME!"
	IF (SCANTYPE .EQ. 3) THEN
		WRITE(USEROUT,*) "   (Time will be adjusted by -1:45 for standard CO study)"
	ENDIF
	WRITE(USEROUT,*) " "
20    Q = 'Time Sampled (MIN.SEC)'
	CALL GETREAL (Q, TIMEDRAW, 0., 120.)
	TIMESECS = SECS (TIMEDRAW)
	IF (SCANTYPE .EQ. 3) TIMESECS = TIMESECS - 105
	IF (TIMESECS .LT. LASTTIME) THEN
		WRITE(USEROUT,*) "Sample Time must be >= ", LASTTIME,BELL
		GOTO 20
	END IF

30    Q = 'Time Counted (MIN.SEC)'
	CALL GETREAL (Q, TIMECNT, 1., 121.0)
	TIMECOUNTED = SECS (TIMECNT)
	IF (SCANTYPE .EQ. 3) TIMECOUNTED = TIMECOUNTED - 105
	IF (TIMECOUNTED .LT. TIMESECS) THEN
		WRITE(USEROUT,*) "Count Time must be >= ", TIMESECS,BELL
		GOTO 30
	END IF

	Q = 'Total counts'
	CALL GETINT (Q, TOTCNTS, 0, 999999)
	Q = 'Background counts'
	CALL GETINT (Q, BKGCNTS, 0, TOTCNTS)
	COUNTS = TOTCNTS - BKGCNTS
C
C	Countrate Correction for well counter (see wellcounter_linearity_20070717_C11.xls)
C	Correct before decay-correction; prompt for correction factor
C
	X = (0.001*COUNTS)*(12/COUNTIME)
	FAC = 0.000005298*X*X + 0.0004575*X + 1.0
	WRITE(USEROUT,*),"Correction Factor= ",FAC
	Q = 'Countrate Correction Factor'
	CALL GETREAL (Q, FAC, 1.0, 2.0)
C
C	Decay correct counts
C
	COUNTS1 = FLOAT(FAC*COUNTS)*LAMBDA/(1.-EXP(-LAMBDA*COUNTIME))
	CORCNTS = BLOODDEN*COUNTS1*EXP(LAMBDA*TIMECOUNTED)/WEIGHT
C
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
