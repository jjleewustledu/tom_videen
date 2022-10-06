CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Program:      IMGROT.f
C                Avi Snyder 29-Jun-93
C
C  Logical Units:
C      6   process listing (stdout)
C      5   process input (stdin)
C     10   used for I/O
C
C  Uses Subroutines:
C     split
C     ALIGNH1
C     POLFIT
C     RMSSUB
C     RMSSUB1
C     IMGROTO
C     IMGROTE
C     IMGROTR
C     MATOPR
C
C  Uses libraries:
C     petutil for GETIMG
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      PROGRAM IMGROT

      PARAMETER (NBYTES = 20224)
      PARAMETER (NPIX   = 10112)
      PARAMETER (NSLMAX = 60)

      REAL*4       PARAM(12)   ! alignment parameters
      pointer (pIMAG,IMAG),(pMASK,MASK),(pIMAGNEW,IMAGNEW)
      INTEGER*2    IMAG(NPIX,NSLMAX)
      INTEGER*2    MASK(NPIX,NSLMAX)
      INTEGER*2    IMAGNEW(NPIX,NSLMAX)
      INTEGER*2    HDR(128)    ! input headers
      INTEGER*2    IN(NPIX)    ! input image file
      INTEGER*4    PB(6)/6*0/  ! parameter block for SYSIO
      REAL*4 MMPPIX(3),DUMMY3(3)
      REAL*4 FOVMM(3)/200.,200.,110./	! aperture in mm
      REAL*4 RAXIS(3),ROT(3,3,8)
      LOGICAL*1 ERR
      LOGICAL*1 HDRONLY/.TRUE./
      LOGICAL*4 LMMPPIX
      CHARACTER*256 AFILSPC,MFILSPC,DFILSPC
      CHARACTER*160 STRING
      character*20 id1
      INTEGER*4 IARRAY(3)      !FOR CALL TO itime

      DATA IOREAD   /'00000058'X/      ! read sequential, bin, wait
      DATA IOWRITE  /'00000038'X/      ! write sequential, bin, wait
      DATA LU/10/                      ! data I/O

      COMMON /ROTCOM/ROT
      COMMON /ACOM/A

      DEGPERAD=45./ATAN(1.)
   10 CALL itime(IARRAY)
      TYPE "(' TIME NOW',3I3)",IARRAY
      call free(pIMAG)
      call free(pIMAGNEW)
      call free(pMASK)
      NSLICE=0
  120 FORMAT(A256)
      write(*,"(' ENTER SCAN FILE NAME')")
      STRING=' '
      READ(5,"(A)",end=90)STRING
      CALL MPARSE(STRING,AFILSPC,MMPPIX,LMMPPIX)
      LA=lnblnk(AFILSPC)
      IF(LA.EQ.0)GOTO 90
      CALL GETIMG(IN,HDR,AFILSPC,1,NSLICE,NPET,LU,6,HDRONLY,ERR)  !HDRONLY=.TRUE.
      if(ERR)STOP
      IF(NSLICE.GT.NSLMAX) STOP 'IMGROT SLICE LIMIT EXCEEDED'
      pIMAG=malloc(NBYTES*NSLICE)
      pMASK=malloc(NBYTES*NSLICE)            
      IF(LMMPPIX)THEN
        CALL IMAGEGET(AFILSPC,LA,HDR,IMAG,NSLICE,DUMMY3)
      ELSE
        CALL IMAGEGET(AFILSPC,LA,HDR,IMAG,NSLICE,MMPPIX)
      ENDIF
      IF(NSLICE.LT.0)STOP
      NSLLO=1
      NSLHI=NSLICE
      write(*,"(' ENTER SCAN FILE ON WHICH TO BASE MASK')")
      READ(5,"(A160)")STRING
      CALL TPARSE(STRING,MFILSPC,THRESHT,NSLLO,NSLHI)
      IF(MFILSPC(1:1).EQ.'*')THEN
        CALL ALLONE(LT,MASK,NSLICE)
      ELSE
        CALL IMAGEGET(MFILSPC,LT,HDR,MASK,NSL,DUMMY3)
        IF(NSL.NE.NSLICE)STOP 'NSLICE MISMATCH'
        WRITE(6,"(' IMAGE SLICES',I3,' TO',I3)")NSLLO,NSLHI
        CALL ATTENY(THRESHT,MASK,NSLICE,NSLLO,NSLHI)
      ENDIF
      WRITE(6,"(' IMGROT version 04-Jul-93')")
      write(*,"(' MMPPIX',3F10.6)")MMPPIX
      CALL ALIGNC(MMPPIX,NSLICE,IMAG,MASK,PARAM(07),NPIXA,TA)
      A=1000./TA
      WRITE(6,"(' IMAGE CENTER mm',3F10.4)")(PARAM(I),I=7,9)

c      goto 601
      CALL IMGROTO(FOVMM,MMPPIX,NSLICE,IMAG,MASK,PARAM,E1)

      T=SQRT(PARAM(1)**2+PARAM(2)**2+PARAM(3)**2)
      WRITE(6,"(' TRANSLATION',F8.4,' mm   DIRECTION',3F8.4)")
     &T,(PARAM(J)/T,J=1,3)
      RAXIS(1)=.5*(-ROT(2,3,8)+ROT(3,2,8))
      RAXIS(2)=.5*(+ROT(1,3,8)-ROT(3,1,8))
      RAXIS(3)=.5*(-ROT(1,2,8)+ROT(2,1,8))
      R=SQRT(RAXIS(1)**2+RAXIS(2)**2+RAXIS(3)**2)
      WRITE(6,"(' ROTATION   ',F8.4,' deg       AXIS',3F8.4)")
     &R*DEGPERAD,(RAXIS(J)/R,J=1,3)
      CALL ALIGNR(MMPPIX,NSLICE,IMAG,MASK,PARAM,RMSROT)
      WRITE(6,"(' ANG MOTION ',F8.4,' mm')")RMSROT
      WRITE(6,"(' TOTAL      ',F8.4,' mm')")SQRT(RMSROT**2+T**2)

C     IF(LD.LE.0)GOTO 10
  601 continue
      DFILSPC=AFILSPC
      DFILSPC(LA+1:LA+4)='.ima'
      pIMAGNEW=malloc(NBYTES*NSLICE)
      CALL IMGROTS(MMPPIX,NSLICE,IMAG,MASK,PARAM,IMAGNEW)
      OPEN(LU,FILE=DFILSPC,ACCESS='DIRECT',RECL=256,ERR=19)
      DO 71 M=1,NSLICE
      CALL SYSIO(PB,IOWRITE,LU,HDR,256,0)
      CALL SYSIO(PB,IOWRITE,LU,IMAGNEW(1,M),NBYTES,0)
   71 CONTINUE
      CLOSE(LU)
      call namget(DFILSPC,id1)
      WRITE(6,122)id1
  122 FORMAT(' NEW IMAGE ',A20)
      GOTO 10
   19 WRITE(6,123)DFILSPC(1:80)
  123 FORMAT(' ERROR OPENING ',A80)
      GOTO 10
   90 STOP 'END IMGROT'
      END

      subroutine namget(string,id)
      character*(*) string,id
      l=lnblnk(string)
c      n=rindex(string(1:l),'/')	! THIS SYSTEM SUBROUTINE DOES NOT WORK
      do 1 n=l,1,-1
    1 if(string(n:n).eq.'/')goto 2
      n=0
    2 id=string(n+1:)
c      write(*,"('nameget: ',i4,2x,a)")n,string(1:80)
      return
      end
       
      SUBROUTINE MPARSE(STRING,AFILSPC,MMPPIX,LMMPPIX)
      CHARACTER*(*) STRING
      CHARACTER*(*) AFILSPC
      CHARACTER*80 SUBSTR(8)
      REAL*4 MMPPIX(3)
      LOGICAL*4 LMMPPIX

      CALL split(STRING,SUBSTR," ",NF)
      IF(NF.LE.0)THEN
        AFILSPC=' '
        RETURN
      ENDIF
      AFILSPC=SUBSTR(1)
      write(*,"(a80)")AFILSPC(1:80)
      LMMPPIX=.FALSE.
      DO 2 N=2,NF
      L=lnblnk(SUBSTR(N))
      IF(INDEX(SUBSTR(N),'voxsiz=').GT.0)THEN
        DECODE(L-7,"(3F)",SUBSTR(N)(8:L))MMPPIX
        LMMPPIX=.TRUE.
        write(*,"(' VOXSIZ',3F10.6)")MMPPIX
      ENDIF
    2 CONTINUE
      RETURN
      END

      SUBROUTINE TPARSE(STRING,MFILSPC,THRESHT,NSLLO,NSLHI)
      CHARACTER*(*) STRING
      CHARACTER*(*) MFILSPC
      CHARACTER*80 SUBSTR(8)

      CALL split(STRING,SUBSTR," ",NF)
      IF(NF.LE.0)THEN
        MFILSPC=' '
        RETURN
      ENDIF
      MFILSPC=SUBSTR(1)
      write(*,"(a80)")MFILSPC(1:80)
      IF(INDEX(MFILSPC,'TR').GT.1.OR.
     &   INDEX(MFILSPC,'tr').GT.1)THEN
        THRESHT=.7
      ELSE
        THRESHT=.35
      ENDIF
      DO 2 N=2,NF
      L=lnblnk(SUBSTR(N))
      IF(INDEX(SUBSTR(N),'%=').GT.0)THEN
        DECODE(L-2,"(I)",SUBSTR(N)(3:L))J
        THRESHT=FLOAT(J)/100.
      ENDIF
      IF(INDEX(SUBSTR(N),'slices=').GT.0)THEN
        I=INDEX(SUBSTR(N),'to')
        IF(I.LE.0)THEN
          WRITE(*,"('IMGROT: slices specification error')")
        ELSE
          DECODE(I-8,"(I)",SUBSTR(N)(8:I-1))NSLLO
          DECODE(L-I-1,"(I)",SUBSTR(N)(I+2:L))NSLHI
        ENDIF
      ENDIF
    2 CONTINUE
      RETURN
      END

      SUBROUTINE IMAGEGET(AFILSPC,LA,HDR,IMAG,NSLICE,MMPPIX)
      PARAMETER (NPIX   = 10112)
      CHARACTER*256 AFILSPC
      character*20 id
      INTEGER*2    HDR(128)    ! input headers
      INTEGER*2    IMAG(NPIX,NSLICE)
      REAL*4 MMPPIX(3)
      INTEGER*2 IMG(128,128)
      LOGICAL*1 ERR,EX
      LOGICAL*1 HDRONLY/.FALSE./
      integer*4 fd,getfd               ! FORTRAN C interface
   
      external get_voxsize !$pragma C(get_voxsize)

      DATA LU/10/                      ! data I/O

      call namget(AFILSPC,id)
      LA=INDEX(AFILSPC,'.')-1
      INQUIRE(FILE=AFILSPC,EXIST=EX,ERR=29)
      IF(.NOT.EX)GOTO 29
      M=0
   20 M=M+1
      CALL GETIMG(IMG,HDR,AFILSPC,M,NSLICE,NPET,LU,6,HDRONLY,ERR)
      IF(ERR)GOTO 29
      CALL F128100(IMG,IMAG(1,M))
      IF(M.LT.NSLICE)GOTO 20
      write(*,102)id,NSLICE,NPET
  102 FORMAT(' READ ',A20,I4,' SLICES PET=',I4)
      IF(NPET.EQ.6.AND.NSLICE.EQ.7)THEN
        MMPPIX(1)= 2.86
        MMPPIX(2)= 2.86
        MMPPIX(3)=14.40
        GOTO 27
      ENDIF
      IF(NPET.EQ.6.AND.NSLICE.EQ.31)THEN
        MMPPIX(1)= 2.607767
        MMPPIX(2)= 2.607767
        MMPPIX(3)= 3.375
        GOTO 27
      ENDIF
      IF(NPET.EQ.6.AND.NSLICE.EQ.49)THEN
        MMPPIX(1)= 2.00
        MMPPIX(2)= 2.00
        MMPPIX(3)= 2.00
        GOTO 27
      ENDIF
      IF(NPET.EQ.10)THEN                         ! ECAT 953b
        OPEN(LU,FILE=AFILSPC,STATUS='OLD',ERR=29)
        fd=getfd(LU)
        CALL get_voxsize(%VAL(fd),AFILSPC,MMPPIX)! returned in cm/pix
        CLOSE(LU)
        DO I=1,3
        MMPPIX(I)=MMPPIX(I)*10.                  ! convert to mm/pix
        ENDDO
        GOTO 27
      ENDIF
      write(*,"(' MMPPIX UNDEFINED IN IMAGE')")
      RETURN
   27 write(*,"(' MMPPIX',3F10.6)")MMPPIX
      RETURN
   29 WRITE(*,103)AFILSPC(1:80)
  103 FORMAT(' CANNOT FIND ',A80)
      NSLICE=-1
      RETURN
      END
