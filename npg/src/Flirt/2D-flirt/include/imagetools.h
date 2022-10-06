#ifndef _IMAGETOOL_H_
#define _IMAGETOOL_H_

#include <stdlib.h>
#include "flirt.h"

/** \file imagetools.h
    \brief basic image stuff; declaraition linear interpolation, etc...
    \author Stefan Heldmann, Nils Papenberg
    \date 20/04/2005
*/




/** \brief Compute gradient of T usTg central fTite differences; at the boundarys
    DIRICHLET or NEUMANN are used,
    d1T[j1][j2] = 1/2* ( T[j1+1][j2] - T[j1-1][j2])
    d2T[j1][j2] = 1/2* ( T[j1][j2+1] - T[j1][j2-1])
    \author Stefan Heldmann, Nils Papenberg
    \date 20/04/2005
*/
void computeFDGrad2D(array2D T,
             size_t m1,
             size_t m2,
                     double h1,
                     double h2,
             boundary_t bc,
             array3D dT);

/* \brief Interpolate the two-dimensional image T linear, such that
    TI[i] = T(X1[i] , X2[i])
*/
/*
void interpLinear2D(array2D T,
            size_t m1,
            size_t m2,
            double* X1,
            double* X2,
            size_t len,
            double* TI);
*/
/** \brief Interpolate the two-dimensional image T linear, such that
        Tphi[j1][j2] = T(j1+U1[j1][j2] , j2+U2[j1][j2])
    \author Stefan Heldmann, Nils Papenberg
    \date 20/04/2005
*/
void evalDisplacementLinear2D(array2D T,
                  size_t m1,
                  size_t m2,
                  array2D U1,
                  array2D U2,
                  array2D Tphi);

#endif
