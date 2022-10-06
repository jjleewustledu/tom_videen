CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  LOGFIELD.FTN
C
C  This procedure gets a "Y" or "N" response from the user.
C     The string is read from a field on the terminal screen
C     at the specified row and column.
C
C     If the user enters a carriage return, the logical variable
C     is returned unchanged.
C     If "Y" or "y" is entered, LVAR = .TRUE.
C     If "N" or "n" is entered, LVAR = .FALSE.
C     Any other character sounds the bell and issues an error
C        message.
C     If WARNLINE <= 0, a bell sounds and no warning is written.
C     The cursor is turned on while the string is being entered,
C     but is left in its initial ON/OFF state.
C
C  Modified 06-Feb-86 to allow termination of input with up or down arrows.
C     Returns a variable NEXT.
C
C  Variables:
C     LVAR = logical variable;
C     ROW = row where the field begins;
C     COL = column where the field begins;
C     NEXT set true or false upon exiting:
C        true ==> subroutine terminated with RETURN or DOWN ARROW;
C        false==> subroutine terminated with UP ARROW;
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
C  T.O. Videen -- 07-Feb-86
C
C
C  Porting Information:
C     Rewritten for HPUX from Concurrent OS32.
C
C  Author:       M. Todd Gamble
C  Date:         11/28/89
C  Written For:  fields.o
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE LOGFIELD(LVAR,ROW,COL,WARNLINE,NEXT)

      INTEGER*4 TLENGTH(120),ROWLOC,COLLOC,USERIN,USEROUT
      CHARACTER TPARMS(120)*40
      LOGICAL*1 CURSOR
      COMMON   /TPARAMS/ TPARMS,TLENGTH,ROWLOC,COLLOC,CURSOR
      COMMON   /USRIO/   USERIN,USEROUT

      INTEGER*4    ROW,COL,POSCUR,WARNLINE,L,PBLK(6),IOWRITE,ALENGTH

      CHARACTER*1  CH,BELL,SPACE,RETURN,BACKSP,LCHAR,
     2             UPARROW1,DNARROW1,UPARROWL,DNARROWL
      CHARACTER*80 ERRMSG
      LOGICAL*1    NEXT,CFLAG,LVAR,ERROR

      DATA  IOWRITE /Z'00000029'/
      DATA  POSCUR  /3/
      BELL   = CHAR(7)
      BACKSP = CHAR(8)
      RETURN = CHAR(13)
      SPACE  = CHAR(32)
      ERRMSG = '*** ERROR -- enter "Y" or "N" ***'//BELL
      ERROR  = .FALSE.
      ALENGTH = TLENGTH(5)
      UPARROW1 = TPARMS(5)(1:1)
      DNARROW1 = TPARMS(6)(1:1)
      IF (ALENGTH.GT.1) THEN
        UPARROWL = TPARMS(5)(ALENGTH:ALENGTH)
        DNARROWL = TPARMS(6)(ALENGTH:ALENGTH)
      END IF

C  Write the "Y" or "N" in the field and leave the
C  cursor over the character (backspace).

      CALL TCONTROL(POSCUR,ROW,COL)
      IF (LVAR) THEN
        LCHAR = 'Y'
      ELSE
        LCHAR = 'N'
      END IF
      CALL SYSIO(PBLK,IOWRITE,USEROUT,LCHAR,1,0)
      CALL PUTCH(BACKSP)
      L = 1

      IF (.NOT.CURSOR) THEN
        CALL CURSON
        CFLAG = .TRUE.
      ELSE
        CFLAG = .FALSE.
      END IF

C  Read a character from the terminal.

100   CALL GETCH(CH)

C  Carriage Return --> exit with the current string.

      IF (CH.EQ.RETURN) THEN
        NEXT = .TRUE.
        GO TO 9000
      ELSE IF (CH.EQ.UPARROW1) THEN
        IF (ALENGTH.EQ.1) THEN
          NEXT = .FALSE.
          GO TO 9000
        ELSE
          DO 110 I=2,ALENGTH
            CALL GETCH(CH)
110       CONTINUE
          IF (CH.EQ.UPARROWL) THEN
            NEXT = .FALSE.
            GO TO 9000
          ELSE IF (CH.EQ.DNARROWL) THEN
            NEXT = .TRUE.
            GO TO 9000
          ELSE
            CALL PUTCH(BELL)
            CALL TCONTROL(POSCUR,ROW,L+COL)
          END IF
        END IF
      ELSE IF (CH.EQ.DNARROW1) THEN
        NEXT = .TRUE.
        GO TO 9000

C  Backspace --> delete the last character from the string unless
C     there are no more characters in the string (in which case,
C     sound the bell).

      ELSE IF (CH.EQ.BACKSP) THEN
        IF (L.EQ.2) THEN
          L = 1
          CALL PUTCH(SPACE)
          CALL PUTCH(BACKSP)
        ELSE
          CALL PUTCH(BELL)
          IF (COL.GT.1) THEN
            CALL PUTCH(SPACE)
          ELSE
            CALL TCONTROL(POSCUR,ROW,COL)
          END IF
        END IF
C
C  Unprintable Character --> sound the bell.
C
      ELSE IF ((ICHAR(CH).LT.32).OR.(ICHAR(CH).GT.126)) THEN
        CALL PUTCH(BELL)
C
C  Printable Character --> add the character to the string,
C     unless the field is full (in which case, sound the bell).
C
      ELSE IF (L.EQ.2) THEN
        CALL PUTCH(BELL)
        CALL PUTCH(BACKSP)
        CALL PUTCH(SPACE)
        CALL PUTCH(BACKSP)
      ELSE
        LCHAR = CH
        L = 2
      END IF
C
      GO TO 100
C
9000  IF ((LCHAR.EQ.'Y').OR.(LCHAR.EQ.'y')) THEN
        LVAR = .TRUE.
      ELSE IF ((LCHAR.EQ.'N').OR.(LCHAR.EQ.'n')) THEN
        LVAR = .FALSE.
      ELSE
        IF (WARNLINE.GT.0) THEN
          ERROR = .TRUE.
          CALL WRTFIELD(ERRMSG,80,WARNLINE,1)
        ELSE
          WRITE(USEROUT,*) BELL
        END IF
        CALL TCONTROL(POSCUR,ROW,COL)
        CALL PUTCH(SPACE)
        CALL PUTCH(BACKSP)
        L = 1
        GO TO 100
      END IF
C
      IF (ERROR) CALL ERSFIELD(80,WARNLINE,1)
      IF (CFLAG) CALL CURSOFF
C
      RETURN
      END
