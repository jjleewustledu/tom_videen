c 
c   for PET tissue curves only
c   Generate file with all regional data in it
C   Use originial .dat files 
C   Omit correction for decay 
C   Compare to results using .prm files
C    Input : file names--should be 5 files for now
C
C    .dat file  :  
C     col 1   midpoint time with adjustment for decay correction
C     col 2   data 
C     col 3   start time for scan
C     col 4   scan length
C     col 5   counts before corrections: don't need
C
C   Joanne Markham   Jan 2006

	character*80	filen
	character*80	nam(20)
	dimension		tmid(100),tstart(100),sdel(100),tac(10,100)

	nn= 0
	jj= 1
10	continue

C Read file name from input list

810	format (a80)
	read (5, 810, end=50) filen 
	nn = nn+1
	nam(nn) = filen
		   
C Open data file; skip first 6 lines

	open (4, file=filen, status ='old', err =910)
	do 20 i=1, 6
		read (4,*)
20	continue

	nop = 100
	do 30 i=1,nop
		read (4,*, end =40)tmid(i),tac(jj,i),tstart(i),sdel(i)
30	continue
40	nop = i-1
	close (4)
	jj = jj +1 
	go to 10

50	continue
	nr = jj-1
	write (6, 820) nam(1), nam(2), nam(3), nam(4), nam(5), nam(6), nam(7), nam(8), nam(9), nam(10)
	j = nr +2
	write (6,*) nop, j
820	format ('  Start  Length ',12(X,A10))

	do 60 i=1,nop
		write (6,830)tstart(i),sdel(i),(tac(j,i),j=1,nr)
60	continue
830	format (2f8.0, 8f11.3)
	stop
	 
910	write (6,*) ' Error opening time file'
	stop 1
920	write (6,*) ' Error opening tissue activity file'
	stop 2
	end

