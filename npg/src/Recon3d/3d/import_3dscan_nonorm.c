/*$Id: import_3dscan_nonorm.c,v 1.2 1995/01/30 15:55:22 ty7777 Exp $*/
/*$Log: import_3dscan_nonorm.c,v $
 * Revision 1.2  1995/01/30  15:55:22  ty7777
 * Added RCS mechanism.
 **/

/* @(#)import_3dscan.c  1.11 12/29/92 */

static char sccsid [] = "@(#)import_3dscan.c   1.11 12/29/92 Copyright 1992 CTI Pet Systems, Inc.";
static char rcsid [] = "$Header: /export/home/npggw/tom/src/recon3d/RCS/import_3dscan_nonorm.c,v 1.2 1995/01/30 15:55:22 ty7777 Exp $";

/*
    This routine extracts from the acquired 256 plane 3d emission scan
    the appropriate sinograms (for a specified max ring difference),
    applies normalization (based on crystal efficiencies read from the
    crystal_efficiency file) after "swapping" lors from opposite cross
    plane, then "mashes", "trims", and stores the resulting sinograms.
    Also, if there is any special input matrix numbering, like old 953B
    3d data sets stored as 4 frames of 64 planes, these are translated
    into the "normal" matrix numbering conventions.  I see why this was
    something that Antoine did!.
*/

#include "matrix.h"
#include <math.h>

#define MAX_NUMRING 32
#define MAX_RING_CRYSTALS 14*8*8
#define MAX_CRYSTALS MAX_NUMRING*MAX_RING_CRYSTALS

static char *progname;

main( argc, argv)
int argc;
char **argv;
{
    MatrixFile *mptr1, *mptr2;
    MatrixData *s1=NULL, *s2=NULL;
    Main_header *mhptr;
    Scan_subheader *sh1, *sh2;
    short int *p1, *p2, t;
    int a, b, rd, matnum1, matnum2;
    int i, j, nswap, *swap_lors, *compute_swap_lors_mashed();
    int rdmax, mash, trim, ncrystals, vflag, nrings;
    float eff_data[MAX_CRYSTALS], *fnorm=NULL, *fscan=NULL, *fp1, *fp2, *fmash=NULL;
    int np, nv, v, e, aoff, boff, deta, detb, nprojs, nviews, ndets;
    struct Matval mv;
    int omat1, omat2, frame, dtype, dets_per_ring, input_mash=1;
    int oframe=1, odtype=0;
    float norm_scale, compute_eff_norm();

    progname = argv[0];
    if (argc<5)
        crash( "%s\nusage: %s 3dscan eff.dat outscan rdmax,mash,trim,vflag,nrings [frame,dtype] [oframe,odtype]\n",
            sccsid, progname);

    mptr1 = matrix_open( argv[1], MAT_READ_ONLY, SCAN_DATA);
    if (!mptr1) matrix_perror( progname);

    ncrystals = read_eff_data( argv[2], eff_data);

    mhptr = matrix_init_main_header( argv[3], SCAN_DATA, mptr1->mhptr);
    if (mhptr->plane_separation <= 0.0) mhptr->plane_separation = 0.3375;

    rdmax = 11; /* +/- rd 11 */
    mash = 2;   /* mash to 1/2 views */
    trim = 128; /* trim to 128 bins wide (~40 cm) */
    vflag = 0;  /* use ACS matrix numbering */
    nrings = 16;    /* default */
    sscanf( argv[4], "%d,%d,%d,%d,%d",
        &rdmax, &mash, &trim, &vflag, &nrings);
    dets_per_ring = ncrystals/nrings;
    printf("%d detectors per ring\n", dets_per_ring);
    frame = 1;
    dtype = 0;
    if (argc>5) sscanf( argv[5], "%d,%d", &frame, &dtype);
    if (argc>6) sscanf( argv[6], "%d,%d", &oframe, &odtype);

    mptr2   = matrix_create (argv [3], (oframe == 1 && odtype == 0)? 
        MAT_CREATE: MAT_OPEN_EXISTING, mhptr);
    if (!mptr2)
        matrix_perror (progname);

    for (rd=0; rd<=rdmax; rd++)
    {
        for (a=0; a+rd<nrings; a++)
        {
            b = a+rd;
            matnum1 = matnumfx( a, b, vflag, frame, dtype);
            omat1 = matnumfx( a, b, 0, oframe, odtype);
            if (s1)
            {
                free( s1->data_ptr);
                free( s1->shptr);
                free( s1);
            }
            s1 = matrix_read( mptr1, matnum1, MAT_WORD);
            if (!s1) matrix_perror( progname);
            nprojs = s1->xdim;
            nviews = s1->ydim;
            ndets = 2*nviews;
            if (ndets != dets_per_ring && input_mash == 1)
            {
                input_mash = dets_per_ring / ndets;
                printf("The input data is already mashed by %d\n", input_mash);
            }
            if (a==0 && rd==1)
            {
                swap_lors = compute_swap_lors_mashed( nprojs, nviews, input_mash, &nswap);
                printf("Swapping %d lines of response per cross_plane.\n", nswap);
            }
            if (rd)
            {
                matnum2 = matnumfx( b, a, vflag, frame, dtype);
                omat2 = matnumfx( b, a, 0, oframe, odtype);
                if (s2)
                {
                    free( s2->data_ptr);
                    free( s2->shptr);
                    free( s2);
                }
                s2 = matrix_read( mptr1, matnum2, MAT_WORD);
                if (!s2) matrix_perror( progname);
                p1 = (short int*) s1->data_ptr;
                p2 = (short int*) s2->data_ptr;
                for (i=0; i<nswap; i++)
                {
                    j = swap_lors[i];
                    t = p1[j];
                    p1[j] = p2[j];
                    p2[j] = t;
                }
            }
            if (!fscan) fscan = (float*) malloc( nprojs*nviews*sizeof(float));
/*            if (!fnorm) fnorm = (float*) malloc( nprojs*nviews*sizeof(float));
            norm_scale = compute_eff_norm( fnorm, nviews, nprojs, eff_data, a, b, dets_per_ring, input_mash); JMO */
            nv = nviews/mash;
            np = trim;
            if (!fmash) fmash = (float*) malloc( np*nv*sizeof(float));
            p1 = (short int*) s1->data_ptr;
            for (i=0; i<nprojs*nviews; i++)
            {
                fscan[i] = s1->scale_factor*p1[i]/**fnorm[i]*norm_scale JMO */;
            }
            for (i=0; i<np*nv; i++)
                fmash[i] =  0.0;
            for (i=0; i<nviews; i++)
            {
                fp1 = fscan+nprojs*i;
                fp2 = fmash+np*(i/mash);
                for (j=0; j<np; j++)
                    fp2[j] += fp1[j+nprojs/2-np/2];
            }
            s1->xdim = np;
            s1->ydim = nv;
            convert_float_scan( s1, fmash);
            mat_numdoc( omat1, &mv);
            matrix_write( mptr2, omat1, s1);
            if (rd)
            {
                p2 = (short int*) s2->data_ptr;
/*                norm_scale = compute_eff_norm( fnorm, nviews, nprojs, eff_data, b, a, dets_per_ring, input_mash); JMO    */
                for (i=0; i<nprojs*nviews; i++)
                {
                    fscan[i] = s2->scale_factor*p2[i]/**fnorm[i]*norm_scale JMO */;
                }
                for (i=0; i<np*nv; i++)
                    fmash[i] =  0.0;
                for (i=0; i<nviews; i++)
                {
                    fp1 = fscan+nprojs*i;
                    fp2 = fmash+np*(i/mash);
                    for (j=0; j<np; j++)
                        fp2[j] += fp1[j+nprojs/2-np/2];
                }
                s2->xdim = np;
                s2->ydim = nv;
                convert_float_scan( s2, fmash);
                mat_numdoc( omat2, &mv);
                matrix_write( mptr2, omat2, s2);
            }
        }   /* next ring */
    }   /* next rd */

}

read_eff_data( fname, fdata)
char *fname;
float *fdata;
{
    int nvals, cnts, nlors;
    FILE *fptr;

    fptr = fopen( fname, "r");
    if (!fptr)
        crash("%s: can't open crystal efficiency file %s\n", progname, fname);
    for (nvals=0; nvals<MAX_CRYSTALS; nvals++)
    {
        if (fscanf( fptr, "%d %d %f", &cnts, &nlors, &fdata[nvals]) != 3)
            break;
    }
    fclose(fptr);
    return nvals;
}

float compute_eff_norm( fnorm, nv, np, eff, a, b, ndets, mash)
float *fnorm, *eff;
int nv, np, a, b, ndets, mash;
{
    int v, p, e, deta, detb, i, aoff, boff;
    float norm, norm_sum, norm_avg;

    bzero( fnorm, nv*np*sizeof(float));
    norm_sum = 0.0;
    aoff = ndets*a;
    boff = ndets*b;
    for (v=0; v<nv*mash; v++)
        for (p=0; p<np; p++)
        {
            e = p-np/2;
            ve_to_dets( v, e, &deta, &detb, ndets);
            norm = eff[aoff+deta]*eff[boff+detb];
            fnorm[p+np*(v/mash)] += norm;
            norm_sum += norm;
        }
    return( sqrt((double)(nv*np/norm_sum)));
}

typedef unsigned char byte;

int * compute_swap_lors_mashed( nprojs, nviews, nmash, nptr)
int nprojs, nviews, nmash, *nptr;
{
    static byte ring_16[8][3] = {
        {0,5,0},
        {0,6,0},
        {0,7,1},
        {1,6,1},
        {10,15,0},
        {9,15,0},
        {8,15,2},
        {9,14,2}
    };

    static byte ring_14[8][3] = {
        {0,4,0},
        {0,5,0},
        {0,6,1},
        {1,5,1},
        {9,13,0},
        {8,13,0},
        {8,12,2},
        {7,13,2}
    };

    static byte ring_12[8][3] = {
        {0,3,0},
        {0,4,0},
        {0,5,1},
        {1,4,1},
        {8,11,0},
        {7,11,0},
        {7,10,2},
        {6,11,2}
    };

    int ndets, deta, detb, v, e, a, b, *list, i, j, n, m;
    int db = 32, off, nodup;
    byte *fixer;

    if (nviews%2 == 0) fixer = (byte *)ring_16;
    if (nviews%7 == 0) fixer = (byte *)ring_14;
    if (nviews%7 == 0) db = 8*7;
    if (nviews%3 == 0) fixer = (byte *)ring_12;
    n = 0;
    ndets = nviews*2*nmash;
    list = (int*) malloc( db*db*8*sizeof(int));
    for (i=0; i<8; i++)
    {
        a = fixer[3*i];
        b = fixer[3*i+1];
        m = fixer[3*i+2];
        for (deta=0; deta<db; deta++)
            for (detb=0; detb<db; detb++)
            {
                dets_to_ve( a*db+deta, b*db+detb, &v, &e, ndets);
                e += nprojs/2;
                off = (v/nmash)*nprojs+e;
                if (m==1 && v<nviews*nmash/2) continue;
                if (m==2 && v>nviews*nmash/2) continue;
                nodup = 1;
                for (j=0; j<n; j++)
                    if (off == list[j]) nodup=0;
                if (nodup && (e+1>0) && (e<nprojs)) list[n++] = off;
            }
    }
    *nptr = n;
    return (list);
}
