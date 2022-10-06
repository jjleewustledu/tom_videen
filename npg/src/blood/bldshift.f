C$Header: /home/npggw/tom/src/blood/RCS/bldshift.f,v 2.0 2004/02/20 16:38:58 tom Exp $
C$Log: bldshift.f,v $
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
C  Subroutine:   BLDSHIFT.FTN
C
C  Author:       Tom O. Videen
C  Date:         17-Sep-86
C
C  Intent:
C     This subroutine shifts the blood curve points.
C
C     Subroutine of BLOOD.FTN
C
C  Uses Function:
C     SECS in BLDSECS
C
C  Uses TERMINAL.LIB
C     GETINT
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE BLDSHIFT
C
      INCLUDE 'blood.inc'
C
      INTEGER*4    I,SHIFT
      REAL         SECS
      CHARACTER*80 Q
      CHARACTER*256 RCSHEADER    ! RCS header information
      RCSHEADER = "$Header: /home/npggw/tom/src/blood/RCS/bldshift.f,v 2.0 2004/02/20 16:38:58 tom Exp $"
C
      SHIFT = NINT(TIMESECS(1)-SECS(TIMEDRAW(1)))
      Q = 'Amount to shift curve (in seconds)'
      CALL GETINT(Q,SHIFT,-60,60)
      DO 100 I=1,N-1
        TIMESECS(I) = SECS(TIMEDRAW(I)) + FLOAT(SHIFT)
100   CONTINUE
C
      RETURN
      END
