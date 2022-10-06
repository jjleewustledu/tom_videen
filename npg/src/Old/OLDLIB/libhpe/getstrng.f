CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  GETSTRNG.FTN
C
C  This procedure gets a character string from the user.
C     It prompts the user with the current value of the string.
C     If the user enters a carriage return, the string is
C     returned unchanged.
C     If a new string is enterred, it is returned.
C     If the user types the ESC key followed by RETURN,
C       the original string is replaced with a null string.
C
C  Uses Common Block  /USRIO/ USERIN,USEROUT
C     USERIN - logical unit for terminal input;
C     USEROUT - logical unit for terminal output;
C
C  Variables:
C     Q = question (passed without a question mark)
C         (must be passed as CHARACTER*80)
C     STR = character string ending with a '~'
C
C  Subroutine of TERMINAL.LIB
C
C  T.O. Videen -- 24-Jan-86
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C $Log: getstrng.f,v $
c Revision 1.1  1992/04/13  19:08:15  tom
c Initial revision
c
c Revision 1.1  89/11/21  12:42:29  12:42:29  todd (M. Todd Gamble)
c Initial revision
c 
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      SUBROUTINE GETSTRNG(Q,STR)
C
      INTEGER*4    I,J,USERIN,USEROUT
      CHARACTER*79 STR2,BLANK
      CHARACTER*80 Q,STR
C
      COMMON /USRIO/ USERIN,USEROUT
C
      BLANK = ' '
C
      I = INANYR(Q,' ')
      J = IANYL(STR,'~')
50    IF (J.EQ.1) THEN
        WRITE(USEROUT,*) Q(1:I),'? < >'
      ELSE
        WRITE(USEROUT,*) Q(1:I),'? <',STR(1:J-1),'>'
      END IF
      READ (USERIN,8379,ERR=80) STR2
      GO TO 100
80    WRITE(USEROUT,*) '*** ERROR---enter up to 79 characters ***'
      GO TO 50
C
C  If user enters a blank string (or just RETURN),
C     return the original string;
C  If user enters ESC as the first character, return a null string;
C
100   IF (STR2(1:1).EQ.CHAR(27)) THEN
        STR = '~'
      ELSE IF (STR2.NE.BLANK) THEN
        STR = STR2
        I = INANYR(STR,' ')
        STR(I+1:I+1) = '~'
      END IF
C
      RETURN
8379  FORMAT(A79)
      END
