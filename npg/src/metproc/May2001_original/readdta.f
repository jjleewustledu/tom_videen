c $Log: readdta.f,v $
c Revision 2.2  1996/10/29  19:27:43  tom
c save corrected activity for linfit (metproc v 2.7)
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
C  Subroutine:   READDTA.FTN
C  Author:       Tom O. Videen
C  Date:          12-Jun-87
C  Written for:  METPROC
C  History:
C     Modified for UNIX 14-Jan-92 TOV:
C       1) FORMAT 8000 changed to I1,1X
C  Intent:
C     This subroutine reads the values for a single blood curve
C     from a .DTA (blood curve) file and restores decay to the
C     activity values.
C
C  Variable Passed:
C     DTAFILE - logical unit assigned to .DTA file;
C
C  Common Blocks:
C     BLOOD 
C     SCANS
C
C  No Subroutines
C  Called by:  METPROC
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE READDTA(DTAFILE)

      INCLUDE 'metproc.inc'

      INTEGER*4    DTAFILE,I
      REAL          FILETAU
      CHARACTER*256 RCSHEADER

      RCSHEADER = "$Id: readdta.f,v 2.2 1996/10/29 19:27:43 tom Exp $"

8000  FORMAT(I1,1X,A4)
      READ(DTAFILE,8000) NTYPE,SCANID

      IF (NTYPE.LE.3 .OR. NTYPE.EQ.7) THEN
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

      READ(DTAFILE,*) SCANST, SCANLEN
      SCANEND = SCANST + SCANLEN
      IF (FILETYPE.LT.3) THEN
         READ(DTAFILE,*) OXYCONT(1)
      ELSE
         READ(DTAFILE,*) PEAKBANK
         READ(DTAFILE,*) OXYCONT(1), HEMATOCRIT
      END IF
      READ(DTAFILE,*) NUMPNTS
      DO 100 I=1,NUMPNTS
         READ(DTAFILE,*) TIME(I), CORACTIV(I)
         ACTIVITY(I) = CORACTIV(I) * EXP(-TIME(I)*FILETAU)
100   CONTINUE

      RETURN
      END
