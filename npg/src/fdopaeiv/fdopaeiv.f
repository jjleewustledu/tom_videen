C  THIS VERSION ASSUMES NO MISSING SCAN DATA 
C  The end of each scan must match the beginning of the next scan
C    
C  Modified:     Joanne Markham  Oct 2002
C     to use EIV regression with the assumption that both X and Y have errors
C     with variance of all errors the same constant value and all errors uncorrelated
C
C  Modified:	May 2006  
C     change to use .tac generated from .dat files
C
C  Modified:	Joanne Markham  March 1998
C
C    Graphical /PATLAK method for  FLUORO-DOPA studies
C    1)  Assumes irreversible binding of ligand
C    2)  Use reference area (non-specific binding region) instead of blood curve
C
C  Input:
C    1) start and ending times of interval to be used for linear fit (minutes)
C    2) name of tissue activity file for reference and receptor regions;
C         reference is 1 tissue region and must be in the 3rd column of input file
C         may contain data for up to 20 regions)
C
C    All tac have same number of data points 
C
C***************************************************************
C  EQUATION:  
C
C  Cref(t) - tissue activity for reference region 
C  CT(t)   - tissue activity for receptor regions 
C
C  CT(t) / Cref(t) = R0 + Ki [ INT (Cref(t) ] / Cref(t) 
C
C  where:
C     Ki = k3 R0
C     [ INT (Cref(t) ] is the integral of the reference curve from t=0 to t
C
C  Linear fit yields estimates of R0 and Ki 
C     
C  Although k3 could be computed by slope/intercept, the value of Ki
C  is usually less variable than the value of k3 computed in this way.
C
C  R0 is the ratio of the free/nonspecifically bound activity in the receptor region
C     to the activity in the reference region; this ratio is assumed to be constant.
C  Note that the equation can be derived using this assumption, although the
C     original derivation was different.
C
C  References:  
C     Patlak CS, J Cereb Blood Flow Metab, 5:584, 1985.   
C     Hoshi H, J Cereb Blood Flow Metab, 13: 57, 1993 
C****************************************************************
C  
C  1)  Read input: 
C
C  2)  Call subroutine REFREG to integrate the reference region.
C      The integration is performed for PET data so there is no error due to approximation.
C      However, this means that the  integrals are defined at the end of the interval
C      instead of midpoint.  Interpolation is then used to find integral 
C      (and reference region intensity) at receptor region times. 
C      (This is more accurate than performing integration from midpoint data.)
C
C  4)  Find starting and ending data points for fit;
C      pass starting number and number of points fof fit.
C
C  5)  Call subroutine fdopa for each receptor region. 
C      Fdopa computes "PATLAK" XY data and call subroutine to perform regression.
C  
C  6)  Save "PATLAK" XY data in file named xxx.xyplots.
C      All data points and the set used for fitting along with the fitted Y values are saved.
C      A summary table of the results is also save in a file named xxx.fit.
C
C ________________________________________________________________
C  VARIABLES
C
C  FILE NAMES:  (assumed to <80 characters); 
C 
C  reffile  - input file for reference region
C  tisfile  - input file for receptor regions
C  fitfile,xyfile - output files
C  		created using name of receptor region file with last extension replaced by .xyplots & .fit
C  rheader  - id for reference region activity data
C  theader  - id for receptor region activity data
C  nop - number of data points read from reference region and receptor regions files
C
C  Data values WILL BE interpolated/deleted as necessary to match
C     the time of the data points in the receptor region file
C
C  ARRAYS:
C
C  (from time info file)
C  tmid    - midpoint of scan interal (will be computed in program)
C  tstart  - starting time of scan
C  scanlen - duration time of each scan
C  refact  - input activity for reference region
C  ref     - reference region activity at times matching receptor region
C  rint    - integral of reference region activity at times matching receptor region
C  tisact  - tissue activity for receptor regions

	character*80 rheader, theader, header
	character*80 tisfile,fitfile,xyfile
	integer nop
	real  tmid(200),tstart(200),scanlen(200)
	real refact(200),ref(200),rint(200),  tisact(200,20)

801	format ('Interval for fit = ',F6.1,' - ',F6.1,' min')
880	format (A80)

10	continue 
	read  (5,*,end=990) fstart,fend 
	write (6,801) fstart,fend
	write (6,*)
	read (5,880,end=920) tisfile
	write (6,*) 'INPUT FILE:'
	write (6,880) tisfile
	write (6,*) 
	fstartm = fstart
	fendm = fend
	fstart = fstart*60.
	fend = fend*60.

20	continue

C  READ tissue activity data for reference & receptor region
C  and get time information at same time

	open (2,file=tisfile,status='old',err=950)
	read (2,880)rheader
	read (2,*) nop, nc
	nreg =nc -3
	do 40 i=1,nop 
		read (2,*) tstart(i),scanlen(i),refact(i),(tisact(i,j),j=1,nreg)
		tmid(i) = tstart(i) + scanlen(i)/2.0
40	continue
	close (unit=2)

	write (6,*) ' tmid '
	write (6,810) (tmid(i),i=1,nop)
810	format (3f12.2)

C   For reference region:  
C   Compute integral of reference curve 
C   Interpolate both integral and intensity curves to match receptor region times

	call refreg (nop,tmid,tstart,scanlen,refact,rint)

C  Find start and ending time in time curve

	do 300 i =1,nop
		if (tmid(i).gt. fstart) go to 320
300	continue
320	nstart = i
	do 330 i=nstart+1 , nop
		if (tmid(i) .ge. fend) go to 340
330	continue
	i =  nop +1
340	nfit = i-nstart 
	write (6,*) ' nstart, nfit ', nstart,nfit

C  CREATE FILE NAMES FOR OUTPUT FILES --replace extension with xyplots and fit

	call fname (tisfile,fitfile,xyfile)
	write (6,*) 
	write (6,*) 'OUTPUT FILES:'
	write (6,880)  fitfile
	write (6,880)  xyfile

C   WRITE summary table in file named xxx.fit
C         and xyplots in file named   xxx.xyplots

	open (2,file = fitfile, status='unknown', err=970)     
	open (4,file = xyfile,   status='unknown', err=980)  

	write (4,*) 'INPUT FILE:'
	write (4,880) tisfile
	write (4,*)
	write (4,*) 'OUTPUT FILES:'
	write (4,880) fitfile
	write (4,880) xyfile
	write (4,*)
	write (4,*) ' INTERVAL FOR FIT (min) ', fstartm,fendm
	write (4,*)
	write (4,*) ' fit from data point # ', nstart
	write (4,*) ' # of points fitted ', nfit
	write (4,*)
820	format (  '  Region     B0         Ki (/min)    Ki/B0 (/min)     COV(%)       R' )
            
	write (2,820) 
	write (2,*)  
	do 500 j=1,nreg
		call fdopa (j,nop,tmid,refact,rint, tisact(1,j),nstart,nfit,slope,b0,cov,rr)
		slope = slope *60.
		temp = slope/b0 
		write (2,830) j,b0,slope,temp,cov ,rr
500	continue
830	format (i6,3e14.5,f11.2,f11.3) 
	write (2,*)
	write (2,*)
	write (2,*)'  *** EIV (errors-in-variables) REGRESSION used'

	close (unit=2)
	close (unit =4) 
	go to 10

990	continue
	stop
910	write (6,*) ' EOF reading name of time info file '
	stop 3
920	write (6,*) ' EOF reading name of reference region file'
	stop 3
930	write (6,*) ' EOF reading name of TISCAT file '
	stop 3
950	write (6,*) ' ERROR opening REF region  file '
	write (6,880) reffile
	stop 4
960	write (6,*) ' ERROR opening TISACT file '
	stop 4
970	write (6,*) ' ERROR opening output file '
	write (6,880) fitfile
	stop 4
980	write (6,*) ' ERROR opening xyplot file '
	write (6,880) xyfile
	stop 4
	end

C  SUBROUTINE REFREG
C
C  Joanne Markham  March 1999 
C
C  1) compute integral of reference region
C
C  2) interpolate both integral and intensity activity
C     of reference region to match receptor region times
 
	Subroutine refreg (nop,tmid,tstart,scanlen,refact,refint)

	integer nop
	real tmid(200),tstart(200),scanlen(200),refact(200),refint(200)
	
	integer nint 
	real rend(200),tint(200),frint(200),rint(200)
      
	do 50 i=1,nop
		tint(i) = tstart(i) + scanlen(i)
		rend(i) = tstart(i) + scanlen(i)
50	continue

C  Compute integral of reference tissue area curve
C  first: use 0 value 1st point
C  first  compute values for integral over interval 
C  store in frint frame integral
           
	do 150 i =1 ,nop   
		frint(i) = refact(i)*scanlen(i) 
150	continue

C  Integrate up to end of each frame; start with 0 for first point

	nint =nop
	j=2 
	sum = frint(1)
	do 180 i=1,nop
		rint(i) = sum
		sum = sum+frint(i+1)
180	continue
	write (6,*) ' INTEGRAL OF REF REG: tmid, frame total '
	write (6,840) (tmid(i),frint(i),rint(i),i=1,nint )
840	format (f10.4,2e20.7)

C  INTERPOLATE INTEGRAL of REFERENCE CURVE to match midpoint times for receptor regions

	do 500 i=1,nop
		tt = tmid(i)
		do 300 j=1,nint  
			if (tint(j) .ge. tt) go to 440
300		continue
440		if (j.eq.1) then     
			refint(i) = rint(1)*tt/tint(1)
			go to 500 
		endif
		jm1 = j-1
		rat = (tt-tint(jm1))/(tint(j) - tint(jm1))
		refint(i) = rint(jm1) + rat*(rint(j)-rint(jm1))
500	continue

	write (6,*) ' tmid, ref ,refint '
	do 520 i=1,nop
		write (6,850) tmid(i),refact(i),refint(i) 
520	continue
850	format (2f12.2,e20.6)
	return
	end

C  SUBROUTINE FDOPA
C
C  MODIFIED:  Joanne Markham  Jan 1999 
C
C  Use reference area (non-specific binding) instead of blood curve
C  from PATLAK method for FDOPA studies
C
C  Subroutine for one region 
C  Assume reference region has been processed and all data interpolated
C  so that times for data points match the receptor region curves
 
	Subroutine fdopa (ireg,nop,time,ref,rint,tisact, nstart,nfit,slope,b0,cov,rr)

	integer	ireg, nop, nstart, nfit
	real	time(200), ref(200), tisact(200), rint(200), slope, b0, cov, rr
	
	real	x(200), y(200), p(2), se(2), fit(200)

C  ireg    -  region number
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
C
C   Y = tisact(i)/refact(i)
C
C   X = integral of refact(i) / refact(i)
C  
C  INTERPOLATION for both INTENSITY and INTEGRAL of Reference Curve
C  must be computed in subroutine REFREG 

	do 400 i = 1,nop 
		x(i) = rint(i)/ref(i)
		y(i) = tisact(i)/ref(i)
400	continue

	write (4,*) ' normalized XY plots : region ' ,ireg
	do 410 i=1,nop
		write (4,860) x(i),y(i)
410	continue
860	format (2f20.5)

C  Fit straight line to data
C   estimated parameters are p(1) = B0 , p(2) = slope
C   SE is standard error of B0 and slope 
C   rr = correlation coef
C  
C   for eiv model don't use rmse --not correct objective

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
		write (4,870)  x(i),y(i),fit(i+1-nstart)
500	continue
870	format (3f20.3)

990	return
	end

C  SUBROUTINE FNAME
C
C  Add extensiont ".fit" to file name for output:
C    if file has no extension, then add .fit to end of name
C    if file has extension, then remove and substitute 'fit'

	Subroutine fname (inname,fitname,xyname)

	character*80 inname, fitname,xyname

	character*80 tempname,tempchar
	character*1  aname(80),bname(80)
          
	equivalence (aname(1),tempname)
	equivalence (bname(1),tempchar)
	tempname = inname
	tempchar = inname

C  Search for "."; if not found, go to 300

	do 100 i=80,2,-1
		if (aname(i).eq.'.') go to 200
100	continue
	go to 300
200	continue
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
220	continue
	fitname= tempname
	do 240 j=i+8,80
		bname(j) = ' '
240	continue
	xyname=tempchar 
	return
300	continue

C  find first blank

	do 310 i=1,80
310		if (aname(i).eq. ' ') go to 320     
	i = 77
320	continue
	aname(i) = '.'
	bname(i) = '.'

	go to 200
	end

C	include "subeiv.f"

