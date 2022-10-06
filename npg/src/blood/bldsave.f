C$Header: /home/npggw/tom/src/blood/RCS/bldsave.f,v 2.0 2004/02/20 16:38:58 tom Exp $
C$Log: bldsave.f,v $
c Revision 2.0  2004/02/20  16:38:58  tom
c Feb 2004
c
c Revision 1.4  2002/12/16  20:21:58  tom
c change FORMAT I to I3
c
CRevision 1.3  2001/06/06 17:59:32  tom
Cadd ntypes 7-9 for steady-state and steady-inhalation oxygen methods
C
c Revision 1.2  1995/05/10  18:18:13  tom
c added RCSHEADER
c
c Revision 1.1  1992/10/12  16:17:30  ty7777
c Initial revision
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   BLDSAVE.FTN
C  Author:       Tom O. Videen
C  Date:         19-Sep-86
C
C  History:
C     Revised 26-Feb-87 by TOV to allow 5 decimal places for
C        syringe weights (instead of 3);
C
C  Intent:
C     This subroutine writes the blood curve points to a disk (.DTA) file.
C
C     Subroutine of BLOOD.FTN
C
C  .DTA file structure:
C  -------------------
C   First 4 lines of file  (written by main program):
C       @01@ Header                       (A4,A70)
C       Column Headings for Blood Curves
C       Column Headings for Blood Curves
C       No. of Blood Curves               (I)
C
C   For each blood curve (on blood curve written by each call to
C   this subroutine):
C       Type of Scan                Scan ID                (I1,1X,A4)
C       Start Time (sec)            Scan Length (sec)      (2F9.0)
C       Peak Bank Pairs (thousands)                        (F10.4)
C       Oxygen Content (ml/ml)      Hematocrit (percent)   (2F10.4)
C       No. of Blood Points                                (I)
C       Sample Time(1)     Counts(1)                       (2F9.0)
C         ...                ...
C       Sample Time(N)     Counts(N)
C
C     Sample Time = seconds after injection;
C     Counts are decay corrected to time of injection and are
C        converted from counts/(g * t sec) to counts/(ml * sec);
C        (using density of whole blood or plasma, as appropriate).
C        (t is normally 10 sec, but may be specified as any time in sec)
C     For SCANTYPE = 1, if the hematocrit is not 0, then the last point
C        in the blood curve is the well counts for plasma taken from the
C        same sample as the whole blood in the next to last point.
C
C  Uses Function:
C     SECS in BLDSECS
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE BLDSAVE(ICURVE)

      INCLUDE 'blood.inc'

      INTEGER*4 ICURVE,I
      REAL      SECS
      CHARACTER*256 RCSHEADER    ! RCS header information
      RCSHEADER = "$Header: /home/npggw/tom/src/blood/RCS/bldsave.f,v 2.0 2004/02/20 16:38:58 tom Exp $"

      N = N-1
      WRITE(DTAFILE,8710) SCANTYPE,SCANID
      WRITE(DTAFILE,8720) SECS(SCANSTART),SECS(SCANLENGTH)
      WRITE(DTAFILE,8730) BANKPAIRS
      WRITE(DTAFILE,8730) OXYCONT,HEMATOCRIT
      WRITE(DTAFILE,8740) N
      DO 100 I=1,N
        WRITE(DTAFILE,8750) TIMESECS(I),CORCNTS(I),DRYWEIGHT(I),
     &     WETWEIGHT(I),TIMEDRAW(I),TIMECNT(I),COUNTS(I),COUNTIME(I)
100   CONTINUE
      WRITE(USEROUT,*)
      WRITE(USEROUT,*) 'Blood Curve Data #',ICURVE,' written to file'
      WRITE(USEROUT,*)

      RETURN

8710  FORMAT(I1,1X,A4)
8720  FORMAT(2F9.0)
8730  FORMAT(2F10.4)
8740  FORMAT(I3)
8750  FORMAT(F8.0,F10.0,F9.5,F9.5,F9.2,F9.2,I10,F6.0)

      END
