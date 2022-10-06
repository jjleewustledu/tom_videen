C$Id: getimg.f,v 1.2 1993/10/21 19:31:51 ty7777 Exp $
C$Log: getimg.f,v $
c Revision 1.2  1993/10/21  19:31:51  ty7777
c Added RCSHEADER.
c
c Revision 1.1  1993/03/03  20:28:04  ty7777
c Initial revision
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   GETIMG.F
C
C  Author:       Tom Yang
C  Date:         11-May-92
C  Written For:  General subroutine for reading encoded image files
C
C  History:
C
C  Passed Variables:
C     IMG (128,128) = image array;
C     HEADER (128) = header from file for specified slice;
C     FNAME = image file name;
C     SLICE = number of the desired slice of the image (1-14);
C     NSLICES = number of slices in the image;
C     PETTNUM = PETT number of the image (6-8 or 1);
C     DISK = logical unit to read the image file;
C     USER = logical unit assigned for writing to the user's terminal;
C     HDRONLY = true--> read & return the header only;
C               false--> read & return the header and image;
C     ERR = true iff unable to open the disk file, unable to
C        read the desired slice, or if the file format is incorrect;
C
C  Local Variables:
C     ENCODED = true--> file is encoded;
C
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE GETIMG (IMG, HEADER, FNAME, SLICE, NSLICES,
     &           PETTNUM, DISK, USER, HDRONLY, ERR)
C
      INTEGER*4    SLICE, NSLICES, PETTNUM, DISK, USER
      INTEGER*4    fd 
      INTEGER*4    getfd 
      INTEGER*4    getimage_type 
      INTEGER*2    IMG (128, 128), HEADER (128),
     &             SLICEIDX, NUMSLICES
      CHARACTER*256 FNAME
      CHARACTER*256 RCSHEADER
      LOGICAL*1    HDRONLY, ERR
      LOGICAL*1    ENCODED
      external getimage !$pragma C (getimage)
      external getimage_type !$pragma C (getimage_type)
C
      RCSHEADER = '$Header: /home/petsun4/ty7777/src/libnpg/RCS/getimg.f,v 1.2 1993/10/21 19:31:51 ty7777 Exp $'
C
      ERR = .FALSE.
      OPEN (DISK, FILE = FNAME, STATUS = 'OLD', ERR = 120)
      GO TO 150
120   WRITE (*, *) '*** Cannot Open ', FNAME
      RETURN
150   fd = getfd (DISK)		! get the HPUX file descriptor
      SLICEIDX = SLICE
      PETTNUM = getimage_type (%VAL(fd), FNAME, 
     &  ENCODED, NUMSLICES)
      CLOSE (DISK)
      NSLICES = NUMSLICES
C
      OPEN (DISK, FILE = FNAME, STATUS = 'OLD', ERR = 220)
      GO TO 250
220   WRITE (*,*) '*** Cannot Open ', FNAME
      RETURN
250   fd = getfd (DISK)		! get the HPUX file descriptor
C
C  Zero the image array
C
      DO 300 J = 1, 128
        DO 200 I = 1, 128
          IMG (I, J) = 0
200     CONTINUE
300   CONTINUE
      Call getimage (IMG, HEADER, %VAL(fd), FNAME, 
     1	%VAL(SLICEIDX), %VAL(ENCODED), %VAL(NUMSLICES), 
     2	%VAL(PETTNUM), ERR)
      CLOSE (DISK)
C
      RETURN
      END
