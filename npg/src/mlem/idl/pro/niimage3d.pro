PRO NIimage3d, image, noerase=noerase, minimum=minimum, maximum=maximum, $
               INTERP = interp, CUBIC = cubic, number=number, $
               firstplane = firstplane, $
               xpos=xpos, ypos=ypos, xsize=xsize, ysize=ysize

imsize = size(image)
IF imsize(0) LE 2 THEN nplanes = 1 $
                  ELSE nplanes = imsize(3)
IF n_elements(firstplane) EQ 0 THEN firstplane = 1

if n_elements(xpos) eq 0 then xpos = 0
if n_elements(ypos) eq 0 then ypos = 0
if n_elements(xsize) eq 0 then xsize = !d.x_size
if n_elements(ysize) eq 0 then ysize = !d.y_size

NIcompute_dispsize, imsize(1), imsize(2), nplanes, 1, $
        xsize=xsize, ysize=ysize, zoomfactor=zoom, slices_perrow=imgperrow
        

plane = 0
IF NOT keyword_set(noerase) THEN erase

rowsize = fix(zoom * imsize(2))
colsize = fix(zoom * imsize(1))

FOR row=0, fix(ysize / rowsize + 0.01)-1 DO BEGIN
  FOR col=0, fix(xsize / colsize + 0.01)-1 DO BEGIN
    NIimage, image(*,*,plane), xs=colsize, ys=rowsize, $
           xpos= xpos + col * colsize, $
           ypos= ypos + ysize-(row+1)*rowsize, $
           /noerase, minimum=minimum, maximum=maximum, $
           INTERP = interp, CUBIC = cubic
    IF keyword_set(number) THEN $
      xyouts, col * colsize, ysize-(row+1)*rowsize, $
       NIstring(plane + firstplane), $
       /dev, col=number
    plane = plane + 1
    IF plane GE nplanes THEN return
  endfor
ENDFOR

END
