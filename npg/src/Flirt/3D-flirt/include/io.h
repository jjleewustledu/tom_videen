//	File:	io.h
//	Low-level routines for image IO.
//	Author:	Stefan Wirtz, Nils Papenberg

#ifndef _IO_H
#define _IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

//  Reading an Image from a Tiff-file.
//	@param name Name of to be read.
//	@param out Reference to data.
//	@param m1 First Dimension of the image.
//	@param m2 Second Dimension of the image.

void readTIFF(char *name, array2D out, int m1, int m2);

//	Reading the size of a given TIFF image
//	@param name Name of TIFF file.
//	@param m1 First Dimension of Image, overwriten.
//	@param m2 Second Dimension of Image, overwriten.

void readTIFFsize(char *name, int *m1, int *m2);

//	Writing an image to Tiff-file.
//	@param name Name of file.
//	@param in Array2D for writing, overvriten.
//	@param m1 First Dimension of the image, overwriten.
//	@param m2 Second Dimension of the image, overwriten.

void writeTIFF(char *name, array2D in, int m1, int m2);

//	Converts a string into an integer.
//	@param digit String of digits to be converted.

int    string2int(char *digit);

//	Converts a string into a double.
//	@param digit String of digits to be converted.

double string2double(char *digit);

//	Read a parameter and provide given parameters for main.
//	@param fileName Name of parameter-file.
//	@param m1 First Dimension of the image, overwriten (read from file).
//	@param m2 Second Dimension of the image, overwriten (read from file).
//	@param h1 Pixel-size in x1-direction.
//	@param h2 Pixel-size in x2-direction.
//	@param fileNameT Name of Template-image T.
//	@param fileNameR Name of Reference-image R.
//	@param fileNameOut File-name for Output (deformed Template).
//	@param smootherName Name of registration type.
//	@param maxIter Maximum number of Iterations.
//	@param alpha Weighting parameter.
//	@param tau Timestepping parameter.

void getMainParameters(char *fileName,
	int *m1,
	int *m2,
	double *h1,
	double *h2,
	char *fileNameT,
	char *fileNameR,
	char *fileNameOut,
	char *smootherName,
	int *maxIter,
	double *alpha,
	double *tau,
	double *mu,
	double *lambda);

#endif
