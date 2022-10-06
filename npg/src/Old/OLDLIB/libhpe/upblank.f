CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  UPBLANK.FTN
C
C  This procedure moves the cursor up 1 row and erases that line.
C
C  Uses:
C     TCONTROL
C
C  Subroutine of TERMINAL.LIB
C
C  T.O. Videen -- 24-Jan-86
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C $Log: upblank.f,v $
c Revision 1.1  1992/04/13  19:08:15  tom
c Initial revision
c
c Revision 1.1  89/11/21  12:54:08  12:54:08  todd (M. Todd Gamble)
c Initial revision
c 
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      SUBROUTINE UPBLANK
C
C  Move the cursor up 1 row
C
      CALL TCONTROL (5, 1, 0)
C
C  Erase the line on which the cursor is located
C
      CALL TCONTROL (4, 0, 0)
C
      RETURN
      END
