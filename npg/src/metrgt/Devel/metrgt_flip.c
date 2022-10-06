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
	float           h1c, c1c, o1c, t1c, h2c, c2c, o2c, t2c;		/* clot counts */
	float           h1p, c1p, o1p, t1p, h2p, c2p, o2p, t2p;		/* peri counts */
	float           cbfcnts1, cbvcnts1, oefcnts1, tiscnts1;
	float           cbfcnts2, cbvcnts2, oefcnts2, tiscnts2;
	float           cbf1, cbv1, oef1, cmr1;
	float           cbf2, cbv2, oef2, cmr2;
	int				proc;
	long			pixc, pixp;
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
	cbfcnts2 = 0;
	cbvcnts2 = 0;
	oefcnts2 = 0;
	tiscnts2 = 0;
	totpix = 0;

	fp = fopen (rgtfile, "r");
	if (fp == NULL) {
		fprintf (stderr, "ERROR [metrgt]: Cannot open region file %s\n", rgtfile);
		return (ERROR);
	}
	while (fscanf (fp, "%s %s %f %f %f %f %f %f %f %f %d",
		 rnum, rnam, &h1c, &c1c, &o1c, &t1c, &h2c, &c2c, &o2c, &t2c, &pixc) == 11) {
		fscanf (fp, "%s %s %f %f %f %f %f %f %f %f %d",
		 rnum, rnam, &h1p, &c1p, &o1p, &t1p, &h2p, &c2p, &o2p, &t2p, &pixp);
		cbfcnts1 += h1p * pixp - h1c * pixc;
		cbvcnts1 += c1p * pixp - c1c * pixc;
		oefcnts1 += o1p * pixp - o1c * pixc;
		tiscnts1 += t1p * pixp - t1c * pixc;
		cbfcnts2 += h2p * pixp - h2c * pixc;
		cbvcnts2 += c2p * pixp - c2c * pixc;
		oefcnts2 += o2p * pixp - o2c * pixc;
		tiscnts2 += t2p * pixp - t2c * pixc;
		totpix += pixp - pixc;
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
	
	tiscnts2 *= 0.001;
	cbfcnts2 /= tiscnts2;
	cbvcnts2 /= tiscnts2;
	oefcnts2 /= tiscnts2;
	proc = PROC_CBF;
	cbf2 = procPET (cbfcnts2, proc, cbfhdr, &hdr, fp);
	proc = PROC_CBV;
	cbv2 = procPET (cbvcnts2, proc, cbvhdr, &hdr, fp);
	proc = PROC_OEF;
	oef2 = procOxygen (oefcnts2, cbfcnts2, cbvcnts2, proc, oefhdr, cbfhdr, cbvhdr, &hdr, fp);
	proc = PROC_CMRO2;
	cmr2 = procOxygen (oefcnts2, cbfcnts2, cbvcnts2, proc, oefhdr, cbfhdr, cbvhdr, &hdr, fp);

	printf ("Pixels:  %d\n", totpix);
	printf ("%s   clot-side  flip-side\n", pid);
	printf ("------------------------------\n");
	printf ("CBF    = %8.3f %10.3f \n", cbf1, cbf2);
	printf ("CBV    = %8.3f %10.3f \n", cbv1, cbv2);
	printf ("OEF    = %8.3f %10.3f \n", oef1, oef2);
	printf ("CMRO2  = %8.3f %10.3f \n", cmr1, cmr2);

	fp = fopen (outfile, "w");
	if (fp == NULL) {
		fprintf (stderr, "ERROR [metrgt]: Cannot open region file %s\n", outfile);
		return (ERROR);
	}
	fprintf (fp, "%s   clot-side  flip-side\n", pid);
	fprintf (fp, "------------------------------\n");
	fprintf (fp, "CBF    = %8.3f %10.3f \n", cbf1, cbf2);
	fprintf (fp, "CBV    = %8.3f %10.3f \n", cbv1, cbv2);
	fprintf (fp, "OEF    = %8.3f %10.3f \n", oef1, oef2);
	fprintf (fp, "CMRO2  = %8.3f %10.3f \n", cmr1, cmr2);
	fclose (fp);
	return (OK);
}
