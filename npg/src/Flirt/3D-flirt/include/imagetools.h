//	File:	imagetools.h
//	Basic image stuff; declaraition linear interpolation, etc...
//	Author:	Stefan Heldmann, Nils Papenberg
//	Date:	20/04/2005

#ifndef _IMAGETOOL_H_
#define _IMAGETOOL_H_

#include <stdlib.h>
#include "flirt.h"

//	Compute gradient of T usTg central fTite differences; at the boundarys
//	DIRICHLET or NEUMANN are used,
//	d1T[j1][j2] = 1/2* ( T[j1+1][j2] - T[j1-1][j2])
//	d2T[j1][j2] = 1/2* ( T[j1][j2+1] - T[j1][j2-1])

void computeFDGrad2D(array2D T,
	size_t m1,
	size_t m2,
	double h1,
	double h2,
	boundary_t bc,
	array3D dT);

void computeFDGrad3D(array3D T,
	size_t m1,
	size_t m2,
	size_t m3,
	double h1,
	double h2,
	double h3,
	boundary_t bc,
	array4D dT);

//	Interpolate the two-dimensional image T linear, such that
//		Tphi[j1][j2] = T(j1+U1[j1][j2] , j2+U2[j1][j2])

void evalDisplacementLinear2D(array2D T,
	size_t m1,
	size_t m2,
	array2D U1,
	array2D U2,
	array2D Tphi);

void evalDisplacementLinear3D(array3D T,
	size_t m1,
	size_t m2,
	size_t m3,
	array3D U1,
	array3D U2,
	array3D U3,
	array3D Tphi);

#endif
