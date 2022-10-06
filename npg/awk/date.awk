#		Module:	date.awk
#		Date:		12-May-92
#		Use:		Change DATE function to INTDATE function.
#  					For porting OS/32 FORTRAN to Sun FORTRAN

/CALL DATE\(/ { 
	gsub(/CALL DATE\(/,"CALL INTDATE(");
}

{ print $0; }
