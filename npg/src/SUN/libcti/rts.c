/*$Id: rts.c,v 1.7 2002/11/27 20:08:37 tom Exp $*/
/*$Log: rts.c,v $
 *Revision 1.7  2002/11/27 20:08:37  tom
 **** empty log message ***
 *
 * Revision 1.6  1999/12/13  22:02:19  tom
 * partially cleaned up with lint
 *
 * Revision 1.5  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.4  1995/10/30  16:54:44  tom
 * changed petutil/matrix.h to matrix.h
 *
 * Revision 1.3  1994/03/03  17:37:48  ty7777
 * Change from id to header.
 *
 * Revision 1.2  1993/09/13  18:58:21  ty7777
 * Introduce acsid and change matrix.h to petutil/matrix.h.
 *
 * Revision 1.1  1993/03/04  19:57:39  ty7777
 * Initial revision
 **/

/*    @(#)rts.c	2.14    6/16/92    */
/* rts.c
* collection of modules to allow rts commands to be issued to the acquisition
* server as defined in the defaults database /Ecat/EcatAcqServer
*
* rts_conf (parm_1), rts_dpln (parm_1, parm_2, parm_3), rts_dfrm (parm_1, parm_2, parm_3, parm_4),
* rts_strt (parm_1, parm_2), rts_gantry (command, response), rts_rgan (response),
* rts_xfer (parm_1, parm_2, buffer), rts_wait (), check_rts_status (), rts_error (parm_1),
* rts_cont (), rts_ecom (parm_1), rts_sinm (parm_1), rts_reso (parm_1), rts_stop (),
* rts_fclr (parm_1) rts_dseg (parm_1, parm_2, parm_3, parm_4), rtsinfo (rtsInfoData),
* rts_modl (model, numberOfRings), rts_dfov (parm_1), rts_comp (parm_1), rts_rems (parm_1, parm_2),
* rts_cfrm (parm_1, parm_2, parm_3), rts_blim (parm_1, parm_2, parm_3),
* rts_sing (parm_1, parm_2, parm_3, parm_4, parm_5), rts_rsin (parm_1, cor_sing, uncor_sing),
* rts_beat (parm_1, beat_data), rts_gimi (parm_1, parm_2, parm_3, parm_4, gimidata),
* rts_rect (parm_1, parm_2, parm_3, parm_4, parm_5, parm_6, gimidata),
* rtsGetem (fileName, frame, gate, plane, datatype, matrix_number), rtsFileName (fileToCreate, fileSize),
* rtsRblk (fileName, blockNumber, buffer), rtsWblk (fileName, blockNumber, buffer), rts_acqm (acqMode),
* rts_dpln6 (logPlane, phyPlane0, phyPlane1, phyPlane2, phyPlane3, phyPlane4, phyPlane5, phyPlane6)
* rts_rrdb (enableOrDisable), rts_rwin (enableOrDisable)
* t.w. jansto		29 nov 1990
*/

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/rts.c,v 1.7 2002/11/27 20:08:37 tom Exp $";
#endif

#define ErrorIssuingRpcCall -2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <rpc/rpc.h>
#include "ecatRts.h"
#include "ecatAcs.h"
#include <matrix.h>

int             rtsMagpie (squawk)
	int             squawk;
{
	int             resp;

	bool_t          doAcsAcqCommand ();

	if (!(doAcsAcqCommand (DBUG, xdr_int, &squawk, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	return (resp);
}

int             rts_conf (clearOrConfig)
	int             clearOrConfig;
{
/*
	int resp;
	float strtod ();
	bool_t doAcsAcqCommand();
	CONF_args confArgs;

	confArgs.config = clearOrConfig;
	confArgs.pile_up_factor = strtod(
		(char *) defaults_get_string("/Ecat/EcatPu", "8.6e-8", 0), (char *) NULL);
	confArgs.plane_factor = strtod(
		(char *) defaults_get_string("/Ecat/EcatPl", "1.0", 0), (char *) NULL);
	if (!(doAcsAcqCommand(CONF, xdr_CONF_args, &confArgs, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	if (!clearOrConfig)
		resetFrameTransferred();
	return (resp);
*/
	return (1);
}

int             rts_acqm (acquisitionMode)
	int             acquisitionMode;
{
	int             resp;

	bool_t          doAcsAcqCommand ();

	if (!(doAcsAcqCommand (ACQM, xdr_int, &acquisitionMode, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	return (resp);
}

int             rts_dpln (logPlane, phyPlane0, phyPlane1, phyPlane2, phyPlane3, phyPlane4, phyPlane5)
	int             logPlane, phyPlane0, phyPlane1, phyPlane2, phyPlane3, phyPlane4, phyPlane5;
{
	int             resp;

	DPLN_args       dplnCmd;

	bool_t          doAcsAcqCommand ();

	dplnCmd.log_plane = logPlane;
	dplnCmd.phy_plane[0] = phyPlane0;
	dplnCmd.phy_plane[1] = phyPlane1;
	dplnCmd.phy_plane[2] = phyPlane2;
	dplnCmd.phy_plane[3] = phyPlane3;
	dplnCmd.phy_plane[4] = phyPlane4;
	dplnCmd.phy_plane[5] = phyPlane5;
	if (!(doAcsAcqCommand (DPLN, xdr_DPLN_args, &dplnCmd, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	return (resp);
}

int             rts_dfrm (nFrames, delay, duration, nsegs)
	int             nFrames, delay, duration, nsegs;
{
	int             resp;

	DFRM_args       dfrmCmd;

	bool_t          doAcsAcqCommand ();

	dfrmCmd.nframes = nFrames;
	dfrmCmd.delay = delay;
	dfrmCmd.duration = duration;
	dfrmCmd.nsegs = nsegs;
	if (!(doAcsAcqCommand (DFRM, xdr_DFRM_args, &dfrmCmd, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	return (resp);
}

int             rts_strt (framesToDo, acqType)
	int             framesToDo, acqType;
{
	int             resp;

	STRT_args       strtCmd;

	bool_t          doAcsAcqCommand ();

	strtCmd.numberFrames = framesToDo;
	strtCmd.acqType = acqType;
	if (!(doAcsAcqCommand (STRT, xdr_STRT_args, &strtCmd, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	return (resp);
}

int             rts_gantry (command, response)
	char           *command, *response;
{
	bool_t          doAcsAcqCommand (), status;

	char            temp_response[80], *temp;

	temp = temp_response;
	if (getenv ("NoSerialBus") == (char *) NULL) {	/* bypass all serial bus activity */
		status = doAcsAcqCommand (GANT, xdr_wrapstring, &command, xdr_wrapstring, &temp);
		sprintf (response, "%s", temp);
	} else {
		status = 1;
		strcpy (response, "N 0");
	}
	return (status);
}

int             rts_rgan (response)
	char           *response;
{
	bool_t          doAcsAcqCommand ();

	if (getenv ("NoSerialBus") == (char *) NULL)	/* bypass all serial bus activity */
		return (doAcsAcqCommand (RGAN, xdr_void, (char *) NULL, xdr_wrapstring, &response));
	else {
		strcpy (response, "0, 0, 0, 0, 0, 0, 0, 0");
		return (1);
	}
}

int             rts_gantry_controller (command, response)
	char           *command, *response;
{
	bool_t          doAcsAcqCommand ();

	if (getenv ("NoSerialBus") == (char *) NULL)	/* bypass all serial bus activity */
		return (doAcsAcqCommand (GCTL, xdr_wrapstring, &command, xdr_wrapstring, &response));
	else {
		strcpy (response, "N 0");
		return (1);
	}
}

int             rts_stop ()
{
	int             resp;

	bool_t          doAcsAcqCommand ();

	if (!(doAcsAcqCommand (STOP, xdr_void, (char *) NULL, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	return (resp);
}

int             rts_sinm (sinoMode)
	int             sinoMode;
{
	int             resp;

	bool_t          doAcsAcqCommand ();

	if (!(doAcsAcqCommand (SINM, xdr_int, &sinoMode, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	return (resp);
}

int             rts_reso (stationaryOrWobbled)
	int             stationaryOrWobbled;
{
	int             resp;

	bool_t          doAcsAcqCommand ();

	if (!(doAcsAcqCommand (RESO, xdr_int, &stationaryOrWobbled, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	return (resp);
}

int             rts_fclr (clearThisLogicalFrame)
	int             clearThisLogicalFrame;
{
	int             resp;

	bool_t          doAcsAcqCommand ();

	if (!(doAcsAcqCommand (FCLR, xdr_int, &clearThisLogicalFrame, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	return (resp);
}

int             createAMatrixFile (fileName, main, dFOV)
	char           *fileName;

	Main_header    *main;

	int             dFOV;
{
	int             resp, determineDataSize ();

	bool_t          doAcsAcqCommand ();

	CREF_args       makeMe;

	bcopy (main, &makeMe.mhead, sizeof (XMAIN_HEAD));
	makeMe.file_name = (char *) calloc (1, 64);
	sprintf (makeMe.file_name, "%s", fileName);
	printf ("Use defaults_get_integer\n");
/*
	makeMe.mhead.system_type = (int) defaults_get_integer("/Ecat/EcatModel", 951, 0);
*/
	if ((makeMe.data_size = determineDataSize (makeMe.mhead.system_type, dFOV,
					 makeMe.mhead.compression_code, makeMe.mhead.wobble_speed,
						   makeMe.mhead.data_type)) == 0) {
		cfree (makeMe.file_name);
		return (1);
	}
	if (!(doAcsAcqCommand (CREF, xdr_CREF_args, &makeMe, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	cfree (makeMe.file_name);
	return (resp);
}

MatrixFile     *createMatrixFile (fileName, main, dFOV)
	char           *fileName;
	Main_header    *main;
	int             dFOV;
{
	MatrixFile     *mFile, *matrix_open (), *matrix_create ();

	if (isThisAnACSFile (fileName)) {
		if (createAMatrixFile (fileName, main, dFOV) != OK)
			return ((MatrixFile *) NULL);
		if ((mFile = matrix_open (fileName, "r+")) == NULL) {
			fprintf (stderr,
				 "createMatrixFile:ERROR...could not open file %s for writing\n",
				 fileName);
			return ((MatrixFile *) NULL);
		}
	} else {				 /* creating a NFS file, so mat_enter will create
						  * only the directories needed */
		printf ("Use defaults_get_integer\n");
/*
		main->system_type = (int) defaults_get_integer("/Ecat/EcatModel", 951, 0);
*/
		if ((mFile = matrix_create (fileName, main)) == (MatrixFile *) NULL) {
			fprintf (stderr,
			    "createMatrixFile:ERROR... Unable to create new file %s\n", fileName);
			return ((MatrixFile *) NULL);
		}
	}
	return (mFile);
}

int             determineDataSize (model, dFOV, mashValue, wobbleFlag, dataType)
	int             model, dFOV, mashValue, wobbleFlag, dataType;
{
	int             power (), dataSize, defaultFOVWidth, defaultAngles;

	switch (model) {			 /* check if mashed or field of view is not
						  * standard */
	case 931:
	case 951:
		defaultFOVWidth = 192;
		defaultAngles = 256;
		break;
	case 933:
	case 953:
	case 921:
		defaultFOVWidth = 160;
		defaultAngles = 192;
		break;
	case 961:
		defaultFOVWidth = 336;
		defaultAngles = 392;
		break;
	}
	dataSize = ((dFOV == 0) ? defaultFOVWidth : dFOV) *
		(defaultAngles / power (2, mashValue)) * 2;
	dataSize /= 512;
	if (wobbleFlag)				 /* check for wobble */
		dataSize *= 4;
	if (dataType == VAX_R4 || dataType == IEEE_R4 ||	/* check if float data */
	    dataType == M68K_I4 || dataType == VAX_I4)
		dataSize *= 2;
	return (dataSize);
}

int             preallocateFileSpace (fileName, main, fileSize)
	char           *fileName;

	Main_header    *main;

	int             fileSize;
{
	FILE           *mPtr, *mat_create ();

	if ((mPtr = mat_create (fileName, main)) == NULL)
		return (1);
	mat_close (mPtr);

	if (truncate (fileName, fileSize < 0))
		return (1);
	return (0);
}

int             rtsGetem (frame, gate, plane, data, bed, gimiData)
	int             frame, plane, gate, data, bed;

	GIMI_resp      *gimiData;
{
	int             resp;

	GIMI_args       gimiCmd;

	bool_t          doAcsAcqCommand ();

	gimiCmd.frame = frame;
	gimiCmd.segment = gate;
	gimiCmd.plane = plane;
	gimiCmd.data = data;
	gimiCmd.bed = bed;
	resp = doAcsAcqCommand (GIMI, xdr_GIMI_args, &gimiCmd, xdr_GIMI_resp, gimiData);
	if (!resp)
		return (TRUE);
	else
		return (gimiData->status);
}

int             rtsGlog (scan, frame, gate, plane, data, bed, glogData)
	char           *scan;

	int             frame, plane, gate, data, bed;

	GLOG_resp      *glogData;
{
	int             resp;

	GLOG_args       glogCmd;

	bool_t          doAcsAcqCommand ();

	glogCmd.filename = scan;
	glogCmd.frame = frame;
	glogCmd.segment = gate;
	glogCmd.plane = plane;
	glogCmd.data = data;
	glogCmd.bed = bed;
	resp = doAcsAcqCommand (GLOG, xdr_GLOG_args, &glogCmd, xdr_GLOG_resp, glogData);
	if (resp == 0 || glogData->status != 0)
		return (TRUE);
	return (FALSE);
}

int             rts_stor (frame, matFrame, bed, scan)
	int             frame, matFrame, bed;

	char           *scan;
{
	int             resp;

	STOR_args       storCmd;

	bool_t          doAcsAcqCommand ();

	storCmd.frame = frame;
	storCmd.matframe = matFrame;
	storCmd.bed = bed;
	storCmd.filename = scan;
	if (!(doAcsAcqCommand (STOR, xdr_STOR_args, &storCmd, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	return (resp);
}

rts_sing (startBucket, endBucket, pollInterval, timeOut, numIpcs)
	int             startBucket, endBucket, pollInterval, timeOut, numIpcs;
{
	int             resp;

	SNGL_args       singCmd;

	bool_t          doAcsAcqCommand ();

	singCmd.start_bucket = startBucket;
	singCmd.end_bucket = endBucket;
	singCmd.frequency = pollInterval;
	singCmd.timeout = timeOut;
	singCmd.num_ipcs = numIpcs;
	if (getenv ("NoSerialBus") == (char *) NULL)
		if (!(doAcsAcqCommand (SNGL, xdr_SNGL_args, &singCmd, xdr_int, &resp)))
			resp = ErrorIssuingRpcCall;
		else
			resp = 0;		 /* bypass all serial bus activity */
	return (resp);
}


int             rts_modl (modelType, numberOfRings)
	int             modelType, numberOfRings;
{
	int             resp;

	MODL_args       modelArgs;

	bool_t          doAcsAcqCommand ();

	modelArgs.model = modelType;
	modelArgs.number_of_rings = numberOfRings;
	if (!(doAcsAcqCommand (MODL, xdr_MODL_args, &modelArgs, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	return (resp);
}

int             rts_rems (numberOfFrames, acqType)
	int             numberOfFrames, acqType;
{
	int             resp;

	STRT_args       remsCmd;

	bool_t          doAcsAcqCommand ();

	remsCmd.numberFrames = numberOfFrames;
	remsCmd.acqType = acqType;
	if (!(doAcsAcqCommand (REMS, xdr_STRT_args, &remsCmd, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	return (resp);
}

int             rts_astr (storeFile, storeIt)
	char           *storeFile;

	int             storeIt;
{
	int             resp;

	bool_t          doAcsAcqCommand ();

	ASTR_args       astrArgs;

	astrArgs.file_name = storeFile;
	astrArgs.auto_store = storeIt;
	if (!(doAcsAcqCommand (ASTR, xdr_ASTR_args, &astrArgs, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	return (resp);
}

int             rts_thrt (throttleValue)
	int             throttleValue;
{
	int             resp;

	bool_t          doAcsAcqCommand ();

	if (!(doAcsAcqCommand (THRT, xdr_int, &throttleValue, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	return (resp);
}

int             rtsinfo (rtsInfo)
	INFO_resp      *rtsInfo;
{
	bool_t          doAcsAcqCommand ();

	return (doAcsAcqCommand (INFO, xdr_void, (char *) NULL, xdr_INFO_resp, rtsInfo));
}

bool_t
whatTimeYaGot (now)
	TIMEX_resp     *now;
{
	bool_t          doAcsAcsCommand ();

	return (doAcsAcsCommand (ACS_WHAT_TIME_IS_IT, xdr_void, (char *) NULL, xdr_TIMEX_resp, now));
}

int             check_rts_status ()
{
	INFO_resp       rtsInfo;
	bool_t          doAcsAcqCommand ();

	if (!(doAcsAcqCommand (INFO, xdr_void, (char *) NULL, xdr_INFO_resp, rtsInfo)))
		rtsInfo.acq_status = FALSE;
	return (rtsInfo.acq_status);
}

int             rts_cont ()
{
	int             resp;
	bool_t          doAcsAcqCommand ();

	if (!(doAcsAcqCommand (CONT, xdr_void, (char *) NULL, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	return (resp);
}

int             rts_mash (mashValue)
	int             mashValue;
{
	int             resp;

	bool_t          doAcsAcqCommand ();

	if (!(doAcsAcqCommand (MASH, xdr_int, &mashValue, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	return (resp);
}

int             rts_dfov (nElems)
	int             nElems;
{
	int             resp;

	bool_t          doAcsAcqCommand ();

	if (!(doAcsAcqCommand (DFOV, xdr_int, &nElems, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	return (resp);
}

int             rts_rwin (enableOrDisable)
	int             enableOrDisable;
{
	int             resp;

	bool_t          doAcsAcqCommand ();

	if (!(doAcsAcqCommand (RWIN, xdr_int, &enableOrDisable, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	return (resp);
}

int             rts_rrdb (enableOrDisable)
	int             enableOrDisable;
{
	int             resp;

	bool_t          doAcsAcqCommand ();

	if (!(doAcsAcqCommand (RRDB, xdr_int, &enableOrDisable, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	return (resp);
}

int             rts_fdel (fileName)
	char           *fileName;
{
	int             resp;

	bool_t          doAcsAcqCommand ();

	if (!(doAcsAcqCommand (FDEL, xdr_wrapstring, &fileName, xdr_int, &resp)))
		resp = ErrorIssuingRpcCall;
	return (resp);
}

int             rts_rect (file, frame, plane, gate, data, startView, endView, rectData)
	char           *file;

	int             frame, plane, gate, data, startView, endView;

	short int      *rectData;
{
	bool_t          doAcsAcqCommand ();

	RDAT_args       rdArgs;

	RDAT_resp       rdResp;

	char            sinoHeader[512];

	Scan_subheader  sH;

	int             i, j, index, matnum, mat_numcod ();

	matnum = mat_numcod (frame, plane, gate, data, 0);
	if (rts_rshd (file, matnum, sinoHeader) < 0)
		return (-1);
	convertScanHeaderFromVax (sinoHeader, &sH);
	rdArgs.file_name = file;
	rdArgs.matnum = matnum;
	rdResp.rdat.rdat_val = (char *) calloc (1, sH.dimension_1 * sH.dimension_2 * sizeof (short));
	if (!(doAcsAcqCommand (RDAT, xdr_RDAT_args, &rdArgs, xdr_RDAT_resp, &rdResp))) {
		cfree (rdResp.rdat.rdat_val);
		return (-1);
	}
	for (i = 0; i < sH.dimension_2; i++) {
		for (j = 0; j < sH.dimension_1; j++) {
			index = j * sH.dimension_2 + i;
			if (i > startView && i <= endView)
				rectData[index] = (short int) rdResp.rdat.rdat_val[2 * index];
			else
				rectData[index] = 0;
		}
	}
	cfree (rdResp.rdat.rdat_val);
	return (rdResp.status);
}

int             rts_rmhd (file, mh)
	char           *file;

	Main_header    *mh;
{
	RMHD_resp       rdResp;

	struct stat     fStats;

	FILE           *fptr, *mat_open ();

	int             mat_read_main_header ();

	if (!stat (file, &fStats)) {
		if ((fptr = mat_open (file, "r")) == (FILE *) NULL) {
			fprintf (stderr, "rts_rmhd:ERROR...Unable to mat_open file %s\n", file);
			return (-1);
		}
		if (mat_read_main_header (fptr, mh)) {
			fprintf (stderr, "rts_rmhd:ERROR...Unable to read main header of file %s\n", file);
			mat_close (fptr);
			return (-1);
		}
		mat_close (fptr);
		return (0);
	} else {
		if (!(doAcsAcqCommand (RMHD, xdr_wrapstring, &file, xdr_RMHD_resp, &rdResp)))
			return (-1);
		bcopy (&rdResp.xmain_head, mh, sizeof (XMAIN_HEAD));
		return (rdResp.status);
	}
}

int             rts_wmhd (file, mh)
	char           *file;

	XMAIN_HEAD     *mh;
{
	WMHD_args       wrArgs;

	int             resp, mat_write_main_header ();

	struct stat     fStats;

	FILE           *fptr, *mat_open ();

	if (!stat (file, &fStats)) {
		if ((fptr = mat_open (file, "r+")) == (FILE *) NULL) {
			fprintf (stderr, "rts_wmhd:ERROR...Unable to mat_open file %s\n", file);
			return (-1);
		}
		if (mat_write_main_header (fptr, mh)) {
			fprintf (stderr,
			       "rts_wmhd:ERROR...Unable to write main header of file %s\n", file);
			mat_close (fptr);
			return (-1);
		}
		mat_close (fptr);
		return (0);
	} else {
		wrArgs.file_name = file;
		bcopy (mh, &wrArgs.xmain_head, sizeof (XMAIN_HEAD));
		if (!(doAcsAcqCommand (WMHD, xdr_WMHD_args, &wrArgs, xdr_int, &resp)))
			resp = -1;
		return (resp);
	}
}

int             rts_rshd (file, matnum, buffer)
	char           *file;

	int             matnum;

	caddr_t         buffer;
{
	RSHD_args       rdArgs;

	RSHD_resp       rdResp;

	int             mat_read_main_header (), mat_lookup (), mat_read_norm_subheader ();

	int             mat_read_scan_subheader (), mat_read_image_subheader (), mat_read_attn_subheader ();

	struct stat     fStats;

	FILE           *fptr, *mat_open ();

	Main_header     mH;

	Scan_subheader  scan;

	Image_subheader image;

	Attn_subheader  attn;

	Norm_subheader  norm;

	struct MatDir   dirEntry;

	if (!stat (file, &fStats)) {
		if ((fptr = mat_open (file, "r")) == (FILE *) NULL) {
			fprintf (stderr, "rts_rshd:ERROR...Unable to mat_open file %s\n", file);
			return (-1);
		}
		if (mat_read_main_header (fptr, &mH)) {
			fprintf (stderr,
				 "rts_rshd:ERROR...Unable to read main header of file %s\n", file);
			mat_close (fptr);
			return (-1);
		}
		if (!mat_lookup (fptr, matnum, &dirEntry)) {
			fprintf (stderr,
				 "rts_rshd:ERROR...Unable to lookup matrix 0x%08x of file %s\n",
				 matnum, file);
			mat_close (fptr);
			return (-1);
		}
		switch (mH.file_type) {
		case RAW_DATA:
			if (mat_read_scan_subheader (fptr, dirEntry.strtblk, &scan)) {
				fprintf (stderr,
				   "rts_rshd:ERROR...Unable to read scan subheader for file %s\n",
					 file);
				mat_close (fptr);
				return (-1);
			}
			convertScanHeaderToVax (buffer, &scan);
			break;
		case IMAGE_DATA:
			if (mat_read_image_subheader (fptr, dirEntry.strtblk, &image)) {
				fprintf (stderr,
				  "rts_rshd:ERROR...Unable to read image subheader for file %s\n",
					 file);
				mat_close (fptr);
				return (-1);
			}
			convertImageHeaderToVax (buffer, &image);
			break;
		case ATTN_DATA:
			if (mat_read_attn_subheader (fptr, dirEntry.strtblk, &attn)) {
				fprintf (stderr,
				   "rts_rshd:ERROR...Unable to read attn subheader for file %s\n",
					 file);
				mat_close (fptr);
				return (-1);
			}
			convertAttnHeaderToVax (buffer, &attn);
			break;
		case NORM_DATA:
			if (mat_read_norm_subheader (fptr, dirEntry.strtblk, &norm)) {
				fprintf (stderr,
				   "rts_rshd:ERROR...Unable to read norm subheader for file %s\n",
					 file);
				mat_close (fptr);
				return (-1);
			}
			convertNormHeaderToVax (buffer, &norm);
			break;
		}
		mat_close (fptr);
		return (0);
	} else {
		rdArgs.file_name = file;
		rdArgs.matnum = matnum;
		if (!(doAcsAcqCommand (RSHD, xdr_RSHD_args, &rdArgs, xdr_RSHD_resp, &rdResp)))
			return (-1);
		bcopy (rdResp.rhdat, buffer, 512);
		return (rdResp.status);
	}
}

int             rts_wshd (file, matnum, buffer)
	char           *file;

	int             matnum;

	caddr_t         buffer;
{
	WSHD_args       wrArgs;

	int             resp, mat_read_main_header (), mat_lookup (), mat_write_norm_subheader ();

	int             mat_write_scan_subheader (), mat_write_image_subheader (), mat_write_attn_subheader ();

	struct stat     fStats;

	FILE           *fptr, *mat_open ();

	Main_header     mH;

	Scan_subheader  scan;

	Image_subheader image;

	Attn_subheader  attn;

	Norm_subheader  norm;

	struct MatDir   dirEntry;

	if (!stat (file, &fStats)) {
		if ((fptr = mat_open (file, "r+")) == (FILE *) NULL) {
			fprintf (stderr, "rts_rshd:ERROR...Unable to mat_open file %s\n", file);
			return (-1);
		}
		if (mat_read_main_header (fptr, &mH)) {
			fprintf (stderr, "rts_rshd:ERROR...Unable to read main header of file %s\n", file);
			mat_close (fptr);
			return (-1);
		}
		if (!mat_lookup (fptr, matnum, &dirEntry)) {
			fprintf (stderr, "rts_rshd:ERROR...Unable to lookup matrix 0x%08x of file %s\n",
				 matnum, file);
			mat_close (fptr);
			return (-1);
		}
		switch (mH.file_type) {
		case RAW_DATA:
			convertScanHeaderFromVax (buffer, &scan);
			if (mat_write_scan_subheader (fptr, dirEntry.strtblk, &scan)) {
				fprintf (stderr,
				  "rts_rshd:ERROR...Unable to write scan subheader for file %s\n",
					 file);
				mat_close (fptr);
				return (-1);
			}
			break;
		case IMAGE_DATA:
			convertImageHeaderFromVax (buffer, &image);
			if (mat_write_image_subheader (fptr, dirEntry.strtblk, &image)) {
				fprintf (stderr,
				"rts_rshd:ERROR... Unable to write image subheader for file %s\n",
					 file);
				mat_close (fptr);
				return (-1);
			}
			break;
		case ATTN_DATA:
			convertAttnHeaderFromVax (buffer, &attn);
			if (mat_write_attn_subheader (fptr, dirEntry.strtblk, &attn)) {
				fprintf (stderr,
				 "rts_rshd:ERROR... Unable to write attn subheader for file %s\n",
					 file);
				mat_close (fptr);
				return (-1);
			}
			break;
		case NORM_DATA:
			convertNormHeaderFromVax (buffer, &norm);
			if (mat_write_norm_subheader (fptr, dirEntry.strtblk, &norm)) {
				fprintf (stderr,
				 "rts_rshd:ERROR... Unable to write norm subheader for file %s\n",
					 file);
				mat_close (fptr);
				return (-1);
			}
			break;
		}
		mat_close (fptr);
		return (0);
	} else {
		wrArgs.file_name = file;
		wrArgs.matnum = matnum;
		bcopy (buffer, wrArgs.whdat, 512);
		if (!(doAcsAcqCommand (WSHD, xdr_WSHD_args, &wrArgs, xdr_int, &resp)))
			return (-1);
		return (resp);
	}
}

int             rts_rdat (file, matnum, buffer, bufferSize)
	char           *file;

	int             matnum, *bufferSize;

	caddr_t         buffer;
{
	RDAT_args       rdArgs;

	RDAT_resp       rdResp;

	int             mat_read_main_header (), mat_lookup (), mat_read_mat_data ();

	struct stat     fStats;

	FILE           *fptr, *mat_open ();

	Main_header     mH;

	struct MatDir   dirEntry;

	if (!stat (file, &fStats)) {
		if ((fptr = mat_open (file, "r")) == (FILE *) NULL) {
			fprintf (stderr, "rts_rdat:ERROR... Unable to mat_open file %s\n", file);
			return (-1);
		}
		if (mat_read_main_header (fptr, &mH)) {
			fprintf (stderr, "rts_rdat:ERROR... Unable to read main header of file %s\n",
				 file);
			mat_close (fptr);
			return (-1);
		}
		if (!mat_lookup (fptr, matnum, &dirEntry)) {
			fprintf (stderr, "rts_rdat:ERROR... Unable to lookup matrix 0x%08x of file %s\n",
				 matnum, file);
			mat_close (fptr);
			return (-1);
		}
		if (mat_read_mat_data (fptr, dirEntry.strtblk + 1,
				       dirEntry.endblk - dirEntry.strtblk, buffer, mH.data_type)) {
			fprintf (stderr, "rts_rdat:ERROR... Unable to read matrix data for file %s\n",
				 file);
			mat_close (fptr);
			return (-1);
		}
		*bufferSize = (dirEntry.endblk - dirEntry.strtblk) * MatBLKSIZE;
		mat_close (fptr);
		return (0);
	} else {
		rdArgs.file_name = file;
		rdArgs.matnum = matnum;
		rdResp.rdat.rdat_val = (char *) buffer;
		if (!(doAcsAcqCommand (RDAT, xdr_RDAT_args, &rdArgs, xdr_RDAT_resp, &rdResp)))
			return (-1);
		*bufferSize = rdResp.rdat.rdat_len;
		return (rdResp.status);
	}
}

int             rts_wdat (file, matnum, dataType, buffer, bufferSize)
	char           *file;
	int             matnum, dataType, bufferSize;
	caddr_t         buffer;
{
	WDAT_args       wrArgs;
	int             mat_read_main_header (), mat_lookup (), resp;
	struct stat     fStats;
	FILE           *fptr, *mat_open ();
	Main_header     mH;
	struct MatDir   dirEntry;

	if (!stat (file, &fStats)) {
		if ((fptr = mat_open (file, "r+")) == (FILE *) NULL) {
			fprintf (stderr, "rts_wdat:ERROR... Unable to mat_open file %s\n", file);
			return (-1);
		}
		if (mat_read_main_header (fptr, &mH)) {
			fprintf (stderr, "rts_wdat:ERROR...Unable to read main header of file %s\n", file);
			mat_close (fptr);
			return (-1);
		}
		if (!mat_lookup (fptr, matnum, &dirEntry)) {
			fprintf (stderr, "rts_wdat:ERROR...Unable to lookup matrix 0x%08x of file %s\n",
				 matnum, file);
			mat_close (fptr);
			return (-1);
		}
		if (write_matrix_data (fptr, dirEntry.strtblk + 1,
				       dirEntry.endblk - dirEntry.strtblk, buffer, mH.data_type)) {
			fprintf (stderr, "rts_wdat:ERROR...Unable to write matrix data for file %s\n",
				 file);
			mat_close (fptr);
			return (-1);
		}
		mat_close (fptr);
		return (0);
	} else {
		wrArgs.file_name = file;
		wrArgs.matnum = matnum;
		wrArgs.data_type = dataType;
		wrArgs.wdat.wdat_len = bufferSize;
		wrArgs.wdat.wdat_val = buffer;
		if (!(doAcsAcqCommand (WDAT, xdr_WDAT_args, &wrArgs, xdr_int, &resp)))
			resp = -1;
		return (resp);
	}
}

int             rtsRblk (file, blockNumber, buffer)
	char           *file;

	int             blockNumber;

	caddr_t         buffer;
{
	RBLK_args       rbArgs;

	RBLK_resp       rbResp;

	rbArgs.filename = file;
	rbArgs.block_number = blockNumber;
	if (!(doAcsAcqCommand (RBLK, xdr_RBLK_args, &rbArgs, xdr_RBLK_resp, &rbResp)))
		return (-1);
	bcopy (rbResp.rblk, buffer, 512);
	return (rbResp.status);
}

int             rtsWblk (file, blockNumber, buffer)
	char           *file;

	int             blockNumber;

	caddr_t         buffer;
{
	WBLK_args       wbArgs;

	int             resp;

	wbArgs.filename = file;
	wbArgs.block_number = blockNumber;
	bcopy (buffer, wbArgs.wblk, 512);
	if (!(doAcsAcqCommand (WBLK, xdr_WBLK_args, &wbArgs, xdr_int, &resp)))
		return (-1);
	return (resp);
}

void            fillInMainHeader (mh, name)
	Main_header    *mh;

	char           *name;
{
/* Tom Yang on 1/28/93.
	char *acqServer;

	acqServer = defaults_get_string("/Ecat/EcatRTSCpu", "Mizar", 0);
	mh->file_type = 1;
	mh->system_type = (int) defaults_get_integer("/Ecat/EcatModel", 951, 0);
	mh->data_type = strcmp(acqServer, "ACS") ? VAX_I2 : M68K_I2;
	sprintf(mh->original_file_name, "%s", name);
	mh->original_file_name[19] = 0;
	sprintf(mh->node_id, "%s", "dummy");
	strcpy(mh->isotope_code, "Ge-68");
	mh->isotope_halflife = 23760000.0;
	sprintf(mh->radiopharmaceutical, "%s", "dummy");
	sprintf(mh->study_name, "%s", "dummy");
	sprintf(mh->patient_id, "%s", " ");
	sprintf(mh->patient_name, "%s", "dummy");
	mh->patient_sex = 'm';
	sprintf(mh->patient_age, "%s", "0");
	sprintf(mh->patient_weight, "%s", "0");
	sprintf(mh->patient_height, "%s", "0");
	mh->patient_dexterity = 'l';
	sprintf(mh->physician_name, "%s", "Dr. No");
	sprintf(mh->operator_name, "%s", "dummy");
	sprintf(mh->study_description, "%s", "dummy");
	sprintf(mh->facility_name, "%s", "dummy");
	sprintf(mh->user_process_code, "%s", " ");
*/
	return;
}

int             singlesOn ()
{
/*
	int numBuckets,
	    numIPCs = 1;

	numBuckets = defaults_get_integer("/Ecat/EcatNumBuckets", 32, 0) - 1;
	if (defaults_get_integer("/Ecat/EcatNumRings", 2, 0) == 1)
	if (1)
		numIPCs = 1;
	else
		numIPCs = 4;
	if (defaults_get_integer("/Ecat/EcatModel", 951, 0) == 921 ||
		defaults_get_integer("/Ecat/EcatModel", 951, 0) == 961)
		numIPCs = 1;
	return (rts_sing(0, numBuckets, 1000, 2000, numIPCs));
*/
	return (1);
}

int             singlesOff ()
{
	return (rts_sing (0, 0, 0, 2000, 0));
}

int             talkToRtsInfo (infoCommand)
	int             infoCommand;
{

	/*
	 * Tom Yang on 1/28/93. struct sockaddr_in addr; struct hostent *hp; int sock, result, i =
	 * 0, pid, retVal; struct timeval timeout; CLIENT *client;
	 * 
	 * bzero((char *) &addr, sizeof(addr)); if ((hp = gethostbyname("localhost")) == NULL) {
	 * fprintf(stderr, "gethostbyname failed\n"); return (-1); } for (pid = 0, i = 0; i < 2;
	 * i++) { bcopy(hp->h_addr, (caddr_t) & addr.sin_addr, hp->h_length); addr.sin_family =
	 * AF_INET; addr.sin_port = 0; sock = RPC_ANYSOCK; if ((client = clnttcp_create(&addr,
	 * RTS_INFO_SERVER, RTS_INFO_VERSION, &sock, 0, 0)) == NULL) { if (!(pid = vfork()))
	 * execlp("rtsinfo", "rtsinfo", 0); sleep(5); } } timeout.tv_sec = 10; timeout.tv_usec = 0;
	 * if ((int) clnt_call(client, infoCommand, xdr_void, NULL, xdr_void, (char *) NULL,
	 * timeout) != (int) RPC_SUCCESS) { fprintf(stderr, "startRtsInfo: Cannot communicate with
	 * rtsinfo\n"); clnt_perror(client, "clnt_call"); return (-1); } clnt_destroy(client);
	 * return (pid);
	 */
	return (1);
}

char           *
                rts_error (parm_1)
	int             parm_1;
{
	char            problem[64];

	switch (parm_1) {
	case ErrorIssuingRpcCall:
		return ("RTS Error... call to talk to the ACS failed\n");
	case -1:
		return ("RTS Error... shut 'er down, chauncy, we're suckin mud\n");
	case 0:
		return ("");
	case 1:
		return ("RTS Error...invalid command\n");
	case 2:
		return ("RTS Error...invalid parameter\n");
	case 8:
		return ("RTS Error...unexpected bus/address error\n");
	case 10:
		return ("RTS Error...rts database is not initialized\n");
	case 11:
		return ("RTS Error...acquisition is already configured\n");
	case 12:
		return ("RTS Error...acquisition is not configured\n");
	case 13:
		return ("RTS Error...maximum number of planes already defined\n");
	case 14:
		return ("RTS Error...maximum number of frames already defined\n");
	case 15:
		return ("RTS Error...unknown model specified\n");
	case 16:
		return ("RTS Error...no planes defined for acquisition\n");
	case 17:
		return ("RTS Error...no frames defined for acquisition\n");
	case 18:
		return ("RTS Error...not enough memory for defined acquisition\n");
	case 19:
		return ("RTS Error...specified sinogram does not exist\n");
	case 50:
		return ("RTS Error...no gantry initialization\n");
	case 51:
		return ("RTS Error...polling timeout is too short\n");
	case 100:
		return ("RTS Error...out of memory value\n");
	case 102:
		return ("RTS Error...memory size mismatch during configure\n");
	case 104:
		return ("RTS Error...memory failure during configure\n");
	case 106:
		return ("RTS Error...definition missing for configure\n");
	case 110:
		return ("RTS Error...bus error during configure -1\n");
	case 112:
		return ("RTS Error...formatter bus error\n");
	case 114:
		return ("RTS Error...sorter bus error\n");
	case 116:
		return ("RTS Error...tally bus error\n");
	case 118:
		return ("RTS Error...dr11-w bus error\n");
	case 120:
		return ("RTS Error...no wobble gate csr response\n");
	case 122:
		return ("RTS Error...no wobble gate\n");
	case 124:
		return ("RTS Error...frame / segment mismatch\n");
	case 126:
		return ("RTS Error...memory not configured for 'strt' command\n");
	case 200:
		return ("RTS Error...no sinogram available for 'gimi' command\n");
	case 210:
		return ("RTS Error...no gated windows defined\n");
	case 212:
		return ("RTS Error...max windows defined\n");
	case 300:
		return ("RTS Error...no planes defined for 'repo' command\n");
	case 333:
		return ("RTS Error...invalid rts unit number\n");
	case 334:
		return ("RTS Error...rts serial communication timeout\n");
	case 335:
		return ("RTS Error...rts dr11w  communication timeout\n");
	case 400:
		return ("RTS Error...no record for 'rgan' command\n");
	case 402:
		return ("RTS Error...gantry polling\n");
	case 500:
		return ("RTS Error...disk format error\n");
	case 502:
		return ("RTS Error...block pointer table over flow (> 32)\n");
	case 504:
		return ("RTS Error...disk busy\n");
	default:
		sprintf (problem, "RTS Error...not sure, passed a %d", parm_1);
		return ("RTS Error...not sure\n");
	}
}

/*****************************************************************************************************
int rts_wait (waitThisLong)
int waitThisLong;
{
    int check_rts_status ();
    bool_t doAcsAcqCommand ();

    sleep (waitThisLong);
    while ((check_rts_status ()) != NULL);
    return (TRUE);
}
*****************************************************************************************************/

/*****************************************************************************************************
rts_dseg (nsegs, direction, offset, duration)
int nsegs, direction, offset, duration;
{
    int resp;
    DSEG_args dsegCmd;
    bool_t doAcsAcqCommand ();

    dsegCmd.nseg = nsegs;
    dsegCmd.dir = direction;
    dsegCmd.offset = offset;
    dsegCmd.duration = duration;
    if (!(doAcsAcqCommand (DSEG, xdr_DSEG_args, dsegCmd, xdr_int, &resp)))
	resp = ErrorIssuingRpcCall;
    return (resp);
}
*****************************************************************************************************/

/*****************************************************************************************************
int rts_blim (frame, lowLimit, highLimit)
int frame, lowLimit, highLimit;
{
    int resp;
    BLIM_args blimCmd;
    bool_t doAcsAcqCommand ();

    blimCmd.frame = frame;
    blimCmd.minBeat = lowLimit;
    blimCmd.maxBeat = highLimit;
    if (!(doAcsAcqCommand (BLIM, xdr_BLIM_args, &blimCmd, xdr_int, &resp)))
	resp = ErrorIssuingRpcCall;
    return (resp);
}
*****************************************************************************************************/

/*****************************************************************************************************
rts_rsin (bucket, cor_sing, uncor_sing)
int bucket, *cor_sing, *uncor_sing;
{
    int resp;
    SNGL_resp singData;
    bool_t doAcsAcqCommand ();

    resp = doAcsAcqCommand (RSIN, xdr_int, &bucket, xdr_SNGL_resp, &singData);
    *cor_sing = singData.corrected_singles;
    *uncor_sing = singData.uncorrected_singles;
    return (resp);
}
*****************************************************************************************************/

/*****************************************************************************************************
int rts_beat (frame, beatData)
int frame;
BEAT_resp *beatData;
{
    bool_t doAcsAcqCommand ();

    doAcsAcqCommand (BEAT, xdr_int, &frame, xdr_BEAT_resp, beatData);
    return (beatData->beat_result);
}
*****************************************************************************************************/
