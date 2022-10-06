#!/usr/local/bin/perl
$wwwpath="/www/htdocs/labs/npg/";
$npgman1path="/usr/local/npg/man/man1/";
$npgman5path="/usr/local/npg/man/man5/";

unlink $wwwpath . "index1.html";
unlink $wwwpath . "index2.html";
chdir $npgman1path;

open INDEX1, "/bin/ls -1 |";
while (<INDEX1>){
   $oldfilename=$_;
   $test = "oldfilename " . $oldfilename . "\n";
   print $test;

   $newfilename=$_;
   $test = "newfilename " . $newfilename . "\n";
   print $test;
   
   chomp($newfilename);
   chop($newfilename);
   chop($newfilename);
   $test = "newfilename2 " . $newfilename . "\n";
   print $test;

   $htmlfilename= $newfilename . ".html";
   $test = "htmlfilename " . $htmlfilename . "\n";
   print $test;
   print $wwwpath.$htmlfilename."\n";
   print $npgman1path.$newfilename."\n";

   unlink $wwwpath.$htmlfilename;
   chdir $npgman1path;

   @args = ("/usr/local/bin/man2html", "-bare", "<".$npgmanpath1.$oldfilename, ">".$wwwpath.$htmlfilename);
   system(@args) == 0
      or die "system @args failed: $?";
   @args = ("cat", $wwwpath."includeheader.html", $wwwpath.$htmlfilename, $wwwpath."includefooter.html", ">", $wwwpath.$htmlfilename);
   system(@args) == 0
      or die "system @args failed: $?";
   @args = ("echo", "<a href=\"".$htmlfilename."\">".$htmlfilename."</a>", ">>" ,$wwwpath."index1.html");
   system(@args) == 0
      or die "system @args failed: $?";
   
}
