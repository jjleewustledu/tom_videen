CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  PROMPT.FTN
C
C  This procedure writes a prompt and waits for an input key.
C     If the input key is RETURN or 'y' or 'Y', A = true.
C     Any other key returns A = false.
C     After a key is struck, the prompt is erased and the
C     cursor is left at the beginning of the line on which the
C     prompt was written.
C
C  Uses Common Block  /USRIO/ USERIN,USEROUT
C     USERIN - logical unit for terminal input;
C     USEROUT - logical unit for terminal output;
C
C  Variables:
C     Q = prompt
C     A = answer
C
C  Uses:
C     UPBLANK
C     TCONTROL
C
C  Subroutine of TERMINAL.LIB
C
C  T.O. Videen -- 24-Jan-86
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE PROMPT(Q,A)
 
      INTEGER*4    PBLK(6),IOREAD,L,USERIN,USEROUT

      CHARACTER*1  CH
      CHARACTER*80 Q
      LOGICAL*1    A
 
      COMMON /USRIO/ USERIN,USEROUT
 
      DATA IOREAD /88/
 
C  Write the prompt on the same line as the cursor
 
      CALL TCONTROL(5,1,0)
      L = INANYR(Q,' ')
      WRITE(USEROUT,*) Q(1:L),'? <Y'
 
C  Return cursor to the end of the prompt
 
      CALL TCONTROL(5,1,0)
      CALL TCONTROL(7,0,L+4)
 
C  If the key struck is RETURN or 'Y' or 'y', then A = true

      CALL SYSIO(PBLK,IOREAD,USERIN,CH,1,0) 

      IF ((CH.EQ.CHAR(13)).OR.(CH.EQ.'Y').OR.(CH.EQ.'y')) THEN
        A = .TRUE.
      ELSE
        A = .FALSE.
      END IF
 
C  Erase the prompt
 
      CALL UPBLANK
 
      RETURN
      END
