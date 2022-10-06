c***********************************************************************
c
c    SUBROUTINE TITLE		pt6unpadimgfin.f
c
c    WRITTEN BY			David G. Politte
c    DATE WRITTEN		October 1, 1989
c    WRITTEN FOR		PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	     This subroutine copies the central portion of a padded
c	image array into a smaller array described by the "fin"
c	parameters in the include file.
c
c
c    SUBROUTINE USAGE
c	This subroutine is called with the following syntax:
c
c	call unpadimgfin(xyarrayin, xyarrayout)
c
c	where
c
c	xyarrayin	is a real array dimensioned from xyindexmin to
c			xyindexmax that is the padded image array.
c			(input)
c
c	xyarrayout	is a real array dimensioned from xyindexminfin
c			to xyindexmaxfin that is the shrunken image
c			extracted from the center of xyarrayin.
c			(output)
c
c***********************************************************************


      subroutine unpadimgfin(xyarrayin, xyarrayout)


      include 'pt6include.f'
      integer baseindex, baseindexfin, xindex, xindexfin, xoffset,
     +	 xyindex, xyindexfin, yindex, yindexfin, yoffset
      real xyarrayin(xyindexmin:xyindexmax),
     +	 xyarrayout(xyindexminfin:xyindexmaxfin)


c    Extract the center portion of the image.

	 xoffset = nint(xindexcent - xindexcentfin)
	 yoffset = nint(yindexcent - yindexcentfin)

	 do 1000 yindexfin = yindexminfin, yindexmaxfin

	    yindex = yindexfin + yoffset
	    baseindex = yindex * numx
	    baseindexfin = yindexfin * numxfin

	    do 1005 xindexfin = xindexminfin, xindexmaxfin

	       xindex = xindexfin + xoffset
	       xyindex = baseindex + xindex
	       xyindexfin = baseindexfin + xindexfin
	       xyarrayout(xyindexfin) = xyarrayin(xyindex)

 1005	    continue

 1000	 continue


      return
      end
