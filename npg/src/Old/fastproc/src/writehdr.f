c $Log: writehdr.f,v $
c Revision 1.2  1992/11/02  16:02:53  tom
c Added RCS identifies
c
c Revision 1.3  1992/10/19  15:56:10  tom
c modified RCSHEADER length
c
c Revision 1.2  1992/10/16  22:32:51  tom
c added RCSHEADER
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   WRITEHDR.FTN
C
C  Author:       Tom O. Videen
C  Date:         14-May-87
C  Written for:  METPROC
C
C  History:
C     Modified 06-Dec-88 by TOV to correct bug in opening a non-default
C        HDR file.
C     Modified 27-Mar-89 by TOV to write PIESLOPE and EFACTOR to HDR file.
C
C			Modified for UNIX 6-Feb-92 TOV
C				RINDEX, LNBLNK used for string manipulation
C				HDRNAME changed to character*256 and passed
C				RECL=256 eliminated from OPEN
C				INTDATE replaced by IDATE
C				use ieee2pe to convert all real no's in header
C			Modified for fastproc 17-Feb-92 TOV to eliminate some user output.
C			Modified to run with libpetutil 11-May-92 TOV.
C
C  Intent:
C     This subroutine writes creates an .HDR file with metabolic
C     constants.  See HDRFORM for values in this file.
C
C  Variables Passed:
C     DISK = logical unit assigned for scan file I/O;
C
C  Variables:
C     OUT = array used to write HDR files;
C     HEAD* = values equivalenced with elements in OUT array header;
C     HDRNAME = name of HDR file;
C
C  Common Block Variables:
C
C  /USRIO/
C     USEROUT = logical unit for user output;
C
C  /BLOOD/
C     OXYCONT = oxygen content of blood (ml/ml);
C
C  /SCANS/
C     NTYPE = type of scan file;
C     SCANST = start time of scan (sec);
C     SCANEND = end time of scan (sec);
C     SCANLEN = scan length (sec);
C     FRAMELEN = frame length (non-zero for 14-slice scans);
C     TAU = decay constant used for specified file (1/sec);
C     TOTALCNTS = Integral of blood counts from time of injection;
C     SCANCNTS = Integral of blood counts from start of scan;
C     O2CNTS = integral of blood oxygen counts from start of scan;
C     PETTCONV = PETT conversion factor;
C        (well counts * pixel)/(PETT no * ml)
C     INITIALS = initials of user;
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
C
C  C function:
C			ieee2pe
C
C  TERMINAL.LIB
C     GTSTRING
C
C  FORTRAN Runtime Library
C     IDATE
C     
C  Called by:  METPROC
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE WRITEHDR(DISK,EFACTOR,HDRNAME)
C
      INTEGER*4    DISK,DATE(3),IOWRITE,NBYTES,L1,L2,I,RINDEX
      INTEGER*2    OUT(128),HEAD4,HEAD5,HEAD8,HEAD9,HEAD12,HEAD13
      INTEGER*4    HEAD15,HEAD16,HEAD17,HEAD18,HEAD19,HEAD20,HEAD21,
     2             HEAD22,HEAD23,HEAD24,HEAD25,HEAD26,HEAD27,HEAD28,
     3             HEAD29,HEAD30,HEAD31,HEAD32,HEAD33,HEAD34,HEAD35,
     4             HEAD36,HEAD37,HEAD38,HEAD39,HEAD40,HEAD41,HEAD42,
     5             HEAD43,HEAD44
			INTEGER*4		 ieee2pe
			REAL*4			 EFACTOR
  		CHARACTER*2	 DD,MM,YY
      CHARACTER*4  HEAD11
      CHARACTER*8  HEAD2,HEAD3,HEAD10,HEAD14,PROCDATE
			CHARACTER*10 HEAD6
      CHARACTER*12 HEAD7
      CHARACTER*14 HEAD1
      CHARACTER*256	HDRNAME
      CHARACTER*80 Q,STR
			CHARACTER*256 RCSHEADER
C
      EQUIVALENCE (OUT(1),  HEAD1),
     2            (OUT(8),  HEAD2),
     3            (OUT(12), HEAD3),
     4            (OUT(16), HEAD4),
     5            (OUT(17), HEAD5),
     6            (OUT(18), HEAD6),
     7            (OUT(23), HEAD7),
     8            (OUT(29), HEAD8),
     9            (OUT(30), HEAD9),
     A            (OUT(31), HEAD10)
      EQUIVALENCE (OUT(35), HEAD11),
     2            (OUT(37), HEAD12),
     3            (OUT(38), HEAD13),
     4            (OUT(39), HEAD14),
     5            (OUT(43), HEAD15),
     6            (OUT(45), HEAD16),
     7            (OUT(47), HEAD17),
     8            (OUT(49), HEAD18),
     9            (OUT(51), HEAD19)
      EQUIVALENCE (OUT(53), HEAD20),
     2            (OUT(55), HEAD21),
     3            (OUT(57), HEAD22),
     4            (OUT(59), HEAD23),
     5            (OUT(61), HEAD24),
     6            (OUT(63), HEAD25),
     7            (OUT(65), HEAD26),
     8            (OUT(67), HEAD27)
      EQUIVALENCE (OUT(69), HEAD28),
     2            (OUT(71), HEAD29),
     3            (OUT(73), HEAD30),
     4            (OUT(75), HEAD31),
     5            (OUT(77), HEAD32),
     6            (OUT(79), HEAD33),
     7            (OUT(81), HEAD34),
     8            (OUT(83), HEAD35),
     9            (OUT(85), HEAD36)
      EQUIVALENCE (OUT(87), HEAD37),
     2            (OUT(89), HEAD38),
     3            (OUT(91), HEAD39),
     4            (OUT(93), HEAD40),
     5            (OUT(95), HEAD41),
     6            (OUT(97), HEAD42),
     7            (OUT(99), HEAD43),
     8            (OUT(101),HEAD44)
C
			external ieee2pe !$pragma C (ieee2pe)
C
      INCLUDE 'metproc.inc'
      DATA IOWRITE,NBYTES  /56,256/
C
			RCSHEADER = "$Header: /home/petsun2/tom/src/fastproc/src/RCS/writehdr.f,v 1.2 1992/11/02 16:02:53 tom Exp $"
C
C  Open HDR file
C
      OPEN(DISK,FILE=HDRNAME,STATUS='OLD',ERR=110)
      WRITE(USEROUT,*) '*** Existing HDR file has been rewritten'
      CLOSE(DISK,STATUS='DELETE')
110   OPEN(DISK,FILE=HDRNAME,STATUS='NEW',ERR=120)
      GO TO 200
120   WRITE(USEROUT,*) '*** Unable to open file:  ',HDRNAME(:LNBLNK(HDRNAME))
      Q = 'File name to use (with extension)'
      STR = HDRNAME
      CALL GTSTRING(Q,STR)
      HDRNAME = STR
      GO TO 110
C
8000  FORMAT(I2)
200		CALL IDATE(DATE)
      WRITE(DD,8000) DATE(1)
      WRITE(MM,8000) DATE(2)
      WRITE(YY,8000) DATE(3) - ((DATE(3)/100)*100)
      PROCDATE = MM//'/'//DD//'/'//YY
C
      DO 300 I=1,128
        OUT(I) = 0
300   CONTINUE
C
C  Insert Coefficients into Header
C
      HEAD1 = SCANNER
      HEAD2 = SCANNAME
      HEAD3 = SCANDATE
      HEAD4 = SLICES
      HEAD5 = SCANTIME
      HEAD6 = COMPOUND
      HEAD7 = FILTER
      HEAD8 = INT(RCONTYPE)
      HEAD9 = INT(RESOLUTION)
C
      HEAD10 = PROCDATE
      HEAD11 = INITIALS
      HEAD12 = NTYPE
      HEAD13 = 0                  ! unused
C
      HEAD14 = ' '
			L1 = RINDEX(PIENAME,'/') + 1
			L2 = RINDEX(PIENAME,'.') - 1
			IF (L1.GT.1) THEN
				IF (L2.GT.L1) THEN
					HEAD14 = PIENAME(L1:L2)
				ELSE
					HEAD14 = PIENAME(L1:LNBLNK(PIENAME))
				END IF
			ELSE
				IF (L2.GT.0) THEN
					HEAD14 = PIENAME(1:L2)
				ELSE
					HEAD14 = PIENAME(1:LNBLNK(PIENAME))
				END IF
			END IF
C
      HEAD15 = ieee2pe(TOTALCNTS)
      HEAD16 = ieee2pe(SCANCNTS)
      HEAD17 = ieee2pe(SCANST)
      HEAD18 = ieee2pe(SCANLEN)
      HEAD19 = ieee2pe(FRAMELEN)
      HEAD20 = ieee2pe(TAU)
      HEAD21 = ieee2pe(PETTCONV(1))
      HEAD32 = ieee2pe(PETTCONV(2))
      IF (NTYPE.EQ.1) THEN
        HEAD25 = ieee2pe(AOXYGEN(1))
        HEAD26 = ieee2pe(BOXYGEN(1))
        HEAD27 = ieee2pe(AWATER(1))
        HEAD28 = ieee2pe(BWATER(1))
        HEAD29 = ieee2pe(O2CNTS(1))
        HEAD30 = ieee2pe(OXYCONT(1))
        HEAD36 = ieee2pe(AOXYGEN(2))
        HEAD37 = ieee2pe(BOXYGEN(2))
        HEAD38 = ieee2pe(AWATER(2))
        HEAD39 = ieee2pe(BWATER(2))
        HEAD40 = ieee2pe(O2CNTS(2))
        HEAD41 = ieee2pe(OXYCONT(2))
      ELSE IF (NTYPE.EQ.2 .OR. NTYPE.EQ.4) THEN
        HEAD22 = ieee2pe(AFLOW(1))
        HEAD23 = ieee2pe(BFLOW(1))
        HEAD33 = ieee2pe(AFLOW(2))
        HEAD34 = ieee2pe(BFLOW(2))
      ELSE IF (NTYPE.EQ.3) THEN
        HEAD24 = ieee2pe(BVFACTOR(1))
        HEAD35 = ieee2pe(BVFACTOR(2))
      ELSE IF (NTYPE.EQ.5 .OR. NTYPE.EQ.6) THEN
        HEAD31 = ieee2pe(PETTCONV(1)*EXP(TAU*0.5*(SCANST+SCANEND)))
        HEAD42 = ieee2pe(PETTCONV(2)*EXP(TAU*0.5*(SCANST+SCANEND)))
      END IF
      HEAD43 = ieee2pe(PIESLOPE)
      HEAD44 = ieee2pe(EFACTOR)
      CALL SYSIO(PB,IOWRITE,DISK,OUT,NBYTES,0)
C
      WRITE(USEROUT,*) 'HDR File = ',HDRNAME( : LNBLNK(HDRNAME))
      CLOSE(DISK)
C
      RETURN
      END
