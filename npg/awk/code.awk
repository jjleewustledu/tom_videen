#		Module:	code.awk
#		Date:		12-May-92
#		Use:		Change ENCODE and DECODE statements to internal WRITE and READ statements.
#  					For porting OS/32 FORTRAN to Sun FORTRAN

/ENCODE\(|DECODE\(|ENCODE \(|DECODE \(/ {
	gsub(/ENCODE\(/,"WRITE(");
	gsub(/ENCODE \(/,"WRITE(");
	gsub(/DECODE\(/,"READ(");
	gsub(/DECODE \(/,"READ(");
}

{print $0};
