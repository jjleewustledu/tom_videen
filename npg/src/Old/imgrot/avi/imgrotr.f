      subroutine imgrotrt
      REAL*4 MMPPIX(3),PARAM(9)
      REAL*4 TROT(4,4),C(4,4),G(4,4),TINV(4,4)
      REAL*4 TROTINV(4,4),CINV(4,4),TROTC(4,4)

      data PARAM/.1,.2,.3,.4,.5,.6,100.,110.,150./
      data MMPPIX/2.5,2.5,3./

      DO 8 L=1,5
      CALL TROTCAL(+1,PARAM,TROT)
c      call imgrotrl(TROT)
      CALL TROTCAL(-1,PARAM,TROTINV)
c      call imgrotrl(TROTINV)
      CALL MATMUL(TROTINV,TROT,G,4)
      call imgrotrl(G)
      CALL img2vrt(MMPPIX,PARAM(7),C)
      CALL vrt2img(MMPPIX,PARAM(7),CINV)
      CALL MATMUL(CINV,C,G,4)
      call imgrotrl(G)
      call MATMUL(TROT,C,TROTC,4)
      call MATMUL(CINV,TROTINV,TINV,4)
      call MATMUL(TINV,TROTC,G,4)
      call imgrotrl(G)
      IF(MOD(L,2).EQ.0)GOTO 8
      DO I=1,6
        PARAM(I)=PARAM(I)*1.1
      ENDDO
    8 CONTINUE
      stop
      end
      subroutine imgrotrl(G)
      real*4 G(4,4)
      do i=1,4
      write(*,"(4f10.6))")(G(i,j),j=1,4)
      enddo
      write(*,"()")
      return
      end
      SUBROUTINE img2vrt(MMPPIX,CENTER,C)
      REAL*4 MMPPIX(3),CENTER(3),C(4,4)
      REAL*4 C0(3)/3*0./
      LOGICAL*4 INIT/.FALSE./
      IF(C0(1).EQ.CENTER(1).AND.C0(2).EQ.CENTER(2)
     &.AND.C0(3).EQ.CENTER(3).AND.INIT)RETURN
      DO 1 I=1,4
      DO 1 J=1,4
    1 C(I,J)=0.
      DO 2 I=1,3
      C(I,I)=MMPPIX(I)
    2 C(I,4)=-CENTER(I)
      C(4,4)=1.
      C0(1)=CENTER(1)
      C0(2)=CENTER(2)
      C0(3)=CENTER(3)
      INIT=.TRUE.
      RETURN
      END
      SUBROUTINE vrt2img(MMPPIX,CENTER,C)
      REAL*4 MMPPIX(3),CENTER(3),C(4,4)
      REAL*4 C0(3)/3*0./
      LOGICAL*4 INIT/.FALSE./
      IF(C0(1).EQ.CENTER(1).AND.C0(2).EQ.CENTER(2)
     &.AND.C0(3).EQ.CENTER(3).AND.INIT)RETURN
      DO 1 I=1,4
      DO 1 J=1,4
    1 C(I,J)=0.
      DO 2 I=1,3
      C(I,I)=1./MMPPIX(I)
    2 C(I,4)=CENTER(I)/MMPPIX(I)
      C(4,4)=1.
      C0(1)=CENTER(1)
      C0(2)=CENTER(2)
      C0(3)=CENTER(3)
      INIT=.TRUE.
      RETURN
      END
      SUBROUTINE TROTCAL(IFWDREV,PARAM,TROT)
      REAL*4 PARAM(6),TROT(4,4)
      REAL*4 PARAM0(3)/3*0./
      LOGICAL*4 INIT/.FALSE./
      LOGICAL*4 LT
      REAL*4 ROT(3,3,8)
      COMMON/ROTCOM/ROT
      CALL ROTCAL(PARAM)
      LT=INIT
      DO 3 I=1,6
    3 LT=LT.AND.PARAM0(I).EQ.PARAM(I)
      LT=LT.AND.I0.EQ.IFWDREV
      IF(LT)RETURN
      IF(IFWDREV.GE.0)THEN
        DO I=1,3
          DO J=1,3
            TROT(I,J)=ROT(I,J,8)
          ENDDO
          TROT(I,4)=PARAM(I)
        ENDDO
      ELSE
        DO I=1,3
          DO J=1,3
            TROT(I,J)=ROT(J,I,8)
          ENDDO
          TROT(I,4)=0.
          DO K=1,3
            TROT(I,4)=TROT(I,4)-ROT(K,I,8)*PARAM(K)
          ENDDO
        ENDDO
      ENDIF
      DO 7 J=1,3
    7 TROT(4,J)=0.
      TROT(4,4)=1.
      DO 4 I=1,6
    4 PARAM0(I)=PARAM(I)
      I0=IFWDREV
      INIT=.TRUE.
      RETURN
      END
