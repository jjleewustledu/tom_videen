c***********************************************************************
c
c    PROGRAM TITLE		pt6fbp.f
c
c    WRITTEN BY			David G. Politte
c    DATE WRITTEN		October 5, 1989
c    WRITTEN FOR		PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	     This program is the executable for two-dimensional image
c	reconstruction of data from PETT VI.  It uses the filtered back-
c	projection algorithm to compute an estimate of the desired
c	image.  This program has the following features:
c
c	a) The computations are done in a coordinate system that is
c	   rotated relative to the image.  This speeds the computations
c	   greatly.
c
c	b) The data are assumed to be precorrected for accidental
c	   coincidences and physical attenuation of photons.
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
c	This program is invoked with the following syntax:
c
c	pt6fbp_2.87mm  emiss_corr(in)  image(out)  reso_mm(in) \
c			[v v1 v2]
c
c		or
c
c	pt6fbp_2.00mm  emiss_corr(in)  image(out)  reso_mm(in) \
c			[v v1 v2]
c
c	where
c
c	emiss_corr(in)	is the file that contains the precorrected
c			emission data at each distance (d) and angle
c			(a).  (input)
c
c	image(out)	is the file that contains an estimate of the
c			desired image as computed by filtered back-
c			projection.  (output)
c
c	reso_mm(in)	is a real parameter that is the full width at
c			half maximum reconstructed resolution expressed
c			in mm.  (input)
c
c	[v v1 v2]	is an optional argument that determines the
c			verbosity of the output.  Informational messages
c			are sent to the standard output if "v", "v1", or
c			"v2" is specified.  "v", "v1", and "v2" have
c			identical effects.  If this argument is omitted
c			or if something else is specified then no
c			messages are sent to the standard output.
c			(input)
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


      program pt6fbp


$alias fcreat_ = 'fcreat_' (%ref, %ref)
$alias fopenr_ = 'fopenr_' (%ref, %ref)
$alias fread_  = 'fread_'  (%ref, %ref, %ref, %ref, %ref)
$alias fwrite_ = 'fwrite_' (%ref, %ref, %ref)
      include 'pt6include.f'
      character*12 resotext
      character*80 progname, string
      character*256 headera
      integer*2 headeri2(128)
      integer angle, backmaskxymax(yindexmin:yindexmax),
     +	 backmaskxymin(yindexmin:yindexmax), backmaskymax, backmaskymin,
     +	 baseindex, daindex, datamaskdmax, datamaskdmin, dindex, eof,
     +	 forwmaskdmax, forwmaskdmin, iargc, luemiss, luoutput,
     +	 mmindexstart, mmindexstop, nbdaarrayorig, nbxyimagefin, numarg,
     +	 rcnt, stringindex, xyindex, yindex
      logical verbose1
      real backmaskdstart(maskbackindexmin:maskbackindexmax),
     +	 daemiss(daindexmin:daindexmax), darray1(dindexmin:dindexmax),
     +	 darray2(dindexmin:dindexmax),
     +	 dasmall(daindexminorig:daindexmaxorig),
     +	 dinc(angindexmin:angindexmax), filter(0:(numd - 1)), fwhmb,
     +	 fwhmr, sum, xyarray(xyindexmin:xyindexmax),
     +	 xysmall(xyindexminfin:xyindexmaxfin)
      equivalence (headeri2, headera)


c    Get the name of the program.

	 call igetarg(0, progname, 80)
	 call stripspace(progname)

c    Figure out it the program was invoked properly.

	 numarg = iargc()
	 if ((numarg .ne. 3) .and .(numarg .ne. 4))
     +	    then
	       write(7,100) progname, progname
	       call exit(1)
	 end if

c    Set the verbosity.

	 if (numarg .eq. 4)
     +	    then
	       call igetarg(4, string, 80)
	       if ((string(1:2) .eq. 'v ') .or.
     +		   (string(1:2) .eq. 'v1') .or.
     +		   (string(1:2) .eq. 'v2'))
     +		  then
		     verbose1 = .true.
		  else
		     verbose1 = .false.
	       end if
	    else
	       verbose1 = .false.
	 end if

	 if (verbose1)
     +	    then
	       write(6,200) progname
	 end if

c    Get names of files, strip trailing blanks, and open or create.
c    Also, read the reconstructed resolution.

	 call igetarg(1, string, 80)
	 call stripspace(string)
	 call fopenr_(string, luemiss)
	 if (verbose1)
     +	    then
	       write(6,205) string
	 end if

	 call igetarg(2, string, 80)
	 call stripspace(string)
	 call fcreat_(string, luoutput)
	 if (verbose1)
     +	    then
	       write(6,210) string
	 end if

	 call igetarg(3, string, 80)
	 read(string, *) fwhmr
	 resotext = string(1:12)
	 do 1000 stringindex = 12, 1, -1
	    if (resotext(stringindex:stringindex) .ne. ' ')
     +	       then
		  mmindexstart = stringindex + 2
		  go to 1005
	    end if
 1000	 continue
 1005	 continue
	 mmindexstop = mmindexstart + 1
	 resotext(mmindexstart:mmindexstop) = 'mm'
	 if (verbose1)
     +	    then
	       write(6,215) fwhmr
	 end if
		  
c    Initialize arrays.

	 do 2000 dindex = dindexmin, dindexmax
	    darray1(dindex) = 0.0
	    darray2(dindex) = 0.0
 2000	 continue

	 do 2005 xyindex = xyindexmin, xyindexmax
	    xyarray(xyindex) = 0.0
 2005	 continue

c    Make the masks.

	 call maskfbp(dinc, datamaskdmin, datamaskdmax, backmaskymin,
     +	    backmaskymax, backmaskxymin, backmaskxymax, backmaskdstart,
     +	    forwmaskdmin, forwmaskdmax)

c    Read the emission data and pad it.

	 nbdaarrayorig = numdaorig * realsize
	 call fread_(luemiss, headeri2, nbhead, eof, rcnt)
	 call fread_(luemiss, dasmall, nbdaarrayorig, eof, rcnt)
	 call paddata(dasmall, daemiss)

c    Update the header.

	 headera(81:92) = resotext
	 headeri2(38) = 2010

c    Create the filter.

	 if (headeri2(16) .eq. 0) then
	    fwhmb = fwhmblow
	    if (verbose1)
     +	       then
		  write(6,220)
	    end if
	 else if (headeri2(16) .eq. 1) then
	    fwhmb = fwhmbhigh
	    if (verbose1)
     +	       then
		  write(6,225)
	    end if
	 else
	    write(7,105) progname
	    call exit(1)
	 end if
	 if (fwhmr .lt. fwhmb)
     +	    then
	       write(7,110) progname
	       call exit(1)
	 end if
	 call filgen1d(fwhmb, fwhmr, filter)

c    Reconstruct with filtered back-projection.

	 do 3000 angle = angindexmin, angindexmax

	    baseindex = angle * numd
	    do 3005 dindex = dindexmin, dindexmax
	       daindex = baseindex + dindex
	       darray1(dindex) = daemiss(daindex)
 3005	    continue
	       
	    call filt1d(darray1, filter, forwmaskdmin, forwmaskdmax,
     +	       darray2)
	    call backproj(darray2, angle, backmaskymin, backmaskymax,
     +	       backmaskxymin, backmaskxymax, backmaskdstart, dinc,
     +	       xyarray)

 3000	 continue

	 if (verbose1)
     +	    then
	       sum = 0.0
	       do 4000 yindex = backmaskymin, backmaskymax
		  do 4005 xyindex = backmaskxymin(yindex),
     +		     backmaskxymax(yindex)
		     sum = sum + xyarray(xyindex)
 4005		  continue
 4000	       continue
	 end if

c    Unpad the image.

	 call unpadimgfin(xyarray, xysmall)

c    Write output image.

	 nbxyimagefin = numxyfin * realsize
	 call fwrite_(luoutput, headeri2, nbhead)
	 call fwrite_(luoutput, xysmall, nbxyimagefin)

	 if (verbose1)
     +	    then
	       write(6,230) sum
	       write(6,235) progname
	 end if


  100 format(/,
     +	 ' ERROR in ', a, '.',/,
     +	 '    Usage:  ', a, ' \\',/,
     +	 '                            emiss_corr(in)  image(out)  ',
     +	 'reso_mm(in)  [v v1 v2]')
  105 format(/,
     +	 ' ERROR in ', a, '.',/,
     +	 '    The header of the emission data contains an error in the',
     +	 ' field that',/,
     +	 '    specifies whether the scan was collected in the low- or ',
     +	 'high-resolution',/,
     +	 '    mode.')
  110 format(/,
     +	 ' ERROR in ', a, '.',/,
     +	 '    Reconstructed resolution is better than intrinsic.')
  200 format(/,
     +	 ' Beginning ', a)
  205 format(/,
     +	 ' Name of emission data file:  ', a)
  210 format(/,
     +	 ' Name of output image file:  ', a)
  215 format(/,
     +	 ' FWHM reconstructed resolution in mm: ', g9.3)
  220 format(/,
     +	 ' Low-resolution emission scan.')
  225 format(/,
     +	 ' High-resolution emission scan.')
  230 format(/,
     +	 ' Sum of output image = ', g15.9)
  235 format(/,
     +	 ' Ending ', a,/)


      end
