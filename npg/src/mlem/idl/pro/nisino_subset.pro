;+
; NAME:
;    NIsino_subset
;
; PURPOSE:
;    Generates an index, defining a subset of projections from a sinogram.
;    The ML-EM algorithm (or any other iterative algorithm) can the be
;    applied to the subset only, to decrease the processing time.
;    The algorithm generates consecutive subsets, in such a way that
;     1) after number_of_subset calls, all projections have been included
;        in exactly one subset
;     2) Each subset is a) as different (in projection angle) as possible from
;        the all previous subset and
;                       b) as different as possible from the previous subset.
;
;     The idea of accelerating the reconstruction with subsets is due
;     to MH Hudson and RS Larkin, "Accelerated image reconstruction
;     using ordered subsets of projection data", IEEE Trans Med Imaging,
;     1994; 13: 601-609.
;
; CALLING SEQUENCE:
;  subset = NIsino_subset, nrsubsets, nrangles, subsetnum, subsetmask, previous
;
; INPUTS:
;   NRSUBSETS
;     The number of subsets to be generated. This number must not be larger
;     than the total number of projection. There is no other restriction.
;     If the remainder of nrangles diveded by nrsubsets is nonzero, the
;     number of projections per subset will vary. As far as I know, this
;     has no noticable effect on the convergence.
;
;   NRANGLES
;     The number of projections or projection angles.
;
;   SUBSETNUM
;     Supply a variable which is equal to zero in the first call.
;     NIsino_subset will then initialize the variables SUBSETMASK and PREVIOUS.
;     NIsino_subset will increment subsetnum after each subsequent call, 
;     and reset it to zero when all projections are processed.
;     
;   SUBSETMASK
;     Variable for internal use, used to remember which subsets have been
;     processed. Do not modify it between calls to NIsino_subset.
;     Passing internal variables to the caller is preferred to a common block,
;     so that, if ever required, two or more iterative schemes can be executed
;     in an interleaved way.
;     
;   PREVIOUS
;     Internal variable, used to remember which subset was processed most
;     recently. Do not modify.
;
;	
; KEYWORD PARAMETERS:
;    none
;
; OUTPUTS:
;    SUBSET:
;      An array of indices defining the subset. This array can be used by
;      NIproj to restrict the scope of its calculations.
;
;
; COMMON BLOCKS:
;    none
;
; SIDE EFFECTS:
;    none
;
;
; PROCEDURE:
;    See comments in the source.
;
;
; MODIFICATION HISTORY:
; 	Written by:	Johan Nuyts, K.U.Leuven, Belgium
;                       aug-1995
;-


function NIsino_subset, nrsubsets, nrangles, subsetnum, subsetmask, previous

  ; Check whether parameters are meaningful
  ;---------
  if nrsubsets gt nrangles then begin
    print, 'NIsino_subset: nrsubsets must not be larger than nrangles!' + $
           string(7b)
    return, -1
  endif

  ; New start. First subset contains first projection.
  ; Initialize the mask.
  ;--------
  if subsetnum eq 0 then begin
    subset = indgen(ceil(float(nrangles) / nrsubsets)) * nrsubsets
    subsetmask = indgen(nrsubsets) * 0
    subsetmask(0) = 1
    previous = 0

  ; subsetmask:
  ;  - subsetmask index represent first projection of the subset.
  ;  - For processed subsets, mask is set to 1, the others are zero.
  ; Algorithm:
  ; Extend the subsetmask with last element in front and first at the end
  ; to reflect cyclic nature of the projections.
  ; Scan from left to right and right to left, and calculate for every 
  ; element in the mask the distance to the closest subset that was already
  ; processed.
  ; Find the subsetmask elements with maximum distance: one of these must be
  ; processed now.
  ; From these, select the one with maximum distance to the previously
  ; processed subset (stored in PREVIOUS).
  ;----------
  endif else begin

    nlen = n_elements(subsetmask)-1
    subsetmask = [subsetmask(nlen), subsetmask, subsetmask(0)]
    newlen = nlen + 2
    leftdistar  = intarr(newlen+1)
    rightdistar = intarr(newlen+1)
    leftdist   = 1000
    rightdist  = 1000
    for i = 0, newlen do begin
      if subsetmask(i) eq 1 $
        then leftdist  = 0 $
        else leftdist  = leftdist + 1
      leftdistar(i) = leftdist

      j = newlen - i
      if subsetmask(j) eq 1 $
        then rightdist = 0 $
        else rightdist = rightdist + 1
      rightdistar(j) = rightdist
    endfor

    distar = leftdistar < rightdistar
    distar = distar(1:newlen-1)
    subsetmask = subsetmask(1:newlen-1)

    firstproj = where(distar eq max(distar))

    compare = abs(firstproj - previous) < $
              (nrsubsets - abs(firstproj - previous))

    firstproj = firstproj(where(compare eq max(compare)))

    firstproj = firstproj(0)
    subsetmask(firstproj) = 1
    subset = indgen(ceil(float(nrangles) / nrsubsets)) * nrsubsets + firstproj

    previous = firstproj

  endelse

  ; Make sure the subset is not too long. This step reduces the calculation
  ; of the subset to a call to indgen with sufficiently large parameter
  ;-------
  subset = subset(where(subset le (nrangles-1)))
  subsetnum = subsetnum + 1
  if subsetnum ge nrsubsets then subsetnum = 0
  return, subset
  
end
