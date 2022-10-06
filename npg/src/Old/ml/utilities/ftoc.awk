#
# ftoc.awk
# 1991 July 24, Gerald C. Johns
# awk code to join parameter continuation lines
# in a FORTRAN include file.
BEGIN {
# suck the entire file into the array ar[]
limit = 1
while ( getline line > 0 ) {
    ar[limit++] = line
    }
# now compare pairs of lines looking for a parameter line
# followed by a single continuation line.
indx = 1
while ( indx < limit ) {
# this code actually refers to an element of ar[] that is
# off the end, but awk is very tolerant of such slop.
    nxt = indx + 1
    if ( ar[indx] ~ /^         parameter/ && ar[nxt] ~ /^     \+/ ) {
        sub ( /^     \+/, "", ar[nxt] )
        printf("%s %s\n",ar[indx], ar[nxt] )
        indx += 2
        }
    else
        printf("%s\n",ar[indx++])
    }
} # end of begin
