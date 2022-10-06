c $Log: readtype.f,v $
c Revision 1.3  1999/12/16  15:27:47  tom
c clean up
c
c Revision 1.2  1995/12/20  15:18:33  tom
c filtername for all images
c
c Revision 1.1  1995/12/20  04:31:34  tom
c Initial revision
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   READTYPE.FTN
C
C  Author:       Tom O. Videen
C  Date:         Dec-95
C  Written For:  METPROC
C
C  History:
C     Replaces READHEAD.FTN
C
C  Intent:
C     This routine gets selected parameters from an image file header.
C     PETT VI and ECAT files can be read.
C
C  Variables Passed:
C     ERRCODE = negative if not a supported scanner
C
C  Common Block Variables:
C
C  /SCANS/  - these variables are assigned in this subroutine:
C     SCANNER = scanner name (PETT VI, SUPER PETT IIH, etc);
C     SCANNAME = name of scan image file;
C     SCANDATE = date of scan (MM/DD/YYYY);
C     SLICES = number of slices in image file;
C     SCANTIME = length of scan (seconds) read from file header;
C     COMPOUND = compound used in scan;
C     FILTER = reconstruction filter;
C     RCONTYPE = reconstruction type;
C     RESOLUTION: 0 = low; 1 = high resolution scan;
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE READTYPE(ERRCODE)

      INTEGER*4    ERRCODE

      INTEGER*4    RINDEX
      INTEGER*4    PETTNUM
      INTEGER*4    NUMSLICES
      INTEGER*4    FILTYPE
      INTEGER*4    LENGTH
      INTEGER*4    NORMAVE
      INTEGER*4    NORMTIME
      CHARACTER*10 SDATE
      CHARACTER*11 CMPND
      CHARACTER*13 FILTERNAME 
      CHARACTER*80 Q
      CHARACTER*256 FNAME
      CHARACTER*256 RCSHEADER
      LOGICAL*1    A

      integer*4 getimagetype
      external getimagetype         !$pragma c (getimagetype)

      INCLUDE 'metproc.inc'

      RCSHEADER = "$Id: readtype.f,v 1.3 1999/12/16 15:27:47 tom Exp $"

C  Read the file header
C      (local variables FNAME and NUMSLICES must be used)

      ERRCODE = 0
      FNAME = IMGNAME
      PETTNUM = getimagetype (fname, filtype, numslices, length, sdate, cmpnd,
     &     filtername, normave, normtime)
      ERRCODE = PETTNUM
      IF (PETTNUM .NE. 6 .AND. PETTNUM .NE. 953 .AND. PETTNUM .NE. 961) RETURN

      SLICES = NUMSLICES
      SCANTIME = LENGTH / 1000
      SCANDATE = SDATE
      COMPOUND = CMPND
      FRAMELEN = 0.0
      RESOLUTION = 0
      FILTER = FILTERNAME
      IF (PETTNUM.EQ.961) THEN
         SCANNER = 'ECAT 961'
         RCONTYPE = 3070
      ELSE IF (PETTNUM.EQ.953) THEN
         SCANNER = 'ECAT 953B'
         RCONTYPE = 3000
      ELSE IF (PETTNUM.EQ.6) THEN
         SCANNER = 'PETT VI'
         RCONTYPE = FILTYPE
         IF (FILTER(1:7) .EQ. 'FILTERG') THEN
            RESOLUTION = 0
         ELSE IF (FILTER(1:7) .EQ. 'FILTERF') THEN
            RESOLUTION = 1
         ELSE
            Q = "Low Resolution Scan"
            A = .TRUE.
            CALL YESNO (Q,A)
            IF (A) THEN
               RESOLUTION = 0
            ELSE
               RESOLUTION = 1
            ENDIF
         ENDIF
         IF (NORMAVE.NE.0 .AND. NORMTIME.NE.0) THEN
            NORMMEAN = 60.*FLOAT(NORMAVE)/FLOAT(NORMTIME)
         ELSE
            NORMMEAN = 0.0
         END IF
      END IF

      IF (RINDEX(FNAME,'.').GT.0) THEN
         SCANNAME = FNAME(RINDEX(FNAME,'/')+1 : RINDEX(FNAME,'.'))
      ELSE
         SCANNAME = FNAME(RINDEX(FNAME,'/')+1 : LNBLNK(FNAME))
      END IF

      RETURN
      END
