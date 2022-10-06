c***********************************************************************
c
c    SUBROUTINE TITLE		pt6genkern.f
c
c    WRITTEN BY			David G. Politte
c    DATE WRITTEN		October 2, 1989
c    WRITTEN FOR		PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	     This subroutine calculates the center value and one-half of
c	an evenly symmetric Gaussian kernel.  Each array element
c	returned by this subroutine is proportional to the integral of
c	the Gaussian function between the boundaries of the pixel
c	centered on the corresponding sample.  The kernel is rescaled so
c	that the total integral is equal to unity.  The first sample is
c	centered at x = 0.
c
c
c    SUBROUTINE USAGE
c	This subroutine is called with the following syntax:
c
c	call genkern(fwhmb, delta, maxsampkernd, kernd)
c
c	where
c
c	fwhmb		is a real variable that is the full width at
c			half maximum of the measurement error in the
c			distance direction.  (input)
c
c	delta		is a real variable that is the sampling
c			interval.  (input)
c
c	maxsampkernd	is an integer variable that is the index of the
c			highest numbered nonzero value in the Gaussian
c			kernel.  Indexing begins at 0.  (output)
c
c	kernd		is a real array dimensioned from 0 to (numd - 1)
c			that contains the Gaussian kernel.  (output)
c
c***********************************************************************


      subroutine genkern(fwhmb, delta, maxsampkernd, kernd)


      include 'pt6include.f'
      integer i, maxsampkernd
      real confac, delta, fwhmb, kernd(0:(numd - 1)), lowlim, mean,
     +	 normarea, scale, sigmab, sum, temp, uplim


	 data mean /0.0/

c    Convert the full width at half maximum to a standard deviation.

	 confac = 2.0 * sqrt(2.0 * alog(2.0))
	 sigmab = fwhmb / confac

c    Clear the Gaussian kernel.

	 do 1000 i = 0, (numd - 1)
	    kernd(i) = 0.0
 1000	 continue

c    Generate the Gaussian kernel.

	 if (sigmab .lt. (0.001 * delta))
     +	    then

	       maxsampkernd = 0
	       kernd(0) = 1.0

	    else

	       maxsampkernd = int((numstdev * sigmab) / delta) + 1
	       do 2000 i = 0, maxsampkernd
		  lowlim = (float(i) - 0.5) * delta
		  uplim = (float(i) + 0.5) * delta
		  kernd(i) = normarea(mean, sigmab, lowlim, uplim)
 2000	       continue

	 end if

c    Rescale the kernel so that the integral is unity.

	 sum = kernd(0)
	 do 3000 i = 1, maxsampkernd
	    sum = sum + (2.0 * kernd(i))
 3000	 continue

	 scale = 1.0 / sum
	 do 3005 i = 0, maxsampkernd
	    temp = kernd(i) * scale
	    kernd(i) = temp
 3005	 continue


      return
      end
