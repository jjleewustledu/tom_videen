#		Module:	creturn.awk
#		Date:		12-May-92
#		Use:		Delete carriage return inserted into files by LEX.
#  					For porting OS/32 FORTRAN to Sun FORTRAN

/\r/ {
	gsub(/\r/,"")
};

{print $0}
