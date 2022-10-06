#		Module:	comment.awk
#		Date:		12-May-92
#		Use:		Change embedded comment flag from ';' to '!'
#  					For porting OS/32 FORTRAN to Sun FORTRAN

/;/ {
	if (substr($0,1,1) != "C") 
		gsub(/;/,"!");
}

{print $0}
