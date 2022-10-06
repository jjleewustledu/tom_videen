c $Log: readhdr.f,v $
c Revision 2.3  1999/12/16  16:29:59  tom
c fix date bug
c
c Revision 2.2  1999/12/16  15:27:47  tom
c clean up
c
c Revision 2.1  1993/07/15  20:25:17  tom
c Changed RCSHEADER to ID
c
c Revision 2.0  1993/07/12  23:13:24  tom
c Handles ECAT image format
c
c Revision 1.3  1992/10/19  15:56:10  tom
c modified RCSHEADER length
c
c Revision 1.2  1992/10/16  22:32:51  tom
c added RCSHEADER
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   READHDR.FTN
C  Author:       Tom O. Videen
C  Date:         14-May-87
C  Written for:  METPROC
C  Intent:
C     This subroutine reads scan information from an existing .HDR file.
C     See HDRFORM for values in this file.
C
C  History:
C      	FNAME --> IMGNAME (in common block, as in original OS32)
C
C  Variables Passed:
C     DISK = logical unit assigned for scan file I/O;
C
C  Variables:
C     IN = array used to read HDR files;
C     HEAD* = values equivalenced with elements in OUT array header;
C
C  Common Block Variables:
C
C  /SCANS/
C     SLICES = number of slices in image file;
C     SCANTIME = length of scan (seconds) read from file header;
C     RCONTYPE = reconstruction type;
C     SCANNAME = name of scan image file;
C     SCANDATE = date of scan (MM/DD/YY);
C     COMPOUND = compound used in scan;
C     FILTER = reconstruction filter;
C     SCANNER = scanner name (PETT VI, SUPER PETT IIH, etc);
C     RESOLUTION: 0 = low; 1 = high resolution scan;
C
C  Uses Subroutines:
C  FORTRAN Runtime Library
C     SYSIO
C
C  Called by:  METPROC
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE READHDR(DISK,ERRCODE)

      INTEGER*4    DISK,ERRCODE,PB(6),IOREAD,NBYTES
      INTEGER*2    IN(30),HEAD4,HEAD5,HEAD8,HEAD9
      CHARACTER*1  BELL
      CHARACTER*8  HEAD2,HEAD3
      CHARACTER*10 HEAD6
      CHARACTER*12 HEAD7
      CHARACTER*14 HEAD1
      CHARACTER*256 RCSHEADER

      EQUIVALENCE (IN(1),  HEAD1),
     2            (IN(8),  HEAD2),
     3            (IN(12), HEAD3),
     4            (IN(16), HEAD4),
     5            (IN(17), HEAD5),
     6            (IN(18), HEAD6),
     7            (IN(23), HEAD7),
     8            (IN(29), HEAD8),
     9            (IN(30), HEAD9)

      INCLUDE 'metproc.inc'
      DATA IOREAD,NBYTES  /88,60/
      RCSHEADER = "$Id: readhdr.f,v 2.3 1999/12/16 16:29:59 tom Exp $"
      BELL = CHAR(7)
      ERRCODE = 0
      OPEN(DISK,FILE=IMGNAME,STATUS='OLD',ERR=9000)
      CALL SYSIO(PB,IOREAD,DISK,IN,NBYTES,0)

      SCANNER  = HEAD1
      SCANNAME = HEAD2
      SCANDATE = HEAD3
      SLICES   = HEAD4
      SCANTIME = HEAD5
      COMPOUND = HEAD6
      FILTER   = HEAD7
      RCONTYPE = INT(HEAD8)
      RESOLUTION = INT(HEAD9)
      CLOSE(DISK)
      RETURN

9000  WRITE(USEROUT,*) '>>> ERROR: Cannot open',IMGNAME,BELL
      ERRCODE = -1
      RETURN
      END
