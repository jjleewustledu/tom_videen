c***********************************************************************
c
c    SUBROUTINE TITLE		pt6stripspace.f
c
c    WRITTEN BY			David G. Politte
c    DATE WRITTEN		November 6, 1987
c    WRITTEN FOR		PETT VI image reconstruction
c
c
c    PROGRAM INTENT
c	     This subroutine removes all trailing spaces from a
c	character string and replaces them with nulls.  The character
c	string may be of any length.
c
c
c    SUBROUTINE USAGE
c	This subroutine is called with the following syntax:
c
c	call stripspace(string)
c
c	where
c
c	string		is a character string of any length that will
c			have the trailing spaces stripped from it.  The
c			spaces (if any) will be replaced by nulls.
c			(input/output)
c
c***********************************************************************


      subroutine stripspace(string)


      integer i, intnull, intspace
      character*(*) string
      character*1 null, space


	 data intspace /32/
	 data intnull /0/
	 space = char(intspace)
	 null  = char(intnull)

	 do 1000 i = len(string), 1, -1
	    if (string(i:i) .eq. space)
     +	       then
		  string(i:i) = null
	       else
		  go to 1005
	    end if
 1000	 continue

 1005	 continue


      return
      end
