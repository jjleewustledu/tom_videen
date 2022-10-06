c $Log: piescale.f,v $
c Revision 1.2  1992/11/02  16:02:53  tom
c Added RCS identifies
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   PIESCALE.FTN
C
C  Author:       Tom O. Videen
C  Date:         27-Mar-89
C  Written For:  METPROC and PETPROC
C
C  Intent:
C     Compute relative efficiency of PETT for pie scan relative
C     to current scan.
C
C     EFACTOR = NORM1 with decay/NORM2
C
C     Decay is that of GE68 (288 days) from DATE1 to DATE2.
C
C  Variables Passed:
C     NORM1 = norm mean counts/bin/min for pie
C     NORM2 = norm mean counts/bin/min for current scan
C     DATE1 = date of pie scan
C     DATE2 = date of pie current scan
C     EFACTOR = scaling factor (>1 means PETT more efficient for pie)
C
C  Called by:  METPROC, PETPROC (alter $INCLUDE appropriately)
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE PIESCALE(NORM1,NORM2,DATE1,DATE2,EFACTOR)
C
      INCLUDE 'metproc.inc'
C
      REAL*4       NORM1, NORM2, EFACTOR, HALFLIFE, DNORM1, ATIME
      INTEGER*4    PIEDAY, SCANDAY, YEAR, MONTH, DAY, DAYS(12)
      CHARACTER*8  DATE1, DATE2
      CHARACTER*256 RCSHEADER
C
			RCSHEADER = "$Header: /home/petsun2/tom/src/fastproc/src/RCS/piescale.f,v 1.2 1992/11/02 16:02:53 tom Exp $"
      HALFLIFE = 288.              ! halflife in days
      DAYS(1)  = 0
      DAYS(2)  = 31                !  JAN
      DAYS(3)  = DAYS(2) + 28      !  FEB
      DAYS(4)  = DAYS(3) + 31      !  MAR
      DAYS(5)  = DAYS(4) + 30      !  APR
      DAYS(6)  = DAYS(5) + 31      !  MAY
      DAYS(7)  = DAYS(6) + 30      !  JUN
      DAYS(8)  = DAYS(7) + 31      !  JUL
      DAYS(9)  = DAYS(8) + 31      !  AUG
      DAYS(10) = DAYS(9) + 30      !  SEP
      DAYS(11) = DAYS(10)+ 31      !  OCT
      DAYS(12) = DAYS(11)+ 30      !  NOV
C
C  Converts dates to the number of days from 01-JAN-81
C  Leap Year corrections (FEB 29: 1984, 1988, 1992, 1996)
C
8100  FORMAT(I2)
      READ(DATE1(1:2),8100) MONTH
      READ(DATE1(4:5),8100) DAY
      READ(DATE1(7:8),8100) YEAR
      PIEDAY = (YEAR-81)*365 + DAYS(MONTH) + DAY
      IF (YEAR.GT.84 .OR. (YEAR.EQ.84 .AND. MONTH.GT.3))
     &  PIEDAY = PIEDAY + 1
      IF (YEAR.GT.88 .OR. (YEAR.EQ.88 .AND. MONTH.GT.3))
     &  PIEDAY = PIEDAY + 1
      IF (YEAR.GT.92 .OR. (YEAR.EQ.92 .AND. MONTH.GT.3))
     &  PIEDAY = PIEDAY + 1
      IF (YEAR.GT.96 .OR. (YEAR.EQ.96 .AND. MONTH.GT.3))
     &  PIEDAY = PIEDAY + 1
C
      READ(DATE2(1:2),8100) MONTH
      READ(DATE2(4:5),8100) DAY
      READ(DATE2(7:8),8100) YEAR
      SCANDAY = (YEAR-81)*365 + DAYS(MONTH) + DAY
      IF (YEAR.GT.84 .OR. (YEAR.EQ.84 .AND. MONTH.GT.3))
     &  SCANDAY = SCANDAY + 1
      IF (YEAR.GT.88 .OR. (YEAR.EQ.88 .AND. MONTH.GT.3))
     &  SCANDAY = SCANDAY + 1
      IF (YEAR.GT.92 .OR. (YEAR.EQ.92 .AND. MONTH.GT.3))
     &  SCANDAY = SCANDAY + 1
      IF (YEAR.GT.96 .OR. (YEAR.EQ.96 .AND. MONTH.GT.3))
     &  SCANDAY = SCANDAY + 1
C
C  Add decay to NORM1 using GE68 halflife
C
      ATIME = FLOAT(SCANDAY - PIEDAY)
      IF (SCANDAY.NE.0 .AND. PIEDAY.NE.0) THEN
        DNORM1 = NORM1*EXP(-ATIME*(ALOG(2.0)/HALFLIFE))
        EFACTOR = DNORM1/NORM2
      ELSE
        EFACTOR = 0.0
      END IF
C
C  Summary to user
C
      WRITE(USEROUT,8200) 'PIE ',NORM1, DATE1
      WRITE(USEROUT,8200) 'SCAN',NORM2, DATE2
      WRITE(USEROUT,8300) NINT(ATIME)
8200  FORMAT(1X,'NORM Mean Counts/Bin/Min for ',A4,' = ',F6.2,5X
     &   'Date = ',A8)
8300  FORMAT(1X,'Days since PIE = ',I)
C
      RETURN
      END
