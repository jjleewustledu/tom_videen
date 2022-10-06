CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  TCONTROL.FTN
C
C  Terminal Control routine.
C  Handles special cursor and screen control of user terminal.
C
C  Variables:
C     FUNCTION is the number of the control parameter this routine
C        should perform on the user's terminal.
C     ROW and COL are the row and column that the cursor should be
C        positioned at if the FUNCTION is to position the cursor.
C     ROW and COL also serve as the number of rows/columns to
C        move the cursor if FUNCTION is move cursor.
C
C  An array containing the terminal's control parameters is passed
C     to the subroutine by the main routine via the following
C     COMMON statement:
C
C     INTEGER*4 TLENGTH(120),ROWLOC,COLLOC,USERIN,USEROUT
C     CHARACTER TPARMS(120)*40
C     LOGICAL*1 CURSOR
C     COMMON    /TPARAMS/ TPARMS,TLENGTH,ROWLOC,COLLOC,CURSOR
C     COMMON    /USRIO/   USERIN,USEROUT
C
C  TPARMS  = character strings for 120 terminal control functions;
C  TLENGTH = length of each character string in TPARMS;
C  ROWLOC  = location of row number in string TPARMS(3);
C  COLLOC  = location of column number in string TPARMS(3);
C            (position cursor function)
C  CURSOR  = true if cursor is on; false if off;
C  USERIN  = logical unit assigned for user input;
C  USEROUT = logical unit assigned for user output;
C
C  Uses:
C     SYSIO
C
C  Written by Gary R. Hoffman on 13 JUN 85
C  Revised by T.O. Videen -- 8/6/85
C  Revised by T.O. Videen -- 14-Nov-85; Function 3 for GraphOn and
C     any other terminal using numbers for setting cursor position.
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE TCONTROL (FUNCTION, ROW, COL)
 
      INTEGER*4		TLENGTH (120), ROWLOC, COLLOC, USERIN, USEROUT
      CHARACTER		TPARMS (120) * 40
      LOGICAL*1		CURSOR
      COMMON		/TPARAMS/ TPARMS, TLENGTH, ROWLOC, COLLOC, CURSOR
      COMMON		/USRIO/ USERIN, USEROUT
C 
      INTEGER*4		FUNCTION, ROW, COL, MAXFUNC, LLEN, I,
     2			PBLK (6), IOWRITE
      CHARACTER		COMMAND*40
C 
C  Set constants
C 
      DATA		MAXFUNC /120/
      DATA		IOWRITE /56/
C 
      PBLK (6)	= 0
C 
C  Check validity of request
C 
      IF (FUNCTION .LT. 1 .OR. FUNCTION .GT. MAXFUNC) GO TO 9000
C 
      COMMAND	= TPARMS (FUNCTION)
      LLEN	= TLENGTH (FUNCTION)
C 
C  POSITION CURSOR command
C     Case 1 for terminals requiring 2-digit numbers for row & column
C     Case 2 for terminals requiring single character for row & column
C 
      IF (FUNCTION .EQ. 3) THEN
	IF (COMMAND (ROWLOC + 1:ROWLOC + 1) .EQ. '*') THEN
		COMMAND (ROWLOC:ROWLOC)			= CHAR ((ROW / 10) + 48)
		COMMAND (ROWLOC + 1:ROWLOC + 1)		= CHAR (MOD (ROW, 10) + 48)
		COMMAND (COLLOC:COLLOC)			= CHAR ((COL / 10) + 48)
		COMMAND (COLLOC + 1:COLLOC + 1)		= CHAR (MOD (COL, 10) + 48)
	ELSE
		COMMAND (ROWLOC:ROWLOC)		= CHAR (ROW + 31)
		COMMAND (COLLOC:COLLOC)		= CHAR (COL + 31)
	END IF
C
	CALL SYSIO (PBLK, IOWRITE, USEROUT, COMMAND, LLEN, 0)
C 
C  Move cursor UP
C 
      ELSE IF (FUNCTION .EQ. 5) THEN
	DO 510 I = 1, ROW
		CALL SYSIO (PBLK, IOWRITE, USEROUT, COMMAND, LLEN, 0)
510	CONTINUE
C
C 
C  Move cursor DOWN
 
      ELSE IF (FUNCTION .EQ. 6) THEN
	DO 610 I = 1, ROW
		CALL SYSIO (PBLK, IOWRITE, USEROUT, COMMAND, LLEN, 0)
610	CONTINUE
C
C 
C  Move cursor RIGHT
C 
      ELSE IF (FUNCTION .EQ. 7) THEN
	DO 710 I = 1, COL
		CALL SYSIO (PBLK, IOWRITE, USEROUT, COMMAND, LLEN, 0)
710	CONTINUE
C 
C  Move cursor LEFT
 
      ELSE IF (FUNCTION .EQ. 8) THEN
	DO 810 I = 1, COL
		CALL SYSIO (PBLK, IOWRITE, USEROUT, COMMAND, LLEN, 0)
810	CONTINUE
C 
C  Execute terminal command (not requiring additional parameters)
C 
      ELSE
	CALL SYSIO (PBLK, IOWRITE, USEROUT, COMMAND, LLEN, 0)
      END IF
 
9000  RETURN
 
      END
