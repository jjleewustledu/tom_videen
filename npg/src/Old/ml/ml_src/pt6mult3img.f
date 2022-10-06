c***********************************************************************
c
c    SUBROUTINE TITLE		pt6mult3img.f
c
c    WRITTEN BY			David G. Politte
c    DATE WRITTEN		October 2, 1989
c    WRITTEN FOR		PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	     This subroutine multiplies three images together inside the
c	back-projection mask region.
c
c
c    SUBROUTINE USAGE
c	This subroutine is called with the following syntax:
c
c	call mult3img(xyarrayin1, xyarrayin2, xyarrayin3, backmaskymin,
c    +	   backmaskymax, backmaskxymin, backmaskxymax, xyarrayout)
c
c	where
c
c	xyarrayin1	is a real array dimensioned from xyindexmin to
c			xyindexmax that is the first array to be
c			multiplied together.  (input)
c
c	xyarrayin2	is a real array dimensioned from xyindexmin to
c			xyindexmax that is the second array to be
c			multiplied together.  (input)
c
c	xyarrayin3	is a real array dimensioned from xyindexmin to
c			to xyindexmax that is the third array to be
c			multiplied together.  (input)
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
c		        minimum linearized (x,y) index of the region
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
c			xyindexmax that is xyarrayin1 multiplied by
c			xyarrayin2 and xyarrayin3 inside the circular
c			back-projection mask.  (output)
c
c***********************************************************************


      subroutine mult3img(xyarrayin1, xyarrayin2, xyarrayin3,
     +	 backmaskymin, backmaskymax, backmaskxymin, backmaskxymax,
     +	 xyarrayout)


      include 'pt6include.f'
      integer backmaskxymax(yindexmin:yindexmax),
     +	 backmaskxymin(yindexmin:yindexmax), backmaskymax, backmaskymin,
     +	 xyindex, yindex
      real temp, xyarrayin1(xyindexmin:xyindexmax),
     +	 xyarrayin2(xyindexmin:xyindexmax),
     +	 xyarrayin3(xyindexmin:xyindexmax),
     +	 xyarrayout(xyindexmin:xyindexmax)


c    Multiply the three input arrays inside the back-projection mask.

	 do 1000 yindex = backmaskymin, backmaskymax

	    do 1005 xyindex = backmaskxymin(yindex),
     +	       backmaskxymax(yindex)

	       temp = xyarrayin1(xyindex) * xyarrayin2(xyindex)
     +		  * xyarrayin3(xyindex)
	       xyarrayout(xyindex) = temp

 1005	    continue

 1000	 continue


      return
      end
