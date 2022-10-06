c***********************************************************************
c
c    SUBROUTINE TITLE		pt6circzero.f
c
c    WRITTEN BY			David G. Politte
c    DATE WRITTEN		October 2, 1989
c    WRITTEN FOR		PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	     This subroutine sets all pixels of an image array that are
c	outside of the back-projection mask to zero.  Pixels that are
c	inside of the back-projection mask are copied to the output
c	unaltered.
c
c
c    SUBROUTINE USAGE
c	This subroutine is called with the following syntax:
c
c	call circzero(xyarrayin, backmaskymin, backmaskymax,
c    +	   backmaskxymin, backmaskxymax, xyarrayout)
c
c	where
c
c	xyarrayin	is a real array dimensioned from xyindexmin to
c			xyindexmax that is the input image.  (input)
c
c	backmaskymin	is an integer variable that is the minimum (y)
c			index of the circular reconstruction region of
c			the back-projection mask.  (input)
c
c	backmaskymax	is an integer variable that is the maximum (y)
c			index of the circular reconstruction region of
c			the back-projection mask.  (input)
c
c	backmaskxymin	is an integer array dimensioned from yindexmin
c			to yindexmax.  backmaskxymin(yindex) is the
c			minimum linearized (x,y) index of the region
c			inside the back-projection mask at row "yindex."
c			(input)
c
c	backmaskxymax	is an integer array dimensioned from yindexmin
c			to yindexmax.  backmaskxymax(yindex) is the
c			maximum linearized (x,y) index of the region
c			inside the back-projection mask at row "yindex."
c			(input)
c
c	xyarrayout	is a real array dimensioned from xyindexmin to
c			xyindexmax that is the output image.  (output)
c
c***********************************************************************


      subroutine circzero(xyarrayin, backmaskymin, backmaskymax,
     +	 backmaskxymin, backmaskxymax, xyarrayout)


      include 'pt6include.f'
      integer backmaskxymax(yindexmin:yindexmax),
     +	 backmaskxymin(yindexmin:yindexmax), backmaskymax, backmaskymin,
     +	 baseindex, xindex, xyindex, xyindex1, xyindex2, yindex
      real temp, xyarrayin(xyindexmin:xyindexmax),
     +	 xyarrayout(xyindexmin:xyindexmax)


c    Set all pixels above the back-projection mask to zero.

	 do 1000 yindex = yindexmin, (backmaskymin - 1)
	    baseindex = yindex * numx
	    do 1005 xindex = xindexmin, xindexmax
	       xyindex = baseindex + xindex
	       xyarrayout(xyindex) = 0.0
 1005	    continue
 1000	 continue

c    Set all pixels to the left of the back-projection mask to zero.
c    Set all pixels inside the back-projection mask to the input array
c    value.  Set all pixels to the right of the back-projection mask to
c    zero.

	 do 2000 yindex = backmaskymin, backmaskymax

	    baseindex = yindex * numx

	    xyindex1 = baseindex + xindexmin
	    xyindex2 = backmaskxymin(yindex) - 1
	    do 2005 xyindex = xyindex1, xyindex2
	       xyarrayout(xyindex) = 0.0
 2005	    continue

	    do 2010 xyindex = backmaskxymin(yindex),
     +	       backmaskxymax(yindex)
	       temp = xyarrayin(xyindex)
	       xyarrayout(xyindex) = temp
 2010	    continue

	    xyindex1 = backmaskxymax(yindex) + 1
	    xyindex2 = baseindex + xindexmax
	    do 2015 xyindex = xyindex1, xyindex2
	       xyarrayout(xyindex) = 0.0
 2015	    continue

 2000	 continue

c    Set all pixels below the back-projection mask to zero.

	 do 3000 yindex = (backmaskymax + 1), yindexmax
	    baseindex = yindex * numx
	    do 3005 xindex = xindexmin, xindexmax
	       xyindex = baseindex + xindex
	       xyarrayout(xyindex) = 0.0
 3005	    continue
 3000	 continue


      return
      end
