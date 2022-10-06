awk 'BEGIN { FS=":";
	nbuckets = 42
	flag = 0
	printf ("s\nn\n")
}

# ---------------------------------------------------------------
# Date:   29-Jan-2003
#
# Set nbuckets to the number of detector buckets for your scanner:
#	EXACT (921)     = 36
#	EXACT HR (961)  = 42
#	EXACT HR+ (962) = 24
#
# Intent: Insert missing uncorrected singles in ECAT Matrix 7 sinograms
#	acquired with 2-sec dynamic frames.
#
# When the singles are missing in a frame, the frame typically has run
# longer than specified. In addition, the singles in the following
# frame will often be eroneous, either high or low. Therefore,
# this script searches for Total Avg. Uncorrected Singles=0
# and then sets the singles values in this frame and the following one
# to interpolated values. The interpolation uses singles in the frame
# prior to the first of a series of frames with singles=0 to the
# second frame following the last of the series with singles=0.
# If multiple frames in a row have 0s, possibly in alternating frames, the
# script will go beyond the last one and then interpolate values for
# all frames in between.
#
# Both Uncorrected Singles and Total Avg. Corrected Singles are changed.
# Uncorrected Singles affects deadtime correction during reconstruction.
# Total Corrected Singles affects nothing but flags that this routine was run.
# Total Avg. Uncorrected Singles is NOT changed; by remaining 0, it can
# be checked to identify frames have been corrected by this routine.
#
# This script is based on the following behavior in the 7.1 sinogram headers:
# 1) Singles values are a rate rather than the total per frame.
#    	Uncorrected Singles             (constant for 2-10 sec frames)
#    	Total Avg. Uncorrected Singles		"
#    	Total Avg. Corrected Singles		"
# 2) This differs from Prompts, Delayed, Net Trues, which are totals per frame.
# 3) Loss Correction Factor is always 1 has no affect on reconstruction.
# 4) Frame Duration does affect reconstruction.
# ---------------------------------------------------------------

# Get matrix number
# -----------------------------------
$1 ~ /^3d Sinogram subheader/ {
	matrix = substr($1,34)
	split(matrix,val,",")
	frame = val[1]
}

# Get Total Avg. Corrected Singles
# -----------------------------------
$1 ~ /^Total Avg. Corrected Singles/ {
	totcor[frame] = $2
}

# Set flag, frm1, frm2 using total uncorrected singles (never changed)
# -----------------------------------
$1 ~ /^Total Avg. Uncorrected Singles/ {
	tuncor = $2
	--flag 
	if (tuncor == 0) {
		flag = 3
	} else if (flag == 1) {
		frm2 = frame
	} else if (flag < 0) {
		flag = 0
		frm1 = frame
	}
}

# Get Frame Duration
# -----------------------------------
$1 ~ /^Frame Duration/ {
	time[frame] = $2
}

# Get Uncorrected Singles for each bucket
# -----------------------------------
$1 ~ /^Uncorrected Singles/ {
	for (i=1; i<=nbuckets; i++) {
		sing[nbuckets*frame+i] = $2
		getline
	}

# If flag == 1, interpolate singles for all frames between frm1 and frm2;
# ----------------------------------------------------------------------
	if (flag == 1) {
		ff = frm2-frm1
		a = totcor[frm1]
		b = totcor[frm2]
		for (i=1; i<=nbuckets; i++) {
			singa[i] = sing[nbuckets*frm1+i]
			singb[i] = sing[nbuckets*frm2+i]
		}
 		for (f=frm1+1; f<frm2; f++) {
 			r1 = (frm2-f)/ff
 			r2 = (f-frm1)/ff
 			new = (r1*a + r2*b)
			printf ("m\n%s,1,1,0,0\ne\ntotal\n%g\n", f, new)
			printf ("uncorrected\n")
			for (i=1; i<=nbuckets; i++) {
				new = (r1*singa[i] + r2*singb[i])
				printf ("%d\n%d\n", i, new)
			}
			printf ("0\n")
			if (time[f] != time[f-1]) {
				printf ("frame_duration\n%g\n", time[f-1])
			    time[f] = time[f-1]
			}
			printf ("d\n")
		}
		flag = 0
	}
}

END {
# Copy values into last frame if its singles = zero
# ---------------------------------------------------
	if (flag == 2) {
		f = frame
 		new = totcor[frm1]
		printf ("m\n%s,1,1,0,0\ne\ntotal\n%g\n", f, new)
		printf ("uncorrected\n")
		for (i=1; i<=nbuckets; i++) {
			new = sing[nbuckets*frm1+i]
			printf ("%d\n%d\n", i, new)
		}
		printf ("0\n")
		if (time[f] != time[f-1]) {
			printf ("frame_duration\n%g\n", time[f-1])
		}
		printf ("d\n")
	}
	printf ("q\n")
}' $1

