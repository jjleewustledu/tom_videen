      SUBROUTINE CONV(F,G,H,N,A)
C     CALCULATE H=F*G given F,G
      REAL*4 H(N),G(N),F(N),A(2*N+4)
      CALL GCONV(F,G,H,N,A,.TRUE.)
      RETURN
      END
      SUBROUTINE DECONV(H,G,F,N,A)
C     CALCULATE F where H=F*G given H,G
      REAL*4 H(N),G(N),F(N),A(2*N+4)
      CALL GCONV(H,G,F,N,A,.FALSE.)
      RETURN
      END
      SUBROUTINE GCONV(H,G,F,N,A,L)
      REAL*4 H(N),G(N),F(N),A(2*N+4)
      LOGICAL*4 L
      COMPLEX*8 CH,CG
      J=N/2+1
      IHR=1
      IHI=IHR+J
      IGR=IHI+J
      IGI=IGR+J
      J=1
      DO 1 I=0,N/2-1
      A(IHR+I)=H(J)
      A(IGR+I)=G(J)
      J=J+1
      A(IHI+I)=H(J)
      A(IGI+I)=G(J)
      J=J+1
    1 CONTINUE
      CALL FFT(A(IHR),A(IHI),1,N/2,1,-1)
      CALL REALS(A(IHR),A(IHI),N/2,-1)
      CALL FFT(A(IGR),A(IGI),1,N/2,1,-1)
      CALL REALS(A(IGR),A(IGI),N/2,-1)
      DO 2 I=0,N/2
      CH=CMPLX(A(IHR+I),A(IHI+I))
      CG=CMPLX(A(IGR+I),A(IGI+I))
C     TYPE 601,I,CH,CG
C 601 FORMAT(I6,4F10.4)
      IF(L)THEN
      CH=CH*CG
      ELSE
      CH=CH/CG
      ENDIF
      A(IHR+I)=REAL(CH)
      A(IHI+I)=AIMAG(CH)
    2 CONTINUE
      CALL REALS(A(IHR),A(IHI),N/2,+1)
      CALL FFT(A(IHR),A(IHI),1,N/2,1,+1)
      J=1
      DO 3 I=0,N/2-1
      F(J)=A(IHR+I)
      J=J+1
      F(J)=A(IHI+I)
      J=J+1
    3 CONTINUE
      RETURN
      END
      SUBROUTINE CONVTST
      PARAMETER (N=32)
      REAL*4 H(N),G(N),F(N),A(2*N+4)
      DO 1 I=1,N/2
    1 F(I)=1.
      DO 2 I=N/2,N
    2 F(I)=0.
      DO 3 I=1,N
      T=FLOAT(I-1)*3./FLOAT(N)
    3 G(I)=EXP(-T)
      CALL CONV(F,G,H,N,A)
      DO 4 I=1,N
    4 TYPE 101,I,F(I),G(I),H(I)
  101 FORMAT(I6,3F10.4)
      CALL DECONV(H,G,F,N,A)
      DO 5 I=1,N
    5 TYPE 101,I,F(I),G(I),H(I)
      STOP
      END
