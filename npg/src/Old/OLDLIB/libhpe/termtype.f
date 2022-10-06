C$Id: termtype.f,v 1.2 1994/06/16 15:53:29 ty7777 Exp $
C$Log: termtype.f,v $
c Revision 1.2  1994/06/16  15:53:29  ty7777
c Introduced LIBPATH.
c
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   TERMTYPE.FTN
C
C  Author:       Tom O. Videen
C  Date:         11-Nov-87
C  Written For:  TERMINAL.LIB
C
C  Intent:
C      Get the user's terminal type from the SYS:IDV file.
C
C  Variables passed:
C     TYPENUM - terminal type no. read from IDV file (1- );
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE TERMTYPE (TYPENUM, LIBPATH)
C
      INTEGER*4     TYPENUM, USERIN, USEROUT, NUM, L1
      INTEGER*4     TEMPLU          ! replaces AUXLU 
      CHARACTER*5   DEVICE
      CHARACTER*80  STR, Q
      CHARACTER*256 LIBPATH
      CHARACTER*256 RCSHEADER
C
      COMMON /USRIO/ USERIN, USEROUT
C
8100  FORMAT (A5)
8200  FORMAT (A80)
C
      RCSHEADER = '$Header: /usr/local/src/libpe/RCS/termtype.f,v 1.2 1994/06/16 15:53:29 ty7777 Exp $'
C
      TEMPLU  = 1
      TYPENUM = 0
      DEVICE  = 'TT4: '
      L1      = INANYR (LIBPATH, ' ')
      OPEN (TEMPLU, FILE = LIBPATH (1:L1)//'/SYS:IDV/S', ERR = 200)
C
100   READ (TEMPLU,8200,ERR=200,END=200) STR
      IF (STR(1:5).EQ.DEVICE) THEN
	READ(STR(6:10),'(I5)') TYPENUM
        IF (TYPENUM.GT.0) THEN
          CLOSE(TEMPLU)
          RETURN
        END IF
      END IF
      IF (STR(1:20).EQ.'!END TERMINAL TYPES ') GO TO 200
      GO TO 100
C
8400  FORMAT(1X,I5,3X,A)
200   CLOSE (TEMPLU)
      OPEN (TEMPLU, FILE = LIBPATH (1:L1)//'/SYS:TPARAM/S', ERR = 500)
      WRITE (USEROUT, *) 'Terminal Types Recognized by System:'
250   READ  (TEMPLU, 8200, ERR = 300, END = 300) STR
      IF (STR(1:2).EQ.'!0' .AND. STR(1:6).NE.'!00000') THEN
	 READ(STR(2:6),'(I5)') NUM
         WRITE (USEROUT,8400) NUM,STR(11:80)
      END IF
      GO TO 250
C
300   CLOSE(TEMPLU)
      WRITE(USEROUT,8400) 0,'None of the above'
      Q = 'Which of the above terminals are you using'
      CALL GETINT(Q,TYPENUM,0,NUM)
      RETURN
C
500   WRITE (USEROUT, *) 
     2  'Unable to open ', LIBPATH (1:L1)//'/SYS:TPARAM/S'
      RETURN
      END
