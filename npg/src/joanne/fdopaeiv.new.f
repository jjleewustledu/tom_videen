C
C   ***  THIS VERSION ASSUMES NO MISSING SCAN DATA 
C   the end of each scan must match the beginning of the next scan
C    
C     Modified:     Joanne Markham  Oct 2002
C                to use EIV regression with the
C                assumption that both X and Y have errors
C               with variance of all errors the same constant
C               value and all errors uncorrelated
C
c   MODIFIED May 2006  
c               change to use .tac generated from .dat files
c   MODIFIED:   Joanne Markham  March 1998
c
c   Graphical /PATLAK method for  FLUORO-DOPA studies
c
c    1)  Assumes irreversible binding of ligand
c  
c    2)  Use reference area (non-specific binding region)
c        instead of blood curve
c
c 
c
c  INPUT:
c    1.   start and ending times of interval to be used
c         for linear fit (units --min) 
c    2.   name of tissue activity file for reference and 
c          receptor regions , ref is 1 tissue region (3rd col)
C         (may contain data for up to 20 regions)
C
C
C    All tac have same number of data points 
C
C
C***************************************************************
C  EQUATION:  
C
C  Cref(t)  -  tissue activity for reference  region 
C  CT(t)     -  tissue activity for receptor regions 
C
C   
C     CT(t)/  Cref(t)   =   R0 +  Ki [  INT (Cref(t) ] / Cref(t) 
C
C      where Ki = k3 R0 ,
C      
C      [  INT (Cref(t) ] is the integral of the reference curve 
C                        from t=0 to t 
C
C      Linear fit yields estimates of R0 and Ki 
C     
C      Although k3 could be computed by  slope/intercept,
C      the value of  Ki is usually less variable than
C      the value of k3 computed in this way.
C
C      R0 is the ratio of the free/nonspecifically bound  activity
C      in the receptor region to the activity in the reference 
C      region; this ratio is assumed to be constant.  Note that the
C      equation can be derived using this assumption, although the
C      original derivation was different.
C
C   References:  
C
C   1)  Patlak CS, J Cereb Blood Flow Metab, 5:584, 1985.   
C
C   2)  Hoshi H, J Cereb Blood Flow Metab, 13: 57, 1993 
C
C

C****************************************************************
C  
C 1.  read input: 
C
C
C 2.  Call subroutine REFREG to integrate the reference region.
C     The integration is performed for PET data so there
C     is no error due to approximation. However, this means
C     that the  integrals are defined at the end of the interval
C     instead of midpoint.  Interpolation is then used to find
C     integral (and reference region intensity) at receptor 
C     region times. (This is more accurate than performing 
C     integration from midpoint data.)
C
C 4.  Find starting and ending data points for fit;
C     pass starting number and number of points fof fit.
C
C 5.  Call subroutine fdopa for each receptor region. 
C     Fdopa computes "PATLAK" XY data and call subroutine
C     to perform regression.
C  
C 6.   Save "PATLAK" XY data in file named xxx.xyplots.
C      all data points and the set used for fitting along
C      with the fitted Y values are saved.  
C      A summary table of the results is also save in a
C      file named xxx.fit.
c
c________________________________________________________________
C   VARIABLES
C
C
C
C
C  FILE NAMES:  (assumed to <80 characters); 
C 
C  reffile  -  input file for reference region
C  tisfile  -  input file for receptor regions
C  fitfile,xyfile  --output files : created using name of 
C                    receptor region file with last extension
C                    replaced by .xyplots & .fit
C
C  rheader  -  id for reference region activity data
C  theader  -  id for receptor region activity data
C
C    nop are number of data points read from
C    reference region and receptor regions files
C
C   Data values WILL BE interpolated/deleted as necessary to match
C   the time of the data points in the receptor region file
C
C  ARRAYS:
C
C   (from time info file)
C  tmid     -   midpoint of scan interal (will be computed in program)
C  tstart   -   starting time of scan
C  scanlen  -   duration time of each scan
C
C       
C  refact  -  input activity for reference region
C  ref     -  reference region activity at times matching receptor region
C  rint    -  integral of reference region activity at times 
C             matching receptor region 
C  
C  tisact  -  tissue activity for receptor regions
C

        character*80 rheader, theader, header
        character*80  tisfile,fitfile,xyfile
        integer nop
	real  tmid(200),tstart(200),scanlen(200),
     1  refact(200),ref(200),rint(200),  tisact(200,20)
        

c
c
c    
10	continue 
         read  (5,*,end=990) fstart,fend 
          write (6,*) ' INTERVAL FOR FIT ', fstart,fend
          write (6,*)
          write (6,*)'                 INPUT FILES '      
          write (6,*)
         read (5,1001,end=920)tisfile
1001     format (a80)
         write (6,1001)  tisfile

          write (6,*) 
          fstartm= fstart
          fendm = fend
 	  fstart = fstart*60
	  fend = fend *60.

c 
c      

20       continue
c
c
c      READ tissue activity data for reference & receptor region
c      and get time information at same time
c

          open (2,file=tisfile,status='old',err=950)
          read (2,1001)rheader
          read (2,*) nop, nc
	  nreg =nc -3
        do  40 i=1,nop 
        read (2,*) tstart(i),scanlen(i),refact(i),(tisact(i,j),j=1,nreg)
40      tmid(i) = tstart(i) + scanlen(i)/2.0
        close (unit=2)

C
C
C

        write (6,*) ' tmid '
        write (6,1031) (tmid(i),i=1,nop)
1031    format (3f12.2)
C
C
C   For reference region:  
C   Compute integral of reference curve 
C   Interpole both integral and intensity curves
C   to match receptor region times
C

        call refreg (nop,tmid,tstart,scanlen,
     1  refact,rint) 

c
c        
c        find start and ending time in time curve
c 
c             write (6,*) ' fstart, fend ', fstart,fend
     
        do 300 i =1,nop
300     if (tmid(i).gt. fstart) go to 320
320     nstart = i
            do 330 i=nstart+1 , nop
	     if (tmid(i) .ge. fend) go to 340
330          continue
	     i =  nop +1
340          nfit = i-nstart 
            write (6,*) ' nstart, nfit ', nstart,nfit

C
C    CREATE FILE NAMES FOR OUTPUT FILES --replace
C    last extension with xyplots and fit

           call fname (tisfile,fitfile,xyfile)
           write (6,*) 
           write (6,*) '                OUTPUT FILES   '
           write (6,1001)  fitfile
            write (6,1001)  xyfile
C
C   WRITE Summary table in file named xxx.fit
C         and xyplots in file named   xxx.xyplots
C           
           open (2,file = fitfile, status='unknown', err=970)     
           open (4,file = xyfile,   status='unknown', err=980)  
C          write (6,1024)

           write (4,*) '                 INPUT FILE  '
           write (4,*)
           write (4,1001) tisfile
           write (4,*)
           write (4,*) '                 OUTPUT FILES  '
           write (4,1001)  fitfile
           write (4,1001)  xyfile
           write (4,*)
           write (4,*) ' INTERVAL FOR FIT (min) ', fstartm,fendm
           write (4,*)
           write (4,*) ' fit from data point # ', nstart
           write (4,*) ' # of points fitted ', nfit
           write (4,*)
1024       format (  '  Region     B0         Ki (/min) ',
     1     '   Ki/B0 (/min)     COV(%)       R' )
            
           write (2,1024) 
           write (2,*)  
           do 500 j=1,nreg
        call fdopa (j,nop,tmid,refact,rint, tisact(1,j),
     1  nstart,nfit,slope,b0,cov,rr)
          slope = slope *60.
          temp = slope/b0 
          
c         write (6,1021) j,b0,slope,temp,cov,rr
          write (2,1021) j,b0,slope,temp,cov ,rr
1021   format (i6,3e14.5,f11.2,f11.3) 
500    continue
        write (2,*)
        write (2,*)
        write (2,*)'  *** EIV (errors-in-variables) REGRESSION used'


	  close (unit=2)
          close (unit =4) 

               go to 10
990     continue
		 stop
910      write (6,*)' EOF while reading name of time info file '
         stop 3
920     write (6,* )' EOF while reading name of ',
     1  ' reference region file '
         stop 3
930      write (6,*)' EOF while reading name of TISCAT file '
         stop 3

950     write (6,* )' ERROR while opening REF region  file '
         write (6,1001 )reffile
         stop 4
960     write (6,* )'  ERROR while opening TISACT file '
          stop 4
970     write (6,*) ' ERROR while opening output file '
        write (6,1001) fitfile
        stop 4

980     write (6,*) ' ERROR while opening xyplot file '
        write (6,1001) xyfile
         stop 4

        end
C
C
C
c  Joanne Markham  March 1999 
c
c  1)   compute integral of reference region
c
c  2)   interpolate both integral and intensity activity
C       of reference region to match receptor region times
c
c
 
	subroutine refreg (nop,tmid,tstart,scanlen,
     1  refact,refint)

        integer nop
        real tmid(200),tstart(200),scanlen(200),
     1  refact(200),refint(200)
	

	
c       local variables 
c
        integer nint
   	real rend(200),tint(200),frint(200),rint(200)
      
         do 50 i=1,nop
         tint(i) = tstart(i) + scanlen(i)
50       rend(i) = tstart(i)+scanlen(i)
c
c        

1033   format (4f15.2)
C
C
c        
c      compute integral of reference tissue area curve
c     first: use 0 value 1st point
C     first  compute values for integral over interval 
C     store in frint frame integral
           
	  do 150 i =1 ,nop   
           frint(i) =  refact(i)*scanlen(i) 
150   	    continue

1012   format (f10.4,2e20.7)

C
C      Integrate up to end of each frame 
C      start with 0 for first point 
C
	  nint =nop
	  j=2 
	  sum = frint(1)
	  do 180 i=1,nop
          rint(i) = sum
          sum = sum+frint(i+1)
180       continue
           write (6,*) ' INTEGRAL OF REF REG: tmid, frame total '
           write (6,1012) (tmid(i),frint(i),rint(i),i=1,nint )

C
C   INTERPOLATE INTEGRAL OF 
C   REFERENCE CURVE TO MATCH Midpoint TIMES FOR RECEPTOR REGIONS
C
             do 500 i=1,nop
             tt = tmid(i)
              
             do 300 j=1,nint  
             if (tint(j) .ge. tt) go to 440
300       continue
440          if (j.eq.1) then     
             refint(i) = rint(1)*tt/tint(1)
             go to 500 
             endif
             jm1 = j-1
             rat = (tt-tint(jm1))/(tint(j) - tint(jm1))
             refint(i) = rint(jm1) + rat*(rint(j)-rint(jm1))
500          continue

             write (6,*) ' tmid, ref ,refint '
             do 520 i=1,nop
 520         write (6,1041) tmid(i),refact(i),refint(i) 
1041     format (2f12.2,e20.6)
              return
	       end

c
c   MODIFIED:     Joanne Markham  Jan 1999 
c
c     from PATLAK method 
c     for FLUORO-DOPA studies
c   Use reference area (non-specific binding)
c     instead of blood curve
c
c
c   Subroutine for one region 
c   Assume reference region has been processed 
c   and all data interpolated so that times for data
c   points match the receptor region curves
c
 

c
	subroutine fdopa (ireg,nop,time,ref,rint,tisact,
     1  nstart,nfit,slope,b0,cov,rr)

        integer ireg,nop,nstart,nfit
	real time(200),ref(200),tisact(200), rint(200),
     1  slope,b0,cov,rr
	
c       local variables 
c
	real   x(200),y(200), p(2),se(2),fit(200)
c
c        
C
C  ireg    -  region #
C  nop     -  number of data points for time,ref,rint,tisact
C  time    -  midpoint times (without decay correction)
C  ref     -  reference region activity
C  rint    -  integral of reference region activity
C  tisact  -  tissue activity for receptor region
C  nstart  -  data point # for start of fit
C  nfit    -  number of data points to be fit
C  slope   -  estimated slope
C  b0      -  estimated intercept
C  cov     -  coefficient of variation for slope  (%) 
C  rr      -  correlation coefficient of data 
c
c   Y = tisact(i)/refact(i)
c
c   X = integral of refact(i) / refact(i)
c  
c    INTERPOLATION FOR BOTH INTENISTY AND INTEGRAL OF 
C    REFERENCE CURVE MUST BE COMPUTED IN subroutine REFREG 
c
c
1033   format (4e15.2)

	   do 400 i = 1,nop 

          x (i) = rint(i)/ref(i)

          y (i) = tisact(i)/ref(i)
400        continue

         write (4,*) ' normalized XY plots : region ' ,ireg
           do 410 i=1,nop
410          write (4,1034) x(i),y(i)
1034   format (2f20.5)
c
c
c  fit straight line to data
c
c
C
C   estimated parameters are p(1) = B0 , p(2) = slope
C   SE is standard error of B0 and slope 
C   rr = correlation coef
C  
C   for eiv model don't use rmse --not correct objective
C
      call fiteiv (nfit,x(nstart),y(nstart),fit,p,se,rr)
        slope = p(2)
        b0 = p(1)
        write (4,*) ' STRAIGHT LINE FIT:  '
        write (4,*) ' intercept & slope       = ', p(1),p(2)
        write (4,*) ' standard error of estimate', se(1),se(2)
        temp1 = 100.*se(1)/p(1) 
        temp2 = 100.*se(2)/p(2) 
        cov = temp2
        write (4,*) ' COV of estimates (%)       ', temp1, temp2
        temp = p(2)/p(1)
        write (4,*) ' correlation coef ', rr
        jj = nstart + nfit -1
        write (4,*) 
       write (4,*) ' XY data and fit; start and # points ' , nstart,nfit
        do 500 i=nstart,jj
       write (4,1020)  x(i),y(i),fit(i+1-nstart)
1020   format (3f20.3)
          
500    continue

990	   return
	   end
C
C
C     ADD .fit to file name for output
C     If file has no extension (.xxx) then
C     add .fit to end of name
C     If file has .xxx remove last characters and
C      put fit instead 
C
        subroutine fname (inname,fitname,xyname)

        Character*80 inname, fitname,xyname

C
C  LOCAL VARIABLES
C
         Character*80 tempname,tempchar
         character*1  aname(80),bname(80)
          
          equivalence (aname(1),tempname)
          equivalence (bname(1),tempchar)
          tempname = inname
          tempchar = inname
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
         aname(i+1) = 'f'
         aname(i+2) = 'i'
         aname(i+3) = 't'

         bname(i+1) = 'x'
         bname(i+2) = 'y'
         bname(i+3) = 'p'
         bname(i+4 )= 'l'
         bname(i+5) = 'o'
         bname(i+6) = 't'
         bname(i+7) = 's'
    
          do 220 j=i+4,80
         aname(j) = ' '
220      continue
         fitname= tempname
          do 240 j=i+8,80
          bname(j) = ' '
240      continue
         xyname=tempchar 
         return
300      continue
c
c
c find first blank
c
c
       do 310 i=1,80
310       if (aname(i).eq. ' ') go to 320     
          i = 77
320    continue
         aname(i) = '.'
         bname(i) = '.'

         go to 200
          end
	  include "subeiv.f"

