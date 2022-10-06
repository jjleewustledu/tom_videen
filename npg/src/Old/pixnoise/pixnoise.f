CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Program:      PIXNOISE.FTN
C
C  Author:       Tom O. Videen
C  Date:         26-Jan-93
C  Written For:  Quantitation of ECAT images in PETT VI format
C
C		COMPUTE SED/MEAN FOR 120 5X5 PIXEL ROIS FOR INDIVIDUAL SLICES
C   If 2nd param passed, compute axial FOV
C
C		Modified 26-Jan to divide Diff by Regional Mean
C			instead of dividing SED by Slice Mean
C
C  Intent:
C     Compute coefficient of variation for regional differences between 2 PET images.
C     The computations are performed on 120 5x5 pixel regions centered
C    		on the image (coordinates XORG, YORG which are defined internally).
C     Computed for each of NPLANES slices.
C			Input Test and Control filenames and scale factors for each
C				are read from a list created by NORMECAT.FTN
C			Note that Scale1 is positive and Scale2 is negative.
C
C  Logical Units:
C     1 = List of files and scalers (LIST)
C     2 = Image file                (INFILE)
C     6 = User output/log file      (USEROUT)
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
      PARAMETER (NDIM=128)   ! dimension of image array
      PARAMETER (NSIZE=5)    ! local region size = NSIZE x NSIZE pixels
      PARAMETER (NTOTAL=12)  ! total area sampled = NTOTAL x NTOTAL regions
      PARAMETER (NPLANES=31) ! number of planes (slices)
C
      INTEGER*4  LIST        ! lu for input list
      INTEGER*4  INFILE      ! lu for input images
      INTEGER*4  USEROUT     ! lu for user output
      INTEGER*4  SLICE       ! slice number
      INTEGER*4  NSLICES     ! number of slices
      INTEGER*4  PETTNUM     ! number of PET
      INTEGER*4  X0, Y0      ! coordinates of upper left of included region
      INTEGER*4  X1, X2      ! x-range to include in region
      INTEGER*4  Y1, Y2      ! y-range to include in region
      INTEGER*4  NREG        ! no. of regions included in slice mean
      INTEGER*4  I,J,K,L     ! local integers (array indices)
			INTEGER*4  S1(4)       ! slice ranges for computing FOVSED
			INTEGER*4  S2(4)       ! slice ranges for computing FOVSED
			INTEGER*4  S3(4)       ! slice ranges for computing FOVSED
			INTEGER*4  S4(4)       ! slice ranges for computing FOVSED
C
      INTEGER*2  IMAGE1(NDIM,NDIM) ! image array 1
      INTEGER*2  IMAGE2(NDIM,NDIM) ! image array 2
      INTEGER*2  HEADER(128)       ! file header record
C
      REAL*4     XORG,YORG ! coordinates of center of phantom
C                             (0.5, 0.5 is upper left; 64.5, 64.5 is center)
      REAL*4     NPIX      ! number of pixels in a small region
      REAL*4     Scale1    ! normalization factor for FILE1
      REAL*4     Scale2    ! normalization factor for FILE2
      REAL*4     TEMP1     ! pixel sum (IMAGE1 + IMAGE2)
      REAL*4     TEMP2     ! pixel difference (IMAGE1 - IMAGE2)
      REAL*4     Sum       ! sum of pixel values (TEMP1) within a region
      REAL*4     Diff      ! sum of pixel differences (TEMP2) within a region
      REAL*4     Mean      ! regional mean
      REAL*4     MeanDiff  ! regional mean difference
      REAL*4     SumMean   ! sum of regional means for a slice
      REAL*4     SumDiff   ! sum of regional differences for a slice
      REAL*4     SumSqr    ! sum of regional differences squared for a slice
      REAL*4     SliceMean(NPLANES) ! mean regional value for each slice
      REAL*4     SliceDiff(NPLANES) ! mean regional differences for each slice
      REAL*4     SliceSED(NPLANES)  ! mean regional standard error of the differences for each slice
      REAL*4     FOV(4)    ! predefined axial FOVs (mm)
      REAL*4     FOVSED(4) ! mean regional SED for predefined axial FOV
C
      LOGICAL*1  MASK(NTOTAL,NTOTAL)  ! mask
      LOGICAL*1  ERR           ! detect errors in GETIMG
      LOGICAL*1  HDRONLY       ! read header only in GETIMG
C
			CHARACTER*1   CH         ! input character flag
			CHARACTER*20  NAME       ! use to read 20-char filenames
      CHARACTER*256 LISTNAME   ! list file name
      CHARACTER*256 FNAME1     ! image file name #1
      CHARACTER*256 FNAME2     ! image file name #2
C
C  Constants
C
      DATA  LIST, INFILE, USEROUT /1, 2, 6/
			DATA  XORG, YORG /64.5, 64.5/
			DATA  S1  / 11,   5,   1,    1/
			DATA  S2  / 16,  10,   4,   16/
			DATA  S3  / 17,  22,  28,   17/
			DATA  S4  / 21,  27,  31,   31/
			DATA  FOV /3.8, 7.9, 10.6, 10.6/
C
C  Read input image file names and scale factors
C
      CALL GETARG(1,LISTNAME)
			OPEN (LIST,FILE=LISTNAME,STATUS='OLD',ERR=11)
			GO TO 12
11		WRITE(USEROUT,*) LISTNAME( : lnblnk(LISTNAME))," not found"
			STOP
12    READ(LIST,*)
			READ(LIST,8000) NAME, Scale1
			FNAME1 = NAME
			READ(LIST,8000) NAME, Scale2
			FNAME2 = NAME
8000	FORMAT(1X,A20,F7.4)
C
			CALL GETARG(2,CH)
			IF (CH .EQ. 'A') THEN
				AFOV = .TRUE.
			ELSE
				AFOV = .FALSE.
			END IF
C
C  Initialize pixel location of upper left of large overlying square
C  region within which small regions are defined.
C
      X0 = NINT(XORG -0.5 - FLOAT(NTOTAL)*0.5*NSIZE)
      Y0 = NINT(YORG -0.5 - FLOAT(NTOTAL)*0.5*NSIZE)
C
C  Define the MASK for the square array of regions:
C    Assuming that the overall square has 12 x 12 regions
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
      HDRONLY = .FALSE.             ! allows GETIMG to read image pixels
      NPIX    = FLOAT(NSIZE*NSIZE)  ! number of pixels in a small region
C
C  Write file name, number of regions, and region size to output file
C
      WRITE(USEROUT,8200) LISTNAME( : lnblnk(LISTNAME)),NREG,NSIZE,NSIZE
8200  FORMAT (A,' using ',I3,I3,' x ',I1,' regions')
C
C  LOOP for all slices of the image.
C
      DO 1000 SLICE=1,NPLANES
C
C  Read 1 slice of image
C
        CALL GETIMG(IMAGE1,HEADER,FNAME1,SLICE,NSLICES,PETTNUM,INFILE,
     &    USEROUT,HDRONLY,ERR)
        IF (ERR) THEN
          WRITE(USEROUT,*) 'ERROR READING ',FNAME1
          STOP
        END IF
        CALL GETIMG(IMAGE2,HEADER,FNAME2,SLICE,NSLICES,PETTNUM,INFILE,
     &    USEROUT,HDRONLY,ERR)
        IF (ERR) THEN
          WRITE(USEROUT,*) 'ERROR READING ',FNAME2
          STOP
        END IF
C
C  Find mean and SDdiff for all regions within area specified by MASK
C
        SumMean = 0.
        SumDiff = 0.
				SumSqr  = 0.
        DO 500 J = 1,NTOTAL
          DO 400 I = 1,NTOTAL
            IF (MASK(I,J)) THEN
              X1 = X0 + (I-1)*NSIZE
              X2 = X1 + NSIZE-1
              Y1 = Y0 + (J-1)*NSIZE
              Y2 = Y1 + NSIZE-1
							Sum = 0.
							Diff = 0.
              DO 300 L=Y1,Y2
                DO 200 K=X1,X2
                  TEMP1 = (Scale1 * FLOAT(IMAGE1(K,L))) - (Scale2 * FLOAT(IMAGE2(K,L)))
                  TEMP2 = (Scale1 * FLOAT(IMAGE1(K,L))) + (Scale2 * FLOAT(IMAGE2(K,L)))
                  Sum  = Sum  + (0.5 * TEMP1)
									Diff = Diff + TEMP2
200             CONTINUE
300           CONTINUE
							Mean     = Sum/NPIX
							MeanDiff = (Diff/NPIX)/Mean
              SumMean  = SumMean + Mean
              SumDiff  = SumDiff + MeanDiff
              SumSqr   = SumSqr  + MeanDiff**2
            END IF
400       CONTINUE
500     CONTINUE
C
C  Compute mean pixel and mean SDdiff for regions on this slice
C
				SliceMean(SLICE) = SumMean/NREG
				SliceDiff(SLICE) = SumDiff/NREG
				SliceSED(SLICE)  = SQRT(SumSqr/NREG - SliceDiff(SLICE)**2)
C
1000	CONTINUE
C
C  Compute mean SED for axial FOV's
C
			IF (AFOV) THEN
				DO 1200 I=1,4
					Sum = 0.
					NSLICES = 0
					DO 1100 SLICE = S1(I), S2(I)
						Sum = Sum + SliceSED(SLICE)
						NSLICES = NSLICES + 1
1100			CONTINUE
					DO 1150 SLICE = S3(I), S4(I)
						Sum = Sum + SliceSED(SLICE)
						NSLICES = NSLICES + 1
1150			CONTINUE
					FOVSED(I) = Sum/FLOAT(NSLICES)
1200		CONTINUE
				WRITE(USEROUT,8500) (FOV(I), I=1,4)
				WRITE(USEROUT,8500) (FOVSED(I), I=1,4)
			ELSE
      	WRITE(USEROUT,8600) (SliceSED(SLICE), SLICE=1,NPLANES)
			END IF
C
8500 	 FORMAT(4(F10.4))
8600  FORMAT(31(F10.4,/))
C
      STOP
      END
