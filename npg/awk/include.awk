#		Module:	include.awk
#		Date:		12-May-92
#		Use:		Rewrite INCLUDE statements from modular input in F7 to single lines.
#  					For porting OS/32 FORTRAN to Sun FORTRAN

function strlower(s) {
	locase["A"] = "a";
	locase["B"] = "b";
	locase["C"] = "c";
	locase["D"] = "d";
	locase["E"] = "e";
	locase["F"] = "f";
	locase["G"] = "g";
	locase["H"] = "h";
	locase["I"] = "i";
	locase["J"] = "j";
	locase["K"] = "k";
	locase["L"] = "l";
	locase["M"] = "m";
	locase["N"] = "n";
	locase["O"] = "o";
	locase["P"] = "p";
	locase["Q"] = "q";
	locase["R"] = "r";
	locase["S"] = "s";
	locase["T"] = "t";
	locase["U"] = "u";
	locase["V"] = "v";
	locase["W"] = "w";
	locase["X"] = "x";
	locase["Y"] = "y";
	locase["Z"] = "z";

	for (i in locase) gsub(i,locase[i],s);
	return s;
}


function getname(s) {
        x = index(s,",");
	if (x==0) x = length(s);
	s = strlower(substr(s,1,x-1));
	return(s);
}


BEGIN {
	check	= 0;
	include	= 0;
	data	= 0;
	ln 	= 0;
	filename= "";
}

{ 
	ln = ln + 1;
	line[ln] = $0;
	if (index($0,"$INCLUDE")==1) {
		line[ln] = "C";
		check = 1; 
		include = ln;
		if (filename == "") 
			filename = getname($2); 
	} 
	if ((index($0,"      DATA")==1)&&(data==0)) {
		line[ln+1] = line[ln];
		line[ln] = "C";
		data = ln;
		ln = ln + 1;
	}
}

END {
	if (check>0 && data>0) 
		line[data] = sprintf("      INCLUDE '%s'",filename);
	else
		if (check>0)
			line[include] = sprintf("      INCLUDE '%s'",filename);
	for (i=1;i<=ln;i++) print line[i];
}
