      SUBROUTINE SCAIMG(F,RES)
      PARAMETER (NPIX=10112)
      INTEGER*2 RES(NPIX)
      DO 1 I=1,10000
    1 RES(I)=INT2(F*FLOAT(RES(I)))
      RETURN
      END
      integer*2 function INT2(A)
      integer*2 short
      INT2=short(NINT(A))
      return
      end
      SUBROUTINE SUMIMG(F,IN,RES)
      PARAMETER (NPIX=10112)
      INTEGER*2 IN(NPIX),RES(NPIX)
      IF(F.EQ.0.)RETURN
      DO 1 I=1,10000
    1 RES(I)=RES(I)+INT2(F*FLOAT(IN(I)))
      RETURN
      END
      SUBROUTINE ALLONE(LA,ARES,NSLICE)
      PARAMETER (NPIX   = 10112)
      INTEGER*2 ARES(NPIX,NSLICE)
      DO 1 I=1,NSLICE
      DO 1 K=1,NPIX
    1 ARES(K,I)=1
      LA=-1
      RETURN
      END
      SUBROUTINE ATTENY(THRESH,IMAGE,NSLICE,NSLLO,NSLHI)
C     modified 26-Mar-93 to use parameters NSLLO and NSLHI
      PARAMETER (NPIX=10112)
      INTEGER*2 IMAGE(NPIX,NSLICE)
      MMAX=0
      DO 11 K=NSLLO,NSLHI
   11 CALL ATTENM(IMAGE(1,K),MMAX)
      M=NINT(FLOAT(MMAX)*THRESH)
      TYPE "(' ATTENY M',I6)",M
      DO 12 K=NSLLO,NSLHI
   12 CALL ATTENF(IMAGE(1,K),M)
      DO 13 K=NSLLO,NSLHI
   13 CALL TOVTEMP(IMAGE(1,K),100,100)
      DO 14 K=1,NSLLO-1
   14 CALL ATTENF(IMAGE(1,K),32767)
      DO 15 K=NSLHI+1,NSLICE
   15 CALL ATTENF(IMAGE(1,K),32767)
      RETURN
      END
      SUBROUTINE ATTENM(IMAGE,M)
      INTEGER*2 IMAGE(100,100)
      DO 44 J=1,100
      DO 44 I=1,100
   44 M=MAX0(IMAGE(I,J),M)
      RETURN
      END
      SUBROUTINE ATTENF(IMAGE,M)
      INTEGER*2 IMAGE(100,100)
      DO 44 J=1,100
      DO 44 I=1,100
      IF(IMAGE(I,J).GT.M)THEN
      IMAGE(I,J)=1000
      ELSE
      IMAGE(I,J)=0
      ENDIF
   44 CONTINUE
      RETURN
      END
      SUBROUTINE TOVTEST
      PARAMETER (NI=19)
      PARAMETER (NJ=19)
      INTEGER*2 IMAGE(NI,NJ)
      PI=ATAN2(0.,-1.)
      DO 9 L=0,0
      R=(.25+.05*FLOAT(L))*FLOAT(NI)
      DO 9 K=0,100
      T=FLOAT(K)*2.*PI/100.
      I=NI/2+1+NINT(R*COS(T))
      J=NJ/2+1+NINT(R*SIN(T))
    9 IMAGE(I,J)=1000
      DO 1 I=1,NI
    1 TYPE "(1X,19I3)",(IMAGE(I,J),J=1,NJ)
      TYPE "()"
      CALL TOVTEMP(IMAGE,NI,NJ)
      DO 2 I=1,NI
    2 TYPE "(1X,19I3)",(IMAGE(I,J),J=1,NJ)
      STOP
      END
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   TOVTEMP.FTN
C  SUBROUTINE version of TEMPIMG6 to be called in context of RMSIMG
C
C  Author:       Avi Snyder
C  Author:       Tom O. Videen
C  Date:         03-Apr-90
C  Written For:  Movement artifact correction
C
C  History:
C     Templating code taken from D2TEMPLA.FTN
C     Modified 02-Jul-90 by TOV to handle 7 or 49 slice files.
C     Modified 17-Oct-90 by TOV to write scaling factor.
C     Modified 05-Dec-92 by AZS to operate as subroutine in RMSIMG
C
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE TOVTEMP(MASK,NI,NJ)
      INTEGER*2    MASK(NI,NJ)
      INTEGER*2    IN/1000/    ! in template
      INTEGER*2    OUT /2/     ! not in template
      LOGICAL*4    MORE        ! more points added to exterior
C
C  Assign OUT to all points along the boundaries of the array
C  which are not IN the template.
C
        DO 300 I=1,NI
          IF (MASK(I,1).NE.IN) MASK(I,1) = OUT
          IF (MASK(I,NJ).NE.IN) MASK(I,NJ) = OUT
300     CONTINUE
        DO 400 J=1,NJ
          IF (MASK(1,J).NE.IN) MASK(1,J) = OUT
          IF (MASK(NI,J).NE.IN) MASK(NI,J) = OUT
400     CONTINUE
C
C  Now, starting with the boundaries of the array, expand the
C  area excluded from the template by assigning OUT to all points
C  directly adjacent to a point whose value is OUT and which
C  are currently zero (neither IN nor OUT).
C
        MORE = .FALSE.
C
C  Outside Rows
C
        DO 500 I=2,NI-1
          IF (MASK(I,1).EQ.OUT .AND. MASK(I,2).LT.IN) THEN
            MASK(I,2) = OUT
            MORE = .TRUE.
          ENDIF
          IF (MASK(I,NI).EQ.OUT .AND. MASK(I,NJ-1).LT.IN) THEN
            MASK(I,NJ-1) = OUT
            MORE = .TRUE.
          ENDIF
500     CONTINUE
C
C  Outside Columns
C
        DO 600 J=2,NJ-1
          IF (MASK(1,J).EQ.OUT .AND. MASK(2,J).LT.IN) THEN
            MASK(2,J) = OUT
            MORE = .TRUE.
          ENDIF
          IF (MASK(NI,J).EQ.OUT .AND. MASK(NI-1,J).LT.IN) THEN
            MASK(NI-1,J) = OUT
            MORE = .TRUE.
          ENDIF
600     CONTINUE
C
C  The following loop is repeated until it cycles with no more points
C  added to the outside of the region.
C
1000    IF (MORE) THEN
        MORE = .FALSE.
        DO 1500 I=2,NI-1
        DO 1400 J=2,NJ-1
        IF (MASK(I,J).EQ.OUT) THEN
          IF(MASK(I-1,J).LT.IN.AND.MASK(I-1,J).NE.OUT)THEN
                  MASK(I-1,J) = OUT
                  MORE = .TRUE.
          ENDIF
          IF(MASK(I+1,J).LT.IN.AND.MASK(I+1,J).NE.OUT)THEN
                  MASK(I+1,J) = OUT
                  MORE = .TRUE.
          ENDIF
          IF(MASK(I,J-1).LT.IN.AND.MASK(I,J-1).NE.OUT)THEN
                  MASK(I,J-1) = OUT
                  MORE = .TRUE.
          ENDIF
          IF(MASK(I,J+1).LT.IN.AND.MASK(I,J+1).NE.OUT)THEN
                  MASK(I,J+1) = OUT
                  MORE = .TRUE.
          ENDIF
        ENDIF
1400    CONTINUE
1500    CONTINUE
       GO TO 1000
       ENDIF
C
C  All points that are not OUT are assigned IN
C  All points that are not IN are assigned 0
C
      DO 51 I=1,NI
      DO 51 J=1,NJ
      IF(MASK(I,J).EQ.OUT)THEN
        MASK(I,J)=0
      ELSE
        MASK(I,J)=IN
      ENDIF
   51 CONTINUE
      RETURN
      END
