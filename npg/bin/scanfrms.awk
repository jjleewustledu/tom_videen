awk 'BEGIN { FS=":";
	printf ("s\nn\n")
}

$1 ~ /^3d Sinogram subheader/ {
	matrix = substr($1,34)
}

$1 ~ /^Total Avg. Corrected Singles/ {
	if ($2 == 0)
		printf ("m\n%s\ne\ntotal\n%g\n", matrix, totcor)
	else
		totcor = $2
}

$1 ~ /^Total Avg. Uncorrected Singles/ {
	if ($2 == 0) {
		#hdrEdit cannot change this in command mode
		#printf ("total uncorr\n%g\n", totuncor)
	}
	else 
		totuncor = $2
}

$1 ~ /^Uncorrected Singles/ {
	if ($2 == 0) {
		printf ("uncor\n")
		for (i=1; i<=42; i++) {
			printf ("%d\n%g\n", i, sing[i])
			getline
		}
		printf ("0\nd\n")
	}
	else {
		for (i=1; i<=42; i++) {
			sing[i] = $2
			getline
		}
	}
}

END { printf ("q\n") }' $1
