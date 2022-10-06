/*$Id: matpkg.c,v 1.2 1995/01/30 17:05:12 ty7777 Exp $*/
/*$Log: matpkg.c,v $
 * Revision 1.2  1995/01/30  17:05:12  ty7777
 * Added program version.
 **/

/* @(#)matpkg.c	1.1 3/16/92 */

#include "matpkg.h"

static char rcsid [] = "$Header: /home/npggw/tom/src/3drecon/RCS/matpkg.c,v 1.2 1995/01/30 17:05:12 ty7777 Exp $";

matmpy(a,b,c)
  Matrix a,b,c;
{	int i,j,k;
	float t;

	for (i=0; i<b->ncols; i++)
	  for (j=0; j<c->nrows; j++)
	  { t = 0.0;
	    for (k=0; k<b->nrows; k++)
		t += b->data[i+k*b->ncols]*c->data[k+j*c->ncols];
	    a->data[i+j*a->ncols]=t;
	  }
}

mat_print(a)
  Matrix a;
{
	int i,j;

	for (j=0;j<a->nrows;j++)
	{ for (i=0;i<a->ncols;i++)
	    printf("%13.6g ",a->data[i+j*a->ncols]);
	  printf("\n");
	}
}

mat_unity(a)
  Matrix a;
{
	int i,j;

	for (j=0; j<a->nrows; j++)
	  for (i=0; i<a->ncols; i++)
	    a->data[i+j*a->ncols]=(i==j)?1.0:0.0;
}

Matrix matrix(ncols,nrows)
  int ncols,nrows;
{
	Matrix t;

	t=(Matrix)malloc(sizeof(struct matrix));
	t->ncols=ncols;
	t->nrows=nrows;
	t->data = (float*)malloc(ncols*nrows*sizeof(float));
	return t;
}

mat_copy(a,b)
  Matrix a,b;
{
	int i;

	for (i=0; i<a->ncols*a->nrows; i++)
	  a->data[i] = b->data[i];
}

rotate(a,rx,ry,rz)
  Matrix a;
  float rx,ry,rz;
{
	Matrix t,b;
	double sint,cost;

	t=matrix(4,4);
	b=matrix(4,4);
	mat_unity(b);

	/*
	sincos(rx*M_PI/180.,&sint,&cost);
	*/
	sint	= sin (rx * M_PI / 180.0);
	cost	= cos (rx * M_PI / 180.0);

	b->data[5]=cost;
	b->data[6]=-sint;
	b->data[9]=sint;
	b->data[10]=cost;
	matmpy(t,a,b);
	mat_unity(b);

	/*
	sincos(ry*M_PI/180.,&sint,&cost);
	*/
	sint	= sin (ry * M_PI / 180.0);
	cost	= cos (ry * M_PI / 180.0);

	b->data[0]=cost;
	b->data[2]=sint;
	b->data[8]=-sint;
	b->data[10]=cost;
	matmpy(a,t,b);
	mat_unity(b);

	/*
	sincos(rz*M_PI/180.,&sint,&cost);
	*/
	sint	= sin (rz * M_PI / 180.0);
	cost	= cos (rz * M_PI / 180.0);

	b->data[0]=cost;
	b->data[1]=-sint;
	b->data[4]=sint;
	b->data[5]=cost;
	matmpy(t,a,b);
	mat_copy(a,t);
	mat_free(t);
	mat_free(b);
}

translate(a,dx,dy,dz)
  Matrix a;
  float dx,dy,dz;
{
	Matrix b,t;

	b=matrix(4,4);
	t=matrix(4,4);
	mat_copy(b,a);
	mat_unity(t);
	t->data[3]=dx;
	t->data[7]=dy;
	t->data[11]=dz;
	matmpy(a,b,t);
	mat_free(b);
	mat_free(t);
}

scale(a,sx,sy,sz)
  Matrix a;
  float sx,sy,sz;
{
	Matrix b,t;

	b=matrix(4,4);
	t=matrix(4,4);
	mat_copy(b,a);
	mat_unity(t);
	t->data[0]=sx;
	t->data[5]=sy;
	t->data[10]=sz;
	matmpy(a,b,t);
	mat_free(b);
	mat_free(t);
}

mat_free(a)
  Matrix a;
{
	free(a->data);
	free(a);
}

#ifdef TEST
main()
{
	Matrix a;

	a=matrix(4,4);
	mat_unity(a);
	translate(a,-.5,-.5,-.5);
	rotate(a,5.,-10.,15.);
	scale(a,.25,.25,.25);
	mat_print(a);
	b=matrix(1,4);
}
#endif
