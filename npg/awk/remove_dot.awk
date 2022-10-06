#		Module:	remove_dot.awk
#		Date:		28-Jul-92
#		Author:	Tom Videen
#		Use:		Remove a trailing dot from a filename
#  					For transferring OS/32 file to Suns

{ 
	loc = index($0,".")
	if (loc > 0 && loc == length($0)) {
		temp = substr($0,0,loc-1)
		command = sprintf("mv %s %s",$0,temp)
		system(command)
	}
}
