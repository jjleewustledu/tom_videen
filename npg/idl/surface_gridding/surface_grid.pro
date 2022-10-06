PRO Example


	; Create some XYZ data to represent the surface.

seed = -1L
x = RandomU(seed, 41)
y = RandomU(seed, 41)
z = Exp(-3*((x - 0.5)^2 + (y - 0.5)^2))

	; Set the window size.

WINDOWSIZE = 300

	; Show the data points randomly distributed.

Window, Title='XY Point Distribution', XSize=WINDOWSIZE, YSize=WINDOWSIZE, Xpos=0, YPos=0
Plot, Findgen(41), /NoData, XRange=[0, Max(x)], YRange=[0, Max(y)]
PlotS, x, y, PSym=1

	; Create the set of Delaunay triangles. The variables
	; "triangles" and "boundaryPts" are output variables.

Triangulate, x, y, triangles, boundaryPts

	; Display the triangles.

s = Size(triangles, /Dimensions)
num_triangles = s[1]
Window, /Free, Title='Delanay Triangles', XSize=WINDOWSIZE, YSize=WINDOWSIZE, XPos=WINDOWSIZE + 10, YPos=0
Plot, Findgen(41), /NoData, XRange=[0, Max(x)], YRange=[0, Max(y)]
PlotS, x, y, PSym=1

   ; Draw the triangles.

FOR j=0,num_triangles-1 DO BEGIN
    thisTriangle = [triangles[*,j], triangles[0,j]]
    PlotS, x[thisTriangle], y[thisTriangle]
ENDFOR

   ; Set the grid spacing. Here we want 0.01 in X and 0.05 in Y.

gridSpace = [0.01, 0.05]

   ; Grid the Z data, using the triangles.The variables "xvector"
   ; and "yvector" are output variables.

griddedData = TriGrid(x, y, z, triangles, gridSpace, $
   XGrid=xvector, YGrid=yvector)

   ; Display the surface.

Window, /Free, Title='Initial Surface', $
   XSize=WINDOWSIZE*1.5, YSize=WINDOWSIZE, XPos=0, YPos=WINDOWSIZE + 30
Surface, griddedData, xvector, yvector, /NoData
Surface, griddedData, xvector, yvector, XStyle=4, $
   YStyle=4, ZStyle=4, /NoErase

   ; Display the smoothed surface.

griddedData = TriGrid(x, y, z, triangles, gridSpace, XGrid=xvector, $
   YGrid=yvector, /Quintic)
Window, /Free, Title='Smoothed Surface', $
   XSize=WINDOWSIZE*1.5, YSize=WINDOWSIZE, $
   XPos=WINDOWSIZE*1.5 + 10, YPos=WINDOWSIZE + 30
Surface, griddedData, xvector, yvector, /NoData
Surface, griddedData, xvector, yvector, XStyle=4, $
   YStyle=4, ZStyle=4, /NoErase

   ; Display the extrapolated, smoothed surface.

griddedData = TriGrid(x, y, z, triangles, gridSpace, XGrid=xvector, $
   YGrid=yvector, /Quintic, Extrapolate=boundaryPts)
Window, /Free, Title='Extrapolated Surface', $
   XSize=WINDOWSIZE*1.5, YSize=WINDOWSIZE, $
   XPos=2*WINDOWSIZE*1.5 + 20, YPos=WINDOWSIZE + 30
Surface, griddedData, xvector, yvector, /NoData
Surface, griddedData, xvector, yvector, XStyle=4, YStyle=4, ZStyle=4, /NoErase

END
