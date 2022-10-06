;+
; NAME:
;    NIkul_coeff
;
; PURPOSE:
;    Pre-computation of the linear interpolation coefficients required for
;    projection and backprojection.
;
; CATEGORY:
;    Reconstruction.
;
; CALLING SEQUENCE:
;    pcoeff = NIkul_coeff(nrdet, nrproj, startangle=S, deltaangle=D, $
;                  radius=R, coroffset=C, arc_radius = arc_radius)
;
; INPUTS:
;    NRDET: Number of detectors (or number of columns in the sinogram)
;
;    NRPROJ: Number of projections (or number of rows in the sinogram).
;
;	
; KEYWORD PARAMETERS:
;    STARTANGLE: projection angle in radians of first sinogram row.
;                Zero means projection along y-axis (and upward). 
;                Default: zero.
;
;    DELTAANGLE: offset in radians to next projection angle.
;                Positive is clockwise.
;                Default: NRPROJ / !pi
;
;    RADIUS:     Radius of the field of view in pixels.
;                If very large or set to -1, the (back)projection will be
;                computed for the entire reconstruction image. Otherwise,
;                the computation will be limited to pixels within the
;                circle.
;                Warning: in iterative reconstruction, artifacts will occur
;                when reconstructing a truncated field of view. RADIUS should
;                be larger than for the largest enclosed circle.
;                Default: -1
;
;    COROFFSET   Center of rotation offset. When not set, ignored.
;                When set to a single value, the same center of rotation
;                offset is used for all angles.
;                When set to an array of length NRPROJ, a different center of
;                rotation offset is used for every angle.
;
;    ARC_RADIUS  Radius of the PET-system in pixels (convert if known in
;                cm and sinogram pixels known). Ignored if set to zero or not
;                set. If specified, an arc-correction is included in the
;                computations of the interpolation coefficients.
;
; OUTPUTS:
;    PCOEFF: a long int, which contains a pointer that can be used by
;            NIkul_proj and NIkul_free.
;
;
; COMMON BLOCKS:
;    Uses NC_lib_common to find the reconstruction library.
;
; SIDE EFFECTS:
;    A structure of a few Mb will be allocated by the C-procedure. This
;    memory cannot be accessed by IDL, it is intended to be reused by other
;    C-modules. Keep the value of PCOEFF unchanged, otherwise that memory
;    gets lost.
;
;
; PROCEDURE:
;    Calls a routine from the C reconstruction library, which computes the
;    coefficients and stores them in newly allocated memory, which can be
;    accessed by other modules from the same library.
;    IDL cannot access that memory.
;    Use NIkul_free to deallocate the memory.
;
;
; MODIFICATION HISTORY:
; 	Written by:	Johan Nuyts, 10 apr 1996
;-

function NIkul_coeff, nrdet, nrproj, startangle = startangle, $
                      deltaangle = deltaangle, radius = radius, $
                      coroffset = coroffset, arc_radius= arc_radius

common NC_lib_common


if n_elements(startangle) eq 0 then startangle = 0.0
if n_elements(deltaangle) eq 0 then deltaangle = !pi / nrproj
if n_elements(radius)     eq 0 then radius     = -1.0
IF n_elements(arc_radius) EQ 0 THEN arc_radius =  0.0;
if n_elements(coroffset)  eq 0 $
   then cor = fltarr(nrproj) $
   else if n_elements(coroffset) eq 1 $
     then cor = fltarr(nrproj) + float(coroffset(0)) $
     else if n_elements(coroffset) eq nrproj $
        then cor = float(coroffset) $
        else begin
        		 print, "NIkul_coeff: size of coroffset incorrect!"
        		 return, 0
             endelse

nrdet      = fix(nrdet)
nrproj     = fix(nrproj)
startangle = float(startangle)
deltaangle = float(deltaangle)
radius     = float(radius)
arc_radius = float(arc_radius)
prcf = call_external(NC_recon, 'NIkul_coeff3', nrdet, nrproj, startangle, $
              deltaangle, radius, cor, arc_radius)

return, prcf

end
