/*	$Log$ */
/* ===============================================================================
 *	Module:			metrgt.c
 *	Date:			Jun 2000
 *	Author:			Tom Videen
 *	Description:	Compute PVC corrected CBF, CBV, OEF, CMRO2
 * ===============================================================================
 */
static char     rcsid[] = "$Header: /home/npggw/tom/src/metrgt/RCS/metrgt.c,v 1.1 1998/05/27 16:50:55 tom Exp $";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>
#include <libimage.h>
#include <imgutil.h>

main (argc, argv)
	int             argc;
	char          **argv;
{
	FILE           *fp = NULL;
	HEADER_DATA     hdr;
	float           h1c, c1c, o1c, t1c;		/* regional counts */
	float           cbfcnts1, cbvcnts1, oefcnts1, tiscnts1;
	float           cbf1, cbv1, oef1, cmr1;
	int				proc;
	long			pixc;
	long			totpix;
	char           *rgtfile, *outfile;
	char           *cbfhdr, *cbvhdr, *oefhdr;
	char           *pid;
	char            rnum[16];
	char            rnam[16];

/*
 * Get command line arguments.
 */
	if (argc != 7) {
		fprintf (stderr, "Usage: metrgt RGT HOhdr COhdr OOhdr OUT PID\n");
		return (ERROR);
	}
	rgtfile = argv[1];
	cbfhdr  = argv[2];
	cbvhdr  = argv[3];
	oefhdr  = argv[4];
	outfile = argv[5];
	pid = argv[6];

	cbfcnts1 = 0;
	cbvcnts1 = 0;
	oefcnts1 = 0;
	tiscnts1 = 0;
	totpix = 0;

	fp = fopen (rgtfile, "r");
	if (fp == NULL) {
		fprintf (stderr, "ERROR [metrgt]: Cannot open region file %s\n", rgtfile);
		return (ERROR);
	}
	while (fscanf (fp, "%s %s %f %f %f %f %d",
		 rnum, rnam, &h1c, &c1c, &o1c, &t1c, &pixc) == 7) {
		cbfcnts1 += h1c * pixc;
		cbvcnts1 += c1c * pixc;
		oefcnts1 += o1c * pixc;
		tiscnts1 += t1c * pixc;
		totpix += pixc;
	}
	fclose (fp);
	tiscnts1 *= 0.001;
	cbfcnts1 /= tiscnts1;
	cbvcnts1 /= tiscnts1;
	oefcnts1 /= tiscnts1;
	proc = PROC_CBF;
	cbf1 = procPET (cbfcnts1, proc, cbfhdr, &hdr, fp);
	proc = PROC_CBV;
	cbv1 = procPET (cbvcnts1, proc, cbvhdr, &hdr, fp);
	proc = PROC_OEF;
	oef1 = procOxygen (oefcnts1, cbfcnts1, cbvcnts1, proc, oefhdr, cbfhdr, cbvhdr, &hdr, fp);
	proc = PROC_CMRO2;
	cmr1 = procOxygen (oefcnts1, cbfcnts1, cbvcnts1, proc, oefhdr, cbfhdr, cbvhdr, &hdr, fp);
	
	printf ("%s  (%d pixels)\n", pid, totpix);
	printf ("-------------------\n");
	printf ("CBF    = %8.3f \n", cbf1);
	printf ("CBV    = %8.3f \n", cbv1);
	printf ("OEF    = %8.3f \n", oef1);
	printf ("CMRO2  = %8.3f \n", cmr1);

	fp = fopen (outfile, "w");
	if (fp == NULL) {
		fprintf (stderr, "ERROR [metrgt]: Cannot open region file %s\n", outfile);
		return (ERROR);
	}
	fprintf (fp, "%s  (%d pixels)\n", pid, totpix);
	fprintf (fp, "-------------------\n");
	fprintf (fp, "CBF    = %8.3f \n", cbf1);
	fprintf (fp, "CBV    = %8.3f \n", cbv1);
	fprintf (fp, "OEF    = %8.3f \n", oef1);
	fprintf (fp, "CMRO2  = %8.3f \n", cmr1);

	fclose (fp);
	return (OK);
}
