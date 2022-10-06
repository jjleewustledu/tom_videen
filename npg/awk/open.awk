#		Module:	open.awk
#		Date:		12-May-92
#		Use:		Change OPEN statement to eliminate:
#							TYPE='CONTIG,'
#							SHARE='SRO,'
#  					For porting OS/32 FORTRAN to Sun FORTRAN

/OPEN/ { 
	gsub(/TYPE='CONTIG',/,"");
	gsub(/SHARE='SRO',/,"");
	gsub(/SIZE=.,/,"");
	gsub(/SIZE=..,/,"");
	gsub(/SIZE=...,/,"");
	gsub(/SIZE=....,/,"");
	gsub(/SIZE=.....,/,"");
}

{ print $0; }
