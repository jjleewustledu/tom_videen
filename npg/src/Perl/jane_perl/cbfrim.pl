#!/usr/local/bin/perl

# Script:  cbfrim.pl (previous name: regcbf.pl)
# Date:    23-Oct-97
# Author:  JDS
# Purpose: Calculate corrected left and right CBF for brain tissue 
#          rim consisting of difference between 2 brain images, one
#          wider than the other. Correction requires that data for
#          two images are present: ho1.v and tis5.v. Currently, we 
#          are using "peel3" (3 pixels in from edge) and "peel8" 
#          (8 pixels in) as the two images, but this script would work 
#          for any width. Note, though, that it assumes that first file
#          on command line is the one with data for the wider
#          of the two images ("peel3" in our current case).
# Syntax:  cbfrim <peel3_file> <peel8_file> <hdr_file> <output_file>
# Example: cbfrim p5007.3 p5007.8 p5007ho1.hdr outfile
# Assumptions:
#     "peel3" and "peel8" files are output from spideram, global averaging.
#     Looks only at global statistics in peel 3 and peel8 files.
#     Assumes that each file contains data for ho1 and tis5 images,
#     in that order.
# Modifications:
#     28-Oct-97: Modified report to distinguish from hemispheric CBF
#                report. Added file checking.

# Get values for reports
$args = join(' ', @ARGV);
$date = `date`;

# Get command-line args

if ($#ARGV < 2) {
    &printusage;
    exit(0);
}
$peel3_file = shift(@ARGV);
$peel8_file = shift(@ARGV);
$hdr_file = shift(@ARGV);
($hdr_file =~ /\.hdr$/) || die "Header file should have \".hdr\" extension: $hdr_file\n";

$out_file = shift(@ARGV);

# Make sure necessary files are present
(-e $peel3_file) || (push(@notfound, $peel3_file));
(-e $peel8_file) || (push(@notfound, $peel8_file));
(-e $hdr_file) || (push(@notfound, $hdr_file));
if (@notfound) {
    $missingfiles = join(' ', @notfound);
    die "Could not find files: $missingfiles\n";
}

# Get PET ID - for reports
$hdr_file =~ /(p\d{4,4})/;
$pet_id = $1;
#print "PET ID: $pet_id\n";

# -----
# Get values from data files
# -----

@peels = (3,8);
foreach $peel (sort(@peels)) {

    $in_file = "peel" . $peel . "_file";
    #print "$in_file\n";
    open(INFILE, "$$in_file") || die "Could not open file: $$in_file\n";
    $file = 0;

IMAGEDATA:
while (<INFILE>) {
    if (/Image File = (.+\.v)\s+/) {
        $image_file = $1;

        if ($file == 0) {    # ho1 file
            ($image_file =~ /ho1/) || die "First image in file should be ho1.\n";
            $file++;
       
            while (<INFILE>) {
                if (/Global Statistics for All of the Above Slices:/) {
                    while(<INFILE>) {
                        if (/Number of Pixels.*\.+\s+(\d+\.*\d*)\s+(\d+\.*\d*)\s+(\d+\.*\d*)/) {
                            $pix_ho1_l{$peel} = $2;
                            $pix_ho1_r{$peel} = $3;
                        }
                        elsif (/Mean Value.*\.+\s+(\d+\.*\d*)\s+(\d+\.*\d*)\s+(\d+\.*\d*)/) {
                            $cts_mean_ho1_l{$peel} = $2;
                            $cts_mean_ho1_r{$peel} = $3;
                        }
                        elsif (/Standard Deviation.*\.+\s+(\d+\.*\d*)\s+(\d+\.*\d*)\s+(\d+\.*\d*)/) {
                            $sd_ho1_l{$peel} = $2;
                            $sd_ho1_r{$peel} = $3;
                            next IMAGEDATA;
                        }
                    }
                }
            }
        }
    
        else  {           # tis5 file
            ($image_file =~ /tis5/) || die "First image in file should be ho1.\n";
       
            while (<INFILE>) {
                if (/Global Statistics for All of the Above Slices:/) {
                    while(<INFILE>) {
                        if (/Number of Pixels.*\.+\s+(\d+\.*\d*)\s+(\d+\.*\d*)\s+(\d+\.*\d*)/) {
                            $pix_tis_l{$peel} = $2;
                            $pix_tis_r{$peel} = $3;
                        }
                        elsif (/Mean Value.*\.+\s+(\d+\.*\d*)\s+(\d+\.*\d*)\s+(\d+\.*\d*)/) {
                            $cts_mean_tis_l{$peel} = $2;
                            $cts_mean_tis_r{$peel} = $3;
                        }
                        elsif (/Standard Deviation.*\.+\s+(\d+\.*\d*)\s+(\d+\.*\d*)\s+(\d+\.*\d*)/) {
                            $sd_tis_l{$peel} = $2;
                            $sd_tis_r{$peel} = $3;
                            last;
                        }
                    }
                }
            }
        }
    }
}

#print "pix_ho1_l$peel: $pix_ho1_l{$peel}\n";
#print "pix_ho1_r$peel: $pix_ho1_r{$peel}\n";
#print "cts_mean_ho1_l$peel: $cts_mean_ho1_l{$peel}\n";
#print "cts_mean_ho1_r$peel: $cts_mean_ho1_r{$peel}\n";
#print "sd_ho1_l$peel: $sd_ho1_l{$peel}\n";
#print "sd_ho1_r$peel: $sd_ho1_r{$peel}\n";

#print "pix_tis_l$peel: $pix_tis_l{$peel}\n";
#print "pix_tis_r$peel: $pix_tis_r{$peel}\n";
#print "cts_mean_tis_l$peel: $cts_mean_tis_l{$peel}\n";
#print "cts_mean_tis_r$peel: $cts_mean_tis_r{$peel}\n";
#print "sd_tis_l$peel: $sd_tis_l{$peel}\n";
#print "sd_tis_r$peel: $sd_tis_r{$peel}\n";

}

# -----
# Get constants from header file
# -----

@hdrinfo = `hdrinfo $hdr_file`;
@hdrinfo || die "Could not get header info.: $hdr_file\n";

#print @hdrinfo;
foreach $line (@hdrinfo) {
    if ($line =~ /A Coefficient \(Flow\) = (\d+.*)$/) {
        $A = $1;
    }
    elsif ($line =~ /B Coefficient \(Flow\) = (\d+.*)$/) {
        $B = $1;
    }
}
#print "A = $A\nB = $B\n";

# -----
# Convert constants from text to corresponding float value
# -----

$A =~ /(\d+\.\d+)E(-\d+)/;
$A_base = $1;
$A_exp = $2;
#print "$A_base $A_exp\n";

$B =~ /(\d+\.\d+)E(-\d+)/;
$B_base = $1;
$B_exp = $2;
#print "$B_base $B_exp\n";

$A = $A_base * 10**($A_exp);
$B = $B_base * 10**($B_exp);

#print "A = $A\nB = $B\n";

# -----
# Calculate CBF
# -----

### Calculate uncorr. CBF for left side - ho1

# Total cts for left side = (tot cts for peel3) - (tot cts for peel8)
$cts_tot_ho1_l{3} = ($cts_mean_ho1_l{3})*($pix_ho1_l{3});
$cts_tot_ho1_l{8} = ($cts_mean_ho1_l{8})*($pix_ho1_l{8});
$cts_tot_ho1_l = $cts_tot_ho1_l{3} - $cts_tot_ho1_l{8};

# Total pixels for left side = (tot pix for peel3) - (tot pix for peel8)
$pix_tot_ho1_l = $pix_ho1_l{3} - $pix_ho1_l{8};

# Mean counts / pixel for left side = tot cts for left / tot pix for left
$cts_mean_ho1_l = $cts_tot_ho1_l / $pix_tot_ho1_l;
$cbf_tot_ho1_l_uncorr = $A*(($cts_mean_ho1_l)**2) + $B*($cts_mean_ho1_l);

### Calculate tissue fraction for left side

# Total cts for left side = (tot cts for peel3) - (tot cts for peel8)
$cts_tot_tis_l{3} = ($cts_mean_tis_l{3})*($pix_tis_l{3});
$cts_tot_tis_l{8} = ($cts_mean_tis_l{8})*($pix_tis_l{8});
$cts_tot_tis_l = $cts_tot_tis_l{3} - $cts_tot_tis_l{8};

# Total pixels for left side = (tot pix for peel3) - (tot pix for peel8)
$pix_tot_tis_l = $pix_tis_l{3} - $pix_tis_l{8};

# Mean counts / pixel for left side = tot cts for left / tot pix for left
$cts_mean_tis_l = $cts_tot_tis_l / $pix_tot_tis_l;
$tis_fr_l = $cts_mean_tis_l / 1000;

### Calculate corr. CBF for left side

$cbf_tot_ho1_l_corr = $A*(($cts_mean_ho1_l/$tis_fr_l)**2) + $B*($cts_mean_ho1_l/$tis_fr_l);

# -----

### Calculate uncorr. CBF for right side - ho1

# Total cts for right side = (tot cts for peel3) - (tot cts for peel8)
$cts_tot_ho1_r{3} = ($cts_mean_ho1_r{3})*($pix_ho1_r{3});
$cts_tot_ho1_r{8} = ($cts_mean_ho1_r{8})*($pix_ho1_r{8});
$cts_tot_ho1_r = $cts_tot_ho1_r{3} - $cts_tot_ho1_r{8};

# Total pixels for right side = (tot pix for peel3) - (tot pix for peel8)
$pix_tot_ho1_r = $pix_ho1_r{3} - $pix_ho1_r{8};

# Mean counts / pixel for right side = tot cts for right / tot pix for right
$cts_mean_ho1_r = $cts_tot_ho1_r / $pix_tot_ho1_r;
$cbf_tot_ho1_r_uncorr = $A*(($cts_mean_ho1_r)**2) + $B*($cts_mean_ho1_r);

### Calculate tissue fraction for right side

# Total cts for right side = (tot cts for peel3) - (tot cts for peel8)
$cts_tot_tis_r{3} = ($cts_mean_tis_r{3})*($pix_tis_r{3});
$cts_tot_tis_r{8} = ($cts_mean_tis_r{8})*($pix_tis_r{8});
$cts_tot_tis_r = $cts_tot_tis_r{3} - $cts_tot_tis_r{8};

# Total pixels for right side = (tot pix for peel3) - (tot pix for peel8)
$pix_tot_tis_r = $pix_tis_r{3} - $pix_tis_r{8};

# Mean counts / pixel for right side = tot cts for right / tot pix for right
$cts_mean_tis_r = $cts_tot_tis_r / $pix_tot_tis_r;
$tis_fr_r = $cts_mean_tis_r / 1000;

### Calculate corr. CBF for right side

$cbf_tot_ho1_r_corr = $A*(($cts_mean_ho1_r/$tis_fr_r)**2) + $B*($cts_mean_ho1_r/$tis_fr_r);

# -----
# Print report to STDOUT and to text file
# -----

open(OUTFILE, "> $out_file");

select OUTFILE;
$~ = REPORT_LONG;
write OUTFILE;

select STDOUT;
$~ = REPORT_SHORT;
write STDOUT;

exit(0);

# =====
# Subroutines
# =====

sub printusage {
    $~ = USAGE;
    write STDOUT;
}

# =====
# Formats
# =====

format USAGE =
   Usage: cbfrim <peel3_file> <peel8_file> <hdr_file> <output_file>
 Example: cbfrim p5007.3 p5007.8 p5007ho1.hdr p5007_3-8.out
    Note: Looks only at global statistics in peel3 and peel8
          files. Assumes that each file contains data for both
          ho1 and tis5 images, in that order.
.

format REPORT_SHORT =
=================
Summary for @<<<<<
            $pet_id
=================

CBF left peel3-8   @>>>>>>>>>
                   $cbf_tot_ho1_l_corr
CBF right peel3-8  @>>>>>>>>>
                   $cbf_tot_ho1_r_corr 

.

format REPORT_LONG =
Date: @<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      $date
Script: cbfrim @<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
               $args

peel3_file: @<<<<<<<<<<<<<<<
            $peel3_file
peel8_file: @<<<<<<<<<<<<<<<
            $peel8_file
header file: @<<<<<<<<<<<<<<
             $hdr_file
output file (this file): @<<<<<<<<<<<<<<<<<<
                         $out_file

=========
Left side
=========

   -----------------------------------------------------------------------
                     # pixels    cts/pixel       SD         total cts

   peel3_ho1      @#######.##  @#######.##   @#######.##   @#######.##
                  $pix_ho1_l{3}  $cts_mean_ho1_l{3} $sd_ho1_l{3} $cts_tot_ho1_l{3}
   peel8_ho1      @#######.##  @#######.##   @#######.##   @#######.##
                  $pix_ho1_l{8}  $cts_mean_ho1_l{8} $sd_ho1_l{8} $cts_tot_ho1_l{8}
   peel3-8_ho1    @#######.##                              @#######.##
                  $pix_tot_ho1_l                           $cts_tot_ho1_l

   mean_cts_ho1 (left)
                  = total cts    /   total pix =
                    @#######.##  / @#######.## = @#######.##
                    $cts_tot_ho1_l     $pix_tot_ho1_l    $cts_mean_ho1_l

   -----------------------------------------------------------------------
                     # pixels    cts/pixel       SD         total cts

   peel3_tis      @#######.##  @#######.##   @#######.##   @#######.##
                  $pix_tis_l{3}  $cts_mean_tis_l{3} $sd_tis_l{3} $cts_tot_tis_l{3}
   peel8_tis      @#######.##  @#######.##   @#######.##   @#######.##
                  $pix_tis_l{8}  $cts_mean_tis_l{8} $sd_tis_l{8} $cts_tot_tis_l{8}
   peel3-8_tis    @#######.##                              @#######.##
                  $pix_tot_tis_l                           $cts_tot_tis_l

   mean_cts_tis (left)
                  = total cts    /   total pix =
                    @#######.##  / @#######.## = @#######.##
                    $cts_tot_tis_l $pix_tot_tis_l $cts_mean_tis_l
   -----------------------------------------------------------------------

   A = @>>>>>>>>>>>>
       $A
   B = @>>>>>>>>>>>>
       $B

   uncorr_mean_cts = @>>>>>>>>>>
                     $cts_mean_ho1_l

   CBF_l_uncorr = A x (mean_cts)(mean_cts) + B x (mean_cts) 
                = @>>>>>>>>>
                  $cbf_tot_ho1_l_uncorr
   
   corr_mean_cts = mean_cts_ho1 / tissue_fraction
                 = mean_cts_ho1 / (mean_cts_tis/1000)
                 = @>>>>>>>>    / @>>>>>>>>
                   $cts_mean_ho1_l $tis_fr_l
                 = @>>>>>>>>>>
                   $cts_mean_ho1_l/$tis_fr_l

   CBF_l_corr = A x (corr_mean_cts)(corr_mean_cts) + B x (corr_mean_cts) 
              = @>>>>>>>>>
                $cbf_tot_ho1_l_corr
      

==========
Right side
==========

   -----------------------------------------------------------------------
                     # pixels    cts/pixel       SD         total cts

   peel3_ho1      @#######.##  @#######.##   @#######.##   @#######.##
                  $pix_ho1_r{3}  $cts_mean_ho1_r{3} $sd_ho1_r{3} $cts_tot_ho1_r{3}
   peel8_ho1      @#######.##  @#######.##   @#######.##   @#######.##
                  $pix_ho1_r{8}  $cts_mean_ho1_r{8} $sd_ho1_r{8} $cts_tot_ho1_r{8}
   peel3-8_ho1    @#######.##                              @#######.##
                  $pix_tot_ho1_r                           $cts_tot_ho1_r

   mean_cts_ho1 (right)
                  = total cts    /   total pix =
                    @#######.##  / @#######.## = @#######.##
                    $cts_tot_ho1_r $pix_tot_ho1_r $cts_mean_ho1_r

   -----------------------------------------------------------------------
                     # pixels    cts/pixel       SD         total cts

   peel3_tis      @#######.##  @#######.##   @#######.##   @#######.##
                  $pix_tis_r{3}  $cts_mean_tis_r{3} $sd_tis_r{3} $cts_tot_tis_r{3}
   peel8_tis      @#######.##  @#######.##   @#######.##   @#######.##
                  $pix_tis_r{8}  $cts_mean_tis_r{8} $sd_tis_r{8} $cts_tot_tis_r{8}
   peel3-8_tis    @#######.##                              @#######.##
                  $pix_tot_tis_r                           $cts_tot_tis_r

   mean_cts_tis (right)
                  = total cts    /   total pix =
                    @#######.##  / @#######.## = @#######.##
                    $cts_tot_tis_r $pix_tot_tis_r $cts_mean_tis_r
   -----------------------------------------------------------------------

   A = @>>>>>>>>>>>>
       $A
   B = @>>>>>>>>>>>>
       $B

   uncorr_mean_cts = @>>>>>>>>>>
                     $cts_mean_ho1_r

   CBF_r_uncorr = A x (mean_cts)(mean_cts) + B x (mean_cts) 
                = @>>>>>>>>>
                  $cbf_tot_ho1_r_uncorr
   
   corr_mean_cts = mean_cts_ho1 / tissue_fraction
                 = mean_cts_ho1 / (mean_cts_tis/1000)
                 = @>>>>>>>>    / @>>>>>>>>
                   $cts_mean_ho1_r $tis_fr_r
                 = @>>>>>>>>>>
                   $cts_mean_ho1_r/$tis_fr_r

   CBF_r_corr = A x (corr_mean_cts)(corr_mean_cts) + B x (corr_mean_cts) 
              = @>>>>>>>>>
                $cbf_tot_ho1_r_corr

=================
Summary for @<<<<<
            $pet_id
=================

CBF left peel3-8   @>>>>>>>>>
                   $cbf_tot_ho1_l_corr
CBF right peel3-8  @>>>>>>>>>
                   $cbf_tot_ho1_r_corr 

.




