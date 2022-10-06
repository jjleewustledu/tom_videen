C$Id: termset.f,v 1.3 1994/06/16 21:02:03 ty7777 Exp $
C$Log: termset.f,v $
c Revision 1.3  1994/06/16  21:02:03  ty7777
c Added LIBPATH.
c
c Revision 1.2  1994/06/16  16:00:48  ty7777
c Introduces LIBPATH.
c
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  termset.f
C
C  Find the user's terminal type, then read in that terminal
C  type's control parameters from the terminal parameter file.
C
C  This subroutine does the following:
C     1 - Reads the terminal mneumonic device name from a
C         specified logical unit. (This LU should be set by
C         the CSS and should contain the current interactive
C         device name. Example of necessary commands in the
C         CSS:
C                   $DEFINE 1,,CURRENT(INTERACTIVE)
C                      .
C                      .
C                   $BUILD 1
C                   @*1
C                   $ENDB
C         The interactive device mneumonic would be read from
C         logical unit 1 in the above example.)
C     2 - Once the device mneumonic has been read, the subroutine
C         will close the logical unit.
C     3 - The subroutine will then use the same logical unit to
C         open the system level IDV (Installation Dependent
C         Variable) file. The subroutine will search the IDV for
C         the device mneumonic. Once found, the subroutine will
C         have the terminal "type" number. The subroutine will then
C         close the logical unit.
C     4 - The subroutine will then use the same logical unit
C         number to open the system level TPARAM (Terminal
C         Parameters) file.
C     5 - If the terminal "type" number is "UNKNOWN", the
C         subroutine will scan the TPARAM file and list all
C         terminal types that it knows. The user is asked to
C         select one or indicate that the user's terminal is none
C         of those listed.
C     6 - When the terminal type is known, the subroutine reads
C         all of that terminal's control parameters from the
C         TPARAM file into a COMMON array. If the user's terminal
C         type was not in the TPARAM file or the subroutine had
C         difficulty opening or reading the IDV or TPARAM files,
C         the COMMON array is filled with nulls.
C     7 - The subroutine will close the logical unit, freeing it
C         for other use, and will return to the main program.
C
C  Subroutine calling syntax:
C
C     CALL TERMSET ()
C
C  The setup for the COMMON blocks is:
C
C     CHARACTER TPARMS(120)*40
C     INTEGER*4 TLENGTH(120),USERIN,USEROUT,ROWLOC,COLLOC
C     LOGICAL*1 CURSOR
C     COMMON    /TPARAMS/ TPARMS,TLENGTH,ROWLOC,COLLOC,CURSOR
C     COMMON    /USRIO/   USERIN,USEROUT
C
C  where:
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
C     SYS:IDV/S
C     SYS:TPARAM/S
C
C  Written by Gary R. Hoffman 12 JUN 85 (TRMSET.FTN)
C  Revised by T.O. Videen -- 10-Feb-86
C  Revised by Tom Yang    -- 16-Jun-94.
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE TERMSET (AUXLU, LIBPATH)

      INTEGER*4     AUXLU    
      CHARACTER*256 LIBPATH
C
      INTEGER*4 TLENGTH (120), USERIN, USEROUT, ROWLOC, COLLOC
      CHARACTER TPARMS (120) * 40
      LOGICAL*1 CURSOR
      COMMON    /TPARAMS/ TPARMS,TLENGTH,ROWLOC,COLLOC,CURSOR
      COMMON    /USRIO/   USERIN,USEROUT

      INTEGER*4 STATUS, TYPE, L1
      CHARACTER CTYPE*6, LINE*80, NULLINE*40


C
C  GENERAL FORMAT STATEMENTS
C
 5    FORMAT (A5)
 80   FORMAT (A80)
 400  FORMAT ('  Sorry .. unable to open terminal parameter file')
 405  FORMAT ('  ???   "1" to "',I4,'", please')
 410  FORMAT ('  Sorry .. unable to read terminal parameter data')

C
C  Set constants
C
      DO 800 I = 1, 40
        NULLINE (I:I) = CHAR (0)
 800  CONTINUE
C
C  Initialize variables
C
      DO 900 I=1,120
         TPARMS(I) = NULLINE
         TLENGTH(I) = 0
 900  CONTINUE
      CURSOR = .TRUE.

      TYPE = 8             ! Graphon GO-230 MTG 12/04/89

C  Open TPARAM file and get control parameters.
C
3000  L1 = INANYR (LIBPATH, ' ')
      OPEN (AUXLU, FILE = LIBPATH (1:L1)//'/SYS:TPARAM',
     2	IOSTAT = STATUS)
      IF (STATUS .NE. 0) THEN
        WRITE (USEROUT, 400)
        GO TO 9000
      ENDIF
C
3005  REWIND (AUXLU)

      WRITE(CTYPE,'("!",I5.5)') TYPE

3010  READ (AUXLU, 80, ERR = 3050, END = 3050) LINE
      IF (LINE(1:6).NE.CTYPE) GO TO 3010
      DO 3030 I=1,60
         READ (AUXLU, 80) LINE
         TPARMS(I) = LINE(1:40)
         TLENGTH(I) = INANYR(TPARMS(I),CHAR(0))
         TPARMS(I+60) = LINE(41:80)
         TLENGTH(I+60) = INANYR(TPARMS(I+60),CHAR(0))
3030  CONTINUE
      DO 3040 I=1,40
        IF (TPARMS(3)(I:I).EQ.'r') ROWLOC = I
        IF (TPARMS(3)(I:I).EQ.'c') COLLOC = I
3040  CONTINUE
      GO TO 9000
C
3050  WRITE (USEROUT, 410)
      GO TO 9000
C
C  TERMINATE
C
9000  CLOSE (AUXLU)
C
      RETURN
      END
