      SUBROUTINE MATMUL(A,B,C,N)
      REAL*4 A(N,N),B(N,N),C(N,N)
			type*,"N = ",N
      DO 2 I=1,N
      DO 2 J=1,N
      C(I,J)=0.
      DO 2 K=1,N
    2 C(I,J)=C(I,J)+A(I,K)*B(K,J)
      RETURN
      END
      SUBROUTINE MATMULG(A,L,M,B,N,C)
      REAL*4 A(L,M),B(M,N),C(L,N)
      DO 1 I=1,L
      DO 1 J=1,N
      C(I,J)=0.
      DO 1 K=1,M
    1 C(I,J)=C(I,J)+A(I,K)*B(K,J)
      RETURN
      END
      SUBROUTINE MATCOP(A,B,N)
      REAL*4 A(N,N),B(N,N)
      DO 1 I=1,N
      DO 1 J=1,N
    1 B(I,J)=A(I,J)
      RETURN
      END
      SUBROUTINE TRANSPOS(A,B,N)
      REAL*4 A(N,N),B(N,N)
      DO 1 I=1,N
      DO 1 J=1,N
    1 B(J,I)=A(I,J)
      RETURN
      END
      SUBROUTINE MATCON(A,B,N)
      REAL*4 A(N,N),B(N,N)
      DO 1 I=1,N
      DO 1 J=1,N
    1 B(J,I)=A(I,J)
      RETURN
      END
      SUBROUTINE MATMULC(A,B,C,N)
      COMPLEX*8 X,A(N,N),B(N,N),C(N,N)
      DO 2 I=1,N
      DO 2 J=1,N
      X=CMPLX(0.,0.)
      DO 1 K=1,N
    1 X=X+A(I,K)*B(K,J)
    2 C(I,J)=X
      RETURN
      END
      SUBROUTINE MATCONJG(A,B,N)
      COMPLEX*8 A(N,N),B(N,N)
      DO 1 I=1,N
      DO 1 J=1,N
    1 B(J,I)=CONJG(A(I,J))
      RETURN
      END
      SUBROUTINE MATINV(A,N,D)
C     ALGORITHM FOLLOWS Bevington, Phillip R.,
C     Data reduction and error analysis for the physical sciences,
C     McGraw-Hill, New York, 1969.
      REAL*4 A(N,N)
      INTEGER*4 IK(64),JK(64)
      D=1.
      DO 100 K=1,N
      AMAX=0.
   21 DO 30 I=K,N
      DO 30 J=K,N
      IF(ABS(AMAX).GT.ABS(A(I,J)))GOTO 30
      AMAX=A(I,J)
      IK(K)=I
      JK(K)=J
   30 CONTINUE
      I=IK(K)
      IF(I-K)21,51,43
   43 DO 50 J=1,N
      T=A(K,J)
      A(K,J)=A(I,J)
   50 A(I,J)=-T
   51 J=JK(K)
      IF(J-K)21,61,53
   53 DO 60 I=1,N
      T=A(I,K)
      A(I,K)=A(I,J)
   60 A(I,J)=-T
   61 DO 70 I=1,N
      IF(I.NE.K)A(I,K)=-A(I,K)/AMAX
   70 CONTINUE
   71 DO 80 I=1,N
      DO 80 J=1,N
      IF(I.NE.K.AND.J.NE.K)A(I,J)=A(I,J)+A(I,K)*A(K,J)
   80 CONTINUE
   81 DO 90 J=1,N
      IF(J.NE.K)A(K,J)=A(K,J)/AMAX
   90 CONTINUE
      A(K,K)=1./AMAX
  100 D=D*AMAX
      DO 130 L=1,N
      K=N-L+1
      J=IK(K)
      IF(J-K)111,111,105
  105 DO 110 I=1,N
      T=A(I,K)
      A(I,K)=-A(I,J)
  110 A(I,J)=T
  111 I=JK(K)
      IF(I-K)130,130,113
  113 DO 120 J=1,N
      T=A(K,J)
      A(K,J)=-A(I,J)
  120 A(I,J)=T
  130 CONTINUE
      RETURN
      END
      SUBROUTINE GENINV(A,G,B,N,DET)
C     Calculates B = inv(A) where A is any square matrix
C     G is a scratch array
      REAL*4 A(N,N),G(N,N),B(N,N)
      DO 2 I=1,N
      DO 2 J=1,N
      G(I,J)=0.
      DO 2 K=1,N
    2 G(I,J)=G(I,J)+A(K,I)*A(K,J)
      CALL MATINV(G,N,DET)
      DO 1 I=1,N
      DO 1 J=1,N
      B(I,J)=0.
      DO 1 K=1,N
    1 B(I,J)=B(I,J)+G(I,K)*A(J,K)
      RETURN
      END
