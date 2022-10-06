c***********************************************************************
c
c    SUBROUTINE TITLE		pt6backproj.f
c
c    WRITTEN BY			David G. Politte
c    DATE WRITTEN		October 2, 1989
c    WRITTEN FOR		PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	     This subroutine back-projects a distance (d) array at a
c	given angle to the (x,y) system and sums the result into an
c	array.
c
c
c    SUBROUTINE USAGE
c	This subroutine is called with the following syntax:
c
c	call backproj(darray, angle, backmaskymin, backmaskymax,
c    +	   backmaskxymin, backmaskxymax, backmaskdstart, dinc, xyarray)
c
c	where
c
c	darray		is a real array dimensioned from dindexmin to
c			dindexmax that is the input array to be back-
c			projected.  (input)
c
c	angle		is an integer variable that is the current angle
c			index.  (input)
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
c	backmaskdstart	is a real array dimensioned from
c			maskbackindexmin to maskbackindexmax.
c			Explanation of backmaskdstart(index):  let
c			index = (angle * numy) + yindex, where "angle"
c			is the angle index, "numy" is the number of
c			samples in the (y) direction of the image, and
c			"yindex" is the (y) index.  Then
c			backmaskdstart(index) is the value to which the
c			(d) index should be initialized when performing
c			a back-projection at angle "angle" and (y) index
c			"yindex."  (input)
c
c	dinc		is a real array dimensioned from angindexmin to
c			angindexmax.  dinc(angle) is the increment in
c			the (d) index that corresponds to an increment
c			by unity in the (x) index at the angle "angle."
c			(input)
c
c	xyarray		is a real array dimensioned from xyindexmin to
c			xyindexmax that is the output array.  (input/
c			output)
c
c***********************************************************************


      subroutine backproj(darray, angle, backmaskymin, backmaskymax,
     +	 backmaskxymin, backmaskxymax, backmaskdstart, dinc, xyarray)


      include 'pt6include.f'
      integer angle, backmaskxymax(yindexmin:yindexmax),
     +	 backmaskxymin(yindexmin:yindexmax), backmaskymax, backmaskymin,
     +	 baseindex, dindex1, dstartindex, xyindex, yindex
      real backmaskdstart(maskbackindexmin:maskbackindexmax), d,
     +	 darray(dindexmin:dindexmax), dinc(angindexmin:angindexmax),
     +	 dincrement, dinterp1, dinterp2, xyarray(xyindexmin:xyindexmax)


c    Perform the back-projection at this angle.

	 baseindex = angle * numy
	 dincrement = dinc(angle)

	 do 1000 yindex = backmaskymin, backmaskymax

	    dstartindex = baseindex + yindex
	    d = backmaskdstart(dstartindex)

	    do 1005 xyindex = backmaskxymin(yindex),
     +	       backmaskxymax(yindex)

	       d = d + dincrement

	       dindex1 = int(d)

	       dinterp2 = d - float(dindex1)
	       dinterp1 = 1.0 - dinterp2

	       xyarray(xyindex) = xyarray(xyindex) +
     +		  (darray(dindex1) * dinterp1)
     +		  + (darray(dindex1 + 1) * dinterp2)

 1005	    continue

 1000	 continue


      return
      end
