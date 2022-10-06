c***********************************************************************
c
c    PROGRAM TITLE		pt6include.f.h
c
c    WRITTEN BY			David G. Politte
c    DATE WRITTEN		July 16, 1991
c    WRITTEN FOR		PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	     This include file contains constants that describe
c
c	a) the sampling and array sizes of the images,
c	b) the sampling and array sizes of the data,
c	c) the transverse measurement error of the scanner,
c	d) the size in bytes of a real or integer datum,
c	e) the size of the masks used to minimize the number of
c	   computations,
c	f) the number of bytes in the initial informational header.
c
c***********************************************************************


      integer angindexmax, angindexmin, daindexmax, daindexmaxorig,
     +	 daindexmin, daindexminorig, dindexmax, dindexmaxorig,
     +	 dindexmin, dindexminorig, intsize, maskbackindexmax,
     +	 maskbackindexmin, maskforwindexmax, maskforwindexmin, nbhead,
     +	 numang, numd, numda, numdaorig, numdorig, numx, numxfin,
     +	 numxorig, numxy, numxyfin, numxyorig, numy, numyfin, numyorig,
     +	 realsize, xindexmax, xindexmaxfin, xindexmaxorig, xindexmin,
     +	 xindexminfin, xindexminorig, xyindexmax, xyindexmaxfin,
     +	 xyindexmaxorig, xyindexmin, xyindexminfin, xyindexminorig,
     +	 yindexmax, yindexmaxfin, yindexmaxorig, yindexmin,
     +	 yindexminfin, yindexminorig
      integer slicerecord
      real anginc, dindexcent, dindexcentorig, dsamp, firsttheta,
     +	 fwhmbhigh, fwhmblow, numstdev, pi, pix, tindexcent, tsamp,
     +	 xindexcent, xindexcentfin, xindexcentorig, yindexcent,
     +	 yindexcentfin, yindexcentorig


c    x, the horizontal coordinate of an image, increases from left to
c    right and is assigned an index from xindexmin to xindexmax,
c    inclusive.  The geometric center of the scanner, xindexcent, is at
c    the boundary between two pixels.  The first four parameters below
c    describe the padded image in which most of the computations are
c    done.  The "orig" and "fin" parameters describe images that are
c    read or written by various programs.

#ifdef TWO_87_MM
	 parameter (xindexmin      =   0  )
	 parameter (xindexmax      = 127  )
	 parameter (numx           = 128  )
	 parameter (xindexcent     =  63.5)

	 parameter (xindexminorig  =   0  )
	 parameter (xindexmaxorig  =  99  )
	 parameter (numxorig       = 100  )
	 parameter (xindexcentorig =  49.5)

	 parameter (xindexminfin   =   0  )
	 parameter (xindexmaxfin   =  99  )
	 parameter (numxfin        = 100  )
	 parameter (xindexcentfin  =  49.5)
#endif

#ifdef TWO_00_MM
	 parameter (xindexmin      =   0  )
	 parameter (xindexmax      = 183  )
	 parameter (numx           = 184  )
	 parameter (xindexcent     =  91.5)

	 parameter (xindexminorig  =   0  )
	 parameter (xindexmaxorig  = 143  )
	 parameter (numxorig       = 144  )
	 parameter (xindexcentorig =  71.5)

	 parameter (xindexminfin   =   0  )
	 parameter (xindexmaxfin   = 127  )
	 parameter (numxfin        = 128  )
	 parameter (xindexcentfin  =  63.5)
#endif

c    y, the vertical coordinate of an image, increases from top to
c    bottom and is assigned an index from yindexmin to yindexmax,
c    inclusive.  The geometric center of the scanner, yindexcent, is at
c    the boundary between two pixels.  The first four parameters below
c    describe the padded image in which most of the computations are
c    done.  The "orig" and "fin" parameters describe images that are
c    read or written by various programs.

#ifdef TWO_87_MM
	 parameter (yindexmin      =   0  )
	 parameter (yindexmax      = 127  )
	 parameter (numy           = 128  )
	 parameter (yindexcent     =  63.5)

	 parameter (yindexminorig  =   0  )
	 parameter (yindexmaxorig  =  99  )
	 parameter (numyorig       = 100  )
	 parameter (yindexcentorig =  49.5)

	 parameter (yindexminfin   =   0  )
	 parameter (yindexmaxfin   =  99  )
	 parameter (numyfin        = 100  )
	 parameter (yindexcentfin  =  49.5)
#endif

#ifdef TWO_00_MM
	 parameter (yindexmin      =   0  )
	 parameter (yindexmax      = 183  )
	 parameter (numy           = 184  )
	 parameter (yindexcent     =  91.5)

	 parameter (yindexminorig  =   0  )
	 parameter (yindexmaxorig  = 143  )
	 parameter (numyorig       = 144  )
	 parameter (yindexcentorig =  71.5)

	 parameter (yindexminfin   =   0  )
	 parameter (yindexmaxfin   = 127  )
	 parameter (numyfin        = 128  )
	 parameter (yindexcentfin  =  63.5)
#endif

c    The following parameters are used to access an (x,y) array that is
c    treated as a one-dimensional array.  The (x) index changes fastest.

	 parameter (xyindexmin = (yindexmin * numx) + xindexmin)
	 parameter (xyindexmax = (yindexmax * numx) + xindexmax)
	 parameter (numxy = numx * numy)

	 parameter (xyindexminorig
     +	    = (yindexminorig * numxorig) + xindexminorig)
	 parameter (xyindexmaxorig
     +	    = (yindexmaxorig * numxorig) + xindexmaxorig)
	 parameter (numxyorig = numxorig * numyorig)

	 parameter (xyindexminfin
     +	    = (yindexminfin * numxfin) + xindexminfin)
	 parameter (xyindexmaxfin
     +	    = (yindexmaxfin * numxfin) + xindexmaxfin)
	 parameter (numxyfin = numxfin * numyfin)

c    t is the time-of-flight coordinate of the data.  PETT VI does not
c    measure differential time-of-flight, so the parameter given below
c    is only used to calculate a "pseudo" time-of-flight coordinate when
c    line integrals are being computed during a forward projection.

#ifdef TWO_87_MM
	 parameter (tindexcent = 63.5)
#endif

#ifdef TWO_00_MM
	 parameter (tindexcent = 91.5)
#endif

c    d, the transverse-distance coordinate of the data, is assigned an
c    index from dindexmin to dindexmax, inclusive.  The geometric center
c    of the scanner, dindexcent, is at the boundary between two bins.
c    The first four parameters below refer to the padded array in which
c    most of the computations are done.  The "orig" parameters refer to
c    actual PETT VI data.

	 parameter (dindexmin      =   0  )
	 parameter (dindexmax      = 127  )
	 parameter (numd           = 128  )
	 parameter (dindexcent     =  63.5)

	 parameter (dindexminorig  =   0  )
	 parameter (dindexmaxorig  = 107  )
	 parameter (numdorig       = 108  )
	 parameter (dindexcentorig =  53.5)

c    The following parameters are the minimum and maximum indices of the
c    quantized angles, the number of angles, and the first angle (in
c    radians).  Angles increment in a counter-clockwise direction.

	 parameter (pi = 3.141592654)

	 parameter (angindexmin =  0)
	 parameter (angindexmax = 71)
	 parameter (numang      = 72)
	 parameter (firsttheta  = pi)

c    The following parameters are used to access a distance-angle (d,a)
c    array that is treated as a one-dimensional array.  The (d) index
c    changes fastest.

	 parameter (daindexmin = (angindexmin * numd) + dindexmin)
	 parameter (daindexmax = (angindexmax * numd) + dindexmax)
	 parameter (numda = numd * numang)

	 parameter (daindexminorig
     +	    = (angindexmin * numdorig) + dindexminorig)
	 parameter (daindexmaxorig
     +	    = (angindexmax * numdorig) + dindexmaxorig)
	 parameter (numdaorig = numdorig * numang)

c    The following parameters describe the sampling of the images and
c    data.  Note:  The size of an image pixel, pix, and the sampling
c    interval of the data in the distance direction, dsamp, were
c    intended to be 2.7 and 2.5 mm, respectively.  These revised values
c    are due to Tom Videen.  The parameter tsamp refers to sampling
c    along the time-of-flight direction - see the note that describes
c    the parameter tindexcent.  The parameter anginc is the angular
c    increment between quantized angles.

#ifdef TWO_87_MM
	 parameter (pix	  = 2.87)
	 parameter (tsamp = pix )
#endif

#ifdef TWO_00_MM
	 parameter (pix   = 2.00)
	 parameter (tsamp = pix )
#endif

	 parameter (dsamp = 2.65740741)
	 parameter (anginc = pi / numang)

c    The following parameters describe the one-dimensional measurement
c    error of PETT VI in the distance direction.  The measurement error
c    is approximated by a one-dimensional Gaussian that is truncated to
c    to zero beyond +/- numstdev standard deviations and that has a
c    full-width at half-maximum of fwhmblow mm when the scanner is
c    operated in the low-resolution mode and fwhmbhigh mm when the
c    scanner is operated in the high-resolution mode.

	 parameter (fwhmbhigh =  7.1)
	 parameter (fwhmblow  = 11.7)
	 parameter (numstdev  =  3.0)

c    These parameters are the number of bytes needed to store a real or
c    integer variable.

	 parameter (intsize  = 4)
	 parameter (realsize = 4)

c    These parameters are the minimum and maximum indices of the arrays
c    used to store the masks.

	 parameter (maskforwindexmin = daindexmin)
	 parameter (maskforwindexmax = daindexmax)
	 parameter (maskbackindexmin = (angindexmin * numy) + yindexmin)
	 parameter (maskbackindexmax = (angindexmax * numy) + yindexmax)

c    This parameter is the number of bytes in the informational header
c    at the beginning of each file.  The data start immediately after
c    the header.

	 parameter (nbhead = 256)

c    This is the size in bytes of a single slice of data in a 7-slice
c    Concurrent data file.

	 parameter (slicerecord = (123 * 256))
