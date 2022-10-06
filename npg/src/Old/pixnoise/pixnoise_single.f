CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Program:      PIXNOISE.FTN
C
C  Author:       Tom O. Videen
C  Date:         20-Jan-93
C  Written For:  Quantitation of ECAT images in PETT VI format
C
C  Intent:
C     Compute mean and SD of pixel values for all 31-slices of a PET image.
C     The computations are performed on 120 5x5 pixel regions centered
C     on the image (coordinates XORG, YORG which are defined internally).
C
C  Logical Units:
C     2 = Image file           (INFILE)
C     6 = User output/log file (USEROUT)
C
C  libpetutil routine:
C     GETIMG
C
C  FORTRAN Runtime Library Routines used:
C     IANYL - returns the first location of a character in a string
C             searching from the left;
C     ENCODE- converts an integer to a string;
C
C  Use Makefile
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      PROGRAM PIXNOISE
C
      PARAMETER (NDIM=128) ! dimension of image array
      PARAMETER (NSIZE=5)  ! local region size = NSIZE x NSIZE pixels
      PARAMETER (NTOTAL=12)! total area sampled = NTOTAL x NTOTAL regions
C
      INTEGER*4  INFILE    ! lu for input image
      INTEGER*4  USEROUT   ! lu for user output
      INTEGER*4  SLICE     ! slice number
      INTEGER*4  NSLICES   ! number of slices
      INTEGER*4  PETTNUM   ! number of PET
      INTEGER*4  SUM       ! sum of pixel values within a region
      INTEGER*4  X0, Y0    ! coordinates of upper left of included region
      INTEGER*4  X1, X2    ! x-range to include in region
      INTEGER*4  Y1, Y2    ! y-range to include in region
      INTEGER*4  NREG      ! no. of regions included in slice mean
      INTEGER*4  I,J,K,L   ! local integers (array indices)
C
      INTEGER*2  IMAGE(NDIM,NDIM) ! image array
      INTEGER*2  HEADER(128)      ! file header record
      INTEGER*2  TEMP             ! local for element of IMAGE
C
      REAL*4     SliceMean(31) ! mean of pixel values for each slice
      REAL*4     SliceSD(31)  ! mean of SD for each slice
      REAL*4     MEAN        ! mean of pixel values within a region
      REAL*4     SD          ! SD of pixel values within a region
      REAL*4     SUMSQR      ! sum of square of IMAGE elements within a region
      REAL*4     SliceRegSum ! sum of means of regions on a slice
      REAL*4     SliceSDSum  ! sum of SDs of regions on a slice
      REAL*4     RegSum      ! sum of region means for all slices
      REAL*4     SDSum       ! sum of SD for all slices
      REAL*4     NPIX        ! number of pixels in a small region
      REAL*4     XORG,YORG   ! coordinates of center of phantom
C                             (0.5, 0.5 is upper left; 64.5, 64.5 is center)
C
      LOGICAL*1  MASK(NTOTAL,NTOTAL)  ! mask
      LOGICAL*1  ERR        ! detect errors in GETIMG
      LOGICAL*1  HDRONLY    ! read header only in GETIMG
C
      CHARACTER*256 FNAME   ! image file name
C
C  Constants
C
      DATA  INFILE, USEROUT /2, 6/
			DATA  XORG, YORG /64.5, 64.5/
C
C  Read input image file name
C
      call getarg(1,FNAME)
C
C  Initialize pixel location of upper left of large overlying square
C  region within which small regions are defined.
C
      X0 = NINT(XORG -0.5 - FLOAT(NTOTAL)*0.5*NSIZE)
      Y0 = NINT(YORG -0.5 - FLOAT(NTOTAL)*0.5*NSIZE)
C
C  Assuming that the overall square has 12 x 12 regions:
C    then the mask is initialized with 6 regions excluded from each
C    corner of the square.
C
      NREG = 0
      DO 30 I=1,NTOTAL
        DO 20 J=1,NTOTAL
          IF (I+J.LE.4 .OR. I+J.GE.22 .OR. I-J.GE.9 .OR. J-I.GE.9) THEN
            MASK(I,J) = .FALSE.
          ELSE
            MASK(I,J) = .TRUE.
            NREG = NREG+1
          END IF
20      CONTINUE
30    CONTINUE
C
C  Other initializations
C
      HDRONLY = .FALSE.          ! allows GETIMG to read image pixels
      NPIX = FLOAT(NSIZE*NSIZE)  ! number of pixels in a small region
      RegSum = 0.
      SDSum = 0.
C
C  Write file name, number of regions, and region size to output file
C
      WRITE(USEROUT,8200) FNAME( : lnblnk(FNAME)),NREG,NSIZE,NSIZE
8200  FORMAT (A,' using ',I3,I3,' x ',I1,' regions')
C
C  LOOP for all slices of the image.
C
      DO 1000 SLICE=1,31
C
C  Read 1 slice of image
C
        CALL GETIMG(IMAGE,HEADER,FNAME,SLICE,NSLICES,PETTNUM,INFILE,
     &    USEROUT,HDRONLY,ERR)
        IF (ERR) THEN
          WRITE(USEROUT,*) 'ERROR READING ',FNAME
          STOP
        END IF
C
C  Find mean and SD for all regions within area specified by MASK
C
        SliceRegSum = 0.
        SliceSDSum = 0.
        DO 500 J=1,NTOTAL
          DO 400 I=1,NTOTAL
            IF (MASK(I,J)) THEN
              SUM = 0
              SUMSQR = 0
              X1 = X0 + (I-1)*NSIZE
              X2 = X1 + NSIZE-1
              Y1 = Y0 + (J-1)*NSIZE
              Y2 = Y1 + NSIZE-1
              DO 300 L=Y1,Y2
                DO 200 K=X1,X2
                  TEMP = IMAGE(K,L)
                  SUM = SUM + IABS(TEMP)
                  SUMSQR = SUMSQR + FLOAT(TEMP)**2
200             CONTINUE
300           CONTINUE
              MEAN = FLOAT(SUM)/NPIX
              SD = SQRT(SUMSQR/NPIX - MEAN**2)
              SliceRegSum = SliceRegSum + MEAN
              SliceSDSum  = SliceSDSum + SD
            END IF
400       CONTINUE
500     CONTINUE
C
C  Compute mean pixel and mean SD for regions on this slice
C
        SliceMean(SLICE) = SliceRegSum/NREG
        SliceSD(SLICE)  = SliceSDSum/NREG
C
C  Sum mean and SD for all images processed
C
        RegSum = RegSum + SliceRegSum
        SDSum   = SDSum + SliceSDSum
1000	CONTINUE
C
C  Write results
C
C     WRITE(USEROUT,8300) (NINT(SliceMean(SLICE)),SLICE=1,31)
C     WRITE(USEROUT,8400) (SliceSD(SLICE),SLICE=1,31)
C     WRITE(USEROUT,8500) (SliceMean(SLICE),SLICE=1,31)
      WRITE(USEROUT,8600) (SliceSD(SLICE)/SliceMean(SLICE),SLICE=1,31)
C
8300  FORMAT(4(8I8,/))
8400  FORMAT(4(8F8.2,/))
8500  FORMAT(4(8F10.2,/))
8600  FORMAT(31(F10.5,/))
C
      STOP
      END
