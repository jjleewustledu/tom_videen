#		Module:	lowercase.awk
#		Date:		12-May-92
#		Use:		Convert filename to lowercase
 

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
	
{ 
	temp = strlower($0);
	command = sprintf("mv %s %s",$0,temp);
	system(command); 
}
