c $Log: getimage.f,v $
c Revision 1.2  1992/11/02  16:02:53  tom
c Added RCS identifies
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   GETIMAGE.FTN
C
C  Author:       Tom O. Videen
C  Date:         18-May-87
C  Written For:  General subroutine for reading encoded image files
C
C  History:
C     Modified 12-Aug-87 by TOV to read write-protected files;
C     Modified 12-Aug-87 by TOV to eliminate INQUIRE statement
C        to check for file size and type.  When the compiler bug
C        is eliminated, these checks should be reinstated.
C        The bug causes INQUIRE to return FSIZE=-1 and FTYPE=blank
C        for write-protected files on the user's private account.
C     Modified 14-Nov-89 by TOV to read Scanditronix PC2048 images
C        in a PETT 8 format as PETTNUM = 1.
C     Modified 18-Apr-90 by TOV to read CTI ECAT-953B images
C        in a PETT 8 format as PETTNUM = 1.
C     Modified 01-Jun-90 by TOV to read 128x128 PETT VI images
C        in a PETT 8 format as PETTNUM = 1.
C     Modified 18-Sep-91 by TY porting to Sun X Window.
C
C			Modified for UNIX 16-Jan-92 TOV
C				FNAME changed to character*1024
C				LNBLNK used to manipulate strings
C
C  Intent:
C     This routine gets one slice of an image from a disk file.
C
C     Decoded and encoded PETT 6-8 images can be read (7-49 slices).
C        (PETT VI, Super PETT I and Super PETT IIH).
C
C     The encoding algorithm is that of ENCODE1 which was created in
C        April '87 and differs from that used previously.
C
C     All images are returned in a 128x128 array.
C        (PETT VI images are centered in the array).
C
C  Passed Variables:
C     IMG(128,128) = image array;
C     HEADER(128) = header from file for specified slice;
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
C  Note:
C     FNAME, SLICE, DISK, USER and HDRONLY must be assigned before calling.
C     IMG, HEADER, NSLICES, PETTNUM, and ERR are altered on return.
C
C  Local Variables:
C     HBYTES = number of bytes in file header;
C     P6IBYTES = number of bytes in PETT 6 image data;
C     P8IBYTES = number of bytes in Super PETT image data;
C     NBYTES = number of bytes to read for encoded image;
C     IOREAD = sequential access read I/O wait read;
C     IOREADR = random access read I/O wait read;
C     PB = parameter block used by SYSIO;
C     ADDR = random address to start reading a template from;
C     LOCAL = array for reading encoded and PETT VI images;
C     HDR = array to read file header;
C     L1,L2 = indices to nonzero elements in each row of encoded images;
C     CODE = type of encoding (1);
C     PNUM = PETT number (6-8, or 1);
C     NSLCS = number of slices in file;
C     NRECS = number of records in file;
C     REC = record no. at which encoded slice data begins;
C     P8SLCNUM = number of slices in PETT 8 file;
C     P7SLCNUM = number of slices in PETT 7 file;
C     P6SLCNUM = number of slices in PETT 6 file;
C     P8NAME = identifies Super PETT II file;
C     P7NAME = identifies Super PETT I file;
C     PETT6 = identifies PETT VI file;
C     FILEFORM = file format (to read 'ENCODED from encoded files);
C     CODED = true--> file is encoded;
C     SMALL = 100x100 array equivalenced with LOCAL to move PETT VI
C        image to 128x128 IMG array;
C     LARGE = 128x128 array equivalenced with LOCAL to move Super
C        PETT image to IMG array;
C
C  Uses:  SYSIO
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE GETIMAGE(IMG,HEADER,FNAME,SLICE,NSLICES,PETTNUM,
     &           DISK,USER,HDRONLY,ERR)
C
      INTEGER*4    SLICE,NSLICES,PETTNUM,DISK,USER,
     &             PB(6),IOREAD,IOREADR,ADDR,HBYTES,NBYTES,
     &             P8IBYTES,P6IBYTES,L1,L2,I,J,N
      INTEGER*2    IMG(128,128),HEADER(128),HDR(128),LOCAL(16384),
     &             CODE,PNUM,NSLCS,NRECS,REC(49),SMALL(100,100),
     &             LARGE(128,128),P8SLCNUM,P7SLCNUM,P6SLCNUM,PETT6
      CHARACTER*1  BELL
      CHARACTER*10 FILEFORM,P8NAME,P7NAME
			CHARACTER*256  RCSHEADER
      CHARACTER*1024 FNAME
      LOGICAL*1    HDRONLY, ERR, CODED, EXISTS
C
      EQUIVALENCE (SMALL,   LOCAL),
     &            (LARGE,   LOCAL)
      EQUIVALENCE (HDR(1),  FILEFORM),
     &            (HDR(6),  CODE),
     &            (HDR(7),  PNUM),
     &            (HDR(8),  NSLCS),
     &            (HDR(9),  NRECS),
     &            (HDR(10), REC)
      EQUIVALENCE (HDR(1),  P8NAME),
     &            (HDR(116),P7NAME),
     &            (HDR(33), P8SLCNUM),
     &            (HDR(19), P7SLCNUM),
     &            (HDR(32), P6SLCNUM),
     &            (HDR(121),PETT6)
C
      DATA IOREAD   /88/          ! Sequential access I/O wait read
      DATA IOREADR  /92/          ! Random access I/O wait read
      DATA HBYTES   /256/         ! Number of bytes in file header
      DATA P8IBYTES /32768/       ! Number of bytes in Super PETT image
      DATA P6IBYTES /20000/       ! Number of bytes in PETT VI image
      BELL = CHAR(7)
      ERR = .FALSE.
			RCSHEADER = "$Header: /home/petsun2/tom/src/fastproc/src/RCS/getimage.f,v 1.2 1992/11/02 16:02:53 tom Exp $"
C
C  Check existence of file FNAME
C
      INQUIRE (FILE=FNAME,EXIST=EXISTS,ERR=100)
      IF (.NOT.EXISTS) THEN
100     ERR = .TRUE.
        WRITE (USER, *) '*** File ',FNAME(:LNBLNK(FNAME)),
     &		' does not exist', BELL
        RETURN
      END IF
C
C  Open the requested file
C
      OPEN (DISK,FILE=FNAME,STATUS='OLD',ERR=120)
      GO TO 150
120   ERR = .TRUE.
      WRITE(USER,*) '*** Cannot Open ',FNAME(:LNBLNK(FNAME)),BELL
      RETURN
C
C  Read first record and check if file is encoded
C
150   CALL SYSIO (PB,IOREAD,DISK,HDR,HBYTES,0)
      IF (FILEFORM.EQ.'ENCODED   ') THEN
        CODED = .TRUE.
        PETTNUM = INT(PNUM)
        NSLICES = INT(NSLCS)
C
      ELSE
        CODED = .FALSE.
        IF (P8NAME.EQ.'SUPER PETT') THEN
          PETTNUM = 8
          NSLICES = INT(P8SLCNUM)
        ELSE IF (P8NAME.EQ.'PC2048-15B') THEN
          PETTNUM = 1
          NSLICES = INT(P8SLCNUM)
        ELSE IF (P8NAME(1:8).EQ.'ECAT-953') THEN
          PETTNUM = 1
          NSLICES = INT(P8SLCNUM)
        ELSE IF (P8NAME(1:8).EQ.'PETT VI ') THEN
          PETTNUM = 1
          NSLICES = INT(P8SLCNUM)
        ELSE IF (P7NAME.EQ.'SUPER PETT') THEN
          PETTNUM = 7
          NSLICES = INT(P7SLCNUM)
        ELSE IF (INT(PETT6).EQ.6) THEN
          PETTNUM = 6
          IF (P6SLCNUM.NE.0) THEN
            NSLICES = INT(P6SLCNUM)
          ELSE
            NSLICES = 7
          END IF
        ELSE
          ERR = .TRUE.
          WRITE(USER,*) '*** Illegal file format ',FNAME(:LNBLNK(FNAME)),BELL
          RETURN
        END IF
      END IF
C
C  If HDRONLY, read the header and return
C
      IF (HDRONLY) THEN
        IF (CODED) THEN
          CALL SYSIO (PB,IOREADR,DISK,HEADER,HBYTES,SLICE)
        ELSE
          IF (PETTNUM.EQ.6) THEN
            ADDR = (SLICE-1)*80
            CALL SYSIO (PB,IOREADR,DISK,HEADER,HBYTES,ADDR)
          ELSE
            ADDR = (SLICE-1)*129
            CALL SYSIO (PB,IOREADR,DISK,HEADER,HBYTES,ADDR)
          END IF
        END IF
        CLOSE(DISK)
        RETURN
      END IF
C
C  Otherwise read the requested slice
C
      IF (CODED) THEN
        ADDR = REC(SLICE)
        IF (SLICE.LT.NSLICES) THEN
          NBYTES = (REC(SLICE+1)-REC(SLICE))*256
        ELSE
          NBYTES = (NRECS-REC(SLICE))*256
        END IF
        CALL SYSIO (PB,IOREADR,DISK,HEADER,HBYTES,SLICE)
        CALL SYSIO (PB,IOREADR,DISK,LOCAL,NBYTES,ADDR)
      ELSE
        IF (PETTNUM.EQ.6) THEN
          ADDR = (SLICE-1)*80
          CALL SYSIO (PB,IOREADR,DISK,HEADER,HBYTES,ADDR)
          CALL SYSIO (PB,IOREAD,DISK,LOCAL,P6IBYTES,0)
        ELSE
          ADDR = (SLICE-1)*129
          CALL SYSIO (PB,IOREADR,DISK,HEADER,HBYTES,ADDR)
          CALL SYSIO (PB,IOREAD,DISK,IMG,P8IBYTES,0)
          CLOSE(DISK)
          RETURN
        END IF
      END IF
      CLOSE(DISK)
C
C  Zero the image array
C
      DO 300 J=1,128
        DO 200 I=1,128
          IMG(I,J) = 0
200     CONTINUE
300   CONTINUE
C
C  Decode PETT 6 image and move to 128*128 array
C
      IF (CODED) THEN
        N = 1
        IF (PETTNUM.EQ.6) THEN
          DO 500 J=1,100
            L1 = LOCAL(N)
            N = N+1
            L2 = LOCAL(N)
            N = N+1
            IF (L2.NE.0 .AND. L1.LE.L2) THEN
              DO 400 I=L1,L2
                IMG(I+14,J+14) = LOCAL(N)
                N = N+1
400           CONTINUE
            END IF
500       CONTINUE
C
C  Decode the Super PETT image
C
        ELSE
          DO 700 J=1,128
            L1 = LOCAL(N)
            N = N+1
            L2 = LOCAL(N)
            N = N+1
            IF (L2.NE.0 .AND. L1.LE.L2) THEN
              DO 600 I=L1,L2
                IMG(I,J) = LOCAL(N)
                N = N+1
600           CONTINUE
            END IF
700       CONTINUE
        END IF
C
C  Move PETT 6 image to 128*128 array
C
      ELSE
        DO 900 J=1,100
          DO 800 I=1,100
            IMG(I+14,J+14) = SMALL(I,J)
800       CONTINUE
900     CONTINUE
      END IF
C
      RETURN
      END
