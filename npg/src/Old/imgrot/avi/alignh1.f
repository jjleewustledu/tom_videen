      SUBROUTINE ALIGNC(MMPPIX,NSLICE,IMAG1,MASK1,CENTER,N,AMEAN)
C     RETURNS IMAG1 MEAN PIXEL VALUE IN AMEAN
C     RETURNS IMAG1 CENTER OF MASS IN CENTER
C     MASK1 IS THE MASK
      REAL*4 MMPPIX(3)
      PARAMETER (NPIX=10112)
      INTEGER*2 IMAG1(NPIX,NSLICE)
      INTEGER*2 MASK1(NPIX,NSLICE)
      REAL*4 CENTER(3)
      N=0
      S=0.
      DO 1 I=1,3
    1 CENTER(I)=0.
      DO 44 K=1,NSLICE
      DO 44 J=1,100
      DO 44 I=1,100
      IF(MASK1(I+100*(J-1),K).GT.0)THEN
      N=N+1
      T=FLOAT(IMAG1(I+100*(J-1),K))
      S=S+T
      CENTER(1)=CENTER(1)+MMPPIX(1)*FLOAT(I)*T
      CENTER(2)=CENTER(2)+MMPPIX(2)*FLOAT(J)*T
      CENTER(3)=CENTER(3)+MMPPIX(3)*FLOAT(K)*T
      ENDIF
   44 CONTINUE
      DO 2 I=1,3
    2 CENTER(I)=CENTER(I)/S
      AMEAN=S/FLOAT(N)
      RETURN
      END
      SUBROUTINE ALIGNR(MMPPIX,NSLICE,IMAG1,MASK1,PARAM,RMSROT)
C     RETURNS IMAG1 WEIGHTED rms ROTATIONAL MOVEMENT IN RMSROT
C     PARAM(7:9) HAS CENTER OF ROTATION
C     PARAM(4:6) HAS ROTATION ANGLES
C     MASK1 IS THE MASK
      REAL*4 MMPPIX(3)
      PARAMETER (NPIX=10112)
      INTEGER*2 IMAG1(NPIX,NSLICE)
      INTEGER*2 MASK1(NPIX,NSLICE)
      REAL*4 PARAM(12)
      S=0.
      A=0.
      DO 44 K=1,NSLICE
      Z=MMPPIX(3)*FLOAT(K)-PARAM(9)
      DO 44 J=1,100
      Y=MMPPIX(2)*FLOAT(J)-PARAM(8)
      DO 44 I=1,100
      IF(MASK1(I+100*(J-1),K).GT.0)THEN
      X=MMPPIX(1)*FLOAT(I)-PARAM(7)
      T=FLOAT(IMAG1(I+100*(J-1),K))
      U=(-PARAM(5)*Z-PARAM(6)*Y)**2
     & +(-PARAM(4)*Z+PARAM(6)*X)**2
     & +( PARAM(4)*Y+PARAM(5)*X)**2
      S=S+T*U
      A=A+T
      ENDIF
   44 CONTINUE
      RMSROT=SQRT(S/A)
      RETURN
      END
      SUBROUTINE ROTCAL(PARAM)
      REAL*4 PARAM(12)
      REAL*4 ANG(3)/0.,0.,0./
      LOGICAL*4 INIT/.FALSE./
      REAL*4 ROT(3,3,8)
      COMMON/ROTCOM/ROT
      IF(ANG(1).EQ.PARAM(4).AND.ANG(2).EQ.PARAM(5).AND.
     &   ANG(3).EQ.PARAM(6).AND.INIT)RETURN
C     TYPE "(' CALCULATE ROT')"
      DO 1 N=1,6
      DO 1 M=1,3
      DO 1 L=1,3
    1 ROT(L,M,N)=0.
      DO 2 N=1,3
      DO 2 M=1,3
    2 ROT(M,M,N)=1.
      C1=COS(PARAM(4))
      S1=SIN(PARAM(4))
      C2=COS(PARAM(5))
      S2=SIN(PARAM(5))
      C3=COS(PARAM(6))
      S3=SIN(PARAM(6))
      ROT(2,2,1)= C1
      ROT(2,3,1)=-S1
      ROT(3,2,1)=+S1
      ROT(3,3,1)= C1
      ROT(1,1,2)= C2
      ROT(1,3,2)=-S2
      ROT(3,1,2)=+S2
      ROT(3,3,2)= C2
      ROT(1,1,3)= C3
      ROT(1,2,3)=-S3
      ROT(2,1,3)=+S3
      ROT(2,2,3)= C3
      ROT(2,2,4)=-S1
      ROT(2,3,4)=-C1
      ROT(3,2,4)=+C1
      ROT(3,3,4)=-S1
      ROT(1,1,5)=-S2
      ROT(1,3,5)=-C2
      ROT(3,1,5)=+C2
      ROT(3,3,5)=-S2
      ROT(1,1,6)=-S3
      ROT(1,2,6)=-C3
      ROT(2,1,6)=+C3
      ROT(2,2,6)=-S3
      CALL MATMUL(ROT(1,1,2),ROT(1,1,1),ROT(1,1,7),3)
      CALL MATMUL(ROT(1,1,3),ROT(1,1,7),ROT(1,1,8),3)
      CALL MATMUL(ROT(1,1,2),ROT(1,1,4),ROT(1,1,7),3)
      CALL MATMUL(ROT(1,1,3),ROT(1,1,7),ROT(1,1,4),3)
      CALL MATMUL(ROT(1,1,5),ROT(1,1,1),ROT(1,1,7),3)
      CALL MATMUL(ROT(1,1,3),ROT(1,1,7),ROT(1,1,5),3)
      CALL MATMUL(ROT(1,1,6),ROT(1,1,2),ROT(1,1,7),3)
      CALL MATMUL(ROT(1,1,7),ROT(1,1,1),ROT(1,1,6),3)
      ANG(1)=PARAM(4)
      ANG(2)=PARAM(5)
      ANG(3)=PARAM(6)
      INIT=.TRUE.
      RETURN
      END
      SUBROUTINE VALU6D
     &(MMPPIX,NSLICE,MASK0,IMAG0,I,J,K,PARAM,V,LVALID,LDIF,DVDPA)
      REAL*4 MMPPIX(3)
      PARAMETER (NPIX=10112)
      INTEGER*2 IMAG0(NPIX,NSLICE)
      INTEGER*2 MASK0(NPIX,NSLICE)
      PARAMETER (NTERM=6)
      REAL*4 PARAM(12),DVDPA(NTERM)
      LOGICAL*4 LVALID
      REAL*4 ROT(3,3,8)
      COMMON/ROTCOM/ROT
      COMMON/ACOM/A
      CALL ROTCAL(PARAM)
      X=MMPPIX(1)*FLOAT(I)-PARAM(10)-PARAM(1)
      Y=MMPPIX(2)*FLOAT(J)-PARAM(11)-PARAM(2)
      Z=MMPPIX(3)*FLOAT(K)-PARAM(12)-PARAM(3)
      XPM=(ROT(1,1,8)*X+ROT(1,2,8)*Y+ROT(1,3,8)*Z)/MMPPIX(1)
      YPM=(ROT(2,1,8)*X+ROT(2,2,8)*Y+ROT(2,3,8)*Z)/MMPPIX(2)
      ZPM=(ROT(3,1,8)*X+ROT(3,2,8)*Y+ROT(3,3,8)*Z)/MMPPIX(3)
      IX=NINT(XPM+PARAM(10)/MMPPIX(1)-.5)
      IF(IX.LE.1.OR.IX.GE.100)GOTO 9
      WX=XPM+PARAM(10)/MMPPIX(1)-FLOAT(IX)
      IY=NINT(YPM+PARAM(11)/MMPPIX(2)-.5)
      IF(IY.LE.1.OR.IY.GE.100)GOTO 9
      WY=YPM+PARAM(11)/MMPPIX(2)-FLOAT(IY)
      IZ=NINT(ZPM+PARAM(12)/MMPPIX(3)-.5)
      WZ=ZPM+PARAM(12)/MMPPIX(3)-FLOAT(IZ)
      IF(IZ.EQ.0.AND.WZ.GT.0.999)THEN
        IZ=1
        WZ=0.
      ENDIF
      IF(IZ.EQ.NSLICE.AND.WZ.LT.0.001)THEN
        IZ=NSLICE-1
        WZ=1.
      ENDIF
      IF(IZ.LT.1.OR.IZ.GE.NSLICE)GOTO 9
      LVALID=.TRUE.
      IF(
     &    MASK0(IX+0+100*(IY+0-1),IZ+0).EQ.0
     &.OR.MASK0(IX+1+100*(IY+0-1),IZ+0).EQ.0
     &.OR.MASK0(IX+0+100*(IY+1-1),IZ+0).EQ.0
     &.OR.MASK0(IX+1+100*(IY+1-1),IZ+0).EQ.0
     &.OR.MASK0(IX+0+100*(IY+0-1),IZ+1).EQ.0
     &.OR.MASK0(IX+1+100*(IY+0-1),IZ+1).EQ.0
     &.OR.MASK0(IX+0+100*(IY+1-1),IZ+1).EQ.0
     &.OR.MASK0(IX+1+100*(IY+1-1),IZ+1).EQ.0
     &)GOTO 9
      V=A*(
     & +(1.-WZ)*((1.-WY)*(
     &               (1.-WX)*FLOAT(IMAG0(IX+0+100*(IY+0-1),IZ+0))
     &                   +WX*FLOAT(IMAG0(IX+1+100*(IY+0-1),IZ+0)))
     &               +WY*(
     &               (1.-WX)*FLOAT(IMAG0(IX+0+100*(IY+1-1),IZ+0))
     &                   +WX*FLOAT(IMAG0(IX+1+100*(IY+1-1),IZ+0))))
     &      +WZ*((1.-WY)*(
     &               (1.-WX)*FLOAT(IMAG0(IX+0+100*(IY+0-1),IZ+1))
     &                   +WX*FLOAT(IMAG0(IX+1+100*(IY+0-1),IZ+1)))
     &               +WY*(
     &               (1.-WX)*FLOAT(IMAG0(IX+0+100*(IY+1-1),IZ+1))
     &                   +WX*FLOAT(IMAG0(IX+1+100*(IY+1-1),IZ+1))))
     &)
      IF(LDIF.EQ.0)RETURN
      IF(
     &    MASK0(IX-1+100*(IY+0-1),IZ+0).EQ.0
     &.OR.MASK0(IX-1+100*(IY+1-1),IZ+0).EQ.0
     &.OR.MASK0(IX-1+100*(IY+0-1),IZ+1).EQ.0
     &.OR.MASK0(IX-1+100*(IY+1-1),IZ+1).EQ.0
     &)GOTO 9
      DVDXP=.5*A*(
     & +(1.-WZ)*((1.-WY)*(
     &                      -FLOAT(IMAG0(IX-1+100*(IY+0-1),IZ+0))
     &                      +FLOAT(IMAG0(IX+1+100*(IY+0-1),IZ+0)))
     &               +WY*(
     &                      -FLOAT(IMAG0(IX-1+100*(IY+1-1),IZ+0))
     &                      +FLOAT(IMAG0(IX+1+100*(IY+1-1),IZ+0))))
     &      +WZ*((1.-WY)*(
     &                      -FLOAT(IMAG0(IX-1+100*(IY+0-1),IZ+1))
     &                      +FLOAT(IMAG0(IX+1+100*(IY+0-1),IZ+1)))
     &               +WY*(
     &                      -FLOAT(IMAG0(IX-1+100*(IY+1-1),IZ+1))
     &                      +FLOAT(IMAG0(IX+1+100*(IY+1-1),IZ+1))))
     &)/MMPPIX(1)
      IF(
     &    MASK0(IX+0+100*(IY-1-1),IZ+0).EQ.0
     &.OR.MASK0(IX+1+100*(IY-1-1),IZ+0).EQ.0
     &.OR.MASK0(IX+0+100*(IY-1-1),IZ+1).EQ.0
     &.OR.MASK0(IX+1+100*(IY-1-1),IZ+1).EQ.0
     &)GOTO 9
      DVDYP=.5*A*(
     & +(1.-WZ)*(       -(
     &               (1.-WX)*FLOAT(IMAG0(IX+0+100*(IY-1-1),IZ+0))
     &                   +WX*FLOAT(IMAG0(IX+1+100*(IY-1-1),IZ+0)))
     &                  +(
     &               (1.-WX)*FLOAT(IMAG0(IX+0+100*(IY+1-1),IZ+0))
     &                   +WX*FLOAT(IMAG0(IX+1+100*(IY+1-1),IZ+0))))
     &      +WZ*(       -(
     &               (1.-WX)*FLOAT(IMAG0(IX+0+100*(IY-1-1),IZ+1))
     &                   +WX*FLOAT(IMAG0(IX+1+100*(IY-1-1),IZ+1)))
     &                  +(
     &               (1.-WX)*FLOAT(IMAG0(IX+0+100*(IY+1-1),IZ+1))
     &                   +WX*FLOAT(IMAG0(IX+1+100*(IY+1-1),IZ+1))))
     &)/MMPPIX(2)
      IF(
     &    MASK0(IX+0+100*(IY+0-1),IZ-0).EQ.0
     &.OR.MASK0(IX+1+100*(IY+0-1),IZ-0).EQ.0
     &.OR.MASK0(IX+0+100*(IY+1-1),IZ-0).EQ.0
     &.OR.MASK0(IX+1+100*(IY+1-1),IZ-0).EQ.0
     &)GOTO 9
      DVDZP=A*(
     &         -((1.-WY)*(
     &               (1.-WX)*FLOAT(IMAG0(IX+0+100*(IY+0-1),IZ-0))
     &                   +WX*FLOAT(IMAG0(IX+1+100*(IY+0-1),IZ-0)))
     &               +WY*(
     &               (1.-WX)*FLOAT(IMAG0(IX+0+100*(IY+1-1),IZ-0))
     &                   +WX*FLOAT(IMAG0(IX+1+100*(IY+1-1),IZ-0))))
     &         +((1.-WY)*(
     &               (1.-WX)*FLOAT(IMAG0(IX+0+100*(IY+0-1),IZ+1))
     &                   +WX*FLOAT(IMAG0(IX+1+100*(IY+0-1),IZ+1)))
     &               +WY*(
     &               (1.-WX)*FLOAT(IMAG0(IX+0+100*(IY+1-1),IZ+1))
     &                   +WX*FLOAT(IMAG0(IX+1+100*(IY+1-1),IZ+1))))
     &)/MMPPIX(3)
      DVDPA(1)=-(ROT(1,1,8)*DVDXP+ROT(2,1,8)*DVDYP+ROT(3,1,8)*DVDZP)
      DVDPA(2)=-(ROT(1,2,8)*DVDXP+ROT(2,2,8)*DVDYP+ROT(3,2,8)*DVDZP)
      DVDPA(3)=-(ROT(1,3,8)*DVDXP+ROT(2,3,8)*DVDYP+ROT(3,3,8)*DVDZP)
      DVDPA(4)=DVDXP*(ROT(1,1,4)*X+ROT(1,2,4)*Y+ROT(1,3,4)*Z)
     &        +DVDYP*(ROT(2,1,4)*X+ROT(2,2,4)*Y+ROT(2,3,4)*Z)
     &        +DVDZP*(ROT(3,1,4)*X+ROT(3,2,4)*Y+ROT(3,3,4)*Z)
      DVDPA(5)=DVDXP*(ROT(1,1,5)*X+ROT(1,2,5)*Y+ROT(1,3,5)*Z)
     &        +DVDYP*(ROT(2,1,5)*X+ROT(2,2,5)*Y+ROT(2,3,5)*Z)
     &        +DVDZP*(ROT(3,1,5)*X+ROT(3,2,5)*Y+ROT(3,3,5)*Z)
      DVDPA(6)=DVDXP*(ROT(1,1,6)*X+ROT(1,2,6)*Y+ROT(1,3,6)*Z)
     &        +DVDYP*(ROT(2,1,6)*X+ROT(2,2,6)*Y+ROT(2,3,6)*Z)
     &        +DVDZP*(ROT(3,1,6)*X+ROT(3,2,6)*Y+ROT(3,3,6)*Z)
      RETURN
    9 LVALID=.FALSE.
      RETURN
      END
      INTEGER*4 FUNCTION LI(IX,IY,IMAGE)
      INTEGER*2 IMAGE(100,100)
      IF(IMAGE(IX,IY).GE.1000)THEN
        LI=1
      ELSE
        LI=0
      ENDIF
      RETURN
      END
      FUNCTION FI(IX,IY,IMAGE)
      INTEGER*2 IMAGE(100,100)
      FI=FLOAT(IMAGE(IX,IY))
      RETURN
      END
      SUBROUTINE VALU3D
     &(MMPPIX,NSLICE,MASK0,IMAG0,I,J,K,PARAM,V,LVALID,LDIF,DVDPA)
      REAL*4 MMPPIX(3)
      PARAMETER (NPIX=10112)
      INTEGER*2 IMAG0(NPIX,NSLICE)
      INTEGER*2 MASK0(NPIX,NSLICE)
      PARAMETER (NTERM=6)
      REAL*4 PARAM(12),DVDPA(NTERM)
      LOGICAL*4 LVALID
      REAL*4 ANG/-10./
      COMMON/ACOM/A
      X=MMPPIX(1)*FLOAT(I)-PARAM(10)-PARAM(1)
      Y=MMPPIX(2)*FLOAT(J)-PARAM(11)-PARAM(2)
      C=COS(PARAM(6))
      S=SIN(PARAM(6))
      XPM=(C*X-S*Y)/MMPPIX(1)
      YPM=(S*X+C*Y)/MMPPIX(2)
      IX=NINT(XPM+PARAM(10)/MMPPIX(1)-.5)
      IF(IX.LE.1.OR.IX.GE.100)GOTO 9
      WX=XPM+PARAM(10)/MMPPIX(1)-FLOAT(IX)
      IY=NINT(YPM+PARAM(11)/MMPPIX(2)-.5)
      IF(IY.LE.1.OR.IY.GE.100)GOTO 9
      WY=YPM+PARAM(11)/MMPPIX(2)-FLOAT(IY)
      IZ=K
      LVALID=.TRUE.
      LV    =LI(IX+0,IY+0,MASK0(1,IZ+0))
     &      *LI(IX+1,IY+0,MASK0(1,IZ+0))
     &      *LI(IX+0,IY+1,MASK0(1,IZ+0))
     &      *LI(IX+1,IY+1,MASK0(1,IZ+0))
      IF(LV.EQ.0)GOTO 9
      V=A*(
     &           (1.-WY)*(
     &                    (1.-WX)*FI(IX+0,IY+0,IMAG0(1,IZ+0))
     &                        +WX*FI(IX+1,IY+0,IMAG0(1,IZ+0)))
     &               +WY*(
     &                    (1.-WX)*FI(IX+0,IY+1,IMAG0(1,IZ+0))
     &                        +WX*FI(IX+1,IY+1,IMAG0(1,IZ+0)))
     &)
      IF(LDIF.EQ.0)RETURN
      LV    =LI(IX-1,IY+0,MASK0(1,IZ+0))
     &      *LI(IX-1,IY+1,MASK0(1,IZ+0))
      IF(LV.EQ.0)GOTO 9
      DVDXP=.5*A*(
     &           (1.-WY)*(
     &                           -FI(IX-1,IY+0,IMAG0(1,IZ+0))
     &                           +FI(IX+1,IY+0,IMAG0(1,IZ+0)))
     &               +WY*(
     &                           -FI(IX-1,IY+1,IMAG0(1,IZ+0))
     &                           +FI(IX+1,IY+1,IMAG0(1,IZ+0)))
     &)/MMPPIX(1)
      LV    =LI(IX+0,IY-1,MASK0(1,IZ+0))
     &      *LI(IX+1,IY-1,MASK0(1,IZ+0))
      IF(LV.EQ.0)GOTO 9
      DVDYP=.5*A*(
     &                  -(
     &                    (1.-WX)*FI(IX+0,IY-1,IMAG0(1,IZ+0))
     &                        +WX*FI(IX+1,IY-1,IMAG0(1,IZ+0)))
     &                  +(
     &                    (1.-WX)*FI(IX+0,IY+1,IMAG0(1,IZ+0))
     &                        +WX*FI(IX+1,IY+1,IMAG0(1,IZ+0)))
     &)/MMPPIX(2)
      DVDPA(1)=-(C*DVDXP-S*DVDYP)
      DVDPA(2)=-(S*DVDXP+C*DVDYP)
      DVDPA(6)=DVDXP*(-S*X-C*Y)+DVDYP*(C*X-S*Y)
      RETURN
    9 LVALID=.FALSE.
      RETURN
      END
