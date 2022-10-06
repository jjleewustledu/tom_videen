c $Log: writehdr.f,v $
c Revision 2.3  1999/12/16  16:29:59  tom
c fix date bug
c
c Revision 2.2  1999/12/16  15:27:47  tom
c clean up
c
c Revision 2.1  1993/07/15  20:25:17  tom
c Changed RCSHEADER to ID
c
c Revision 2.0  1993/07/12  23:13:24  tom
c Handles ECAT image format
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
C  Author:       Tom O. Videen
C  Date:         14-May-87
C  Written for:  METPROC
C  Intent:
C     This subroutine writes creates an .HDR file with metabolic
C     constants.  See HDRFORM for values in this file.
C
C  History:
C     Modified 06-Dec-88 by TOV to correct bug in opening a non-default
C        HDR file.
C     Modified 27-Mar-89 by TOV to write PIESLOPE and EFACTOR to HDR file.
C     Modified for UNIX 6-Feb-92 TOV
C       RINDEX, LNBLNK used for string manipulation
C       HDRNAME changed to character*256 and passed
C       RECL=256 eliminated from OPEN
C       INTDATE replaced by IDATE
C       use ieee2pe to convert all real no's in header
C      Modified for fastproc 17-Feb-92 TOV to eliminate some user output.
C      Modified to run with libpetutil 11-May-92 TOV.
C      Modified May 2001 TOV for steady-state oxygen
C         add HOB2P, OOB2P, OOPLASMA to hdr
C         (HEAD45,HEAD46,HEAD47)
C
C  Variables Passed:
C     DISK = logical unit assigned for scan file I/O;
C
C  Variables:
C     OUT = array used to write HDR files;
C     HEAD* = values equivalenced with elements in OUT array header;
C     HDRNAME = name of HDR file;
C
C  Common Blocks:
C     /USRIO/
C     /BLOOD/
C     /SCANS/
C
C  Uses Subroutines:
C     ieee2pe
C     GTSTRING
C     IDATE
C     
C  Called by:  METPROC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE WRITEHDR(DISK,EFACTOR,HDRNAME)

      INTEGER*4     DISK,DATE(3),IOWRITE,NBYTES,L1,L2,I,RINDEX
      INTEGER*2     OUT(128),HEAD4,HEAD5,HEAD8,HEAD9,HEAD12,HEAD13
      INTEGER*4     HEAD15,HEAD16,HEAD17,HEAD18,HEAD19,HEAD20,HEAD21,
     &              HEAD22,HEAD23,HEAD24,HEAD25,HEAD26,HEAD27,HEAD28,
     &              HEAD29,HEAD30,HEAD31,HEAD32,HEAD33,HEAD34,HEAD35,
     &              HEAD36,HEAD37,HEAD38,HEAD39,HEAD40,HEAD41,HEAD42,
     &              HEAD43,HEAD44,HEAD45,HEAD46,HEAD47
      INTEGER*4     ieee2pe
      REAL*4        EFACTOR
      CHARACTER*2   DD,MM,YY
      CHARACTER*4   HEAD11
      CHARACTER*8   HEAD2,HEAD3,HEAD10,HEAD14,PROCDATE,SHORTDATE
      CHARACTER*10  HEAD6
      CHARACTER*12  HEAD7
      CHARACTER*14  HEAD1
      CHARACTER*256 HDRNAME
      CHARACTER*80  Q, STR
      CHARACTER*256 RCSHEADER

      EQUIVALENCE (OUT(1),  HEAD1),
     &            (OUT(8),  HEAD2),
     &            (OUT(12), HEAD3),
     &            (OUT(16), HEAD4),
     &            (OUT(17), HEAD5),
     &            (OUT(18), HEAD6),
     &            (OUT(23), HEAD7),
     &            (OUT(29), HEAD8),
     &            (OUT(30), HEAD9),
     &            (OUT(31), HEAD10)
      EQUIVALENCE (OUT(35), HEAD11),
     &            (OUT(37), HEAD12),
     &            (OUT(38), HEAD13),
     &            (OUT(39), HEAD14),
     &            (OUT(43), HEAD15),
     &            (OUT(45), HEAD16),
     &            (OUT(47), HEAD17),
     &            (OUT(49), HEAD18),
     &            (OUT(51), HEAD19)
      EQUIVALENCE (OUT(53), HEAD20),
     &            (OUT(55), HEAD21),
     &            (OUT(57), HEAD22),
     &            (OUT(59), HEAD23),
     &            (OUT(61), HEAD24),
     &            (OUT(63), HEAD25),
     &            (OUT(65), HEAD26),
     &            (OUT(67), HEAD27)
      EQUIVALENCE (OUT(69), HEAD28),
     &            (OUT(71), HEAD29),
     &            (OUT(73), HEAD30),
     &            (OUT(75), HEAD31),
     &            (OUT(77), HEAD32),
     &            (OUT(79), HEAD33),
     &            (OUT(81), HEAD34),
     &            (OUT(83), HEAD35),
     &            (OUT(85), HEAD36)
      EQUIVALENCE (OUT(87), HEAD37),
     &            (OUT(89), HEAD38),
     &            (OUT(91), HEAD39),
     &            (OUT(93), HEAD40),
     &            (OUT(95), HEAD41),
     &            (OUT(97), HEAD42),
     &            (OUT(99), HEAD43),
     &            (OUT(101),HEAD44)
      EQUIVALENCE (OUT(103),HEAD45),
     &            (OUT(105),HEAD46),
     &            (OUT(107),HEAD47)

      external ieee2pe !$pragma C (ieee2pe)

      INCLUDE 'metproc.inc'
      DATA IOWRITE,NBYTES  /56,256/
      RCSHEADER = "$Id: writehdr.f,v 2.3 1999/12/16 16:29:59 tom Exp tom $"

C  Open HDR file

      OPEN(DISK,FILE=HDRNAME,STATUS='OLD',ERR=110)
      WRITE(USEROUT,*) '>>> Replacing ',HDRNAME(:LNBLNK(HDRNAME))
      CLOSE(DISK,STATUS='DELETE')
110   OPEN(DISK,FILE=HDRNAME,STATUS='NEW',ERR=120)
      GO TO 200
120   WRITE(USEROUT,*) '>>> ERROR: Unable to open ',HDRNAME(:LNBLNK(HDRNAME))
      Q = 'File name to use (with extension)'
      STR = HDRNAME
      CALL GTSTRING(Q,STR)
      HDRNAME = STR
      GO TO 110

8000  FORMAT(I2)
200   CALL IDATE(DATE)
      WRITE(DD,8000) DATE(1)
      WRITE(MM,8000) DATE(2)
      WRITE(YY,8000) DATE(3) - ((DATE(3)/100)*100)
      PROCDATE = MM//'/'//DD//'/'//YY
      SHORTDATE = SCANDATE(1:6)//SCANDATE(9:10)

      DO 300 I=1,128
         OUT(I) = 0
300   CONTINUE

C  Insert Coefficients into Header

      HEAD1 = SCANNER
      HEAD2 = SCANNAME
      HEAD3 = SHORTDATE
      HEAD4 = SLICES
      HEAD5 = SCANTIME
      HEAD6 = COMPOUND
      HEAD7 = FILTER
      HEAD8 = INT(RCONTYPE)
      HEAD9 = INT(RESOLUTION)
      HEAD10 = PROCDATE
      HEAD11 = INITIALS
      HEAD12 = NTYPE
      HEAD13 = 0                  ! unused
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

      HEAD15 = ieee2pe(TOTALCNTS)
      HEAD16 = ieee2pe(SCANCNTS)
      HEAD17 = ieee2pe(SCANST)
      HEAD18 = ieee2pe(SCANLEN)
      HEAD19 = ieee2pe(FRAMELEN)
      HEAD20 = ieee2pe(TAU)
      HEAD21 = ieee2pe(PETTCONV(1))
      HEAD32 = ieee2pe(PETTCONV(2))
      IF (NTYPE.EQ.1 .OR. NTYPE.EQ.7) THEN
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
         HEAD46 = ieee2pe(OOB2P)
         HEAD47 = ieee2pe(OOPLASMA)
      ELSE IF (NTYPE.EQ.2 .OR. NTYPE.EQ.4) THEN
         HEAD22 = ieee2pe(AFLOW(1))
         HEAD23 = ieee2pe(BFLOW(1))
         HEAD33 = ieee2pe(AFLOW(2))
         HEAD34 = ieee2pe(BFLOW(2))
         HEAD45 = ieee2pe(HOB2P)
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
      WRITE(USEROUT,*) 'HDR File = ',HDRNAME( : LNBLNK(HDRNAME))
      CLOSE(DISK)
      RETURN
      END
