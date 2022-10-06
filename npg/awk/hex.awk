#		Module:	hex.awk
#		Date:		12-May-92
#		Use:		Change hexidecimal numbers given by /Y' and /X' to /Z'
#  					For porting OS/32 FORTRAN to Sun FORTRAN

/\/X'|\/Y'/ { 
	gsub(/\/X'/,"/Z'");
	gsub(/\/Y'/,"/Z'");
}

{ print $0; }
