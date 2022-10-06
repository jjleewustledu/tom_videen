/* @(#)extract_frame.c	2.0 8/9/93 */
/*
 * Updated by Sibomana@topo.ucl.ac.be for ECAT 7.0 support
 * CAUTION : Scan3D Subheader is stored in 2 blocks
 */

#define PORTMAP
#include <sys/types.h>
#include <sys/socket.h>
#include <rpc/rpc.h>
#ifdef __SVR4
#include <rpc/clnt_soc.h>
#endif
#include <netdb.h>
#include <sys/time.h>
#include <stdio.h>
#include <errno.h>
#include "matrix.h"
#include "matrix_xdr.h"
#define ERROR -1

static char sccsid[]="@(#)extract_frame.c	2.0 6/95";
static char prgnam[]="extract_frame";
static CLIENT *acqClient;
static struct timeval acqTimeout;

static int initAcqTcpClient ()
{
    struct sockaddr_in sock;
    struct hostent *hp;
    char    *host, *getenv() ;

    int channel,  response;
	 u_long program = MATRIX_SERVER, version = MATRIX_SERVER_VERSION;
   host = getenv("VXHOST") ;
    if (!host) host="acs" ;
    hp = gethostbyname (host);
    memcpy ((caddr_t) & sock.sin_addr, hp->h_addr, hp->h_length);
    sock.sin_family = AF_INET;
    sock.sin_port = 0;
    channel = RPC_ANYSOCK;
    acqTimeout.tv_sec = 300;
    acqTimeout.tv_usec = 0;

    if ((acqClient = clnttcp_create (&sock, program, version, &channel,
        1024, 1024)) == NULL)
    {
    clnt_pcreateerror ("acqClient_error");
    return (0);
    }
    return (1);
}

static usage() { crash( "usage: %s -i matspec -o matspec\n", prgnam);}

main( argc, argv)
  int argc;
  char **argv;
{
	MatrixFile *mp1=NULL, *mp2=NULL;
	MatrixData *mat=NULL, *mat2=NULL;
	char fname[256];
	int c, matnum1, matnum2;
	int blk, nblks, in_blk, out_blk;
	u_char buf[MatBLKSIZE];
	Main_header mh;
	struct MatDir matdir1, matdir2, dir_entry ;
	RDAT_args rdArgs;
	RDAT_resp rdResp;
	int status;
	extern char *optarg;

	 while ((c = getopt (argc, argv, "i:o:v")) != EOF) {
		switch (c) {
		case 'i' :
			matspec(optarg,fname,&matnum1);
			mp1 = matrix_open(fname, MAT_READ_ONLY, Short3dSinogram);
			if (mp1 == NULL) crash("extract_frame: can't read %s\n",fname);
			if (matrix_find(mp1,matnum1,&matdir1) == ERROR)
				crash("extract_frame: %s matrix not found\n",optarg);
			break;
		case 'o' :
			matspec(optarg,fname,&matnum2);
			mh = *mp1->mhptr;
			mh.num_frames = 0;
			mp2 = matrix_create(fname, MAT_OPEN_EXISTING, &mh);
			if (mp2 == NULL) crash("extract_frame: can't write %s\n",fname);
			break;
		}
	}
	if (mp1==NULL || mp2==NULL) usage();
	if (matnum2 == 0) matnum2 = matnum1; /* use same specs if not specified */
	nblks = matdir1.endblk - matdir1.strtblk;
	in_blk = matdir1.strtblk+2;
	mat = matrix_read( mp1, matnum1, MAT_SUB_HEADER);
	if (matrix_find(mp2, matnum2, &matdir2) == ERROR) {
		blk = mat_enter(mp2->fptr, mp2->mhptr, matnum2, nblks) ;
		dir_entry.matnum = matnum2 ;
		dir_entry.strtblk = blk ;
		dir_entry.endblk = dir_entry.strtblk + nblks - 1 ;
		dir_entry.matstat = 1 ;
		insert_mdir(dir_entry, mp2->dirlist) ;
		matdir2 = dir_entry;
	}
	mat_write_Scan3D_subheader(mp2->fptr, mp2->mhptr, matdir2.strtblk,
		mat->shptr);
	out_blk = matdir2.strtblk+2;
	if (fseek(mp2->fptr,(out_blk-1)*MatBLKSIZE, 0) == EOF) 
		crash("%s : fseek error\n",mp2->fname);
	if (mp1->acs) {
		memset(&rdArgs,0,sizeof(rdArgs));
		memset(&rdResp,0,sizeof(rdResp));
		rdArgs.file_name = mp1->fname;
		rdArgs.matnum = matnum1;
		initAcqTcpClient();
		status = clnt_call(acqClient, RDAT,xdr_RDAT_args, &rdArgs,
              xdr_RDAT_resp, &rdResp, acqTimeout);
		if ((status != RPC_SUCCESS) && (errno != EINTR))
			clnt_perror (acqClient, "acqClient_error");
		exit(1);
		if (rdResp.rdat.rdat_len <= 0) crash("ACS RDAT error\n");
		if (fwrite(rdResp.rdat.rdat_val,1,rdResp.rdat.rdat_len,mp2->fptr)
			!= rdResp.rdat.rdat_len) crash("%s write error\n",mp2->fname);
		clnt_destroy(acqClient);
	} else {
		if (fseek(mp1->fptr,(in_blk-1)*MatBLKSIZE, 0) == EOF) 
			crash("%s : fseek error\n",mp1->fname);
		for (blk=2; blk<=nblks; blk++, in_blk++) {
			if (fread(buf,1,MatBLKSIZE,mp1->fptr) != MatBLKSIZE)
				crash("%s read error\n",mp1->fname);
			if (fwrite(buf,1,MatBLKSIZE,mp2->fptr) != MatBLKSIZE)
				crash("%s write error\n",mp2->fname);
		}
	}
	mp2->mhptr->num_frames += 1;
	mat_write_main_header(mp2->fptr, mp2->mhptr);	/* update main header */
	matrix_free(mat);
	matrix_close(mp1);
	matrix_close(mp2);
	exit(0);
}
