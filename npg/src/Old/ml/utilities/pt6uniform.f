c***********************************************************************
c
c    PROGRAM TITLE		pt6uniform.f
c
c    WRITTEN BY			David G. Politte
c    DATE WRITTEN		July 22, 1991
c    WRITTEN FOR		PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	     This program creates the uniform initial estimate for the
c	maximum-likelihood algorithm.  The size of the file is chosen
c	for the worst case - the 2 mm pixels.
c
c
c    MAIN PROGRAM USAGE
c	This program is invoked with the following syntax:
c
c	pt6uniform  image(out)  value(in)  [v v1 v2]
c
c	where
c
c	image(out)	is the file that contains an image with the same
c			floating-poing number in every pixel.  (output)
c
c	value(in)	is the floating-point value placed in each
c			pixel.  (input)
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


      program pt6uniform


$alias fcreat_ = 'fcreat_' (%ref, %ref)
$alias fwrite_ = 'fwrite_' (%ref, %ref, %ref)
      include 'pt6include.f'
      character*80 progname, string
      integer*2 header(128)
      integer i, iargc, luoutput, nbxyimageorig, numarg, xyindexorig
      logical verbose1
      real xyarray(xyindexminorig:xyindexmaxorig), value


c    Get the name of the program.

	 call igetarg(0, progname, 80)
	 call stripspace(progname)

c    Figure out if the program was invoked properly.

	 numarg = iargc()
	 if ((numarg .ne. 2) .and. (numarg .ne. 3))
     +	    then
	       write(7,100) progname, progname
	       call exit(1)
	 end if

c    Set the verbosity.

	 if (numarg .eq. 3)
     +	    then
	       call igetarg(3, string, 80)
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

c    Get names of files, strip trailing blanks, and create output.
c    Also, read the floating-point value to be placed in each pixel.

	 call igetarg(1, string, 80)
	 call stripspace(string)
	 call fcreat_(string, luoutput)
	 if (verbose1)
     +	    then
	       write(6,205) string
	 end if

	 call igetarg(2, string, 80)
	 read(string, *) value
	 if (verbose1)
     +	    then
	       write(6,210) value
	 end if

c    Initialize the header.

	 do 1000 i = 1, 128
	    header(i) = 0
 1000	 continue

c    Create the uniform image.

	 do 2000 xyindexorig = xyindexminorig, xyindexmaxorig
	    xyarray(xyindexorig) = value
 2000	 continue

c    Write the output file.

	 nbxyimageorig = numxyorig * realsize
	 call fwrite_(luoutput, header, nbhead)
	 call fwrite_(luoutput, xyarray, nbxyimageorig)

	 if (verbose1)
     +	    then
	       write(6,215) progname
	 end if


  100 format(/,
     +	 ' ERROR in ', a, '.',/,
     +	 '    Usage:  ', a, ' \\',/,
     +	 '                            image(out)  value(in)  [v v1 v2]')
  200 format(/,
     +	 ' Beginning ', a)
  205 format(/,
     +	 ' Name of output image file:  ', a)
  210 format(/,
     +	 ' All pixels set to ', g13.7)
  215 format(/,
     +	 ' Ending ', a,/)


      end
