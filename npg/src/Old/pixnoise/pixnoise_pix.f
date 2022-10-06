CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Program:      PIXNOISE.FTN
C
C  Author:       Tom O. Videen
C  Date:         26-Jan-93
C  Written For:  Quantitation of ECAT images in PETT VI format
C
C	THIS VERSION COMPUTES VALUES FOR SINGLE PIXELS WITHIN A MASK.
C		Divide Diff by Pixel Mean instead of dividing SED by Slice Mean (C*1)
C   If 3rd param passed, compute axial FOV
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
      PARAMETER (NPLANES=31) ! number of planes (slices)
C
      INTEGER*4  LIST        ! lu for input list
      INTEGER*4  INFILE      ! lu for input images
      INTEGER*4  USEROUT     ! lu for user output
      INTEGER*4  SLICE       ! slice number
      INTEGER*4  NSLICES     ! number of slices
      INTEGER*4  PETTNUM     ! number of PET
      INTEGER*4  NREG        ! no. of regions included in slice mean
      INTEGER*4  I,J,I,J     ! local integers (array indices)
			INTEGER*4  S1(4)       ! slice ranges for computing FOVSED
			INTEGER*4  S2(4)       ! slice ranges for computing FOVSED
			INTEGER*4  S3(4)       ! slice ranges for computing FOVSED
			INTEGER*4  S4(4)       ! slice ranges for computing FOVSED
C
      INTEGER*2  IMAGE1(NDIM,NDIM) ! image array 1
      INTEGER*2  IMAGE2(NDIM,NDIM) ! image array 2
      INTEGER*2  MASK(NDIM,NDIM)   ! image array for mask
      INTEGER*2  HEADER(128)       ! file header record
C
      REAL*4     Scale1    ! normalization factor for FILE1
      REAL*4     Scale2    ! normalization factor for FILE2
      REAL*4     Sum       ! sum of pixel values
      REAL*4     Diff      ! pixel differences
      REAL*4     SumMean   ! sum of pixel means for a slice
      REAL*4     NDiff     ! normalized pixel difference
      REAL*4     SumDiff   ! sum of regional differences for a slice
      REAL*4     SumSqr    ! sum of regional differences squared for a slice
      REAL*4     SliceMean(NPLANES) ! mean regional value for each slice
      REAL*4     SliceDiff(NPLANES) ! mean regional differences for each slice
      REAL*4     SliceSED(NPLANES)  ! mean regional standard error of the differences for each slice
C
      REAL*4     FOV(4)    ! predefined axial FOVs (mm)
      REAL*4     FOVSED(4) ! mean regional SED for predefined axial FOV
C
      LOGICAL*1  ERR           ! detect errors in GETIMG
      LOGICAL*1  HDRONLY       ! read header only in GETIMG
      LOGICAL*1  AFOV          ! compute axial FOVs instead of individual slice values
C
			CHARACTER*1   CH         ! input character flag
      CHARACTER*20  NAME  		 ! file name
      CHARACTER*256 LISTNAME   ! list file name
      CHARACTER*256 FNAME1     ! image file name #1
      CHARACTER*256 FNAME2     ! image file name #2
      CHARACTER*256 MASKNAME   ! image file name of mask
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
      HDRONLY = .FALSE.             ! allows GETIMG to read image pixels
C
C  Read input image file names and scale factors
C
      CALL GETARG(1,LISTNAME)
			OPEN (LIST,FILE=LISTNAME,STATUS='OLD',ERR=11)
			GO TO 12
11		WRITE(USEROUT,*) LISTNAME( : lnblnk(LISTNAME))," not found"
			STOP

12    CALL GETARG(2,MASKNAME)
			OPEN (INFILE,FILE=MASKNAME,STATUS='OLD',ERR=13)
			GO TO 14
13		WRITE(USEROUT,*) MASKNAME( : lnblnk(MASKNAME))," not found"
			STOP
14		CLOSE(INFILE)

		  READ(LIST,*)
			READ(LIST,8000) NAME, Scale1
			FNAME1 = NAME
			READ(LIST,8000) NAME, Scale2
			FNAME2 = NAME
8000	FORMAT(1X,A20,F7.4)
C
			CALL GETARG(3,CH)
			IF (CH .EQ. 'A') THEN
				AFOV = .TRUE.
			ELSE
				AFOV = .FALSE.
			END IF
C
C  Write file name, number of regions, and region size to output file
C
      WRITE(USEROUT,8200) LISTNAME( : lnblnk(LISTNAME)), MASKNAME( : lnblnk(MASKNAME))
8200  FORMAT (A,' using pixels in ',A)
C
C  LOOP for all slices of the image.
C
      DO 1000 SLICE=1,NPLANES
C
C  Read 1 slice of image
C
        CALL GETIMG(IMAGE1,HEADER,FNAME1,SLICE,NSLICES,PETTNUM,INFILE,USEROUT,HDRONLY,ERR)
        IF (ERR) THEN
          WRITE(USEROUT,*) 'ERROR READING ',FNAME1( : LNBLNK(FNAME1))
          STOP
        END IF
        CALL GETIMG(IMAGE2,HEADER,FNAME2,SLICE,NSLICES,PETTNUM,INFILE,USEROUT,HDRONLY,ERR)
        IF (ERR) THEN
          WRITE(USEROUT,*) 'ERROR READING ',FNAME2( : LNBLNK(FNAME2))
          STOP
        END IF
        CALL GETIMG(MASK,HEADER,MASKNAME,SLICE,NSLICES,PETTNUM,INFILE,USEROUT,HDRONLY,ERR)
        IF (ERR) THEN
          WRITE(USEROUT,*) 'ERROR READING ',MASKNAME( : LNBLNK(MASKNAME))
          STOP
        END IF
C
C  Find mean and SDdiff for all regions within area specified by MASK
C
        SumMean = 0.
        SumDiff = 0.
				SumSqr  = 0.
				NREG = 0
        DO 500 J = 1,NDIM
          DO 400 I = 1,NDIM
            IF (MASK(I,J) .GE. 1) THEN
              Sum  = 0.5 * (Scale1 * FLOAT(IMAGE1(I,J))) - (Scale2 * FLOAT(IMAGE2(I,J)))
              Diff = (Scale1 * FLOAT(IMAGE1(I,J))) + (Scale2 * FLOAT(IMAGE2(I,J)))
              SumMean  = SumMean + Sum
							NDiff = Diff/Sum
              SumDiff  = SumDiff + NDiff
              SumSqr   = SumSqr  + NDiff**2
							NREG = NREG + 1
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
8500  FORMAT(4(F10.4))
8600  FORMAT(31(F10.4,/))
C
      STOP
      END
