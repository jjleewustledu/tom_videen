CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  YESNO.FTN
C
C  This procedure prompts the user with a question (Q) requiring
C     a 'Y' or 'N' answer.  If the user enters carriage return
C     in response to the prompt, the previous value of the
C     answer (A) is unchanged.
C
C  Variables:
C     Q = question (passed without a question mark)
C     A = answer
C
C  Uses Common Block  /USRIO/ USERIN,USEROUT
C     USERIN - logical unit for terminal input;
C     USEROUT - logical unit for terminal output;
C
C  Subroutine of TERMINAL.LIB
C
C  T.O. Videen -- 18-SEP-86
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE YESNO(Q,A)
C
      INTEGER*4    I,USERIN,USEROUT
      CHARACTER*1  NULL,CH,CH1,BELL
      CHARACTER*80 Q
      LOGICAL*1    A
C
      COMMON /USRIO/ USERIN,USEROUT
C
      NULL = CHAR(32)
      BELL = CHAR(7)
C
      IF (A) THEN
        CH1 = 'Y'
      ELSE
        CH1 = 'N'
      END IF
C
      I = INANYR(Q,' ')
50    WRITE(USEROUT,*) Q(1:I),'? <',CH1,'>'
      READ (USERIN,8300,ERR=80) CH
C
      IF ((CH.EQ.'Y').OR.(CH.EQ.'y')) THEN
        A = .TRUE.
      ELSE IF ((CH.EQ.'N').OR.(CH.EQ.'n')) THEN
        A = .FALSE.
      ELSE IF (CH.NE.NULL) THEN
	GO TO 80
      END IF
C
      RETURN
80      WRITE(USEROUT,*) '*** ERROR---enter Y or N ***',BELL
        GO TO 50
8300  FORMAT(A1)
      END
