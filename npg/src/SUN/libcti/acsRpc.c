/*$Id: acsRpc.c,v 1.5 1999/12/13 22:02:19 tom Exp $*/
/*$Log: acsRpc.c,v $
 *Revision 1.5  1999/12/13 22:02:19  tom
 *partially cleaned up with lint
 *
 * Revision 1.4  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.3  1994/03/03  17:13:53  ty7777
 * Changed id to header.
 **/

/*
 * acsRpc.c collection of modules to allow rpc communications with the acquisition node.
 * all modules look at the environment variable VXHOST or the process's default
 * database for the value of /Ecat/EcatAcqServer
 * to determine who the acquisition node is.
 * t.w. jansto 3/28/90
 */

#include <stdio.h>
#include <signal.h>
#include <strings.h>
#include <netdb.h>
#include <rpc/rpc.h>
#include <rpc/pmap_prot.h>
#include "ecatrpc.h"
extern int      errno;

#define RETRIES 3
#define AF_INET 2

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/acsRpc.c,v 1.5 1999/12/13 22:02:19 tom Exp $";
#endif

static CLIENT  *client;

/*
static struct timeval timeout = {10, 0};
*/

int             init_netacp ()
{
	initializeTCPClient ();
	return (0);
}

int             initializeTCPClient ()
{
	struct sockaddr_in sock;
	struct hostent *hp;

	/*
	 * int channel, status, response; u_long program = RTS_SERVER, version =
	 * RTS_SERVER_VERSION;
	 * 
	 * hp = gethostbyname ((getenv ("VXHOST") ? getenv ("VXHOST") : defaults_get_string
	 * ("/Ecat/EcatAcqServer", "localhost", 0))); if (hp == NULL) { fprintf (stderr,
	 * "ERROR...no host available\n"); return (FALSE); } bcopy (hp->h_addr, (caddr_t) &
	 * sock.sin_addr, hp->h_length); sock.sin_family = AF_INET; sock.sin_port = 0; channel =
	 * RPC_ANYSOCK; if ((client = clnttcp_create (&sock, program, version, &channel, 1024,
	 * 1024)) == NULL) { clnt_pcreateerror ("client_error"); return (FALSE); }
	 */
	return (TRUE);
}

bool_t          do_netacp_command (command, inProc, inArg, outProc, outArg)
	int             command;

	xdrproc_t       inProc, outProc;

	caddr_t         inArg, outArg;
{
	enum clnt_stat  status;

	/*
	 * Commented out by Tom Yang on 1/28/93. if (!(initializeTCPClient ())) return (FALSE);
	 * status = clnt_call (client, command, inProc, inArg, outProc, outArg, timeout); if
	 * ((status != RPC_SUCCESS) && (errno != EINTR)) { clnt_perror (client, "client_error");
	 * return (FALSE); }
	 */
	return (TRUE);
}

static CLIENT  *acqClient;

/* Tom Yang on 1/28/93
static struct timeval acqTimeout = {60, 0};
*/

int             initAcqTcpClient ()
{
	struct sockaddr_in sock;

	struct hostent *hp;

	int             retry = RETRIES + 1;

	int             channel, status, response;

	u_long          program = RTS_SERVER, version = RTS_SERVER_VERSION;

/*
    hp = gethostbyname ((getenv ("VXHOST") ? getenv ("VXHOST") :
	defaults_get_string ("/Ecat/EcatAcqServer", "localhost", 0)));
    if (hp == NULL)
    {
	fprintf (stderr, "ERROR...no host available\n");
	return (FALSE);
    }
    bcopy (hp->h_addr, (caddr_t) & sock.sin_addr, hp->h_length);
    sock.sin_family = AF_INET;
    sock.sin_port = 0;
    channel = RPC_ANYSOCK;
    while((acqClient = clnttcp_create (&sock, program, version, &channel,
		1024, 1024)) == NULL && --retry)
    {
	fprintf(stderr, "%d - ", RETRIES - retry + 1);
	clnt_pcreateerror ("acqClient_error");
    	sock.sin_port = 0;
    	channel = RPC_ANYSOCK;
    }
*/
	return (retry);
}

bool_t          doAcsAcqCommand (command, inProc, inArg, outProc, outArg)
	int             command;

	xdrproc_t       inProc, outProc;

	caddr_t         inArg, outArg;
{
	enum clnt_stat  status;

	/*
	 * if (!(initAcqTcpClient ())) return (FALSE); status = clnt_call (acqClient, command,
	 * inProc, inArg, outProc, outArg, acqTimeout); if (status != RPC_SUCCESS && errno !=
	 * EINTR) { clnt_perror (acqClient, "acqClient_error"); clnt_destroy (acqClient); return
	 * (FALSE); } clnt_destroy (acqClient);
	 */
	return (TRUE);
}

static CLIENT  *mcsClient;

/*
static struct timeval mcsTimeout = {60, 0};
*/

int             initMcsTcpClient ()
{
	struct sockaddr_in sock;

	struct hostent *hp;

/*
    int channel, status, response;
    u_long program = MCS_SERVER, version = MCS_SERVER_VERSION;

    hp = gethostbyname ((getenv ("VXHOST") ? getenv ("VXHOST") :
	defaults_get_string ("/Ecat/EcatAcqServer", "localhost", 0)));
    if (hp == NULL)
    {
	fprintf (stderr, "ERROR...no host available\n");
	return (FALSE);
    }
    bcopy (hp->h_addr, (caddr_t) & sock.sin_addr, hp->h_length);
    sock.sin_family = AF_INET;
    sock.sin_port = 0;
    channel = RPC_ANYSOCK;
    if ((mcsClient = clnttcp_create (&sock, program, version, &channel, 1024, 1024)) == NULL)
    {
	clnt_pcreateerror ("mcsClient_error");
	return (FALSE);
    }
*/
	return (TRUE);
}

bool_t          doAcsMcsCommand (command, inProc, inArg, outProc, outArg)
	int             command;

	xdrproc_t       inProc, outProc;

	caddr_t         inArg, outArg;
{
	enum clnt_stat  status;

	/*
	 * if (!(initMcsTcpClient ())) return (FALSE); status = clnt_call (mcsClient, command,
	 * inProc, inArg, outProc, outArg, mcsTimeout); if (status != RPC_SUCCESS && errno !=
	 * EINTR) { clnt_perror (mcsClient, "mcsClient_error"); clnt_destroy (mcsClient); return
	 * (FALSE); } clnt_destroy (mcsClient);
	 */
	return (TRUE);
}

bool_t          changeMcsTimeoutValue (waitThisManySeconds)
	int             waitThisManySeconds;
{

	/*
	 * mcsTimeout.tv_sec = waitThisManySeconds;
	 */
	return (TRUE);
}

static CLIENT  *reconClient;

/* Tom Yang 1/28/93.
static struct timeval reconTimeout = {15, 0};
*/

int             initReconTcpClient ()
{
	struct sockaddr_in sock;

	struct hostent *hp;

/*
    int channel, status, response;
    u_long program = RECON_SERVER, version = RECON_SERVER_VERSION;

    hp = gethostbyname ((getenv ("VXHOST") ? getenv ("VXHOST") :
	defaults_get_string ("/Ecat/EcatAcqServer", "localhost", 0)));
    if (hp == NULL)
    {
	fprintf (stderr, "ERROR...no host available\n");
	return (FALSE);
    }
    bcopy (hp->h_addr, (caddr_t) & sock.sin_addr, hp->h_length);
    sock.sin_family = AF_INET;
    sock.sin_port = 0;
    channel = RPC_ANYSOCK;
    if ((reconClient = clntudp_create (&sock, program, version, reconTimeout, &channel, 1024, 1024)) == NULL)
    {
	clnt_pcreateerror ("reconClient_error");
	return (FALSE);
    }
*/
	return (TRUE);
}

bool_t          doAcsReconCommand (command, inProc, inArg, outProc, outArg)
	int             command;

	xdrproc_t       inProc, outProc;

	caddr_t         inArg, outArg;
{
	enum clnt_stat  status;

	/*
	 * if (!(initReconTcpClient ())) return (FALSE); status = clnt_call (reconClient, command,
	 * inProc, inArg, outProc, outArg, reconTimeout); if (status != RPC_SUCCESS && errno !=
	 * EINTR) { clnt_perror (reconClient, "reconClient_error"); clnt_destroy (reconClient);
	 * return (FALSE); } clnt_destroy (reconClient);
	 */
	return (TRUE);
}

static CLIENT  *AutoQueClnt;

/*
static struct timeval AutoQueTimeout = {10, 0};
*/

int             initAutoQue ()
{
	initAutoQueClient ();
	return (0);
}

int             initAutoQueClient ()
{
	struct sockaddr_in sock;

	struct hostent *hp;

/* Tom Yang on 1/28/93.
    int channel, status, response;
    u_long program = RTS_SERVER, version = RTS_SERVER_VERSION;

    hp = gethostbyname ((getenv ("VXHOST") ? getenv ("VXHOST") :
	defaults_get_string ("/Ecat/EcatAcqServer", "localhost", 0)));
    if (hp == NULL)
    {
	fprintf (stderr, "ERROR...no host available\n");
	return (FALSE);
    }
    bcopy (hp->h_addr, (caddr_t) & sock.sin_addr, hp->h_length);
    sock.sin_family = AF_INET;
    sock.sin_port = 0;
    channel = RPC_ANYSOCK;
    if ((AutoQueClnt = clnttcp_create (&sock, program, version, &channel,
		1024, 1024)) == NULL)
    {
	clnt_pcreateerror ("AutoQueClnt_error");
	return (FALSE);
    }
*/
	return (TRUE);
}

bool_t          doAutoQueCmd (command, inProc, inArg, outProc, outArg)
	int             command;

	xdrproc_t       inProc, outProc;

	caddr_t         inArg, outArg;
{
	enum clnt_stat  status;

	/*
	 * Tom Yang 1/28/93. if (!(initAutoQueClient ())) return (FALSE); status = clnt_call
	 * (AutoQueClnt, command, inProc, inArg, outProc, outArg, AutoQueTimeout); if (status !=
	 * RPC_SUCCESS && errno != EINTR) { clnt_perror (AutoQueClnt, "AutoQueClnt_error"); return
	 * (FALSE); }
	 */
	return (TRUE);
}

static CLIENT  *acsClnt;

/*
static struct timeval acsTimeout = {20, 0};
*/

int             initAcsClient ()
{
	struct sockaddr_in sock;

	struct hostent *hp;

/*
    int channel, status, response;
    u_long program = ECAT_ACS_SERVER, version = ECAT_ACS_VERSION;

    hp = gethostbyname ((getenv ("VXHOST") ? getenv ("VXHOST") :
	defaults_get_string ("/Ecat/EcatAcqServer", "localhost", 0)));
    if (hp == NULL)
    {
	fprintf (stderr, "ERROR...no host available\n");
	return (FALSE);
    }
    bcopy (hp->h_addr, (caddr_t) & sock.sin_addr, hp->h_length);
    sock.sin_family = AF_INET;
    sock.sin_port = 0;
    channel = RPC_ANYSOCK;
    if ((acsClnt = clntudp_create (&sock, program, version, acsTimeout, &channel)) == NULL)
    {
	clnt_pcreateerror ("acsClnt_error");
	return (FALSE);
    }
*/
	return (TRUE);
}

bool_t          doAcsAcsCommand (command, inProc, inArg, outProc, outArg)
	int             command;

	xdrproc_t       inProc, outProc;

	caddr_t         inArg, outArg;
{
	enum clnt_stat  status;

	/*
	 * Tom Yang on 1/28/93. if (!(initAcsClient ())) return (FALSE); status = clnt_call
	 * (acsClnt, command, inProc, inArg, outProc, outArg, acsTimeout); if (status !=
	 * RPC_SUCCESS && errno != EINTR) { clnt_perror (acsClnt, "acsClnt_error"); clnt_destroy
	 * (acsClnt); return (FALSE); } clnt_destroy (acsClnt);
	 */
	return (TRUE);
}

static CLIENT  *rfaClnt;

/* Tom Yang on 1/28/93.
static struct timeval rfaTimeout = {20, 0};
*/

int             initRfaClient ()
{
	struct sockaddr_in sock;

	struct hostent *hp;

/* Tom Yang on 1/28/93.
    int channel, status, response;
    u_long program = RFA_SERVER, version = RFA_SERVER_VERSION;

    hp = gethostbyname ((getenv ("VXHOST") ? getenv ("VXHOST") :
	defaults_get_string ("/Ecat/EcatAcqServer", "localhost", 0)));
    if (hp == NULL)
    {
	fprintf (stderr, "ERROR...no host available\n");
	return (FALSE);
    }
    bcopy (hp->h_addr, (caddr_t) & sock.sin_addr, hp->h_length);
    sock.sin_family = AF_INET;
    sock.sin_port = 0;
    channel = RPC_ANYSOCK;
    if ((rfaClnt =  clnttcp_create (&sock, program, version, &channel, 1024, 1024)) == NULL)
    {
	clnt_pcreateerror ("rfaClnt_error");
	return (FALSE);;
    }
*/
	return (TRUE);
}

bool_t          doAcsRfaCommand (command, inProc, inArg, outProc, outArg)
	int             command;

	xdrproc_t       inProc, outProc;

	caddr_t         inArg, outArg;
{
	enum clnt_stat  status;

	/*
	 * if (!(initRfaClient ())) return (FALSE); status = clnt_call (rfaClnt, command, inProc,
	 * inArg, outProc, outArg, rfaTimeout); if (status != RPC_SUCCESS && errno != EINTR) {
	 * clnt_perror (rfaClnt, "rfaClnt_error"); clnt_destroy (rfaClnt); return (FALSE); }
	 * clnt_destroy (rfaClnt);
	 */
	return (TRUE);
}

bool_t          changeRfaTimeoutValue (waitThisManySeconds)
	int             waitThisManySeconds;
{

	/*
	 * rfaTimeout.tv_sec = waitThisManySeconds;
	 */
	return (TRUE);
}

static CLIENT  *arcClient;

/* Tom Yang on 1/28/93.
static struct timeval arcTimeout = {60, 0};
*/

int             initArcTcpClient ()
{
	struct sockaddr_in sock;

	struct hostent *hp;

	int             channel, status, response;

	u_long          program = ARC_SERVER, version = ARC_SERVER_VERSION;

/* Tom Yang on 1/28/93
    hp = gethostbyname ((getenv ("VXHOST") ? getenv ("VXHOST") :
	 defaults_get_string ("/Ecat/EcatAcqServer", "localhost", 0)));
    if (hp == NULL)
    {
	fprintf (stderr, "ERROR...no host available\n");
	return (FALSE);
    }
    bcopy (hp->h_addr, (caddr_t) & sock.sin_addr, hp->h_length);
    sock.sin_family = AF_INET;
    sock.sin_port = 0;
    channel = RPC_ANYSOCK;
    if ((arcClient = clnttcp_create (&sock, program, version, &channel, 1024, 1024)) == NULL)
    {
	clnt_pcreateerror ("arcClient_error");
	return (FALSE);
    }
*/
	return (TRUE);
}

bool_t          doAcsArcCommand (command, inProc, inArg, outProc, outArg)
	int             command;

	xdrproc_t       inProc, outProc;

	caddr_t         inArg, outArg;
{
	enum clnt_stat  status;

	/*
	 * if (!(initArcTcpClient ())) return (FALSE); status = clnt_call (arcClient, command,
	 * inProc, inArg, outProc, outArg, arcTimeout); if (status != RPC_SUCCESS && errno !=
	 * EINTR) { clnt_perror (arcClient, "arcClient_error"); clnt_destroy (arcClient); return
	 * (FALSE); } clnt_destroy (arcClient);
	 */
	return (TRUE);
}

bool_t          changeArcTimeoutValue (waitThisManySeconds)
	int             waitThisManySeconds;
{

	/*
	 * arcTimeout.tv_sec = waitThisManySeconds;
	 */
	return (TRUE);
}

int             getNextRPCVersion (rpcProgram)
	int             rpcProgram;
{
	struct pmaplist *pmap_getmaps (), *pMapList = NULL;

	struct sockaddr_in sock;

	struct hostent *hp;

	int             channel, status, response;

	u_long          version = 0;

	hp = gethostbyname ("localhost");
	bcopy (hp->h_addr, (caddr_t) & sock.sin_addr, hp->h_length);
	sock.sin_family = AF_INET;
	sock.sin_port = 0;
	channel = RPC_ANYSOCK;
	pMapList = pmap_getmaps (&sock);
	if (pMapList) {
		for (; pMapList != NULL; pMapList = pMapList->pml_next) {
			if (rpcProgram == pMapList->pml_map.pm_prog)
				if (pMapList->pml_map.pm_vers > version)
					version = pMapList->pml_map.pm_vers;
		}
	}
	return (version + 1);
}
