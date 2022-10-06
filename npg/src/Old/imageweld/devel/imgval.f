C$Log$
      
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

      rcsheader = "$Id$"

      call vrt2img(mmppix,center,c)
      call trotset(dist,ang,trot)

c     compute [a] = [c][trot]

      do 1 i=1,4
      	do 1 j=1,4
      		a(i,j)=0.
      		do 1 k=1,4
    1 			a(i,j)=a(i,j)+c(i,k)*trot(k,j)

      call img2vrt(mmppix,center,b)

c     compute [t4] = [a][b]

      do 2 i=1,4
      	do 2 j=1,4
      		t4(i,j)=0.
     			do 2 k=1,4
    2 			t4(i,j)=t4(i,j)+a(i,k)*b(k,j)
      return
      end
