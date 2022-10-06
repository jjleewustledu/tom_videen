CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   RDATA.FTN
C
C  Author:       Tom Videen
C  Date:         2/10/85
C  Written For:  TISCRV
C
C  History
C     Modified 03-Jun-92 TOV of Joanne Markham RPORDAT.FTN
C			Ported to Unix 19-Oct-93 TOV
C
C  Intent:
C     This subroutine reads in ROI coordinates and sizes.
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE RDATA(NX,NY,NZ,NZX,NZY,NMAX,REG,AT,REGLU,USEROUT)
C
      REAL*4       NX(100),NY(100),NZ(100)
      INTEGER*4    NZX(100),NZY(100)
      INTEGER*4    NMAX
      INTEGER*4    REGLU, USEROUT
      CHARACTER*1  RTYPE
      CHARACTER*10 REG(100)
      CHARACTER*256 AT
      CHARACTER*70 HEAD
C
8000  FORMAT(A20)
8100  FORMAT(A70)
8200  FORMAT(I3,3X,A10,5X,A1,F9.2,F8.2,F8.2,3X,I4,3X,I4)

      READ(REGLU,*,ERR=9000,END=9000) ITYPE
      IF (ITYPE.LT.11) THEN
        WRITE(USEROUT,*)'ERROR: file type ',ITYPE,' is not compatable!'
        STOP
      END IF
C
      READ (REGLU,8000) AT
      READ (REGLU,8100) HEAD
      WRITE(USEROUT,*)'> ',HEAD
C
      DO 100 I=1,100
        READ(REGLU,8200,ERR=9000,END=9000) M, REG(I), RTYPE, NZ(I),
     &    NX(I), NY(I), NZX(I), NZY(I)
        IF (M.EQ.0) THEN
          NMAX = NZY(I)
          GO TO 200
        END IF
        IF (RTYPE.NE.'R') THEN
          WRITE(USEROUT,*)'ERROR: region ',I,' is not rectangular!'
          STOP
        END IF
100   CONTINUE
200   WRITE(USEROUT,*) NMAX,' regions have been read'
C
      RETURN
9000  WRITE(USEROUT,*)'ERROR: reading REG file!'
      STOP
      END
