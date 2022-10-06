CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   OPENFILE.FTN
C
C  Author:       Tom O. Videen
C  Date:         13-May-87
C  Written For:  TERMINAL.LIB
C
C  Intent:
C     This procedure opens a disk file for read or write.
C     If no file name is entered (i.e., file name is blank),
C     then subroutine returns with FNAME = ' ' and no file is opened.
C
C  Variables:
C     Q1 = query in first prompt;
C     LU = logical unit to assign to the file;
C     FNAME = file name;
C     VOL = default volume name;
C        If not blank, the file is automatically given this volume
C           unless the user enters a volume name;
C        If blank, then no volume is assumed;
C     EXT = default file extension;
C        If not blank, the file is automatically given this extension
C           unless the user enters an extension;
C        If blank, the extension must be entered by the user;
C     NEW = True--> the file is to be written to (and erased,
C        if it already exists); opened 'ERW';
C        False-->read only; opened 'SRO';
C
C     Iff NEW=.TRUE. then the following variables are used:
C
C       FTYPE = file type ('INDEX', 'CONTIG'); default is INDEX;
C       RSIZE = record length (80, 132, 256);
C          default is 132 for INDEX, always 256 for CONTIG;
C       NREC  = no. of records for CONTIG;
C
C     Q1, LU, VOL, EXT, NEW, FTYPE, RSIZE, and NREC are passed to
C        this subroutine.
C     FNAME may be passed to this subroutine (as the default in the
C        first prompt) and is passed back to the calling routine.
C
C  Uses library subroutines from TERMINAL.LIB:
C     GETINT
C     GTSTRING
C     YESNO
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C $Log: openfile.f,v $
c Revision 1.2  1992/04/30  19:15:01  ty7777
c Deleted "return" at the end of file to get rid of warning message.
c
c Revision 1.1  1992/04/13  19:08:15  tom
c Initial revision
c
c Revision 1.3  89/11/21  13:11:25  13:11:25  todd (M. Todd Gamble)
c Removed references to obsolete I/O control word SIZE.
c 
c Revision 1.2  89/11/21  12:49:56  12:49:56  todd (M. Todd Gamble)
c Removed references to obsolete I/O control words SHARE and TYPE.
c 
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE OPENFILE(Q1,LU,FNAME,VOL,EXT,NEW,FTYPE,RSIZE,NREC)
C
      INTEGER*4    LU,RSIZE,NREC,USERIN,USEROUT,L
      CHARACTER*1  BELL
      CHARACTER*3  EXT,NULLEXT
      CHARACTER*4  VOL,NULLVOL
      CHARACTER*6  FTYPE,NULLTYPE
      CHARACTER*20 FNAME,BLANK
      CHARACTER*80 Q1,Q,STR
      LOGICAL*1    NEW,ERASE
C
      COMMON /USRIO/ USERIN,USEROUT
C
      BELL = CHAR(7)
      BLANK = ' '
      NULLEXT = ' '
      NULLVOL = ' '
      NULLTYPE= ' '
C
C  Get the file name
C
100   STR = FNAME
      CALL GTSTRING(Q1,STR)
      FNAME = STR(1:20)
      IF (FNAME.EQ.BLANK) RETURN
C
C  If the default VOL is not null and if no volume name is entered,
C  Then use the default VOL
C
      IF (VOL.NE.NULLVOL) THEN
        L = IANYL(STR,':')
        IF (L.EQ.0) THEN
          L = INANYR(VOL,' ')
          FNAME = VOL(1:L)//STR(1:20-L)
          STR = FNAME
        END IF
      END IF
C
C  If the default EXT is not null and if no extension is entered,
C  Then use the default EXT
C
      IF (EXT.NE.NULLEXT) THEN
        L = IANYL(STR,'.')
        IF (L.EQ.0) THEN
          L = INANYR(STR,' ')
          FNAME = STR(1:L)//'.'//EXT
        END IF
      END IF
C
C  Check if the file exists
C
      IF (.NOT.NEW) THEN
        OPEN(LU,FILE=FNAME,STATUS='OLD',ERR=400)
        RETURN
      ELSE
        OPEN (LU,FILE=FNAME,STATUS='OLD',ERR=300)
        WRITE(USEROUT,*) '*** WARNING -- file exists ***',BELL
        ERASE = .FALSE.
        Q = 'Do you want to rewrite the file'
        CALL YESNO(Q,ERASE)
        IF (ERASE) THEN
          CLOSE(LU,STATUS='DELETE')
        ELSE
          CLOSE(LU)
          FNAME = ' '
          GO TO 100
        END IF
      END IF
C
C  If the file does not exist, allocate it
C
300   IF (FTYPE.EQ.'CONTIG') THEN
        RSIZE = 256
        IF (NREC.LE.0) THEN
          Q = 'File size (number of contiguous records)'
          CALL GETINT(Q,NREC,1,10000)
        END IF
      ELSE
        FTYPE = 'INDEX'
        NREC = 0
        IF (RSIZE.LE.0 .OR. RSIZE.GT.256) RSIZE=132
      END IF
      OPEN(LU,FILE=FNAME,STATUS='NEW',RECL=RSIZE,ERR=400)
      RETURN
C
400   WRITE(USEROUT,*) '*** ERROR:  cannot open file ',FNAME,BELL
      FNAME = ' '
      GO TO 100
C
      END
