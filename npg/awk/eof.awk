#		Module:	eof.awk
#		Date:		12-May-92
#		Use:		Change EOF marker (in INCLUDE statements) from '/*' to 'C'
#  					For porting OS/32 FORTRAN to Sun FORTRAN

/^\/\*/ { $0 = "C"; }

{print $0;}
