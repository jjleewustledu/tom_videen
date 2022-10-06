c 
c   for PET tissue curves only
c   Generate file with all regional data in it
C   Use originial .dat files 
C   Omit correction for decay 
C   Compare to results using .prm files
C    Input : file names--should be 5 files for now
C
C    .dat file  :  
C     col 1   midpint time with adjustment for decay correction
C     col 2   data 
C     col 3   start time for scan
C     col 4   scan length
C     col 5   counts before corrections: don't need
C
c   Joanne Markham   Jan 2006
c
c
c 
	character*80 filen
	dimension  tmid(100),tstart(100),sdel(100),tac(10,100)
	dimension title(20)
c
c    read file name 
c
	jj= 1
10       continue

        read (5,1001,end=500) filen 
1001    format (a80)

		   
c
c  open data file
c

       open (4,file=filen,status ='old',err =910)
c
c    skip 6 lines
C 
	      do 20 i=1, 6
	      read (4,*)
20    continue

          nop = 100
         do 100 i=1,nop
         read (4,*, end =120 )tmid(i),tac(jj,i),tstart(i),sdel(i)
100      continue
120      nop = i-1
	 close (4)
	 jj = jj +1 
	 go to 10
500   continue
        nr = jj-1
       write (6, * ) ' m xxxx  TAC , occip, LC , RC, LP, RP'
        j = nr +2
       write (6,*) nop, j
1020    format (f12.3,4f14.2)

          do 540 i=1,nop
	  write (6,1024)tstart(i),sdel(i),(tac(j,i),j=1,nr)
540      continue
1024     format (2f8.0, 8f11.3)
999	   stop
	 
910     write (6,*) ' error in opening time file '
	 stop 1
920     write (6,*) ' error in opening tissue activity file  '       
	 stop 2
	 end

