CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Program:      NORMECAT.FTN
C
C  Author:       Tom O. Videen
C  Date:         18-Jan-93
C  Written For:  Get normalization values for ECAT images.
C
C  History:
C     Modified for Unix 28-Jan-93 TOV.
C
C  Intent:  Create list of 2 images and scale factors for SUMECAT using
C     an input image for a mask.
C
C  Logical Units:
C     1 = input images
C     3 = list output file
C     5 = user output
C
C  Uses subroutines:
C     GETIMG
C     SYSIO
C
C  Makefile
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      PROGRAM NORMECAT
C
      INTEGER*2    IMG(128,128) ! input image file
      INTEGER*2    MASK(128,128)! input mask image
      INTEGER*2    HEADER(128) ! header
C
      INTEGER*4    DISK        ! lu for reading image
      INTEGER*4    LISTOUT     ! lu for output list suitable for SUMECAT
      INTEGER*4    USEROUT     ! lu for user output
      INTEGER*4    SLICE       ! slice no.
      INTEGER*4    NSLICES     ! no. slices
      INTEGER*4    TOTSLICES   ! no. slices of slices to process
      INTEGER*4    PETTNUM     ! PETT number
      INTEGER*4    I,J         ! array indices
      INTEGER*4    NPIX        ! number of pixels in template
      INTEGER*4    SLICESUM    ! sum of pixel values for 1 slice
      REAL*4       SUM         ! sum of pixel values within template
      REAL*4       MEAN        ! mean pixel value within template
      REAL*4			 SCALETO     ! normalize to this constant
C
      CHARACTER*256 FNAME       ! file name
      CHARACTER*256 FNAME1      ! file name 1
      CHARACTER*256 FNAME2      ! file name 2
      CHARACTER*256 MASKNAME    ! mask file name
      CHARACTER*256 LISTNAME    ! list file name
C
      LOGICAL*1    HDRONLY
      LOGICAL*1    ERR
C
      DATA DISK, MASKLU, LISTOUT, USEROUT /1,2,3,6/
      DATA HDRONLY  /.FALSE./
			DATA SCALETO  /1000./
			DATA TOTSLICES /31/
C
      WRITE(USEROUT,*) '*** normecat  28-Jan-93 {Create a list ',
     & 'for SUMECAT -- scaling to ',SCALETO,'}'
C
C  Get input file name
C
			CALL GETARG(1,FNAME1)
			CALL GETARG(2,FNAME2)
			CALL GETARG(3,MASKNAME)
			CALL GETARG(4,LISTNAME)
C
			OPEN(LISTOUT,FILE=LISTNAME,ERR=10)
			GO TO 20
10		WRITE(USEROUT,*) 'ERROR opening file :',LISTNAME(: LNBLNK(LISTNAME))
			STOP
C
20    WRITE(LISTOUT,*) '2   1.0  ',TOTSLICES,'  ',MASKNAME(:LNBLNK(MASKNAME))
      FNAME = FNAME1
C
100   NPIX = 0
      SUM = 0.
      DO 500 SLICE = 1,TOTSLICES
        CALL GETIMG(IMG,HEADER,FNAME,SLICE,NSLICES,PETTNUM,DISK,USEROUT,HDRONLY,ERR)
        IF (ERR) THEN
          WRITE(USEROUT,*) '*** ERROR opening file ',FNAME(: LNBLNK(FNAME))
          STOP
        ELSE IF (NSLICES.NE.TOTSLICES) THEN
          WRITE(USEROUT,*) 'Error:  Input file is  ',NSLICES,' slices'
          WRITE(USEROUT,*) '        Output file is ',TOTSLICES,' slices'
          STOP
        END IF
C
        CALL GETIMG(MASK,HEADER,MASKNAME,SLICE,NSLICES,PETTNUM,DISK,USEROUT,HDRONLY,ERR)
        IF (ERR) GO TO 210
        GO TO 220
210     WRITE(USEROUT,*) '*** ERROR opening file ',FNAME(: LNBLNK(FNAME))
        STOP
C
220     SLICESUM = 0
        DO 400 I=1,128
          DO 300 J=1,128
            IF (MASK(I,J).GT.0) THEN
              NPIX = NPIX + 1
              SLICESUM = SLICESUM + IMG(I,J)
            END IF
300       CONTINUE
400     CONTINUE
        SUM = SUM + FLOAT(SLICESUM)
500   CONTINUE
C
      MEAN = SUM/FLOAT(NPIX)
      WRITE(USEROUT,8600) NPIX, MEAN, SCALETO/MEAN
C
      IF (FNAME.EQ.FNAME1) THEN
        WRITE(LISTOUT,8700)  FNAME, SCALETO/MEAN
        FNAME = FNAME2
        GO TO 100
      ELSE
        WRITE(LISTOUT,8700)  FNAME, -SCALETO/MEAN
      END IF
C
8600  FORMAT(1X,'TOTAL     Pixels =',I7,'  Mean = ',F8.2,
     &  '  Factor =',F7.4)
8700  FORMAT(1X,A20,F7.4)
C
      STOP
      END
