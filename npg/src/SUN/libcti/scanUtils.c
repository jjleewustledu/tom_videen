/*$Id: scanUtils.c,v 1.6 1999/12/13 22:02:19 tom Exp $*/
/*$Log: scanUtils.c,v $
 *Revision 1.6  1999/12/13 22:02:19  tom
 *partially cleaned up with lint
 *
 * Revision 1.5  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.4  1995/10/30  16:54:44  tom
 * changed petutil/matrix.h to matrix.h
 *
 * Revision 1.3  1994/03/03  17:38:29  ty7777
 * Change from id to header.
 *
 * Revision 1.2  1993/09/13  18:59:40  ty7777
 * Introduce acsid and change matrix.h to petutil/matrix.h.
 *
 * Revision 1.1  1993/03/04  19:58:21  ty7777
 * Initial revision
 **/

/* @(#)scanUtils.c	2.13 6/1/92  */
#include <stdio.h>
#include <stdlib.h>
#include <rpc/rpc.h>
#include "ecatrpc.h"
#include <string.h>
#include <strings.h>
#include <matrix.h>

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/scanUtils.c,v 1.6 1999/12/13 22:02:19 tom Exp $";
#endif

/*
#define NumberOfRings defaults_get_integer ("/Ecat/EcatNumRings", 2, 0)
#define EcatModel defaults_get_integer ("/Ecat/EcatModel", 951, 0)
*/
#define NumberOfRings 2
#define EcatModel 951

int             writeLogFileHeader (sinoName, startTime, wobbleMode, acqMode, dataType, duration, logptr)
	char           *sinoName;
	time_t          startTime;
	int             wobbleMode, acqMode, dataType, duration;
	FILE           *logptr;
{
	char            msg[132], header[256];

	sprintf (msg, "Filename : %s\n", sinoName);
	fwrite (msg, strlen (msg), 1, logptr);
	sprintf (msg, "Acquired : %s", ctime (&startTime));
	fwrite (msg, strlen (msg), 1, logptr);
	sprintf (msg, "Wobble : %d\n", wobbleMode);
	fwrite (msg, strlen (msg), 1, logptr);
	sprintf (msg, "Acquisition Mode : %d\n", acqMode);
	fwrite (msg, strlen (msg), 1, logptr);
	sprintf (msg, "Data : %d\n", dataType);
	fwrite (msg, strlen (msg), 1, logptr);
	sprintf (msg, "Scan Time : %d seconds\n", duration);
	fwrite (msg, 1, strlen (msg), logptr);
	sprintf (header, "%s %s %s  %s  %s      %s        %s    %s      %s     %s     %s  %s %s  %s",
		 "Frame", "Segm", "Plane", "Bed", "Data", "Start", "Duration", "Prompts", "Delayed", "Multiples",
		 "Net Trues", "Minimum", "Maximum", "Dtcor");
	sprintf (msg, "%s\n", header);
	fwrite (msg, 1, strlen (msg), logptr);
	return (1);
}

int             configureScan (numFrames, numGates, planeDef, sinmMode, statOrWob,
			                    acqTimeInSeconds, scanFile, autoStor, mashValue, dFOV)
	int             numFrames, numGates, sinmMode, statOrWob, acqTimeInSeconds, autoStor, mashValue, dFOV;

	char           *planeDef, *scanFile;
{
	int             parseOutFileName (), status, moveWobble (), wobbleGantry (), maxPlanes;

	int             rts_conf (), rts_dfrm (), rts_strt (), rts_reso (), rts_sinm (), rts_mash (), rts_dfov (), talkToRtsInfo ();

	char           *rts_error (), *mcs_error (), sinoFileWithPath[64], sinoName[64];

	float           temp;

	Main_header     mheader;

	if (scanFile != (char *) NULL) {
		bzero (sinoFileWithPath, 64);
		bzero (sinoName, 64);
		bzero (&mheader, sizeof (Main_header));
		makeDefaultName (scanFile, sinoFileWithPath, 0);
		parseOutFileName (sinoFileWithPath, sinoName);
		fillInMainHeader (&mheader, sinoName);
		printf ("Use defaults_get_integer\n");
/*
	maxPlanes = 2 * defaults_get_integer ("/Ecat/EcatNumDirect", 16, 0) - 1;
*/
		mheader.num_frames = numFrames;
		mheader.num_gates = (numGates) ? numGates : 1;
		mheader.num_bed_pos = 0;
		mheader.coin_samp_mode = sinmMode;
		mheader.compression_code = mashValue;
	}
	if ((status = rts_conf (-1))) {
		fprintf (stderr, "rts_conf: %s\n", rts_error (status));
		return (1);
	}
	if ((status = rts_reso (statOrWob))) {
		fprintf (stderr, "rts_reso: %s\n", rts_error (status));
		return (1);
	}
	if ((status = rts_mash (mashValue))) {
		fprintf (stderr, "rts_mash: %s\n", rts_error (status));
		return (1);
	}
	if (dFOV)				 /* the default value is 0, so don't send unless
						  * specifically set */
		if ((status = rts_dfov (dFOV))) {
			fprintf (stderr, "rts_dfov: %s\n", rts_error (status));
			return (1);
		}
	if (!statOrWob) {
		reportWobble (&temp);
		if (temp != 0.0) {
			if ((status = moveWobble (0.0)))
				fprintf (stderr, "%s\n", mcs_error (status));
		}
		mheader.wobble_speed = 0;
	} else {
		if ((status = wobbleGantry (30.0))) {
			fprintf (stderr, "%s\n", mcs_error (status));
			return (1);
		}
		mheader.wobble_speed = 30;
	}
	if ((status = rts_sinm (sinmMode))) {
		fprintf (stderr, "rts_sinm: %s\n", rts_error (status));
		return (1);
	}
	if ((status = rts_dfrm (numFrames, 0, acqTimeInSeconds * 1000, numGates))) {
		fprintf (stderr, "rts_dfrm: %s\n", rts_error (status));
		return (1);
	}
	switch ((int) *planeDef) {
	case 'a':
		mheader.num_planes = maxPlanes = all_planes (NumberOfRings);
		break;
	case 'r':
		mheader.num_planes = maxPlanes = realPlanes (NumberOfRings);
		break;
	case 's':
		maxPlanes = mheader.num_planes = straightPlanes (NumberOfRings);
		break;
	case 'd':
		mheader.num_planes = maxPlanes = directPlanes (NumberOfRings);
		printf ("Use defaults_get_integer\n");
/*
	maxPlanes = mheader.num_planes = defaults_get_integer ("/Ecat/EcatNumDirect", 16, 0) - 1;
*/
		break;
	case '3':
		mheader.num_planes = maxPlanes =
			acs_3d ((NumberOfRings * 8) * (NumberOfRings * 8));
		break;
	}
	if ((status = rts_conf (0))) {
		fprintf (stderr, "rts_conf: %s\n", rts_error (status));
		return (1);
	}
	if (scanFile != (char *) NULL) {
		if ((createAMatrixFile (sinoFileWithPath, &mheader, dFOV))) {
			fprintf (stderr,
				 "configureScan:ERROR... unable to create matrix file %s containing %d matrices\n",
				 sinoFileWithPath, mheader.num_planes * mheader.num_frames *
			     ((numGates == 0) ? 1 : numGates) * ((sinmMode == 0) ? 1 : sinmMode));
			return (1);
		}
		if (autoStor) {
			if ((status = rts_astr (sinoFileWithPath, TRUE))) {
				fprintf (stderr, "astr: %s\n", rts_error (status));
				return (1);
			}
		}
	}
	singlesOn ();
	return (0);
}

int             transferSinos (frame, plane, gate, data, bed, logptr,
	                  promptTotal, delayedTotal, multTotal, correctedTotal, scanFile, verbose)
	int             frame, plane, gate, data, bed, verbose;

	FILE           *logptr;

	float          *promptTotal, *delayedTotal, *multTotal, *correctedTotal;

	char           *scanFile;
{
/*
    GLOG_resp gLogData;
    Scan_subheader scan;
    char buff[512];
    float compute921_8x8_dtcor (),
          compute_8x8_dtcor ();
    short *idata;
    double pileUp,
           planeCorr,
           strtod ();

    if (rts_rshd (scanFile, mat_numcod (frame + 1, plane + 1, gate + 1, data, bed), buff) < 0)
    {
	printf ("transferSinos: ERROR...unable to get scan subheader for %s.\n", scanFile);
	fflush (stdout);
	return (1);
    }
    convertScanHeaderFromVax (buff, &scan);
    pileUp = strtod ((char *) defaults_get_string ("/Ecat/EcatPu", "8.6e-8", 0), (char *) NULL);
    planeCorr = strtod ((char *) defaults_get_string ("/Ecat/EcatPl", "1.0", 0), (char *) NULL);
    if (EcatModel == 921)
    	scan.loss_correction_fctr = compute921_8x8_dtcor (&scan, plane, pileUp, planeCorr, 0);
    else
    	scan.loss_correction_fctr = compute_8x8_dtcor (&scan, plane, pileUp, planeCorr, 0);
    convertScanHeaderToVax (buff, &scan);
    if (rts_wshd (scanFile, mat_numcod (frame + 1, plane + 1, gate + 1, data, bed), buff) < 0)
    {
	printf ("transferSinos: ERROR...unable to write scan subheader for %s.\n", scanFile);
	fflush (stdout);
	return (1);
    }
    gLogData.start_time = scan.frame_start_time;
    gLogData.time = scan.frame_duration;
    gLogData.prompts = scan.prompts;
    gLogData.delayeds = scan.delayed;
    gLogData.multiples = scan.multiples;
    gLogData.minval = scan.scan_min;
    gLogData.maxval = scan.scan_max;
    if (verbose)
    {
	printf ("f: %3d g: %3d p: %2d d: %d b: %2d prompts: %8d delays: %6d mult: %6d min: %3d max: %6d dtcor: %4.2f\n",
	    frame + 1, gate + 1, plane + 1, data, bed, gLogData.prompts, gLogData.delayeds, gLogData.multiples,
	    gLogData.minval, gLogData.maxval, scan.loss_correction_fctr);
	fflush (stdout);
    }
    if (logptr != (FILE *) NULL)
	writeLogFileData (frame + 1, gate + 1, plane + 1, data, bed, scan.loss_correction_fctr, gLogData, logptr);
    sumGimiTotals (promptTotal, delayedTotal, multTotal, correctedTotal, gLogData);
    (*correctedTotal) *= scan.loss_correction_fctr;
*/
	return (0);
}

int             writeLogFileData (frame, gate, plane, data, bed, dtcor, gLogData, logptr)
	int             frame, gate, plane, data, bed;

	GLOG_resp       gLogData;

	float           dtcor;

	FILE           *logptr;
{
	char            msg[132];

	bzero (msg, 132);
	sprintf (msg,
		 " %3d  %3d   %3d   %2d        %7d     %7d     %7d      %7d     %7d     %7d         %3d       %5d    %6.4f\n",
		 frame, gate, plane, bed, gLogData.start_time, gLogData.time, gLogData.prompts, gLogData.delayeds,
		 gLogData.multiples, gLogData.prompts - gLogData.delayeds, gLogData.minval, gLogData.maxval, dtcor);
	fwrite (msg, strlen (msg), 1, logptr);
	return (1);
}

int             writeLogFileSummary (frameNumber, promptTotal, delayedTotal, multTotal,
	                    netTrueTotal, correctedTotal, numberOfSinograms, transferTime, logptr)
	int             frameNumber, numberOfSinograms;

	float           promptTotal, delayedTotal, multTotal, netTrueTotal, correctedTotal;

	long            transferTime;

	FILE           *logptr;
{
	char            msg[132];

	sprintf (msg,
		 "Totals for Frame %3d                              %.3e    %.3e    %.3e    %.3e\nCorrected Trues   %.3e\n",
		 frameNumber, promptTotal, delayedTotal, multTotal, netTrueTotal, correctedTotal);
	fwrite (msg, strlen (msg), 1, logptr);
	sprintf (msg, "Total transfer time for %4d sinograms was %d seconds.\n\n",
		 numberOfSinograms, transferTime);
	fwrite (msg, strlen (msg), 1, logptr);
	return (1);
}

int             computeSourceAngle (bucket, angle, numBuckets, numRings)
	int             bucket, numBuckets, numRings;

	float          *angle;
{
	int             bucketsPerRing;

	float           bucketOffset, degreesPerBucket;

	bucketsPerRing = numBuckets / numRings;
	switch (bucketsPerRing) {
	case 16:
		bucketOffset = 11.25;
		break;
	case 12:
		bucketOffset = 7.5;
		break;
	case 10:
		bucketOffset = 0.0;
		break;
	}
	degreesPerBucket = 360.0 / bucketsPerRing;
	*angle = bucketOffset + (bucket * degreesPerBucket);
	if (*angle > 360.0)
		*angle = *angle - 360.0;
	return (1);
}

int             figureModelSpecifics (numberOfRings, numberOfBuckets, ecatModel, numDetsPerRing,
				                      sino_max, numProjs, numViews, numDirect, acqServer, blocksPerBucket, detectorsPerBlockRow)
	int            *numberOfRings, *numberOfBuckets, *ecatModel, *numDetsPerRing, *sino_max, *numProjs, *numViews, *numDirect, *blocksPerBucket, *detectorsPerBlockRow;

	char           *acqServer;
{
/*
    char *host;

    *numberOfRings = (int) defaults_get_integer ("/Ecat/EcatNumRings", 2, 0);
    *numberOfBuckets = (int) defaults_get_integer ("/Ecat/EcatNumBuckets", 32, 0);
    *ecatModel = (int) defaults_get_integer ("/Ecat/EcatModel", 951, 0);
    *numDirect = (int) defaults_get_integer ("/Ecat/EcatNumDirect", 16, 0);
    host = (char *) defaults_get_string ("/Ecat/EcatAcqServer", "localhost", 0);
    strncpy (acqServer, host, strlen (host));
    switch (*ecatModel)
    {
      case 951:
	*blocksPerBucket = 4;
	*detectorsPerBlockRow = 8;
	*numProjs = 192;
	*numViews = 256;
	if (*numberOfRings == 1)
	    *sino_max = 15;
	else if (*numberOfRings == 2)
	    *sino_max = 31;
	else
	    *sino_max = 47;
	break;
      case 931:
	*blocksPerBucket = 4;
	*detectorsPerBlockRow = 8;
	*numProjs = 192;
	*numViews = 256;
	if (*numberOfRings == 1)
	    *sino_max = 7;
	else
	    *sino_max = 15;
	break;
      case 953:
      case 921:
	*numProjs = 160;
	*numViews = 192;
	*blocksPerBucket = 4;
	*detectorsPerBlockRow = 8;
	if (*numberOfRings == 1)
	    *sino_max = 15;
	else if (*numberOfRings == 2)
	    *sino_max = 31;
	else
	    *sino_max = 47;
	break;
      case 933:
	*numProjs = 160;
	*numViews = 192;
	*blocksPerBucket = 4;
	*detectorsPerBlockRow = 8;
	if (*numberOfRings == 1)
	    *sino_max = 7;
	else
	    *sino_max = 15;
	break;
      case 961:
	*numProjs = 336;
	*numViews = 392;
        *blocksPerBucket = 8;
        *detectorsPerBlockRow = 7;
	if (*numberOfRings == 1)
	    *sino_max = 15;
	else if (*numberOfRings == 2)
	    *sino_max = 31;
	else
	    *sino_max = 47;
	break;
    }
    *numDetsPerRing = *detectorsPerBlockRow * (*blocksPerBucket) * (*numberOfBuckets / *numberOfRings);
*/
	return (1);
}

sumGimiTotals (promptTotal, delayedTotal, multTotal, correctedTotal, gLogData)
	float          *promptTotal, *delayedTotal, *multTotal, *correctedTotal;

	GLOG_resp       gLogData;
{
	*promptTotal += gLogData.prompts;
	*delayedTotal += gLogData.delayeds;
	*multTotal += gLogData.multiples;
	*correctedTotal += (gLogData.prompts - gLogData.delayeds);
	return (1);
}

int             howManyTrues (fileSpec, mat)
	char           *fileSpec;

	struct Matval   mat;
{
	MatrixFile     *fPtr, *matrix_open ();

	MatrixData     *mDat, *matrix_read ();

	Scan_subheader  scan;

	int             j, k, integral, matnum, nprojs, nviews;

	short int      *ibufr;

	if ((fPtr = matrix_open (fileSpec, "r")) == NULL) {
		fprintf (stderr, "howManyTrues:ERROR...could not open file %s for reading\n", fileSpec);
		return (0);
	}
	matnum = mat_numcod (mat.frame, mat.plane, mat.gate, mat.data, mat.bed);
	if ((mDat = matrix_read (fPtr, matnum)) == NULL) {
		fprintf (stderr, "howManyTrues:ERROR... couldn't read matrix for %3d,%3d,%2d,%1d,%2d\n",
			 mat.frame, mat.plane, mat.gate, mat.data, mat.bed);
		matrix_close (fPtr);
		return (0);
	}
	bcopy (mDat->shptr, &scan, sizeof (Scan_subheader));
	nprojs = scan.dimension_1;
	nviews = scan.dimension_2;
	ibufr = (short *) mDat->data_ptr;
	for (integral = 0, k = 0; k < nviews; k++)
		for (j = 0; j < nprojs; j++)
			integral += ibufr[j + k * nprojs];
	free_matrix_data (mDat);
	matrix_close (fPtr);
	return (integral);
}

int             makeDefaultName (specifiedString, resultName, fileType)
	char           *specifiedString, *resultName;

	int             fileType;
{
	char           *fileExtension, filePath[16];

	switch (fileType) {
	case 0:
		fileExtension = ".scn";
		strcpy (filePath, "sd0");
		break;
	case 1:
		fileExtension = ".img";
		strcpy (filePath, "ecat_data");
		break;
	case 2:
		fileExtension = ".nrm";
		strcpy (filePath, "sd0");
		break;
	case 3:
		fileExtension = ".atn";
		strcpy (filePath, "sd0");
		break;
	}
	if (specifiedString == (char *) NULL)	 /* nothing in, nothing out */
		resultName = (char *) NULL;
	if (!(strpbrk (specifiedString, "/")) && (!(strpbrk (specifiedString, "."))))	/* no path or extension */
		sprintf (resultName, "/%s/%s%s", filePath, specifiedString, fileExtension);
	if (!(strpbrk (specifiedString, "/")) && ((strpbrk (specifiedString, ".") != 0)))	/* no path with
												 * extension */
		sprintf (resultName, "/%s/%s", filePath, specifiedString);
	if ((strpbrk (specifiedString, "/") != 0) && (!(strpbrk (specifiedString, "."))))	/* path with no
												 * extension */
		sprintf (resultName, "%s%s", specifiedString, fileExtension);
	if ((strpbrk (specifiedString, "/") != 0) &&
	    ((strpbrk (specifiedString, ".") != 0)))	/* path with extension */
		sprintf (resultName, "%s", specifiedString);
	return (0);
}

void            fillInCharFields (mh, name)
	Main_header    *mh;

	char           *name;
{
	int             parseOutFileName ();

	char            scanName[64];

	parseOutFileName (name, scanName);
	strncpy (mh->original_file_name, scanName, 19);
	mh->original_file_name[19] = 0;		 /* make sure name doesn't go too far */
	sprintf (mh->node_id, "%s", "enola");
	sprintf (mh->isotope_code, "%s", "Ge-68");
	mh->isotope_halflife = 23760000.0;
	sprintf (mh->radiopharmaceutical, "%s", "radio what?");
	sprintf (mh->study_name, "%s", "mytest");
	sprintf (mh->patient_id, "%s", " ");
	sprintf (mh->patient_name, "%s", "twjansto");
	mh->patient_sex = 'm';
	sprintf (mh->patient_age, "%s", "32");
	sprintf (mh->patient_weight, "%s", "215 lbs");
	sprintf (mh->patient_height, "%s", "77 inches");
	mh->patient_dexterity = 'l';
	sprintf (mh->physician_name, "%s", "Dr. Luke");
	sprintf (mh->operator_name, "%s", "jansto");
	sprintf (mh->study_description, "%s", "my test");
	sprintf (mh->facility_name, "%s", "cti");
	sprintf (mh->user_process_code, "%s", " ");
	return;
}

int             power (x, n)
	int             x, n;
{
	int             p;

	for (p = 1; n > 0; --n)
		p = p * x;
	return (p);
}


int             makeDriftEntry (entry)
	char           *entry;
{
	FILE           *driftLog;

	time_t          today;

	char           *out;

	driftLog = fopen ("/ecat_data/daily.drift", "a+");
	if (!driftLog) {
		printf ("ERROR...cannot open /ecat_data/daily.drift\n");
		fflush (stdout);
		return (1);
	}
	out = (char *) malloc (strlen (entry) + 32);
	today = (time_t) time ((time_t) NULL);
	sprintf (out, "%s", ctime (&today));
	fputs (out, driftLog);
	bzero (out, strlen (out) + 1);
	sprintf (out, "     %s\n", entry);
	fputs (out, driftLog);
	fclose (driftLog);
	free ((char *) out);
	return (0);
}
