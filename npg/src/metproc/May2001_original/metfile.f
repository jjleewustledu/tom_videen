c $Log: metfile.f,v $
c Revision 2.2  1999/12/16  15:27:47  tom
c clean up
c
c Revision 2.1  1993/07/15  20:25:17  tom
c Changed RCSHEADER to ID
c
c Revision 2.0  1993/07/12  23:13:24  tom
c Handles ECAT image format
c
c Revision 1.4  1992/11/02  16:27:14  tom
c *** empty log message ***
c
c Revision 1.3  1992/10/19  15:56:10  tom
c modified RCSHEADER length
c
c Revision 1.2  1992/10/16  22:32:51  tom
c added RCSHEADER
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   METFILE.FTN
C
C  Author:       Tom O. Videen
C  Date:         09-Jun-87
C  Written for:  METPROC
C
C  History:
C      Modified for UNIX 23-Jan-92 TOV
C         FNAME, HDRNAME, RINDEX
C      Modified 12-May-92 TOV
C         FNAME --> IMGNAME (in common block, as in original OS32)
C         STR declared as char*256
C      Modified 28-Oct-92 by TOV
C         1) Added HDREXT to call to METFILE (metfile.f)
C
C  Intent:
C     This subroutine checks that the scan file to be processed exists.
C     The scan file name normally is comprised of the subject
C     id followed by the scan id and may have any extension but has
C     traditionally been .C, .CMG, .IMG, or none.
C
C  Variables Passed:
C     HDRNAME
C     SKIPSCAN - true--> do not process the scan;
C                false--> process the scan;
C      HDREXT  - extension for hdr file
C
C  Common Block Variables:
C
C  /USRIO/
C     USEROUT = logical unit for user output;
C
C  /SCANS/
C     NTYPE = type of scan file;
C     COUNT = number of scan files processed of each type;
C     SCANID = ID used at end of scan file name;
C     SUBJECT = subject ID;
C     PATH =path to scan file;
C     EXTENSION = scan file extension;
C     IMGNAME = scan file name;
C     SCANTYPE = type of scan;
C     HEADER = header of DTA file;
C
C  Subroutines:
C
C  TERMINAL.LIB
C     GTSTRING
C     YESNO
C
C  Called by:  METPROC
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE METFILE(HDRNAME,SKIPSCAN,HDREXT)

      INCLUDE 'metproc.inc'

      INTEGER*4    RINDEX
      CHARACTER*1  BELL
      CHARACTER*3  HDREXT
      CHARACTER*80 Q
      CHARACTER*256 HDRNAME, STR
      LOGICAL*1    SKIPSCAN,EXISTS,A
      CHARACTER*256 RCSHEADER

      BELL = CHAR(7)
      RCSHEADER = "$Id: metfile.f,v 2.2 1999/12/16 15:27:47 tom Exp $"

100   WRITE(USEROUT,*)'This is ',SCANTYPE(NTYPE),' Scan #',COUNT(NTYPE)
      WRITE(USEROUT,*)'Scan ID = ',SCANID
      Q = 'Process this Scan'
      A = .TRUE.
      CALL YESNO(Q,A)
      IF (.NOT.A) THEN
        SKIPSCAN = .TRUE.
        RETURN
      ELSE
        SKIPSCAN = .FALSE.
      END IF

C  Get file name

      IMGNAME = PATH(: LNBLNK(PATH))//'/'//SUBJECT(: LNBLNK(SUBJECT))//
     &   SCANID(: LNBLNK(SCANID))//EXTENSION(: LNBLNK(EXTENSION))
      STR = IMGNAME(:LNBLNK(IMGNAME))
      Q = 'Name of File to process'
      CALL GTSTRING(Q,STR)
      IF (STR(1:1).NE.'/') THEN
        IMGNAME = PATH(: LNBLNK(PATH))//'/'//STR(: LNBLNK(STR))//
     &   	EXTENSION(: LNBLNK(EXTENSION))
      ELSE
        IMGNAME = STR(: LNBLNK(STR))
      END IF
      INQUIRE(FILE=IMGNAME,EXIST=EXISTS,ERR=200)

C  Set HDR file name and return

      IF (EXISTS) THEN
         IF (LNBLNK(EXTENSION).EQ.0) THEN
         	HDRNAME = IMGNAME(: LNBLNK(IMGNAME))//'.'//HDREXT
         ELSE
         	HDRNAME = IMGNAME(: RINDEX(IMGNAME,'.'))//HDREXT
         END IF
         RETURN
      END IF

200   WRITE(USEROUT,*)'>>> ERROR: Cannot find ',IMGNAME,BELL
      GO TO 100
      END
