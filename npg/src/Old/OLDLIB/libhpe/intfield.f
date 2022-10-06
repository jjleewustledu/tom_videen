CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   INTFIELD.FTN
C
C  Author:       Tom O. Videen
C  Date:         12-Feb-86
C  Written For:  FIELDS.LIB library of terminal fields I/O
C
C  Intent:
C     This procedure gets an integer from the user.
C     The integer is read from a specified field on the terminal screen.
C     The current value of the integer is written to the field.
C     If the user enters only RETURN, the current value of the
C     integer is returned (if it is in the allowed range).
C     The value of the integer entered is checked with the
C     input variables MINV and MAXV (MINV<=I<=MAXV).
C     If WARNLINE <= 0, a bell sounds and no warning is written.
C
C  Modified 06-Feb-86 to allow termination of input with up or down arrows.
C     Returns a variable NEXT.
C  Modified 10-Apr-87 to leave the cursor at the beginning of the field
C     at the start.  Return leaves the value the same.  Entering any other
C     character erases the entire field and enters the character at the
C     beginning of the field.
C
C  Modified 18-Sep-91 by Tom Yang to write default, wrase control characters
C      for backspace and reture when porting to Sun workstation X window.
C
C  Variables:
C     I = integer value;
C     MINV = minimum allowed value;
C     MAXV = maximum allowed value;
C     LLEN = length of the field (character spaces);
C     ROW,COL = location of the first character of the field;
C     WARNLINE = line number (row) in which to write warnings;
C     NEXT set true or false upon exiting:
C        true ==> subroutine terminated with RETURN or DOWN ARROW;
C        false==> subroutine terminated with UP ARROW;
C
C  Uses:
C     ERSFIELD
C     WRTFIELD
C
C  Subroutine of FIELDS.LIB
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE INTFIELD (I, MINV, MAXV, LENGTH, ROW,
     2				COL, WARNLINE, NEXT)
C
      INTEGER*4    I, MINV, MAXV, LLEN, ROW, COL, WARNLINE, POSCUR, MINLEN, MAXLEN,
     2             LENGTH, ALENGTH, J, K, L1, L2
      CHARACTER*1  CH, BELL, SPACE, RETURN, BACKSP, UPARROW1, DNARROW1,
     2             UPARROWL, DNARROWL
      CHARACTER*12 NUMSTR, MINNUM, MAXNUM, ITOC
      CHARACTER*80 STR, ERRMSG, BLANK
      LOGICAL*1    NEXT, WARNFLAG, CFLAG, FIRST
C
      INTEGER*4 TLENGTH(120), ROWLOC, COLLOC, USERIN, USEROUT
      CHARACTER TPARMS(120)*40
      LOGICAL*1 CURSOR
C
      COMMON  /USRIO/   USERIN, USEROUT
      COMMON  /TPARAMS/ TPARMS, TLENGTH, ROWLOC, COLLOC, CURSOR
C
      DATA POSCUR /3/
      DATA BLANK  /' '/
C
      BELL	= CHAR (7)
      BACKSP	= CHAR (8)
      RETURN	= CHAR (13)
      SPACE	= CHAR (32)
      ALENGTH	= TLENGTH (5)
      UPARROW1	= TPARMS (5)(1:1)
      DNARROW1	= TPARMS (6)(1:1)
      IF (ALENGTH .GT. 1) THEN
	UPARROWL	= TPARMS (5) (ALENGTH:ALENGTH)
	DNARROWL	= TPARMS (6) (ALENGTH:ALENGTH)
      END IF
      WARNFLAG	= .FALSE.
      FIRST	= .TRUE.
      NUMSTR	= ITOC (I, K)
      MINNUM	= ITOC (MINV, MINLEN)
      MAXNUM	= ITOC (MAXV, MAXLEN)
      WRITE (ERRMSG, *), 
     1	'* ERROR - enter an integer between',
     2	MINV, ' and', MAXV
C
C  Do not accept LEN+COL > 80
C
      IF ((LENGTH + COL) .GT. 80) THEN
	LLEN	= 80 - COL
      ELSE
	LLEN	= LENGTH
      END IF
C
C  Write the current value of STR in the field and leave the
C  cursor is at the BEGINNING of the string.
C
50    STR	= NUMSTR
      L1	= INANYL (STR, SPACE)
      L2	= INANYR (STR, SPACE)
      IF (L1 .LE. L2) CALL WRTFIELD (STR (L1:L2), 
     &	L2 - L1 + 1, ROW, COL)
      IF (.NOT. CURSOR) THEN
	CALL CURSON
	CFLAG	= .TRUE.
      ELSE
	CFLAG	= .FALSE.
      END IF
      CALL TCONTROL (POSCUR, ROW, COL)
      L	= 0
C
C  Read a character from the terminal.
C  Carriage Return or Up Arrow --> exit with the current string.
C
100   CALL GETCH (CH)

      IF (CH .EQ. RETURN) THEN
	NEXT	= .TRUE.
	IF (FIRST) THEN
		L1	= LLEN
	ELSE
		L1	= LLEN + 2
	END IF
	CALL ERSFIELD (L1, ROW, COL)
	L1	= INANYL (STR, SPACE)
	L2	= INANYR (STR, SPACE)
	IF (L1 .LE. L2) CALL WRTFIELD (STR (L1:L2), 
     &		L2 - L1 + 1, ROW, COL)
	GO TO 1000
      ELSE IF (CH .EQ. UPARROW1) THEN
	IF (ALENGTH .EQ. 1) THEN
		NEXT	= .FALSE.
		GO TO 1000
	ELSE
		DO 110 I = 2, ALENGTH
			CALL GETCH (CH)
110		CONTINUE
		IF (CH .EQ. UPARROWL) THEN
			NEXT	= .FALSE.
			GO TO 1000
		ELSE IF (CH .EQ. DNARROWL) THEN
			NEXT	= .TRUE.
			GO TO 1000
		ELSE
			CALL PUTCH (BELL)
			CALL TCONTROL (POSCUR, ROW, L + COL)
		END IF
	END IF
      ELSE IF (CH .EQ. DNARROW1) THEN
	NEXT	= .TRUE.
	GO TO 1000
C
C  Backspace --> delete the last character from the string unless
C     there are no more characters in the string (in which case,
C     sound the bell).
C
      ELSE IF (CH .EQ. BACKSP) THEN
	IF (L .GT. 0) THEN
		STR (L:L)	= ' '
		CALL ERSFIELD (2, ROW, COL + L)
		L	= L - 1
		CALL TCONTROL (POSCUR, ROW, COL + L)
	ELSE
		CALL PUTCH (BELL)
		CALL TCONTROL (POSCUR, ROW, COL)
	END IF
C
C  Non-numerical Character --> sound the bell.
C
      ELSE IF (IANYL ('1234567890+- ', CH) .EQ. 0) THEN
	CALL PUTCH (BELL)
	CALL TCONTROL (POSCUR, ROW, L + COL)
C
C  If the field is full, sound the bell.
C
      ELSE IF (L .EQ. LLEN) THEN
	CALL PUTCH (BELL)
	CALL TCONTROL (POSCUR, ROW, L + COL)
	CALL PUTCH (SPACE)
	CALL TCONTROL (POSCUR, ROW, L + COL)
C
C  If first character, erase the string and set string equal to this character
C
      ELSE IF (FIRST) THEN
	FIRST	= .FALSE.
	L	= 1
	STR	= CH
	L1	= INANYL (STR, SPACE)
	CALL WRTFIELD (STR (L1:LLEN), LLEN - L1 + 1, ROW, COL)
C
C  Otherwise, add the character to the string.
C
      ELSE
	L		= L + 1
	STR(L:L)	= CH
      END IF
      GO TO 100
C
C  Check for inappropriate input string
C  (e.g., blank string, more than one sign, presence of a character
C   other than a digit within the string, non-blank character preceding
C   the sign)
C

1000  IF (CFLAG) CALL CURSOFF
      IF (STR .EQ. BLANK) THEN
	GO TO 8010
      ENDIF
      L1	= IANYL (STR, '-+')
      IF (L1 .NE. 0) THEN
	L2	= IANYR (STR, '-+')
	IF (L1 .NE. L2) THEN
		GO TO 8010
        ENDIF
	L2	= INANYL (STR, ' ')
	IF (L1. GT. L2) THEN
		GO TO 8010
	END IF
      END IF
      IF (L1 .EQ. 0) THEN
	L1	= INANYL (STR, ' ')
      ELSE
	L1	= L1 + 1
      END IF
	L2	= INANYR (STR, ' ')
	K	= INANYL (STR (L1:L2), '1234567890')
      IF (K .NE. 0) THEN
	GO TO 8010
      ENDIF
C
C  Convert string to an integer
C
      READ (STR, *) J
C
C  Check that the integer is within the allowed range
C
      IF ((J .LT. MINV) .OR. (J .GT. MAXV)) THEN
	GO TO 8010
      ENDIF
C
C  If nothing has failed yet, return the new value of I
C
      I	= J
      IF (WARNFLAG) CALL ERSFIELD (79, WARNLINE, 1)
C
      RETURN
C
8010  IF (WARNLINE .GT. 0) THEN
	WARNFLAG	= .TRUE.
	CALL ERSFIELD (79, WARNLINE, 1)
	CALL WRTFIELD (ERRMSG, 79, WARNLINE, 1)
      ELSE
	WRITE (USEROUT, *) BELL
      END IF
      CALL ERSFIELD (LLEN, ROW, COL)
      GO TO 50
C
      END
