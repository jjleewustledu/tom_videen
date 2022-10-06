//	File:	io3d.h
//	Low-level routines for image IO.
//	Author:	Stefan Wirtz, Nils Papenberg, Konstantin Ens

#ifndef _IO3D_H
#define _IO3D_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

//	Reading an Image from a file:
//	name = name of file to be read
//	out	= reference to data
//	m1	= first dimension of image
//	m2	= second dimension of image
//	m3	= third dimension of image

void read3D(char *name, double *out, int m1, int m2, int m3);

//	Reading the size of a given image:
//	name = name of file
//	m1	= first dimension of image, overwriten
//	m2	= second dimension of image, overwriten
//	m3	= third dimension of image, overwriten

void read3Dsize(char *name, int *m1, int *m2, int *m3);

//	Writing an image to File:
//	name = name of file.
//	in	= Array3D for writing, overvriten.
//	m1	= first dimension of the image, overwriten.
//	m2	= second dimension of the image, overwriten.
//	m3	= third dimension of the image, overwriten.

void write3D(char *name, double *in, int m1, int m2, int m3);

//	Converts a string into an integer.
//	digit = string of digits to be converted.

int    string2int(char *digit);

//	Converts a string into a double.
//	digit = string of digits to be converted.

double string2double(char *digit);

//	Read a parameter file and provide given parameters for main:
//	fileName = name of parameter-file.
//	m1 = first dimension of the image, overwriten (read from file).
//	m2 = second dimension of the image, overwriten (read from file).
//	m3 = third dimension of the image, overwriten (read from file).
//	h1 = pixel-size in x1-direction.
//	h2 = pixel-size in x2-direction.
//	h3 = pixel-size in x3-direction.
//	fileNameT = name of Template-image T.
//	fileNameR = name of Reference-image R.
//	fileNameOut = filefame for Output (deformed Template).
//	smootherName = name of registration type.
//	maxIter = maximum number of Iterations.
//	alpha = weighting parameter.
//	tau = timestepping parameter.

void getMainParameters3D(char *fileName,
	int *m1,
	int *m2,
	int *m3,
	double *h1,
	double *h2,
	double *h3,
	char *fileNameT,
	char *fileNameR,
	char *fileNameOut,
	char *fileNameU,
	char *smootherName,
	int *maxIter,
	double *alpha,
	double *tau,
	double *mu,
	double *lambda);

void getWarp3DParm(char *fileName,
	int *m1,
	int *m2,
	int *m3,
	double *h1,
	double *h2,
	double *h3,
	char *fileNameT,
	char *fileNameOut,
	char *fileNameU);

#endif
