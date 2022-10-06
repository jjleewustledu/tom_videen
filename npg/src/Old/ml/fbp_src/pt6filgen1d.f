c***********************************************************************
c
c    SUBROUTINE TITLE		pt6filgen1d.f
c
c    WRITTEN BY			David G. Politte
c    DATE WRITTEN		October 5, 1989
c    WRITTEN FOR		PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	     This subroutine generates the one-dimensional, spatial-
c	domain filters used by "pt6fbp" to perform filtered back-
c	projection reconstructions.  Note that the filter is even and
c	that this program computes approximately half of it.  The
c	frequency-domain characterization of the filter is given by
c
c		FILTER(f) = pi * (abs(f) * H(f) / P(f)) * W(f),
c
c	where
c
c	pi	is the familiar mathematical constant (3.14159...).
c
c	abs(f)	is the Jacobian of the transformation from Cartesian to
c		polar coordinates.
c
c	H(f)	is a slice through the origin of the Fourier transform
c		of the resolution function h(x).  We take h(x) to be a
c		two-dimensional circularly symmetric Gaussian with a
c		profile having a full width at half maximum (FWHM) of
c		"fwhmr" millimeters, which corresponds to the
c		"reconstructed resolution" of the image.  Then H(f) is
c		also a circularly symmetric two-dimensional Gaussian
c		given by
c
c		H(f) = exp(-2 * pi**2 * sigmar**2 * f**2),
c
c		where sigmar is the standard deviation corresponding to
c		fwhmr, f is one-dimensional frequency, exp denotes base
c		e (2.71828...) exponentiation, and "**" denotes the
c		involution operation.
c
c	P(f)	is the Fourier transform of the one-dimensional
c		measurement error, given by
c
c		P(f) = exp(-2 * pi**2 * sigmab**2 * f**2),
c
c		where sigmab is the standard deviation of the spatial
c		error due mainly to finite detector size.
c
c	W(f)	is a window function that suppresses high frequency
c		noise.  We have chosen
c
c		       1, 0 <= f < f1,
c		W(f) = 0.5 * (1 + cos(pi * (f - f1) / (f2 - f1))),
c			   f1 <= f < f2,
c		       0, f >= f2,
c
c		where f1 and f2 are each the frequency
c
c		f1 = f2 = (sigmab / sigmar) / (2.0 * dsamp),
c
c		where dsamp is the sampling interval along the distance
c		axis of the data.
c
c
c    SUBROUTINE USAGE
c	This subroutine is called with the following syntax:
c
c	call filgen1d(fwhmb, fwhmr, filter)
c
c	where
c
c	fwhmb		is a real variable that is the full width at
c			half maximum of the measurement error in the
c			distance direction.  (input)
c
c	fwhmr		is a real variable that is the full width at
c			half maximum of the resolution function.  This
c			is the reconstructed resolution of the image.
c			(input)
c
c	filter		is a real array dimensioned from 0 to (numd - 1)
c			that contains the center value and half of the
c			filter coefficients.  (output)
c
c***********************************************************************


      subroutine filgen1d(fwhmb, fwhmr, filter)


      include 'pt6include.f'
      integer dindex, freqsamp, freqsampmax, freqsampmin, numfreqsamp
      real confac, cosarg, cosmult, d, deltaf, exparg, expmult,
     +	 filter(0:(numd - 1)), freq, freqsq, freq1, freq2, fwhmb, fwhmr,
     +	 nyquist, scalefactor, sigbsq, sigmab, sigmar, sigrsq, sum,
     +	 twopi, twopisq, window


c    Set arithmetic constants.

	 twopi = 2.0 * pi
	 twopisq = 2.0 * pi * pi

c    Convert all full width at half maximum quantities to standard
c    deviations and variances.

	 confac = 2.0 * sqrt(2.0 * alog(2.0))
	 sigmab = fwhmb / confac
	 sigmar = fwhmr / confac

	 sigbsq = sigmab * sigmab
	 sigrsq = sigmar * sigmar

c    Set constants used in the high-frequency cutoff window.
c    freq1:  frequency at which cutoff begins
c    freq2:  frequency at which cutoff ends (filter is 0 beyond freq2)

	 nyquist = 1.0 / (2.0 * dsamp)
	 freq1 = nyquist * (sigmab / sigmar)
	 freq2 = freq1

c    Set constants associated with sampling in the frequency domain.

	 numfreqsamp = numd * 16
	 deltaf = nyquist / float(numfreqsamp)
	 freqsampmin = 0
	 freqsampmax = int((freq2 / deltaf) - 0.5)

c    Set miscellaneous constants.

	 expmult = twopisq * (sigbsq - sigrsq)
	 scalefactor = twopi * deltaf * dsamp * ((pix * pix) / dsamp)

c    Compute the filter.

	 do 1000 dindex = 0, (numd - 1)

	    d = float(dindex) * dsamp
	    cosmult = twopi * d
	    sum = 0.0

	    do 1005 freqsamp = freqsampmin, freqsampmax

	       freq = (float(freqsamp) + 0.5) * deltaf
	       freqsq = freq * freq
	       exparg = expmult * freqsq
	       cosarg = cosmult * freq

	       if (freq .lt. freq1) then

		  sum = sum + (freq * exp(exparg) * cos(cosarg))

	       else if (freq .lt. freq2) then

		  window = 0.5 * (1.0 + cos(pi * (freq - freq1)
     +		     / (freq2 - freq1)))
		  sum = sum
     +		     + (freq * exp(exparg) * cos(cosarg) * window)

	       end if

 1005	    continue

	    filter(dindex) = sum * scalefactor

 1000	 continue


      end
