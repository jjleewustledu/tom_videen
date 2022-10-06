c $Log: interpie.f,v $
c Revision 1.2  1992/11/02  16:02:53  tom
c Added RCS identifies
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   INTERPIE.FTN
C
C  Author:       Tom O. Videen
C  Date:         14-Feb-92
C  Written For:  fastproc
C
C  Intent:
C     This program takes pie data and interpolates for
C     a given bankpair value a specific pie-slope.
C
C  History:
C			Modified 14-Feb-92 by TOV for fastproc eliminating all prompts.
C
C  Variables Passed:
C     PIESLOPE = pie slope to be used;
C     BPAIR = bank pair to be used;
C     NPIES  - number of pie slope/bank pair values;
C     PSLOPES - pie slopes;
C     BNKPAIRS - bank pairs;
C     PEAKBANK = peak bank pair from DTA file;
C     FACTOR = weighting factor to convert peak bank pair to mean;
C
C  Called by:  GETFACTR
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE INTERPIE(PIESLOPE,BPAIR,NPIES,BNKPAIRS,PSLOPES,
     &   PEAKBANK,FACTOR)
C
      INTEGER*4    NPIES,I,J
      REAL         PIESLOPE,BPAIR,BNKPAIRS(10),PSLOPES(10),PEAKBANK,
     &             FACTOR
      CHARACTER*1  BELL 
			CHARACTER*256 RCSHEADER
C
			RCSHEADER = "$Header: /home/petsun2/tom/src/fastproc/src/RCS/interpie.f,v 1.2 1992/11/02 16:02:53 tom Exp $"
      BELL = CHAR(7)
C
      BPAIR = PEAKBANK*FACTOR
      WRITE(USEROUT,*) 'Average Bank Pairs = ',BPAIR
C
      IF (BPAIR.LT.BNKPAIRS(1)) THEN
        WRITE(USEROUT,*)'*** Average Bank Pairs ',BPAIR,
     &     ' LESS than Smallest Pie ',BNKPAIRS(1)
        WRITE(USEROUT,*)'*** A Linear Extrapolation was made ',BELL
        PIESLOPE = PSLOPES(1) -(BNKPAIRS(1)-BPAIR)*
     &     (PSLOPES(2)-PSLOPES(1))/(BNKPAIRS(2)-BNKPAIRS(1))
      ELSE IF (BPAIR.GT.BNKPAIRS(NPIES)) THEN
        WRITE(USEROUT,*)'*** Average Bank Pairs ',BPAIR,
     &     ' GREATER than Largest Pie ',BNKPAIRS(NPIES)
        WRITE(USEROUT,*)'*** A Linear Extrapolation was made ',BELL
        I = NPIES
        J = I-1
        PIESLOPE = PSLOPES(I) +(BPAIR-BNKPAIRS(I))*
     &     (PSLOPES(I)-PSLOPES(J))/(BNKPAIRS(I)-BNKPAIRS(J))
      ELSE
        DO 100 I=2,NPIES
          IF (BNKPAIRS(I).GE.BPAIR) GOTO 200
100     CONTINUE
200     PIESLOPE = PSLOPES(I-1) + (BPAIR-BNKPAIRS(I-1))*
     &     (PSLOPES(I)-PSLOPES(I-1))/(BNKPAIRS(I)-BNKPAIRS(I-1))
      END IF
C
      RETURN
      END
