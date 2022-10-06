c***********************************************************************
c
c    SUBROUTINE TITLE		pt6conv1d.f
c
c    WRITTEN BY			David G. Politte
c    DATE WRITTEN		October 2, 1989
c    WRITTEN FOR		PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	     This subroutine performs a "take" one-dimensional
c	convolution.  A "take" convolution is one in which a pixel
c	"sucks" weighted values from the pixels around it.
c
c
c    SUBROUTINE USAGE
c	This subroutine is called with the following syntax:
c
c	call conv1d(darrayin, kernd, maxsampkernd, dindex1, dindex2,
c    +	   darrayout)
c
c	where
c
c	darrayin	is a real array dimensioned from dindexmin to
c			dindexmax that is the input array to be
c			convolved.  (input)
c
c	kernd		is a real array dimensioned from 0 to (numd - 1)
c			that is the kernel in the (d) direction that
c			will be convolved with darrayin.  (input)
c
c	maxsampkernd	is an integer variable that is the maximum
c			index beyond which kernd is zero.  (input)
c
c	dindex1		is an integer variable that is the minumum (d)
c			index of the region over which the convolution
c			is computed.  (input)
c
c	dindex2		is an integer variable that is the maximum (d)
c			index of the region over which the convolution
c			is computed.  (input)
c
c	darrayout	is a real array dimensioned from dindexmin to
c			dindexmax that is darrayin convolved with kernd.
c			(output)
c
c***********************************************************************


      subroutine conv1d(darrayin, kernd, maxsampkernd, dindex1, dindex2,
     +	 darrayout)


      include 'pt6include.f'
      integer dindex, dindex1, dindex2, kernindex, maxsampkernd
      real darrayin(dindexmin:dindexmax),
     +	 darrayout(dindexmin:dindexmax), kernd(0:(numd - 1))


c    Convolve in the (d) direction and put the result in darrayout.

	 do 1000 dindex = dindex1, dindex2

	    darrayout(dindex) = darrayin(dindex) * kernd(0)

	    do 1005 kernindex = 1, maxsampkernd

	       darrayout(dindex) = darrayout(dindex) +
     +		 ((darrayin(dindex - kernindex) +
     +		   darrayin(dindex + kernindex)) * kernd(kernindex))

 1005	    continue

 1000	 continue


      return
      end
