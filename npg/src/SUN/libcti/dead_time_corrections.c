/*$Id: dead_time_corrections.c,v 1.7 1999/12/13 22:02:19 tom Exp $*/
/*$Log: dead_time_corrections.c,v $
 *Revision 1.7  1999/12/13 22:02:19  tom
 *partially cleaned up with lint
 *
 * Revision 1.6  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.5  1995/10/30  16:54:44  tom
 * changed petutil/matrix.h to matrix.h
 *
 * Revision 1.4  1994/03/03  17:17:24  ty7777
 * Change id to header.
 **/

#include <stdio.h>
#include <matrix.h>

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/dead_time_corrections.c,v 1.7 1999/12/13 22:02:19 tom Exp $";
#endif

float           compute921_8x8_dtcor (scan, plane_no, pu_cor, pl_cor, debug_flag)
	Scan_subheader *scan;
	int             plane_no, debug_flag;
	double          pu_cor, pl_cor;
{
	int             numBuckets, bucket1, bucketOffset, nPairs;
	double          scor, denom, numer, prob_single, prob_coinc, ipcor, mcor;
	double          singles, plane_cor, pileup_cor, dtcor;
	static float    plane_table[47] =
	{
		0.46, 0.36, 0.32, 0.22, 0.16, 0.11, 0.08, 0.12, 0.08, 0.13,
		0.13, 0.03, 0.11, 0.15, 0.23, 0.16, 0.26, 0.21, 0.22, 0.17,
		-0.01, -0.01, -0.07, -0.05, -0.08, 0.00, -0.02, 0.07, 0.10, 0.00,
		0.02, -0.12, -0.11, -0.17, -0.23, -0.26, -0.26, -0.28, -0.31, -0.28,
		-0.33, -0.28, -0.28, -0.22, -0.17, -0.10, -0.02
	};

	int             i, j, jstart, jend;
	float           ipow ();

	if (pu_cor == 0.0)
		pu_cor = 8.6e-8;
	if (pl_cor == 0.0)
		pl_cor = 1.0;
	printf ("Use defaults_get_integer\n");
/*
    switch ((int) defaults_get_integer ("/Ecat/EcatModel", 951, 0))
*/
	switch ((int) 951) {
	case 931:
	case 951:
	default:
		bucket1 = 11;
		bucketOffset = 5;
		numBuckets = 16;
		nPairs = 56;
		break;
	case 933:
	case 953:
	case 921:
		bucket1 = 9;
		bucketOffset = 3;
		numBuckets = 12;
		nPairs = 42;
		break;
	case 961:
		bucket1 = 10;
		bucketOffset = 4;
		numBuckets = 14;
		nPairs = 49;
		break;
	}
	scor = 0.0;
	for (i = 0; i < bucket1; i++) {
		jstart = i + bucketOffset;
		jend = (1 + i + bucket1 < numBuckets) ? 1 + i + bucket1 : numBuckets;

		for (j = jstart; j < jend; j++) {
			denom = scan->cor_singles[i] * scan->cor_singles[j];
			numer = scan->uncor_singles[i] * scan->uncor_singles[j];
			if (denom > 0.0)
				scor += (numer / denom);
		}
	}
	if (scor == 0.0)
		scor = (float) nPairs;
	if (scor < 1.0)
		scor = 1.0;
	scor = (float) nPairs / scor;
	if (debug_flag)
		printf ("scor = %10.6f, tot_uncor = %f, numBuck= %d\n",
			scor, scan->tot_avg_uncor, numBuckets);
	prob_single = 256.0e-9 * scan->tot_avg_uncor / (float) numBuckets;
	prob_coinc = prob_single * prob_single * 380. / 4096.;
	if (debug_flag)
		printf ("prob_single = %10.7e, prob_coin = %10.7e\n", prob_single, prob_coinc);
	if (prob_coinc > 0.0)
		ipcor = (1.0 - ipow (1.0 - prob_coinc, nPairs))
			/ ((float) nPairs * prob_coinc * ipow (1.0 - prob_coinc, nPairs - 1));
	else
		ipcor = 1.0;
	if (ipcor < 1.0)
		ipcor = 1.0;
	mcor = 1.0;
	if (scan->prompts > 0)
		mcor = 1.0 + (float) scan->multiples / (float) scan->prompts;
	singles = scan->tot_avg_cor / (float) numBuckets;
	plane_cor = 1.0 + plane_table[plane_no] * pu_cor * pl_cor * singles;
	pileup_cor = 1.0 + pu_cor * singles;
	dtcor = scor * ipcor * mcor * plane_cor * pileup_cor;
	if (dtcor < 1.0)
		dtcor = 1.0;
	if (debug_flag)
		printf ("%2d %10.6f %10.7e %10.7e %10.6f %10.6f %10.6f %10.6f  %10.6f\n",
			plane_no, scor, prob_single, prob_coinc, ipcor, mcor, plane_cor,
			pileup_cor, dtcor);
	return (dtcor);
}

float           compute_8x8_dtcor (scan, plane_no, pu_cor, pl_cor, debug_flag)
	Scan_subheader *scan;

	int             plane_no, debug_flag;

	double          pu_cor, pl_cor;
{
	int             numBuckets, bucket1, bucketOffset, nPairs;

	double          scor, denom, numer, prob_single, prob_coinc, ipcor, mcor;

	double          singles, plane_cor, pileup_cor, dtcor;

	static float    plane_table[31] =
	{
		1.03, 1.00, 0.41, -0.1, -0.23, -0.45, -0.75, -0.75,
		-0.75, -0.45, -0.23, -0.1, 0.41, 1.00, 1.03, 1.00,
		1.03, 1.00, 0.41, -0.1, -0.23, -0.45, -0.75, -0.75,
		-0.75, -0.45, -0.23, -0.1, 0.41, 1.00, 1.03
	};

	int             i, j, jstart, jend;

	float           ipow ();

	if (pu_cor == 0.0)
		pu_cor = 8.6e-8;
	if (pl_cor == 0.0)
		pl_cor = 1.0;
	printf ("Use defaults_get_integer\n");
/*
    switch ((int) defaults_get_integer ("/Ecat/EcatModel", 951, 0))
*/
	switch ((int) 951) {
	case 931:
	case 951:
	default:
		bucket1 = 11;
		bucketOffset = 5;
		numBuckets = 16;
		nPairs = 56;
		break;
	case 933:
	case 953:
	case 921:
		bucket1 = 9;
		bucketOffset = 3;
		numBuckets = 12;
		nPairs = 42;
		break;
	}
	scor = 0.0;
	for (i = 0; i < bucket1; i++) {
		jstart = i + bucketOffset;
		jend = (1 + i + bucket1 < numBuckets) ? 1 + i + bucket1 : numBuckets;

		for (j = jstart; j < jend; j++) {
			denom = scan->cor_singles[i] * scan->cor_singles[j];
			numer = scan->uncor_singles[i] * scan->uncor_singles[j];
			if (denom > 0.0)
				scor += (numer / denom);
		}
	}
	if (scor == 0.0)
		scor = (float) nPairs;
	if (scor < 1.0)
		scor = 1.0;
	scor = (float) nPairs / scor;
	if (debug_flag)
		printf ("scor = %10.6f, tot_uncor = %f, numBuck= %d\n",
			scor, scan->tot_avg_uncor, numBuckets);
	prob_single = 256.0e-9 * scan->tot_avg_uncor / (float) numBuckets;
	prob_coinc = prob_single * prob_single * 380. / 4096.;
	if (debug_flag)
		printf ("prob_single = %10.7e, prob_coin = %10.7e\n", prob_single, prob_coinc);
	if (prob_coinc > 0.0)
		ipcor = (1.0 - ipow (1.0 - prob_coinc, nPairs))
			/ ((float) nPairs * prob_coinc * ipow (1.0 - prob_coinc, nPairs - 1));
	else
		ipcor = 1.0;
	if (ipcor < 1.0)
		ipcor = 1.0;
	mcor = 1.0;
	if (scan->prompts > 0)
		mcor = 1.0 + (float) scan->multiples / (float) scan->prompts;
	singles = scan->tot_avg_cor / (float) numBuckets;
	plane_cor = 1.0 + plane_table[plane_no] * pu_cor * pl_cor * singles;
	pileup_cor = 1.0 + pu_cor * singles;
	dtcor = scor * ipcor * mcor * plane_cor * pileup_cor;
	if (dtcor < 1.0)
		dtcor = 1.0;
	if (debug_flag)
		printf ("%2d %10.6f %10.7e %10.7e %10.6f %10.6f %10.6f %10.6f  %10.6f\n",
			plane_no, scor, prob_single, prob_coinc, ipcor, mcor, plane_cor,
			pileup_cor, dtcor);
	return (dtcor);
}

float           ipow (val, pow)
	double          val;

	int             pow;
{
	float           ans;

	int             i;

	ans = 1.0;
	for (i = 0; i < pow; i++)
		ans = ans * val;
	return (ans);
}
