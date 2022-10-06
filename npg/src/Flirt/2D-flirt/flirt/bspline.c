#include "bspline.h"
#include "defs.h"
#include "fftw3.h"
#include <stdlib.h>
#include <math.h>



void computeBSplineCoeff1D(double I[],int N,double C[]) {
//  double inv_l[N];
//  double u[N-1];
//  int i;
//
//  /* compute LU factorization */
//  inv_l[0] = 6.0 / 4.0;
//  u[0]     = 1.0 / 4.0;
//  for (i=1; i<N-1; ++i) {
//    inv_l[i] = 1.0 / ((1.0/6.0) * (4.0 - u[i-1]));
//    u[i]     = (1.0/6.0) * inv_l[i];
//  }
//  inv_l[N-1] = 1.0 / ((1.0/6.0) * (4.0 - u[N-2]));
//
//
//  C[0] = I[0] * inv_l[0];
//  for (i=1; i<N; ++i) {
//    /* forward substitution */
//    C[i] = (I[i] - (1.0/6.0)*C[i-1]) * inv_l[i];
//  }
//
//  /* backward substitution */
//  for (i=N-2; i >= 0; --i) {
//    C[i] -= u[i]*C[i+1];
//  }
}

void computeBSplineCoeff2D(double I[],int m ,int n,double C[]) {
//  int N = (m>n) ? m : n;
//  double inv_l[N];
//  double u[N-1];
//  int i,j,ij;
//
//  /* compute LU factorization */
//  inv_l[0] = 6.0 / 4.0;
//  u[0]     = 1.0 / 4.0;
//  for (i=1; i<N-1; ++i) {
//    inv_l[i] = 1.0 / ((1.0/6.0) * (4.0 - u[i-1]));
//    u[i]     = (1.0/6.0) * inv_l[i];
//  }
//  inv_l[N-1] = 1.0 / ((1.0/6.0) * (4.0 - u[N-2]));
//
//
//  /* for all rows do */
//  for (i=0; i<m; ++i) {
//    /* forward substitution */
//    ij = i;
//    C[ij] = I[ij] * inv_l[0];
//    for (j=1; j<n; ++j) {
//      ij += m;
//      C[ij] = (I[ij] - (1.0/6.0)*C[ij-m]) * inv_l[j];
//    }
//
//    /* backward substitution */
//    ij = i+m*(n-1);
//    for (j=n-2; j >= 0; --j) {
//      ij -= m;
//      C[ij] -= u[j]*C[ij+m];
//    }
//  }
//
//
//  /* for all cols do */
//  for (j=0; j<n; ++j) {
//
//    /* forward substitution */
//    ij = m*j;
//    C[ij] *= inv_l[0];
//
//    for (i=1; i<m; ++i) {
//      ij ++;
//      C[ij] = (C[ij] - (1.0/6.0)*C[ij-1]) * inv_l[i];
//    }
//
//    /* backward substitution */
//    ij = (m-1)+m*j;
//    for (i=m-2; i >= 0; --i) {
//      ij--;
//      C[ij] -= u[i]*C[ij+1];
//    }
//  }

}


void computeBSplineCoeff3D(double I[], int m, int n, int p, double C[]) {
//  int N = (((m>n) ? m : n) > p) ? ((m>n) ? m : n) : p; /* N = max{m,n,p} */
//  double inv_l[N];
//  double u[N-1];
//  int i,j,k,ijk,mn;
//
//
//
//  /* compute LU factorization */
//  inv_l[0] = 6.0 / 4.0;
//  u[0]     = 1.0 / 4.0;
//  for (i=1; i<N-1; ++i) {
//    inv_l[i] = 1.0 / ((1.0/6.0) * (4.0 - u[i-1]));
//    u[i]     = (1.0/6.0) * inv_l[i];
//  }
//  inv_l[N-1] = 1.0 / ((1.0/6.0) * (4.0 - u[N-2]));
//
//
//  mn = m*n;
//
//
//
//  /* for the 3rd dimensions do */
//  for (i=0; i<m; ++i) {
//    for (j=0; j<n; ++j) {
//
//      /* forward substitution */
//      ijk = i + m*j;
//      C[ijk] = I[ijk] * inv_l[0];
//      for (k=1; k<p; ++k) {
//    ijk += mn;
//    C[ijk] = (I[ijk] - (1.0/6.0)*C[ijk-mn]) * inv_l[k];
//      }
//      /* backward substitution */
//      ijk = i + m*j + mn*(p-1);
//      for (k=p-2; k >= 0; --k) {
//    ijk -= mn;
//    C[ijk] -= u[k]*C[ijk+mn];
//      }
//
//    }
//  }
//
//
//  /* for the 2nd dimensions do */
//  for (i=0; i<m; ++i) {
//    for (k=0; k<p; ++k) {
//
//      /* forward substitution */
//      ijk = i + mn*k;
//      C[ijk] = C[ijk] * inv_l[0];
//      for (j=1; j<n; ++j) {
//    ijk +=m;
//    C[ijk] = (C[ijk] - (1.0/6.0)*C[ijk-m]) * inv_l[j];
//      }
//
//      /* backward substitution */
//      ijk = i + m*(n-1) + mn*k;
//      for (j=n-2; j >= 0; --j) {
//    ijk -= m;
//    C[ijk] -= u[j]*C[ijk+m];
//      }
//
//    }
//  }
//
//
//  /* for the 1st dimensions do */
//  for (j=0; j<n; ++j) {
//    for (k=0; k<p; ++k) {
//
//      /* forward substitution */
//      ijk = m*j + mn*k;
//      C[ijk] = C[ijk] * inv_l[0];
//      for (i=1; i<m; ++i) {
//    ijk++;
//    C[ijk] = (C[ijk] - (1.0/6.0)*C[ijk-1]) * inv_l[i];
//      }
//
//      /* backward substitution */
//      ijk = (m-1) + m*j + mn*k;
//      for (i=m-2; i >= 0; --i) {
//    ijk--;
//    C[ijk] -= u[i]*C[ijk+1];
//      }
//
//    }
//  }
//

}


void evalBSpline1D(double C[],
           int m ,
           double x[],
           int N,
           double S[]) {

  int i,cnt,index;
  int I[4];
  double w[4];

  double tau;

  for (cnt=0; cnt<N; ++cnt) {
    /* compute interpolation weights for the 1st dimension */
    tau = x[cnt] - floor(x[cnt]);
    w[0] = 1.0/6.0 * tau*tau*tau;
    w[3] = 1.0/6.0 + 0.5*tau*(tau-1.0) - w[0];
    w[1] = tau - 2.0*w[0] + w[3];
    w[2] = 1.0 - w[0] - w[1] - w[3];


    /* check range for the 1st dimension */
    index = ((int) floor(x[cnt])) + 2;
    for (i=0; i<4; ++i) {
      if ((index < 0) || (index >= m)) {
    I[i] = 0;
    w[i] = 0.0;
      } else {
    I[i] = index;
      }
      index--;
    }


    /* perform interpolation */
    S[cnt] = C[I[0]]*w[0] + C[I[1]]*w[1] + C[I[2]]*w[2] + C[I[3]]*w[3];

  }

}


void evalBSplineAndGrad1D(double C[],
              int m ,
              double x[],
              int N,
              double S[],
              double d_S[] ) {

  int i,cnt,index;
  int I[4];
  double w[4];
  double d_w[4];
  double tau;

  for (cnt=0; cnt<N; ++cnt) {
    /* compute interpolation weights for the 1st dimension */
    tau = x[cnt] - floor(x[cnt]);
    w[0] = 1.0/6.0 * tau*tau*tau;
    w[3] = 1.0/6.0 + 0.5*tau*(tau-1.0) - w[0];
    w[1] = tau - 2.0*w[0] + w[3];
    w[2] = 1.0 - w[0] - w[1] - w[3];

    d_w[0] = 0.5*tau*tau;
    d_w[3] = -0.5 + tau - d_w[0];
    d_w[1] = 1 - 2*d_w[0] + d_w[3];
    d_w[2] = -d_w[0] - d_w[1] - d_w[3];



    /* check range for the 1st dimension */
    index = ((int) floor(x[cnt])) + 2;
    for (i=0; i<4; ++i) {
      if ((index < 0) || (index >= m)) {
    I[i]    = 0;
    w[i]   = 0.0;
    d_w[i] = 0.0;
      } else {
    I[i] = index;
      }
      index--;
    }


    /* perform interpolation */

    S[cnt]   = C[I[0]]*w[0] + C[I[1]]*w[1] + C[I[2]]*w[2] + C[I[3]]*w[3];
    d_S[cnt] =
      C[I[0]]*d_w[0] +
      C[I[1]]*d_w[1] +
      C[I[2]]*d_w[2] +
      C[I[3]]*d_w[3];
  }

}


void evalBSpline2D(double C[],
           int m ,
           int n,
           double x[],
           double y[],
           int N,
           double S[]) {

  int i,j,cnt,index;
  int I[4];
  int J[4];
  double w1[4];
  double w2[4];

  double tau;

  for (cnt=0; cnt<N; ++cnt) {
    /* compute interpolation weights for the 1st dimension */
    tau = x[cnt] - floor(x[cnt]);
    w1[0] = 1.0/6.0 * tau*tau*tau;
    w1[3] = 1.0/6.0 + 0.5*tau*(tau-1.0) - w1[0];
    w1[1] = tau - 2.0*w1[0] + w1[3];
    w1[2] = 1.0 - w1[0] - w1[1] - w1[3];

    /* compute interpolation weights for the 2nd dimension */
    tau = y[cnt] - floor(y[cnt]);
    w2[0] = 1.0/6.0 * tau*tau*tau;
    w2[3] = 1.0/6.0 + 0.5*tau*(tau-1.0) - w2[0];
    w2[1] = tau - 2.0*w2[0] + w2[3];
    w2[2] = 1.0 - w2[0] - w2[1] - w2[3];

    /* check range for the 1st dimension */
    index = ((int) floor(x[cnt])) + 2;
    for (i=0; i<4; ++i) {
      if ((index < 0) || (index >= m)) {
    I[i] = 0;
    w1[i] = 0.0;
      } else {
    I[i] = index;
      }
      index--;
    }

    /* check range for the 2nd dimension */
    index = ((int) floor(y[cnt])) + 2;
    for (j=0; j<4; ++j) {
      if ((index < 0) || (index >= n)) {
    J[j] = 0;
    w2[j] = 0.0;
      } else {
    J[j] = m*index;
      }
      index--;
    }

    /* perform interpolation */
    S[cnt] = 0.0;
    for (i=0; i<4; ++i) {
      S[cnt] += w1[i]* ( C[ I[i]+J[0] ] * w2[0] +
             C[ I[i]+J[1] ] * w2[1] +
             C[ I[i]+J[2] ] * w2[2] +
             C[ I[i]+J[3] ] * w2[3]   );
    }

  }

}


void evalBSplineAndGrad2D(double C[],
              int m ,
              int n,
              double x[],
              double y[],
              int N,
              double S[],
              double d1_S[],
              double d2_S[] ) {

  int i,j,cnt,index;
  int I[4];
  int J[4];
  double w1[4];
  double d_w1[4];
  double w2[4];
  double d_w2[4];
  double sum_j;
  double tau;

  for (cnt=0; cnt<N; ++cnt) {
    /* compute interpolation weights for the 1st dimension */
    tau = x[cnt] - floor(x[cnt]);
    w1[0] = 1.0/6.0 * tau*tau*tau;
    w1[3] = 1.0/6.0 + 0.5*tau*(tau-1.0) - w1[0];
    w1[1] = tau - 2.0*w1[0] + w1[3];
    w1[2] = 1.0 - w1[0] - w1[1] - w1[3];

    d_w1[0] = 0.5*tau*tau;
    d_w1[3] = -0.5 + tau - d_w1[0];
    d_w1[1] = 1 - 2*d_w1[0] + d_w1[3];
    d_w1[2] = -d_w1[0] - d_w1[1] - d_w1[3];

    /* compute interpolation weights for the 2nd dimension */
    tau = y[cnt] - floor(y[cnt]);
    w2[0] = 1.0/6.0 * tau*tau*tau;
    w2[3] = 1.0/6.0 + 0.5*tau*(tau-1.0) - w2[0];
    w2[1] = tau - 2.0*w2[0] + w2[3];
    w2[2] = 1.0 - w2[0] - w2[1] - w2[3];

    d_w2[0] = 0.5*tau*tau;
    d_w2[3] = -0.5 + tau - d_w2[0];
    d_w2[1] = 1 - 2*d_w2[0] + d_w2[3];
    d_w2[2] = -d_w2[0] - d_w2[1] - d_w2[3];


    /* check range for the 1st dimension */
    index = ((int) floor(x[cnt])) + 2;
    for (i=0; i<4; ++i) {
      if ((index < 0) || (index >= m)) {
    I[i]    = 0;
    w1[i]   = 0.0;
    d_w1[i] = 0.0;
      } else {
    I[i] = index;
      }
      index--;
    }

    /* check range for the 2nd dimension */
    index = ((int) floor(y[cnt])) + 2;
    for (j=0; j<4; ++j) {
      if ((index < 0) || (index >= n)) {
    J[j]    = 0;
    w2[j]   = 0.0;
    d_w2[j] = 0.0;
      } else {
    J[j] = m*index;
      }
      index--;
    }

    /* perform interpolation */
    S[cnt]    = 0.0;
    d1_S[cnt] = 0.0;
    d2_S[cnt] = 0.0;
    for (i=0; i<4; ++i) {
      sum_j = ( C[ I[i]+J[0] ] * w2[0] +
        C[ I[i]+J[1] ] * w2[1] +
        C[ I[i]+J[2] ] * w2[2] +
        C[ I[i]+J[3] ] * w2[3]   );

      S[cnt]    +=   w1[i] * sum_j;
      d1_S[cnt] += d_w1[i] * sum_j;
      d2_S[cnt] +=   w1[i] * ( C[ I[i]+J[0] ] * d_w2[0] +
                   C[ I[i]+J[1] ] * d_w2[1] +
                   C[ I[i]+J[2] ] * d_w2[2] +
                   C[ I[i]+J[3] ] * d_w2[3]   );
    }


  }

}


void evalBSpline3D(double C[],
           int m ,
           int n,
           int p,
           double x[],
           double y[],
           double z[],
           int N,
           double S[]) {

  int i,j,k,ij,cnt,index,mn;
  int I[4];
  int J[4];
  int K[4];
  double w1[4];
  double w2[4];
  double w3[4];

  double tau,sum_jk,sum_k;

  mn = m*n;

  for (cnt=0; cnt<N; ++cnt) {
    /* compute interpolation weights for the 1st dimension */
    tau = x[cnt] - floor(x[cnt]);
    w1[0] = 1.0/6.0 * tau*tau*tau;
    w1[3] = 1.0/6.0 + 0.5*tau*(tau-1.0) - w1[0];
    w1[1] = tau - 2.0*w1[0] + w1[3];
    w1[2] = 1.0 - w1[0] - w1[1] - w1[3];

    /* compute interpolation weights for the 2nd dimension */
    tau = y[cnt] - floor(y[cnt]);
    w2[0] = 1.0/6.0 * tau*tau*tau;
    w2[3] = 1.0/6.0 + 0.5*tau*(tau-1.0) - w2[0];
    w2[1] = tau - 2.0*w2[0] + w2[3];
    w2[2] = 1.0 - w2[0] - w2[1] - w2[3];

    /* compute interpolation weights for the 3rd dimension */
    tau = z[cnt] - floor(z[cnt]);
    w3[0] = 1.0/6.0 * tau*tau*tau;
    w3[3] = 1.0/6.0 + 0.5*tau*(tau-1.0) - w3[0];
    w3[1] = tau - 2.0*w3[0] + w3[3];
    w3[2] = 1.0 - w3[0] - w3[1] - w3[3];


    /* check range for the 1st dimension */
    index = ((int) floor(x[cnt])) + 2;
    for (i=0; i<4; ++i) {
      if ((index < 0) || (index >= m)) {
    I[i] = 0;
    w1[i] = 0.0;
      } else {
    I[i] = index;
      }
      index--;
    }

    /* check range for the 2nd dimension */
    index = ((int) floor(y[cnt])) + 2;
    for (j=0; j<4; ++j) {
      if ((index < 0) || (index >= n)) {
    J[j] = 0;
    w2[j] = 0.0;
      } else {
    J[j] = m*index;
      }
      index--;
    }

    /* check range for the 3rd dimension */
    index = ((int) floor(z[cnt])) + 2;
    for (k=0; k<4; ++k) {
      if ((index < 0) || (index >= p)) {
    K[k] = 0;
    w3[k] = 0.0;
      } else {
    K[k] = mn*index;
      }
      index--;
    }

    /* perform interpolation */
    S[cnt] = 0.0;
    for (i=0; i<4; ++i) {
      sum_jk = 0.0;
      for (j=0; j<4; ++j) {
    sum_k = 0.0;
    ij = I[i]+J[j];
    for (k=0; k<4; ++k) {
      sum_k += w3[k] * C[ ij+K[k] ];
    }
    sum_jk += w2[j]*sum_k;
      }
      S[cnt] += w1[i]*sum_jk;
    }
  }

}

void evalBSplineAndGrad3D(double C[],
              int m ,
              int n,
              int p,
              double x[],
              double y[],
              double z[],
              int N,
              double S[],
              double d1_S[],
              double d2_S[],
              double d3_S[] ) {
  int i,j,k,ij,cnt,index,mn;
  int I[4];
  int J[4];
  int K[4];
  double w1[4];
  double w2[4];
  double w3[4];
  double d_w1[4];
  double d_w2[4];
  double d_w3[4];

  double tau,sum_jk,sum_k, d3_sum_k, d2_sum_jk, d3_sum_jk;

  mn = m*n;

  for (cnt=0; cnt<N; ++cnt) {
    /* compute interpolation weights for the 1st dimension */
    tau = x[cnt] - floor(x[cnt]);
    w1[0] = 1.0/6.0 * tau*tau*tau;
    w1[3] = 1.0/6.0 + 0.5*tau*(tau-1.0) - w1[0];
    w1[1] = tau - 2.0*w1[0] + w1[3];
    w1[2] = 1.0 - w1[0] - w1[1] - w1[3];

    d_w1[0] = 0.5*tau*tau;
    d_w1[3] = -0.5 + tau - d_w1[0];
    d_w1[1] = 1 - 2*d_w1[0] + d_w1[3];
    d_w1[2] = -d_w1[0] - d_w1[1] - d_w1[3];

    /* compute interpolation weights for the 2nd dimension */
    tau = y[cnt] - floor(y[cnt]);
    w2[0] = 1.0/6.0 * tau*tau*tau;
    w2[3] = 1.0/6.0 + 0.5*tau*(tau-1.0) - w2[0];
    w2[1] = tau - 2.0*w2[0] + w2[3];
    w2[2] = 1.0 - w2[0] - w2[1] - w2[3];

    d_w2[0] = 0.5*tau*tau;
    d_w2[3] = -0.5 + tau - d_w2[0];
    d_w2[1] = 1 - 2*d_w2[0] + d_w2[3];
    d_w2[2] = -d_w2[0] - d_w2[1] - d_w2[3];

    /* compute interpolation weights for the 3rd dimension */
    tau = z[cnt] - floor(z[cnt]);
    w3[0] = 1.0/6.0 * tau*tau*tau;
    w3[3] = 1.0/6.0 + 0.5*tau*(tau-1.0) - w3[0];
    w3[1] = tau - 2.0*w3[0] + w3[3];
    w3[2] = 1.0 - w3[0] - w3[1] - w3[3];

    d_w3[0] = 0.5*tau*tau;
    d_w3[3] = -0.5 + tau - d_w3[0];
    d_w3[1] = 1 - 2*d_w3[0] + d_w3[3];
    d_w3[2] = -d_w3[0] - d_w3[1] - d_w3[3];


    /* check range for the 1st dimension */
    index = ((int) floor(x[cnt])) + 2;
    for (i=0; i<4; ++i) {
      if ((index < 0) || (index >= m)) {
    I[i]   = 0;
    w1[i]  = 0.0;
    d_w1[i] = 0.0;
      } else {
    I[i] = index;
      }
      index--;
    }

    /* check range for the 2nd dimension */
    index = ((int) floor(y[cnt])) + 2;
    for (j=0; j<4; ++j) {
      if ((index < 0) || (index >= n)) {
    J[j]   = 0;
    w2[j]  = 0.0;
    d_w2[j] = 0.0;
      } else {
    J[j] = m*index;
      }
      index--;
    }

    /* check range for the 3rd dimension */
    index = ((int) floor(z[cnt])) + 2;
    for (k=0; k<4; ++k) {
      if ((index < 0) || (index >= p)) {
    K[k]   = 0;
    w3[k]  = 0.0;
    d_w3[k] = 0.0;
      } else {
    K[k] = mn*index;
      }
      index--;
    }

    /* perform interpolation */
    S[cnt] = 0.0;
    d1_S[cnt] = 0.0;
    d2_S[cnt] = 0.0;
    d3_S[cnt] = 0.0;
    for (i=0; i<4; ++i) {
      sum_jk = 0.0;
      d2_sum_jk = 0.0;
      d3_sum_jk = 0.0;
      for (j=0; j<4; ++j) {
    sum_k = 0.0;
    d3_sum_k = 0.0;
    ij = I[i]+J[j];
    for (k=0; k<4; ++k) {
      sum_k    +=   w3[k] * C[ ij+K[k] ];
      d3_sum_k += d_w3[k] * C[ ij+K[k] ];
    }
    sum_jk    +=   w2[j] *    sum_k;
    d2_sum_jk += d_w2[j] *    sum_k;
    d3_sum_jk +=   w2[j] * d3_sum_k;
      }
      S[cnt]    +=   w1[i] *    sum_jk;
      d1_S[cnt] += d_w1[i] *    sum_jk;
      d2_S[cnt] +=   w1[i] * d2_sum_jk;
      d3_S[cnt] +=   w1[i] * d3_sum_jk;
    }
  }

}





/* ++++++++++++++++ */
void convertToBSplineCoeff2D(double* C, int m1, int m2) {
  computeBSplineCoeff2D(C,m1,m2,C);
}



void computeApproxBSplineCoeff2D(double* I,
                 int m1,
                 int m2,
                 double tau,
                 boundary_t bc,
                 double* C) {
  int j1,j2,k;
  double* lambda1 = (double*) fftw_malloc(sizeof(double) * m1);
  double* lambda2 = (double*) fftw_malloc(sizeof(double) * m2);
  double* tmp     = (double*) fftw_malloc(sizeof(double) * m1*m2);

  double fftw_normfac = 1.0 / ((double) (2 *m1 * 2*m2));

  fftw_plan forward;
  fftw_plan backward;

  switch (bc) {
  case DIRICHLET:
    forward  = fftw_plan_r2r_2d(m2,m1, I  , tmp, FFTW_RODFT10, FFTW_RODFT10, FFTW_ESTIMATE | FFTW_PRESERVE_INPUT);
    backward = fftw_plan_r2r_2d(m2,m1, tmp, C  , FFTW_RODFT01, FFTW_RODFT01, FFTW_ESTIMATE);
    /* precompute eigenvalues */
    for (k=0; k<m1 ; ++k)
      lambda1[k] = (2.0 - 2.0*cos( ((double)(k+1))*PI / ((double)m1) ));
    for (k=0; k<m2 ; ++k)
      lambda2[k] = (2.0 - 2.0*cos( ((double)(k+1))*PI / ((double)m2) ));
    break;

  case NEUMANN:
    forward  = fftw_plan_r2r_2d(m2,m1, I  , tmp, FFTW_REDFT10, FFTW_REDFT10, FFTW_ESTIMATE | FFTW_PRESERVE_INPUT);
    backward = fftw_plan_r2r_2d(m2,m1, tmp, C  , FFTW_REDFT01, FFTW_REDFT01, FFTW_ESTIMATE);
    /* precompute eigenvalues */
    for (k=0; k<m1 ; ++k)
      lambda1[k] = (2.0 - 2.0*cos( ((double)k)*PI / ((double)m1) ));
    for (k=0; k<m2 ; ++k)
      lambda2[k] = (2.0 - 2.0*cos( ((double)k)*PI / ((double)m2) ));

  }



  fftw_execute(forward);

  for (j2=0; j2<m2; ++j2)
    for (j1=0; j1<m1; ++j1)
      tmp[j1+m1*j2] *= 1.0/(1.0+tau*(lambda1[j1]+lambda2[j2])*(lambda1[j1]+lambda2[j2]) );

  fftw_execute(backward);

  convertToBSplineCoeff2D(C,m1,m2);

  fftw_destroy_plan(forward);
  fftw_destroy_plan(backward);
  fftw_free(lambda1);
  fftw_free(lambda2);
  fftw_free(tmp);
}



/* ++++++++++++++++ */
void convertToBSplineCoeff3D(double* C, int m1, int m2,int m3) {
  computeBSplineCoeff3D(C,m1,m2,m3,C);
}



void computeApproxBSplineCoeff3D(double* I,
                 int m1,
                 int m2,
                 int m3,
                 double tau,
                 boundary_t bc,
                 double* C) {
  int j1,j2,j3,k;
  double* lambda1 = (double*) fftw_malloc(sizeof(double) * m1);
  double* lambda2 = (double*) fftw_malloc(sizeof(double) * m2);
  double* lambda3 = (double*) fftw_malloc(sizeof(double) * m3);
  double* tmp     = (double*) fftw_malloc(sizeof(double) * m1*m2*m3);

  double fftw_normfac = 1.0 / ((double) (2 *m1 * 2*m2 * 2*m3));

  fftw_plan forward;
  fftw_plan backward;

  switch (bc) {
  case DIRICHLET:
    forward  = fftw_plan_r2r_3d(m3,m2,m1, I  , tmp,
                FFTW_RODFT10, FFTW_RODFT10, FFTW_RODFT10,
                FFTW_ESTIMATE | FFTW_PRESERVE_INPUT);
    backward = fftw_plan_r2r_3d(m3,m2,m1, tmp, C  ,
                FFTW_RODFT01, FFTW_RODFT01, FFTW_RODFT01,
                FFTW_ESTIMATE);
    /* precompute eigenvalues */
    for (k=0; k<m1 ; ++k)
      lambda1[k] = (2.0 - 2.0*cos( ((double)(k+1))*PI / ((double)m1) ));
    for (k=0; k<m2 ; ++k)
      lambda2[k] = (2.0 - 2.0*cos( ((double)(k+1))*PI / ((double)m2) ));
    for (k=0; k<m3 ; ++k)
      lambda3[k] = (2.0 - 2.0*cos( ((double)(k+1))*PI / ((double)m3) ));
    break;

  case NEUMANN:
    forward  = fftw_plan_r2r_3d(m3,m2,m1, I  , tmp,
                FFTW_REDFT10, FFTW_REDFT10, FFTW_REDFT10,
                FFTW_ESTIMATE | FFTW_PRESERVE_INPUT);
    backward = fftw_plan_r2r_3d(m3,m2,m1, tmp, C  ,
                FFTW_REDFT01, FFTW_REDFT01, FFTW_REDFT01,
                FFTW_ESTIMATE);
    /* precompute eigenvalues */
    for (k=0; k<m1 ; ++k)
      lambda1[k] = (2.0 - 2.0*cos( ((double)k)*PI / ((double)m1) ));
    for (k=0; k<m2 ; ++k)
      lambda2[k] = (2.0 - 2.0*cos( ((double)k)*PI / ((double)m2) ));
    for (k=0; k<m3 ; ++k)
      lambda3[k] = (2.0 - 2.0*cos( ((double)k)*PI / ((double)m3) ));

  }



  fftw_execute(forward);

  for (j3=0; j3<m3; ++j3)
    for (j2=0; j2<m2; ++j2)
      for (j1=0; j1<m1; ++j1)
    tmp[j1+m1*(j2+m2*j3)] *=
      1.0/(1.0+tau*(lambda1[j1]+lambda2[j2]+lambda3[j3])*(lambda1[j1]+lambda2[j2]+lambda3[j3]) );

  fftw_execute(backward);

  convertToBSplineCoeff3D(C,m1,m2,m3);

  fftw_destroy_plan(forward);
  fftw_destroy_plan(backward);
  fftw_free(lambda1);
  fftw_free(lambda2);
  fftw_free(lambda3);
  fftw_free(tmp);
}
