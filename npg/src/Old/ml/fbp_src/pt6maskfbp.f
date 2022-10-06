c***********************************************************************
c
c    SUBROUTINE TITLE		pt6maskfbp.f
c
c    WRITTEN BY			David G. Politte
c    DATE WRITTEN		October 6, 1989
c    WRITTEN FOR		PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	     This subroutine computes
c
c	a) the increment table,
c	b) the data mask,
c	c) the back-projection mask, and
c	d) the forward projection mask.
c
c	All of the outputs are returned by the subroutine arguments that
c	are explained below.
c
c
c    SUBROUTINE USAGE
c	This subroutine is called with the following syntax:
c
c	call maskfbp(dinc, datamaskdmin, datamaskdmax, backmaskymin,
c    +	   backmaskymax, backmaskxymin, backmaskxymax, backmaskdstart,
c    +	   forwmaskdmin, forwmaskdmax)
c
c	where
c
c	dinc		is a real array dimensioned from angindexmin to
c			angindexmax.  dinc(angle) is the increment in
c			the (d) index that corresponds to an increment
c			by unity in the (x) index at the angle "angle."
c			(output)
c
c	datamaskdmin	is an integer variable that is the minimum (d)
c			index at which the data are nonzero after the
c			data array is padded.  (output)
c
c	datamaskdmax	is an integer variable that is the maximum (d)
c			index at which the data are nonzero after the
c			data array is padded.  (output)
c
c	backmaskymin	is an integer variable that is the minimum (y)
c			index of the circular reconstruction region of
c			the back-projection mask.  (output)
c
c	backmaskymax	is an integer variable that is the maximum (y)
c			index of the circular reconstruction region of
c			the back-projection mask.  (output)
c
c	backmaskxymin	is an integer array dimensioned from yindexmin
c			to yindexmax.  backmaskxymin(yindex) is the
c			minimum linearized (x,y) index of the region
c			inside the back-projection mask at row "yindex."
c			(output)
c
c	backmaskxymax	is an integer array dimensioned from yindexmin
c			to yindexmax.  backmaskxymax(yindex) is the
c			maximum linearized (x,y) index of the region
c			inside the back-projection mask at row "yindex."
c			(output)
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
c			"yindex."  (output)
c
c	forwmaskdmin	is an integer variable that is the minimum (d)
c			index of the region over which the forward
c			projection is computed.  (output)
c
c	forwmaskdmax	is an integer variable that is the maximum (d)
c			index of the region over which the forward
c			projection is computed.  (output)
c
c***********************************************************************


      subroutine maskfbp(dinc, datamaskdmin, datamaskdmax, backmaskymin,
     +	 backmaskymax, backmaskxymin, backmaskxymax, backmaskdstart,
     +	 forwmaskdmin, forwmaskdmax)


      include 'pt6include.f'
      integer angle, backmaskxymax(yindexmin:yindexmax),
     +	 backmaskxymin(yindexmin:yindexmax), backmaskymax, backmaskymin,
     +	 baseindex, datamaskdmax, datamaskdmin, forwmaskdmax,
     +	 forwmaskdmin, index, xindexa, xindexb, yindex
      real backmaskdstart(maskbackindexmin:maskbackindexmax),
     +	 costhe(angindexmin:angindexmax), d,
     +	 dinc(angindexmin:angindexmax), dindexstart, pixvdsamp, radback,
     +	 radbacksq, radtest, sinthe(angindexmin:angindexmax), theta, xa,
     +	 xb, xinit, y


c    Compute sampling ratio.

	 pixvdsamp   = pix   / dsamp

c    Compute the increment table.
c    dinc is the increment in the d index that results from an increment
c	by unity in the x index.

	 do 1000 angle = angindexmin, angindexmax

	    theta = (float(angle - angindexmin) * anginc) + firsttheta
	    costhe(angle) = cos(theta)
	    sinthe(angle) = sin(theta)
	    dinc(angle) =  costhe(angle) * pixvdsamp

 1000	 continue

c    Compute the data mask.  The data mask is the minimum and maximum
c    (d) indices of the region over which the padded data array is
c    nonzero.

	 datamaskdmin = nint(
     +	    dindexcent - (dindexcentorig - float(dindexminorig)))
	 datamaskdmax = nint(
     +	    dindexcent + (float(dindexmaxorig) - dindexcentorig))

c    Compute the back-projection mask.  The back-projection mask
c    includes all of the pixels inside of a circular region of radius
c    radback.

	 backmaskymin = nint(
     +	    yindexcent - (yindexcentorig - float(yindexminorig)))
	 backmaskymax = nint(
     +	    yindexcent + (float(yindexmaxorig) - yindexcentorig))

	 radback = amin1((xindexcentorig - float(xindexminorig)),
     +			 (float(xindexmaxorig) - xindexcentorig),
     +			 (yindexcentorig - float(yindexminorig)),
     +			 (float(yindexmaxorig) - yindexcentorig))
     +		   + 0.5
	 radbacksq = radback * radback

	 do 2000 yindex = backmaskymin, backmaskymax

	    baseindex = yindex * numx
	    y = (float(yindex) - yindexcent)
	    xb = sqrt(radbacksq - (y * y))
	    xa = -xb
	    xindexb = int(xb + xindexcent)
	    xindexa = int(xa + xindexcent + 1.0)
	    backmaskxymin(yindex) = baseindex + xindexa
	    backmaskxymax(yindex) = baseindex + xindexb
	    xinit = float(xindexa) - xindexcent - 1.0

	    do 2005 angle = angindexmin, angindexmax

	       index = (angle * numy) + yindex
	       d = ((xinit * costhe(angle)) - (y * sinthe(angle)))
     +		  * pixvdsamp
	       dindexstart = d + dindexcent
	       backmaskdstart(index) = dindexstart

 2005	    continue

 2000	 continue

c    Compute the forward projection masks.  The forward projection mask
c    includes all of the (pseudo-t,d) bins necessary to cover the back-
c    projection mask.

	 radtest = radback * pixvdsamp

	 forwmaskdmax = int( radtest + dindexcent + 1.0)
	 forwmaskdmin = int(-radtest + dindexcent)


      return
      end
