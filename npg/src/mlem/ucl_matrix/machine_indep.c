#include <stdlib.h>
#include <string.h>
#include "matrix.h"


#ifdef _WIN32
u_short ntohs(u_short s) {
	union { unsigned char t[2]; unsigned short t2; } a, b;
	a.t2 = s;
	b.t[0] = a.t[1];
	b.t[1] = a.t[0];
	return b.t2;
}
unsigned long ntohl(unsigned long l) {
	int i;
	union { unsigned char t[4]; unsigned long t4; } a, b;
	a.t4 = l;
	for (i=0; i<4; i++) b.t[i] = a.t[3-i];
	return b.t4;
}
#endif


#if defined(__STDC__)
void SWAB(const void *from, void *to, int length)
#else
void SWAB( from, to, length)
char *from, *to;
int length;
#endif
{
	if (ntohs(1) == 1) swab(from, to, length);
	else memcpy(to,from,length);
}

void SWAW ( from, to, length)
#if defined(__STDC__)
const short *from;
#else
short *from;
#endif
short *to;
int length;
{
	if (ntohs(1) == 1) swaw(from,to,length);
	else memcpy(to,from,length*2);
} 

#if defined(mips) && defined(ultrix)   /* something obsolete */
float vaxftohf( bufr, off)
unsigned short *bufr;
int off;
{
	union { unsigned short t[2]; float t4; } test ;

	test.t[0] = bufr[off];
	test.t[1] = bufr[off+1];
	ftoi(&test.t4);
	return test.t4;
}
ftovaxf(f, bufr)
float f;
unsigned short *bufr;
{
	union { unsigned short t[2]; float t4; } test ;

	test.t4 = f;
	bufr[0] = 0;
	bufr[1] = 0;
	if (test.t4 == 0.0) return;
	itof(&test.t4);
	bufr[1] = test.t[1];
	bufr[0] = test.t[0];
	return;
}

#else  /* modern acrhitectures */
float vaxftohf( bufr, off)
unsigned short *bufr;
int off;
{
    unsigned short t1, t2;
    union {unsigned int t3; float t4;} test;

    if (bufr[off]==0 && bufr[off+1]==0) return(0.0);
    t1 = bufr[off] & 0x80ff;
    t2=(((bufr[off])&0x7f00)+0xff00)&0x7f00;
    test.t3 = (t1+t2)<<16;
    test.t3 =test.t3+bufr[off+1];
    return(test.t4);
}


#if defined(sun) || defined(sgi) || defined(hp)		/* BIG ENDIAN */
ftovaxf(orig,number)
  unsigned short number[2];
  float orig;
{

  	/* convert from sun float to vax float */

  	union {
	 	  unsigned short t[2]; 
		  float t4;
	      } test;
	unsigned short int exp;

	number[0] = 0;
	number[1] = 0;

	test.t4 = orig;
	if (test.t4 == 0.0) return;

	number[1] = test.t[1];

	exp = ((test.t[0] & 0x7f00) + 0x0100) & 0x7f00;
	test.t[0] = (test.t[0] & 0x80ff) + exp;

	number[0] = test.t[0];

}
# else  /* LITTLE ENDIAN (alpha, intel) */
ftovaxf(f, bufr)
float f;
unsigned short *bufr;
{
	unsigned *u, sign_exp, low, high, mantissa, ret;
	float f_tmp = f;

	u = (unsigned*)(&f_tmp);
	if (u == 0)  {
		bufr[0]	= bufr[1] = 0;
		return;
	}
	sign_exp = *u & 0xff800000;
	sign_exp = sign_exp >> 16;
	sign_exp = (sign_exp + 0x0110) & 0xff80; 
	low   = *u & 0x0000ffff;
	high  = *u & 0x007f0000;
	mantissa = (high >> 16) + (low << 16);
	ret = sign_exp + mantissa;
	bufr[0] = ret;
	bufr[1] =  ret >>16;
}
#endif /* modern arrchitecture */
#endif 

int file_data_to_host(dptr,nblks,dtype)
char *dptr;
int nblks,dtype;
{
	int i, j;
	char *tmp;


        tmp = (char*) calloc(512, sizeof(char));

	switch(dtype)
	{
	case ByteData:
		break;
	case VAX_Ix2:
		if (ntohs(1) == 1) 
			for (i=0, j=0; i<nblks; i++, j+=512) {
				swab( dptr+j, tmp, 512);
				memcpy(dptr+j,tmp,512);
			}
		break;
	case VAX_Ix4:
		if (ntohs(1) == 1)
			for (i=0, j=0; i<nblks; i++, j+=512) {
				swab(dptr+j, tmp, 512);
				swaw(tmp, dptr+j, 256);
			}
		break;
	case VAX_Rx4:
	 	if (ntohs(1) == 1) 
			 for (i=0, j=0; i<nblks; i++, j+=512) {
                         /* Johan Nuyts 27-oct-1997: error: only bytes
                            should be swapped prior to vaxftohf */
                            /* swab( dptr+j, tmp, 512);
			       swaw(tmp, dptr+j, 256); */
                            swab(dptr+j, dptr+j, 512);
			}
		for (i=0; i<nblks*128; i++)
			((float *)dptr)[i] = vaxftohf( (unsigned short *)dptr, i*2) ;
		break;
	case SunShort:
		if (ntohs(1) != 1)
			for (i=0, j=0; i<nblks; i++, j+=512) {
				swab(dptr+j, tmp, 512);
				memcpy(dptr+j,tmp,512);
			}
		break;
	case SunLong:
	case IeeeFloat:
		if (ntohs(1) != 1) 
			for (i=0, j=0; i<nblks; i++, j+=512) {
				swab(dptr+j, tmp, 512);
				swaw(tmp, dptr+j, 256);
			}
		break;
	default:	/* something else...treat as Vax I*2 */
		if (ntohs(1) == 1)
			for (i=0, j=0; i<nblks; i++, j+=512) {
				swab(dptr+j, tmp, 512);
				memcpy(dptr+j,tmp,512);
			}
		break;
	}
	free(tmp);
	return 0;
}

int read_raw_acs_data( fname, strtblk, nblks, dptr, dtype)
char *fname;
int strtblk, nblks, dtype;
char * dptr;
{
	int i, err;

	for (i=0; i<nblks; i++)
	{
	  err = rtsRblk( fname, strtblk+i, dptr+512*i);
	  if (err) return -1;
	}
	return file_data_to_host(dptr,nblks,dtype);
}


read_matrix_data( fptr, strtblk, nblks, dptr, dtype)
  FILE *fptr;
  int strtblk, nblks, dtype;
  char * dptr;
{
	int i, err;

	err = mat_rblk( fptr, strtblk, dptr, nblks);
	if (err) return -1;
	return file_data_to_host(dptr,nblks,dtype);
}


write_matrix_data( fptr, strtblk, nblks, dptr, dtype)
FILE *fptr;
int strtblk, nblks, dtype;
char *dptr;
{
	int error_flag = 0;
	int i, j, k;
	char *bufr1, *bufr2;
	bufr1 = malloc(512);
	bufr2 = malloc(512);
	switch( dtype)
	{
	case ByteData:
		break;
	case VAX_Ix2: 
	default:	/* something else...treat as Vax I*2 */
		if (ntohs(1) == 1) {
			for (i=0, j=0; i<nblks && !error_flag; i++, j += 512) {
				swab( dptr+j, bufr1, 512);
				memcpy(bufr2,bufr1,512);
				if ( mat_wblk( fptr, strtblk+i, bufr2, 1) < 0) error_flag++;
			}
		} else if ( mat_wblk( fptr, strtblk, dptr, nblks) < 0) error_flag++;
		break;
	case VAX_Ix4:
		if (ntohs(1) == 1)  {
			for (i=0, j=0; i<nblks && !error_flag; i++, j += 512) {
				swab( dptr, bufr1, 512);
				swaw( bufr1, bufr2, 256);
				if ( mat_wblk( fptr, strtblk+i, bufr2, 1) < 0) error_flag++;
			} 
		} else if ( mat_wblk( fptr, strtblk, dptr, nblks) < 0) error_flag++;
		break;
	case VAX_Rx4:
		k = 0;
		for (i=0; i<nblks; i++) {
		  for (j=0; j<512; j += sizeof(float), k += sizeof(float)) 
	            ftovaxf(*((float*)&dptr[k]),&bufr2[j]);
                  /* Johan Nuyts, 14 juni 1999: swab after ftovaxf */
                  if (ntohs(1) == 1)
                    swab( bufr2, bufr2, 512);
		  if ( mat_wblk( fptr, strtblk+i, bufr2, 1) < 0) error_flag++;
		}
		break;
	case IeeeFloat:
	case SunLong:
		if (ntohs(1) != 1) {
			for (i=0, j=0; i<nblks && !error_flag; i++, j += 512) {
				swab( dptr+j, bufr1, 512);
				swaw( bufr1, bufr2, 256);
				if ( mat_wblk( fptr, strtblk+i, bufr2, 1) < 0) error_flag++;
			}
		} else if ( mat_wblk( fptr, strtblk, dptr, nblks) < 0) error_flag++;
		break;
	case SunShort:
		if (ntohs(1) != 1) {
			for (i=0, j=0; i<nblks && !error_flag; i++, j += 512) {
				swab( dptr+j, bufr1, 512);
				memcpy(bufr2,bufr1,512);
				if ( mat_wblk( fptr, strtblk+i, bufr2, 1) < 0) error_flag++;
			}
		} else if ( mat_wblk( fptr, strtblk, dptr, nblks) < 0) error_flag++;
		break;
	}
	free(bufr1);
	free(bufr2);
	if (error_flag == 0) return 0;
	return -1;
}

/* buf...(...) - functions to handle copying header data to and from a buffer
   in the most type safe way possible; note that i is incremented
   by the size of the item copied so these functions must be
   called in the right order
*/


void bufWrite(s, buf, i, len)
char *s, *buf;
int *i, len;
{
   strncpy(&buf[*i], s, len);
    *i += len;
}

#ifdef __STDC__
void bufWrite_s(short val, char *buf, int *i)
#else
void bufWrite_s(val, buf, i)
short val;
char *buf;
int *i;
#endif
{
	union { short s; char b[2]; } tmp;
	tmp.s = val;
	if (ntohs(1) != 1) swab(tmp.b,&buf[*i],2);
    else memcpy(&buf[*i], tmp.b, sizeof(short));
    *i += sizeof(short);
}

void bufWrite_i(val, buf, i)
int val, *i;
char *buf;
{
	union { int i; char b[4]; } tmp;
	union { short s[2]; char b[4]; } tmp1;
	tmp.i = val;
	if (ntohs(1) != 1) {
		swab(tmp.b,tmp1.b,4);
		swaw(tmp1.s,(short*)&buf[*i],2);
	} else memcpy(&buf[*i], tmp.b, sizeof(int));
    *i += sizeof(int);
}

#ifdef __STDC__
void bufWrite_f(float val, char *buf, int *i)
#else
void bufWrite_f(val, buf, i)
float val;
char *buf;
int *i;
#endif
{
	union { float f; char b[4]; } tmp;
	union { short s[2]; char b[4]; } tmp1;
	tmp.f = val;
	if (ntohs(1) != 1) {
		swab(tmp.b,tmp1.b,4);
		swaw(tmp1.s,(short*)&buf[*i],2);
	} else memcpy(&buf[*i], tmp.b, sizeof(float));
    *i += sizeof(float);
}

void bufRead(s, buf, i, len)
char *s, *buf;
int *i, len;
{
    strncpy(s, &buf[*i], len);
    *i += len;
}

void bufRead_s(val, buf, i)
short* val;
char* buf;
int* i;
{
	union { short s; char b[2]; } tmp, tmp1;
	memcpy(tmp.b,&buf[*i],2);
	if (ntohs(1) != 1) {
		swab(tmp.b,tmp1.b,2);
		*val = tmp1.s;
	} else *val = tmp.s;
    *i += sizeof(short);
}

void bufRead_i(val, buf, i)
int* val;
char* buf;
int* i;
{
	union {int i; char b[4]; } tmp, tmp1;
	memcpy(tmp1.b,&buf[*i],4);
	if (ntohs(1) != 1) {
		swab(tmp1.b,tmp.b,4);
		swaw(tmp.b,tmp1.b,2);
	}
	*val = tmp1.i;
    *i += sizeof(int);
}

void bufRead_f(val, buf, i)
float* val;
char* buf;
int* i;
{
	union {float f; char b[2]; } tmp, tmp1;
    memcpy(tmp1.b, &buf[*i], sizeof(float));
	if (ntohs(1) != 1) {
		swab(tmp1.b,tmp.b,4);
		swaw(tmp.b,tmp1.b,2);
	}
	*val = tmp1.f;
    *i += sizeof(float);
}
