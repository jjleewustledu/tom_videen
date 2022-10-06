c***********************************************************************
c
c    SUBROUTINE TITLE		pt6forwproj.f
c
c    WRITTEN BY			David G. Politte
c    DATE WRITTEN		October 2, 1989
c    WRITTEN FOR		PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	     This subroutine forward projects an image in the (x,y)
c	coordinate system into a distance (d) array.
c
c
c    SUBROUTINE USAGE
c	This subroutine is called with the following syntax:
c
c	call forwproj(xyarray, angle, forwmaskdmin, forwmaskdmax,
c    +	   forwmasknumt, forwmaskxstart, forwmaskystart, xinc, yinc,
c    +	   darray)
c
c	where
c
c	xyarray		is a real array dimensioned from xyindexmin to
c			xyindexmax that is the input array to be forward
c			projected.  (input)
c
c	angle		is an integer variable that is the current angle
c			index.  (input)
c
c	forwmaskdmin	is an integer variable that is the minimum (d)
c			index of the region over which the forward
c			projection is computed.  (input)
c
c	forwmaskdmax	is an integer variable that is the maximum (d)
c			index of the region over which the forward
c			projection is computed.  (input)
c
c	forwmasknumt	is an integer array dimensioned from dindexmin
c			to dindexmax.  forwmasknumt(dindex) is the
c			number of (pseudo-t) samples that contribute to
c			the forward projection at the (d) index
c			"dindex."  (input)
c
c	forwmaskxstart	is a real array dimensioned from
c			maskforwindexmin to maskforwindexmax.
c			Explanation of forwmaskxstart(index):  let
c			index = (angle * numd) + dindex, where "angle"
c			is the angle index, "numd" is the number of
c			distance (d) samples, and "dindex" is the (d)
c			index.  Then forwmaskxstart(index) is the value
c			to which the (x) index should be initialized
c			when performing a forward projection at angle
c			"angle" and (d) index "dindex."  (input)
c
c	forwmaskystart	is a real array dimensioned from
c			maskforwindexmin to maskforwindexmax.
c			Explanation of forwmaskystart(index):  let
c			index = (angle * numd) + dindex, where "angle"
c			is the angle index, "numd" is the number of
c			distance (d) samples, and "dindex" is the (d)
c			index.  Then forwmaskystart(index) is the value
c			to which the (y) index should be initialized
c			when performing a forward projection at angle
c			"angle" and (d) index "dindex."  (input)
c
c	xinc		is a real array dimensioned from angindexmin to
c			angindexmax.  xinc(angle) is the increment in
c			the (x) index that corresponds to an increment
c			by unity in the (pseudo-t) index at the angle
c			"angle."  (input)
c
c	yinc		is a real array dimensioned from angindexmin to
c			angindexmax.  yinc(angle) is the increment in
c			the (y) index that corresponds to an increment
c			by unity in the (pseudo-t) index at the angle
c			"angle."  (input)
c
c	darray		is a real array dimensioned from dindexmin to
c			dindexmax that is the output array.  (output)
c
c***********************************************************************


      subroutine forwproj(xyarray, angle, forwmaskdmin, forwmaskdmax,
     +	 forwmasknumt, forwmaskxstart, forwmaskystart, xinc, yinc,
     +	 darray)


      include 'pt6include.f'
      integer angle, baseindex, dindex, forwmaskdmax, forwmaskdmin,
     +	 forwmasknumt(dindexmin:dindexmax), pseudot, x1, xyindex1,
     +	 xyindex2, xystartindex, y1
      real darray(dindexmin:dindexmax),
     +	 forwmaskxstart(maskforwindexmin:maskforwindexmax),
     +	 forwmaskystart(maskforwindexmin:maskforwindexmax), x,
     +	 xinc(angindexmin:angindexmax), xincrement, xinterp1, xinterp2,
     +	 xyarray(xyindexmin:xyindexmax), y,
     +	 yinc(angindexmin:angindexmax), yincrement, yinterp1, yinterp2


c    Clear the output array.

	 do 1000 dindex = dindexmin, dindexmax
	    darray(dindex) = 0.0
 1000	 continue

c    Perform the forward projection at this angle.

	 baseindex = angle * numd
	 xincrement = xinc(angle)
	 yincrement = yinc(angle)

	 do 2000 dindex = forwmaskdmin, forwmaskdmax

	    xystartindex = baseindex + dindex
	    x = forwmaskxstart(xystartindex)
	    y = forwmaskystart(xystartindex)

	    do 2005 pseudot = 1, forwmasknumt(dindex)

	       x = x + xincrement
	       y = y + yincrement

	       x1 = int(x)
	       y1 = int(y)

	       xinterp2 = x - float(x1)
	       xinterp1 = 1.0 - xinterp2

	       yinterp2 = y - float(y1)
	       yinterp1 = 1.0 - yinterp2

	       xyindex1 = (y1 * numx) + x1
	       xyindex2 = xyindex1 + numx

	       darray(dindex) = darray(dindex) +
     +		  (((xyarray(xyindex1) * yinterp1)
     +		  + (xyarray(xyindex2) * yinterp2)) * xinterp1) +
     +		  (((xyarray(xyindex1 + 1) * yinterp1)
     +		  + (xyarray(xyindex2 + 1) * yinterp2)) * xinterp2)

 2005	    continue

 2000	 continue


      return
      end
