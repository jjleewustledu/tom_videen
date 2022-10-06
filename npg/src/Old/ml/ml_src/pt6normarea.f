c***********************************************************************
c
c    FUNCTION TITLE		pt6normarea.f
c
c    WRITTEN BY			David G. Politte
c    DATE WRITTEN		April 10, 1985
c    WRITTEN FOR		PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	     This function calculates the area under a one-dimensional
c	normal (Gaussian) probability density.  The mean and standard
c	deviation of the density and the upper and lower limits of the
c	integration are specified via the function arguments.
c
c
c    FUNCTION USAGE
c	This function is called with the following syntax:
c
c	normarea(mean, standev, lowlim, uplim)
c
c	where
c
c	normarea	is a real function variable that is the area
c			under the chosen normal density between the
c			specified limits.  (output)
c
c	mean		is a real variable that is the mean of the
c			normal density function.  (input)
c
c	standev		is a real variable that is the standard
c			deviation of the normal density function.  This
c			argument must be positive.  (input)
c
c	lowlim		is a real variable that is the lower limit of
c			the region over which the area is to be
c			computed.  (input)
c
c	uplim		is a real variable that is the upper limit of
c			the region over which the area is to be
c			computed.  (input)
c
c
c    FUNCTIONAL DETAILS
c	1.  The area is computed by using the polynomial approximation
c	    given in equation 26.2.17 of
c
c	    M. Abramowitz and I. A. Stegun (editors), Handbook of
c	    Mathematical Functions with Formulas, Graphs, and
c	    Mathematical Tables, Dover Publications, Inc., New York,
c	    1972.
c
c	2.  If the lower limit (lowlim) is greater than the upper limit
c	    (uplim) the area computed by this function is negative.
c
c***********************************************************************
 
 
      real function normarea(mean, standev, lowlim, uplim)
 
 
      double precision oneoverstandev, plower, pupper, t, z, zlow,
     +	 zlower, zup, zupper
      real lowlim, mean, standev, uplim


	 oneoverstandev = 1.0d0 / dble(standev)
 
	 zlower = (dble(lowlim) - dble(mean)) * oneoverstandev
	 zlow = dabs(zlower)
	 if (zlow .le. 11.75394d0)
     +	    then
	       t = 1.0d0 / (1.0d0 + (0.2316419d0 * zlow))
	       z = 0.3989422803d0 * dexp(-0.5d0 * zlow * zlow)
	       plower = 1.0d0 - (z * t * ((((((((1.330274429d0 * t)
     +		  - 1.821255978d0) * t) + 1.781477937d0) * t)
     +		  - 0.356563782d0) * t) + 0.319381530d0))
	    else
	       plower = 1.0d0
	 end if
	 if (zlower .lt. 0.0d0)
     +	    then
	       plower = 1.0d0 - plower
	 end if
 
	 zupper = (dble(uplim) - dble(mean)) * oneoverstandev
	 zup = dabs(zupper)
	 if (zup .le. 11.75394d0)
     +	    then
	       t = 1.0d0 / (1.0d0 + (0.2316419d0 * zup))
	       z = 0.3989422803d0 * dexp(-0.5d0 * zup * zup)
	       pupper = 1.0d0 - (z * t * ((((((((1.330274429d0 * t)
     +		  - 1.821255978d0) * t) + 1.781477937d0) * t)
     +		  - 0.356563782d0) * t) + 0.319381530d0))
	    else
	       pupper = 1.0d0
	 end if
	 if (zupper .lt. 0.0d0)
     +	    then
	       pupper = 1.0d0 - pupper
	 end if
 
	 normarea = sngl(pupper - plower)
 
 
      end
