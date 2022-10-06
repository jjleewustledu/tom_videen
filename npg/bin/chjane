#!/usr/local/bin/perl

# -----
# Author:  Jane Dunford-Shore
# Date:    11oct98
# Usage:   chjane -r <rootdir> -g <new group> -p <new permissions>
# Purpose: Find all files/dirs owned by user "jane" in tree <rootdir>,
#          and change group to <group> and permissions to <permissions>.
# Note:    This script runs as user "jane" and group "videen",
#          as user and group ID bits were set with "chmod +s chjane".
#          If file is edited, these bits must be reset.
# -----

$~ = "USAGE";  # format for currently-selected filehandle (STDOUT)

if ($#ARGV < 0) {
    write;     # Print format ("USAGE") associated with 
               # currently-selected filehandle (STDOUT) 
    exit(0);
}
elsif ($#ARGV == 5) {
    while ($#ARGV >= 0) {
         $_ = shift(@ARGV);
         if (/-r/) {
             $rootdir = shift(@ARGV);
         }
         elsif (/-g/) {
             $group = shift(@ARGV);
         }
         elsif (/-p/) {
             $permissions = shift(@ARGV);
         }
    }
}
else {
    write;
    exit(0);
}

# FOR TESTING ONLY
#print "rootdir: $rootdir\n";
#print "group: $group\n";
#print "permissions: $permissions\n";
#exit(0);
 
# Change to specified directory

chdir $rootdir || die "Can't cd to $rootdir\n";

# Change group and permissions to specified values

`chgrp -R $group *`;
`chmod -R $permissions *`;

# -------
# Formats
# -------

format USAGE =
Usage:    chjane -r <rootdir> -g <new group> -p <new permissions>
Examples: chjane -r /usr/local/npg -g videen -p 775
          chjane -r . -g videen -p 770
Purpose:  Set all files/dirs owned by user "jane" to group <new group>
          and permissions <new permissions>. Do this recursively,
          beginning in directory <rootdir>.
Note:     This script runs as user "jane" and group "videen", as
          user/group ID bits were set by jane with "chmod +s chjane".
.
