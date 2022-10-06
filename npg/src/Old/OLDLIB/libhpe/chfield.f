CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  CHFIELD.FTN
C
C  This procedure gets a character from a specified field of the
C     user's terminal.  The field is erased after a character is
C     entered.  If the character does not belong to the list
C     of characters CHARSET, a warning is written on the specified
C     line.
C     If WARNLINE <= 0, a bell sounds and no warning is written.
C     The subroutine returns immediately and does not require a
C     carriage return.
C
C  Uses Common Block  /USRIO/ USERIN,USEROUT
C     USERIN - logical unit for terminal input;
C     USEROUT - logical unit for terminal output;
C
C  Variables:
C     CH = character entered from terminal;
C     CHARSET = string containing all allowed characters;
C     ROW,LOC = location of field (row and column);
C     WARNLINE = line number (row) in which to write warnings;
C
C  Uses Subroutines:
C     WRTFIELD
C     ERSFIELD
C     TCONTROL
C     CURSON
C     CURSOFF
C     SYSIO
C
C  Subroutine of FIELDS.LIB
C
C  T.O. Videen -- 14-Feb-86
C
C
C  Porting Information:
C     Rewritten for HPUX from Concurrent OS32.
C
C  Author:       M. Todd Gamble
C  Date:         11/28/89
C  Written For:  fields.o
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE CHFIELD(CH,CHARSET,ROW,COL,WARNLINE)
C
      INTEGER*4    ROW,COL,WARNLINE,WARNCOL,LENWARN,
     2             IOREAD,IOWRITE,POSCUR,K,L
      CHARACTER*1  CH,BLANK,BELL
      CHARACTER*80 CHARSET,WARNING
      LOGICAL*1    WARN,CFLAG
C
      INTEGER*4 TLENGTH(120),ROWLOC,COLLOC,USERIN,USEROUT
      CHARACTER TPARMS(120)*40
      LOGICAL*1 CURSOR
      COMMON   /TPARAMS/ TPARMS,TLENGTH,ROWLOC,COLLOC,CURSOR
      COMMON   /USRIO/   USERIN,USEROUT
C
      DATA IOREAD   /88/
      DATA IOWRITE  /41/
      DATA POSCUR   /3/
      DATA WARNCOL  /1/
C
      BELL  = CHAR(7)
      BLANK = ' '
      WARN = .FALSE.
      L = INANYR(CHARSET,' ')
      WARNING = '*** ERROR -- allowed characters are '//CHARSET(1:L)//
     2          ' ***'//BELL
      LENWARN = INANYR(WARNING,' ')
C
C  Position cursor
C
100   CALL TCONTROL(POSCUR,ROW,COL)
      IF (.NOT.CURSOR) THEN
        CALL CURSON
        CFLAG = .TRUE.
      ELSE
        CFLAG = .FALSE.
      END IF

      CALL GETCH(CH)
      CALL TCONTROL(POSCUR,ROW,COL)
      CALL PUTCH(BLANK)
C
C  Check that input character belongs to allowed set
C
      K = IANYL(CH,CHARSET)
      IF (K.EQ.0) THEN
        IF (WARNLINE.GT.0) THEN
          WARN = .TRUE.
          L = INANYR(WARNING,' ')
          CALL WRTFIELD(WARNING,LENWARN,WARNLINE,WARNCOL)
        ELSE
          IF (ROW.GE.23) CALL TCONTROL(POSCUR,22,1)
          WRITE(USEROUT,*) BELL
        END IF
        GO TO 100
      END IF
C
C  Erase warning
C
      IF (WARN) CALL ERSFIELD(LENWARN,WARNLINE,WARNCOL)
C
C  If the cursor was off when this routine was entered, turn it off
C
      IF (CFLAG) CALL CURSOFF
C
      RETURN
      END
