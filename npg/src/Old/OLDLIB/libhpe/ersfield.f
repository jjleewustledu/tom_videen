CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  ERSFIELD.FTN
C
C  This procedure erases a field of length LLEN beginning at a
C     specified row and column.
C     The cursor is turned off during erasing.
C     The cursor is left at the beginning of the field in its
C     initial ON/OFF state.
C
C  Variables:
C     LLEN = length of the field (no. of characters);
C     ROW = row where the field begins;
C     COL = column where the field begins;
C
C  Uses:
C     WRTFIELD
C
C  Subroutine of FIELDS.LIB
C
C  T.O. Videen -- 07-Feb-86
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C $Log: ersfield.f,v $
c Revision 1.1  1992/04/13  19:10:47  tom
c Initial revision
c
c Revision 1.2  90/03/19  09:17:56  09:17:56  todd (M. Todd Gamble)
c *** empty log message ***
c 
c Revision 1.1  89/11/28  12:38:25  12:38:25  todd (M. Todd Gamble)
c Initial revision
c 
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      SUBROUTINE ERSFIELD (LLEN, ROW, COL)
C
      INTEGER*4		LLEN, ROW, COL
      CHARACTER*80	BLANK
C
      DATA BLANK /' '/
C
      CALL WRTFIELD (BLANK, LLEN, ROW, COL)
C
      RETURN
      END
