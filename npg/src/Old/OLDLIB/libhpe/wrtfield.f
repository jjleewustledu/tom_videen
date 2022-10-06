CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  WRTFIELD.FTN
C
C  This procedure writes LLEN characters of the string STR to
C     a field on the terminal screen beginning at the
C     specified row and column.
C     The cursor is turned off during all writes.
C     The cursor is left at the end of the string in its
C     initial ON/OFF state.
C
C  Variables:
C     STR = character string;
C     LLEN = length of the field (no. of characters);
C     ROW = row where the field begins;
C     COL = column where the field begins;
C
C  Uses:
C     TCONTROL
C     CURSON
C     CURSOFF
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
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE WRTFIELD (STR, LLEN, ROW, COL)

      INTEGER*4 TLENGTH(120), ROWLOC, COLLOC, USERIN, USEROUT
      CHARACTER TPARMS (120)*40
      LOGICAL*1 CURSOR
      COMMON   /TPARAMS/ TPARMS, TLENGTH, ROWLOC, COLLOC, CURSOR
      COMMON   /USRIO/  USERIN, USEROUT

      INTEGER*4    LLEN, ROW, COL, POSCUR, LAST, PBLK (6), IOWRITE

      CHARACTER*80 STR
      LOGICAL*1    CFLAG

      DATA  POSCUR   /3/
      DATA  IOWRITE  /56/
C
      PBLK(6)	= 0
C
C  Write the current value of STR in the field and leave the
C  cursor at the end of the last character (if any).
C
      IF (CURSOR) THEN
	CALL CURSOFF
	CFLAG	= .TRUE.
      ELSE
	CFLAG	= .FALSE.
      END IF
      CALL TCONTROL (POSCUR, ROW, COL)
      CALL SYSIO (PBLK, IOWRITE, USEROUT, STR, LLEN, 0)
      LAST	= INANYR (STR (1:LLEN), ' ')
      IF (LAST .LT. LLEN) CALL TCONTROL (POSCUR, ROW, COL + LAST)
      IF (CFLAG) CALL CURSON
C
      RETURN
      END
