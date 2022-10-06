#!/usr/local/bin/perl

# Date: 5mar97
# Revised: 13jul98
# Author: Jane Dunford-Shore
# Purpose: Given a dir. with sequential *.ima's, rename them
#    <first>.ima - <last - first + 1>.ima, maintaining original sequence.
# Usage: ima_reorder <firstfile> <lastfile
# Example:
#        Files in dir. are 00001.ima - 00045.ima, with 00006.ima
#        and 00024.ima missing.
#
#        ima_reorder 2 44
#
#        New filenames will be:
#            00001.ima unchanged
#            00002.ima - 00044.ima => 00002.ima - 00042.ima
#            00045.ima unchanged

# Get command-line parameters
if ($#ARGV != 1) {
    printusage();
    exit(0);
}

$first = shift(@ARGV);
$last = shift(@ARGV);

# Open log file
open(LOGFILE, ">rename.log");
print LOGFILE `date`;
print LOGFILE `pwd`;
print LOGFILE "\n";

# Get current names of *.ima's
foreach $fileno ($first..$last) {
    if ($fileno < 10) {
        $prefix = "0000";
    }
    elsif ($fileno < 100) {
        $prefix = "000";
    }
    else {   # $fileno > 99
        $prefix = "00";
    }
    $filename = $prefix . $fileno . ".ima";
    if (-e $filename) {
        push(@ima_names, $filename)
	}
} 

# Rename files
$newfileno = $first;
foreach $oldname (@ima_names) {
    if ($newfileno < 10) {
        $prefix = "0000";
    }
    elsif ($newfileno < 100) {
        $prefix = "000";
    }
    else {   # $newfileno > 99
        $prefix = "00";
    }
    $newname = $prefix . $newfileno . ".ima"; 
    if ($oldname ne $newname) {   
        print STDOUT "mv $oldname $newname\n";
        print LOGFILE "mv $oldname $newname\n";
        `mv $oldname $newname`;
    }
    $newfileno++;
}
   
# =====
# Subroutines
# =====

# -----
# Subroutine: printusage
# Purpose: Print program usage info., as contained in format "USAGE".
# Parameters: none
# Returns: 0 if successful
# Requires: 1) definition of format "USAGE"
# -----
sub printusage {
    $~ = "USAGE";
    write STDOUT;
    return 0;
}

# =====
# Formats
# =====

format USAGE =
   Usage: ima_reorder <firstfile> <lastfile>
 Example: ima_reorder 2 47 
.
    
