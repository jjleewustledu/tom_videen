;+
; NAME:
;    NIgrad_priorvalues
;
; PURPOSE: 
;     The routine postulates a prior distribution based on the
;     calling parameters. It computes the derivative of that distribution
;     to the values of an input array, returning an array of the same size with
;     derivatives.
;     The intention is to use this in constraining the set of possible
;     solutions in transmission reconstruction (e.g. NImlostr.pro).
;     Constraining is achieved by assigning a higher a priori probability
;     to a small set of image values. The prior distribution is assumed to
;     be Gaussian-like, so it is specified with a mean value and a
;     standard deviation.
;
; CATEGORY:
;    Reconstruction from projections.
;
; CALLING SEQUENCE:
;    NIgrad_priorvalues, gprior, g2prior, img, priormean, priorstd
;
; INPUTS:
;
;    IMG: The input array (usually a 2D or 3D image). Since the computations
;         are carried out on a pixel by pixel basis, any number of dimensions
;         is allowed.
;
;    PRIORMEAN: An array of image values (e.g. attenuation coefficients) which
;               are assigned maximum a priori value.
;
;    PRIORSTD: An array of standard deviations. This value is used to
;              compute the strength of the prior: stronger for smaller
;              standard deviation
;
;	
; KEYWORD PARAMETERS:
;
;    PRIORWIDTH: An array of numbers, specifying the relative width of
;                each mode of the prior. Default is 1 for all. These
;                numbers are normalized, so multiplying them all with
;                a constant factor doesn't change anything.
;
;    LIKELIHOOD: When this variable exists, it is set to the value of the
;                prior. This value is needed if convergence of the constrained
;                ML algorithm (or MAP algorithm) must be checked.
;              
;
; OUTPUTS:
;    
;   GPRIOR: The output image. It has the same size as IMG.  If GPRIOR
;           does not exist or contains only a single element, an
;           appropriate array is created.
;
;   G2PRIOR: To optimize a function using a gradient approach, one can divide
;            the gradient by minus the second derivate (Newton
;            Raphson). For a kwadratic function, this brings you
;            exactly in the extremum. G2PRIOR receives minus the
;            second derivate in every pixel value.
;
; PROCEDURE:
;
;    A set of Gaussian-shaped a-priori distributions is specified by
;    PRIORMEAN and PRIORSTD. They are combined in a single
;    distribution, not by adding them (because that would change the
;    position of the maxima, desired to be at PRIORMEAN), but by
;    combining them by using the highest value. Consequently, the
;    prior value is determined by a single Gaussian, in the range
;    where that Gaussian dominates the others.  
;    Each Gaussian is connected to its neighbor such that the distance
;    between the two means and the intersection point is proportional
;    to the corresponding entry in priorwidth.
;    If priorwidth is not supplied, same value for all is assumed:
;    the Gaussians then intersect half way between the means.
;
;    Since the function is designed to be combined with our
;    ML-reconstruction algorithms, we compute actually the logarithm
;    of the Gaussians. This is: -(x - x0)^2 / (2 sigma^2) + c, where c
;    is a constant (depending on sigma only). The gradient then simply
;    equals -(x - x0) / sigma^2 (see the local routine
;    NIgrad_prior_compute). This value is valid in the range where the
;    current Gaussian dominates.
;
;    This approach has the disadvantage that the gradient may be highly
;    discontinuous: it jumps from very negative to very positive at the
;    points where two Gaussians are equal. This may result in arbitrary
;    separation of two nearly identical values. We prefer a continuous
;    gradient.
;    The problem is solved by splitting the interval dominated by a single
;    Gaussian in four intervals as follows:
;
;    a-----------------b---------------x0---------------c--------------d
;
;    x=a:  Point where Gaussian equals its left neighbor
;    x=b:  center of interval [a,x0]
;    x=x0: Center point of the Gaussian (reaching its max, derivative is zero)
;    x=c:  center of interval [x0,d]
;    x=d:  Point where Gaussian equals its right neighbor.
;
;    The gradient is now defined as follows:
;
;      x in [a,b]:  g =  (x - xa) / sigma^2
;      x in [b,c]:  g = -(x - x0) / sigma^2
;      x in [c,d]:  g =  (x - d)  / sigma^2
;
;    The prior is defined as the integral of this gradient. It behaves like
;    a parabol (the log of a Gaussian) near x0, as was required.
;    In addition, it is smoothly connected to another parabol at b and c,
;    which reaches its minimum in a, resp. d.
;    Consequently, there are no discontinuities at the points where the
;    prior reaches it minimum.
;
;    Minus the second derivate equals 1/sigma^2 in [b,c] and
;    -1/sigma^2 in [a,b] and [c,d]. However, in G2PRIOR, we will
;    allways return 1/sigma^2, so that the sign of GPRIOR will not be
;    changed. Otherwise, the optimization procedure will converge
;    towards a or d when in the intervals [a,b] or [c,d].
;
;
; MODIFICATION HISTORY:
; 	Written by:	Johan Nuyts, dec 1995
;-
;---------------------------------------------------------------
pro NIgrad_prior_compute, gprior, g2prior, img, daar, meanval, stdval, $
                  priorval, invert=invert, likelihood = likelihood

  stdval2 = stdval * stdval  
  if daar(0) ge 0 then begin
    if keyword_set(invert) $
      then gprior(daar) = -(meanval - img(daar)) / stdval2 $
      else gprior(daar) =  (meanval - img(daar)) / stdval2
    g2prior(daar) = 1. / stdval2
    if n_elements(likelihood) ne 0 then begin
      likelihood = likelihood - $
                   total((img(daar) - priorval)^2 / (2. * stdval2))
    end
  end
end

;---------------------------------------------------------------
pro NIgrad_priorvalues, gprior, g2prior, img, priormean, priorstd,  $
                        priorwidth = priorwidth, likelihood = likelihood

;
; sort in ascending order
;
daar = sort(priormean)
meanval = priormean(daar)
stdval = priorstd(daar)

;
; Produce empty output image
;
if n_elements(gprior) gt 1 then gprior(*) = 0 $
                           else gprior = img * 0
if n_elements(g2prior) gt 1 then g2prior(*) = 0 $
                            else g2prior = img * 0
if n_elements(priorwidth) eq 0 $
   then priorwidth = priormean * 0 + 1.0

last  = n_elements(meanval) -1

;
; values smaller than smallest mu
;
daar = where(img le meanval(0))
NIgrad_prior_compute, gprior, g2prior, img, daar, meanval(0), stdval(0), $
                      meanval(0), likelihood = likelihood

;
; values larger than largest mu
;
daar = where(img gt meanval(last))
NIgrad_prior_compute, gprior, g2prior, img, daar, meanval(last), $
           stdval(last), meanval(last), likelihood = likelihood


if n_elements(likelihood) gt 0 then lik = 0
;
; values in between
;
for left = 0, last-1 do begin
  right = left+1

  xval = (priorwidth(right)*meanval(left) + priorwidth(left)*meanval(right)) $
         / (priorwidth(right) + priorwidth(left))

  xval = [meanval(left), $
          (xval + meanval(left)) / 2., $
          xval, $
          (xval + meanval(right)) / 2., $
          meanval(right)]
  
  daar = where((img gt xval(0)) and (img le xval(1)))
  NIgrad_prior_compute, gprior, g2prior, img, daar, xval(0), stdval(left), $
                        meanval(left), likelihood = lik

  daar = where((img gt xval(1)) and (img le xval(2)))
  NIgrad_prior_compute, gprior, g2prior, img, daar, xval(2), stdval(left), $
                        meanval(left), likelihood = lik, /inv

  daar = where((img gt xval(2)) and (img le xval(3)))
  NIgrad_prior_compute, gprior, g2prior, img, daar, xval(2), stdval(right), $
                    meanval(right), likelihood = lik, /inv

  daar = where((img gt xval(3)) and (img le xval(4)))
  NIgrad_prior_compute, gprior, g2prior, img, daar, xval(4), stdval(right), $
                    meanval(right), likelihood = lik
                          
endfor

if n_elements(lik) gt 0 then likelihood = lik
end

