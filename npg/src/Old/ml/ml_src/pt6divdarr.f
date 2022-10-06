c***********************************************************************
c
c    SUBROUTINE TITLE		pt6divdarr.f
c
c    WRITTEN BY			David G. Politte
c    DATE WRITTEN		October 2, 1989
c    WRITTEN FOR		PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	     This subroutine divides a distance-angle (d,a) array by a
c	distance (d) array.  The result is stored in a (d) array.
c
c
c    SUBROUTINE USAGE
c	This subroutine is called with the following syntax:
c
c	call divdarr(daarrayin, darrayin, angle, dindex1, dindex2,
c    +	   darrayout)
c
c	where
c
c	daarrayin	is a real array dimensioned from daindexmin to
c			daindexmax that is the numerator.  (input)
c
c	darrayin	is a real array dimensioned from dindexmin to
c			dindexmax that is the denominator.  (input)
c
c	angle		is an integer variable that is the current angle
c			index.  (input)
c
c	dindex1		is an integer variable that is the minimum (d)
c			index of the region over which the quotient is
c			computed.  (input)
c
c	dindex2		is an integer variable that is the maximum (d)
c			index of the region over which the quotient is
c			computed.  (input)
c
c	darrayout	is a real array dimensioned from dindexmin to
c			dindexmax that is the quotient.  (output)
c
c***********************************************************************


      subroutine divdarr(daarrayin, darrayin, angle, dindex1, dindex2,
     +	 darrayout)


      include 'pt6include.f'
      integer angle, baseindex, daindex, dindex, dindex1, dindex2
      real daarrayin(daindexmin:daindexmax),
     +	 darrayin(dindexmin:dindexmax), darrayout(dindexmin:dindexmax),
     +	 temp


c    Divide the two input arrays.

	 baseindex = angle * numd

	 do 1000 dindex = dindex1, dindex2

	    daindex = baseindex + dindex
	    if (darrayin(dindex) .le. 0.0)
     +	       then
		  darrayout(dindex) = 0.0
	       else
		  temp = daarrayin(daindex) / darrayin(dindex)
		  darrayout(dindex) = temp
	    end if

 1000	 continue


      return
      end
