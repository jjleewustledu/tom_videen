CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C ianyl should search for the first character OF a specified set 
C starting from the LEFT. inanyl should search for the first 
C character NOT of a specified set starting from the LEFT.
C
C ianyr and inanyr are equivalent, but start the search from 
C the RIGHT.
C
C	IANYL('ABCABC','AB')	= 1
C	INANYL('ABCABC','AB')	= 3
C	IANYR('ABCABC','AB')	= 5
C	INANYR('ABCABC','AB')	= 6
C	
C	Author:		Tom Yang
C	Date:		Sep. 17, 1991.
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

	INTEGER FUNCTION INANYL (S1, S2)

	CHARACTER*(*)	S1, S2
	INTEGER		I, J, POS, FOUND

	POS	= 0
	FOUND	= 0
	DO 200 I = 1, LEN (S1)
		DO 100 J = 1, LEN (S2)
			IF (S1 (I:I) .EQ. S2 (J:J)) THEN
				FOUND	= 1
			ENDIF
100		CONTINUE
		IF (FOUND .NE. 1 .AND. POS .EQ. 0) THEN
			POS	= I
		ELSE
			FOUND	= 0
		ENDIF
200	CONTINUE

300	INANYL	= POS
      
	RETURN
	END

