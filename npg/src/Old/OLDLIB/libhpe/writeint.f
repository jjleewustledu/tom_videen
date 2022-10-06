CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  WRITEINT.FTN
C
C  This procedure writes an integer to a specified field on the
C     terminal screen.
C
C  Variables:
C     I = integer value;
C     LEN = length of the field (character spaces);
C     ROW,COL = location of the first character of the field;
C
C  Uses:
C     TCONTROL - terminal control;
C
C
C  Subroutine of FIELDS.LIB
C
C  T.O. Videen -  18-Apr-86
C
C  History:
C     Modified 11-Oct-91 by Tom Yang for adding the format
C	when the length is 1.
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C $Log: writeint.f,v $
c Revision 1.1  1992/04/13  19:10:47  tom
c Initial revision
c
c Revision 1.4  90/03/19  09:18:17  09:18:17  todd (M. Todd Gamble)
c *** empty log message ***
c 
c Revision 1.3  89/12/02  11:54:30  11:54:30  todd (M. Todd Gamble)
c removed CARCON carriage control command, unsupported.
c 
c Revision 1.2  89/11/28  13:42:48  13:42:48  todd (M. Todd Gamble)
c modified for HPUX
c 
c Revision 1.1  89/11/28  12:38:35  12:38:35  todd (M. Todd Gamble)
c Initial revision
c 
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      SUBROUTINE WRITEINT(I,LEN,ROW,COL)
C
      INTEGER*4    I,LEN,ROW,COL,L,USERIN,USEROUT
      CHARACTER*20 BLANK
C
      COMMON /USRIO/ USERIN,USEROUT
      DATA BLANK /'                    '/
C
      IF (LEN.LE.20) THEN
        L = LEN
      ELSE
        L = 20
      END IF
C
      CALL TCONTROL(3,ROW,COL)
      WRITE (USEROUT,8100) BLANK(1:LEN)
      CALL TCONTROL(3,ROW,COL)
      IF (L.EQ.1) THEN
        WRITE (USEROUT,8201) I
      ELSE IF (L.EQ.2) THEN
        WRITE (USEROUT,8202) I
      ELSE IF (L.EQ.3) THEN
        WRITE (USEROUT,8203) I
      ELSE IF (L.EQ.4) THEN
        WRITE (USEROUT,8204) I
      ELSE IF (L.EQ.5) THEN
        WRITE (USEROUT,8205) I
      ELSE IF (L.EQ.6) THEN
        WRITE (USEROUT,8206) I
      ELSE IF (L.EQ.7) THEN
        WRITE (USEROUT,8207) I
      ELSE IF (L.EQ.8) THEN
        WRITE (USEROUT,8208) I
      ELSE IF (L.EQ.9) THEN
        WRITE (USEROUT,8209) I
      ELSE IF (L.EQ.10) THEN
        WRITE (USEROUT,8210) I
      ELSE
        WRITE (USEROUT,*) I
      END IF
C
      RETURN
8100  FORMAT(A)
8201  FORMAT(I1)
8202  FORMAT(I2)
8203  FORMAT(I3)
8204  FORMAT(I4)
8205  FORMAT(I5)
8206  FORMAT(I6)
8207  FORMAT(I7)
8208  FORMAT(I8)
8209  FORMAT(I9)
8210  FORMAT(I10)
      END
