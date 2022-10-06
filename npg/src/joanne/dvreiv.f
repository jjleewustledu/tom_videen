C      Modified:     Joanne Markham  Oct 2002
C                to use EIV regression with the
C                assumption that both X and Y have errors
C               with variance of all errors the same constant
C               values and all errors uncorrelated
C 
C
C   Modified:     Joanne Markham  May 2002
C                 for new format for NMB data 
C               all tissue data in one file
C
c   MODIFIED:     Joanne Markham  Feb 1999
c  as proposed by Logan 1996 (JCBFM 16:p 834)
c
c   MODIFIED:     Joanne Markham  March 1998
c
c     from PATLAK method 
c   Use reference area (non-specific binding)
c    instead of blood curve
C ******************************************************
C   NOTE:  This version assume that Cref/k2 becomes
C          a constant after some time and can be 
C          absorbed into the intercept term
C ******************************************************
C
c 1.  read start and stop times for slope fit
C     times entered in mim --change to sec
C
C 2.  read all tissue data from one file 
C     file has : start time, scan length, 
C     then tracer activity for reference region and 
C     all specific binding regions.
C
c 3.  compute  integral of reference region
c 
C  for each region compute 
C
C 4.  compute X and Y and then estimate slope
C   and intercep
C
C 5.  write 2 output files:  one with all calculations
C    and another with BP values only
c
C
C
C
C     VARIABLES:
C
C     fstart, fend -- start and end time for fit
C     scanlen      -- duration of scan 
C     tstart       -- start times for scans
C     tend         -- end times for scans 
C     tmid         -- midpoint of scans
C     ref          -- activity curve for reference region
C     tisact (I,j) -- tissue activity for receptor region
C     nint         -- number of points in integrals (=nt+1)
C		      because first point is for t =0
C     tint         -- times for end of integral [tint(1) =0]
C     refint       -- integral of reference region activity
C     sint         -- integral of specific binding region
C
	 IMPLICIT NONE 
        INTEGER i,j,k,jj,nt,nc,nref,nreg,ireg,nint 
	INTEGER lu7, lu8
        character*80 tisfile, calfile,bpfile  
        REAL fstart,fend,temp,temp1,temp2,
     1  tt,ratio,rmse,rr,bp
	REAL tstart(200),tend(200),tmid(200),scanlen(200),
     1  ref(200), tisreg(20,200), tisact(200),
     2  x(200),y(200),refint(200),tint(200),sint(200),
     3  xl(200),yl(200),fit(100),p(2),se(2),bptab(20),
     4  covtab(20), corc(20),title(15)
        DATA LU7/7/,LU8/8/
c
c    
10	 read (5,*) fstart,fend 
	 read (5,1003)tisfile
         fstart = fstart*60.
         fend = fend *60.
1003     format (a80)
c 
c      read all tissue activity data
c      
	 open (4,file=tisfile,status='old',err=910)
1001    format (15a4)
	 read (4,1001) title
         read (4,*) nt,nc
         nreg = nc-3 
	 do  20 i=1,nt
	 read (4,*) tstart(i),temp,ref(i),(tisreg(j,i),
     1 	 j = 1, nreg)
	 scanlen(i) = temp
         tmid(i) = tstart(i) + temp/2.
         tend(i) = tstart(i) + temp
20       continue
        close (unit=4)
        nref = nt

          if (tstart(1).lt.0.)then
	  write (6,*) ' ERROR:  tstart(1) < 0 '
	  write (6,*) ' DATA will be corrected by starting at 0'
          temp = tstart(1) 
	  do 40 j=1,nt
	  tstart(j) = tstart(j) - temp
40        continue	 
          endif
c
c
c   compute integral of reference curve
c
       call intreg (nref,tmid,tstart,tend,scanlen,
     1       ref,nint,tint, refint)

         write (6,*) ' INPUT FILES '
         write (6,1001) title
	 write (6,*) ' # of data points ', nt
         write (6,*) ' # of regions  ', nreg


      	 call fname (tisfile, calfile, bpfile)
         write (6,1003) calfile
	 write (6,1003) bpfile
          open (LU7,file=CALFILE,status='unknown',err=920)

           write (LU7,*) ' INPUT FILES '
         write (LU7,1001) title
         write (LU7,*) ' # of data points ', nt
         write (LU7,*) ' # of regions  ', nreg

c
c 
c
c
        ireg = 1
50      continue
        do 60 i=1,nt
60	tisact(i) = tisreg(ireg,i)
c      
c
c
c   compute  integral of  receptor region curves 
C
 
        call  intreg(nt,tmid,tstart,tend,scanlen,
     1	tisact,nint,tint, sint)

c      write (6,*)
c      write (6,*) ' INTEGRAL OF REF & RECEPTOR REGIONS  '
c      write (6,1012) (tint(i),refint(i),sint(i),i=1,nref+1)
1012   format (f10.4,2e20.5)
c       write (6,*)
    

c     
c     
c   INTERPOLATE to find INTEGRAL VALUES AT MIDPOINTS 
c    
           do 220 i=1,nt
           tt=tmid (i)
           do 160 j=1,nint
           if (tint(j)- tt) 160,170, 180
160   continue
c    too few data points--just don't use last tissue point
c

        write (6,*) ' ERROR:  TIS time >REF time '
        write (6,*) ' omit last data points '
        nt = i
               go to 230           
170   continue
           x(i) = refint(j)
           y(i) = sint(j)
           go to 200
c
c    interpolate
c
180    continue
        if (j.eq. 1) then
           x(i) =  tt*refint(1)/tint(1)
           y(i) =  tt*sint(1)/tint(1) 
            go to 200
             endif
          k= j-1
          ratio = (tt-tint(k))/(tint(j)-tint(k))
         x(i) =  refint(k) + ratio*(refint(j) - refint(k))
         y(i) =  sint(k) + ratio*(sint(j) - sint(k))
200      continue

         x(i) = x(i) /tisact(i)
         y(i) = y(i) /tisact(i)
220      continue

230    continue



c    
c
c    now find data values to be fit
c

c           write (6,*) ' start and stop ', fstart, fend
            jj=0
           do 420 i =1,nt
           tt = tmid(i)
           if (tt.lt. fstart) go to 420
           if (tt.gt. fend) go to 440
           jj = jj +1
            xl(jj) =x(i)
            yl(jj) = y(i)  
c           w(jj) =1.0
420        continue
440        continue
c
c  fit straight line to data
c
c
      
      call fiteiv (jj,xl,yl,fit,p,se,rr)
        write (LU7,*) '_______________________________________',
     1  '_______________________________________'
	write (LU7,*)
        write (LU7,1032) ireg,rr
1032    format ('  straight line fit for REGION ',i5,
     1  / '         Correlation Coef =  ', f10.4 )
	 write (LU7,*)  
	  temp1 = abs(100.*se(1)/p(1))
	  temp2 = 100.*se(2)/p(2) 
        write (LU7,*) ' slope      = ', p(2), '   SE = ', se(2), 
     1  '    COV = ', temp2
        write (LU7,*) ' intercept  = ', p(1), '   SE = ', se(1),
     1  '        COV = ', temp1

        bp  = p(2)  -1.
        temp1 = 100.*se(2)/bp 
        write (LU7,*) ' BP = DVR -1  & COV(BP) ', bp ,temp1
        write (LU7,*) 
	write (LU7,*) '        X             Y             FIT-Y'
	do 500 i=1,jj
       write (LU7,1020)  xl(i),yl(i),fit(i)
1020   format (3f15.3)
          
500    continue
       corc(ireg) = rr
       bptab(ireg) = bp 
       covtab(ireg) = temp1
       if (ireg.eq. nreg) go to 600
       ireg = ireg +1
       go to 50
600    continue

       open (LU8,file=bpfile,status='unknown',err=930)
       write (LU8,*) ' REGIONS ACTIVITY DATA FILE '
       write (LU8,1003) tisfile
       write (LU8,*)
       write (LU8,*) '           DVR:  BP TABLE '
       write (LU8,*)
       write (LU8,*) '   Region #    DVR      COV[DVR]    ',
     1 '   BP        COV[BP]', '      R'
       write (LU8,*) '                           %         ',
     1 '               % '
		    

       write (LU8,*)

	  do 620 i=1, ireg
	  temp = bptab(i)+1
	  temp2 = covtab(i)*bptab(i)/temp
	  write (LU8,1071)i, temp, temp2, bptab(i),covtab(i),corc(i)
620       continue
1071    format (i7,4f12.2,f12.4)

            write (LU8,*)
	    write (LU8,*) 
        write (LU8,*)'  *** EIV (errors-in-variables) regression used'
	    close (unit=LU7)
	    close (unit=LU8)
	    write (6,*) ' END OF JOB '
	    go to 990
910     write (6,* )' error in reading tissue file '
	write (6,1003)tisfile
        stop 2
920     write (6,* )' error in opening CAL  file '
	write (6,1003)CALfile
	stop 3
930     write (6,* )' error in opening .BP  file '
	write (6,1003)BPfile
		stop 4

990	   stop
	   end

c
c   Subroutine to compute integral of activity in region 
c
c   assumes PET data so that integral over an interal is 
c   just Count-rate* interval 
c    allow for missing scans ; must interpolate for 
C    activity during missing time
C
	subroutine intreg (nop,time,start,tend,scanlen,
     1   	ref,nint,tint, rint)


        integer nop,nint
	real time(200),start(200),tend(200),ref(200),
     1  rint(200),tint(200),scanlen(200)
	
c       local variables 
c
c
c        
c       write (6,*)
c       write (6,*) ' in intreg--nop ',nop

1033   format (4f15.2)
c
c        
c      compute integral of reference tissue area curve
c 
          
          rint(1) =0.
          tint(1) =0.
          if (start(1) .ne.0) then
	  i1 = 1
	  rint(2) = ref(1)*(start(1)**2)/(2.0*time(1))
          tint(2) = start(1) 
	  rint(3) = rint(2) + ref(1)*scanlen(1)
          tint(3) = tend(1)
          j=3
           else
	  j=2
	  rint(2) = ref(1)*scanlen(1)
	  tint(2) = tend(1)
          endif

	  do 150 i =2 ,nop 
          j = j+1

        if (start(i).lt. tend(i-1)) then
        write (6,*) ' ERROR: in scan time data'
        write (6,*) ' scan starts before end of previous one'
        write (6,*) ' #, start, end of previous ', i,start(i),tend(i-1)
        stop 8
         endif


         if (start(i) .ne. tend(i-1)) then
          temp =  (start(i) - tend(i-1))*(ref(i) + ref(i-1))/2. 
          else
           temp =0.
            endif
	   rint(j) = rint(j-1) + ref(i)*scanlen(i) +temp
           tint(j) = tend(i)
150   	    continue
            nint = j
c          write (6,*) ' INTEGRAL OF TIS REGION  '
c          write (6,1012) (tint(i),rint(i),i=1,nint )
1012   format (f10.4,e20.7)
c     

              return
	       end
C
C
C     find first . and replace with blanks up to 12 characters
C
        subroutine fname (inname,cname,pname)

        Character*80 inname,cname,pname

C
C  LOCAL VARIABLES
C
         Character*80 tempname
         character*1  aname(80),CEXT(4),PEXT(4),dot
          DATA CEXT/'C','A','L','B'/, DOT/'.'/,
     1	  PEXT/'B','P',' ',' '/
          equivalence (aname(1),tempname)
          tempname = inname

c
c   search for .
c
          do 100 i=80,2,-1
          if (aname(i).eq.'.') go to 200
100       continue
C
C      no . found
C

         go to 300

200      continue
	 ii = i+1
         do 220 j=ii,80
         aname(j) = ' '
220      continue
   
C
C  add extensions 
C
         i2 = ii+3 

	 do 230 j = ii,i2
	 aname(j) = CEXT(j+1 -ii)
230      continue

        cname = tempname

	do 240 j = ii,i2
        aname(j) = PEXT(j+1 -ii)
240      continue


       pname = tempname

        return
300      continue
         write (6,*) ' ERROR '
	 write (6,*) ' no . in filename '
         write (6,*) ' search for nonblank'
	 do  320 i=80,4,-1
         if (aname(i).ne.' ') go to 350 
320      continue     
           
350     continue
	 aname(i+1) = dot
	 i=i+1
	 go to 200
	 end
	 include "subeiv.f"
