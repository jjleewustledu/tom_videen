#include "mediman_rpc.h"
#include <sys/time.h>

bool_t xdr_TEXT_REQUEST(xdrs, objp)
XDR *xdrs;
TEXT_REQUEST *objp;
{
	if (!xdr_string(xdrs, &objp->text, ~0)) {
		return (FALSE);
	}
	return (TRUE);
}

bool_t svc_getargsC(xprt, in)
     SVCXPRT *xprt;
     char *in;
{
		return svc_getargs(xprt,xdr_TEXT_REQUEST,in);
}

 bool_t svc_freeargsC(xprt,  in)
     SVCXPRT *xprt;
     char *in;
{
	return svc_freeargs(xprt, xdr_TEXT_REQUEST, in);
}
  
int clnt_callC(clnt, procnum, in, outproc, out, timeout)
     CLIENT *clnt;
     u_long procnum;
     xdrproc_t outproc;
     char *in, *out;
     struct timeval timeout;
{
	return clnt_call(clnt, procnum, xdr_TEXT_REQUEST, in, outproc, out, timeout);
}
void close_connectionC(client,sock)
	CLIENT* client;
	int sock;
{
    clnt_destroy(client);
    close(sock);
}
