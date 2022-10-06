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
	cnt    = 0
}
# ---------------------------------------------------------------
# Date:   29-Dec-2003
# Intent: Update frame duration and adjust matrix scale factor
# 	following fixsingles and reconstruction of of ECAT Matrix 7 sinograms.
#	Works most, but not all, of the time...
# Note: all frames that were supposed to be 2 sec should be set to 2 sec prior to recon
#	but only frames < 3 sec should be reset to longer duration in image.
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
#	and then correct both the duration and scale factor if they differ.
#
# Current  Frame: frm1, ftime1, len1, fact1
# Previous Frame: frm0, ftime0, len0, fact0
# Corrections to Previous Frame:
#	Increase frame_duration (true_len) to: ftime1-ftime0 
#	Reduce scale_factor (fact0) by: len0/true_len
# ----------------------------------------------------------------------
	if (ftime0 > 0) {
		if (len0 < 3000) {
			true_len = ftime1 - ftime0
			new_fact = (len0 / true_len) * fact0
			if (true_len < 3000) {
				if (true_len != len0) {
					printf ("m\n%s,1,1,0,0\ne\nframe_duration\n%s\n", frm0, true_len)
					printf ("scale_factor\n%g\n", new_fact)
					printf ("d\n")
					cnt = cnt + 1
				}
			}
		}
	}
}
END {
	if (cnt > 0) {
		printf ("d\n")
	}
	printf ("q\n")
}' $1

