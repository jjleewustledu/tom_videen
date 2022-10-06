C
C   Joanne Markham  October 2002
C    Classical  Errors-in-variables model
C    assuming UNCORRELATED errors in both x and y
C    with constant variance and estimate for the 
C     ratio of the variance of y to x 
C    
C    ESTIMATE slope and intercept for this case
C    assume the ratio of variances is same 
C
C    must solve quadratic: check on sign? 
C    REF: Wayne A. Fuller: Measurement Error Models
C         John Wiley, 1987 pp 30-35
C
       subroutine fiteiv (nop,x,y,fit,p,se,rr)
       IMPLICIT NONE
       INTEGER nop
       REAL x(*),y(*),fit(*),p(2),se(2),rmse,rr

       INTEGER i,j
       REAL ratio,fn1,fn2,xm,ym
       REAL temp,tempx, tempy,r1,r2,b0,b1
       REAL varb0,varb1,covar,den
       REAL*8 TERM1,TERM2,TERM3
       REAL*8  Qxx, Qxy, Qyy,sumx,sumy
       REAL*8 svv,sigxx,siguu,siguv
c
c   compute means
c
	  ratio = 1.0
          sumx  =0.0d0
	  sumy  =0.0d0
	  do 20 i=1,nop
	  sumx = sumx +x(I)
20	  sumy = sumy + y(i)
          xm = sumx/nop
	  ym = sumy/nop
c         write (6,*) ' mean of x & y ', xm,ym
	  Qxx =0.0d0
          Qxy =0.0d0 
          Qyy =0.0d0

	  do 50 i=1,nop
	  tempx= x(i)-xm
	  tempy = y(i) - ym
	  Qxx = Qxx + tempx*tempx
	  Qxy = Qxy + tempx*tempy
	  Qyy = Qyy + tempy*tempy
50    continue
         fn1 = nop-1
         fn2 = fn1-1.0
         Qxx = Qxx/fn1
       	Qxy = Qxy/fn1
        Qyy = Qyy/fn1
c	 write (6,*) ' Qxx,  Qxy, Qyy', Qxx, Qxy, Qyy
C
C     solve quadratic for slope 
C
C   Eq:  s**2 [Qxy] - s [Qyy-R*Qxx] - R*Qxy = 0
C  R = ratio
         TERM1 = Qyy-ratio* Qxx 
	 TERM2 = TERM1**2 +4.0d0*ratio*Qxy*Qxy
	 TERM3 = dsqrt(TERM2)
         r1 = 0.5*(TERM1+TERM3)/Qxy	 
c        r2 = 0.5*(TERM1-TERM3)/Qxy
c	 write (6,*) ' r1, r2 ',r1, r2
    
C   if r1 < 1 error? 
	 b1 = r1
	 b0 = ym - xm*b1
	 p(1) = b0
	 p(2) = b1
	  
c	  write (6,*) ' b0, b1', b0, b1
          sigxx = Qxy/b1
C        
C   Alternative definition
C    sigxx = 0.5*( TERM3-TERM1)/ratio
C
	  sumx =0.0d0
	  sumy =0.0d0
	  do 120 i=1,nop
         fit(i) = b0 + b1*x(i) 
	 sumx = sumx + (y(i)-ym - b1*(x(i)-xm))**2
   	 sumy = sumy + (y(i)-b1*x(i) - b0 )**2

120       continue
	  svv = sumx/fn2
          sumy = sumy/(1.0+ b1*b1)
C  
C  compute siguu
C
	 siguu =0.5*(Qyy+ ratio*Qxx- TERM3)/ratio
         siguv = - b1*siguu 
c	 write (6,*) ' Sxx, Suu, Suv, Svv'
c	 write (6,*) sigxx,siguu,siguv,svv
       
C 
C    variance matrix 
C 
	den= fn1*sigxx**2
        varb1 = (svv*(sigxx+siguu) - siguv**2)/den
        varb0 = svv/nop + xm*xm*varb1
        covar = - xm*varb1
c        write (6,*) ' VARIANCE MATRIX '
c	write (6,*)  varb0, covar,varb1
          se(1)  = sqrt(varb0)
	  se(2)  = sqrt(varb1)
c	  write (6,*) ' se ', se(1),se(2)
C
C    compute correlation coefficient
C
C
        rr= Qxy/dsqrt(Qxx*Qyy)
c	write (6,*) ' correlation coef ', rr
990      return
	 end

