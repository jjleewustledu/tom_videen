/*$Id: mcs.c,v 1.5 1999/12/13 22:02:19 tom Exp $*/
/*$Log: mcs.c,v $
 *Revision 1.5  1999/12/13 22:02:19  tom
 *partially cleaned up with lint
 *
 * Revision 1.4  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.3  1994/03/03  17:32:54  ty7777
 * Change from if to header.
 *
 * Revision 1.2  1993/09/13  19:44:45  ty7777
 * Introduce rcsid.
 *
 * Revision 1.1  1993/03/04  19:48:59  ty7777
 * Initial revision
 **/

/*    @(#)mcs.c	2.10 6/8/92    */
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <rpc/rpc.h>
#include "ecatMcs.h"

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/mcs.c,v 1.5 1999/12/13 22:02:19 tom Exp $";
#endif

/*
#define EcatModel defaults_get_integer ("/Ecat/EcatModel", 951, 0)
*/
#define EcatModel 951
#define EcatMcsRpcTimeout -2
#define GANTRY_TILT_SCALE       0.123
#define GANTRY_ROTATE_SCALE     0.09
#define HORIZONTAL_SCALE        0.082447
#define VERTICAL_SCALE          0.16667
#define CTI_HORIZONTAL_SCALE    0.8
#define CTI_VERTICAL_SCALE      0.33
#define DETECTOR_ORBIT_SCALE    0.351563
#define SOURCE_ROTATE_SCALE     0.02
#define WOBBLE_SPEED_SCALE      60.0
#define ROTATE_SPEED_SCALE      24.0

/*
 * twjansto		24 may 1990
 */

static MCS_STATUS mcsStatus;

int             moveHorizontalBed (moveToHere)
	float           moveToHere;
{
	int             resp;
	float           encoderValue, currentBedEncoder, refBedEncoder;
	bool_t          doAcsMcsCommand ();

	if (!(doAcsMcsCommand (REPORT_MOTIONS, xdr_void, (char *) NULL, xdr_MCS_STATUS, &mcsStatus)))
		return (EcatMcsRpcTimeout);
	currentBedEncoder = mcsStatus.hb_encoder;
	refBedEncoder = mcsStatus.bed_ref_encoder;
	encoderValue = (moveToHere / ((EcatModel == 921 || EcatModel == 961) ?
				      1.0 : HORIZONTAL_SCALE)) + refBedEncoder;
	if (encoderValue == currentBedEncoder)
		return (0);
	if (!(doAcsMcsCommand (MOVE_HORIZONTAL_BED, xdr_float, &encoderValue, xdr_int, &resp)))
		return (EcatMcsRpcTimeout);
	return (resp);
}

int             moveHorizontalBedAbs (moveToHere)
	float           moveToHere;
{
	int             resp;
	float           encoderValue;
	bool_t          doAcsMcsCommand ();

	encoderValue = moveToHere / ((EcatModel == 921 || EcatModel == 961) ? 1.0 : HORIZONTAL_SCALE);
	if (!(doAcsMcsCommand (MOVE_HORIZONTAL_BED, xdr_float, &encoderValue, xdr_int, &resp)))
		return (EcatMcsRpcTimeout);
	return (resp);
}

int             moveVerticalBed (moveToHere)
	float           moveToHere;
{

	/*
	 * Tom Yang on 2/3/93 int resp, encoderValue; bool_t doAcsMcsCommand();
	 * 
	 * encoderValue = nint(moveToHere / VERTICAL_SCALE); if (!(doAcsMcsCommand(MOVE_VERTICAL_BED,
	 * xdr_int, &encoderValue, xdr_int, &resp))) return (EcatMcsRpcTimeout); return (resp);
	 */
	return (1);
}

int             moveWobble (moveToHere)
	float           moveToHere;
{

	/*
	 * Tom Yang on 2/3/93. int resp, encoderValue; bool_t doAcsMcsCommand();
	 * 
	 * encoderValue = nint(moveToHere / DETECTOR_ORBIT_SCALE); if (!(doAcsMcsCommand(MOVE_WOBBLE,
	 * xdr_int, &encoderValue, xdr_int, &resp))) return (EcatMcsRpcTimeout); return (resp);
	 */
	return (1);
}

int             moveRotatingSource (moveToHere)
	float           moveToHere;
{
/*
	int resp,
	    encoderValue;
	bool_t doAcsMcsCommand();

	if (strncmp(defaults_get_string("/Ecat/EcatTransmissionSource", "None", 0),
			"Rod", strlen("Rod")) == NULL)
		return (1001);
	encoderValue = nint(moveToHere / SOURCE_ROTATE_SCALE);
	if (!(doAcsMcsCommand(MOVE_ROTATING_SOURCE, xdr_int, &encoderValue, xdr_int, &resp)))
		return (EcatMcsRpcTimeout);
	return (resp);
*/
	return (1);
}

int             wobbleGantry (wobbleSpeed)
	float           wobbleSpeed;
{
	int             resp, encoderValue;

	bool_t          doAcsMcsCommand ();

	encoderValue = (wobbleSpeed / WOBBLE_SPEED_SCALE) * 127.0;
	if (!(doAcsMcsCommand (ROTATE_WOBBLE, xdr_int, &encoderValue, xdr_int, &resp)))
		return (EcatMcsRpcTimeout);
	return (resp);
}

int             rotateSource (rotateSpeed)
	float           rotateSpeed;
{
	int             resp, encoderValue;

	bool_t          doAcsMcsCommand ();

	printf ("Use defaults_get_string\n");
/*
	if (strncmp(defaults_get_string("/Ecat/EcatTransmissionSource", "None", 0),
			"Rod", strlen("Rod")) == NULL)
		return (1001);
*/
	encoderValue = (rotateSpeed / ROTATE_SPEED_SCALE) * 127.0;
	if (!(doAcsMcsCommand (ROTATE_SOURCE, xdr_int, &encoderValue, xdr_int, &resp)))
		return (EcatMcsRpcTimeout);
	return (resp);
}

int             extendRingsOnly ()
{
	int             resp, extend = TRUE;

	bool_t          doAcsMcsCommand ();

	printf ("Use defaults_get_string\n");
/*
	if (strncmp(defaults_get_string("/Ecat/EcatTransmissionSource", "None", 0),
			"None", strlen("None")) == NULL)
		return (1002);
*/
	changeMcsTimeoutValue (300);
	if (!(doAcsMcsCommand (MOVE_TR_SOURCE, xdr_int, &extend, xdr_int, &resp)))
		resp = EcatMcsRpcTimeout;
	changeMcsTimeoutValue (60);
	return (resp);
}

int             retractRingsOnly ()
{
	int             resp, retract = FALSE;

	bool_t          doAcsMcsCommand ();

	printf ("Use defaults_get_string\n");
/*
	if (strncmp(defaults_get_string("/Ecat/EcatTransmissionSource", "None", 0),
			"None", strlen("None")) == NULL)
		return (1002);
*/
	changeMcsTimeoutValue (300);
	if (!(doAcsMcsCommand (MOVE_TR_SOURCE, xdr_int, &retract, xdr_int, &resp)))
		resp = EcatMcsRpcTimeout;
	changeMcsTimeoutValue (60);
	return (resp);
}

int             extendRings ()
{
	int             resp, extend = TRUE;

	bool_t          doAcsMcsCommand ();

	printf ("Use defaults_get_string\n");
/*
	if (strncmp(defaults_get_string("/Ecat/EcatTransmissionSource", "None", 0),
			"None", strlen("None")) == NULL)
		return (1002);
*/
	changeMcsTimeoutValue (300);
	if (!(doAcsMcsCommand (MOVE_TR_SOURCE, xdr_int, &extend, xdr_int, &resp)))
		resp = EcatMcsRpcTimeout;
	changeMcsTimeoutValue (60);
	printf ("Use defaults_get_integer\n");
/*
	if (setULDValue((int) defaults_get_integer("/Ecat/EcatNumBuckets", 32, 0), 650) >= 0)
		return (FALSE);
*/
	return (resp);
}

int             retractRings ()
{
	int             resp, retract = FALSE;

	bool_t          doAcsMcsCommand ();

	printf ("Use defaults_get_integer\n");
/*
	if (setULDValue((int) defaults_get_integer("/Ecat/EcatNumBuckets", 32, 0), 850) >= 0)
		return (FALSE);
*/
	printf ("Use defaults_get_string\n");
/*
	if (strncmp(defaults_get_string("/Ecat/EcatTransmissionSource", "None", 0),
			"None", strlen("None")) == NULL)
		return (1002);
*/
	changeMcsTimeoutValue (300);
	if (!(doAcsMcsCommand (MOVE_TR_SOURCE, xdr_int, &retract, xdr_int, &resp)))
		resp = EcatMcsRpcTimeout;
	changeMcsTimeoutValue (60);
	return (resp);
}

#define IsLost  0x0
#define IsExtended  0x1
#define IsRetracted 0x2
#define ExtendFailed 0x4
#define LastComandCompletedOK 0x8
#define BedPositionUnknownOnA921 0x8
#define RelaysStuck 0x8
#define RetractFailed 0x10
#define IsExtending 0x20
#define IsRetracting 0x40
#define IsExtendingOnA921 0x10
#define IsRetractingOnA921 0x20
#define ExtendedKillSwitchHit 0x40
#define RetractedKillSwitchHit 0x80
#define MoveCommandActiveOnA921 0x80
#define SeptaMotionWait 150

int             extendSepta ()
{
	int             resp, extend = TRUE;
	bool_t          doAcsMcsCommand ();

	if (EcatModel == 921 || EcatModel == 961) {	/* for 921 scanners, use ACS MOVE_SEPTA
							 * command */
		changeMcsTimeoutValue (SeptaMotionWait);
		if (!(doAcsMcsCommand (MOVE_SEPTA, xdr_int, &extend, xdr_int, &resp))) {
			changeMcsTimeoutValue (60);
			return (EcatMcsRpcTimeout);
		} else {
			changeMcsTimeoutValue (60);
			return (resp);
		}
	} else					 /* for 950 series scanners, ACS passes through
						  * commands */
		return (extendSeptaSerially ());
}

int             retractSepta ()
{
	int             resp, retract = FALSE;

	bool_t          doAcsMcsCommand ();

	if (EcatModel == 921 || EcatModel == 961) {	/* for 921 scanners, use ACS MOVE_SEPTA
							 * command */
		changeMcsTimeoutValue (SeptaMotionWait);
		if (!(doAcsMcsCommand (MOVE_SEPTA, xdr_int, &retract, xdr_int, &resp))) {
			changeMcsTimeoutValue (60);
			return (EcatMcsRpcTimeout);
		} else {
			changeMcsTimeoutValue (60);
			return (resp);
		}
	} else					 /* for 950 series scanners, ACS passes through
						  * commands */
		return (retractSeptaSerially ());
}

int             extendSeptaSerially ()
{
	int             status, waitTillSeptaFinishes ();

	char            resp[8];

	resp[0] = '\0';
	mcsSerialBus ("246X", resp);
	if (resp[0] == '\0')
		return (990);
	if ((strcmp ("N 1", resp) != 0) && (strcmp ("N 0", resp) != 0))
		return (250);
	sleep (2);				 /* wait till it gets started */
	status = waitTillSeptaFinishes (SeptaMotionWait);
	if (!status)
		return (990);
	if (status > SeptaMotionWait)
		return (250);
	return (0);
}


int             retractSeptaSerially ()
{
	int             status, waitTillSeptaFinishes ();

	char            resp[8];

	resp[0] = '\0';
	mcsSerialBus ("246R", resp);
	if (resp[0] == '\0')
		return (990);
	if ((strcmp ("N 1", resp) != 0) && (strcmp ("N 0", resp) != 0))
		return (250);
	sleep (2);				 /* wait till it gets started */
	status = waitTillSeptaFinishes (SeptaMotionWait);
	if (!status)
		return (990);
	if (status > SeptaMotionWait)
		return (250);
	return (0);
}

int             waitTillSeptaFinishes (numberOfLoops)
	int             numberOfLoops;
{
	int             retcode, mcsSerialBus (), i;

	char            resp[8], hdr[2];

	for (i = 1; i <= numberOfLoops; i++) {
		resp[0] = '\0';
		mcsSerialBus ("246S", resp);	 /* get status of septa */
		if (resp[0] == '\0')
			return (EcatMcsRpcTimeout);
		sscanf (resp, "%c%c%d", &hdr[0], &hdr[1], &retcode);
		if (((retcode & IsExtended) == IsExtended) ||
		    ((retcode & IsRetracted) == IsRetracted))
			break;
		sleep (1);
	}
	return (i);
}

int             reportGantryTilt (tiltPosition)
	float          *tiltPosition;
{
	bool_t          doAcsMcsCommand ();

	if (!(doAcsMcsCommand (REPORT_MOTIONS, xdr_void, (char *) NULL, xdr_MCS_STATUS, &mcsStatus)))
		return (EcatMcsRpcTimeout);
	if (mcsStatus.tilt_encoder > 767)
		mcsStatus.tilt_encoder -= 1024;
	*tiltPosition = mcsStatus.tilt_encoder * GANTRY_TILT_SCALE;
	return (TRUE);
}

int             reportGantryRotation (rotatePosition)
	float          *rotatePosition;
{
	bool_t          doAcsMcsCommand ();

	if (!(doAcsMcsCommand (REPORT_MOTIONS, xdr_void, (char *) NULL, xdr_MCS_STATUS, &mcsStatus)))
		return (EcatMcsRpcTimeout);
	if (mcsStatus.rot_encoder > 512)
		mcsStatus.rot_encoder -= 1024;
	*rotatePosition = mcsStatus.rot_encoder * GANTRY_ROTATE_SCALE;
	return (TRUE);

}

int             reportHorizontalBed (bedPosition)
	float          *bedPosition;
{
	bool_t          doAcsMcsCommand ();

	if (!(doAcsMcsCommand (REPORT_MOTIONS, xdr_void, (char *) NULL, xdr_MCS_STATUS, &mcsStatus)))
		return (EcatMcsRpcTimeout);
	*bedPosition = (mcsStatus.hb_encoder - mcsStatus.bed_ref_encoder) *
		((EcatModel == 921 || EcatModel == 961) ? 1.0 : HORIZONTAL_SCALE);
	return (TRUE);

}

int             reportVerticalBed (bedPosition)
	float          *bedPosition;
{
	bool_t          doAcsMcsCommand ();

	if (!(doAcsMcsCommand (REPORT_MOTIONS, xdr_void, (char *) NULL, xdr_MCS_STATUS, &mcsStatus)))
		return (EcatMcsRpcTimeout);
	*bedPosition = mcsStatus.vb_encoder * VERTICAL_SCALE;
	return (TRUE);

}

int             reportWobble (wobblePosition)
	float          *wobblePosition;
{
	bool_t          doAcsMcsCommand ();

	if (!(doAcsMcsCommand (REPORT_MOTIONS, xdr_void, (char *) NULL, xdr_MCS_STATUS, &mcsStatus)))
		return (EcatMcsRpcTimeout);
	*wobblePosition = mcsStatus.wob_encoder * DETECTOR_ORBIT_SCALE;
	return (TRUE);

}

int             reportSource (sourcePosition)
	float          *sourcePosition;
{
	bool_t          doAcsMcsCommand ();

	printf ("Use defaults_get_string\n");
/*
	if (strncmp(defaults_get_string("/Ecat/EcatTransmissionSource", "None", 0),
			"Rod", strlen("Rod")) == NULL)
		return (1001);
*/
	if (!(doAcsMcsCommand (REPORT_MOTIONS, xdr_void, (char *) NULL, xdr_MCS_STATUS, &mcsStatus)))
		return (EcatMcsRpcTimeout);
	*sourcePosition = mcsStatus.ps_encoder * SOURCE_ROTATE_SCALE;
	return (TRUE);

}

int             wobbleSpeedIs (wobbleSpeed)
	float          *wobbleSpeed;
{
	bool_t          doAcsMcsCommand ();

	if (!(doAcsMcsCommand (REPORT_MOTIONS, xdr_void, (char *) NULL, xdr_MCS_STATUS, &mcsStatus)))
		return (EcatMcsRpcTimeout);
	*wobbleSpeed = (mcsStatus.wob_speed * WOBBLE_SPEED_SCALE) / 127.0;
	return (TRUE);

}

int             sourceSpeedIs (sourceSpeed)
	float          *sourceSpeed;
{
	bool_t          doAcsMcsCommand ();

	printf ("Use defaults_get_string\n");
/*
	if (strncmp(defaults_get_string("/Ecat/EcatTransmissionSource", "None", 0),
			"Rod", strlen("Rod")) == NULL)
		return (1001);
*/
	if (!(doAcsMcsCommand (REPORT_MOTIONS, xdr_void, (char *) NULL, xdr_MCS_STATUS, &mcsStatus)))
		return (EcatMcsRpcTimeout);
	*sourceSpeed = (mcsStatus.ps_speed * ROTATE_SPEED_SCALE) / 127.0;
	return (TRUE);

}

int             reportRings (ringStatus)
	int            *ringStatus;
{
	bool_t          doAcsMcsCommand ();

	printf ("Use defaults_get_string\n");
/*
	if (strncmp(defaults_get_string("/Ecat/EcatTransmissionSource", "None", 0),
			"None", strlen("None")) == NULL)
		return (1002);
*/
	if (!(doAcsMcsCommand (REPORT_MOTIONS, xdr_void, (char *) NULL, xdr_MCS_STATUS, &mcsStatus)))
		return (EcatMcsRpcTimeout);
	*ringStatus = mcsStatus.rs_status;
	return (TRUE);

}

int             reportSwitchStatus (switchStatus)
	int            *switchStatus;
{
	bool_t          doAcsMcsCommand ();

	if (!(doAcsMcsCommand (REPORT_MOTIONS, xdr_void, (char *) NULL, xdr_MCS_STATUS, &mcsStatus)))
		return (EcatMcsRpcTimeout);
	*switchStatus = mcsStatus.mcs_inp;
	return (TRUE);

}

int             reportSeptaStatus (respString)
	char           *respString;
{
	bool_t          doAcsMcsCommand ();

	int             decodeSeptaStatusCode ();

	if (!(doAcsMcsCommand (REPORT_MOTIONS, xdr_void, (char *) NULL, xdr_MCS_STATUS, &mcsStatus)))
		return (EcatMcsRpcTimeout);
	if (EcatModel == 921 || EcatModel == 961) {
		if ((mcsStatus.septa_status & LastComandCompletedOK) == LastComandCompletedOK)
			mcsStatus.septa_status &= (IsExtended | IsRetracted |
						   IsExtendingOnA921 | IsRetractingOnA921);
		if ((mcsStatus.septa_status & IsExtendingOnA921) == IsExtendingOnA921)
			mcsStatus.septa_status = IsExtending;
		else if ((mcsStatus.septa_status & IsRetractingOnA921) == IsRetractingOnA921)
			mcsStatus.septa_status = IsRetracting;
	}
	decodeSeptaStatusCode (mcsStatus.septa_status, respString);
	return (mcsStatus.septa_status);

}

int             reportSeptaStatusSerially (respString)
	char           *respString;
{
	int             retcode, mcsSerialBus (), decodeSeptaStatusCode ();
	char            resp[80], hdr[2];

	resp[0] = '\0';
	retcode = mcsSerialBus ("246S", resp);
	if (resp[0] == '\0')
		return (EcatMcsRpcTimeout);
	sscanf (resp, "%c%c%d", &hdr[0], &hdr[1], &retcode);
	strcpy (respString, "unknown");
	if (EcatModel == 921 || EcatModel == 961) {
		if ((retcode & LastComandCompletedOK) == LastComandCompletedOK)
			retcode -= LastComandCompletedOK;
	}
	decodeSeptaStatusCode (retcode, respString);
	return (TRUE);
}

int             decodeSeptaStatusCode (septaStatus, respString)
	int             septaStatus;

	char           *respString;
{
	switch (septaStatus) {
	case IsExtended:
		strcpy (respString, "is extended");
		break;
	case IsRetracted:
		strcpy (respString, "is retracted");
		break;
	case IsExtending:
		strcpy (respString, "is extending");
		break;
	case IsRetracting:
		strcpy (respString, "is retracting");
		break;
	case 300:
		strcpy (respString, "is not present");
		break;
	case 990:
		strcpy (respString, "is not present");
		break;
	case 0:
		strcpy (respString, "is not retractable");
		break;
	}
	return (0);
}

int             initMcs ()
{
	int             resp;

	bool_t          doAcsMcsCommand ();

	if (!(doAcsMcsCommand (MCS_INIT, xdr_void, (char *) NULL, xdr_int, &resp)))
		return (EcatMcsRpcTimeout);
	return (resp);

}

int             powerOnMotions (motion)
	int             motion;
{
	int             resp, hack;

	bool_t          doAcsMcsCommand ();

	hack = motion;
	if (!(doAcsMcsCommand (MCS_PON, xdr_int, &hack, xdr_int, &resp)))
		return (EcatMcsRpcTimeout);
	return (resp);

}

int             powerOffMotions (motion)
	int             motion;
{
	int             resp, hack;

	bool_t          doAcsMcsCommand ();

	hack = motion;
	if (!(doAcsMcsCommand (MCS_POFF, xdr_int, &hack, xdr_int, &resp)))
		return (EcatMcsRpcTimeout);
	return (resp);

}

int             zeroMcs (zeroParms)
	MCS_ZEROES     *zeroParms;
{
	int             resp;

	bool_t          doAcsMcsCommand ();

	if (!(doAcsMcsCommand (MOTION_ZEROES, xdr_MCS_ZEROES, zeroParms, xdr_int, &resp)))
		return (EcatMcsRpcTimeout);
	return (resp);
}

int             mcsSerialBus (command, resp)
	char           *command, *resp;
{
	bool_t          doAcsMcsCommand ();

	return (doAcsMcsCommand (MCS_SERIAL, xdr_wrapstring, &command, xdr_wrapstring, &resp));
}

int             trSourcePoll (isItHere)
	int             isItHere;
{
	int             resp, trPresent = FALSE;

	bool_t          doAcsMcsCommand ();

	if (isItHere)
		trPresent = TRUE;
	if (!(doAcsMcsCommand (MCS_RING, xdr_int, &trPresent, xdr_int, &resp)))
		return (EcatMcsRpcTimeout);
	return (resp);
}

int             toggleMcsUpdate (toggleState)
	int             toggleState;
{
	int             resp, updateOn = FALSE;

	bool_t          doAcsMcsCommand ();

	if (toggleState)
		updateOn = TRUE;
	if (!(doAcsMcsCommand (MCS_UPDATE, xdr_int, &updateOn, xdr_int, &resp)))
		return (EcatMcsRpcTimeout);
	return (resp);
}

static char     temp[64];

char           *
                decode_ring_status (parm_1)
	int             parm_1;
{
	char           *decode921TransmissionStatus ();

	bzero (temp, 64);
	if (EcatModel == 921 || EcatModel == 961)
		return (decode921TransmissionStatus (parm_1));
	switch (parm_1) {
	default:
		sprintf (temp, "returned a status of %x\n", parm_1);
		return (temp);
	case IsLost:
		return ("does not know its present status\n");
	case IsExtended:
		return ("is in the extended position\n");
	case IsRetracted:
		return ("is in the retracted position\n");
	case ExtendFailed:
		return ("failed to extend\n");
	case RelaysStuck:
		return ("relays are stuck\n");
	case RetractFailed:
		return ("failed to retract\n");
	case IsExtending:
		return ("is extending\n");
	case IsRetracting:
		return ("is retracting\n");
	case 1002:
		return ("is non-existant\n");
	}
}

char           *
                decode921TransmissionStatus (transmissionSourceStatus)
	int             transmissionSourceStatus;
{
	if (transmissionSourceStatus == IsLost)
		strcat (temp, "does not know its present status\n");
	else {
		if ((transmissionSourceStatus & LastComandCompletedOK) == LastComandCompletedOK)
			strcat (temp, "completed its last command, ");
		if ((transmissionSourceStatus & IsExtended) == IsExtended)
			strcat (temp, "is in the extended position ");
		if ((transmissionSourceStatus & IsRetracted) == IsRetracted)
			strcat (temp, "is in the retracted position ");
		if ((transmissionSourceStatus & ExtendFailed) == ExtendFailed)
			strcat (temp, "failed to extend ");
		if ((transmissionSourceStatus & IsExtendingOnA921) == IsExtendingOnA921)
			strcat (temp, "is extending ");
		if ((transmissionSourceStatus & IsRetractingOnA921) == IsRetractingOnA921)
			strcat (temp, "is retracting ");
		if ((transmissionSourceStatus & RetractedKillSwitchHit) == RetractedKillSwitchHit)
			strcat (temp, "is on the retract limit switch ");
		strcat (temp, "\n");
	}
	return (temp);
}

char           *
                mcs_error (mcsErrorFlag)
	int             mcsErrorFlag;
{
	switch (mcsErrorFlag) {
	default:
		return ("MCS Error... requested could not be performed\n");
	case EcatMcsRpcTimeout:
		return ("MCS Error... call to talk to the ACS failed\n");
	case 1:
		return ("MCS Error...invalid command\n");
	case -1:
	case 15:
		return ("MCS Error...MCS communications timeout\n");
	case 10:
	case -4:
		return ("MCS Error...ECAT system in ESTOP\n");
	case 20:
		return ("MCS Error...Requested horizontal bed target position out of range\n");
	case 21:
		return ("MCS Error...Requested move rejected, previous move in progress\n");
	case 22:
		return ("MCS Error...Requested move rejected, bed is jogging\n");
	case 23:
		return ("MCS Error...Bed can not be extended, safety check\n");
	case 24:
		return ("MCS Error...Bed can not be extended, neuro limit reached\n");
	case 25:
		return ("MCS Error...Requested jog rejected, previous move in progress\n");
	case 26:
		return ("MCS Error...Bed can not be retracted, retract limit switch set\n");
	case 27:
		return ("MCS Error...Bed can not be extended, extend limit switch set\n");
	case 28:
		return ("MCS Error...Requested jog rejected, bed is jogging\n");
	case 29:
		return ("MCS Error...Move bed refused due to jog condition\n");
	case 30:
		return ("MCS Error...Requested vertical bed target position out of range\n");
	case 31:
		return ("MCS Error...Bed can not be lowered, lower limit switch set\n");
	case 32:
		return ("MCS Error...Requested move rejected, previous move in progress\n");
	case 33:
		return ("MCS Error...Requested lower rejected, bed not in safe move position\n");
	case 34:
		return ("MCS Error...Requested move rejected, tilt > 5 degrees\n");
	case 35:
		return ("MCS Error...Requested move rejected, tilt more than +/- 15 degrees\n");
	case 36:
		return ("MCS Error...Requested jog up rejected, bed is jogging down\n");
	case 37:
		return ("MCS Error...Requested jog down rejected, bed is jogging down\n");
	case 38:
		return ("MCS Error...Bed can not be raised, neuro limit reached\n");
	case 39:
		return ("MCS Error...Bed can not be raised, upper limit switch set\n");
	case 40:
		return ("MCS Error...Detector orbit is being jogged\n");
	case 41:
		return ("MCS Error...Detector orbit encoder lost\n");
	case 42:
		return ("MCS Error...Detector orbit not wobbling\n");
	case 43:
		return ("MCS Error...Detector orbit being moved to absolute position\n");
	case 44:
		return ("MCS Error...Wobble power is disabled\n");
	case 50:
		return ("MCS Error...Rotating source is being jogged\n");
	case 51:
		return ("MCS Error...Rotating source encoder lost\n");
	case 52:
		return ("MCS Error...Rotating source encoder is not changing\n");
	case 53:
		return ("MCS Error...Rotating source is being moved to absolute\n");
	case 54:
		return ("MCS Error...Rotating source power is disabled\n");
	case 55:
		return ("MCS Error...Source move command refused, source not zeroed\n");
	case 60:
		return ("MCS Error...Gantry tilt motion error\n");
	case 70:
		return ("MCS Error...Gantry rotate motion error\n");
	case 80:
		return ("MCS Error...Rotate wobble command refused due to prior motion\n");
	case 91:
		return ("MCS Error...Transmission source not extended\n");
	case 92:
		return ("MCS Error...Transmission source extend in progress\n");
	case 94:
		return ("MCS Error...Transmission source relays active\n");
	case 96:
		return ("MCS Error...Transmission source not retracted, can't extend\n");
	case 97:
		return ("MCS Error...Transmission source illegal jog \n");
	case 98:
		return ("MCS Error...Transmission source switch not disabled\n");
	case 99:
		return ("MCS Error...Transmission source in maintenance mode\n");
	case 100:
		return ("MCS Error...Transmission source not extending\n");
	case 101:
		return ("MCS Error...Transmission source not retracting\n");
	case 102:
		return ("MCS Error...Transmission source failed to extend\n");
	case 103:
		return ("MCS Error...Transmission source failed to retract\n");
	case 104:
		return ("MCS Error...Transmission source failed to move\n");
	case 200:
		return ("MCS Error...Illegal motion axis specified\n");
	case 201:
		return ("MCS Error...Illegal encoder specified\n");
	case 202:
		return ("MCS Error...Illegal speed requested\n");
	case 220:
		return ("MCS Error...Invalid move parameter specified\n");
	case 230:
		return ("MCS Error...Detector move refused due to a limit switch setting\n");
	case 250:
		return ("MCS Error...Septa move did not complete\n");
	case 300:
		return ("MCS Error...Communications timeout with the serial bus\n");
	case 301:
		return ("MCS Error...Communications timeout with the transmission source\n");
	case 302:
		return ("MCS Error...Communications timeout with the bed\n");
	case 330:
		return ("MCS Error...Move command stopped by ESTOP condition\n");
	case 400:
		return ("MCS Error...Invalid parameter for display switch\n");
	case 470:
		return ("MCS Error...Move terminated due to a limit switch setting\n");
	case 500:
		return ("MCS Error...Invalid parameter for display encoder\n");
	case 550:
		return ("MCS Error...Rotate command refused due to ESTOP condition\n");
	case 560:
		return ("MCS Error...Move command refused due to ESTOP condition\n");
	case 600:
		return ("MCS Error...Invalid parameter for report command\n");
	case 610:
		return ("MCS Error...Move command on collimator refused, jog in progress\n");
	case 620:
		return ("MCS Error...Move command on wobble refused due to jog condition\n");
	case 630:
		return ("MCS Error...Source rotate command refused due to prior motion\n");
	case 650:
		return ("MCS Error...Source move command refused due to jog condition\n");
	case 660:
		return ("MCS Error...Invalid parameter for zero command\n");
	case 680:
		return ("MCS Error...Cannot zero bed encoder while bed is moving\n");
	case 670:
		return ("MCS Error...Cannot zero encoder while motion axis is moving\n");
	case 720:
		return ("MCS Error...Invalid parameter for move bed\n");
	case 730:
		return ("MCS Error...Invalid parameter for move wobble\n");
	case 750:
		return ("MCS Error...Move bed refused due to minimum height requirement\n");
	case 760:
		return ("MCS Error...Invalid parameter for collimator move command\n");
	case 980:
		return ("MCS Error...Move bed refused, bed going into a set limit switch\n");
	case 990:
		return ("MCS Error...The motion did not respond to the firmware and timed out\n");
	case 1000:
		return ("MCS Error...MCS allocation error\n");
	case 1001:
		return ("MCS Error...machine does not have a plane source\n");
	case 1002:
		return ("MCS Error...machine does not have a retractible transmission source\n");
	}
}
