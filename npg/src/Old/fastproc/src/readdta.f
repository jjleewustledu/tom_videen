c $Log: readdta.f,v $
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
C  Subroutine:   READDTA.FTN
C
C  Author:       Tom O. Videen
C  Date:         12-Jun-87
C  Written for:  METPROC
C
C	History:
C			Modified for UNIX 14-Jan-92 TOV:
C				1) FORMAT 8000 changed to I1,1X
C
C  Intent:
C     This subroutine reads the values for a single blood curve
C     from a .DTA (blood curve) file and restores decay to the
C     activity values.
C
C  Variable Passed:
C     DTAFILE - logical unit assigned to .DTA file;
C
C  BLOOD Variables (common block):
C     TIME = time of blood samples (sec);
C     ACTIVITY = activity of blood samples (counts/(sec*ml));
C     HEMATOCRIT = hematocrit;
C     OXYCONT = oxygen content of blood (ml/ml);
C     NUMPNTS = no. of points;
C     FILETYPE = version of DTA/DAT file (1-5);
C
C  SCANS Variables (common block):
C     NTYPE = type;
C     SCANID = ID used at end of scan file name;
C     SCANST = start time of scan (sec);
C     SCANEND = end time of scan (sec) -- computed;
C     SCANLEN = scan length (sec);
C     PEAKBANK = peak bank pair observed during scan;
C     OLDO15  - decay constant used in BLOOD FOR O15;
C
C  No Subroutines
C
C  Called by:  METPROC
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE READDTA(DTAFILE)
C
      INCLUDE 'metproc.inc'
C
      INTEGER*4    DTAFILE,I
      REAL         FILETAU
			CHARACTER*256 RCSHEADER
C
			RCSHEADER = "$Header: /home/petsun2/tom/src/fastproc/src/RCS/readdta.f,v 1.2 1992/11/02 16:02:53 tom Exp $"
C
8000  FORMAT(I1,1X,A4)
      READ(DTAFILE,8000) NTYPE,SCANID
C
      IF (NTYPE.LE.3) THEN
        TAU = TAUO15
        FILETAU = OLDO15
      ELSE IF (NTYPE.EQ.4) THEN
        TAU = TAUC11
        FILETAU = TAU
      ELSE IF (NTYPE.EQ.5) THEN
        TAU = TAUF18
        FILETAU = TAU
      ELSE IF (NTYPE.LE.9) THEN
        TAU = TAUC11
        FILETAU = TAU
      ELSE
        TAU = TAUGA68
        FILETAU = TAU
        NTYPE = 3
      END IF
C
      READ(DTAFILE,*) SCANST,SCANLEN
      SCANEND = SCANST+SCANLEN
      IF (FILETYPE.LT.3) THEN
        READ(DTAFILE,*) OXYCONT(1)
      ELSE
        READ(DTAFILE,*) PEAKBANK
        READ(DTAFILE,*) OXYCONT(1),HEMATOCRIT
      END IF
      READ(DTAFILE,*) NUMPNTS
      DO 100 I=1,NUMPNTS
        READ(DTAFILE,*) TIME(I),ACTIVITY(I)
        ACTIVITY(I) = ACTIVITY(I)*EXP(-TIME(I)*FILETAU)
100   CONTINUE
C
      RETURN
      END
