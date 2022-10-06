;+
; NAME:
;         NIfilterfunction
;
; PURPOSE:
;         Calculate the 1D frequency representation for the specified
;         filter function.  The filter can be directly multiplied
;         applied to the result of a forward FFT.  The function is
;         designed to be used for filtered backprojection (nifbp.pro).
;
; CALLING SEQUENCE:
;         filterarray = NIfilterfunction(filter, nrelements)
;
; INPUTS:
;
;   FILTER: 'rect' : an array of ones is returned.
;           'hann' : Hann low pass filter
;           'but'  : Butterworth low pass filter
;
;   NRELEMENTS: The number of elements in the returned
;               filterarray. This value equals the number of elements
;               of the array that you want to filter.
;
; 
; KEYWORD PARAMETERS:
;
;   CUTOFF_FREQ: The interpretation depends on the filter.
;                For 'rect': all frequencies > cutoff_freq are set to
;                zero, the others to 1.
;                For 'hann': the filter becomes zero at the
;                cutoff_freq (with zero derivative)
;                For 'but' : the filter amplitude equals 0.5 at the
;                cutoff_freq.
;
;   POWER:
;        The power of the Butterworth filter. Ignored when an other
;        filter is selected.  Default is 10.
;
;   RAMP:
;        When set, the specified low-pass filter is multiplied by a
;        ramp filter.
;
;   TWOD:
;        When set, a filter image is returned, which can be directly
;        multiplied with the FFT of an image.
;        Otherwise, the filter is returned as a one dimensional array,
;        to be multiplied with the FFT of a 1D array.
;
; OUTPUTS:
;
;   FILTERARRAY: a 1D array of NRELEMENTS elements, containing the filter
;                amplitude.  The first element is the DC point,
;                frequencies then increase up to the Nyquist
;                frequency. The second half of the array is a mirrored
;                version of the first half.
;                
;
; MODIFICATION HISTORY:
;    written by: Stefaan Vandenberghe en Swana Vanacker,  dec 1995
;    modification: 
;        jan 1996, Johan Nuyts: several bugs fixed
;        mar 1996, Johan Nuyts: adding small constant to the ramp filter,
;                           to avoid over-attenuation of low frequencies.
;-
;-----------------------------------------------------------------------------

FUNCTION NIfilterfunction, filter, nrelements, cutoff_freq = cutoff_freq, $
                           power = power, ramp = ramp, twoD= twoD


; An array with frequencies: starting from zero to Nyquist, followed by
; -Nyquist to zero. (Same as after FFT).
; Note that the sign of the frequencies is irrelevant, the filter is
; always a function of the absolute value. So for the two-D frequency image,
; we don't care restoring the correct sign as is done in the 1-D
; frequencyarray.
;---------------------------------------------------------

frequencyarray = findgen(nrelements)
middle         = fix((nrelements - 1)/2)
frequencyarray(middle +1:nrelements-1) = $
                       -REVERSE(frequencyarray(0:(nrelements-1)-(middle +1)))
IF keyword_set(twoD) THEN BEGIN
  kwadfreq =  frequencyarray^2 # (fltarr(1,nrelements)+1)
  frequencyarray =  sqrt(kwadfreq + transpose(kwadfreq))
ENDIF

if n_elements(cutoff_freq) eq 0 then cutoff_freq = 0.5
cutoff = nrelements * cutoff_freq


; calculation of the filterarrays for the different filters
;----------------------------------------------------------

CASE filter of
'rect': BEGIN
        filterarray = frequencyarray*0 + 1
        daar = where(abs(frequencyarray) gt cutoff)
        if daar(0) ne -1 then filterarray(daar) = 0
        END

'hann': BEGIN
        filterarray =   0.5 + 0.5 * cos (!pi * frequencyarray/cutoff)
        daar = where(abs(frequencyarray) gt cutoff)
        if daar(0) ne -1 then filterarray(daar) = 0
        END

'but': BEGIN
       if n_elements(power) eq 0 then power = 10
       filterarray = 1.0 / (1+ (abs(frequencyarray)/ cutoff)^power)
       END

else: BEGIN
      print, 'Illegal filter function: ', filter
      filterarray = -1
      END

ENDCASE

; The DC-amplification should not be set to zero. In the analogue
; case, there are an infinite number of projections, and the
; DC-amplification of the backprojection goes to infinity.  In the
; digital case, the number of projections is limited, and the
; DC-amplification of the backprojection is a finite value. Hence, the
; ramp-filter must be adapted. There is a paper about this, but I
; don't have it at the moment, so I solved the problem by adding a
; small value, determined heuristically on a simulation. The result is
; that decent reconstructions are obtained without having to use large
; zeropadding values in the reconstruction (to increase the sampling in the
; frequency domain). The influence of the term decreases when the
; number of frequency samples is increased. The tests suggest that in all
; cases superior reconstructions are obtained with this term, but the
; difference becomes negligible for large zeropadding. 0.37
;-------
if keyword_set(ramp) then $
   filterarray = filterarray * (abs(frequencyarray) + 0.)  / nrelements

RETURN, filterarray

END

