PRO vdisplay__define
   tmp = {VDISPLAY,		$
          set      : 0,		$ ; display is set
          id       : 1,		$ ; window index
          init     : 0,		$ ; window initialized
          xsize    : 128,	$ ; horizontal window size
          ysize    : 128,	$ ; vertical window size
          zoom     : 1,		$ ; zoom factor
          nx       : 1,		$ ; number of images per row
          ny       : 1,		$ ; number of images per col
          start    : 1,		$ ; first plane
          space    : 1,		$ ; plane spacing
          min      : 0.0,	$ ; minimum value for display
          max      : 0.0,	$ ; maximum value for display
          amin     : 0.0,	$ ; actual minumum value displayed
          amax     : 0.0,	$ ; actual maximum value displayed
          cut      : 0L,	$ ; cutoff value for display
          type     : 0,		$ ; 0=auto, 1=manual
          range    : 0,		$ ; 0=pos, 1=neg, 2=both
          bot      : 0,		$ ; bottom of color LUT
          nc       : 1,		$ ; number of colors in LUT
          table    : 0,		$ ; LUT number
          voi      : ptr_new()	$ ; VOI points
         }
END
