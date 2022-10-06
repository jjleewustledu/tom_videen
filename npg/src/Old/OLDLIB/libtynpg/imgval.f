c$Id: imgval.f,v 1.4 1994/03/03 20:48:51 ty7777 Exp $
c$Log: imgval.f,v $
c Revision 1.4  1994/03/03  20:48:51  ty7777
c Replace id with header.
c
c Revision 1.3  1994/01/07  21:37:46  ty7777
c Change back for b and c arrays.
c
c Revision 1.2  1994/01/07  16:17:35  ty7777
c Fix the mmppix problem related with arrays b and c.
c
c Revision 1.1  1993/10/25  17:43:12  ty7777
c Initial revision
c
c
      subroutine t4set(mmppix,dist,ang,center,t4)
c     Avi Snyder 02-10-93
c
c     computes composite transform matrix (real*4 t4) to used by imgval
c     t4 including the following steps:
c     1. transform from image to virtual coordinates
c     2. rotate 
c     3. translate (nudge)
c     4. transform back to image coodinates
c    
c     subroutines vrt2img, trotset and img2vrt are in module param6opr.f

      real*4 mmppix(3),dist(3),ang(3),center(3),t4(4,4)
      real*4 trot(4,4),a(4,4),b(4,4),c(4,4)
      character*256 rcsheader
c
      rcsheader = '$Header: /home/petsun4/ty7777/src/libnpg/RCS/imgval.f,v 1.4 1994/03/03 20:48:51 ty7777 Exp $'
c
      call vrt2img(mmppix,center,c)
c     The following corrects for the fact that rmsimg uses a 100 x 100
c     slice arrays whereas C library functions use 128 x 128 arrays.
      c (1, 4) = c (1, 4) + 14.0
      c (2, 4) = c (2, 4) + 14.0
      call trotset(dist,ang,trot)
c     compute [a] = [c][trot]
      do 1 i=1,4
      do 1 j=1,4
      a(i,j)=0.
      do 1 k=1,4
    1 a(i,j)=a(i,j)+c(i,k)*trot(k,j)
      call img2vrt(mmppix,center,b)
      b (1, 4) = b (1, 4) - 14.0 * mmppix (1)
      b (2, 4) = b (2, 4) - 14.0 * mmppix (2)
c     compute [t4] = [a][b]
      do 2 i=1,4
      do 2 j=1,4
      t4(i,j)=0.
      do 2 k=1,4
    2 t4(i,j)=t4(i,j)+a(i,k)*b(k,j)
      return
      end

      subroutine imgval(T4,IMAG,NX,NY,NSLICE,I,J,K,V,LSLICE)
c     Avi Snyder 02-10-93
c     Returns in V the interpolated value of IMAG 
c                                     at transformed coordinates (I,J,K).   
c     real*4 IMAG(NX,NY,NSLICE)
c     If T4 = [I] then V = IMAG(I,J,K).
c     T4 must be set (by a call to t4set) before imgval is called.
c     LSLICE returns the slice from which most of the data is taken.
c     LSLICE = 0 (and V = 0.) if the image is undefined at transformed (I,J,K).

      real*4 IMAG(NX,NY,NSLICE),T4(4,4)
      REAL*4 X(3),XP(3)
      X(1)=FLOAT(I)
      X(2)=FLOAT(J)
      X(3)=FLOAT(K)
      DO L=1,3
        XP(L)=T4(L,1)*X(1)+T4(L,2)*X(2)+T4(L,3)*X(3)+T4(L,4)
      ENDDO
      IX=NINT(XP(1)-.5)
      WX=XP(1)-FLOAT(IX)
      IF(IX.EQ.0.AND.WX.GT.0.999)THEN
        IX=1
        WX=0.
      ENDIF
      IF(IX.EQ.NX.AND.WX.LT.0.001)THEN
        IX=NX-1
        WX=1.
      ENDIF
      IF(IX.LT.1.OR.IX.GE.NX)GOTO 9
      IY=NINT(XP(2)-.5)
      WY=XP(2)-FLOAT(IY)
      IF(IY.EQ.0.AND.WY.GT.0.999)THEN
        IY=1
        WY=0.
      ENDIF
      IF(IY.EQ.NY.AND.WY.LT.0.001)THEN
        IY=NY-1
        WY=1.
      ENDIF
      IF(IY.LT.1.OR.IY.GE.NX)GOTO 9
      IZ=NINT(XP(3)-.5)
      WZ=XP(3)-FLOAT(IZ)
      IF(IZ.EQ.0.AND.WZ.GT.0.999)THEN
        IZ=1
        WZ=0.
      ENDIF
      IF(IZ.EQ.NSLICE.AND.WZ.LT.0.001)THEN
        IZ=NSLICE-1
        WZ=1.
      ENDIF
      IF(IZ.LT.1.OR.IZ.GE.NSLICE)GOTO 9
      V=
     & +(1.-WZ)*((1.-WY)*(
     &               (1.-WX)*IMAG(IX+0,IY+0,IZ+0)
     &                   +WX*IMAG(IX+1,IY+0,IZ+0))
     &               +WY*(
     &               (1.-WX)*IMAG(IX+0,IY+1,IZ+0)
     &                   +WX*IMAG(IX+1,IY+1,IZ+0)))
     &      +WZ*((1.-WY)*(
     &               (1.-WX)*IMAG(IX+0,IY+0,IZ+1)
     &                   +WX*IMAG(IX+1,IY+0,IZ+1))
     &               +WY*(
     &               (1.-WX)*IMAG(IX+0,IY+1,IZ+1)
     &                   +WX*IMAG(IX+1,IY+1,IZ+1)))
      LSLICE=IZ
      IF(WZ.GT.0.5)LSLICE=LSLICE+1
      RETURN
    9 V=0.
      LSLICE=0
      RETURN
      END
