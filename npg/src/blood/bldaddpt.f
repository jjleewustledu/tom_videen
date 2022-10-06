C$Header: /home/npggw/tom/src/blood/RCS/bldaddpt.f,v 2.0 2004/02/20 16:38:58 tom Exp $
C$Log: bldaddpt.f,v $
c Revision 2.0  2004/02/20  16:38:58  tom
c Feb 2004
c
c Revision 1.2  1995/05/10  16:13:20  tom
c default count time differs for different scanners
c
c Revision 1.1  1992/10/12  16:17:30  ty7777
c Initial revision
c
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   BLDADDPT.FTN
C
C  Author:       Tom O. Videen
C  Date:         27-Oct-86
C  Written for:  BLOOD.FTN
C
C  Intent:
C     This subroutine adds a single point to a blood curve.
C
C  History:
C     Modified 14-Jan-88 by TOV so decay correction is computed by
C        including the average decay during the well counting period.
C        This is now always computed through a single subroutine BLDDECOR.
C
C  Variable Passed:
C     M = number of the point to add a point BEFORE;
C         (i.e., M is the number of the new point)
C
C  Common Variables whose values are set in this subroutine:
C     N - number of current point in blood curve;
C     COUNTS - counts per COUNTIME seconds from well counter;
C     DRYWEIGHT - dry weight of syringe;
C     WETWEIGHT - weight of syringe with blood sample;
C     COUNTIME - seconds summed in well counter (usually 10);
C     TIMEDRAW - time blood sample was taken (MIN.SEC);
C     TIMESECS - time blood sample was taken in seconds;
C     TIMECNT - time blood sample was counted (MIN.SEC);
C     CORCNTS - decay corrected counts/(ml*sec);
C        (corrected to time of injection)
C
C  Uses Function:
C     BLDDECOR  - decay correct a single point in a blood curve;
C     SECS in BLDSECS
C
C  Uses TERMINAL.LIB
C     GETINT
C     GETREAL
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE BLDADDPT(M)
C
      INCLUDE 'blood.inc'
C
      INTEGER*4    M,I,J
      REAL         SECS
      CHARACTER*80 Q
      CHARACTER*256 RCSHEADER    ! RCS header information

      RCSHEADER = "$Header: /home/npggw/tom/src/blood/RCS/bldaddpt.f,v 2.0 2004/02/20 16:38:58 tom Exp $"
C
C  Shift all points from M to the end of the array
C
      DO 100 I=N,M+1,-1
        J = I-1
        DRYWEIGHT(I) = DRYWEIGHT(J)
        WETWEIGHT(I) = WETWEIGHT(J)
        TIMEDRAW(I)  = TIMEDRAW(J)
        TIMESECS(I)  = TIMESECS(J)
        TIMECNT(I)   = TIMECNT(J)
        COUNTIME(I)  = COUNTIME(J)
        COUNTS(I)    = COUNTS(J)
        CORCNTS(I)   = CORCNTS(J)
100   CONTINUE
C
C  Zero the values of the point M
C
      DRYWEIGHT(M) = 0.
      WETWEIGHT(M) = 0.
      TIMEDRAW(M)  = 0.
      TIMESECS(M)  = 0.
      TIMECNT(M)   = 0.
      COUNTS(M)    = 0
      CORCNTS(M)   = 0.
C
      WRITE(USEROUT,*)
      IF (SAVE) THEN
        WRITE(USEROUT,*)'Point #',(M)
        WRITE(USEROUT,*)'---------'
      END IF
      Q = 'Dry syringe weight (grams)'
      CALL GETREAL(Q,DRYWEIGHT(M),0.,100.)
      Q = 'Wet syringe weight (grams)'
      CALL GETREAL(Q,WETWEIGHT(M),DRYWEIGHT(M)+0.001,100.)
      Q = 'Time Drawn (MIN.SEC)'
      IF (M.EQ.1) THEN
        CALL GETREAL(Q,TIMEDRAW(M),0.,TIMEDRAW(M+1))
      ELSE IF (M.LT.N-1) THEN
        CALL GETREAL(Q,TIMEDRAW(M),TIMEDRAW(M-1),TIMEDRAW(M+1))
      ELSE
        CALL GETREAL(Q,TIMEDRAW(M),TIMEDRAW(M-1),600.)
      END IF
      TIMESECS(M) = SECS(TIMEDRAW(M))
      Q = 'Time Counted (MIN.SEC)'
      CALL GETREAL(Q,TIMECNT(M),TIMEDRAW(M)+0.01,601.)
      IF (VARYTIME) THEN
        IF (M.EQ.1) THEN
          COUNTIME(M) = FLOAT(DEFTIME)
        ELSE
          COUNTIME(M) = COUNTIME(M-1)
        END IF
        Q = 'Well count period (seconds)'
        CALL GETREAL(Q,COUNTIME(M),1.,1000.)
        Q = 'Number of counts'
      ELSE IF (DEFTIME.EQ.12) THEN
        Q = 'Number of counts per 12 seconds'
      ELSE
        Q = 'Number of counts per 10 seconds'
      END IF
      CALL GETINT(Q,COUNTS(M),0,999999)
      CALL BLDDECOR(M)
C
C  There is one more point in the array
C
      N = N+1
C
      RETURN
      END
