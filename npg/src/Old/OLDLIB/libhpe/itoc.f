
*******************************************************************************
*
*  ITOC FUNCTION: itoc.f 
*
*  ITOC (I,K) returns the CHARACTER*12 value, left justified, which is the
*  ASCII representation of the numeric value contained in the integer I.
*  The integer argument K is an output argument into which is stored the
*  number of nonblank characters returned by ITOC.  If I is negative, a minus
*  sign precedes the characters representing the integer I.
*
*  M. Todd Gamble
*  11/20/89
*******************************************************************************


      CHARACTER*12 FUNCTION ITOC(I,K)     ! integer to character string 

      IMPLICIT NONE

      INTEGER*4     K                     ! number of characters returned 
      INTEGER*4     I,J                   ! integer to convert
      CHARACTER*13  T1                    ! temporary buffers
      CHARACTER*12  T2

      WRITE(T1,*) I
      IF (T1(1:1) .EQ. ' ') THEN          ! is left justification necessary?
	 T2 = T1(2:13)                    ! remove leading space
      ELSE
	 T2 = T1(1:12)
      ENDIF
      DO 10 J = 2,LEN(T2)                 ! find number of characters returned
	 IF (T2(J:J) .EQ. ' ') THEN       ! look for space at end of number 
	    K = J - 1
	    GOTO 20 
         ENDIF
10    CONTINUE
20    ITOC = T2                           ! return the value
      RETURN
      END
