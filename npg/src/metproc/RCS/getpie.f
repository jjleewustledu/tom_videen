c $Log: getpie.f,v $
c Revision 2.1  1993/07/15  20:25:17  tom
c Changed RCSHEADER to ID
c
c Revision 2.0  1993/07/12  23:13:24  tom
c Handles ECAT image format
c
c Revision 1.4  1993/07/12  22:48:11  tom
c Revised to handle ECAT images
c PIE factors are adjusted for the type of reconstruction and acquisition
c
c Revision 1.3  1992/10/19  15:56:10  tom
c modified RCSHEADER length
c
c Revision 1.2  1992/10/16  22:32:51  tom
c added RCSHEADER
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   GETPIE.FTN
C
C  Author:       Tom O. Videen
C  Date:         14-May-87
C  Written For:  METPROC, ACTPROC, and PETPROC
C
C  Intent:
C     This subroutine reads PIE values from a disk file.
C
C  History:
C     Modified 13-Aug-87 by TOV to read write-protected files.
C     Modified 10-Sep-87 by TOV to search for PIE files on DT1 if
C        they are not found on the user's volume when no volume
C        name is specified for the PIE file.
C     Modified 10-Oct-88 by TOV to read PIE file types 4 and 5
C        and to check for correct pie based on IMGTYPE.
C     Modified 23-Mar-89 by TOV to read PIEMEAN and PIEDATE.
C     Modified 06-Oct-89 by TOV to prevent error it PIEMEAN and PIEDATE
C        do not exist at the end of the pie file.
C     Modified 25-Jan-91 by TOV to read pie file type 6 and image type 2000.
C
C			Modified for UNIX 12-May-92 TOV
C				LNBLNK used for string manipulation & to attach PIE extension
C				PIENAME changed to character*256
C			Modified 19-May-93 TOV
C				1) recognize image type 3000 and pie file type 7
C				2) require user to enter "pie" extension rather than force it to "PIE"
C			Modified 07-Jul-93 TOV
C				1) new function adjust_ecat_pie
C				2) pass IMGNAME for use by adjust_ecat_pie
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
C  Contains Common Block /USRIO/
C
C  Uses Subroutines:
C     GTSTRING
C     YESNO
C			adjust_ecat_pie
C
C  Called by:  PIEFILE and PETPROC
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE GETPIE(NPIES, PSLOPES, BNKPAIRS, DISK, PIENAME, IMGNAME, IMGTYPE, PMEAN, PDATE)
C
      INTEGER*4    USERIN, USEROUT
      INTEGER*4    NPIES, DISK, ITYPE, IMGTYPE, SLICE
      REAL*4       PSLOPES(10), BNKPAIRS(10), PIE_SLOPE, PMEAN
      CHARACTER*1  BELL
      CHARACTER*8  PDATE
      CHARACTER*256 PIENAME, IMGNAME
      CHARACTER*80 Q, STR, PHEADER
      LOGICAL*1    A, ERR
			CHARACTER*256 RCSHEADER
C
      COMMON /USRIO/ USERIN,USEROUT
C
			RCSHEADER = "$Id: getpie.f,v 2.1 1993/07/15 20:25:17 tom Exp $"
      PIENAME = ' '
      BELL = CHAR(7)
			SLICE = 1
			IF (IMGTYPE .EQ. 3000)	PIENAME = '/data/raw/953b/pie/930601sta.pie'
C
100   Q = 'Pie File Name (with extension)'
      STR = PIENAME( : LNBLNK(PIENAME))
      CALL GTSTRING(Q,STR)
      PIENAME = STR
      IF (LNBLNK(PIENAME).LE.0) RETURN
C
C  Open the file
C
      OPEN(DISK,FILE=PIENAME,STATUS='OLD',ERR=220)
      GO TO 230
C
C  Set default path (when one becomes available)
C
C210   L = IANYL(PIENAME,':')
C      IF (L.NE.0) GO TO 220
C      STR = PIENAME
C      PIENAME = 'DT1:'//STR(1:17)
C      OPEN(DISK,FILE=PIENAME,STATUS='OLD',ERR=220)
C      GO TO 230
C
220   WRITE(USEROUT,*)'*** ERROR: unable to find pie file ',
     &	PIENAME( : LNBLNK(PIENAME)),BELL
      GOTO 100
C
C  Check for appropriate image type and pie file type:
C     Image type 1000 must have pie file type 3 or 4 (old reconstruction);
C     Image type 1020 must have pie file type 4 or 5 (new reconstruction);
C     Image type 2000 must have pie file type 6 (EM reconstruction);
C     Image type 3000 must have pie file type 7 (ECAT);
C
230   READ (DISK,*,ERR=555,END=555) ITYPE
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
      READ(DISK,8500) PHEADER
      WRITE(USEROUT,*) PHEADER
      Q = 'Use this file'
      A = .TRUE.
      CALL YESNO(Q,A)
      IF (.NOT.A) THEN
        CLOSE(DISK)
        GOTO 100
      END IF
C
      READ(DISK,*) NPIES
      IF (ITYPE.EQ.3 .OR. (ITYPE.GE.5 .AND. ITYPE.LE.7) .OR.
     &   (ITYPE.EQ.4 .AND. IMGTYPE.EQ.1000)) THEN
        DO 270 J=1,NPIES
          READ(DISK,*,ERR=555) PSLOPES(J),BNKPAIRS(J)
270     CONTINUE
      ELSE IF (ITYPE.EQ.4 .AND. IMGTYPE.EQ.1020) THEN
        DO 280 J=1,NPIES
          READ(DISK,*,ERR=555) PIE_SLOPE,BNKPAIRS(J),PSLOPES(J)
280     CONTINUE
      ELSE
        GO TO 555
      END IF
      IF (ITYPE.GE.4 .AND. ITYPE.LE.7) THEN
        READ(DISK,8900,ERR=333,END=333) PDATE,PMEAN
        GO TO 340
333     WRITE(USEROUT,*) 'Sensitivity scaler set to 1.0'
        PMEAN = 1.0
        PDATE = ' '
340     CONTINUE
      END IF
8900  FORMAT(A8,3X,F)
      WRITE(USEROUT,*)'Pie data have been read from disk file ',PIENAME( : LNBLNK(PIENAME))
      CLOSE(DISK)
C
C  ECAT pies require adjustment for reconstruction type
C
			IF (IMGTYPE .EQ. 3000) THEN
				PIE_SLOPE = PSLOPES(1)
				CALL ADJUST_ECAT_PIE (PIE_SLOPE, IMGNAME, SLICE, DISK, USEROUT, ERR)
				IF (ERR) THEN
			  	WRITE(USEROUT,*)'*** ERROR reading file ',IMGNAME( : LNBLNK(IMGNAME)),BELL
				ELSE
					PSLOPES(1) = PIE_SLOPE
					PSLOPES(2) = PIE_SLOPE
				END IF
			END IF
      RETURN
C
555   WRITE(USEROUT,*)'*** ERROR: Unable to read data from ',
     &	PIENAME( : LNBLNK(PIENAME)),BELL
      CLOSE(DISK)
      GOTO 100
C
      END
