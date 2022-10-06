# $Log: Image_util.pm,v $
# Revision 1.1  1999/09/10  15:15:03  tom
# Initial revision
#
# Revision 1.1  1998/12/30  19:22:50  tom
# Initial revision
#
# ===
# Name: Image_util.pm
# Date: 5-28-98
# Author: Jane Dunford-Shore
# Purpose: Image utilities
# ===

use strict;

package Image_util;

# -----
# Subroutine: reverse_rows(int $ROWS, int $COLS, int $FILESIZE, str $INFILENAME, r_fp $INFILE, r_int $PIXMAX, r_int $PIXMIN, int $debug) 
# Purpose: Rotate image 180 deg. on x-axis so that anterior is top
# Parameters: $ROWS 
#             $COLS
#             $FILESIZE
#             $infile
#             $r_INFILE
#             $r_pix_min
#             $r_pix_max
#             $debug            
# Returns:    
# Requires: 
# -----
sub reverse_rows {

    my ($ROWS, $COLS, $FILESIZE, $infile, $INFILE, $pix_min, $pix_max, $debug) = @_;

    my $dumpfile;
    my @orig_pix_array;
    my @new_pix_array;
    my $pix_no;
    my $pix_val;
    my $buffer;
    my $ct;
    my $row;
    my $col;
    my $new_pix_buf;

    # File for holding pixel data in original order
    $dumpfile = "pixdump.tmp";
    open(DUMPFILE, "> $dumpfile");
    binmode(DUMPFILE);

    # To avoid errors on forking ('system' call below), set '$|' to non-zero 
    # value so that no buffer is defined for files, and output is written out 
    # right away
    select(DUMPFILE);
    $| = 1;                                # non-zero value = no buffer defined
    select(STDOUT);                        # Reselect STDOUT, normal default

    # Use ERL's facility "dcm_dump_element" to dump pixel data
    # (element = 7fe0 0010) to temporary file
    system "dcm_dump_element 7fe0 0010 $infile $dumpfile";

    # Open pixel data file, and read into buffer
    open(BINFILE, "$dumpfile");
    binmode(BINFILE);
    read(BINFILE, $buffer, $FILESIZE, 0);

    # "Unpack" buffer into array of 16-bit integers
    @orig_pix_array = unpack("s262144", $buffer);     # 'S' = short integer
    $debug && print STDERR "---------------------\n";
    $debug && print STDERR "\$#orig_pix_array = $#orig_pix_array\n";

    # Reorder rows: last=first, etc.
    $ct = 0;
    for($row = $ROWS - 1; $row >= 0; $row--) {
        for($col = 0; $col < $COLS; $col++) {
            $pix_no = ($row * $COLS) + $col;
            $pix_val = $orig_pix_array[$pix_no];
            $new_pix_array[$ct++] = $orig_pix_array[$pix_no];
            ($pix_val > $$pix_max) && ($$pix_max = $pix_val);
            ($pix_val < $$pix_min) && ($$pix_min = $pix_val);
        }
    }
 
    # "Pack" array as buffer of 262144 16-bit integers
    $debug && print STDERR "\$#new_pix_array = $#new_pix_array\n";
    $debug && print STDERR "---------------------\n";
    $new_pix_buf = pack("s262144", @new_pix_array);

    # Remove temporary file
    `rm $dumpfile`;

    return \$new_pix_buf; 

}

# =====
# Subroutine: dcmfchar2num(char $fstring)
# Purpose: Convert Dicom float value represented as character string
#          to float value as numeric - e.g., -27.000000E+01 => -27
# Example: dcmfchar2float('-27.000000E+01')
# Returns: numeric representation of value passed
# =====
sub dcmfchar2num {
    my($fstring) = @_;
    my($base, $exp, $float);
    if ($fstring =~ /^([-+]*\d+\.\d+)E([-+]\d+)/) {
                # e.g., -27.000000E+01
        $base = $1;
        $exp = $2;
        $float = $base * (10**exp);
        return $float;
    }
    else {
        return $fstring;
#        print STDERR "Image_util.pm::dcmfchar2float(): could not convert $fstring\n";
    }
}

# =====
# Subroutine: rm_lead_char(char $char, char $string)
# =====
sub rm_lead_char {
    my($char, $string) = @_;
    $string =~ s/^[$char]*(.*)/$1/;
    return $string;
}

# =====
# Subroutine: rm_trail_char(char $char, char $string)
# =====
sub rm_trail_char {
    my($char, $string) = @_;
    $string =~ s/(.*)[$char]*$/$1/;
    return $string;
}

# $filename = ( ($fileno < 10) ? "0000" : "000" ) . $fileno . ".dcm";
sub fileno2name {
    my($fileno, $file_ext) = @_;
    my $filename;
    if ($fileno < 10) {
        $file_prefix = "0000"
    } else if fileno < 100) {
        $file_prefix = "000"
    } else {
        $file_prefix = "00"
    }
    $filename = $file_prefix . $fileno . ".dcm";
    return $filename;
}

sub monum2char {
    my($monum) = @_;
    my(@months, $mochar);
    @months = ('JAN','FEB','MAR','APR','MAY','JUN','JUL','AUG','SEP','OCT','NOV','DEC');
    $mochar = $months[$monum-1];
    return $mochar;
}

1;
