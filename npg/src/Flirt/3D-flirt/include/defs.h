//	File defs.h
//	Author Stefan Heldman

#ifndef _DEFS_H_
#define _DEFS_H_

#include <math.h>

// Boundary conditions; self explaining ;o)

typedef enum
{
	DIRICHLET,									  /* Dirichlet-Null BC */
	NEUMANN,									  /* Neumann-Null BC */
	PERIODIC									  /* periodic BC */
} boundary_t;

// Grid symmetry of the discretization

typedef enum
{
	GRIDPOINT,									  // grid-point symmetric grid
	MIDPOINT									  // mid-point symmetric grid
} gridsymmetry_t;

#define PI M_PI
#define max(a,b) ((a)>(b))?(a):(b)
#define maxAbs(a,b) max(fabs(a),fabs(b))
#endif
