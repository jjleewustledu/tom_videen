CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  QUERY.FTN
C
C  This procedure gets a "Y" or "N" response from the user.
C     The string is read from a field on the terminal screen
C     at the specified row and column.
C
C     If the user enters a carriage return, LVAR = .FALSE.
C     If "Y" or "y" is entered, LVAR = .TRUE.
C     If "N" or "n" is entered, LVAR = .FALSE.
C     Any other character sounds the bell and waits for a legal
C        input.
C     The cursor is turned on while the string is being entered,
C     but is left in its initial ON/OFF state.
C
C  This procedure differs from LOGFIELD in that:
C     1) it responds to a single keystroke (does not need RETURN);
C     2) the default value is always .FALSE.
C     3) a prompt is written;
C
C  Variables:
C     LVAR = logical variable;
C     ROW = row where the field begins;
C     COL = column where the field begins;
C
C  Uses:
C     WRTFIELD
C     ERSFIELD
C     TCONTROL
C     CURSON
C     CURSOFF
C     PUTCH
C     GETCH
C     SYSIO
C
C  Subroutine of FIELDS.LIB
C
C  T.O. Videen -- 11-Feb-86
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      SUBROUTINE QUERY(Q,LVAR,ROW,COL)
C
      INTEGER*4 TLENGTH(120),ROWLOC,COLLOC
      CHARACTER TPARMS(120)*40
      LOGICAL*1 CURSOR
      COMMON   /TPARAMS/ TPARMS,TLENGTH,ROWLOC,COLLOC,CURSOR
C
      INTEGER*4    ROW,COL,POSCUR,L
      CHARACTER*1  CH,BELL,RETURN,BACKSP
      CHARACTER*80 Q
      LOGICAL*1    LVAR,CFLAG,EXIT
C
      DATA  POSCUR  /3/
      BELL   = CHAR(7)
      BACKSP = CHAR(8)
      RETURN = CHAR(13)
C
      IF (.NOT.CURSOR) THEN
        CALL CURSON
        CFLAG = .TRUE.
      ELSE
        CFLAG = .FALSE.
      END IF
C
C  Write the prompt
C
      L = INANYR(Q,' ')
      CALL WRTFIELD (Q,L,ROW,COL)
C
C  Read a character from the terminal.
C
100   EXIT = .TRUE.
      CALL TCONTROL(POSCUR,ROW,COL+L+2)
      CH = 'N'
      CALL PUTCH(CH)
      CALL TCONTROL(POSCUR,ROW,COL+L+2)
      CALL GETCH(CH)
C
C  Carriage Return --> exit with LVAR = .FALSE.
C
      IF (CH.EQ.RETURN) THEN
        LVAR = .FALSE.
C
C  'Y' ---> exit with LVAR = .TRUE.
C
      ELSE IF (CH.EQ.'Y' .OR. CH.EQ.'y') THEN
        LVAR = .TRUE.
C
C  'N' ---> exit with LVAR = .FALSE.
C
      ELSE IF (CH.EQ.'N' .OR. CH.EQ.'n') THEN
        LVAR = .FALSE.
C
C  Anything else  --> sound bell and get another character
C
      ELSE
        EXIT = .FALSE.
        CALL PUTCH(BELL)
      END IF
C
      IF (.NOT.EXIT) GO TO 100
C
      IF (CFLAG) CALL CURSOFF
      RETURN
      END
