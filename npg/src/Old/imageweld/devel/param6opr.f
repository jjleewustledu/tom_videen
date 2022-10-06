C$Log$

      SUBROUTINE ang2rot(ang,rot)
      real*4 ang(3),rot(3,3)
      c1=cos(ang(1))
      s1=sin(ang(1))
      c2=cos(ang(2))
      s2=sin(ang(2))
      c3=cos(ang(3))
      s3=sin(ang(3))
      rot(1,1)= c3*c2
      rot(1,2)=-c3*s2*s1-s3*c1
      rot(1,3)=-c3*s2*c1+s3*s1
      rot(2,1)= s3*c2
      rot(2,2)=-s3*s2*s1+c3*c1
      rot(2,3)=-s3*s2*c1-c3*s1
      rot(3,1)= s2
      rot(3,2)= c2*s1
      rot(3,3)= c2*c1
      return
      end

      SUBROUTINE img2vrt(MMPPIX,CENTER,C)
      REAL*4 MMPPIX(3),CENTER(3),C(4,4)
      DO 1 I=1,4
      	DO 1 J=1,4
    1 		C(I,J)=0.
      DO 2 I=1,3
      	C(I,I)=MMPPIX(I)
    2 	C(I,4)=-CENTER(I)
      C(4,4)=1.
      RETURN
      END

      SUBROUTINE vrt2img(MMPPIX,CENTER,C)
      REAL*4 MMPPIX(3),CENTER(3),C(4,4)
      DO 1 I=1,4
      	DO 1 J=1,4
    1 		C(I,J)=0.
      DO 2 I=1,3
      	C(I,I)=1./MMPPIX(I)
    2 	C(I,4)=CENTER(I)/MMPPIX(I)
      C(4,4)=1.
      RETURN
      END

      SUBROUTINE TROTSET(DIST,ANG,TROT)
      REAL*4 DIST(3),ANG(3),TROT(4,4)
      REAL*4 ROT(3,3)
      CALL ang2rot(ANG,ROT)
      DO I=1,3
        DO J=1,3
          TROT(I,J)=ROT(I,J)
        ENDDO
        TROT(I,4)=DIST(I)
      ENDDO
      DO 7 J=1,3
    7 	TROT(4,J)=0.
      TROT(4,4)=1.
      RETURN
      END
