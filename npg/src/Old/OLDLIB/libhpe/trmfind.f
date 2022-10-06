C$Id: trmfind.f,v 1.3 1994/06/16 16:09:12 ty7777 Exp $
C$Log: trmfind.f,v $
c Revision 1.3  1994/06/16  16:09:12  ty7777
c Added LIBPATH as an argument.
c
C
C**************************************************************
C
C     TRMFIND.FTN              (subroutine)
C
C     Find "type" number for user's terminal
C
C     Written by Gary R. Hoffman 17 MAR 85
C     Modified 16-Jun-94 by Tom Yang to add LIBPATH. 
C
C     This subroutine does the following:
C        1 - Reads the terminal mneumonic device name from a
C            specified logical unit. (This LU should be set by
C            the CSS and should contain the current interactive
C            device name. Example of necessary commands in the
C            CSS:
C                      $DEFINE 1,,CURRENT(INTERACTIVE)
C                         .
C                         .
C                      $BUILD 1
C                      @*1
C                      $ENDB
C            The interactive device mneumonic would be read from
C            logical unit 1 in the above example.)
C        2 - Once the device mneumonic has been read, the subroutine
C            will close the logical unit.
C        3 - The subroutine will then use the same logical unit to
C            open the system level IDV (installation Dependent
C            Variable) file. The subroutine will search the IDV for
C            the device mneumonic. Once found, the subroutine will
C            have the terminal "type" number.
C            The subroutine will close the logical unit, freeing it
C            for other use by the main routine.
C        4 - If the terminal "type" number is "UNKNOWN", the
C            subroutine will prompt the user with a list of known
C            terminals and ask the user to select the correct one.
C            (This would happen in the case of the IDV list showing
C            that the user's terminal is a phone-in line. The user
C            would have to tell the subroutine what type of terminal
C            is being used.)
C        5 - The subroutine returns the terminal type to the main
C            routine. If the user's terminal is not in the list of
C            known terminals, a "0" is returned.
C
C     Subroutine calling syntax:
C
C        CALL TRMFIND (AUXLU,USER,TYPE)
C
C     ..where AUXLU is the auxiliary logical unit that is attached
C     to a temporary file containing the device mneumonic name. This
C     logical unit will later be used by the subroutine to open
C     the IDV file, then released for future use by the main routine.
C     USER is the logical unit of the user's terminal. This is used
C     only if the terminal type found in the IDV is "UNKNOWN"  and
C     the subroutine must query the user directly. TYPE is the
C     terminal type returned to the main routine ("0" if the
C     subroutine is unable to determine the type). All of these
C     variables are INTEGER*4 type.
C
C     All logical units are determined by the main routine.
C
C     This subroutine calls no other subroutines, but does open
C     SYS:IDV/S temporarily.
C
C******************************************************************

      SUBROUTINE TRMFIND (AUXLU, USER, TYPE, LIBPATH)

      INTEGER*4 AUXLU, USER, TYPE, STATUS, L1
      CHARACTER DEVM*5, LINE*80
      CHARACTER*256 LIBPATH
      CHARACTER*256 RCSHEADER
C
C  GENERAL FORMAT STATEMENTS
C
 5    FORMAT (A5)
 80   FORMAT (A80)
C
      RCSHEADER = '$Header: /usr/local/src/libpe/RCS/trmfind.f,v 1.3 1994/06/16 16:09:12 ty7777 Exp $'
C
C  GET DEVICE MNEUMONIC
C
      READ (AUXLU,5) DEVM
      CLOSE (AUXLU,IOSTAT=STATUS)
      IF (STATUS.NE.0) GO TO 2000
C
C  OPEN IDV, FIND TERMINAL TYPE
C
      L1 = INANYR (LIBPATH, ' ')
      OPEN (AUXLU, FILE = LIBPATH (1:L1)//'/SYS:IDV/S', IOSTAT = STATUS)
      IF (STATUS.NE.0) GO TO 2000
 1000 CONTINUE
      READ (AUXLU,80,ERR=2000) LINE
      IF (LINE(1:20).EQ.'!TERMINAL TYPES    ') GO TO 1010
      GO TO 1000
 1010 CONTINUE
      READ (AUXLU,80,ERR=2000) LINE
      IF (LINE(1:5).EQ.DEVM) THEN
         READ(LINE(6:10),'(I5)') TYPE
         IF (TYPE.EQ.0) THEN
            GO TO 2000
         ELSE
            GO TO 9000
         ENDIF
      ENDIF
      IF (LINE(1:20).EQ.'!END TERMINAL TYPES ') GO TO 2000
      GO TO 1010
C
C  TERMINAL TYPE UNKNOWN OR NOT FOUND FOR SOME REASON.
C  PROMPT USER FOR TERMINAL TYPE.
C
 2000 CONTINUE
      WRITE (USER,2005)
 2005 FORMAT ('  Which of the following terminals are you using?',
     2       /'       1 - Perkin-Elmer 550 VDU',
     3       /'       2 - Televideo 910',
     4       /'       3 - Televideo 914',
     5       /'       4 - Televideo 925',
     6       /'       5 - Perkin-Elmer 6100',
     7       /'       6 - Freedom 100',
     8       /'       7 - None of the above')
      READ (USER,80) LINE
      IF (LINE(1:1).LT.'1'.OR.LINE(1:1).GT.'7') THEN
         WRITE (USER,2010)
 2010    FORMAT ('  ???   "1" to "7", please')
         GO TO 2000
      ENDIF
      READ(LINE(1:1),'(I1)') TYPE
      IF (TYPE.EQ.7) TYPE=0
      GO TO 9000
C
C  TERMINATE
C
 9000 CONTINUE
      CLOSE (AUXLU)
      RETURN
      END
