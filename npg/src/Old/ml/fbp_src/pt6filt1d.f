c***********************************************************************
c
c    SUBROUTINE TITLE		pt6filt1d.f
c
c    WRITTEN BY			David G. Politte
c    DATE WRITTEN		October 5, 1989
c    WRITTEN FOR		PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	This subroutine performs a one-dimensional convolution.
c
c
c    SUBROUTINE USAGE
c	This subroutine is called with the following syntax:
c
c	call filt1d(darrayin, filter, dindex1, dindex2, darrayout)
c
c	where
c
c	darrayin	is a real array dimensioned from dindexmin to
c			dindexmax that is the input array to be
c			convolved.  (input)
c
c	filter		is a real array dimensioned from 0 to (numd - 1)
c			that is the filter in the (d) direction that
c			will be convolved with darrayin.  (input)
c
c	dindex1		is an integer variable that is the minimum (d)
c			index of the region over which the convolution
c			is computed.  (input)
c
c	dindex2		is an integer variable that is the maximum (d)
c			index of the region over which the convolution
c			is computed.  (input)
c
c	darrayout	is a real array dimensioned from dindexmin to
c			dindexmax that is darrayin convolved with
c			filter.  (output)
c
c***********************************************************************


      subroutine filt1d(darrayin, filter, dindex1, dindex2, darrayout)


      include 'pt6include.f'
      integer dindex1, dindex2, dindexfilt, dindexin, dindexout
      real darrayin(dindexmin:dindexmax),
     +	 darrayout(dindexmin:dindexmax), filter(0:(numd - 1))


c    Clear the output array.

	 do 1000 dindexout = dindexmin, dindexmax
	    darrayout(dindexout) = 0.0
 1000	 continue

c    Convolve in the (d) direction and put the result in darrayout.

	 do 2000 dindexout = dindex1, dindex2

	    do 2005 dindexin = dindex1, dindex2

	       dindexfilt = iabs(dindexout - dindexin)
	       darrayout(dindexout) = darrayout(dindexout)
     +		  + (darrayin(dindexin) * filter(dindexfilt))

 2005	    continue

 2000	 continue


      return
      end
