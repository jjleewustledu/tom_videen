c $Log: getfactr.f,v $
c Revision 2.2  1995/12/20  04:32:31  tom
c 961 version
c
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
C  Subroutine:   GETFACTR.FTN
C  Author:       Tom O. Videen
C  Date:         02-Jun-87
C  Written for:  METPROC
C  Intent:
C     This subroutine gets values for the adjusted pie slope
C     (PETTCONV) and the partition coefficient (LAMBDA).
C     If the pie slope is interpolated from values in a PIE file,
C     the subroutine also gets the bank pair value (BPAIR) used
C     in the interpolation.
C  History:
C     Modified 24-Mar-89 by TOV to scale pie slope by EFACTOR.
C     Modified 07-Jul-93 by TOV to skip INTERPIE for ECAT images.
C
C  Variables Passed:
C     IOPTION: true--> Pie Slope is computed internally by
C        reading a PIE file and interpolating peak bank pairs;
C     PEAKUSED: true--> user enters peak of Bank Pairs and
C        program estimates the mean;
C        false--> user enters mean Bank Pairs;
C     NPIES = number of PSLOPES/BNKPAIRS;
C     PSLOPES = pie slopes read from PIE file;
C     BNKPAIRS = bank pairs read from PIE file;
C     EFACTOR = efficiency of PETT for pie relative to current scan;
C
C  Common Blocks:
C     USRIO
C     SCANS
C     VALUES
C
C  Subroutines:
C     INTERPIE
C     GETREAL
C
C  Called by:  METPROC
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE GETFACTR(IOPTION,PEAKUSED,NPIES,PSLOPES,BNKPAIRS,EFACTOR)

      INCLUDE 'metproc.inc'

      INTEGER*4    NPIES
      REAL*4       BNKPAIRS(10),PSLOPES(10),FACTOR,EFACTOR
      LOGICAL*1    IOPTION,PEAKUSED
      CHARACTER*80 Q
      CHARACTER*256 RCSHEADER

      RCSHEADER = "$Id: getfactr.f,v 2.2 1995/12/20 04:32:31 tom Exp tom $"

C  Get Pie Slope

      IF (RCONTYPE .GE. 3000) THEN   
         PIESLOPE = PSLOPES(1)
      ELSE IF (IOPTION) THEN
         IF (PEAKUSED .AND. NTYPE.LE.4 .AND. NTYPE.NE.3) THEN
            FACTOR = 0.9
         ELSE IF (PEAKUSED .AND. NTYPE.EQ.3) THEN
            FACTOR = 0.59
         ELSE
            FACTOR = 1.0
         END IF
         CALL INTERPIE(PIESLOPE,BPAIR,NPIES,BNKPAIRS,PSLOPES,PEAKBANK,FACTOR)
      ELSE
         WRITE(USEROUT,*) 'Observed Peak Bank Pair = ',PEAKBANK
         Q = 'Pie Slope (without PETT efficiency scaling)'
         CALL GETREAL(Q,PIESLOPE,0.0001,10000.)
      END IF

C  Get PETT conversion factor; (convert PIESLOPE from min to sec)

      PETTCONV(1) = PIESLOPE*60.*EFACTOR

C  Get Partition Coefficient for NTYPEs 1,2,4,7

      IF (NTYPE.LE.2 .OR. NTYPE.EQ.4 .OR. NTYPE.EQ.7) THEN
         IF (NTYPE.EQ.4) THEN
            LAMBDA = BULAMBDA
         ELSE
            LAMBDA = HOLAMBDA
         END IF
         WRITE(USEROUT,*) 'Partition coefficients:'
         WRITE(USEROUT,8000) 'Water  ',HOLAMBDA
         WRITE(USEROUT,8000) 'Butanol',BULAMBDA
8000     FORMAT(1X,'     ',A7,' ',F4.2,' ml/ml')
         WRITE(USEROUT,*)
         Q = 'Partition Coefficient (ml/ml)'
         CALL GETREAL(Q,LAMBDA,0.001,100.)
      END IF

      RETURN
      END
