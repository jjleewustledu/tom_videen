CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  writerel.f
C
C  This procedure writes a real no. to a specified field on the
C     terminal screen. It first erases the field.
C     LEN must be greater than 1.
C
C  Variables:
C     R = real number;
C     LEN = length of the field (character spaces);
C     ROW,COL = location of the first character of the field;
C
C  Uses:
C     TCONTROL - terminal control;
C
C
C  Subroutine of FIELDS.LIB
C
C  T.O. Videen -- 18-Apr-86
C
C
C  Porting Information:
C     Rewritten for HPUX from Concurrent OS32.
C
C  Author:       M. Todd Gamble
C  Date:         11/28/89
C  Written For:  fields.o
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE WRITEREL(R,LEN,ROW,COL)
C
      INTEGER*4    LEN,ROW,COL,L,L1,USERIN,USEROUT
      REAL         R
      CHARACTER*20 BLANK
      CHARACTER*80 NUMSTR
C
      COMMON /USRIO/ USERIN,USEROUT
      DATA BLANK /'                    '/
C
      IF (LEN.LT.2) THEN
        RETURN
      ELSE IF (LEN.LE.20) THEN
        L = LEN
      ELSE
        L = 20
      END IF
C
      CALL TCONTROL(3,ROW,COL)
      WRITE (USEROUT,8100) BLANK(1:L)
      CALL TCONTROL(3,ROW,COL)
      NUMSTR = ' '
      CALL RELFORMT (R,L,NUMSTR)
      L1 = INANYL (NUMSTR, ' 0')
      L = INANYR (NUMSTR, ' 0')
      WRITE (USEROUT, 8100) NUMSTR(L1:L)
C
      RETURN
8100  FORMAT(A)
      END
