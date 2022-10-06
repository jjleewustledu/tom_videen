c***********************************************************************
c
c    SUBROUTINE TITLE		pt6padimg.f
c
c    WRITTEN BY			David G. Politte
c    DATE WRITTEN		October 2, 1989
c    WRITTEN FOR		PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	     This subroutine copies an image array into a larger image
c	array and pads with zeros.
c
c
c    SUBROUTINE USAGE
c	This subroutine is called with the following syntax:
c
c	call padimg(xyarrayin, xyarrayout)
c
c	where
c
c	xyarrayin	is a real array dimensioned from xyindexminorig
c			to xyindexmaxorig that is the image array to be
c			padded.  (input)
c
c	xyarrayout	is a real array dimensioned from xyindexmin to
c			xyindexmax that is the image array after
c			padding.  (output)
c
c***********************************************************************


      subroutine padimg(xyarrayin, xyarrayout)


      include 'pt6include.f'
      integer baseindex, baseindexorig, xindex, xindexorig, xoffset,
     +	 xyindex, xyindexorig, yindex, yindexorig, yoffset
      real xyarrayin(xyindexminorig:xyindexmaxorig),
     +	 xyarrayout(xyindexmin:xyindexmax)


c    Clear the output array.

	 do 1000 xyindex = xyindexmin, xyindexmax
	    xyarrayout(xyindex) = 0.0
 1000	 continue

c    Place the padded input array into the output array.

	 xoffset = nint(xindexcent - xindexcentorig)
	 yoffset = nint(yindexcent - yindexcentorig)

	 do 2000 yindexorig = yindexminorig, yindexmaxorig

	    yindex = yindexorig + yoffset
	    baseindex = yindex * numx
	    baseindexorig = yindexorig * numxorig

	    do 2005 xindexorig = xindexminorig, xindexmaxorig

	       xindex = xindexorig + xoffset
	       xyindex = baseindex + xindex
	       xyindexorig = baseindexorig + xindexorig
	       xyarrayout(xyindex) = xyarrayin(xyindexorig)

 2005	    continue

 2000	 continue


      return
      end
