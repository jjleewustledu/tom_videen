c $Log: getfactr.f,v $
c Revision 1.3  1993/04/26  17:24:31  tom
c Revised for ECAT images (pie file type 7)
c
c Revision 1.2  1992/11/02  16:02:53  tom
c Added RCS identifies
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   GETFACTR.FTN
C
C  Author:       Tom O. Videen
C  Date:         02-Jun-87
C  Written for:  fastproc
C
C  Intent:
C     This subroutine gets values for the adjusted pie slope
C     (PETTCONV) and the partition coefficient (LAMBDA).
C     If the pie slope is interpolated from values in a PIE file,
C     the subroutine also gets the bank pair value (BPAIR) used
C     in the interpolation.
C
C  History:
C     Modified 24-Mar-89 by TOV to scale pie slope by EFACTOR.
C			Modified 17-Feb-92 by TOV to eliminate prompts
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
C  Common Block Variables:
C
C  /USRIO/
C     USEROUT = logical unit for user output;
C
C  /SCANS/
C     NTYPE = type of scan file;
C     SCANST = start time of scan (sec);
C     SCANLEN = scan length (sec);
C     PEAKBANK = peak bank pair observed during scan;
C     LAMBDA = partition coefficient (ml/ml);
C     PIESLOPE = pie slope used;
C        (well counts * pixel * min)/(PETT no * ml * sec)
C     BPAIR = bank pair used;
C     PETTCONV = PETT conversion factor;
C        (well counts * pixel)/(PETT no * ml)
C
C  /VALUES/
C     HOLAMBDA - partition coefficient for water (ml blood/ml brain)
C     BULAMBDA - partition coefficient for butanol (ml blood/ml brain)
C     BRAINDEN - density of brain (g/ml)
C
C  Uses Subroutines:
C
C  Main Subroutines:
C     INTERPIE
C
C  Called by:  METPROC
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE GETFACTR(IOPTION,PEAKUSED,NPIES,PSLOPES,BNKPAIRS,
     &   EFACTOR)
C
      INCLUDE 'metproc.inc'
C
      INTEGER*4    NPIES
      REAL*4       BNKPAIRS(10),PSLOPES(10),FACTOR,EFACTOR
      LOGICAL*1    IOPTION,PEAKUSED
C
			RCSHEADER = "$Header: /home/petsun2/tom/src/fastproc/src/RCS/getfactr.f,v 1.3 1993/04/26 17:24:31 tom Exp $"
C
C  Get Pie Slope
C
      IF (IOPTION) THEN
        IF (PEAKUSED .AND. NTYPE.LE.4 .AND. NTYPE.NE.3) THEN
          FACTOR = 0.9
        ELSE IF (PEAKUSED .AND. NTYPE.EQ.3) THEN
          FACTOR = 0.59
        ELSE
          FACTOR = 1.0
        END IF
        CALL INTERPIE(PIESLOPE,BPAIR,NPIES,BNKPAIRS,PSLOPES,PEAKBANK,
     &     FACTOR)
      ELSE
        WRITE(USEROUT,*) 'Observed Peak Bank Pair = ',PEAKBANK
      END IF
C
C  Get PETT conversion factor; (convert PIESLOPE from min to sec)
C
      PETTCONV(1) = PIESLOPE*60.*EFACTOR
C
C  Get Partition Coefficient for NTYPEs 1,2, and 4
C
      IF (NTYPE.LE.2 .OR. NTYPE.EQ.4) THEN
        IF (NTYPE.EQ.4) THEN
          LAMBDA = BULAMBDA
        ELSE
          LAMBDA = HOLAMBDA
        END IF
        WRITE(USEROUT,*) 'Partition coefficient =',LAMBDA
      END IF
C
      RETURN
      END
