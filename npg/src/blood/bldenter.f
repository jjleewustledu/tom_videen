C$Header: /home/npggw/tom/src/blood/RCS/bldenter.f,v 2.0 2004/02/20 16:38:58 tom Exp $
C$Log: bldenter.f,v $
c Revision 2.0  2004/02/20  16:38:58  tom
c Feb 2004
c
c Revision 1.4  2001/06/06  17:59:32  tom
c add ntypes 7-9 for steady-state and steady-inhalation oxygen methods
c
c Revision 1.3  1995/05/10  16:13:20  tom
c default count time differs for different scanners
c
c Revision 1.2  1992/10/13  19:06:32  ty7777
c Working version.
c
c Revision 1.1  1992/10/12  16:17:30  ty7777
c Initial revision
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   bldenter.f
C  Author:       Tom O. Videen
C  Date:         27-Oct-86
C  Written for:  BLOOD.FTN
C  Intent:
C     This subroutine get blood curve points from the user.
C
C  History:
C     Modified 14-Jan-88 by TOV so decay correction is computed by
C        including the average decay during the well counting period.
C        This is now always computed through a single subroutine BLDDECOR.
C
C  Common Variables whose values are set in this subroutine:
C     COUNTS - counts per COUNTIME seconds from well counter;
C     N - number of current point in blood curve;
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
C     BLDDECOR  - decay correct a single point in a blood curve;
C     SECS in BLDSECS
C
C  Uses TERMINAL.LIB
C     GETINT
C     GETREAL
C     YESNO
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE BLDENTER(DONE)

      INCLUDE 'blood.inc'

      REAL         SECS
      CHARACTER*80 Q
      CHARACTER*256 RCSHEADER    ! RCS header information
      LOGICAL*1    DONE

      RCSHEADER = "$Header: /home/npggw/tom/src/blood/RCS/bldenter.f,v 2.0 2004/02/20 16:38:58 tom Exp $"
      DONE = .FALSE.
      WRITE(USEROUT,*)
      IF (SAVE) THEN
        WRITE(USEROUT,*)'Point #',(N)
        WRITE(USEROUT,*)'---------'
      END IF
      WRITE(USEROUT,*) 'If no more points, enter 0 for dry and wet weights.'
      Q = 'Dry syringe weight (grams)'
      CALL GETREAL (Q, DRYWEIGHT (N), 0.0, 100.0)
      Q = 'Wet syringe weight (grams)'
      CALL GETREAL (Q, WETWEIGHT (N), 0.0, 100.0)
      IF (WETWEIGHT (N) .EQ. DRYWEIGHT (N)) THEN
        DONE = .TRUE.
        RETURN
      END IF
      Q = 'Time Drawn (MIN.SEC)'
      IF (N.EQ.1) THEN
        CALL GETREAL (Q, TIMEDRAW (N), 0.0, 600.0)
      ELSE
        CALL GETREAL (Q, TIMEDRAW (N), TIMEDRAW (N - 1), 600.0)
      END IF
      TIMESECS (N) = SECS (TIMEDRAW (N))
      Q = 'Time Counted (MIN.SEC)'
      CALL GETREAL (Q, TIMECNT (N), TIMEDRAW (N) + 0.01, 601.0)
      IF (VARYTIME) THEN
        IF (N .EQ. 1) THEN
          COUNTIME (N) = FLOAT(DEFTIME)
        ELSE
          COUNTIME (N) = COUNTIME (N - 1)
        END IF
        Q = 'Well count period (seconds)'
        CALL GETREAL (Q, COUNTIME (N), 1.0, 1000.0)
        Q = 'Number of counts'
      ELSE IF (DEFTIME.EQ.12) THEN
        Q = 'Number of counts per 12 seconds'
      ELSE
        Q = 'Number of counts per 10 seconds'
      END IF
      CALL GETINT (Q, COUNTS (N), 0, 999999)
      CALL BLDDECOR (N)

      RETURN
      END
