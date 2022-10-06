# $Log: My_util.pm,v $
# Revision 1.1  1999/09/10  15:15:03  tom
# Initial revision
#
# Revision 1.1  1998/12/30  19:22:50  tom
# Initial revision
#
# ===
# Filename: My_util.pm
# Date: 5-28-98
# Author: Jane Dunford-Shore
# Purpose: Miscellaneous Perl utilities
# Subroutines:
#    bool2yn - Convert Boolean to yes/no
# ===

use strict;

package My_util;

# -----
# Subroutine: bool2yn(int BOOL_VAL)
# Purpose: Convert Boolean value - 0 or non-0 - to "YES" or "NO"
# Parameters: int BOOL_VAL = Boolean value of 0 or non-0
# Returns: "YES" or "NO"
# Requires: 1) definition of format "USAGE"
# -----
sub bool2yn {

    my ($integer) = @_;
    my $yesno;

    if ($integer > 0) {
        $yesno = "YES";
    }
    else {
        $yesno = "NO";
    }

    return $yesno;
}

1;
