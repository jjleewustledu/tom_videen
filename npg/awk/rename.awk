#		Module:	rename.awk
#		Date:		12-May-92
#		Use:		Rename files:  FILENAME.FTN --> filename.f
#  					For porting OS/32 FORTRAN to Sun FORTRAN
 
function strupper(s) {
	upcase["a"] = "A";
	upcase["b"] = "B";
	upcase["c"] = "C";
	upcase["d"] = "D";
	upcase["e"] = "E";
	upcase["f"] = "F";
	upcase["g"] = "G";
	upcase["h"] = "H";
	upcase["i"] = "I";
	upcase["j"] = "J";
	upcase["k"] = "K";
	upcase["l"] = "L";
	upcase["m"] = "M";
	upcase["n"] = "N";
	upcase["o"] = "O";
	upcase["p"] = "P";
	upcase["q"] = "Q";
	upcase["r"] = "R";
	upcase["s"] = "S";
	upcase["t"] = "T";
	upcase["u"] = "U";
	upcase["v"] = "V";
	upcase["w"] = "W";
	upcase["x"] = "X";
	upcase["y"] = "Y";
	upcase["z"] = "Z";

	for (i in upcase) gsub(i,upcase[i],s);
	return s;
}

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
	gsub("ftn","f",temp); 
	command = sprintf("mv %s %s",$0,temp);
	system(command); 
}
