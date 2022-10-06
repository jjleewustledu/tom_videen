CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  INCHAR.FTN
C
C  This procedure gets a character from the user's terminal
C     and returns the cursor to the beginning of the line on
C     which the prompt for the character was located and
C     erases that line.
C
C  Uses Common Block  /USRIO/ USERIN,USEROUT
C     USERIN - logical unit for terminal input;
C     USEROUT - logical unit for terminal output;
C
C  Variables:
C     CH = character entered from terminal;
C
C  Uses:
C     UPBLANK
C
C  Subroutine of TERMINAL.LIB
C
C  Called by D2ZOOM in d2zoom.f
C
C  T.O. Videen -- 24-Jan-86
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE INCHAR (CH)
 
      INTEGER*4		PBLK (6), IOREAD, USERIN, USEROUT
      CHARACTER*1	CH
      COMMON		/USRIO/	USERIN, USEROUT
      DATA		IOREAD /88/
C  Read a character
      CALL SYSIO (PBLK, IOREAD, USERIN, CH, 1, 0)
C  Erase the prompt and character
      CALL UPBLANK
C 
      RETURN
      END
