CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  UPPERCAS.FTN
C
C  This procedure converts a character string to all upper case
C     letters.
C
C  Variables:
C     STR = character string;
C     LEN = length of the string;
C
C  Subroutine of FIELDS.LIB and TERMINAL.LIB
C
C  T.O. Videen -- 24-Jan-86
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C $Log: uppercas.f,v $
c Revision 1.1  1992/04/13  19:08:15  tom
c Initial revision
c
c Revision 1.1  89/11/21  12:54:29  12:54:29  todd (M. Todd Gamble)
c Initial revision
c 
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      SUBROUTINE UPPERCAS(STR,LEN)
C
      INTEGER*4 LEN,I,K,A,Z,DIFF
      CHARACTER*80 STR
C
      A = ICHAR('a')
      Z = ICHAR('z')
      DIFF = ICHAR('a') - ICHAR('A')
C
      IF (LEN.GE.1) THEN
        DO 100 I=1,LEN
          K = ICHAR(STR(I:I))
          IF ((K.GE.A).AND.(K.LE.Z)) STR(I:I) = CHAR(K-DIFF)
100     CONTINUE
      END IF
C
      RETURN
      END
