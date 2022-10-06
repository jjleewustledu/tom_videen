awk 'BEGIN { FS=":";
	printf ("s\nn\n")
	ftime0 = 0
	len0   = 0
	fact0  = 0
	frm0   = 0
	ftime1 = 0
	len1   = 0
	fact1  = 0
	frm1   = 0
}
# ---------------------------------------------------------------
# Date:   01-Dec-2003
# Intent: Update frame duration and adjust matrix scale factor
# 	following fixsingles and reconstruction of of ECAT Matrix 7 sinograms.
#	Works most, but not all, of the time...
# 
# Get Matrix Number, Scale Factor, Frame Duration, and Start Time
# ------------------------------------------------------
$1 ~ /^Image subheader/ {
	matrix = substr($1,28)
	split(matrix,val,",")
	frm0 = frm1
	frm1 = val[1]
}
$1 ~ /^Scale Factor/ {
	fact0 = fact1
	fact1 = $2
}
$1 ~ /^Frame Duration/ {
	len0 = len1
	len1 = $2
}
$1 ~ /^Frame Start Time/ {
	ftime0 = ftime1
	ftime1 = $2

# For all frames following first one:
#	if it is a 2-sec frame (less than 3000 msec)
#	then compare frame duration to the difference in frame start times
#	and then correct the duration if they differ.
#	and the scale factor if the true length is less than 3200 msec (?)
#
# Current  Frame: frm1, ftime1, len1, fact1
# Previous Frame: frm0, ftime0, len0, fact0
# Corrections to Previous Frame:
#	Increase frame_duration (true_len) to: ftime1-ftime0 
#	Reduce scale_factor (fact0) by: len0/true_len
# ----------------------------------------------------------------------
	dif = frm1 - frm0
	if (dif < 2) {
		if (ftime0 > 0) {
			if (len0 < 3000) {
				true_len = ftime1 - ftime0
				new_fact = (len0 / true_len) * fact0
				if (true_len != len0) {
					printf ("m\n%s,1,1,0,0\ne\nframe_duration\n%s\n", frm0, true_len)
					if (true_len < 3200)
						printf ("scale_factor\n%g\n", new_fact)
					printf ("d\n")
				}
			}
		}
	}
}
END {
	printf ("d\n")
	printf ("q\n")
}' $1

