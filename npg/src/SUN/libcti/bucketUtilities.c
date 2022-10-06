/*$Id: bucketUtilities.c,v 1.5 2002/12/17 22:15:13 tom Exp $*/
/*$Log: bucketUtilities.c,v $
 *Revision 1.5  2002/12/17 22:15:13  tom
 *fix gcc warnings
 *
 *Revision 1.4  1999/12/13 22:02:19  tom
 *partially cleaned up with lint
 *
 * Revision 1.3  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.2  1994/03/03  17:15:32  ty7777
 * Changed id to header.
 **/

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/bucketUtilities.c,v 1.5 2002/12/17 22:15:13 tom Exp $";
#endif

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

int             setCurrentBucketEnergySettings (startBucket, numberOfBuckets, uld, lld, scatter)
	int             startBucket, numberOfBuckets, uld, lld, scatter;
{
	int             i, issueCommand ();

	char            bucketCommand[64], bucketResponse[64], msg[64];

	for (i = startBucket; i < numberOfBuckets; i++) {
		sprintf (bucketCommand, "%dS0", i);
		if (issueCommand (bucketCommand, bucketResponse)) {
			sprintf (msg, "Gantry timeout from command - %s\n", bucketCommand);
			perror (msg);
			return (i);
		}
	}
	if (uld > 0)
		for (i = startBucket; i < numberOfBuckets; i++) {
			sprintf (bucketCommand, "%dU%d", i, uld);
			if (issueCommand (bucketCommand, bucketResponse)) {
				sprintf (msg, "Gantry timeout from command - %s\n", bucketCommand);
				perror (msg);
				return (i);
			}
		}
	if (lld > 0)
		for (i = startBucket; i < numberOfBuckets; i++) {
			sprintf (bucketCommand, "%dV%d", i, lld);
			if (issueCommand (bucketCommand, bucketResponse)) {
				sprintf (msg, "Gantry timeout from command - %s\n", bucketCommand);
				perror (msg);
				return (i);
			}
		}
	if (scatter > -1)
		for (i = startBucket; i < numberOfBuckets; i++) {
			if (scatter)
				sprintf (bucketCommand, "%dY%d", i, scatter);
			else
				sprintf (bucketCommand, "%dY1", i);
			if (issueCommand (bucketCommand, bucketResponse)) {
				sprintf (msg, "Gantry timeout from command - %s\n", bucketCommand);
				perror (msg);
				return (i);
			}
		}
	return (-1);
}

int             getCurrentBucketEnergySettings (bucket, currentULD, currentLLD, currentScatter)
	int             bucket, *currentULD, *currentLLD, *currentScatter;
{
	int             issueCommand (), atoi ();

	char            bucketCommand[64], bucketResponse[64], msg[64];

	sprintf (bucketCommand, "%dS0", bucket);
	if (issueCommand (bucketCommand, bucketResponse)) {
		sprintf (msg, "Gantry timeout from command - %s\n", bucketCommand);
		perror (msg);
		return (bucket);
	}
	sprintf (bucketCommand, "%dU0", bucket);
	if (issueCommand (bucketCommand, bucketResponse)) {
		sprintf (msg, "Gantry timeout from command - %s\n", bucketCommand);
		perror (msg);
		return (bucket);
	}
	*currentULD = atoi (&bucketResponse[2]);
	sprintf (bucketCommand, "%dV0", bucket);
	if (issueCommand (bucketCommand, bucketResponse)) {
		sprintf (msg, "Gantry timeout from command - %s\n", bucketCommand);
		perror (msg);
		return (bucket);
	}
	*currentLLD = atoi (&bucketResponse[2]);
	sprintf (bucketCommand, "%dY0", bucket);
	if (issueCommand (bucketCommand, bucketResponse)) {
		sprintf (msg, "Gantry timeout from command - %s\n", bucketCommand);
		perror (msg);
		return (bucket);
	}
	*currentScatter = atoi (&bucketResponse[2]);
	return (-1);
}

int             issueCommand (command, response)
	char           *command, *response;
{
	do {
		rts_gantry (command, response);
		if (strcmp (response, "") != 0)
			return (1);
		if (response[0] == 'X')
			sleep (1);
	}
	while (response[0] == 'X');
	return (0);
}

int             setULDValue (numberOfBuckets, value)
	int             numberOfBuckets, value;
{
	char            bucketCommand[16], bucketResponse[16];

	int             i;

	for (i = 0; i < numberOfBuckets; i++) {
		bzero (bucketResponse, 16);
		sprintf (bucketCommand, "%dS0", i);
		rts_gantry (bucketCommand, bucketResponse);
		if (bucketResponse == 0) {
			printf ("setULDValue:ERROR...could not talk to bucket %2d\n", i);
			fflush (stdout);
			return (i);
		}
		sprintf (bucketCommand, "%dU%d", i, value);
		do {
			bzero (bucketResponse, 16);
			rts_gantry (bucketCommand, bucketResponse);
			if (bucketResponse == 0) {
				printf ("setULDValue:ERROR...could not set ULD on bucket %2d to %d\n",
					i, value);
				fflush (stdout);
				return (i);
			}
		}
		while (strncmp (bucketResponse, "X", strlen ("X")) != 0);
	}
	return (-1);
}

int             setLLDValue (numberOfBuckets, value)
	int             numberOfBuckets, value;
{
	char            bucketCommand[16], bucketResponse[16];

	int             i;

	for (i = 0; i < numberOfBuckets; i++) {
		bzero (bucketResponse, 16);
		sprintf (bucketCommand, "%dS0", i);
		rts_gantry (bucketCommand, bucketResponse);
		if (bucketResponse == 0) {
			printf ("setLLDValue:ERROR...could not talk to bucket %2d\n", i);
			fflush (stdout);
			return (i);
		}
		sprintf (bucketCommand, "%dV%d", i, value);
		do {
			bzero (bucketResponse, 16);
			rts_gantry (bucketCommand, bucketResponse);
			if (bucketResponse == 0) {
				printf ("setLLDValue:ERROR...could not set LLD on bucket %2d to %d\n",
					i, value);
				fflush (stdout);
				return (i);
			}
		}
		while (strncmp (bucketResponse, "X", strlen ("X")) != 0);
	}
	return (-1);
}
