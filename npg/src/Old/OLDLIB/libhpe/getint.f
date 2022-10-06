C$Id: getint.f,v 1.2 1992/10/13 18:50:40 ty7777 Exp $
C$Log: getint.f,v $
c Revision 1.2  1992/10/13  18:50:40  ty7777
c Replace CTOI with NEWCTOI.
c
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  GETINT.FTN
C
C  This procedure gets an integer from the user.
C     It prompts the user with the current value of the integer.
C     If the user enters only RETURN, the current value of the
C     integer is returned (if it is in the allowed range).
C     The value of the integer entered is checked with the
C     input variables MIN and MAX (MIN<=I<=MAX).
C
C  Uses Common Block  /USRIO/ USERIN,USEROUT
C     USERIN - logical unit for terminal input;
C     USEROUT - logical unit for terminal output;
C
C  Variables:
C     Q = question (passed without a question mark)
C     I = integer value
C     MIN = minimum allowed value
C     MAX = maximum allowed value
C
C  Subroutine of TERMINAL.LIB
C
C  T.O. Videen -- 18-SEP-86
C
C  Modified by Tom Yang on Oct. 13, 1992 to replace CTOI to
C  NEWCTOI.
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      SUBROUTINE GETINT (Q, I, MIN, MAX)
C
      INTEGER*4    I, J, L, L1, L2, L3, M, MIN, MAX, USERIN, USEROUT
      CHARACTER*1  CH, BELL
      CHARACTER*20 STR, BLANK
      CHARACTER*80 Q
C
      COMMON /USRIO/ USERIN, USEROUT
C
      BLANK = ' '
      BELL  = CHAR (7)
C
      L = INANYR (Q, ' ')
50    WRITE (USEROUT, *) Q(1:L), '? <', I, '>'
      READ (USERIN, 8320, ERR = 9000) STR
C
C  If no characters were entered, check that the old value is
C    within the allowed range.
C
      IF (STR .EQ. BLANK) THEN
        J = I
        GO TO 7000
      END IF
C
C  Find the first non-blank character
C
      L1 = INANYL (STR, ' ')
      IF (L1 .EQ. 0) GO TO 9000
C
C  If this character is '+' or '-', the following character
C    must be a digit
C
      CH = STR(L1: L1)
      IF ((CH .EQ. '+') .OR. (CH .EQ. '-')) THEN
        L2 = L1 + 1
      ELSE
        L2 = L1
      END IF
C
C  The character at L2 must be a digit
C
      M = IANYL (STR (L2: L2), '1234567890')
      IF (M .EQ. 0) GO TO 9000
C
C  Get character just beyond the last digit of the integer
C
      L3 = INANYL (STR (L2: 20), '1234567890')
      IF (L3 .EQ. 0) THEN
        L3 = 21
      ELSE
        L3 = L3 + L2 - 1
      END IF
C
C  All remaining characters in the string must be blank
C
      IF (L3 .LE. 20) THEN
        M = INANYL (STR (L3: 20), ' ')
        IF (M .NE. 0) GO TO 9000
      END IF
C
C  The string is a syntactically correct integer and can be converted
C
      J = NEWCTOI (STR (L1: L3 - 1))
C
C  Check that the integer is within the allowed range
C
7000  If ((J .LT. MIN) .OR. (J .GT. MAX)) GO TO 9000
C
C  If nothing has failed yet, return the new value of I
C
      I = J
C
      RETURN
C
8320  FORMAT (A20)
C
9000  TYPE *,'*** ERROR---enter an integer between ',
     2  MIN, ' and ', MAX,' ***', BELL
      GO TO 50
C
      END
