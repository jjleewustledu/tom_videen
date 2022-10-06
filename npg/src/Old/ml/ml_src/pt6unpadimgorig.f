c***********************************************************************
c
c    SUBROUTINE TITLE		pt6unpadimgorig.f
c
c    WRITTEN BY			David G. Politte
c    DATE WRITTEN		October 1, 1989
c    WRITTEN FOR		PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	     This subroutine copies the central portion of a padded
c	image array into a smaller array described by the "orig"
c	parameters in the include file.
c
c
c    SUBROUTINE USAGE
c	This subroutine is called with the following syntax:
c
c	call unpadimgorig(xyarrayin, xyarrayout)
c
c	where
c
c	xyarrayin	is a real array dimensioned from xyindexmin to
c			xyindexmax that is the padded image array.
c			(input)
c
c	xyarrayout	is a real array dimensioned from xyindexminorig
c			to xyindexmaxorig that is the shrunken image
c			extracted from the center of xyarrayin.
c			(output)
c
c***********************************************************************


      subroutine unpadimgorig(xyarrayin, xyarrayout)


      include 'pt6include.f'
      integer baseindex, baseindexorig, xindex, xindexorig, xoffset,
     +	 xyindex, xyindexorig, yindex, yindexorig, yoffset
      real xyarrayin(xyindexmin:xyindexmax),
     +	 xyarrayout(xyindexminorig:xyindexmaxorig)


c    Extract the center portion of the image.

	 xoffset = nint(xindexcent - xindexcentorig)
	 yoffset = nint(yindexcent - yindexcentorig)

	 do 1000 yindexorig = yindexminorig, yindexmaxorig

	    yindex = yindexorig + yoffset
	    baseindex = yindex * numx
	    baseindexorig = yindexorig * numxorig

	    do 1005 xindexorig = xindexminorig, xindexmaxorig

	       xindex = xindexorig + xoffset
	       xyindex = baseindex + xindex
	       xyindexorig = baseindexorig + xindexorig
	       xyarrayout(xyindexorig) = xyarrayin(xyindex)

 1005	    continue

 1000	 continue


      return
      end
