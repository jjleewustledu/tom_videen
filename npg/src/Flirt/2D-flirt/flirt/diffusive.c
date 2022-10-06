// (c) NP, 2005
#include <stdio.h>
#include "diffusive.h"

// alpha = alpha * tau * ( 1 / h);
void initTriDiagMatrix(size_t m,
               double alpha,
               boundary_t bc,
               triDiagMatrix* pMatrix)
{
    double factor;
    size_t j,k;
    triDiagMatrix M;
    M       = (triDiagMatrix) malloc(sizeof(triDiagMatrix_t));
    M->diag = (double*) malloc(sizeof(double) * m);
    M->up   = (double*) malloc(sizeof(double) * (m-1));
    M->low  = (double*) malloc(sizeof(double) * (m-1));

    M->len = m;

    factor = alpha;
    for (k = 1; k < m; k ++) {
        M->diag[k] = 1 + 2 * factor;
        M->low[k-1] = (-1.0)*factor;
        M->up[k-1] = (-1.0)*factor;
    }

    if (bc == DIRICHLET) {
        factor = alpha ;
        M->diag[0] = 1 + 2*factor;
        M->diag[m-1] = 1 + 2*factor;
    }

    if (bc == NEUMANN) {
      factor = alpha;
      M->diag[0] = 1 + factor;
      M->diag[m-1] = 1 + factor;
    }

    // factorize
    M->up[0] = M->up[0] / M->diag[0];
    for (j = 1; j < m-1; j++) {
        M->diag[j] = (M->diag[j] - M->low[j-1] * M->up[j-1]);
        M->up[j] = M->up[j] / M->diag[j];
    }

    M->diag[m-1] =  (M->diag[m-1] - M->low[m-2] * M->up[m-2]);

    *(pMatrix) = M;
}

/* --------------------------------------------------------------------- */

void initDiffusivePlan2D(size_t m1,
             size_t m2,
             double alpha,
             double tau,
             double h1,
             double h2,
             boundary_t bc,
             diffusivePlan* pPlan)
{

  diffusivePlan M;
  M = (diffusivePlan) malloc(sizeof(diffusivePlan_t));

  initTriDiagMatrix(m1, 2*alpha*tau/h1, bc, &(M->M1));
  initTriDiagMatrix(m2, 2*alpha*tau/h2, bc, &(M->M2));
  M->M3 = NULL;

  /** SB */
#if 0
  M->tmp = createArray2D(m2,m1);
#endif
  M->tmp = createTreeStructure2D(m2,m1, (array1D) calloc(m2*m1,sizeof(array_t)));
  /** \SB */

  M->alpha = alpha;
  M->tau = tau;
  M->dim = 2;
  M->m1 = m1;
  M->m2 = m2;
  M->m3 = 1;
  M->h1 = h1;
  M->h2 = h2;
  M->h3 = 0;

  /* printf("%s %d (Build %s %s) -- \n"
     ,__FILE__,__LINE__,__DATE__,__TIME__); */
  /*   printDiffusivePlan(M);   */

  *(pPlan) = M;
}

/* --------------------------------------------------------------------- */

void solveThomas(triDiagMatrix A,
         double* b,
         int inc,
         double* x)
{
  size_t j,k;
  /* Kopieren der rechten Seite nach x */
  /* und Vorwaertseinsetzen */

  x[0] = b[0] / A->diag[0];
  for (j = inc,k=1; k < A->len; j += inc, k++) {
    x[j] = (b[j] - A->low[k-1]*x[j-inc]) / A->diag[k];
  }

  /* Rueckwaertseinsetzen */
  for (j = (A->len-2)*inc, k=A->len-2; k > -1; j-=inc, k--) {
    x[j] = x[j] - A->up[k] * x[j+inc];
  }
}

/* --------------------------------------------------------------------- */

void printDiffusivePlan(const diffusivePlan M) {
  printf("-- diffusivePlan at %p\n",M);
  printf("%30s : %p\n","M->M1",M->M1);
  printf("%30s : %p\n","M->M2",M->M2);
  printf("%30s : %p\n","M->M3",M->M3);
  printf("%30s : %p\n","M->tmp",M->tmp);
  printf("%30s : %g\n","M->alpha",M->alpha);
  printf("%30s : %g\n","M->tau",M->tau);
  printf("%30s : %d\n","M->dim",M->dim);
  printf("%30s : %g\n","M->h1",M->h1);
  printf("%30s : %g\n","M->h2",M->h2);
  printf("%30s : %g\n","M->h3",M->h3);
  printf("%30s : %d\n","M->m1",M->m1);
  printf("%30s : %d\n","M->m2",M->m2);
  printf("%30s : %d\n","M->m3",M->m3);
}
/* ---------------------------------------------------------------------- */

void printTriDiagMatrix(const triDiagMatrix A)
{
    size_t k1,k2;
    printf("printTriDiagMatrix \n");
    for (k1 = 0; k1 < A->len - 1; k1++) {
                if (k1 == 0)
                {
                    printf("%g\t%g\n",A->diag[k1],A->up[k1]);
                }
                else
                {
                    for (k2 = 0; k2 < k1-1; k2++) printf("\t");
                    printf("%g\t%g\t%g\n",A->low[k1-1],A->diag[k1],A->up[k1]);
                }
        }
    for (k2 = 0; k2 < k1-1; k2++) printf("\t");
    printf("%g\t%g\n\n",A->low[k1-1],A->diag[k1]);


}

/* ---------------------------------------------------------------------- */

void solveDiffusive2D(diffusivePlan M,
              double** b,
              double** x)
{
  int j1, j2;

  for (j2 = 0; j2 < M->m2; j2++) {
    solveThomas(M->M1,b[j2],1,(double*)(M->tmp[j2]));
  }


  for (j1 = 0; j1 < M->m1; j1++) {
    solveThomas(M->M2,b[0]+j1,M->m1, x[0]+j1);
  }

  /*
  printf("\n");
  printToFileArray2D(M->tmp,M->m2,M->m1, stdout);
  printf("\n");
  printToFileArray2D(x,M->m2,M->m1, stdout);
  */
  /* Averaging results */
  mult(0.5, M->tmp[0],M->tmp[0], M->m1 * M->m2);
  mult(0.5, x[0], x[0], M->m1 * M->m2);

  add(M->tmp[0], x[0], x[0], M->m1 * M->m2);
}

    /* --------------------------------------------------------------------- */
void deleteTriDiagMatrix(triDiagMatrix A)
{
    free(A->diag);
    free(A->low);
    free(A->up);
    free(A);
}
/* --------------------------------------------------------------------- */
void deleteDiffusivePlan(diffusivePlan M)
{
    if (M->dim == 2) {
        deleteTriDiagMatrix(M->M1);
        deleteTriDiagMatrix(M->M2);
    }
    deleteArray2D(M->tmp);
    free(M);
}
