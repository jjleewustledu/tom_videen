C$Header: /home/npggw/tom/src/blood/RCS/bldrepro.f,v 2.0 2004/02/20 16:38:58 tom Exp $
C$Log: bldrepro.f,v $
c Revision 2.0  2004/02/20  16:38:58  tom
c Feb 2004
c
c Revision 1.5  2002/12/16  20:21:58  tom
c change FORMAT I to I3
c
CRevision 1.4  2001/06/06 17:59:32  tom
Cadd ntypes 7-9 for steady-state and steady-inhalation oxygen methods
C
c Revision 1.3  1995/05/10  16:13:20  tom
c default count time differs for different scanners
c
c Revision 1.2  1992/10/14  20:52:38  ty7777
c Release version of blood.
c
c Revision 1.1  1992/10/12  16:17:30  ty7777
c Initial revision
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   BLDREPRO.FTN
C  Author:       Tom O. Videen
C  Date:         03-Oct-86
C
C  History:
C     Revised 26-Feb-87 by TOV to change header spacing on DTA file;
C     Revised 24-Aug-87 by TOV to reprocess every curve as soon as
C        it is read;
C
C  Intent:
C     This subroutine is used for reviewing and reprocessing
C     an existing DTA file.
C
C     The old DTA file is read one curve at a time. The user may:
C        1) alter points in the curve;
C        2) add points to the curve;
C        3) delete points from the curve;
C        4) shift the times of points in the curve;
C
C     For reprocessing, the program computes the counts per second for
C     each blood sample corrected for decay from the time of sampling,
C     the volume of the sample used in the well counter, and the
C     position of the sample in the well counter.
C
C     When adding a point, the user enters syringe dry & wet weights,
C     times drawn & counted, and the counts per n seconds.
C
C  Uses BLOOD Subroutines:
C     BLDSECS:
C       SECS   - convert time from form MIN.SEC to seconds;
C       MINUTES  - convert time from seconds to form MIN.SEC;
C     BLDPARM  - gets scan parameters for each blood curve;
C     BLDLIST  - writes scan parameters to terminal;
C     BLDCURVE - writes blood curve values to the terminal;
C     BLDSAVE  - writes blood curve values to a disk (.DTA) file;
C     BLDALTER - alter a point in a blood curve;
C     BLDADDPT - add a point to a blood curve;
C     BLDDELPT - delete a point from a blood curve;
C     BLDCALC  - calculate corrected counts for all points in curve;
C     BLDSHIFT - shift times of a blood curve;
C
C  Uses TERMINAL.LIB
C     GETINT
C     GETCHAR
C     YESNO
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE BLDREPRO

      INCLUDE 'blood.inc'

      INTEGER*4    NCURVES, ICURVE, I, M
      REAL         STARTSEC, LENGTHSEC, MINUTES, SECS
      CHARACTER*1  CH
      CHARACTER*80 Q, STR, CHARSET
      CHARACTER*256 RCSHEADER    ! RCS header information

      LOGICAL*1    ANS, DONE, ALTERED

      RCSHEADER = "$Header: /home/npggw/tom/src/blood/RCS/bldrepro.f,v 2.0 2004/02/20 16:38:58 tom Exp $"

      SAVE = .TRUE.
      DO 10 I = 1, 6
        SCANNUM(I) = 1
10    CONTINUE

C  Read the header, the column headings, and the number of blood
C  curves from the old DTA file and copy them to the new DTA file.
C  (Note: 8702 and 8703 should correspond to same in BLDNEW.FTN)

8700  FORMAT (A80)
8702  FORMAT (5X, '  Corrected     Syringe Weight      Sample Time    Measured   Per')
8703  FORMAT (5X, 'Sec   Counts     Dry      Wet      Drawn  Counted    Counts   Sec')
8705  FORMAT(I3)

      READ  (OLDDTA, 8700) STR
      WRITE (DTAFILE,8700) STR
      READ  (OLDDTA, 8700) STR
      READ  (OLDDTA, 8700) STR
      WRITE (DTAFILE,8702)
      WRITE (DTAFILE,8703)
      READ  (OLDDTA, 8705) NCURVES
      WRITE (DTAFILE,8705) NCURVES

C  Select option for varying well count times

      VARYTIME = .FALSE.
      IF (DEFTIME.EQ.12) THEN
        Q = 'Were well counter periods other than 12 seconds used'
      ELSE
        Q = 'Were well counter periods other than 10 seconds used'
      ENDIF
      CALL YESNO (Q, VARYTIME)

C  LOOP for each blood curve

      DO 500 ICURVE = 1, NCURVES

C  Read start time, length of scan, bank pairs, oxygen content
C  and hematocrit from the old DTA file

8730    FORMAT (4X, I1, ' = ', A)
        READ (OLDDTA, *) SCANTYPE, SCANID

C    Set ISOTOPE to value corresponding to the scan type

        IF (SCANTYPE .EQ. 4) THEN
          ISOTOPE = 3
        ELSE IF (SCANTYPE .EQ. 5) THEN
          ISOTOPE = 5
        ELSE IF (SCANTYPE .NE. 6) THEN
          ISOTOPE = 1
        ELSE
          WRITE (USEROUT, *)
          DO 30  I = 1, 6
            WRITE (USEROUT, 8730) I, NUCLIDE(I)
30        CONTINUE
          WRITE (USEROUT, *)
          Q = 'Isotope Number'
          CALL GETINT (Q, ISOTOPE, 1, 6)
          WRITE (USEROUT, *) 'Isotope = ', NUCLIDE(ISOTOPE)
        END IF

        READ (OLDDTA, *) STARTSEC, LENGTHSEC
        SCANSTART  = MINUTES(STARTSEC)
        SCANLENGTH = MINUTES(LENGTHSEC)
        READ (OLDDTA, *) BANKPAIRS
        READ (OLDDTA, *) OXYCONT, HEMATOCRIT
150     CALL BLDLIST
        ANS = .FALSE.
        Q   = 'Redo these values'
        CALL YESNO (Q, ANS)
        IF (ANS) THEN
          CALL BLDPARM (ICURVE)
          GO TO 150
        END IF

C  Check BLDSAVE.FTN 

        READ (OLDDTA, *) N
        DO 200 I = 1, N
          READ (OLDDTA, *) TIMESECS(I), CORCNTS(I), DRYWEIGHT(I), WETWEIGHT(I),
     &      TIMEDRAW(I), TIMECNT(I), COUNTS(I), COUNTIME(I)
200     CONTINUE

C  Initialize all array values which were not read

        N = N + 1
        DO 300 I = N, 200
          DRYWEIGHT(I) = 0.
          WETWEIGHT(I) = 0.
          TIMEDRAW(I)  = 0.
          TIMESECS(I)  = 0.
          TIMECNT(I)   = 0.
          COUNTIME(I)  = 10.
          COUNTS(I)    = 0
          CORCNTS(I)   = 0.
300     CONTINUE
        DONE    = .TRUE.
        ALTERED = .FALSE.

C  Reprocess the blood curve before displaying it

        CALL BLDCALC

400     CALL BLDCURVE (DONE)

        IF ((SCANTYPE.EQ.1 .OR. SCANTYPE.EQ.7) .AND. HEMATOCRIT.NE.0.0 .AND. N.GT.2) THEN
          WRITE (USEROUT, *) 'The last point should be the well counts for plasma.'
          IF (TIMEDRAW(N - 1) .NE. TIMEDRAW(N - 2)) THEN
            WRITE(USEROUT,*) 'WARNING: The last 2 points should be whole-blood & plasma counts'
            WRITE(USEROUT,*) '         sample at the same time!'
          END IF
          WRITE (USEROUT, *)
        END IF

        WRITE (USEROUT, *) 'Curve is shifted by ',NINT(TIMESECS(1)-SECS(TIMEDRAW(1))),'seconds.'

        Q ='Next, Shift, Change point, Add point, Delete point (N,S,C,A,D)'
        CHARSET = 'NSCADnscad'
        CH = 'N'
        CALL GETCHAR (Q, CH, CHARSET)
        IF (CH .EQ. 'C' .OR. CH .EQ. 'c') THEN
          Q = 'Change point #'
          M = N - 1
          CALL GETINT (Q, M, 1, N - 1)
          CALL BLDALTER (M)
          ALTERED = .TRUE.
          GO TO 400
        ELSE IF (CH .EQ. 'S' .OR. CH .EQ. 's') THEN
          CALL BLDSHIFT
          GO TO 400
        ELSE IF (CH .EQ. 'A' .OR. CH .EQ. 'a') THEN
          Q = 'Add a point BEFORE point #'
          M = N
          CALL GETINT (Q, M, 1, N)
          CALL BLDADDPT (M)
          ALTERED = .TRUE.
          GO TO 400
        ELSE IF (CH .EQ. 'D' .OR. CH .EQ. 'd') THEN
          Q = 'Delete point #'
          M = N - 1
          CALL GETINT (Q, M, 1, M)
          CALL BLDDELPT (M)
          GO TO 400
        END IF

C  If any points have been added or altered, reprocess all
C  points before saving the curve and be sure all points
C  are shifted by the appropriate amount.

        IF (ALTERED) THEN
          CALL BLDCALC
          CALL BLDSHIFT
        END IF
        CALL BLDSAVE (ICURVE)
        SCANNUM(SCANTYPE) = SCANNUM(SCANTYPE) + 1
500   CONTINUE

      RETURN
      END
