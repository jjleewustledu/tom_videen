c*******************************************************************
c
c    SUBROUTINE TITLE	    pt6multdarr.f
c
c    WRITTEN BY		    David G. Politte
c    DATE WRITTEN	    October 2, 1989
c    WRITTEN FOR	    PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	       This subroutine multiplies a distance (d) array and
c	  a distance-angle (d,a) array.  The result is stored in a
c	  (d) array.
c
c
c    SUBROUTINE USAGE
c	  This subroutine is called with the following syntax:
c
c	  call multdarr(darrayin, daarrayin, angle, dindex1,
c    +	     dindex2, darrayout)
c
c	  where
c
c	  darrayin  is a real array dimensioned from dindexmin to
c		    dindexmax that is the first array to be
c		    multiplied together.  (input)
c
c	  daarrayin is a real array dimensioned from daindexmin to
c		    daindexmax that is the second array to be
c		    multiplied together.  (input)
c
c	  angle	    is an integer variable that is the current
c		    angle index.  (input)
c
c	  dindex1   is an integer variable that is the minimum (d)
c		    index of the region over which the two arrays
c		    are multiplied.  (input)
c
c	  dindex2   is an integer variable that is the maximum (d)
c		    index of the region over which the two arrays
c		    are multiplied.  (input)
c
c	  darrayout is a real array dimensioned from dindexmin to
c		    dindexmax that is darrayin multiplied by
c		    daarrayin.  (output)
c
c*******************************************************************


      subroutine multdarr(darrayin, daarrayin, angle, dindex1,
     +	 dindex2, darrayout)


      include 'pt6include.f'
      integer angle, baseindex, daindex, dindex, dindex1, dindex2
      real daarrayin(daindexmin:daindexmax),
     +	 darrayin(dindexmin:dindexmax),
     +	 darrayout(dindexmin:dindexmax), temp


c    Multiply the two input arrays.

	 baseindex = angle * numd

	 do 1000 dindex = dindex1, dindex2

	    daindex = baseindex + dindex
	    temp = darrayin(dindex) * daarrayin(daindex)
	    darrayout(dindex) = temp

 1000	 continue


      return
      end
