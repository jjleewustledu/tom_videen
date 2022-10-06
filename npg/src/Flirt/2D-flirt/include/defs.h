#ifndef _DEFS_H_
#define _DEFS_H_

#include <math.h>

/** \file defs.h
\brief Some definitions...
*/


/**
\brief Boundary conditions; self explaining ;o)
\author Stefan Heldman
*/
typedef enum {
    DIRICHLET , /**< Dirichlet-Null BC */
    NEUMANN,    /**< Neumann-Null BC */
    PERIODIC    /**< periodic BC */
} boundary_t;

/**
    \brief Grid symmetry of the discretization
    \author Stefan Heldman
 */
typedef enum {
  GRIDPOINT , /**< grid-point symmetric grid.    */
  MIDPOINT    /**< mid-point symmetric grid.   */
} gridsymmetry_t;


/** \brief The famous constant
*/
#define PI M_PI


//#define max(a,b) ((a)>(b))?(a):(b)
#define maxAbs(a,b) max(fabs(a),fabs(b))

#endif
