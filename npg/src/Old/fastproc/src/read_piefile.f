c $Log: read_piefile.f,v $
c Revision 1.3  1993/04/26  17:24:31  tom
c Revised for ECAT images (pie file type 7)
c
c Revision 1.2  1992/11/02  16:02:53  tom
c Added RCS identifies
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   READ_PIEFILE.FTN
C
C  Author:       Tom O. Videen
C  Date:         14-FEB-92
C  Written For:  fastproc
C
C  Intent:
C     This subroutine reads PIE values from a disk file.
C
C  History:
C			Modification of GETPIE which eliminates all prompts.
C			Modified 26-Apr-93 TOV:
C					Default IMGTYPE = 3000 (ECAT)
C					Handle PIE file type 7
C
C  Variables Passed:
C     NPIES  - number of pie slope/bank pair values;
C     PSLOPES - pie slopes;
C     BNKPAIRS - bank pairs;
C     DISK - logical unit for reading or writing pie file;
C     PIENAME - name of PIE file;
C     IMGTYPE - image type: 1000 = old reconstruction
C                           1020 = new reconstruction (1988)
C                           2000 = EM  reconstruction (1990)
C                           3000 = ECAT images (1992)
C
C  Called by:  fastproc
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE READ_PIEFILE(NPIES,PSLOPES,BNKPAIRS,DISK,PIENAME,IMGTYPE,PMEAN,PDATE)
C
      INTEGER*4    NPIES,DISK,ITYPE,IMGTYPE
      REAL*4       PSLOPES(10),BNKPAIRS(10),DUMMY,PMEAN
      CHARACTER*1  BELL
      CHARACTER*8  PDATE
      CHARACTER*1024 PIENAME
      CHARACTER*80 PHEADER
      CHARACTER*256 RCSHEADER
C			INTEGER*4		 L
C			CHARACTER*80 STR
C
			RCSHEADER = "$Header: /home/petsun2/tom/src/fastproc/src/RCS/read_piefile.f,v 1.3 1993/04/26 17:24:31 tom Exp $"
      BELL = CHAR(7)
C
C  Open the file
C
      OPEN(DISK,FILE=PIENAME,STATUS='OLD',ERR=220)
      GO TO 230
C
220   WRITE(USEROUT,*)'*** ERROR: unable to find pie file ',
     &	PIENAME( : LNBLNK(PIENAME)),BELL
			STOP
C
C  Check for appropriate image type and pie file type:
C     Image type 1000 must have pie file type 3 or 4 (old reconstruction);
C     Image type 1020 must have pie file type 4 or 5 (new reconstruction);
C     Image type 2000 must have pie file type 6 (EM reconstruction);
C     Image type 3000 must have pie file type 7 (EM reconstruction);
C
8000	FORMAT(I1)
230   READ (DISK,8000,ERR=555,END=555) ITYPE
      IF (ITYPE.LT.3 .OR. ITYPE.GT.7) THEN
        WRITE(USEROUT,*)'*** ERROR: Pie File Type ',ITYPE,
     2    ' is not compatable with this program.',BELL
        GO TO 555
      END IF
      IF (IMGTYPE.EQ.1000 .AND. (ITYPE.NE.3 .AND. ITYPE.NE.4)) THEN
        WRITE(USEROUT,*)'*** ERROR: Pie File Type ',ITYPE,
     2    ' is not compatable with Image Type ',IMGTYPE,BELL
        GO TO 555
      ELSE IF (IMGTYPE.EQ.1020 .AND. (ITYPE.NE.5 .AND. ITYPE.NE.4)) THEN
        WRITE(USEROUT,*)'*** ERROR: Pie File Type ',ITYPE,
     2    ' is not compatable with Image Type ',IMGTYPE,BELL
        GO TO 555
      ELSE IF (IMGTYPE.EQ.2000 .AND. ITYPE.NE.6) THEN
        WRITE(USEROUT,*)'*** ERROR: Pie File Type ',ITYPE,
     2    ' is not compatable with Image Type ',IMGTYPE,BELL
        GO TO 555
      ELSE IF (IMGTYPE.EQ.3000 .AND. ITYPE.NE.7) THEN
        WRITE(USEROUT,*)'*** ERROR: Pie File Type ',ITYPE,
     2    ' is not compatable with Image Type ',IMGTYPE,BELL
        GO TO 555
      END IF
C
8500  FORMAT(A80)
8510  FORMAT('> ',A)
      READ(DISK,8500) PHEADER
      WRITE(USEROUT,8510) PHEADER( : LNBLNK(PHEADER))
C
8520	FORMAT(I2)
8530	FORMAT(F8.4,F11.4)
      READ(DISK,8520) NPIES
      IF (ITYPE.EQ.3 .OR. ITYPE.GE.5 .OR. (ITYPE.EQ.4 .AND. IMGTYPE.EQ.1000)) THEN
        DO 270 J=1,NPIES
          READ(DISK,8530,ERR=555) PSLOPES(J),BNKPAIRS(J)
270     CONTINUE
      ELSE IF (ITYPE.EQ.4 .AND. IMGTYPE.EQ.1020) THEN
        DO 280 J=1,NPIES
          READ(DISK,*,ERR=555) DUMMY,BNKPAIRS(J),PSLOPES(J)
280     CONTINUE
      ELSE
        GO TO 555
      END IF
      IF (ITYPE.EQ.4 .OR. ITYPE.EQ.5 .OR. ITYPE.EQ.6) THEN
        READ(DISK,8900,ERR=333,END=333) PDATE,PMEAN
        GO TO 340
333     WRITE(USEROUT,*)'Sensitivity scaler set to 1.0'
        PMEAN = 1.0
        PDATE = ' '
340     CONTINUE
      END IF
8900  FORMAT(A8,3X,F7.2)
      WRITE(USEROUT,*)'PIE file read = ',
     &	PIENAME( : LNBLNK(PIENAME))
      CLOSE(DISK)
      RETURN
C
555   WRITE(USEROUT,*)'*** ERROR: Unable to retrieve data from file ',
     &	PIENAME( : LNBLNK(PIENAME)),BELL
      CLOSE(DISK)
			STOP
C
      END
