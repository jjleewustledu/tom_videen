c $Log: interpie.f,v $
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
C  Subroutine:   INTERPIE.FTN
C  Author:       Tom O. Videen
C  Date:         14-May-87
C  Written For:  METPROC
C  Intent:
C     This program takes pie data and interpolates for
C     a given bankpair value a specific pie-slope.
C
C  History:
C     Modified 13-Oct-88 by TOV to allow minimum bank pairs to be 0.1.
C     Modified 24-Mar-89 by TOV to prompt for pie slope unscaled for
C        PETT efficiency.
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
C  Uses Subroutine:
C  TERMINAL.LIB
C     GETREAL
C  Called by:  GETFACTR
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE INTERPIE(PIESLOPE,BPAIR,NPIES,BNKPAIRS,PSLOPES,PEAKBANK,FACTOR)

      INTEGER*4 USERIN,USEROUT
      INTEGER*4    NPIES,I,J
      REAL         PIESLOPE,BPAIR,BNKPAIRS(10),PSLOPES(10),PEAKBANK,FACTOR
      CHARACTER*1  BELL
 
      CHARACTER*80 Q
      CHARACTER*256 RCSHEADER

      COMMON /USRIO/ USERIN,USEROUT

      RCSHEADER = "$Id: interpie.f,v 2.2 1999/12/16 15:27:47 tom Exp $"
      BELL = CHAR(7)

      WRITE(USEROUT,*) 'If PEAK Bank Pairs are entered, FACTOR should not be 1.0'
      Q = 'FACTOR to multiply times Bank Pairs'
      CALL GETREAL(Q,FACTOR,0.1,1.0)
      Q = 'Bank Pairs for this scan (1000 counts/sec)'
      CALL GETREAL(Q,PEAKBANK,0.1,100.)
      BPAIR = PEAKBANK*FACTOR
      WRITE(USEROUT,*) 'Average Bank Pairs = ',BPAIR

      IF (BPAIR.LT.BNKPAIRS(1)) THEN
        WRITE(USEROUT,*)'>>> Average Bank Pairs ',BPAIR,
     &     ' LESS than Smallest Pie ',BNKPAIRS(1)
        WRITE(USEROUT,*)'>>> A Linear Extrapolation was made ',BELL
        PIESLOPE = PSLOPES(1) -(BNKPAIRS(1)-BPAIR)*
     &     (PSLOPES(2)-PSLOPES(1))/(BNKPAIRS(2)-BNKPAIRS(1))
      ELSE IF (BPAIR.GT.BNKPAIRS(NPIES)) THEN
        WRITE(USEROUT,*)'>>> Average Bank Pairs ',BPAIR,
     &     ' GREATER than Largest Pie ',BNKPAIRS(NPIES)
        WRITE(USEROUT,*)'>>> A Linear Extrapolation was made ',BELL
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

      Q = 'PIE SLOPE to be used (before PETT efficiency scaling)'
      CALL GETREAL(Q,PIESLOPE,0.00001,10000.)

      RETURN
      END
