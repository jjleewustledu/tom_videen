#		Module:	int2.awk
#		Date:		12-May-92
#		Use:		Change INT2 to INT
#						For porting OS/32 FORTRAN to Sun FORTRAN

/INT2\(|INT2 \(/ {
	gsub(/INT2\(/,"INT(");
	gsub(/INT2 \(/,"INT(");
}

{print $0};
