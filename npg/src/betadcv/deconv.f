C$Header: /home/npggw/tom/src/betadcv/RCS/deconv.f,v 2.2 2008/11/14 20:40:31 tom Exp $
C$Log: deconv.f,v $
c Revision 2.2  2008/11/14  20:40:31  tom
c added countrate correction for well counter nonlinearity
c
c Revision 2.1  2007/03/20  22:33:36  tom
c change prompt in bldenter
c
c Revision 2.0  2004/02/13  19:45:46  tom
c Feb 2004
c
c Revision 1.2  2002/11/27  22:01:56  tom
c *** empty log message ***
c
c Revision 1.1  1995/10/03  18:37:03  tom
c Initial revision
c
c Revision 1.3  1993/12/02  22:30:02  tom
c Fix rcsheader
c
c Revision 1.2  1993/12/02  22:22:34  tom
c Added libpe utilities.
c Allow all scan types (co, oo, etc.)
c Add extra points of oxygen
c Add RCSHEADER
c
      SUBROUTINE CONV(F,G,H,N,A)
C     CALCULATE H=F*G given F,G
      REAL*4 H(N),G(N),F(N),A(2*N+4)
			CHARACTER*256	RCSHEADER
			RCSHEADER = "$Id: deconv.f,v 2.2 2008/11/14 20:40:31 tom Exp $"
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
      COMMON /USRIO/ USERIN,USEROUT
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
    4 WRITE(USEROUT,101) I,F(I),G(I),H(I)
  101 FORMAT(I6,3F10.4)
      CALL DECONV(H,G,F,N,A)
      DO 5 I=1,N
    5 WRITE(USEROUT,101) I,F(I),G(I),H(I)
      STOP
      END
