c***********************************************************************
c
c    PROGRAM TITLE		pt6sievecirc.f
c
c    WRITTEN BY			David G. Politte
c    DATE WRITTEN		May 3, 1990
c    WRITTEN FOR		PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	     pt6sievecirc (this program) and pt6ml are the executables
c	for two-dimensional image reconstruction of data from PETT VI.
c	Together these programs compute maximum-likelihood estimates of
c	the desired image.  pt6ml performs the iterative expectation-
c	maximization algorithm.  The result is the input for
c	pt6sievecirc, which applies a linear filter called a sieve to
c	produce an estimate of the desired image.  The sieve is assumed
c	to be Gaussian and separable.  This program performs the two-
c	dimensional convolution as two cascaded one-dimensional
c	convolutions.  The convolution is performed over a circular
c	region with a diameter one pixel smaller than the back-
c	projection region.
c
c
c    MAIN PROGRAM USAGE
c	This program is invoked with the following syntax:
c
c	pt6sievecirc_2.87mm  image_in(in)  image_out(out)  reso_mm(in) \
c			[v v1 v2]
c
c	or
c
c	pt6sievecirc_2.00mm  image_in(in)  image_out(out)  reso_mm(in) \
c			[v v1 v2]
c
c	where
c
c	image_in(in)	is the file that contains the result of the
c			iterative portion of the algorithm, computed by
c			pt6ml.  (input)
c
c	image_out(out)	is the file that contains the input image
c			convolved with the sieve kernel.  (output)
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
c	  standard input - not used
c
c         standard output - used, see [v v1 v2] option
c
c	  standard error - used
c
c***********************************************************************


      program pt6sievecirc


$alias fcreat_ = 'fcreat_' (%ref, %ref)
$alias fopenr_ = 'fopenr_' (%ref, %ref)
$alias fread_  = 'fread_'  (%ref, %ref, %ref, %ref, %ref)
$alias fwrite_ = 'fwrite_' (%ref, %ref, %ref)
      include 'pt6include.f'
      character*12 resotext
      character*80 progname, string
      character*256 headera
      integer*2 headeri2(128)
      integer baseindex, eof, iargc, kernindex, luinput, luoutput,
     +	 maxsampkerns, mmindexstart, mmindexstop, nbxyimagefin,
     +	 nbxyimageorig, numarg, offset(0:(numx - 1)), rcnt, stringindex,
     +	 xindex, xindexa, xindexb, xindexmaxxconv, xindexminxconv,
     +	 xyindex, xyindexa, xyindexb,
     +	 xyindexmaxxconv(yindexmin:yindexmax),
     +	 xyindexmaxyconv(yindexmin:yindexmax),
     +	 xyindexminxconv(yindexmin:yindexmax),
     +	 xyindexminyconv(yindexmin:yindexmax), yindex, yindexmaxxconv,
     +	 yindexmaxyconv, yindexminxconv, yindexminyconv
      logical verbose1
      real fwhmb, fwhmr, fwhms, rad, radsq, sieve(0:(numd - 1)), xa, xb,
     +	 xyarray1(xyindexmin:xyindexmax),
     +	 xyarray2(xyindexmin:xyindexmax),
     +	 xysmallfin(xyindexminfin:xyindexmaxfin),
     +	 xysmallorig(xyindexminorig:xyindexmaxorig), y
      equivalence (headeri2, headera)


c    Get the name of the program.

	 call igetarg(0, progname, 80)
	 call stripspace(progname)

c    Figure out if the program was invoked properly.

	 numarg = iargc()
	 if ((numarg .ne. 3) .and. (numarg .ne. 4))
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
	 call fopenr_(string, luinput)
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
	 fwhms = fwhmr
	 if (verbose1)
     +	    then
	      write(6,215) fwhmr
	 end if

c    Read the input image and pad it.

	 nbxyimageorig = numxyorig * realsize
	 call fread_(luinput, headeri2, nbhead, eof, rcnt)
	 call fread_(luinput, xysmallorig, nbxyimageorig, eof, rcnt)

	 call padimg(xysmallorig, xyarray1)

c    Update the header.

	 headera(81:92) = resotext
	 headeri2(38) = 2000

c    Generate the sieve kernel.

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
	 call genkern(fwhms, pix, maxsampkerns, sieve)

c    The table offset(i) calculated below is used to index pixels that
c    are in the same column but in different rows.  The entry in
c    offset(i) for index i is the difference in the linearized (x,y)
c    index that corresponds to a change of i units in the (y) index.

	 do 2000 kernindex = 0, maxsampkerns
	    offset(kernindex) = numx * kernindex
 2000	 continue

c    Generate masks that will be used in the convolution in the x
c    direction.  The mask limits the region of computation to a square
c    of size numxorig by numyorig that is centered inside of the padded
c    image.

	 xindexminxconv = nint(
     +	    xindexcent - (xindexcentorig - float(xindexminorig)))
	 xindexmaxxconv = nint(
     +	    xindexcent + (float(xindexmaxorig) - xindexcentorig))
	 yindexminxconv = nint(
     +	    yindexcent - (yindexcentorig - float(yindexminorig)))
	 yindexmaxxconv = nint(
     +	    yindexcent + (float(yindexmaxorig) - yindexcentorig))

	 do 3000 yindex = yindexminxconv, yindexmaxxconv
	    baseindex = yindex * numx
	    xyindexminxconv(yindex) = baseindex + xindexminxconv
	    xyindexmaxxconv(yindex) = baseindex + xindexmaxxconv
 3000	 continue

c    Generate masks that will be used in the convolution in the y
c    direction.  The mask limits the region of computation to a circle
c    that has a radius one pixel smaller than the back-projection
c    region.  The back-projection region is a circle inscribed in a
c    square of size numxorig by numyorig.  The code that follows is only
c    slightly different than the code that generates the back-projection
c    mask in pt6maskml.

	 yindexminyconv = yindexminxconv + 1
	 yindexmaxyconv = yindexmaxxconv - 1

	 rad = amin1((xindexcentorig - float(xindexminorig)),
     +		     (float(xindexmaxorig) - xindexcentorig),
     +		     (yindexcentorig - float(yindexminorig)),
     +		     (float(yindexmaxorig) - yindexcentorig))
     +	       - 0.5
	 radsq = rad * rad

	 do 4000 yindex = yindexminyconv, yindexmaxyconv
	    baseindex = yindex * numx
	    y = float(yindex) - yindexcent
	    xb = sqrt(radsq - (y * y))
	    xa = -xb
	    xindexb = int(xb + xindexcent)
	    xindexa = int(xa + xindexcent + 1.0)
	    xyindexminyconv(yindex) = baseindex + xindexa
	    xyindexmaxyconv(yindex) = baseindex + xindexb
 4000	 continue

c    Set pixels outside of the region to zero.

	 do 5000 yindex = yindexmin, (yindexminyconv - 1)
	    baseindex = yindex * numx
	    do 5005 xindex = xindexmin, xindexmax
	       xyindex = baseindex + xindex
	       xyarray1(xyindex) = 0.0
 5005	    continue
 5000	 continue

	 do 5010 yindex = yindexminyconv, yindexmaxyconv
	    baseindex = yindex * numx
	    xyindexa = baseindex + xindexmin
	    xyindexb = xyindexminyconv(yindex) - 1
	    do 5015 xyindex = xyindexa, xyindexb
	       xyarray1(xyindex) = 0.0
 5015	    continue
	    xyindexa = xyindexmaxyconv(yindex) + 1
	    xyindexb = baseindex + xindexmax
	    do 5020 xyindex = xyindexa, xyindexb
	       xyarray1(xyindex) = 0.0
 5020	    continue
 5010	 continue

	 do 5025 yindex = (yindexmaxyconv + 1), yindexmax
	    baseindex = yindex * numx
	    do 5030 xindex = xindexmin, xindexmax
	       xyindex = baseindex + xindex
	       xyarray1(xyindex) = 0.0
 5030	    continue
 5025	 continue

c    Convolve in the (x) direction and put the result in xyarray2.  The
c    result is computed for all pixels inside a square of size numxorig
c    by numyorig that is centered inside of the padded image.

	 do 6000 xyindex = xyindexmin, xyindexmax
	    xyarray2(xyindex) = 0.0
 6000	 continue

	 do 6005 yindex = yindexminxconv, yindexmaxxconv

	    do 6010 xyindex = xyindexminxconv(yindex),
     +	       xyindexmaxxconv(yindex)

	       xyarray2(xyindex) = xyarray1(xyindex) * sieve(0)

	       do 6015 kernindex = 1, maxsampkerns

		  xyarray2(xyindex) = xyarray2(xyindex) +
     +		     ((xyarray1(xyindex - kernindex)
     +		     + xyarray1(xyindex + kernindex))
     +		     * sieve(kernindex))

 6015	       continue

 6010	    continue

 6005	 continue

c    Convolve in the (y) direction and put the result in xyarray1.  The
c    result is computed for all pixels inside of a circle with a radius
c    that is one pixel smaller than the back-projection region.

	 do 7000 xyindex = xyindexmin, xyindexmax
	    xyarray1(xyindex) = 0.0
 7000	 continue

	 do 7005 yindex = yindexminyconv, yindexmaxyconv

	    do 7010 xyindex = xyindexminyconv(yindex),
     +	       xyindexmaxyconv(yindex)

	       xyarray1(xyindex) = xyarray2(xyindex) * sieve(0)

	       do 7015 kernindex = 1, maxsampkerns

		  xyarray1(xyindex) = xyarray1(xyindex) +
     +		     ((xyarray2(xyindex - offset(kernindex))
     +		     + xyarray2(xyindex + offset(kernindex)))
     +		     * sieve(kernindex))

 7015	       continue

 7010	    continue

 7005	 continue

c    Unpad the image.

	 call unpadimgfin(xyarray1, xysmallfin)

c    Write outut image.

	 nbxyimagefin = numxyfin * realsize
	 call fwrite_(luoutput, headeri2, nbhead)
	 call fwrite_(luoutput, xysmallfin, nbxyimagefin)

	 if (verbose1)
     +	    then
	       write(6,230) progname
	 end if


  100 format(/,
     +	 ' ERROR in ', a, '.',/,
     +	 '    Usage:  ', a, ' \\',/,
     +	 '                            image_in(in)  image_out(out)  ',
     +	 'reso_mm(in)  \\',/,
     +	 '                            [v v1 v2]')
  105 format(/,
     +	 ' ERROR in ', a, '.',/,
     +	 '    Resolution field in header is incorrect.')
  110 format(/,
     +	 ' ERROR in ', a, '.',/,
     +	 '    Reconstructed resolution is better than intrinsic.')
  200 format(/,
     +	 ' Beginning ', a)
  205 format(/,
     +	 ' Name of input image file:  ', a)
  210 format(/,
     +	 ' Name of output image file:  ', a)
  215 format(/,
     +	 ' FWHM reconstructed resolution in mm: ', g9.3)
  220 format(/,
     +	 ' Low-resolution emission scan.')
  225 format(/,
     +	 ' High-resolution emission scan.')
  230 format(/,
     +	 ' Ending ', a,/)


      end
