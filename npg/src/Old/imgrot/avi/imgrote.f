      SUBROUTINE IMGROTE(FOV,MMPPIX,NSLICE,IMAG,MASK,PARAM,DEL,JAXIS,ERR)
      REAL*4 FOV(3),MMPPIX(3),PARAM(12)
      PARAMETER (NROW=100)
      PARAMETER (NCOL=100)
      PARAMETER (NPIX=10112)
      INTEGER*2 IMAG(NPIX,NSLICE)
      INTEGER*2 MASK(NPIX,NSLICE)
      REAL*4 V(2),X(3),XP(3),TROT(4,4),C(4,4),T(4,4)
      LOGICAL*4 LVALID(2)
      COMMON/ACOM/SCALE

      IF(JAXIS.LT.1.OR.JAXIS.GT.3)STOP 'IMGROTE: JAXIS BAD'
      NX=NINT(.5*FOV(1)/DEL)
      NY=NINT(.5*FOV(2)/DEL)
      NZ=NINT(.5*FOV(3)/DEL)
      IF(JAXIS.EQ.1)THEN
        NX0=1
      ELSE
        NX0=-NX+1
      ENDIF
      IF(JAXIS.EQ.2)THEN
        NY0=1
      ELSE
        NY0=-NY+1
      ENDIF
      IF(JAXIS.EQ.3)THEN
        NZ0=1
      ELSE
        NZ0=-NZ+1
      ENDIF

      CALL vrt2img(MMPPIX,PARAM(7),C)
      CALL TROTCAL(+1,PARAM,TROT)
      CALL MATMUL(C,TROT,T,4)

      ERR=0.
      N=0
      DO 41 KX=NX0,NX
      DO 41 KY=NY0,NY
      DO 41 KZ=NZ0,NZ
      DO 11 JSIGN=1,2
      X(1)=(FLOAT(KX)-.5)*DEL
      X(2)=(FLOAT(KY)-.5)*DEL
      X(3)=(FLOAT(KZ)-.5)*DEL
      IF(JSIGN.EQ.2)X(JAXIS)=-X(JAXIS)
      DO L=1,3
        XP(L)=T(L,1)*X(1)+T(L,2)*X(2)+T(L,3)*X(3)+T(L,4)
      ENDDO
      IX=NINT(XP(1)-.5)
      WX=XP(1)-FLOAT(IX)
      IY=NINT(XP(2)-.5)
      WY=XP(2)-FLOAT(IY)
      IZ=NINT(XP(3)-.5)
      WZ=XP(3)-FLOAT(IZ)
      IF(IX.LE.1.OR.IX.GE.100)GOTO 9
      IF(IY.LE.1.OR.IY.GE.100)GOTO 9
      IF(IZ.EQ.0.AND.WZ.GT.0.999)THEN
        IZ=1
        WZ=0.
      ENDIF
      IF(IZ.EQ.NSLICE.AND.WZ.LT.0.001)THEN
        IZ=NSLICE-1
        WZ=1.
      ENDIF
      IF(IZ.LT.1.OR.IZ.GE.NSLICE)GOTO 9
      IF(
     &    MASK(IX+0+100*(IY+0-1),IZ+0).EQ.0
     &.OR.MASK(IX+1+100*(IY+0-1),IZ+0).EQ.0
     &.OR.MASK(IX+0+100*(IY+1-1),IZ+0).EQ.0
     &.OR.MASK(IX+1+100*(IY+1-1),IZ+0).EQ.0
     &.OR.MASK(IX+0+100*(IY+0-1),IZ+1).EQ.0
     &.OR.MASK(IX+1+100*(IY+0-1),IZ+1).EQ.0
     &.OR.MASK(IX+0+100*(IY+1-1),IZ+1).EQ.0
     &.OR.MASK(IX+1+100*(IY+1-1),IZ+1).EQ.0
     &)GOTO 9
      V(JSIGN)=SCALE*(
     & +(1.-WZ)*((1.-WY)*(
     &               (1.-WX)*FLOAT(IMAG(IX+0+100*(IY+0-1),IZ+0))
     &                   +WX*FLOAT(IMAG(IX+1+100*(IY+0-1),IZ+0)))
     &               +WY*(
     &               (1.-WX)*FLOAT(IMAG(IX+0+100*(IY+1-1),IZ+0))
     &                   +WX*FLOAT(IMAG(IX+1+100*(IY+1-1),IZ+0))))
     &      +WZ*((1.-WY)*(
     &               (1.-WX)*FLOAT(IMAG(IX+0+100*(IY+0-1),IZ+1))
     &                   +WX*FLOAT(IMAG(IX+1+100*(IY+0-1),IZ+1)))
     &               +WY*(
     &               (1.-WX)*FLOAT(IMAG(IX+0+100*(IY+1-1),IZ+1))
     &                   +WX*FLOAT(IMAG(IX+1+100*(IY+1-1),IZ+1))))
     &)
      LVALID(JSIGN)=.TRUE.
      GOTO 11
    9 LVALID(JSIGN)=.FALSE.
   11 CONTINUE
      IF(.NOT.(LVALID(1).AND.LVALID(2)))GOTO 41
      N=N+1
      ERR=ERR+(V(1)-V(2))**2
   41 CONTINUE
      ERR=ERR/FLOAT(N)
C      NK=4*NX*NY*NZ
C      type "('npix',2i6,F10.4)",NK,N,FLOAT(N)/FLOAT(NK)
      RETURN
      END

      SUBROUTINE IMGROTS(MMPPIX,NSLICE,IMAG,MASK,PARAM,IMAGNEW)
      REAL*4 MMPPIX(3),PARAM(12)
      PARAMETER (NROW=100)
      PARAMETER (NCOL=100)
      PARAMETER (NPIX=10112)
      INTEGER*2 IMAG(NPIX,NSLICE)
      INTEGER*2 MASK(NPIX,NSLICE)
      INTEGER*2 IMAGNEW(NPIX,NSLICE)
      REAL*4 X(3),XP(3),TROT(4,4),A(4,4),B(4,4),C(4,4),T(4,4)
      LOGICAL*4 LVALID

      CALL vrt2img(MMPPIX,PARAM(1),C)
      CALL vrt2img(MMPPIX,PARAM(7),C)
      CALL img2vrt(MMPPIX,PARAM(7),B)
      CALL TROTCAL(+0,PARAM,TROT)
      CALL TROTCAL(+1,PARAM,TROT)
      CALL imgrotrl(TROT)
      CALL MATMUL(C,TROT,A,4)
      CALL MATMUL(A,B,T,4)
      CALL imgrotrl(T)

      N=0
      DO 41 K=1,NSLICE
      DO 41 J=1,100
      DO 41 I=1,100
      X(1)=FLOAT(I)
      X(2)=FLOAT(J)
      X(3)=FLOAT(K)
      DO L=1,3
        XP(L)=T(L,1)*X(1)+T(L,2)*X(2)+T(L,3)*X(3)+T(L,4)
      ENDDO
      IX=NINT(XP(1)-.5)
      WX=XP(1)-FLOAT(IX)
      IY=NINT(XP(2)-.5)
      WY=XP(2)-FLOAT(IY)
      IZ=NINT(XP(3)-.5)
      WZ=XP(3)-FLOAT(IZ)
      IF(IX.LE.1.OR.IX.GE.100)GOTO 9
      IF(IY.LE.1.OR.IY.GE.100)GOTO 9
      IF(IZ.EQ.0.AND.WZ.GT.0.999)THEN
        IZ=1
        WZ=0.
      ENDIF
      IF(IZ.EQ.NSLICE.AND.WZ.LT.0.001)THEN
        IZ=NSLICE-1
        WZ=1.
      ENDIF
      IF(IZ.LT.1.OR.IZ.GE.NSLICE)GOTO 9
      IF(
     &    MASK(IX+0+100*(IY+0-1),IZ+0).EQ.0
     &.OR.MASK(IX+1+100*(IY+0-1),IZ+0).EQ.0
     &.OR.MASK(IX+0+100*(IY+1-1),IZ+0).EQ.0
     &.OR.MASK(IX+1+100*(IY+1-1),IZ+0).EQ.0
     &.OR.MASK(IX+0+100*(IY+0-1),IZ+1).EQ.0
     &.OR.MASK(IX+1+100*(IY+0-1),IZ+1).EQ.0
     &.OR.MASK(IX+0+100*(IY+1-1),IZ+1).EQ.0
     &.OR.MASK(IX+1+100*(IY+1-1),IZ+1).EQ.0
     &)GOTO 9
      V=(
     & +(1.-WZ)*((1.-WY)*(
     &               (1.-WX)*FLOAT(IMAG(IX+0+100*(IY+0-1),IZ+0))
     &                   +WX*FLOAT(IMAG(IX+1+100*(IY+0-1),IZ+0)))
     &               +WY*(
     &               (1.-WX)*FLOAT(IMAG(IX+0+100*(IY+1-1),IZ+0))
     &                   +WX*FLOAT(IMAG(IX+1+100*(IY+1-1),IZ+0))))
     &      +WZ*((1.-WY)*(
     &               (1.-WX)*FLOAT(IMAG(IX+0+100*(IY+0-1),IZ+1))
     &                   +WX*FLOAT(IMAG(IX+1+100*(IY+0-1),IZ+1)))
     &               +WY*(
     &               (1.-WX)*FLOAT(IMAG(IX+0+100*(IY+1-1),IZ+1))
     &                   +WX*FLOAT(IMAG(IX+1+100*(IY+1-1),IZ+1))))
     &)
      LVALID=.TRUE.
      N=N+1
      GOTO 41
    9 V=0.
      LVALID=.FALSE.
   41 IMAGNEW(I+100*(J-1),K)=V
      type "('number of non-zero pixles saved',i8)",N
      RETURN
      END
