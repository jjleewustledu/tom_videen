c***********************************************************************
c
c    SUBROUTINE TITLE		pt6paddata.f
c
c    WRITTEN BY			David G. Politte
c    DATE WRITTEN		October 1, 1989
c    WRITTEN FOR		PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	     This subroutine copies a distance-angle (d,a) data array
c	into a larger array and pads with zeros.
c
c
c    SUBROUTINE USAGE
c	This subroutine is called with the following syntax:
c
c	call paddata(daarrayin, daarrayout)
c
c	where
c
c	daarrayin	is a real array dimensioned from daindexminorig
c			to daindexmaxorig that is the distance-angle
c			(d,a) array to be padded.  (input)
c
c	daarrayout	is a real array dimensioned from daindexmin to
c			daindexmax that is the padded (d,a) array.
c			(output)
c
c***********************************************************************


      subroutine paddata(daarrayin, daarrayout)


      include 'pt6include.f'
      integer angindex, baseindex, baseindexorig, daindex, daindexorig,
     +	 dindex, dindexorig, doffset
      real daarrayin(daindexminorig:daindexmaxorig),
     +	 daarrayout(daindexmin:daindexmax)


c    Clear the output array.

	 do 1000 daindex = daindexmin, daindexmax
	    daarrayout(daindex) = 0.0
 1000	 continue

c    Place the padded input array into the output array.

	 doffset = nint(dindexcent - dindexcentorig)

	 do 2000 angindex = angindexmin, angindexmax

	    baseindex = angindex * numd
	    baseindexorig = angindex * numdorig

	    do 2005 dindexorig = dindexminorig, dindexmaxorig

	       dindex = dindexorig + doffset
	       daindex = baseindex + dindex
	       daindexorig = baseindexorig + dindexorig
	       daarrayout(daindex) = daarrayin(daindexorig)

 2005	    continue

 2000	 continue


      return
      end
