C$Header: /home/npggw/tom/src/blood/RCS/bldnew.f,v 2.0 2004/02/20 16:38:58 tom Exp $
C$Log: bldnew.f,v $
c Revision 2.0  2004/02/20  16:38:58  tom
c Feb 2004
c
c Revision 1.6  2002/12/16  20:21:58  tom
c change FORMAT I to I3
c
CRevision 1.5  2001/06/06 17:59:32  tom
Cadd ntypes 7-9 for steady-state and steady-inhalation oxygen methods
C
c Revision 1.4  1995/05/10  16:13:20  tom
c default count time differs for different scanners
c
c Revision 1.3  1993/12/17  21:34:24  tom
c changed max NCURVES to 50
c
c Revision 1.2  1992/10/13  19:07:24  ty7777
c blood working version.
c
c Revision 1.1  1992/10/12  16:17:30  ty7777
c Initial revision
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   bldnew.f
C  Author:       Tom O. Videen
C  Date:         03-Oct-86
C
C  History:
C     Revised 26-Feb-87 by TOV to change header spacing on DTA file;
C     Revised 19-Aug-87 by TOV so that SAVE=FALSE really deletes the
C        DTA file instead of saving one with 0 records.
C			Revised 17-Dec-93 TOV changing max NCURVES from 9 to 50.
C
C  Intent:
C     This subroutine is called for entering new blood curves.
C
C     The program computes the counts per second for each blood sample
C     corrected for decay from the time of sampling, the volume of the
C     sample used in the well counter, and the position of the sample
C     in the well counter.
C     The user enters syringe dry & wet weights, times drawn & counted,
C     and the counts per n seconds, as well as pertinent data concerning
C     the PET scan.
C
C     If SAVE=FALSE then no .DTA file will be written, but corrected
C     counts will be displayed.   The allocated DTA file will be deleted
C     when the subroutine exits.
C
C  Uses BLOOD Subroutines:
C     BLDPARM  - gets scan parameters for each blood curve;
C     BLDLIST  - writes scan parameters to terminal;
C     BLDENTER - gets values of each point in blood curve and
C        computes corrected counts;
C     BLDCURVE - writes blood curve values to the terminal;
C     BLDSAVE  - writes blood curve values to a disk (.DTA) file;
C     BLDALTER - alter a point in a blood curve;
C     BLDADDPT - add a point to a blood curve;
C     BLDDELPT - delete a point from a blood curve;
C     BLDSHIFT - shift times of a blood curve;
C
C  Uses TERMINAL.LIB
C     GETINT
C     GTSTRING
C     YESNO
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE BLDNEW

      INCLUDE 'blood.inc'

      INTEGER*4    NCURVES,ICURVE,I,M
      CHARACTER*1  CH
      CHARACTER*80 HEAD,Q,CHARSET
      CHARACTER*256 RCSHEADER    ! RCS header information

      LOGICAL*1    ANS,DONE,A

C  Initializations

      RCSHEADER = "$Header: /home/npggw/tom/src/blood/RCS/bldnew.f,v 2.0 2004/02/20 16:38:58 tom Exp $"
      HEAD = ' '
      NCURVES = 1
      ISOTOPE = 1
      SCANTYPE = 1
      DO 10 I=1,6
        SCANNUM(I) = 1
10    CONTINUE

C  If counts are saved, write the header, the column headings,
C  and the number of blood curves to the .DTA file

      SAVE = .TRUE.
      Q = 'Save corrected counts in a .DTA file'
      CALL YESNO(Q,SAVE)
      IF (SAVE) THEN
        Q = 'Header'
        CALL GTSTRING(Q,HEAD)
        WRITE(DTAFILE,8701) HEAD
        WRITE(DTAFILE,8702)
        WRITE(DTAFILE,8703)
        Q = 'Number of Blood Curves Obtained'
        CALL GETINT(Q,NCURVES,1,50)
        WRITE(DTAFILE,8704) NCURVES
      ELSE
        CLOSE(DTAFILE,STATUS='DELETE')
      END IF

8701  FORMAT ('@01@ ', A69)
8702  FORMAT (5X, '  Corrected     Syringe Weight      Sample Time    Measured   Per')
8703  FORMAT (5X, 'Sec   Counts     Dry      Wet      Drawn  Counted    Counts   Sec')
8704  FORMAT (I3)

C  Select option for varying well count times

      VARYTIME = .FALSE.
      IF (SCANNER.EQ.3) THEN
         Q = 'Were well counter periods other than 12 seconds used'
      ELSE
         Q = 'Were well counter periods other than 10 seconds used'
      ENDIF
      CALL YESNO(Q,VARYTIME)

C  LOOP for each blood curve

      N = 200
      DO 500 ICURVE = 1, NCURVES
        DO 100 I = 1, N
          DRYWEIGHT (I) = 0.0
          WETWEIGHT (I) = 0.0
          TIMEDRAW (I) = 0.0
          TIMESECS (I) = 0.0
          TIMECNT (I) = 0.0
          COUNTIME (I) = FLOAT(DEFTIME)
          COUNTS (I) = 0
          CORCNTS (I) = 0.0
100     CONTINUE

150     CALL BLDPARM (ICURVE)
        CALL BLDLIST
        ANS = .FALSE.
        Q = 'Redo these values'
        CALL YESNO(Q,ANS)
        IF (ANS) GO TO 150

        N = 1
        WRITE(USEROUT,*)
        WRITE(USEROUT,*)'  BLOOD POINT ENTRY'
200     DONE = .FALSE.
        CALL BLDENTER(DONE)
300     CALL BLDCURVE(DONE)
        IF (.NOT.DONE) THEN
          ANS = .FALSE.
          Q = 'Redo these values'
          CALL YESNO(Q,ANS)
          IF (.NOT.ANS .AND. SAVE) N = N+1
          GO TO 200
        END IF

C   N-1 is index to last data point entered

        IF (.NOT.SAVE) STOP
        ANS = .FALSE.
        IF (SCANTYPE.EQ.1 .AND. HEMATOCRIT.NE.0.0 .AND. N.GT.2) THEN
          WRITE(USEROUT,*) 'The last point should be the well counts for plasma.'
          IF (TIMEDRAW(N-1) .NE. TIMEDRAW(N-2)) THEN
            WRITE(USEROUT,*) 'WARNING: The last 2 points should be whole-blood & plasma counts'
            WRITE(USEROUT,*) '         sample at the same time!'
          END IF
          WRITE(USEROUT,*)
        END IF

        Q ='End, Resume, Change a point, Add a point, or Delete a point (E,R,C,A,D)'
        CHARSET = 'ERCADercad'
        CH = 'R'
        CALL GETCHAR(Q,CH,CHARSET)
        IF (CH.EQ.'R' .OR. CH.EQ.'r') THEN
          Q = 'Continue from point #'
          CALL GETINT(Q,N,1,N)
          GO TO 200
        ELSE IF (CH.EQ.'C' .OR. CH.EQ.'c') THEN
          Q = 'Change point #'
          M = N-1
          CALL GETINT(Q,M,1,N-1)
          CALL BLDALTER(M)
          GO TO 300
        ELSE IF (CH.EQ.'A' .OR. CH.EQ.'a') THEN
          Q = 'Add a point BEFORE point #'
          M = N
          CALL GETINT(Q,M,1,N)
          CALL BLDADDPT(M)
          GO TO 300
        ELSE IF (CH.EQ.'D' .OR. CH.EQ.'d') THEN
          Q = 'Delete point #'
          M = N-1
          CALL GETINT(Q,M,1,M)
          CALL BLDDELPT(M)
          GO TO 300
        END IF

C    Points sampled from plasma should have densities adjusted 

        IF ((SCANTYPE.EQ.1 .OR. SCANTYPE.EQ.7) .AND. HEMATOCRIT.NE.0.0) THEN
          CORCNTS(N-1) = CORCNTS(N-1)*PLASMADN/BLOODDEN
        ELSE IF (SCANTYPE.EQ.8) THEN
          DO 400 I=1,N-1
            CORCNTS(I) = CORCNTS(I)*PLASMADN/BLOODDEN
400       CONTINUE
        ENDIF

        Q = 'Shift the points in this curve'
        A = .FALSE.
        CALL YESNO(Q,A)
        IF (A) CALL BLDSHIFT
        CALL BLDSAVE(ICURVE)
        SCANNUM(SCANTYPE) = SCANNUM(SCANTYPE) + 1
500   CONTINUE

      RETURN
      END
