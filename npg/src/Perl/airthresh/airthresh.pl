#!/usr/local/bin/perl

# Date:    13jun97
# Author:  JDS
# Purpose: Align two images with AIR using different thresholds, and create
#          data files that can be used to graph values.
# Usage: airthresh <standard> <reslice> <start_thresh> <end_thresh> <airdif_mask> [<stdmax>] [<reslmax>]
# Files created:
#    1. airthresh.log: results report
#    2. cost.dat: data file of threshold (as %max) and cost function
#    3. vector.dat: data file of threshold (as %max) and dif. vector distance

# Get command-line arguments
if ($#ARGV < 4) {
    &printusage;
    exit(0);
}

$stdfile = shift(@ARGV);
$reslfile = shift(@ARGV);
$start = shift(@ARGV);
$end = shift(@ARGV);
$mask = shift(@ARGV);

while ($ARGV[0] =~ /^-/) {
    $_ = shift;
    if (/^-smax\s*(.*)/) {
	$stdmax = $1;
    }
    if (/^-rmax\s*(.*)/) {    
	$reslmax = $1;
    }
}

# Open necessary files:
# 1. results report
# 2. data file of threshold (as %max) and cost function
# 3. data file of threshold (as %max) and dif. vector distance
open(LOG, ">airthresh.log");
open(DATA1, ">cost.dat");
open(DATA2, ">vector.dat");

# Get maximum pixel values for images
if (!defined($stdmax)) {
    @out = `scanheader $stdfile`;
    foreach $line (@out) {
	$line =~ /global maximum=(\d+)/ && ($stdmax = $1);
    }
    if ($stdmax) { 
	"$stdfile pixel max = $stdmax\n";
    }
    else {
	print STDOUT "Can't get pixel maximum for standard file:\n";
	print STDOUT @out;
	exit(0);
    }
}

@out = ();
if (!defined($reslmax)) {
    @out = `scanheader $reslfile`;
    foreach $line (@out) {
	$line =~ /global maximum=(\d+)/ && ($reslmax = $1);
    }
    if ($reslmax) { 
	"$reslfile pixel max = $reslmax\n";
    }
    else {
        print STDOUT "Can't get pixel maximum for reslice file:\n";
        print STDOUT @out;
        exit(0);
    }
}

#print $stdfile, $reslfile, $start, $end, $stdmax, $reslmax, "\n";
#exit(0);

# Create AIR command string
$airfile = "tmp.air";
$aircmd_beg = "/usr/local/pkg/AIR3.0/alignlinear $stdfile $reslfile $airfile -m 6 -b1 0.0 0.0 0.0 -b2 0.0 0.0 0.0";
$aircmd_mid = "-t1 <t1> -t2 <t2>";
$aircmd_end = "-p1 0 -p2 256 -x 1 -s 81 1 3 -r 25 -h 5 -c .00001 -v";

# Print info. to log file "airthresh.log"
select(LOG);
@date = `date`;
print LOG @date, "\n";
print LOG "Standard file: $stdfile\n";
print LOG "Pixel max: $stdmax\n\n";
print LOG "Reslice file: $reslfile\n";
print LOG "Pixel max: $reslmax\n\n";
print LOG "AIR command: $aircmd_beg $aircmd_mid $aircmd_end\n\n";
$~ = LOGHDR;
write LOG;

# Create identity matrix
`/usr/local/pkg/AIR3.0/alignlinear $stdfile $stdfile ident.air -m 6`;

# Perform alignment at different thresholds
foreach $percent ($start..$end) {

    $fraction = $percent/100;
    $t1 = $fraction * $stdmax;
    $t2 = $fraction * $reslmax;

    print STDOUT "-----\nAligning with threshold = $fraction x maximum\n";

    @out = `$aircmd_beg -t1 $t1 -t2 $t2 $aircmd_end`;  # alignment
    $costfn = &getcostfn(\@out);

    # Get distance vector
    @out = `airdif tmp.air ident.air $mask`;
    $vectdist = &getvectdist(\@out);

    # Print results to STDOUT, log file, and to data files
    $~ = LOGRECORD;
    write LOG;
    print DATA1 "$fraction   $costfn\n";
    print DATA2 "$fraction   $vectdist\n";
    print STDOUT "   Cost function: $costfn     Vector distance: $vectdist\n";
} 
print STDOUT "-----\n";

# Clean up
`rm tmp.air`;
`rm ident.air`;

# -----------
# Subroutines
# -----------

sub printusage {
    $~ = "USAGE";
    write STDOUT;
}

sub getcostfn {
    local($ptr_ar) = pop(@_);
    local($costfn);
    foreach $line (@$ptr_ar) {
        $line =~ /cost function=(\d+\.\d+)/ && ($costfn = $1);
    }
    return $costfn;
}

sub getvectdist {
    local($ptr_ar) = pop(@_);
    local($vectdist);
    foreach $line (@$ptr_ar) {
        $line =~ /Vector dist = (\d+\.\d+)/ && ($vectdist = $1);
    }
    return $vectdist;
}


# -------
# Formats
# -------

format USAGE =
  Usage: airthresh <standard> <reslice> <start_thresh> <end_thresh> <airdif_mask>
         [-smax <stdmax>] [-rmax <reslmax>]
Example: airthresh ap5261tr1_g3.img ap5261ho1_g3.img 40 50 -rmax 255
Required files: mask for airdif - e.g., p5261ho1_g3.v
                identity matrix - e.g., ident.air
.

format LOGHDR =
=============================================================
Threshold                             Cost         Vector
as fraction       t1         t2       function     distance
=============================================================
.

format LOGRECORD =
   @#.##     @#####.##  @#####.##   @##.#######   @###.######
$percent/100  $t1        $t2        $costfn       $vectdist
.

