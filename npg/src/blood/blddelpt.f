C$Header: /home/npggw/tom/src/blood/RCS/blddelpt.f,v 2.0 2004/02/20 16:38:58 tom Exp $
C$Log: blddelpt.f,v $
c Revision 2.0  2004/02/20  16:38:58  tom
c Feb 2004
c
c Revision 1.2  1995/05/10  18:18:13  tom
c added RCSHEADER
c
c Revision 1.1  1992/10/12  16:17:30  ty7777
c Initial revision
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   BLDDELPT.FTN
C
C  Author:       Tom O. Videen
C  Date:         21-Aug-86
C
C  Intent:
C     This subroutine deletes a single point from a blood curve.
C
C     Subroutine of BLOOD.FTN
C
C  Variable Passed:
C     M = number of the point to delete;
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
C
C  No Subroutines
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE BLDDELPT(M)
C
      INCLUDE 'blood.inc'
C
      INTEGER*4    M,I,J
      CHARACTER*1  BELL
      CHARACTER*256 RCSHEADER    ! RCS header information
      RCSHEADER = "$Header: /home/npggw/tom/src/blood/RCS/blddelpt.f,v 2.0 2004/02/20 16:38:58 tom Exp $"
C
      BELL = CHAR(7)
      IF (M.GE.N) THEN
        WRITE(USEROUT,*) 'Point #',M,' does not exist',BELL
        RETURN
      END IF
C
C  From point M to the next to last point,
C  set the values to those of the next higher point in the array
C
      DO 100 I=M,N-2
        J = I+1
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
C  Zero the values of the last point
C
      I = N-1
      DRYWEIGHT(I) = 0.
      WETWEIGHT(I) = 0.
      TIMEDRAW(I)  = 0.
      TIMESECS(I)  = 0.
      TIMECNT(I)   = 0.
      COUNTS(I)    = 0
      CORCNTS(I)   = 0.
C
C  There is one less point in the array
C
      N = N-1
C
      RETURN
      END
