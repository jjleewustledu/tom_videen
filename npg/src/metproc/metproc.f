c $Log: metproc.f,v $
c Revision 2.32  2010/12/30 20:52:42  tom
c *** empty log message ***
c
c Revision 2.31  2008/09/04  14:59:31  tom
c no change
c
c Revision 2.29  2007/08/15  18:50:17  tom
c added getpie_962
c
c Revision 2.28  2007/03/21  21:17:56  tom
c get initials from shell
c eliminate option to reprocess hdr files
c
c Revision 2.27  2005/08/08  16:27:56  tom
c add pie Aug 2005
c
c Revision 2.26  2004/11/03  15:31:15  tom
c prompt with ho1 image
c
c Revision 2.25  2004/11/03  14:20:17  tom
c add getpie_961
c
c Revision 2.24  2004/11/02  16:37:24  tom
c select 961 pies from scandate, software version, acquisition
c
c Revision 2.23  2004/11/02  16:35:51  tom
c *** empty log message ***
c
c Revision 2.22  2004/11/01  20:02:27  tom
c select 961 pies by date, sw_version, and acquisition type
c
c Revision 2.21  2004/07/08  14:41:37  tom
c modify getpie for 961 versions
c
c Revision 2.20  2004/02/17  14:13:40  tom
c Feb 2004
c
c Revision 2.18  2002/12/02  17:08:36  tom
c modifications for gcc
c
c Revision 2.17  2001/06/01  18:48:41  tom
c ntype = 7 for steady-state oxygen
c
c Revision 2.16  1999/12/16  15:27:47  tom
c clean up
c
c Revision 2.15  1999/12/15  22:49:03  tom
c format fix
c
c Revision 2.14  1999/12/15  22:41:18  tom
c y2k and lint clean
c
c Revision 2.13  1998/07/15  16:52:25  tom
c increased array sizes in metproc.inc
c
c Revision 2.12  1997/03/19  20:20:07  tom
c fix getimagetype.c   getimginfo.c
c
c Revision 2.11  1997/03/10  17:15:28  tom
c use firstimgfrm in getimagetype and getimginfo
c
c Revision 2.10  1997/03/10  16:18:32  tom
c read frame 1,2, or 3
c
c Revision 2.9  1997/01/20  19:00:46  tom
c appropriate factor for JMO 3D reconstructed images
c
c Revision 2.8  1996/10/29  22:19:37  tom
c change N2=60 for default
c
c Revision 2.7  1996/10/29  19:27:43  tom
c permit adjustment of calibration using hand-drawn sample
c
c Revision 2.6  1996/10/16  16:52:28  tom
c new libimage
c
c Revision 2.5  1996/10/14  20:55:02  tom
c solaris
c
c Revision 2.4  1995/12/20  04:33:22  tom
c 961 version
c
c Revision 2.3  1993/10/15  17:51:17  tom
c fix bug in get_recon_type
c
c Revision 2.2  1993/10/15  16:33:22  tom
c change tive-activity arrays from 200 to 1000
c
c Revision 2.1  1993/07/15  20:25:17  tom
c Corrected error in pie adjustment for no scatter-correction
c Changed RCSHEADER to ID
c
c Revision 2.0  1993/07/12  23:13:24  tom
c Handles ECAT image format
c
c Revision 1.5  1993/07/12  22:48:11  tom
c Revised to handle ECAT images
c PIE factors are adjusted for the type of reconstruction and acquisition
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
C  Program:      METPROC.FTN
C  Author:       Tom O. Videen
C  Date:         12-Jun-87
C  Intent:
C     This program reads .DTA (blood curve) files, computes metabolic
C     conversion constants, and writes these plus certain parameters
C     from the scan image file (.IMG or .CMG) to a header file (.HDR)
C     associated with the image.  The specific values written to the
C     HDR file are documented in WRITEHDR.
C     These values in the .HDR file are necessary for SPIDERAM to
C     display metabolic images instead of merely PET numbers.
C
C  History:
C     Revision of METEVAL.FTN written by Mark Mintun.
C     Modified 13-Aug-87 by TOV to read write-protected files.
C     Modified 10-Sep-87 by TOV to search for PIE files on DT1 if
C        they are not found on the user's volume when no volume
C        name is specified for the PIE file.
C     Typo in printout from WRITERCD fixed 02-May-88 by TOV.
C     Modified 24-May-88 by TOV to eliminate the need for a redundant
C        blood curve time in processing oxygen (O2UTIL.FTN).
C     Modified 10-Oct-88 by TOV to read PIE file types 4 and 5 (as well
C        as 3) and to check for correct pie based on IMGTYPE
C        (PIEFILE, GETPIE, SAVEPIE)
C        The header from 1 image file must be read before reading a pie file.
C        (METPROC).  IMGTYPE is assigned 1000 or 1020 based on this header
C        and is checked against all other image files during processing.
C        Write version date in output file (WRITERCD).
C        Decay constant of O15 entered correctly as 0.005668 instead of
C        0.005568 as it was entered on 31-Mar-88.
C     Modified 13-Oct-88 by TOV to allow minimum bank pairs to be 0.1 in
C        INTERPIE.
C     Modified 07-Dec-88 by TOV:
C        1) fix minor bugs in PIEFILES, SAVEPIE, and WRITEHDR;
C        2) alter BLOODFLW to write flow vs. PETT COUNTS;
C        3) user may now select maximum flow used to fit curve in BLOODFLW;
C     Modified 27-Mar-89 by TOV:
C        1) user has option of correcting for PETT VI efficiency changes
C           since the specified pie file:
C        2) reads NORM mean counts/bin/minute from scan file header for
C           PETT VI (NORMMEAN)
C        3) reads NORM mean counts/bin/minute and scan date from pie file
C        4) decay corrects pie NORM mean to date of current scan (PIEMEAN)
C           and multiplies all factors by the ratio PIEMEAN/NORMMEAN.
C        5) write PIESLOPE and EFACTOR to HDR file.
C        Altered: METPROC.INC, METPROC, READHEAD, PIEFILE, GETPIE
C           GETFACTR, WRITERCD, INTERPIE, WRITEHDR
C        Add:  PIESCALE
C     Modified 06-Oct-89 by TOV to prevent error it PIEMEAN and PIEDATE
C        do not exist at the end of the pie file; EFACTOR set to 1.0.
C        Altered:  GETPIE, METPROC
C     Modified 25-Jan-91 by TOV to read pie file type 6 and image type 2000.
C     Modified 08-Oct-91 by TOV to correct reading of P8FILTER.
C        Altered:  READHEAD
C
C         Modified for UNIX by TOV 14-Jan to 14-Feb-92:
C            USERIN = 5; USEROUT = 6 (defaults)
C            All files opened internally by name. 
C               PIENAME, DTANAME, RCDNAME, IMGNAME, HDRNAME 
C            Add subroutines: GETARG, GETCWD, ieee2pe
C            Eliminate TERMSET, TCONTROL
C            Changes in:   BLOODFLW, GETIMAGE, GETPIE, INTCONV, METFILE, METPROC
C               READDTA, READHEAD, SAVEPIE, WRITEHDR, WRITERCD, METPROC.INC
C         Modified 12-May-92 by TOV 
C            1) to incorporate libpetutil and libpe.
C                  Changes in:   Makefile, WRITEHDR, READHEAD
C                  Use GETIMG from libpetutil.
C            2) changed remaining FNAME to IMGNAME
C                  Changes in: METFILE, READHDR
C         Modified 28-Oct-92 by TOV
C            1) read dta, DTA, or DAT files (in that order)
C            2) read ECAT pie files (type 7) (getpie.f)
C            3) change extension RCD to rcd
C            4) Default extension for hdrfile is 'hdr' (HDREXT).
C            5) Added HDREXT to call to METFILE (metfile.f)
C         Modified 19-May-93 by TOV
C            1) read ECAT pie files (type 7) (getpie.f)
C            2) user must enter pie file extension (getpie.f)
C         Modified 12-Jul-93 by TO
C            1) read ECAT image files and correct pie according to type of image
C            2) eliminate PIEFILE routine with SAVEPIE option
C            3) eliminate call uppercase(piename)
C         Modified 15-Oct-93 by TOV
C            1) time activity curves of 1000 points ok (metproc.inc)
C            2) fix bug in get_recon_type for scan_month
C
C  Main Subroutines:
C     READDTA  - read blood curve (.DTA) file;
C     METFILE  - open a scan file and set HDRNAME;
C     READTYPE - read selected paramters from scan file header;
C     GETFACTR - get pie slope and partition coefficient;
C     GETPIE   - read PIE files;
C     INTERPIE - get pie slope from interpolation of PIE file values;
C     PIESCALE - compute PETT efficiency factor (EFACTOR) of PIE rel. to scan;
C     O2UTIL   - oxygen utilization model;
C     BLOODFLW - blood flow model;
C     BLOODVOL - blood volume model;
C     INTEGRAT - integrate a function;
C     READHDR  - read scan parameters from an .HDR file;
C     WRITEHDR - create .HDR file;
C     WRITERCD - write record (.RCD) file;
C
C  libpe
C     GTSTRING
C     YESNO
C
C   Sun FORTRAN
C      GETARG
C      GETCWD
C      LNBLNK
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      PROGRAM METPROC

      INTEGER*4    DTAFILE   ! file descriptor (lu) of .DTA file
      INTEGER*4    RCDFILE   ! file descriptor (lu) of .RCD file
      INTEGER*4    DISK      ! file descriptor (lu) for image, PIE, and HDR files
      INTEGER*4    NPIES     ! number of PSLOPES/BNKPAIRS
      INTEGER*4    I,L,N
      INTEGER*4    IMGTYPE
      INTEGER*4    N1, N2, LAST
      REAL*4       EFFSCALER, MEANHAND, FITABS, SUM, SLOPE, INTERCEPT, COR
      REAL*4       PSLOPES(10)   ! pie slopes read from PIE file
      REAL*4       BNKPAIRS(10)  ! bank pairs read from PIE file
      REAL*4       EFACTOR
      REAL*4       NORM1,NORM2
      REAL*4       PMEAN
      LOGICAL*1    PEAKUSED   ! true--> user enters peak of Bank Pairs and
C                                       program estimates the mean;
C                               false--> user enters mean Bank Pairs;
      LOGICAL*1    SKIPSCAN   ! true--> do not process the scan;
C                               false--> process the scan;
      LOGICAL*1    IOPTION    ! true--> Pie Slope is computed internally by
C                               reading a PIE file and interpolating peak bank pairs;
      INTEGER*4    ERRCODE
      LOGICAL*1    REPROCES   ! true--> reprocess existing HDR files
C                               false-->get scan parameters from original scan files;

      LOGICAL*1    ADJUST     ! true--> adjust pie factor
      LOGICAL*1    A

      CHARACTER*3   MON
      CHARACTER*3   HDREXT
      CHARACTER*3   FSTATUS   ! file status (OLD or NEW)
      CHARACTER*4   FILELABL  ! 4 characters at the beginning of a DTA file
C               which are used to check agreement with the current program version;
      CHARACTER*8   DATE1, PDATE
      CHARACTER*10  DATE2
      CHARACTER*11  VERSION
      CHARACTER*80  Q
      CHARACTER*256 STR
      CHARACTER*256 RCSHEADER

      CHARACTER*256 HDRNAME   ! name of .HDR file
      CHARACTER*256 DTANAME   ! name of .DTA file
      CHARACTER*256 RCDNAME   ! name of .RCD file

      INCLUDE 'metproc.inc'

      DATA DTAFILE, RCDFILE, DISK /1,2,3/
      DATA USERIN, USEROUT /5,6/
      DATA SCANTYPE(1) /'Oxygen: Mintun Bolus                 '/
      DATA SCANTYPE(2) /'Blood Flow (H2O)                     '/
      DATA SCANTYPE(3) /'Blood Volume (CO)                    '/
      DATA SCANTYPE(4) /'Blood Flow (Butanol)                 '/
      DATA SCANTYPE(5) /'Spiperone                            '/
      DATA SCANTYPE(6) /'Glucose                              '/
      DATA SCANTYPE(7) /'Oxygen: Steady-State                 '/
      DATA SCANTYPE(8) /'Oxygen: Steady-Inhalation Plasma     '/
      DATA SCANTYPE(9) /'Oxygen: Steady-Inhalation Whole-Blood'/
      DATA VERSION /'15-Aug-2007'/
C -------------------------------------------------------------------
      DATA TAUO15    /0.005668/
      DATA TAUO1502  /0.005592/
      DATA TAUO1501  /0.005677/
      DATA TAUO1500  /0.00568033/
      DATA TAUC11    /0.0005668/
      DATA TAUGA68   /0.0001691/
      DATA TAUF18    /0.0010529/
      DATA HOLAMBDA  /0.95/
      DATA BULAMBDA  /0.81/
      DATA BRAINDEN  /1.05/
      DATA BLOODDEN  /1.06/
      DATA SLVRATIO  /0.85/
      DATA RBCFACTOR /0.766/

C  Source for decay constants: first listed halflife for each in
C     Lederer & Shirley, Table of Isotopes, 1978 (7th edition), Wiley.
C     (same no. of significant figures as halflives in this source)
C  Except for O15 where the decay constant was measured using six 20 min
C     decay curves by Mike Welch taken Jan. 1988.  Curves were fit by
C     Tom Videen.
C  Source for HOLAMBDA,BRAINDEN,BLOODDEN,RBCFACTOR:
C     Herscovitch & Raichle (1985) J. Cereb Blood Flow Metabol, 5:65
C  Source for SLVRATIO:
C     Grubb, Raichle, Higgins & Eichling (1978) Annals of Neurology, 4:322
C -------------------------------------------------------------------
C  Initialize constants

      RCSHEADER = "$Id: metproc.f,v 2.32 2010/12/30 20:52:42 tom Exp $"
      HDREXT = 'hdr'
      IMGTYPE = 0           ! avoid compiler warning
      EFACTOR = 1.0

      REPROCES = .FALSE.
      ADJUST = .FALSE.
      PIEMEAN = 1.0
      N2 = 60
8100  FORMAT (1X,A,A,A)
      WRITE(USEROUT,*) '>>> METABOLIC PROCESSING  (',VERSION,')'
      WRITE(USEROUT,*) 
      CALL GETENV('PWD',PATH)
      CALL GETARG(1,SUBJECT)
      L = LNBLNK(SUBJECT)
      CALL GETARG(2,STR)
      INITIALS = STR(1:4)
      WRITE(USEROUT,8105) PATH(: LNBLNK(PATH)), SUBJECT, INITIALS
8105  FORMAT('Current Directory = ',A,/,'Subject ID = ',A,/,'Your ID = ',A)
      DO 10 I=1,7
         COUNT(I) = 1
10    CONTINUE

C  Open DTA and RCD files

      DTANAME = SUBJECT(1:L)//'.dta'
      FSTATUS = 'OLD'
      OPEN(DTAFILE,FILE=DTANAME,STATUS='OLD',ERR=15)
      GO TO 30
15    DTANAME = SUBJECT(1:L)//'.DTA'
      OPEN(DTAFILE,FILE=DTANAME,STATUS='OLD',ERR=20)
      GO TO 30
20    DTANAME = SUBJECT(1:L)//'.DAT'
      OPEN(DTAFILE,FILE=DTANAME,STATUS='OLD',ERR=25)
      GO TO 30
25    WRITE(USEROUT,8110) SUBJECT(1:L), SUBJECT(1:L)
      STOP
30    WRITE(USEROUT,8115) DTANAME(: LNBLNK(DTANAME))

8110   FORMAT('Neither blood curve file ',A,'.dta ',A,'.DTA nor ',A,'.DAT exists!')
8115   FORMAT('Reading blood curve from ',A)

      RCDNAME = SUBJECT(1:L)//'.rcd'
      OPEN(RCDFILE,FILE=RCDNAME,ERR=35)
      GO TO 40
35    WRITE(USEROUT,8120) RCDNAME(: LNBLNK(RCDNAME))
      STOP
40    WRITE(USEROUT,8125) RCDNAME(: LNBLNK(RCDNAME))
8120  FORMAT('Cannot open file ',A)
8125  FORMAT('Log of processing will be written to ',A)

C  Obtain initials of USER

C      STR = ' '
C      Q = 'Enter your initials (max=4)'
C      CALL GTSTRING(Q,STR)
C      INITIALS = STR(1:4)

C  Reprocess existing HDR files

C      WRITE(USEROUT,*) 'WARNING:  If you REPROCESS HDR files you must be certain that all'
C      WRITE(USEROUT,*) 'images were processed with the same reconstruction stream.'
C      WRITE(USEROUT,*) 'If they were not, the wrong pie factor may be used.'
C      WRITE(USEROUT,*)
C      Q = 'Reprocess existing HDR files without reading scan files'
C      CALL YESNO(Q,REPROCES)

C  Get default volume and extension

      IF (REPROCES) THEN
         Q = 'Directory path for HDR files'
         STR = PATH(: LNBLNK(PATH))
         CALL GTSTRING(Q,STR)
         PATH = STR(: LNBLNK(PATH))
         EXTENSION = HDREXT
      ELSE
         Q = 'Directory path for scan files'
         STR = PATH(: LNBLNK(PATH))
         CALL GTSTRING(Q,STR)
         PATH = STR(: LNBLNK(PATH))
         WRITE(USEROUT,*) 'Extension may be up to 10 characters but MUST '//
     &      'include the period if there is one'
         Q = 'Extension for scan files (i.e., .img, .v, _g3.v, or blank)'
         STR = ' '
         CALL GTSTRING(Q,STR)
         EXTENSION = STR
      END IF

C  Read PIE file

      IOPTION = .TRUE.
      PEAKUSED = .TRUE.
45    Q = 'Let the program compute the Pie Slope'
      CALL YESNO(Q,IOPTION)
      IF (IOPTION) THEN

C  Must get RCONTYPE (image type) before reading a pie file

50       Q = 'Enter the name of an image file you will be processing (without extension)'
         STR = SUBJECT(1:L)//'ho1'
         CALL GTSTRING(Q,STR)
         IMGNAME = STR
         IF (.NOT.REPROCES) THEN
            IF (STR(1:1).NE.'/') THEN
               IMGNAME = PATH(: LNBLNK(PATH))//'/'//STR(: LNBLNK(STR))//
     &                EXTENSION(: LNBLNK(EXTENSION))
            ELSE
               IMGNAME = STR(: LNBLNK(STR))//EXTENSION(: LNBLNK(EXTENSION))
            END IF
         END IF

         CALL READTYPE (ERRCODE)
         IMGTYPE = RCONTYPE
         IF (ERRCODE.LT.0) THEN
            WRITE(USEROUT,*) '>>> ERROR: Cannot read ',IMGNAME(:LNBLNK(IMGNAME)),' [',ERRCODE,']'
            GO TO 50
         ELSE IF (IMGTYPE.NE.1000 .AND. IMGTYPE.NE.1020 .AND. IMGTYPE.NE.3071
     &        .AND. IMGTYPE.NE.2000 .AND. IMGTYPE.NE.3000 .AND. IMGTYPE.NE.3070) THEN
            WRITE(USEROUT,*)'>>> ERROR: Unknown image type ',IMGTYPE
            GO TO 50
         END IF

C  For images reconstructed with new stream on PETT VI:
C  If image file does not contain NORM mean, try an attenuation image
C  (in order to get an efficiency scaler)

         IF ((IMGTYPE.EQ.1020 .OR. IMGTYPE.EQ.2000) .AND. NORMMEAN.LE.0.0) THEN
60          Q = 'Enter the name of an attenuation image'
            STR = ' '
            CALL GTSTRING(Q,STR)
            IMGNAME = STR
            IF (STR(1:1).NE.'/') THEN
               IMGNAME = PATH(: LNBLNK(PATH))//'/'//STR(: LNBLNK(STR))
            END IF

            CALL READTYPE (ERRCODE)
            IF (ERRCODE.LT.0) THEN
               WRITE(USEROUT,*) '>>> ERROR: Cannot read ',IMGNAME(:LNBLNK(IMGNAME)),' [',ERRCODE,']'
               GO TO 60
            ELSE IF (RCONTYPE.NE.1010) THEN
               WRITE(USEROUT,*)'>>> ERROR: Image type ',RCONTYPE,' is not a new attenuation image'
               GO TO 60
            END IF
         END IF

         IF (IMGTYPE.EQ.3071) THEN
            CALL GETPIE_962(PSLOPES, BNKPAIRS)
         ELSE IF (IMGTYPE.EQ.3070) THEN
            CALL GETPIE_961(PSLOPES, BNKPAIRS)
         ELSE
            CALL GETPIE(NPIES, PSLOPES, BNKPAIRS, DISK, PIENAME, IMGNAME, IMGTYPE, PMEAN, PDATE)
            IF (PIENAME.EQ.' ') GO TO 45            

            PIEMEAN = PMEAN
            PIEDATE = PDATE
   
            IF (RCONTYPE .GE. 3000) THEN
               EFACTOR = 1.0
            ELSE
               WRITE(USEROUT,*)
               WRITE(USEROUT,*) 'If you choose to enter PEAK bank pairs instead of a calculated mean,'
               WRITE(USEROUT,*) 'the program will estimate the mean.'
               WRITE(USEROUT,*) 'The program assumes your scans are of the usual length,'
               WRITE(USEROUT,*) 'and uses the following formula:'
               WRITE(USEROUT,*)
               WRITE(USEROUT,*) '  Mean Bank Pairs = Peak Bank Pairs x Factor'
               WRITE(USEROUT,*)
               WRITE(USEROUT,*)'   Scan Type     Usual Length     Factor'
               WRITE(USEROUT,*)'   ---------     ------------     ------'
               WRITE(USEROUT,*)'   H2O (CBF)        40 sec         0.9'
               WRITE(USEROUT,*)'   CO  (CBV)         5 min         0.59'
               WRITE(USEROUT,*)'   O2  (CMRO2)      40 sec         0.9'
               WRITE(USEROUT,*)
               Q = 'Are you using PEAK Bank Pairs (and NOT calculated means)'
               CALL YESNO(Q,PEAKUSED)

C  If PIEMEAN=1, no info was available in pie file for efficiency correction

               IF (PIEMEAN.EQ.1.0) THEN
                  EFACTOR = 1.0
                  WRITE(USEROUT,*) 'PETT efficiency factor set to 1.0'
               ELSE IF (IMGTYPE.NE.1020 .AND. IMGTYPE.NE.2000) THEN
                  EFACTOR = 1.0
                  WRITE(USEROUT,*) 'PETT efficiency factor set to 1.0'
               ELSE
                  DATE1 = PIEDATE
                  DATE2 = SCANDATE
                  NORM1 = PIEMEAN
                  NORM2 = NORMMEAN
                  CALL PIESCALE(NORM1,NORM2,DATE1,DATE2,EFACTOR)
                  IF (EFACTOR.LT.0.1) THEN
                      EFACTOR = 1.0
                      WRITE(USEROUT,*) 'PETT efficiency factor set to 1.0'
                  ELSE
                     WRITE(USEROUT,*)
                     WRITE(USEROUT,*) 'PETT efficiency of PIE relative to scan = ',EFACTOR
                     Q = 'Multiply pie factors by this efficiency'
                     A = .TRUE.
                     CALL YESNO(Q,A)
                     IF (.NOT.A) EFACTOR = 1.0
                  END IF
               END IF
            END IF
         END IF
      END IF

8300  FORMAT(A4,A50)
      READ(DTAFILE,8300) FILELABL,HEADER
      IF (FILELABL.EQ.'@01@') THEN
         FILETYPE = 4
         OLDO15 = TAUO1501
         READ(DTAFILE,*)
         READ(DTAFILE,*)
      ELSE IF (FILELABL.EQ.'@02@') THEN
         FILETYPE = 5
         OLDO15 = TAUO1502
         READ(DTAFILE,*)
         READ(DTAFILE,*)
      ELSE IF (FILELABL.EQ.'@ @@') THEN
         FILETYPE = 3
         OLDO15 = TAUO1500
      ELSE IF (FILELABL.EQ.'@@@@') THEN
         FILETYPE = 2
         OLDO15 = TAUO1500
      ELSE
         MON = FILELABL(1:3)
         IF (MON.EQ.'JAN' .OR. MON.EQ.'FEB' .OR. MON.EQ.'MAR') GO TO 100
         IF (MON.EQ.'APR' .OR. MON.EQ.'MAY' .OR. MON.EQ.'JUN') GO TO 100
         IF (MON.EQ.'JUL' .OR. MON.EQ.'AUG' .OR. MON.EQ.'SEP') GO TO 100
         IF (MON.EQ.'OCT' .OR. MON.EQ.'NOV' .OR. MON.EQ.'DEC') GO TO 100
         WRITE(USEROUT,*) '>>> ERROR:  This is not a blood curve file',BELl
         STOP
100      FILETYPE = 1
         OLDO15 = TAUO1500
         READ(DTAFILE,*)
         READ(DTAFILE,*) N
         DO 200 I=1,(N*2)+6
            READ(DTAFILE,*)
200      CONTINUE
      END IF
      READ(DTAFILE,*) NUMSCANS

C ===================================================================
C  Loop for each scan

      DO 1000 N=1,NUMSCANS
         CALL READDTA(DTAFILE)
         IF (NTYPE.GT.9) THEN
            WRITE(USEROUT,*) '>>> ERROR: Unknown scan type=',NTYPE,' cannot be processed'
            GO TO 1000
         END IF

C  Check that the entire scan can be mapped into TIME

         IF (SCANEND.GT.TIME(NUMPNTS)) THEN
            WRITE(USEROUT,8400) SCANEND,TIME(NUMPNTS)
            STOP
         END IF
8400     FORMAT('>>> ERROR End of Scan,',F10.2,' is AFTER End of Curve,',F10.2)

C  If ntype=8, process this curve to get water counts and then get next curve (whole blood)

         IF (NTYPE.EQ.8) THEN
            CALL O2PLASMA
            WRITE(USEROUT,*) 'Processed scan type ',NTYPE,' as plasma samples'
            GO TO 1000
         END IF

C  Get parameters from the scan file header

         CALL METFILE(HDRNAME,SKIPSCAN,HDREXT)
         IF (SKIPSCAN) GO TO 1000
         IF (REPROCES) THEN
            CALL READHDR(HDRNAME,ERRCODE)
            IF (ERRCODE.LT.0) THEN
               WRITE(USEROUT,*) '>>> ERROR: Cannot read ',HDRNAME(:LNBLNK(IMGNAME)),' [',ERRCODE,']'
               GO TO 1000
            END IF
         ELSE
            CALL READTYPE (ERRCODE)
            IF (ERRCODE.LT.0) THEN
               WRITE(USEROUT,*) '>>> ERROR: Cannot read ',IMGNAME(:LNBLNK(IMGNAME)),' [',ERRCODE,']'
               GO TO 1000
            END IF
            IF (IOPTION .AND. RCONTYPE.NE.IMGTYPE) THEN
               WRITE(USEROUT,*) '>>> ERROR: Pie factors for type: ',IMGTYPE
               WRITE(USEROUT,*) '>>>        Image file is type:   ',RCONTYPE
               GO TO 1000
            END IF
         END IF

C  Get pie slope and partition coefficient

         CALL GETFACTR (IOPTION,PEAKUSED,NPIES,PSLOPES,BNKPAIRS,EFACTOR)

C  Check calibration

         EFFSCALER = EFACTOR
         Q = 'Adjust pie factor'
         CALL YESNO(Q,ADJUST)
         IF (ADJUST) THEN
            LAST = NUMPNTS
            DO WHILE (CORACTIV(LAST) .GT. 0)
               LAST = LAST-1
            END DO
            N1 = NUMPNTS - LAST 
            DO WHILE (CORACTIV(LAST) .EQ. 0)
               LAST = LAST-1
            END DO
            Q = 'Number of hand-drawn points'
            CALL GETINT(Q,N1,1,10)
            SUM = 0.
            DO 500 I=1,N1
               SUM = SUM + CORACTIV(NUMPNTS-I+1)
500         CONTINUE
            MEANHAND = SUM / FLOAT(N1)
501         Q = 'Number of automated-sampled points'
            CALL GETINT(Q,N2,1,1000)
            SUM = 0.
            CALL LINFIT (INTERCEPT, SLOPE, LAST-N2+1, LAST, COR)
            FITABS = INTERCEPT + SLOPE * TIME(NUMPNTS)
            WRITE(USEROUT,*) " R = ",COR
            EFFSCALER = MEANHAND/FITABS
            Q = 'Blood sampler efficiency scaler'
            CALL GETREAL(Q,EFFSCALER,0.0,1000.0)
            IF (EFFSCALER .LE. 0) GOTO 501
            EFFSCALER = EFFSCALER * EFACTOR
            PETTCONV(1) = EFFSCALER * PETTCONV(1)
         END IF

C  Calculate the integral of the Blood Curve Activity

         CALL INTEGRAT(TIME,ACTIVITY,0.,SCANEND,TOTALCNTS)
         CALL INTEGRAT(TIME,ACTIVITY,SCANST,SCANEND,SCANCNTS)
         WRITE(USEROUT,*) 'Blood Counts:  (Non-Decay-Corrected)'
         WRITE(USEROUT,8500) 'Injection     = ',TOTALCNTS
         WRITE(USEROUT,8500) 'Start of Scan = ',SCANCNTS
8500     FORMAT(' Integrated Blood Curve from ',A,F15.1)

C  Get Metabolic Conversion Constants appropriate to the file

         IF (NTYPE.EQ.1) THEN
            CALL O2UTIL
         ELSE IF (NTYPE.EQ.2 .OR. NTYPE.EQ.4) THEN
            CALL BLOODFLW
         ELSE IF (NTYPE.EQ.3) THEN
            CALL BLOODVOL
         ELSE IF (NTYPE.EQ.7) THEN
            CALL O2STEADY
         ELSE IF (NTYPE.EQ.9) THEN
            CALL O2INHALATION
         END IF
         COUNT(NTYPE) = COUNT(NTYPE)+1

C  Write information to .RCD file

         CALL WRITERCD (N,RCDFILE,EFFSCALER,VERSION)

C  Create the HDR file

         CALL WRITEHDR (DISK,EFFSCALER,HDRNAME)

1000  CONTINUE
C ===================================================================
      STOP
      END
