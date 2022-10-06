CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  getreal.f
C
C  This procedure gets a real number from the user.
C     It prompts the user with the current value of the real number.
C     If the user enters only RETURN, the current value of the
C     real number is returned.
C     If a new value is entered, it's range is checked with the
C     input variables MIN and MAX (MIN <= R <= MAX).
C
C  Uses Common Block  /USRIO/ USERIN,USEROUT
C     USERIN - logical unit for terminal input;
C     USEROUT - logical unit for terminal output;
C
C  Variables:
C     Q = question (passed without a question mark)
C     R = real number
C     MIN = minimum allowed value
C     MAX = maximum allowed value
C
C  Subroutine of TERMINAL.LIB
C
C  T.O. Videen -- 18-SEP-86
C
C  Modified by Tom Yang on Oct. 13, 1992 to replace CTOI with
C	NEWCTOI because a CTOI bug.
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE GETREAL (Q, R, MIN, MAX)
C
      INTEGER*4    L, L1, L2, L3, DP, M, N, WHOLE, FRACT,
     &             USERIN, USEROUT
      REAL         R, X, MIN, MAX, SIGN, DPLACES
      CHARACTER*1  CH, BELL
      CHARACTER*20 STR, BLANK
      CHARACTER*80 Q
C
      COMMON /USRIO/ USERIN,USEROUT
C
C  The following variables refer to locations in the string STR:
C     L1 is the first non-blank character
C     L2 is the first non-blank character which is not '+' or '-'
C     DP is the decimal point
C     L3 is the first character following the number
C
      BLANK = ' '
      BELL  = CHAR (7)
C
      L = INANYR (Q, ' ')
50    WRITE (USEROUT, *) ' ', Q(1:L), '?  <', R, '>'
      READ (USERIN, 8320, ERR = 9000) STR
C
      SIGN    = +1.0
      WHOLE   = 0
      FRACT   = 0
      DPLACES = 0.0
C
C  If no characters were entered, check that the old value
C  is within the allowed range.
C  ---------------------------------------------------------
C
      IF (STR .EQ. BLANK) THEN
        X = R
        GO TO 7000
      END IF
C
C  Find the first non-blank character
C
      L1 = INANYL (STR, ' ')
      IF (L1 .EQ. 0) GO TO 9000
C
C  If this character is '+' or '-', the following character
C    must be a digit or '.'
C
      CH = STR (L1:L1)
      IF (CH .EQ. '+') THEN
        L2 = L1 + 1
      ELSE IF (CH .EQ. '-') THEN
        SIGN = -1.0
        L2 = L1 + 1
      ELSE
        L2 = L1
      END IF
C
C  If the character at L2 is not a digit, then
C    it must be a decimal followed by a digit.
C
      M = IANYL (STR (L2:L2), '1234567890')
      IF (M .EQ. 0) THEN
        CH = STR (L2:L2)
        IF (CH .EQ. '.') THEN
          N = IANYL (STR (L2 + 1: L2 + 1), '1234567890')
          IF (N .NE. 0) THEN
            DP = L2
            GO TO 200
          ELSE
            GO TO 9000
          END IF
        ELSE
          GO TO 9000
        END IF
      END IF
C
C  Get the whole part of the number (L2 to DP-1)
C  ---------------------------------------------
C
      M = INANYL (STR (L2:20), '1234567890')
      IF (M .EQ. 0) THEN
        DP = 21
      ELSE
        DP = M + L2 - 1
      END IF
C
C  Maximum integer is 2147483647, so more than 9 digits
C    will not convert
C
      IF ((DP - L2) .GT. 9) GO TO 9100
      WHOLE = NEWCTOI (STR (L2: DP - 1))
C
C  If the next character is not a '.', this is the end of the number
C
      CH = STR (DP: DP)
      IF (CH .NE. '.') THEN
        L3 = DP
        GO TO 300
      END IF
C
C  If the character following the decimal is not a digit,
C    this it the end of the number
C
      M = IANYL (STR (DP + 1: DP + 1), '1234567890')
      IF (M .EQ. 0) THEN
        L3 = DP+1
        GO TO 300
      END IF
C
C  Get the fractional part of the number (DP+1 to L3-1)
C  ----------------------------------------------------
C
200   L3 = INANYL (STR (DP + 1: 20), '1234567890')
      IF (L3 .EQ. 0) THEN
        L3 = 21
      ELSE
        L3 = L3 + DP
      END IF
C
C  Maximum integer is 2147483647, so more than 9 digits
C    will not convert
C
      IF ((L3 - DP - 1) .GT. 9) GO TO 9100
      FRACT = NEWCTOI (STR (DP + 1: L3 - 1))
      DPLACES = FLOAT (L3 - 1 - DP)
C
C  All remaining characters in the string must be blank
C
300   IF (L3 .LE. 20) THEN
        M = INANYL (STR (L3: 20), ' ')
        IF (M .NE. 0) GO TO 9000
      END IF
C
C  The string is a syntactically correct real number
C    and can be converted
C  ---------------------------------------------------
C
      IF (FRACT .NE. 0) THEN
        X = FLOAT (WHOLE) + (FLOAT (FRACT) / (10.0 ** DPLACES))
      ELSE
        X = FLOAT (WHOLE)
      END IF
      X = X * SIGN
C
C  Check that the real number is within the allowed range
C
7000  If ((X .LT. MIN) .OR. (X .GT. MAX)) GO TO 9000
C
C  If nothing has failed yet, return the new value of R
C
      R = X
C
      RETURN
C
8320  FORMAT (A20)

9000  WRITE (USEROUT,*) '*** ERROR---enter a real number between ',
     2 MIN, ' and ', MAX,' ***', BELL
      GO TO 50
C
9100  TYPE *,'*** ERROR---no more than 9 digits are allowed   ***'
      TYPE *,'***         on either side of the decimal point ***',BELL
      GO TO 50
C
      END
