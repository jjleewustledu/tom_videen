; $Id: fit_triangle_1n.pro,v 1.1 1996/02/14 17:57:12 tom Exp $
;
;+
; NAME:
;   FIT_TRIANGLE_1N
;
; PURPOSE:
;   Fit line segments to one triangular-shaped and one N-shaped fiducial.
;
; CATEGORY:
;   Image processing.
;
; CALLING SEQUENCE:
;   FIT_TRIANGLE_1N
;
; INPUTS:
;   None.  Prompts for filename.
;
; OUTPUTS:
;   File:  Containing the coordinates of 5 points of intersection.
;
; MODIFICATION HISTORY:
;   Written by: Tom Videen, June 1995
;-

pro fit_triangle_1n

START:
rootname = " "
read, PROMPT="Root name: ", rootname
file1 = rootname + "backndiag.img"
if (not exist_file(file1)) then begin
    print, "ERROR: File ", file1, " not found"
    goto, START
endif
outfilename = rootname + ".fid"
if (exist_file(outfilename)) then begin
    get_lun, lu
    openw, lu, outfilename
    close, lu
    free_lun, lu
endif
 
parallel = 'Y'
read,PROMPT="Diagonal branch segmented parallel to end lines <Y> ",parallel
if (parallel eq 'N' or parallel eq 'n') $
    then flag = 0 $
else if (parallel eq '' or parallel eq 'Y' or parallel eq 'y') then $
    flag = 1 $
else begin
    print,"Error"
    exit
endelse
 
;!P.MULTI = [0,1,1]
;plot,[10],[10],xrange=[0,20],yrange=[0,20],psym=1
;dummy = " "
;read, PROMPT="Adjust size for 2 side-by-side plots",dummy

set_plot, 'ps'
device, filename="fiducials.ps",/landscape,xsize=9.8,ysize=4.3,xoffset=2.0,yoffset=10.5,/inches
loadct,0

!P.MULTI = [0,2,1]
fit_fiducial_N_nc, rootname, "backn", flag
fit_fiducial_triangle, rootname, "lftri", flag

device,/close
free_lun,lu
ch="X"
read, PROMPT="Type X to exit  ", ch
if (ch ne 'X' and ch ne 'x') then goto, START
exit
end

