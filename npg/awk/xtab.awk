#	Module:	xtab.awk
#	Date:	Apr 2002
#	Use:	Remove tabs from a file

/\t/ {
	gsub(/\t/,"")
};

{print $0}
