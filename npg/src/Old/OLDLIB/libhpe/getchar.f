CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  GETCHAR.FTN
C
C  This procedure gets a character from the user.
C     It prompts the user with the current value of the character.
C     If the user enters a carriage return (or a blank space)
C       the character is returned unchanged.
C     If a new character is entered, it is returned.
C     The character must belong to the set of characters in the
C       string CHARSET.
C
C  Uses Common Block  /USRIO/ USERIN,USEROUT
C     USERIN - logical unit for terminal input;
C     USEROUT - logical unit for terminal output;
C
C  Variables:
C     Q = question (passed without a question mark)
C         (must be passed as CHARACTER*80)
C     CH = character
C     CHARSET = character string containing all acceptable characters;
C
C  Subroutine of TERMINAL.LIB
C
C  T.O. Videen -- 18-SEP-86
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE GETCHAR(Q,CH,CHARSET)
C
      INTEGER*4    I,K,L,USERIN,USEROUT
      CHARACTER*1  CH,INCHAR,BELL
      CHARACTER*80 Q,CHARSET
C
      COMMON /USRIO/ USERIN,USEROUT
C
      BELL = CHAR(7)
      I = INANYR(Q,' ')
100   WRITE(USEROUT,*) Q(1:I),'? <',CH,'>'
      READ (USERIN,8300,ERR=200) INCHAR
C
C  If user enters a blank character (or just RETURN),
C     return the original character;
C  Otherwise check that the character belongs to CHARSET.
C
      IF (INCHAR.EQ.' ') THEN
        GO TO 9000
      ELSE
        K = IANYL(INCHAR,CHARSET)
        IF (K.NE.0) THEN
          CH = INCHAR
          GO TO 9000
        END IF
      END IF
C
C  If illegal input, write an error message and repeat the input prompt
C
200   L = INANYR(CHARSET,' ')
      WRITE (USEROUT,8500) CHARSET(1:L),BELL
      GO TO 100
C
9000  RETURN
C
8300  FORMAT(A1)
8500  FORMAT(' ','*** ERROR -- allowed characters are ',A,' ***',A)
C
      END
