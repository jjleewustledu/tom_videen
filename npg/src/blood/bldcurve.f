C$Header: /home/npggw/tom/src/blood/RCS/bldcurve.f,v 2.0 2004/02/20 16:38:58 tom Exp $
C$Log: bldcurve.f,v $
c Revision 2.0  2004/02/20  16:38:58  tom
c Feb 2004
c
c Revision 1.4  2001/06/06  17:59:32  tom
c add ntypes 7-9 for steady-state and steady-inhalation oxygen methods
c
c Revision 1.3  1995/05/10  18:18:13  tom
c added RCSHEADER
c
c Revision 1.2  1995/05/10  16:13:20  tom
c default count time differs for different scanners
c
c Revision 1.1  1992/10/12  16:17:30  ty7777
c Initial revision
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   BLDCURVE.FTN
C  Author:       Tom O. Videen
C  Date:         06-Aug-86
C
C  History:
C     Revised 26-Feb-87 by TOV to allow 5 decimal places for
C        syringe weights (instead of 3);
C     Revised 24-Aug-87 by TOV to fix format statements for the
C        table header;
C
C  Intent:
C     This subroutine writes the values entered for the current
C     blood curve to the terminal.
C
C     Subroutine of BLOOD.FTN
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE BLDCURVE(DONE)

      INCLUDE 'blood.inc'

      REAL      NETWEIGHT
      LOGICAL*1 DONE
      CHARACTER*256 RCSHEADER    ! RCS header information
      RCSHEADER = "$Header: /home/npggw/tom/src/blood/RCS/bldcurve.f,v 2.0 2004/02/20 16:38:58 tom Exp $"

      WRITE(USEROUT,8610)
      WRITE(USEROUT,8620)
      IF (DONE .AND. N.GT.1) THEN
        DO 100 I=1,N-1
          NETWEIGHT = WETWEIGHT(I)-DRYWEIGHT(I)
          WRITE(USEROUT,8630) I,DRYWEIGHT(I),WETWEIGHT(I),NETWEIGHT,
     &      TIMEDRAW(I),TIMECNT(I),COUNTS(I),CORCNTS(I)
100     CONTINUE
      ELSE
        NETWEIGHT = WETWEIGHT(N)-DRYWEIGHT(N)
        WRITE(USEROUT,8630) N,DRYWEIGHT(N),WETWEIGHT(N),NETWEIGHT,
     &    TIMEDRAW(N),TIMECNT(N),COUNTS(N),CORCNTS(N)
      END IF
      WRITE(USEROUT,*)

8610  FORMAT(1X,'SYR     DRY       WET       NET     DRAWN     COUNTED    COUNTS  COUNTS/(ML*SEC)')
8620  FORMAT(1X,'---   -------   -------   -------   ------    -------    ------  ---------------')
8630  FORMAT(1X,I3,2X,F8.5,2X,F8.5,2X,F8.5,3X,F6.2,4X,F6.2,4X,I7,6X,F9.0)

      RETURN
      END
