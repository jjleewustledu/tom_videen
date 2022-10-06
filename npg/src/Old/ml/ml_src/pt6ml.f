c***********************************************************************
c
c    PROGRAM TITLE		pt6ml.f
c
c    WRITTEN BY			David G. Politte
c    DATE WRITTEN		October 2, 1989
c    WRITTEN FOR		PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	     pt6ml (this program) and either pt6sieve or pt6sievecirc 
c	are the executables for two-dimensional image reconstruction of
c	data from PETT VI.  Together these programs compute maximum-
c	likelihood estimates of the desired image.  pt6ml performs the
c	iterative expectation-maximization algorithm.  The result is the
c	input for pt6sieve or pt6sievecirc, either of which applies a
c	linear filter called a sieve to produce an estimate of the
c	desired image.  This program has the following features:
c
c	a) The computations are done in a coordinate system that is
c	   rotated relative to the image.  This speeds the computations
c	   greatly.
c
c	b) Corrections for accidental coincidences and physical
c	   attenuation of photons are incorporated directly into the
c	   iterations of the expectation-maximization algorithm.  The
c	   data are not precorrected.
c
c	c) Data structures called "masks" are used to limit the spatial
c	   regions over which computations are performed.  If a
c	   numerical operation on an image pixel or data bin will not
c	   affect the final result then that operation is skipped.
c
c	d) Parameters that characterize the sampling and array sizes of
c	   the images and data, the transverse measurement error of the
c	   scanner, the masks, and the informational header are given in
c	   the file pt6include.f.h.
c
c
c    MAIN PROGRAM USAGE
c	This program is invoked with the follwing syntax:
c
c	pt6ml_2.87mm  iter_init(in)  iter_final(out)  emiss_uncor(in) \
c			surv_prob(in)  accid_data(in)  number_iter(in) \
c			[v v1 v2]
c
c		or
c
c	pt6ml_2.00mm  iter_init(in)  iter_final(out)  emiss_uncor(in) \
c			surv_prob(in)  accid_data(in)  number_iter(in) \
c			[v v1 v2]
c
c	where
c
c	iter_init(in)	is the file that contains an image representing
c			the initial guess.  (input)
c
c	iter_final(out)	is the file that contains the computed iterate
c			after the specified number of iterations.
c			(output)
c
c	emiss_uncor(in)	is the file that contains the uncorrected
c			emission data at each distance (d) and angle
c			(a).  (input)
c
c	surv_prob(in)	is the file that contains the survival
c			probability of pairs of photons at each distance
c			(d) and angle (a).  The survival probability is
c			computed using data from the transmission and
c			blank scans.  (input)
c
c	accid_data(in)	is the file that contains an estimate of the
c			accidental coincidences at each distance (d) and
c			angle (a).  (input)
c
c	number_iter(in)	is an integer parameter that is the number of
c			iterations of the EM algorithm to compute.
c			(input)
c
c	[v v1 v2]	is an optional argument that determines the
c			verbosity of the output.  Informational messages
c			are sent to the standard output if "v", "v1", or
c			"v2" is specified.  "v" and "v2" have identical
c			effects and are more verbose than "v1".  If this
c			argument is omitted or if something else is
c			specified then no messages are sent to the
c			standard output.  (input)
c
c
c    STANDARD I/O UNITS
c	standard input - not used
c
c	standard output - used, see [v v1 v2] option
c
c	standard error - used
c
c***********************************************************************


      program pt6ml


$alias fcreat_ = 'fcreat_' (%ref, %ref)
$alias fopenr_ = 'fopenr_' (%ref, %ref)
$alias fread_  = 'fread_'  (%ref, %ref, %ref, %ref, %ref)
$alias fseek_  = 'fseek_'  (%ref, %ref)
$alias fwrite_ = 'fwrite_' (%ref, %ref, %ref)
      include 'pt6include.f'
      character*80 progname, string
      integer*2 header(128)
      integer angle, backmaskxymax(yindexmin:yindexmax),
     +	 backmaskxymin(yindexmin:yindexmax), backmaskymax, backmaskymin,
     +	 baseindex, daindex, datamaskdmax, datamaskdmin, dindex, eof,
     +	 forwmaskdmax, forwmaskdmin, forwmasknumt(dindexmin:dindexmax),
     +	 iargc, iter, luaccid, luemiss, luinputiter, luoutputiter,
     +	 lusurvprob, maxsampkernd, nbdaarrayorig, nbxyimageorig, numarg,
     +	 numiter, rcnt, xyindex, yindex
      logical verbose1, verbose2
      real adjsum, backmaskdstart(maskbackindexmin:maskbackindexmax),
     +	 daaccid(daindexmin:daindexmax), daemiss(daindexmin:daindexmax),
     +	 darray1(dindexmin:dindexmax), darray2(dindexmin:dindexmax),
     +	 darray3(dindexmin:dindexmax), darray4(dindexmin:dindexmax),
     +	 darray5(dindexmin:dindexmax), darray6(dindexmin:dindexmax),
     +	 darray7(dindexmin:dindexmax),
     +	 dasmall(daindexminorig:daindexmaxorig),
     +	 dasurvprob(daindexmin:daindexmax),
     +	 dinc(angindexmin:angindexmax), fnumang,
     +	 forwmaskxstart(maskforwindexmin:maskforwindexmax),
     +	 forwmaskystart(maskforwindexmin:maskforwindexmax), fwhmb,
     +	 kernd(0:(numd - 1)), sampscale, sum, temp,
     +	 xinc(angindexmin:angindexmax), xyaccum(xyindexmin:xyindexmax),
     +	 xyiterate(xyindexmin:xyindexmax),
     +	 xyoversurvprob(xyindexmin:xyindexmax),
     +	 xysmall(xyindexminorig:xyindexmaxorig),
     +	 yinc(angindexmin:angindexmax)


c    Get the name of the program.

	 call igetarg(0, progname, 80)
	 call stripspace(progname)

c    Figure out if the program was invoked properly.

	 numarg = iargc()
	 if ((numarg .ne. 6) .and. (numarg .ne. 7))
     +	    then
	       write(7,100) progname, progname
	       call exit(1)
	 end if

c    Set the verbosity.

	 if (numarg .eq. 7)
     +	    then
	       call igetarg(7, string, 80)
	       if ((string(1:2) .eq. 'v ') .or.
     +		   (string(1:2) .eq. 'v2')) then
		  verbose1 = .true.
		  verbose2 = .true.
	       else if (string(1:2) .eq. 'v1') then
		  verbose1 = .true.  
		  verbose2 = .false.
	       else
		  verbose1 = .false.
		  verbose2 = .false.
	       end if
	    else
	       verbose1 = .false.
	       verbose2 = .false.
	 end if

	 if (verbose1)
     +	    then
	       write(6,200) progname
	 end if

c    Get names of files, strip trailing blanks, and open or create.
c    Also, read the number of iterations to compute.

	 call igetarg(1, string, 80)
	 call stripspace(string)
	 call fopenr_(string, luinputiter)
	 if (verbose1)
     +	    then
	       write(6,205) string
	 end if

	 call igetarg(2, string, 80)
	 call stripspace(string)
	 call fcreat_(string, luoutputiter)
	 if (verbose1)
     +	    then
	       write(6,210) string
	 end if

	 call igetarg(3, string, 80)
	 call stripspace(string)
	 call fopenr_(string, luemiss)
	 if (verbose1)
     +	    then
	       write(6,215) string
	 end if

	 call igetarg(4, string, 80)
	 call stripspace(string)
	 call fopenr_(string, lusurvprob)
	 if (verbose1)
     +	    then
	      write(6,220) string
	 end if

	 call igetarg(5, string, 80)
	 call stripspace(string)
	 call fopenr_(string, luaccid)
	 if (verbose1)
     +	    then
	       write(6,225) string
	 end if

	 call igetarg(6, string, 80)
	 read(string, *) numiter
	 if (verbose1)
     +	    then
	       write(6,230) numiter
	 end if

c    The following scale factor adjusts the image so that it has the
c    correct number of counts.

	 sampscale = (pix * pix) / (tsamp * dsamp)

c    Initialize arrays.

	 do 1000 xyindex = xyindexmin, xyindexmax
	    xyaccum(xyindex)	    = 0.0
	    xyoversurvprob(xyindex) = 0.0
 1000	 continue

	 do 1005 dindex = dindexmin, dindexmax
	    darray1(dindex) = 0.0
	    darray2(dindex) = 0.0
	    darray3(dindex) = 0.0
	    darray4(dindex) = 0.0
	    darray5(dindex) = 0.0
	    darray6(dindex) = 0.0
	    darray7(dindex) = 0.0
 1005	 continue

c    Make the masks.

	 call maskml(xinc, yinc, dinc, datamaskdmin, datamaskdmax,
     +	    backmaskymin, backmaskymax, backmaskxymin, backmaskxymax,
     +	    backmaskdstart, forwmaskdmin, forwmaskdmax, forwmasknumt,
     +	    forwmaskxstart, forwmaskystart)

c    Read the input iterate, pad it, and set pixels outside of the
c    reconstruction region to zero.  The array xyaccum is used here for
c    temporary storage only.

	 nbxyimageorig = numxyorig * realsize
	 call fseek_(luinputiter, nbhead)
	 call fread_(luinputiter, xysmall, nbxyimageorig, eof, rcnt)

	 call padimg(xysmall, xyaccum)

	 call circzero(xyaccum, backmaskymin, backmaskymax,
     +	    backmaskxymin, backmaskxymax, xyiterate)

c    Read the emission data and pad it.

	 nbdaarrayorig = numdaorig * realsize
	 call fread_(luemiss, header, nbhead, eof, rcnt)
	 call fread_(luemiss, dasmall, nbdaarrayorig, eof, rcnt)

	 call paddata(dasmall, daemiss)

c    Generate Gaussian kernel.

	 if (header(16) .eq. 0) then
	    fwhmb = fwhmblow
	    if (verbose1)
     +	       then
		  write(6,235)
	    end if
	 else if (header(16) .eq. 1) then
	    fwhmb = fwhmbhigh
	    if (verbose1)
     +	       then
		  write(6,240)
	    end if
	 else
	    write(7,105) progname
	    call exit(1)
	 end if
	 if (verbose1 .or. verbose2)
     +	    then
	       write(6,245)
	 end if
	 call genkern(fwhmb, dsamp, maxsampkernd, kernd)

c    Read the survival probability data and pad it.  Then compute an
c    unfiltered back-projection to find the average survival probability
c    at each pixel.  Finally, because the algorithm requires a division
c    by the average survival probability, compute the reciprocal so that
c    we can multiply instead.

	 call fseek_(lusurvprob, nbhead)
	 call fread_(lusurvprob, dasmall, nbdaarrayorig, eof, rcnt)

	 call paddata(dasmall, dasurvprob)

	 do 2000 angle = angindexmin, angindexmax
	    baseindex = angle * numd
	    do 2005 dindex = datamaskdmin, datamaskdmax
	       daindex = baseindex + dindex
	       darray1(dindex) = dasurvprob(daindex)
 2005	    continue
	    call conv1d(darray1, kernd, maxsampkernd, datamaskdmin,
     +	       datamaskdmax, darray2)
	    call backproj(darray2, angle, backmaskymin, backmaskymax,
     +	       backmaskxymin, backmaskxymax, backmaskdstart, dinc,
     +	       xyoversurvprob)
 2000	 continue

	 fnumang = float(numang)
	 do 2010 yindex = backmaskymin, backmaskymax
	    do 2015 xyindex = backmaskxymin(yindex),
     +	       backmaskxymax(yindex)
	       if (xyoversurvprob(xyindex) .ne. 0.0)
     +		  then
		     temp = fnumang / xyoversurvprob(xyindex)
		     xyoversurvprob(xyindex) = temp
		  else
		     xyoversurvprob(xyindex) = 0.0
	       end if
 2015	    continue
 2010	 continue

c    Read the accidentals data and pad it.  Scale it by the number of
c    angles so that it has the same magnitude as the iterate.

	 call fseek_(luaccid, nbhead)
	 call fread_(luaccid, dasmall, nbdaarrayorig, eof, rcnt)

	 do 3000 daindex = daindexminorig, daindexmaxorig
	    temp = dasmall(daindex) * fnumang
	    dasmall(daindex) = temp
 3000	 continue

	 call paddata(dasmall, daaccid)

c    Perform the iterations.

	 do 4000 iter = 1, numiter

	    do 4005 yindex = backmaskymin, backmaskymax
	       do 4010 xyindex = backmaskxymin(yindex),
     +		  backmaskxymax(yindex)
		  xyaccum(xyindex) = 0.0
 4010	       continue
 4005	    continue

	    do 4015 angle = angindexmin, angindexmax

	       call forwproj(xyiterate, angle, forwmaskdmin,
     +	          forwmaskdmax, forwmasknumt, forwmaskxstart,
     +	          forwmaskystart, xinc, yinc, darray1)
	       call conv1d(darray1, kernd, maxsampkernd, datamaskdmin,
     +	          datamaskdmax, darray2)
	       call multdarr(darray2, dasurvprob, angle, forwmaskdmin,
     +	          forwmaskdmax, darray3)
	       call adddarr(darray3, daaccid, angle, datamaskdmin,
     +	          datamaskdmax, darray4)
	       call divdarr(daemiss, darray4, angle, datamaskdmin,
     +	          datamaskdmax, darray5)
	       call multdarr(darray5, dasurvprob, angle, forwmaskdmin,
     +	          forwmaskdmax, darray6)
	       call conv1d(darray6, kernd, maxsampkernd, forwmaskdmin,
     +	          forwmaskdmax, darray7)
	       call backproj(darray7, angle, backmaskymin, backmaskymax,
     +	          backmaskxymin, backmaskxymax, backmaskdstart, dinc,
     +	          xyaccum)

 4015	    continue

	    call mult3img(xyiterate, xyoversurvprob, xyaccum,
     +	       backmaskymin, backmaskymax, backmaskxymin, backmaskxymax,
     +	       xyiterate)

	    if (verbose2 .or. (verbose1 .and. (iter .eq. numiter)))
     +	       then
		  sum = 0.0
		  do 4020 yindex = backmaskymin, backmaskymax
		     do 4025 xyindex = backmaskxymin(yindex),
     +			backmaskxymax(yindex)
			sum = sum + xyiterate(xyindex)
 4025		     continue
 4020		  continue
		  adjsum = sum * sampscale
		  write(6,250) iter, adjsum
c		  call flush(6)
	    end if

 4000	 continue

c    Scale and unpad the image.

	 do 5000 yindex = backmaskymin, backmaskymax
	    do 5005 xyindex = backmaskxymin(yindex),
     +	       backmaskxymax(yindex)
	       temp = xyiterate(xyindex) * sampscale
	       xyiterate(xyindex) = temp
 5005	    continue
 5000	 continue

	 call unpadimgorig(xyiterate, xysmall)

c    Write the output iterate.

	 call fwrite_(luoutputiter, header, nbhead)
	 call fwrite_(luoutputiter, xysmall, nbxyimageorig)

	 if (verbose1)
     +	    then
	       write(6,255) progname
	 end if


  100 format(/,
     +	 ' ERROR in ', a, '.',/,
     +	 '    Usage:  ', a, ' \\',/,
     +	 '                            iter_init(in)  iter_final(out)  ',
     +	 'emiss_uncor(in) \\',/,
     +	 '                            surv_prob(in)  accid_data(in)  ',
     +	 'number_iter(in) \\',/,
     +	 '                            [v v1 v2]')
  105 format(/,
     +	 ' ERROR in ', a, '.',/,
     +	 '    The header of the emission data contains an error in the',
     +	 ' field that',/,
     +	 '    specifies whether the scan was collected in the low- or ',
     +	 'high-resolution',/,
     +	 '    mode.')
  200 format(/,
     +	 ' Beginning ', a)
  205 format(/,
     +	 ' Name of initial iterate file:  ', a)
  210 format(/,
     +	 ' Name of final iterate file:  ', a)
  215 format(/,
     +	 ' Name of emission data file:  ', a)
  220 format(/,
     +	 ' Name of survival probability data file:  ', a)
  225 format(/,
     +	 ' Name of accidental coincidences data file:  ', a)
  230 format(/,
     +	 ' Number of new iterations to calculate: ', i4)
  235 format(/,
     +	 ' Low-resolution emission scan.')
  240 format(/,
     +	 ' High-resolution emission scan.')
  245 format(' ')
  250 format(
     +	 ' New iteration number', i5, '.  Sum = ', g15.9)
  255 format(/,
     +	 ' Ending ', a,/)


      end
