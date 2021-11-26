/*--------------------------------------------------------------------
  
  NAS Parallel Benchmarks 2.3 OpenMP C versions - SP

  This benchmark is an OpenMP C version of the NPB SP code.
  
  The OpenMP C versions are developed by RWCP and derived from the serial
  Fortran versions in "NPB 2.3-serial" developed by NAS.

  Permission to use, copy, distribute and modify this software for any
  purpose with or without fee is hereby granted.
  This software is provided "as is" without express or implied warranty.
  
  Send comments on the OpenMP C versions to pdp-openmp@rwcp.or.jp

  Information on OpenMP activities at RWCP is available at:

           http://pdplab.trc.rwcp.or.jp/pdperf/Omni/
  
  Information on NAS Parallel Benchmarks 2.3 is available at:
  
           http://www.nas.nasa.gov/NAS/NPB/

--------------------------------------------------------------------*/
/*--------------------------------------------------------------------

  Author: R. Van der Wijngaart
          W. Saphir

  OpenMP C version: S. Satoh
  
--------------------------------------------------------------------*/

//#include "npb-C.h"
/*
  NAS Parallel Benchmarks 2.3 OpenMP C Versions
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#if defined(_OPENMP)
#include <omp.h>
#include "Heartbeat_Support_OpenMP.h" 
#include<sys/time.h>
#endif /* _OPENMP */

typedef int boolean;
typedef struct { double real; double imag; } dcomplex;

#define TRUE    1
#define FALSE   0

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define pow2(a) ((a)*(a))

#define get_real(c) c.real
#define get_imag(c) c.imag
#define cadd(c,a,b) (c.real = a.real + b.real, c.imag = a.imag + b.imag)
#define csub(c,a,b) (c.real = a.real - b.real, c.imag = a.imag - b.imag)
#define cmul(c,a,b) (c.real = a.real * b.real - a.imag * b.imag, \
                     c.imag = a.real * b.imag + a.imag * b.real)
#define crmul(c,a,b) (c.real = a.real * b, c.imag = a.imag * b)

extern double randlc(double *, double);
extern void vranlc(int, double *, double, double *);
extern void timer_clear(int);
extern void timer_start(int);
extern void timer_stop(int);
extern double timer_read(int);

extern void c_print_results(char *name, char cclass, int n1, int n2,
                            int n3, int niter, int nthreads, double t,
                            double mops, char *optype, int passed_verification,
                            char *npbversion, char *compiletime, char *cc,
                            char *clink, char *c_lib, char *c_inc,
                            char *cflags, char *clinkflags, char *rand);


/* global variables */
//#include "header.h"

/******************/
/* default values */
/******************/
#ifndef CLASS
#define CLASS 'A'
#endif

#if CLASS == 'S'
/* CLASS = S */
/*
c  This file is generated automatically by the setparams utility.
c  It sets the number of processors and the classc of the NPB
c  in this directory. Do not modify it by hand.
*/
#define PROBLEM_SIZE    12
#define NITER_DEFAULT   100
#define DT_DEFAULT      0.015
#define CONVERTDOUBLE   FALSE
#endif

#if CLASS == 'W'
/* CLASS = W */
/*
c  This file is generated automatically by the setparams utility.
c  It sets the number of processors and the classc of the NPB
c  in this directory. Do not modify it by hand.
*/
#define PROBLEM_SIZE    36
#define NITER_DEFAULT   400
#define DT_DEFAULT      0.0015
#define CONVERTDOUBLE   FALSE
#endif

#if CLASS == 'A'
/* CLASS = A */
/*
c  This file is generated automatically by the setparams utility.
c  It sets the number of processors and the classc of the NPB
c  in this directory. Do not modify it by hand.
*/
#define PROBLEM_SIZE    64
#define NITER_DEFAULT   400
#define DT_DEFAULT      0.0015
#define CONVERTDOUBLE   FALSE
#endif

#if CLASS == 'B'
/* CLASS = B */
/*
c  This file is generated automatically by the setparams utility.
c  It sets the number of processors and the classc of the NPB
c  in this directory. Do not modify it by hand.
*/
#define PROBLEM_SIZE    102
#define NITER_DEFAULT   400
#define DT_DEFAULT      0.001
#define CONVERTDOUBLE   FALSE
#endif

#if CLASS == 'C'
/* CLASS = C */
/*
c  This file is generated automatically by the setparams utility.
c  It sets the number of processors and the classc of the NPB
c  in this directory. Do not modify it by hand.
*/
#define PROBLEM_SIZE    162
#define NITER_DEFAULT   400
#define DT_DEFAULT      0.00067
#define CONVERTDOUBLE   FALSE
#endif

#define COMPILETIME "28 Oct 2014"
#define NPBVERSION "2.3"
#define CS1 "gcc"
#define CS2 "$(CC)"
#define CS3 "(none)"
#define CS4 "-I../common"
#define CS5 "-fopenmp -O2"
#define CS6 "-lm -fopenmp"
#define CS7 "randdp"

/* common /global */
static int grid_points[3];

/* common /constants/ */
static double tx1, tx2, tx3, ty1, ty2, ty3, tz1, tz2, tz3,
              dx1, dx2, dx3, dx4, dx5, dy1, dy2, dy3, dy4,
              dy5, dz1, dz2, dz3, dz4, dz5, dssp, dt,
              ce[13][5], dxmax, dymax, dzmax, xxcon1, xxcon2,
              xxcon3, xxcon4, xxcon5, dx1tx1, dx2tx1, dx3tx1,
              dx4tx1, dx5tx1, yycon1, yycon2, yycon3, yycon4,
              yycon5, dy1ty1, dy2ty1, dy3ty1, dy4ty1, dy5ty1,
              zzcon1, zzcon2, zzcon3, zzcon4, zzcon5, dz1tz1,
              dz2tz1, dz3tz1, dz4tz1, dz5tz1, dnxm1, dnym1,
              dnzm1, c1c2, c1c5, c3c4, c1345, conz1, c1, c2,
              c3, c4, c5, c4dssp, c5dssp, dtdssp, dttx1, bt,
              dttx2, dtty1, dtty2, dttz1, dttz2, c2dttx1,
              c2dtty1, c2dttz1, comz1, comz4, comz5, comz6,
              c3c4tx3, c3c4ty3, c3c4tz3, c2iv, con43, con16;


#define IMAX    PROBLEM_SIZE
#define JMAX    PROBLEM_SIZE
#define KMAX    PROBLEM_SIZE

/*--------------------------------------------------------------------
c   To improve cache performance, first two dimensions padded by 1 
c   for even number sizes only
c-------------------------------------------------------------------*/

/* common /fields/ */
static double u       [5][IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1],
              us      [IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1],
              vs      [IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1],
              ws      [IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1],
              qs      [IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1],
              ainv    [IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1],
              rho_i   [IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1],
              speed   [IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1],
              square  [IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1],
              rhs     [5][IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1],
              forcing [5][IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1],
              lhs     [15][IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1];

/* common /work_1d/ */
static double cv[PROBLEM_SIZE], rhon[PROBLEM_SIZE],
              rhos[PROBLEM_SIZE], rhoq[PROBLEM_SIZE],
              cuf[PROBLEM_SIZE], q[PROBLEM_SIZE],
              ue[5][PROBLEM_SIZE], buf[5][PROBLEM_SIZE];


/* function declarations */
static void add(void);
static void adi(void);
static void error_norm(double rms[5]);
static void rhs_norm(double rms[5]);
static void exact_rhs(void);
static void exact_solution(double xi, double eta, double zeta,
			   double dtemp[5]);
static void initialize(void);
static void lhsinit(void);
static void lhsx(void);
static void lhsy(void);
static void lhsz(void);
static void ninvr(void);
static void pinvr(void);
static void compute_rhs(void);
static void set_constants(void);
static void txinvr(void);
static void tzetar(void);
static void verify(int no_time_steps, char *cclass, boolean *verified);
static void x_solve(void);
static void y_solve(void);
static void z_solve(void);

/*--------------------------------------------------------------------
       program SP
c-------------------------------------------------------------------*/
int main(int argc, char **argv) {
    
  int niter, step;
  double mflops, tmax;
  int nthreads = 1;
  boolean verified;
  char cclass;
  FILE *fp;
//**************************************
    L_Heartbeat_OpenMP_Init();             
//**************************************



/*--------------------------------------------------------------------
c      Read input file (if it exists), else take
c      defaults from parameters
c-------------------------------------------------------------------*/

  printf("\n\n NAS Parallel Benchmarks 2.3 OpenMP C version"
	 " - SP Benchmark\n\n");

  fp = fopen("inputsp.data", "r");
  if (fp != NULL) {
      printf(" Reading from input file inputsp.data\n");
      fscanf(fp, "%d", &niter);
      while (fgetc(fp) != '\n');
      fscanf(fp, "%lf", &dt);
      while (fgetc(fp) != '\n');
      fscanf(fp, "%d%d%d",
	     &grid_points[0], &grid_points[1], &grid_points[2]);
      fclose(fp);
  } else {
      printf(" No input file inputsp.data. Using compiled defaults");

      niter = NITER_DEFAULT;
      dt = DT_DEFAULT;
      grid_points[0] = PROBLEM_SIZE;
      grid_points[1] = PROBLEM_SIZE;
      grid_points[2] = PROBLEM_SIZE;
  }

  printf(" Size: %3dx%3dx%3d\n",
	 grid_points[0], grid_points[1], grid_points[2]);
  printf(" Iterations: %3d   dt: %10.6f\n", niter, dt);
  
  if ( (grid_points[0] > IMAX) ||
       (grid_points[1] > JMAX) ||
       (grid_points[2] > KMAX) ) {
    printf("%d, %d, %d\n", grid_points[0], grid_points[1], grid_points[2]);
    printf(" Problem size too big for compiled array sizes\n");
    exit(1);
  }

  set_constants();

  initialize();

  lhsinit();

  exact_rhs();
  
/*--------------------------------------------------------------------
c      do one time step to touch all code, and reinitialize
c-------------------------------------------------------------------*/

#pragma omp parallel
  {  
    adi();
  }
  
  initialize();

  timer_clear(1);
  timer_start(1);
#pragma omp parallel private(step) num_threads(4)
  {  
  for (step = 1; step <= niter; step++) {
    if (step % 20 == 0 || step == 1) {
#pragma omp master	
      printf(" Time step %4d\n", step);
    }
    adi();
  }
  
#if defined(_OPENMP)
#pragma omp master
  nthreads = omp_get_num_threads();
#endif /* _OPENMP */  
  } /* end parallel */

  timer_stop(1);
  tmax = timer_read(1);

  verify(niter, &cclass, &verified);

  if (tmax != 0) {
    mflops = ( 881.174 * pow((double)PROBLEM_SIZE, 3.0)
	       - 4683.91 * pow2((double)PROBLEM_SIZE)
	       + 11484.5 * (double)PROBLEM_SIZE
	       - 19272.4) * (double)niter / (tmax*1000000.0);
  } else {
    mflops = 0.0;
  }
  
  c_print_results("SP", cclass, grid_points[0],
		  grid_points[1], grid_points[2], niter, nthreads,
		  tmax, mflops, "          floating point", 
		  verified, NPBVERSION, COMPILETIME, CS1, CS2, CS3, CS4, CS5, 
		  CS6, "(none)");

  L_Heartbeat_OpenMP_Finished();


}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

static void add(void) {
  int i, j, k, m,loopnum=0;

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c addition of update to the vector u
c-------------------------------------------------------------------*/
#pragma omp for
  for (m = 0; m < 5; m++) {
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	for (k = 1; k <= grid_points[2]-2; k++) {
	  u[m][i][j][k] = u[m][i][j][k] + rhs[m][i][j][k];
  }
      }
    }
  }
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

static void adi(void) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

  compute_rhs();

  txinvr();
  
  x_solve();

  y_solve();

  z_solve();

  add();

  //************************************

  //************************************
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

static void error_norm(double rms[5]) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c this function computes the norm of the difference between the
c computed solution and the exact solution
c-------------------------------------------------------------------*/

  int i, j, k, m, d;
  double xi, eta, zeta, u_exact[5], add;

  for (m = 0; m < 5; m++) {
    rms[m] = 0.0;
  }

  for (i = 0; i <= grid_points[0]-1; i++) {
    xi = (double)i * dnxm1;
    for (j = 0; j <= grid_points[1]-1; j++) {
      eta = (double)j * dnym1;
      for (k = 0; k <= grid_points[2]-1; k++) {
	zeta = (double)k * dnzm1;
	exact_solution(xi, eta, zeta, u_exact);
	for (m = 0; m < 5; m++) {
	  add = u[m][i][j][k] - u_exact[m];
	  rms[m] = rms[m] + add*add;
	}
      }
    }
  }

  for (m = 0; m < 5; m++) {
    for (d = 0; d < 3; d++) {
      rms[m] = rms[m] / (double)(grid_points[d]-2);
    }
    rms[m] = sqrt(rms[m]);
  }
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

static void rhs_norm(double rms[5]) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

  int i, j, k, d, m;
  double add;

  for (m = 0; m < 5; m++) {
    rms[m] = 0.0;
  }

  for (i = 0; i <= grid_points[0]-2; i++) {
    for (j = 0; j <= grid_points[1]-2; j++) {
  for (k = 0; k <= grid_points[2]-2; k++) {
	for (m = 0; m < 5; m++) {
	  add = rhs[m][i][j][k];
	  rms[m] = rms[m] + add*add;
	}
      }
    }
  }

  for (m = 0; m < 5; m++) {
    for (d = 0; d < 3; d++) {
      rms[m] = rms[m] / (double)(grid_points[d]-2);
    }
    rms[m] = sqrt(rms[m]);
  }
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

static void exact_rhs(void) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c compute the right hand side based on exact solution
c-------------------------------------------------------------------*/

  double dtemp[5], xi, eta, zeta, dtpp;
  int m, i, j, k, ip1, im1, jp1, jm1, km1, kp1;

/*--------------------------------------------------------------------
c      initialize                                  
c-------------------------------------------------------------------*/
  for (m = 0; m < 5; m++) {
    for (i = 0; i <= grid_points[0]-1; i++) {
      for (j = 0; j <= grid_points[1]-1; j++) {
	for (k= 0; k <= grid_points[2]-1; k++) {
	  forcing[m][i][j][k] = 0.0;
	}
      }
    }
  }

/*--------------------------------------------------------------------
c      xi-direction flux differences                      
c-------------------------------------------------------------------*/
  for (k = 1; k <= grid_points[2]-2; k++) {
    zeta = (double)k * dnzm1;
    for (j = 1; j <= grid_points[1]-2; j++) {
      eta = (double)j * dnym1;

      for (i = 0; i <= grid_points[0]-1; i++) {
	xi = (double)i * dnxm1;

	exact_solution(xi, eta, zeta, dtemp);
	for (m = 0; m < 5; m++) {
	  ue[m][i] = dtemp[m];
	}

	dtpp = 1.0 / dtemp[0];

	for (m = 1; m < 5; m++) {
	  buf[m][i] = dtpp * dtemp[m];
	}

	cuf[i] = buf[1][i] * buf[1][i];
	buf[0][i] = cuf[i] + buf[2][i] * buf[2][i] + buf[3][i] * buf[3][i];
	q[i] = 0.5 * (buf[1][i]*ue[1][i] + buf[2][i]*ue[2][i]
		      + buf[3][i]*ue[3][i]);
      }
 
      for (i = 1; i <= grid_points[0]-2; i++) {
	im1 = i-1;
	ip1 = i+1;

	forcing[0][i][j][k] = forcing[0][i][j][k] -
	  tx2*( ue[1][ip1]-ue[1][im1] )+
	  dx1tx1*(ue[0][ip1]-2.0*ue[0][i]+ue[0][im1]);

	forcing[1][i][j][k] = forcing[1][i][j][k]
	  - tx2 * ((ue[1][ip1]*buf[1][ip1]+c2*(ue[4][ip1]-q[ip1]))-
                   (ue[1][im1]*buf[1][im1]+c2*(ue[4][im1]-q[im1])))+
	  xxcon1*(buf[1][ip1]-2.0*buf[1][i]+buf[1][im1])+
	  dx2tx1*( ue[1][ip1]-2.0* ue[1][i]+ue[1][im1]);

	forcing[2][i][j][k] = forcing[2][i][j][k]
	  - tx2 * (ue[2][ip1]*buf[1][ip1]-ue[2][im1]*buf[1][im1])+
	  xxcon2*(buf[2][ip1]-2.0*buf[2][i]+buf[2][im1])+
	  dx3tx1*( ue[2][ip1]-2.0*ue[2][i] +ue[2][im1]);
                  
	forcing[3][i][j][k] = forcing[3][i][j][k]
	  - tx2*(ue[3][ip1]*buf[1][ip1]-ue[3][im1]*buf[1][im1])+
	  xxcon2*(buf[3][ip1]-2.0*buf[3][i]+buf[3][im1])+
	  dx4tx1*( ue[3][ip1]-2.0* ue[3][i]+ ue[3][im1]);

	forcing[4][i][j][k] = forcing[4][i][j][k]
	  - tx2*(buf[1][ip1]*(c1*ue[4][ip1]-c2*q[ip1])-
		 buf[1][im1]*(c1*ue[4][im1]-c2*q[im1]))+
	  0.5*xxcon3*(buf[0][ip1]-2.0*buf[0][i]+
		      buf[0][im1])+
	  xxcon4*(cuf[ip1]-2.0*cuf[i]+cuf[im1])+
	  xxcon5*(buf[4][ip1]-2.0*buf[4][i]+buf[4][im1])+
	  dx5tx1*( ue[4][ip1]-2.0* ue[4][i]+ ue[4][im1]);
      }

/*--------------------------------------------------------------------
c            Fourth-order dissipation                         
c-------------------------------------------------------------------*/
      for (m = 0; m < 5; m++) {
	i = 1;
	forcing[m][i][j][k] = forcing[m][i][j][k] - dssp *
	  (5.0*ue[m][i] - 4.0*ue[m][i+1] +ue[m][i+2]);
	i = 2;
	forcing[m][i][j][k] = forcing[m][i][j][k] - dssp *
	  (-4.0*ue[m][i-1] + 6.0*ue[m][i] -
 	    4.0*ue[m][i+1] +     ue[m][i+2]);
      }

      for (m = 0; m < 5; m++) {
	for (i = 3; i <= grid_points[0]-4; i++) {
	  forcing[m][i][j][k] = forcing[m][i][j][k] - dssp*
	    (ue[m][i-2] - 4.0*ue[m][i-1] +
	     6.0*ue[m][i] - 4.0*ue[m][i+1] + ue[m][i+2]);
	}
      }

      for (m = 0; m < 5; m++) {
	i = grid_points[0]-3;
	forcing[m][i][j][k] = forcing[m][i][j][k] - dssp *
	  (ue[m][i-2] - 4.0*ue[m][i-1] +
	   6.0*ue[m][i] - 4.0*ue[m][i+1]);
	i = grid_points[0]-2;
	forcing[m][i][j][k] = forcing[m][i][j][k] - dssp *
	  (ue[m][i-2] - 4.0*ue[m][i-1] + 5.0*ue[m][i]);
      }
    }
  }

/*--------------------------------------------------------------------
c  eta-direction flux differences             
c-------------------------------------------------------------------*/
  for (k = 1; k <= grid_points[2]-2; k++) {
    zeta = (double)k * dnzm1;
    for (i = 1; i <= grid_points[0]-2; i++) {
      xi = (double)i * dnxm1;

      for (j = 0; j <= grid_points[1]-1; j++) {
	eta = (double)j * dnym1;

	exact_solution(xi, eta, zeta, dtemp);
	for (m = 0; m < 5; m++) {
	  ue[m][j] = dtemp[m];
	}
	dtpp = 1.0/dtemp[0];

	for (m = 1; m < 5; m++) {
	  buf[m][j] = dtpp * dtemp[m];
	}

	cuf[j]   = buf[2][j] * buf[2][j];
	buf[0][j] = cuf[j] + buf[1][j] * buf[1][j] + 
	  buf[3][j] * buf[3][j];
	q[j] = 0.5*(buf[1][j]*ue[1][j] + buf[2][j]*ue[2][j] +
		    buf[3][j]*ue[3][j]);
      }

      for (j = 1; j <= grid_points[1]-2; j++) {
	jm1 = j-1;
	jp1 = j+1;
                  
	forcing[0][i][j][k] = forcing[0][i][j][k] -
	  ty2*( ue[2][jp1]-ue[2][jm1] )+
	  dy1ty1*(ue[0][jp1]-2.0*ue[0][j]+ue[0][jm1]);

	forcing[1][i][j][k] = forcing[1][i][j][k]
	  - ty2*(ue[1][jp1]*buf[2][jp1]-ue[1][jm1]*buf[2][jm1])+
	  yycon2*(buf[1][jp1]-2.0*buf[1][j]+buf[1][jm1])+
	  dy2ty1*( ue[1][jp1]-2.0* ue[1][j]+ ue[1][jm1]);

	forcing[2][i][j][k] = forcing[2][i][j][k]
	  - ty2*((ue[2][jp1]*buf[2][jp1]+c2*(ue[4][jp1]-q[jp1]))-
		 (ue[2][jm1]*buf[2][jm1]+c2*(ue[4][jm1]-q[jm1])))+
	  yycon1*(buf[2][jp1]-2.0*buf[2][j]+buf[2][jm1])+
	  dy3ty1*( ue[2][jp1]-2.0*ue[2][j] +ue[2][jm1]);

	forcing[3][i][j][k] = forcing[3][i][j][k]
	  - ty2*(ue[3][jp1]*buf[2][jp1]-ue[3][jm1]*buf[2][jm1])+
	  yycon2*(buf[3][jp1]-2.0*buf[3][j]+buf[3][jm1])+
	  dy4ty1*( ue[3][jp1]-2.0*ue[3][j]+ ue[3][jm1]);

	forcing[4][i][j][k] = forcing[4][i][j][k]
	  - ty2*(buf[2][jp1]*(c1*ue[4][jp1]-c2*q[jp1])-
		 buf[2][jm1]*(c1*ue[4][jm1]-c2*q[jm1]))+
	  0.5*yycon3*(buf[0][jp1]-2.0*buf[0][j]+
		      buf[0][jm1])+
	  yycon4*(cuf[jp1]-2.0*cuf[j]+cuf[jm1])+
	  yycon5*(buf[4][jp1]-2.0*buf[4][j]+buf[4][jm1])+
	  dy5ty1*(ue[4][jp1]-2.0*ue[4][j]+ue[4][jm1]);
      }

/*--------------------------------------------------------------------
c            Fourth-order dissipation                      
c-------------------------------------------------------------------*/
      for (m = 0; m < 5; m++) {
	j = 1;
	forcing[m][i][j][k] = forcing[m][i][j][k] - dssp *
	  (5.0*ue[m][j] - 4.0*ue[m][j+1] +ue[m][j+2]);
	j = 2;
	forcing[m][i][j][k] = forcing[m][i][j][k] - dssp *
	  (-4.0*ue[m][j-1] + 6.0*ue[m][j] -
	   4.0*ue[m][j+1] +       ue[m][j+2]);
      }

      for (m = 0; m < 5; m++) {
	for (j = 3; j <= grid_points[1]-4; j++) {
	  forcing[m][i][j][k] = forcing[m][i][j][k] - dssp*
	    (ue[m][j-2] - 4.0*ue[m][j-1] +
	     6.0*ue[m][j] - 4.0*ue[m][j+1] + ue[m][j+2]);
	}
      }

      for (m = 0; m < 5; m++) {
	j = grid_points[1]-3;
	forcing[m][i][j][k] = forcing[m][i][j][k] - dssp *
	  (ue[m][j-2] - 4.0*ue[m][j-1] +
	   6.0*ue[m][j] - 4.0*ue[m][j+1]);
	j = grid_points[1]-2;
	forcing[m][i][j][k] = forcing[m][i][j][k] - dssp *
	  (ue[m][j-2] - 4.0*ue[m][j-1] + 5.0*ue[m][j]);

      }
    }
  }

/*--------------------------------------------------------------------
c      zeta-direction flux differences                      
c-------------------------------------------------------------------*/
  for (j = 1; j <= grid_points[1]-2; j++) {
    eta = (double)j * dnym1;
    for (i = 1; i <= grid_points[0]-2; i++) {
      xi = (double)i * dnxm1;

      for (k = 0; k <= grid_points[2]-1; k++) {
	zeta = (double)k * dnzm1;

	exact_solution(xi, eta, zeta, dtemp);
	for (m = 0; m < 5; m++) {
	  ue[m][k] = dtemp[m];
	}

	dtpp = 1.0/dtemp[0];

	for (m = 1; m < 5; m++) {
	  buf[m][k] = dtpp * dtemp[m];
	}

	cuf[k] = buf[3][k] * buf[3][k];
	buf[0][k] = cuf[k] + buf[1][k] * buf[1][k] + 
	  buf[2][k] * buf[2][k];
	q[k] = 0.5*(buf[1][k]*ue[1][k] + buf[2][k]*ue[2][k] +
		    buf[3][k]*ue[3][k]);
      }

      for (k = 1; k <= grid_points[2]-2; k++) {
	km1 = k-1;
	kp1 = k+1;
                  
	forcing[0][i][j][k] = forcing[0][i][j][k] -
	  tz2*( ue[3][kp1]-ue[3][km1] )+
	  dz1tz1*(ue[0][kp1]-2.0*ue[0][k]+ue[0][km1]);

	forcing[1][i][j][k] = forcing[1][i][j][k]
	  - tz2 * (ue[1][kp1]*buf[3][kp1]-ue[1][km1]*buf[3][km1])+
	  zzcon2*(buf[1][kp1]-2.0*buf[1][k]+buf[1][km1])+
	  dz2tz1*( ue[1][kp1]-2.0* ue[1][k]+ ue[1][km1]);

	forcing[2][i][j][k] = forcing[2][i][j][k]
	  - tz2 * (ue[2][kp1]*buf[3][kp1]-ue[2][km1]*buf[3][km1])+
	  zzcon2*(buf[2][kp1]-2.0*buf[2][k]+buf[2][km1])+
	  dz3tz1*(ue[2][kp1]-2.0*ue[2][k]+ue[2][km1]);

	forcing[3][i][j][k] = forcing[3][i][j][k]
	  - tz2 * ((ue[3][kp1]*buf[3][kp1]+c2*(ue[4][kp1]-q[kp1]))-
		   (ue[3][km1]*buf[3][km1]+c2*(ue[4][km1]-q[km1])))+
	  zzcon1*(buf[3][kp1]-2.0*buf[3][k]+buf[3][km1])+
	  dz4tz1*( ue[3][kp1]-2.0*ue[3][k] +ue[3][km1]);

	forcing[4][i][j][k] = forcing[4][i][j][k]
	  - tz2 * (buf[3][kp1]*(c1*ue[4][kp1]-c2*q[kp1])-
		   buf[3][km1]*(c1*ue[4][km1]-c2*q[km1]))+
	  0.5*zzcon3*(buf[0][kp1]-2.0*buf[0][k]
		      +buf[0][km1])+
	  zzcon4*(cuf[kp1]-2.0*cuf[k]+cuf[km1])+
	  zzcon5*(buf[4][kp1]-2.0*buf[4][k]+buf[4][km1])+
	  dz5tz1*( ue[4][kp1]-2.0*ue[4][k]+ ue[4][km1]);
      }

/*--------------------------------------------------------------------
c            Fourth-order dissipation                        
c-------------------------------------------------------------------*/
      for (m = 0; m < 5; m++) {
	k = 1;
	forcing[m][i][j][k] = forcing[m][i][j][k] - dssp *
	  (5.0*ue[m][k] - 4.0*ue[m][k+1] +ue[m][k+2]);
	k = 2;
	forcing[m][i][j][k] = forcing[m][i][j][k] - dssp *
	  (-4.0*ue[m][k-1] + 6.0*ue[m][k] -
	   4.0*ue[m][k+1] +       ue[m][k+2]);
      }

      for (m = 0; m < 5; m++) {
	for (k = 3; k <= grid_points[2]-4; k++) {
	  forcing[m][i][j][k] = forcing[m][i][j][k] - dssp*
	    (ue[m][k-2] - 4.0*ue[m][k-1] +
	     6.0*ue[m][k] - 4.0*ue[m][k+1] + ue[m][k+2]);
	}
      }

      for (m = 0; m < 5; m++) {
	k = grid_points[2]-3;
	forcing[m][i][j][k] = forcing[m][i][j][k] - dssp *
	  (ue[m][k-2] - 4.0*ue[m][k-1] +
	   6.0*ue[m][k] - 4.0*ue[m][k+1]);
	k = grid_points[2]-2;
	forcing[m][i][j][k] = forcing[m][i][j][k] - dssp *
	  (ue[m][k-2] - 4.0*ue[m][k-1] + 5.0*ue[m][k]);
      }
    }
  }

/*--------------------------------------------------------------------
c now change the sign of the forcing function, 
c-------------------------------------------------------------------*/
  for (m = 0; m < 5; m++) {
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	for (k = 1; k <= grid_points[2]-2; k++) {
	  forcing[m][i][j][k] = -1.0 * forcing[m][i][j][k];
	}
      }
    }
  }
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

static void exact_solution(double xi, double eta, double zeta,
			   double dtemp[5]) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c this function returns the exact solution at point xi, eta, zeta  
c-------------------------------------------------------------------*/

  int m;

  for (m = 0; m < 5; m++) {
    dtemp[m] =  ce[0][m] +
      xi*(ce[1][m] + xi*(ce[4][m] + 
			     xi*(ce[7][m] + xi*ce[10][m]))) +
      eta*(ce[2][m] + eta*(ce[5][m] + 
			       eta*(ce[8][m] + eta*ce[11][m])))+
      zeta*(ce[3][m] + zeta*(ce[6][m] +
				 zeta*(ce[9][m] + 
				       zeta*ce[12][m])));
  }
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

static void initialize(void) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c This subroutine initializes the field variable u using 
c tri-linear transfinite interpolation of the boundary values     
c-------------------------------------------------------------------*/

  int i, j, k, m, ix, iy, iz;
  double xi, eta, zeta, Pface[2][3][5], Pxi, Peta, Pzeta, temp[5];

/*--------------------------------------------------------------------
c  Later (in compute_rhs) we compute 1/u for every element. A few of 
c  the corner elements are not used, but it convenient (and faster) 
c  to compute the whole thing with a simple loop. Make sure those 
c  values are nonzero by initializing the whole thing here. 
c-------------------------------------------------------------------*/

  for (i = 0; i <= IMAX-1; i++) {
    for (j = 0; j <= IMAX-1; j++) {
      for (k = 0; k <= IMAX-1; k++) {
	u[0][i][j][k] = 1.0;
	u[1][i][j][k] = 0.0;
	u[2][i][j][k] = 0.0;
	u[3][i][j][k] = 0.0;
	u[4][i][j][k] = 1.0;
      }
    }
  }

/*--------------------------------------------------------------------
c first store the "interpolated" values everywhere on the grid    
c-------------------------------------------------------------------*/

  for (i = 0; i <= grid_points[0]-1; i++) {
    xi = (double)i * dnxm1;
    for (j = 0; j <= grid_points[1]-1; j++) {
      eta = (double)j * dnym1;
      for (k = 0; k <= grid_points[2]-1; k++) {
	zeta = (double)k * dnzm1;
                  
	for (ix = 0; ix < 2; ix++) {
	  exact_solution((double)ix, eta, zeta, 
			 &Pface[ix][0][0]);
	}

	for (iy = 0; iy < 2; iy++) {
	  exact_solution(xi, (double)iy , zeta, 
			 &Pface[iy][1][0]);
	}

	for (iz = 0; iz < 2; iz++) {
	  exact_solution(xi, eta, (double)iz,   
			 &Pface[iz][2][0]);
	}

	for (m = 0; m < 5; m++) {
	  Pxi   = xi   * Pface[1][0][m] + 
	    (1.0-xi)   * Pface[0][0][m];
	  Peta  = eta  * Pface[1][1][m] + 
	    (1.0-eta)  * Pface[0][1][m];
	  Pzeta = zeta * Pface[1][2][m] + 
	    (1.0-zeta) * Pface[0][2][m];
 
	  u[m][i][j][k] = Pxi + Peta + Pzeta - 
	    Pxi*Peta - Pxi*Pzeta - Peta*Pzeta + 
	    Pxi*Peta*Pzeta;

	}
      }
    }
  }

/*--------------------------------------------------------------------
c now store the exact values on the boundaries        
c-------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c west face                                                  
c-------------------------------------------------------------------*/

  xi = 0.0;
  i  = 0;
  for (j = 0; j < grid_points[1]; j++) {
    eta = (double)j * dnym1;
    for (k = 0; k < grid_points[2]; k++) {
      zeta = (double)k * dnzm1;
      exact_solution(xi, eta, zeta, temp);
      for (m = 0; m < 5; m++) {
	u[m][i][j][k] = temp[m];
      }
    }
  }

/*--------------------------------------------------------------------
c east face                                                      
c-------------------------------------------------------------------*/

  xi = 1.0;
  i  = grid_points[0]-1;
  for (j = 0; j < grid_points[1]; j++) {
    eta = (double)j * dnym1;
    for (k = 0; k < grid_points[2]; k++) {
      zeta = (double)k * dnzm1;
      exact_solution(xi, eta, zeta, temp);
      for (m = 0; m < 5; m++) {
	u[m][i][j][k] = temp[m];
      }
    }
  }

/*--------------------------------------------------------------------
c south face                                                 
c-------------------------------------------------------------------*/

  eta = 0.0;
  j   = 0;
  for (i = 0; i < grid_points[0]; i++) {
    xi = (double)i * dnxm1;
    for (k = 0; k < grid_points[2]; k++) {
      zeta = (double)k * dnzm1;
      exact_solution(xi, eta, zeta, temp);
      for (m = 0; m < 5; m++) {
	u[m][i][j][k] = temp[m];
      }
    }
  }

/*--------------------------------------------------------------------
c north face                                    
c-------------------------------------------------------------------*/

  eta = 1.0;
  j   = grid_points[1]-1;
  for (i = 0; i < grid_points[0]; i++) {
    xi = (double)i * dnxm1;
    for (k = 0; k < grid_points[2]; k++) {
      zeta = (double)k * dnzm1;
      exact_solution(xi, eta, zeta, temp);
      for (m = 0; m < 5; m++) {
	u[m][i][j][k] = temp[m];
      }
    }
  }

/*--------------------------------------------------------------------
c bottom face                                       
c-------------------------------------------------------------------*/

  zeta = 0.0;
  k    = 0;
  for (i = 0; i < grid_points[0]; i++) {
    xi = (double)i *dnxm1;
    for (j = 0; j < grid_points[1]; j++) {
      eta = (double)j * dnym1;
      exact_solution(xi, eta, zeta, temp);
      for (m = 0; m < 5; m++) {
	u[m][i][j][k] = temp[m];
      }
    }
  }

/*--------------------------------------------------------------------
c top face     
c-------------------------------------------------------------------*/

  zeta = 1.0;
  k    = grid_points[2]-1;
  for (i = 0; i < grid_points[0]; i++) {
    xi = (double)i * dnxm1;
    for (j = 0; j < grid_points[1]; j++) {
      eta = (double)j * dnym1;
      exact_solution(xi, eta, zeta, temp);
      for (m = 0; m < 5; m++) {
	u[m][i][j][k] = temp[m];
      }
    }
  }
}


/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

static void lhsinit(void) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

  int i, j, k, n;

/*--------------------------------------------------------------------
c     zap the whole left hand side for starters
c-------------------------------------------------------------------*/
  for (n = 0; n < 15; n++) {
#pragma omp for nowait
    for (i = 0; i < grid_points[0]; i++) {
      for (j = 0; j < grid_points[1]; j++) {
	for (k = 0; k < grid_points[2]; k++) {
	  lhs[n][i][j][k] = 0.0;
	}
      }
    }
  }
#pragma omp barrier  

/*--------------------------------------------------------------------
c      next, set all diagonal values to 1. This is overkill, but 
c      convenient
c-------------------------------------------------------------------*/
  for (n = 0; n < 3; n++) {
#pragma omp for    
    for (i = 0; i < grid_points[0]; i++) {
      for (j = 0; j < grid_points[1]; j++) {
	for (k = 0; k < grid_points[2]; k++) {
	  lhs[5*n+2][i][j][k] = 1.0;
	}
      }
    }
  }
}
	 
/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

static void lhsx(void) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c This function computes the left hand side for the three x-factors  
c-------------------------------------------------------------------*/

  double ru1;
  int i, j, k;

/*--------------------------------------------------------------------
c      first fill the lhs for the u-eigenvalue                   
c-------------------------------------------------------------------*/
  for (j = 1; j <= grid_points[1]-2; j++) {
    for (k = 1; k <= grid_points[2]-2; k++) {
#pragma omp for  
      for (i = 0; i <= grid_points[0]-1; i++) {
	ru1 = c3c4*rho_i[i][j][k];
	cv[i] = us[i][j][k];
	rhon[i] = max(dx2+con43*ru1, 
		      max(dx5+c1c5*ru1,
			  max(dxmax+ru1,
			      dx1)));
      }

#pragma omp for  
      for (i = 1; i <= grid_points[0]-2; i++) {
	lhs[0][i][j][k] =   0.0;
	lhs[1][i][j][k] = - dttx2 * cv[i-1] - dttx1 * rhon[i-1];
	lhs[2][i][j][k] =   1.0 + c2dttx1 * rhon[i];
	lhs[3][i][j][k] =   dttx2 * cv[i+1] - dttx1 * rhon[i+1];
	lhs[4][i][j][k] =   0.0;
      }
    }
  }

/*--------------------------------------------------------------------
c      add fourth order dissipation                             
c-------------------------------------------------------------------*/

  i = 1;
#pragma omp for nowait
  for (j = 1; j <= grid_points[1]-2; j++) {
    for (k = 1; k <= grid_points[2]-2; k++) {
      lhs[2][i][j][k] = lhs[2][i][j][k] + comz5;
      lhs[3][i][j][k] = lhs[3][i][j][k] - comz4;
      lhs[4][i][j][k] = lhs[4][i][j][k] + comz1;
      lhs[1][i+1][j][k] = lhs[1][i+1][j][k] - comz4;
      lhs[2][i+1][j][k] = lhs[2][i+1][j][k] + comz6;
      lhs[3][i+1][j][k] = lhs[3][i+1][j][k] - comz4;
      lhs[4][i+1][j][k] = lhs[4][i+1][j][k] + comz1;
    }
  }

#pragma omp for nowait
  for (i = 3; i <= grid_points[0]-4; i++) {
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	lhs[0][i][j][k] = lhs[0][i][j][k] + comz1;
	lhs[1][i][j][k] = lhs[1][i][j][k] - comz4;
	lhs[2][i][j][k] = lhs[2][i][j][k] + comz6;
	lhs[3][i][j][k] = lhs[3][i][j][k] - comz4;
	lhs[4][i][j][k] = lhs[4][i][j][k] + comz1;
      }
    }
  }

  i = grid_points[0]-3;
#pragma omp for  
  for (j = 1; j <= grid_points[1]-2; j++) {
    for (k = 1; k <= grid_points[2]-2; k++) {
      lhs[0][i][j][k] = lhs[0][i][j][k] + comz1;
      lhs[1][i][j][k] = lhs[1][i][j][k] - comz4;
      lhs[2][i][j][k] = lhs[2][i][j][k] + comz6;
      lhs[3][i][j][k] = lhs[3][i][j][k] - comz4;

      lhs[0][i+1][j][k] = lhs[0][i+1][j][k] + comz1;
      lhs[1][i+1][j][k] = lhs[1][i+1][j][k] - comz4;
      lhs[2][i+1][j][k] = lhs[2][i+1][j][k] + comz5;
    }
  }

/*--------------------------------------------------------------------
c      subsequently, fill the other factors (u+c), (u-c) by adding to 
c      the first  
c-------------------------------------------------------------------*/
#pragma omp for  
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	lhs[0+5][i][j][k]  = lhs[0][i][j][k];
	lhs[1+5][i][j][k]  = lhs[1][i][j][k] - 
	  dttx2 * speed[i-1][j][k];
	lhs[2+5][i][j][k]  = lhs[2][i][j][k];
	lhs[3+5][i][j][k]  = lhs[3][i][j][k] + 
	  dttx2 * speed[i+1][j][k];
	lhs[4+5][i][j][k]  = lhs[4][i][j][k];
	lhs[0+10][i][j][k] = lhs[0][i][j][k];
	lhs[1+10][i][j][k] = lhs[1][i][j][k] + 
	  dttx2 * speed[i-1][j][k];
	lhs[2+10][i][j][k] = lhs[2][i][j][k];
	lhs[3+10][i][j][k] = lhs[3][i][j][k] - 
	  dttx2 * speed[i+1][j][k];
	lhs[4+10][i][j][k] = lhs[4][i][j][k];
      }
    }
  }
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

static void lhsy(void) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c This function computes the left hand side for the three y-factors   
c-------------------------------------------------------------------*/

  double ru1;
  int i, j, k;

/*--------------------------------------------------------------------
c      first fill the lhs for the u-eigenvalue         
c-------------------------------------------------------------------*/
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (k = 1; k <= grid_points[2]-2; k++) {
#pragma omp for  
      for (j = 0; j <= grid_points[1]-1; j++) {
	ru1 = c3c4*rho_i[i][j][k];
	cv[j] = vs[i][j][k];
	rhoq[j] = max(dy3 + con43 * ru1,
		      max(dy5 + c1c5*ru1,
			  max(dymax + ru1,
			      dy1)));
      }
            
#pragma omp for  
      for (j = 1; j <= grid_points[1]-2; j++) {
	lhs[0][i][j][k] =  0.0;
	lhs[1][i][j][k] = -dtty2 * cv[j-1] - dtty1 * rhoq[j-1];
	lhs[2][i][j][k] =  1.0 + c2dtty1 * rhoq[j];
	lhs[3][i][j][k] =  dtty2 * cv[j+1] - dtty1 * rhoq[j+1];
	lhs[4][i][j][k] =  0.0;
      }
    }
  }

/*--------------------------------------------------------------------
c      add fourth order dissipation                             
c-------------------------------------------------------------------*/

  j = 1;
#pragma omp for nowait
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (k = 1; k <= grid_points[2]-2; k++) {

      lhs[2][i][j][k] = lhs[2][i][j][k] + comz5;
      lhs[3][i][j][k] = lhs[3][i][j][k] - comz4;
      lhs[4][i][j][k] = lhs[4][i][j][k] + comz1;
       
      lhs[1][i][j+1][k] = lhs[1][i][j+1][k] - comz4;
      lhs[2][i][j+1][k] = lhs[2][i][j+1][k] + comz6;
      lhs[3][i][j+1][k] = lhs[3][i][j+1][k] - comz4;
      lhs[4][i][j+1][k] = lhs[4][i][j+1][k] + comz1;
    }
  }

#pragma omp for nowait
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (j = 3; j <= grid_points[1]-4; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	lhs[0][i][j][k] = lhs[0][i][j][k] + comz1;
	lhs[1][i][j][k] = lhs[1][i][j][k] - comz4;
	lhs[2][i][j][k] = lhs[2][i][j][k] + comz6;
	lhs[3][i][j][k] = lhs[3][i][j][k] - comz4;
	lhs[4][i][j][k] = lhs[4][i][j][k] + comz1;
      }
    }
  }

  j = grid_points[1]-3;
#pragma omp for  
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (k = 1; k <= grid_points[2]-2; k++) {
      lhs[0][i][j][k] = lhs[0][i][j][k] + comz1;
      lhs[1][i][j][k] = lhs[1][i][j][k] - comz4;
      lhs[2][i][j][k] = lhs[2][i][j][k] + comz6;
      lhs[3][i][j][k] = lhs[3][i][j][k] - comz4;

      lhs[0][i][j+1][k] = lhs[0][i][j+1][k] + comz1;
      lhs[1][i][j+1][k] = lhs[1][i][j+1][k] - comz4;
      lhs[2][i][j+1][k] = lhs[2][i][j+1][k] + comz5;
    }
  }

/*--------------------------------------------------------------------
c      subsequently, do the other two factors                    
c-------------------------------------------------------------------*/
#pragma omp for  
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	lhs[0+5][i][j][k]  = lhs[0][i][j][k];
	lhs[1+5][i][j][k]  = lhs[1][i][j][k] - 
	  dtty2 * speed[i][j-1][k];
	lhs[2+5][i][j][k]  = lhs[2][i][j][k];
	lhs[3+5][i][j][k]  = lhs[3][i][j][k] + 
	  dtty2 * speed[i][j+1][k];
	lhs[4+5][i][j][k] = lhs[4][i][j][k];
	lhs[0+10][i][j][k] = lhs[0][i][j][k];
	lhs[1+10][i][j][k] = lhs[1][i][j][k] + 
	  dtty2 * speed[i][j-1][k];
	lhs[2+10][i][j][k] = lhs[2][i][j][k];
	lhs[3+10][i][j][k] = lhs[3][i][j][k] - 
	  dtty2 * speed[i][j+1][k];
	lhs[4+10][i][j][k] = lhs[4][i][j][k];
      }
    }
  }
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

static void lhsz(void) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c This function computes the left hand side for the three z-factors   
c-------------------------------------------------------------------*/

  double ru1;
  int i, j, k;

/*--------------------------------------------------------------------
c first fill the lhs for the u-eigenvalue                          
c-------------------------------------------------------------------*/
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (j = 1; j <= grid_points[1]-2; j++) {
#pragma omp for  
      for (k = 0; k <= grid_points[2]-1; k++) {
	ru1 = c3c4*rho_i[i][j][k];
	cv[k] = ws[i][j][k];
	rhos[k] = max(dz4 + con43 * ru1,
		      max(dz5 + c1c5 * ru1,
			  max(dzmax + ru1,
			      dz1)));
      }

#pragma omp for  
      for (k = 1; k <= grid_points[2]-2; k++) {
	lhs[0][i][j][k] =  0.0;
	lhs[1][i][j][k] = -dttz2 * cv[k-1] - dttz1 * rhos[k-1];
	lhs[2][i][j][k] =  1.0 + c2dttz1 * rhos[k];
	lhs[3][i][j][k] =  dttz2 * cv[k+1] - dttz1 * rhos[k+1];
	lhs[4][i][j][k] =  0.0;
      }
    }
  }

/*--------------------------------------------------------------------
c      add fourth order dissipation                                  
c-------------------------------------------------------------------*/

  k = 1;
#pragma omp for nowait
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (j = 1; j <= grid_points[1]-2; j++) {
      lhs[2][i][j][k] = lhs[2][i][j][k] + comz5;
      lhs[3][i][j][k] = lhs[3][i][j][k] - comz4;
      lhs[4][i][j][k] = lhs[4][i][j][k] + comz1;

      lhs[1][i][j][k+1] = lhs[1][i][j][k+1] - comz4;
      lhs[2][i][j][k+1] = lhs[2][i][j][k+1] + comz6;
      lhs[3][i][j][k+1] = lhs[3][i][j][k+1] - comz4;
      lhs[4][i][j][k+1] = lhs[4][i][j][k+1] + comz1;
    }
  }

#pragma omp for nowait
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 3; k <= grid_points[2]-4; k++) {
	lhs[0][i][j][k] = lhs[0][i][j][k] + comz1;
	lhs[1][i][j][k] = lhs[1][i][j][k] - comz4;
	lhs[2][i][j][k] = lhs[2][i][j][k] + comz6;
	lhs[3][i][j][k] = lhs[3][i][j][k] - comz4;
	lhs[4][i][j][k] = lhs[4][i][j][k] + comz1;
      }
    }
  }

  k = grid_points[2]-3;
#pragma omp for  
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (j = 1; j <= grid_points[1]-2; j++) {
      lhs[0][i][j][k] = lhs[0][i][j][k] + comz1;
      lhs[1][i][j][k] = lhs[1][i][j][k] - comz4;
      lhs[2][i][j][k] = lhs[2][i][j][k] + comz6;
      lhs[3][i][j][k] = lhs[3][i][j][k] - comz4;

      lhs[0][i][j][k+1] = lhs[0][i][j][k+1] + comz1;
      lhs[1][i][j][k+1] = lhs[1][i][j][k+1] - comz4;
      lhs[2][i][j][k+1] = lhs[2][i][j][k+1] + comz5;
    }
  }

/*--------------------------------------------------------------------
c      subsequently, fill the other factors (u+c), (u-c) 
c-------------------------------------------------------------------*/
#pragma omp for  
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	lhs[0+5][i][j][k]  = lhs[0][i][j][k];
	lhs[1+5][i][j][k]  = lhs[1][i][j][k] - 
	  dttz2 * speed[i][j][k-1];
	lhs[2+5][i][j][k]  = lhs[2][i][j][k];
	lhs[3+5][i][j][k]  = lhs[3][i][j][k] + 
	  dttz2 * speed[i][j][k+1];
	lhs[4+5][i][j][k]  = lhs[4][i][j][k];
	lhs[0+10][i][j][k] = lhs[0][i][j][k];
	lhs[1+10][i][j][k] = lhs[1][i][j][k] + 
	  dttz2 * speed[i][j][k-1];
	lhs[2+10][i][j][k] = lhs[2][i][j][k];
	lhs[3+10][i][j][k] = lhs[3][i][j][k] - 
	  dttz2 * speed[i][j][k+1];
	lhs[4+10][i][j][k] = lhs[4][i][j][k];
      }
    }
  }
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

static void ninvr(void) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c   block-diagonal matrix-vector multiplication              
c-------------------------------------------------------------------*/

  int i, j, k;
  double r1, r2, r3, r4, r5, t1, t2;
#pragma omp for
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {

	r1 = rhs[0][i][j][k];
	r2 = rhs[1][i][j][k];
	r3 = rhs[2][i][j][k];
	r4 = rhs[3][i][j][k];
	r5 = rhs[4][i][j][k];
               
	t1 = bt * r3;
	t2 = 0.5 * ( r4 + r5 );

	rhs[0][i][j][k] = -r2;
	rhs[1][i][j][k] =  r1;
	rhs[2][i][j][k] = bt * ( r4 - r5 );
	rhs[3][i][j][k] = -t1 + t2;
	rhs[4][i][j][k] =  t1 + t2;
      }
    }
  }
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

static void pinvr(void) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c   block-diagonal matrix-vector multiplication                       
c-------------------------------------------------------------------*/

  int i, j, k;
  double r1, r2, r3, r4, r5, t1, t2;

#pragma omp for
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {

	r1 = rhs[0][i][j][k];
	r2 = rhs[1][i][j][k];
	r3 = rhs[2][i][j][k];
	r4 = rhs[3][i][j][k];
	r5 = rhs[4][i][j][k];

	t1 = bt * r1;
	t2 = 0.5 * ( r4 + r5 );

	rhs[0][i][j][k] =  bt * ( r4 - r5 );
	rhs[1][i][j][k] = -r3;
	rhs[2][i][j][k] =  r2;
	rhs[3][i][j][k] = -t1 + t2;
	rhs[4][i][j][k] =  t1 + t2;
      }
    }
  }
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

static void compute_rhs(void) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

  int i, j, k, m;
  double aux, rho_inv, uijk, up1, um1, vijk, vp1, vm1,
    wijk, wp1, wm1;

/*--------------------------------------------------------------------
c      compute the reciprocal of density, and the kinetic energy, 
c      and the speed of sound. 
c-------------------------------------------------------------------*/

#pragma omp for nowait
  for (i = 0; i <= grid_points[0]-1; i++) {
    for (j = 0; j <= grid_points[1]-1; j++) {
      for (k = 0; k <= grid_points[2]-1; k++) {
	rho_inv = 1.0/u[0][i][j][k];
	rho_i[i][j][k] = rho_inv;
	us[i][j][k] = u[1][i][j][k] * rho_inv;
	vs[i][j][k] = u[2][i][j][k] * rho_inv;
	ws[i][j][k] = u[3][i][j][k] * rho_inv;
	square[i][j][k] = 0.5* (u[1][i][j][k]*u[1][i][j][k] + 
				u[2][i][j][k]*u[2][i][j][k] +
				u[3][i][j][k]*u[3][i][j][k] ) * rho_inv;
	qs[i][j][k] = square[i][j][k] * rho_inv;
/*--------------------------------------------------------------------
c               (do not need speed and ainx until the lhs computation)
c-------------------------------------------------------------------*/
	aux = c1c2*rho_inv* (u[4][i][j][k] - square[i][j][k]);
	aux = sqrt(aux);
	speed[i][j][k] = aux;
	ainv[i][j][k]  = 1.0/aux;
      }
    }
  }

/*--------------------------------------------------------------------
c copy the exact forcing term to the right hand side;  because 
c this forcing term is known, we can store it on the whole grid
c including the boundary                   
c-------------------------------------------------------------------*/

  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 0; i <= grid_points[0]-1; i++) {
      for (j = 0; j <= grid_points[1]-1; j++) {
	for (k = 0; k <= grid_points[2]-1; k++) {
	  rhs[m][i][j][k] = forcing[m][i][j][k];
	
  
  
  
  }
      }
    }
  }

/*--------------------------------------------------------------------
c      compute xi-direction fluxes 
c-------------------------------------------------------------------*/
#pragma omp for
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	uijk = us[i][j][k];
	up1  = us[i+1][j][k];
	um1  = us[i-1][j][k];

	rhs[0][i][j][k] = rhs[0][i][j][k] + dx1tx1 * 
	  (u[0][i+1][j][k] - 2.0*u[0][i][j][k] + 
	   u[0][i-1][j][k]) -
	  tx2 * (u[1][i+1][j][k] - u[1][i-1][j][k]);
	rhs[1][i][j][k] = rhs[1][i][j][k] + dx2tx1 * 
	  (u[1][i+1][j][k] - 2.0*u[1][i][j][k] + 
	   u[1][i-1][j][k]) +
	  xxcon2*con43 * (up1 - 2.0*uijk + um1) -
	  tx2 * (u[1][i+1][j][k]*up1 - 
		 u[1][i-1][j][k]*um1 +
		 (u[4][i+1][j][k]- square[i+1][j][k]-
		  u[4][i-1][j][k]+ square[i-1][j][k])*
		 c2);

	rhs[2][i][j][k] = rhs[2][i][j][k] + dx3tx1 * 
	  (u[2][i+1][j][k] - 2.0*u[2][i][j][k] +
	   u[2][i-1][j][k]) +
	  xxcon2 * (vs[i+1][j][k] - 2.0*vs[i][j][k] +
		    vs[i-1][j][k]) -
	  tx2 * (u[2][i+1][j][k]*up1 - 
		 u[2][i-1][j][k]*um1);

	rhs[3][i][j][k] = rhs[3][i][j][k] + dx4tx1 * 
	  (u[3][i+1][j][k] - 2.0*u[3][i][j][k] +
	   u[3][i-1][j][k]) +
	  xxcon2 * (ws[i+1][j][k] - 2.0*ws[i][j][k] +
		    ws[i-1][j][k]) -
	  tx2 * (u[3][i+1][j][k]*up1 - 
		 u[3][i-1][j][k]*um1);

	rhs[4][i][j][k] = rhs[4][i][j][k] + dx5tx1 * 
	  (u[4][i+1][j][k] - 2.0*u[4][i][j][k] +
	   u[4][i-1][j][k]) +
	  xxcon3 * (qs[i+1][j][k] - 2.0*qs[i][j][k] +
		    qs[i-1][j][k]) +
	  xxcon4 * (up1*up1 -       2.0*uijk*uijk + 
		    um1*um1) +
	  xxcon5 * (u[4][i+1][j][k]*rho_i[i+1][j][k] - 
		    2.0*u[4][i][j][k]*rho_i[i][j][k] +
		    u[4][i-1][j][k]*rho_i[i-1][j][k]) -
	  tx2 * ( (c1*u[4][i+1][j][k] - 
		   c2*square[i+1][j][k])*up1 -
		  (c1*u[4][i-1][j][k] - 
		   c2*square[i-1][j][k])*um1 );
      }
    }
  }

/*--------------------------------------------------------------------
c      add fourth order xi-direction dissipation               
c-------------------------------------------------------------------*/

  i = 1;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	rhs[m][i][j][k] = rhs[m][i][j][k]- dssp * 
	  ( 5.0*u[m][i][j][k] - 4.0*u[m][i+1][j][k] +
	    u[m][i+2][j][k]);
      }
    }
  }
  i = 2;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] - dssp * 
	  (-4.0*u[m][i-1][j][k] + 6.0*u[m][i][j][k] -
	   4.0*u[m][i+1][j][k] + u[m][i+2][j][k]);
      }
    }
  }

  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 3*1; i <= grid_points[0]-3*1-1; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	for (k = 1; k <= grid_points[2]-2; k++) {
	  rhs[m][i][j][k] = rhs[m][i][j][k] - dssp * 
	    (  u[m][i-2][j][k] - 4.0*u[m][i-1][j][k] + 
	       6.0*u[m][i][j][k] - 4.0*u[m][i+1][j][k] + 
	       u[m][i+2][j][k] );
	}
      }
    }
  }

  i = grid_points[0]-3;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] - dssp *
	  ( u[m][i-2][j][k] - 4.0*u[m][i-1][j][k] + 
	    6.0*u[m][i][j][k] - 4.0*u[m][i+1][j][k] );
      }
    }
  }

  i = grid_points[0]-2;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] - dssp *
	  ( u[m][i-2][j][k] - 4.0*u[m][i-1][j][k] +
	    5.0*u[m][i][j][k] );
      }
    }
  }
#pragma omp barrier
  
/*--------------------------------------------------------------------
c      compute eta-direction fluxes 
c-------------------------------------------------------------------*/
#pragma omp for
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	vijk = vs[i][j][k];
	vp1  = vs[i][j+1][k];
	vm1  = vs[i][j-1][k];
	rhs[0][i][j][k] = rhs[0][i][j][k] + dy1ty1 * 
	  (u[0][i][j+1][k] - 2.0*u[0][i][j][k] + 
	   u[0][i][j-1][k]) -
	  ty2 * (u[2][i][j+1][k] - u[2][i][j-1][k]);
	rhs[1][i][j][k] = rhs[1][i][j][k] + dy2ty1 * 
	  (u[1][i][j+1][k] - 2.0*u[1][i][j][k] + 
	   u[1][i][j-1][k]) +
	  yycon2 * (us[i][j+1][k] - 2.0*us[i][j][k] + 
		    us[i][j-1][k]) -
	  ty2 * (u[1][i][j+1][k]*vp1 - 
		 u[1][i][j-1][k]*vm1);
	rhs[2][i][j][k] = rhs[2][i][j][k] + dy3ty1 * 
	  (u[2][i][j+1][k] - 2.0*u[2][i][j][k] + 
	   u[2][i][j-1][k]) +
	  yycon2*con43 * (vp1 - 2.0*vijk + vm1) -
	  ty2 * (u[2][i][j+1][k]*vp1 - 
		 u[2][i][j-1][k]*vm1 +
		 (u[4][i][j+1][k] - square[i][j+1][k] - 
		  u[4][i][j-1][k] + square[i][j-1][k])
		 *c2);
	rhs[3][i][j][k] = rhs[3][i][j][k] + dy4ty1 * 
	  (u[3][i][j+1][k] - 2.0*u[3][i][j][k] + 
	   u[3][i][j-1][k]) +
	  yycon2 * (ws[i][j+1][k] - 2.0*ws[i][j][k] + 
		    ws[i][j-1][k]) -
	  ty2 * (u[3][i][j+1][k]*vp1 - 
		 u[3][i][j-1][k]*vm1);
	rhs[4][i][j][k] = rhs[4][i][j][k] + dy5ty1 * 
	  (u[4][i][j+1][k] - 2.0*u[4][i][j][k] + 
	   u[4][i][j-1][k]) +
	  yycon3 * (qs[i][j+1][k] - 2.0*qs[i][j][k] + 
		    qs[i][j-1][k]) +
	  yycon4 * (vp1*vp1       - 2.0*vijk*vijk + 
		    vm1*vm1) +
	  yycon5 * (u[4][i][j+1][k]*rho_i[i][j+1][k] - 
		    2.0*u[4][i][j][k]*rho_i[i][j][k] +
		    u[4][i][j-1][k]*rho_i[i][j-1][k]) -
	  ty2 * ((c1*u[4][i][j+1][k] - 
		  c2*square[i][j+1][k]) * vp1 -
		 (c1*u[4][i][j-1][k] - 
		  c2*square[i][j-1][k]) * vm1);
      }
    }
  }

/*--------------------------------------------------------------------
c      add fourth order eta-direction dissipation         
c-------------------------------------------------------------------*/

  j = 1;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	rhs[m][i][j][k] = rhs[m][i][j][k]- dssp * 
	  ( 5.0*u[m][i][j][k] - 4.0*u[m][i][j+1][k] +
	    u[m][i][j+2][k]);
      }
    }
  }

  j = 2;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] - dssp * 
	  (-4.0*u[m][i][j-1][k] + 6.0*u[m][i][j][k] -
	   4.0*u[m][i][j+1][k] + u[m][i][j+2][k]);
      }
    }
  }

  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 3*1; j <= grid_points[1]-3*1-1; j++) {
	for (k = 1; k <= grid_points[2]-2; k++) {
	  rhs[m][i][j][k] = rhs[m][i][j][k] - dssp * 
	    (  u[m][i][j-2][k] - 4.0*u[m][i][j-1][k] + 
	       6.0*u[m][i][j][k] - 4.0*u[m][i][j+1][k] + 
	       u[m][i][j+2][k] );
	}
      }
    }
  }
 
  j = grid_points[1]-3;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] - dssp *
	  ( u[m][i][j-2][k] - 4.0*u[m][i][j-1][k] + 
	    6.0*u[m][i][j][k] - 4.0*u[m][i][j+1][k] );
      }
    }
  }

  j = grid_points[1]-2;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] - dssp *
	  ( u[m][i][j-2][k] - 4.0*u[m][i][j-1][k] +
	    5.0*u[m][i][j][k] );
      }
    }
  }
#pragma omp barrier  

/*--------------------------------------------------------------------
c      compute zeta-direction fluxes 
c-------------------------------------------------------------------*/
#pragma omp for
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	wijk = ws[i][j][k];
	wp1  = ws[i][j][k+1];
	wm1  = ws[i][j][k-1];

	rhs[0][i][j][k] = rhs[0][i][j][k] + dz1tz1 * 
	  (u[0][i][j][k+1] - 2.0*u[0][i][j][k] + 
	   u[0][i][j][k-1]) -
	  tz2 * (u[3][i][j][k+1] - u[3][i][j][k-1]);
	rhs[1][i][j][k] = rhs[1][i][j][k] + dz2tz1 * 
	  (u[1][i][j][k+1] - 2.0*u[1][i][j][k] + 
	   u[1][i][j][k-1]) +
	  zzcon2 * (us[i][j][k+1] - 2.0*us[i][j][k] + 
		    us[i][j][k-1]) -
	  tz2 * (u[1][i][j][k+1]*wp1 - 
		 u[1][i][j][k-1]*wm1);
	rhs[2][i][j][k] = rhs[2][i][j][k] + dz3tz1 * 
	  (u[2][i][j][k+1] - 2.0*u[2][i][j][k] + 
	   u[2][i][j][k-1]) +
	  zzcon2 * (vs[i][j][k+1] - 2.0*vs[i][j][k] + 
		    vs[i][j][k-1]) -
	  tz2 * (u[2][i][j][k+1]*wp1 - 
		 u[2][i][j][k-1]*wm1);
	rhs[3][i][j][k] = rhs[3][i][j][k] + dz4tz1 * 
	  (u[3][i][j][k+1] - 2.0*u[3][i][j][k] + 
	   u[3][i][j][k-1]) +
	  zzcon2*con43 * (wp1 - 2.0*wijk + wm1) -
	  tz2 * (u[3][i][j][k+1]*wp1 - 
		 u[3][i][j][k-1]*wm1 +
		 (u[4][i][j][k+1] - square[i][j][k+1] - 
		  u[4][i][j][k-1] + square[i][j][k-1])
		 *c2);
	rhs[4][i][j][k] = rhs[4][i][j][k] + dz5tz1 * 
	  (u[4][i][j][k+1] - 2.0*u[4][i][j][k] + 
	   u[4][i][j][k-1]) +
	  zzcon3 * (qs[i][j][k+1] - 2.0*qs[i][j][k] + 
		    qs[i][j][k-1]) +
	  zzcon4 * (wp1*wp1 - 2.0*wijk*wijk + 
		    wm1*wm1) +
	  zzcon5 * (u[4][i][j][k+1]*rho_i[i][j][k+1] - 
		    2.0*u[4][i][j][k]*rho_i[i][j][k] +
		    u[4][i][j][k-1]*rho_i[i][j][k-1]) -
	  tz2 * ( (c1*u[4][i][j][k+1] - 
		   c2*square[i][j][k+1])*wp1 -
		  (c1*u[4][i][j][k-1] - 
		   c2*square[i][j][k-1])*wm1);
      }
    }
  }

/*--------------------------------------------------------------------
c      add fourth order zeta-direction dissipation                
c-------------------------------------------------------------------*/

  k = 1;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	rhs[m][i][j][k] = rhs[m][i][j][k]- dssp * 
	  ( 5.0*u[m][i][j][k] - 4.0*u[m][i][j][k+1] +
	    u[m][i][j][k+2]);
      }
    }
  }

  k = 2;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] - dssp * 
	  (-4.0*u[m][i][j][k-1] + 6.0*u[m][i][j][k] -
	   4.0*u[m][i][j][k+1] + u[m][i][j][k+2]);
      }
    }
  }

  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	for (k = 3*1; k <= grid_points[2]-3*1-1; k++) {
	  rhs[m][i][j][k] = rhs[m][i][j][k] - dssp * 
	    (  u[m][i][j][k-2] - 4.0*u[m][i][j][k-1] + 
	       6.0*u[m][i][j][k] - 4.0*u[m][i][j][k+1] + 
	       u[m][i][j][k+2] );
	}
      }
    }
  }
 
  k = grid_points[2]-3;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] - dssp *
	  ( u[m][i][j][k-2] - 4.0*u[m][i][j][k-1] + 
	    6.0*u[m][i][j][k] - 4.0*u[m][i][j][k+1] );
      }
    }
  }

  k = grid_points[2]-2;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] - dssp *
	  ( u[m][i][j][k-2] - 4.0*u[m][i][j][k-1] +
	    5.0*u[m][i][j][k] );
      }
    }
  }

  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	for (k = 1; k <= grid_points[2]-2; k++) {
	  rhs[m][i][j][k] = rhs[m][i][j][k] * dt;
	}
      }
    }
  }
}    

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

static void set_constants(void) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

  ce[0][0]  = 2.0;
  ce[1][0]  = 0.0;
  ce[2][0]  = 0.0;
  ce[3][0]  = 4.0;
  ce[4][0]  = 5.0;
  ce[5][0]  = 3.0;
  ce[6][0]  = 0.5;
  ce[7][0]  = 0.02;
  ce[8][0]  = 0.01;
  ce[9][0] = 0.03;
  ce[10][0] = 0.5;
  ce[11][0] = 0.4;
  ce[12][0] = 0.3;
 
  ce[0][1]  = 1.0;
  ce[1][1]  = 0.0;
  ce[2][1]  = 0.0;
  ce[3][1]  = 0.0;
  ce[4][1]  = 1.0;
  ce[5][1]  = 2.0;
  ce[6][1]  = 3.0;
  ce[7][1]  = 0.01;
  ce[8][1]  = 0.03;
  ce[9][1] = 0.02;
  ce[10][1] = 0.4;
  ce[11][1] = 0.3;
  ce[12][1] = 0.5;

  ce[0][2]  = 2.0;
  ce[1][2]  = 2.0;
  ce[2][2]  = 0.0;
  ce[3][2]  = 0.0;
  ce[4][2]  = 0.0;
  ce[5][2]  = 2.0;
  ce[6][2]  = 3.0;
  ce[7][2]  = 0.04;
  ce[8][2]  = 0.03;
  ce[9][2] = 0.05;
  ce[10][2] = 0.3;
  ce[11][2] = 0.5;
  ce[12][2] = 0.4;

  ce[0][3]  = 2.0;
  ce[1][3]  = 2.0;
  ce[2][3]  = 0.0;
  ce[3][3]  = 0.0;
  ce[4][3]  = 0.0;
  ce[5][3]  = 2.0;
  ce[6][3]  = 3.0;
  ce[7][3]  = 0.03;
  ce[8][3]  = 0.05;
  ce[9][3] = 0.04;
  ce[10][3] = 0.2;
  ce[11][3] = 0.1;
  ce[12][3] = 0.3;

  ce[0][4]  = 5.0;
  ce[1][4]  = 4.0;
  ce[2][4]  = 3.0;
  ce[3][4]  = 2.0;
  ce[4][4]  = 0.1;
  ce[5][4]  = 0.4;
  ce[6][4]  = 0.3;
  ce[7][4]  = 0.05;
  ce[8][4]  = 0.04;
  ce[9][4] = 0.03;
  ce[10][4] = 0.1;
  ce[11][4] = 0.3;
  ce[12][4] = 0.2;

  c1 = 1.4;
  c2 = 0.4;
  c3 = 0.1;
  c4 = 1.0;
  c5 = 1.4;

  bt = sqrt(0.5);

  dnxm1 = 1.0 / (double)(grid_points[0]-1);
  dnym1 = 1.0 / (double)(grid_points[1]-1);
  dnzm1 = 1.0 / (double)(grid_points[2]-1);

  c1c2 = c1 * c2;
  c1c5 = c1 * c5;
  c3c4 = c3 * c4;
  c1345 = c1c5 * c3c4;

  conz1 = (1.0-c1c5);

  tx1 = 1.0 / (dnxm1 * dnxm1);
  tx2 = 1.0 / (2.0 * dnxm1);
  tx3 = 1.0 / dnxm1;

  ty1 = 1.0 / (dnym1 * dnym1);
  ty2 = 1.0 / (2.0 * dnym1);
  ty3 = 1.0 / dnym1;
 
  tz1 = 1.0 / (dnzm1 * dnzm1);
  tz2 = 1.0 / (2.0 * dnzm1);
  tz3 = 1.0 / dnzm1;

  dx1 = 0.75;
  dx2 = 0.75;
  dx3 = 0.75;
  dx4 = 0.75;
  dx5 = 0.75;

  dy1 = 0.75;
  dy2 = 0.75;
  dy3 = 0.75;
  dy4 = 0.75;
  dy5 = 0.75;

  dz1 = 1.0;
  dz2 = 1.0;
  dz3 = 1.0;
  dz4 = 1.0;
  dz5 = 1.0;

  dxmax = max(dx3, dx4);
  dymax = max(dy2, dy4);
  dzmax = max(dz2, dz3);

  dssp = 0.25 * max(dx1, max(dy1, dz1) );

  c4dssp = 4.0 * dssp;
  c5dssp = 5.0 * dssp;

  dttx1 = dt*tx1;
  dttx2 = dt*tx2;
  dtty1 = dt*ty1;
  dtty2 = dt*ty2;
  dttz1 = dt*tz1;
  dttz2 = dt*tz2;

  c2dttx1 = 2.0*dttx1;
  c2dtty1 = 2.0*dtty1;
  c2dttz1 = 2.0*dttz1;

  dtdssp = dt*dssp;

  comz1  = dtdssp;
  comz4  = 4.0*dtdssp;
  comz5  = 5.0*dtdssp;
  comz6  = 6.0*dtdssp;

  c3c4tx3 = c3c4*tx3;
  c3c4ty3 = c3c4*ty3;
  c3c4tz3 = c3c4*tz3;

  dx1tx1 = dx1*tx1;
  dx2tx1 = dx2*tx1;
  dx3tx1 = dx3*tx1;
  dx4tx1 = dx4*tx1;
  dx5tx1 = dx5*tx1;
        
  dy1ty1 = dy1*ty1;
  dy2ty1 = dy2*ty1;
  dy3ty1 = dy3*ty1;
  dy4ty1 = dy4*ty1;
  dy5ty1 = dy5*ty1;
        
  dz1tz1 = dz1*tz1;
  dz2tz1 = dz2*tz1;
  dz3tz1 = dz3*tz1;
  dz4tz1 = dz4*tz1;
  dz5tz1 = dz5*tz1;

  c2iv  = 2.5;
  con43 = 4.0/3.0;
  con16 = 1.0/6.0;
        
  xxcon1 = c3c4tx3*con43*tx3;
  xxcon2 = c3c4tx3*tx3;
  xxcon3 = c3c4tx3*conz1*tx3;
  xxcon4 = c3c4tx3*con16*tx3;
  xxcon5 = c3c4tx3*c1c5*tx3;

  yycon1 = c3c4ty3*con43*ty3;
  yycon2 = c3c4ty3*ty3;
  yycon3 = c3c4ty3*conz1*ty3;
  yycon4 = c3c4ty3*con16*ty3;
  yycon5 = c3c4ty3*c1c5*ty3;

  zzcon1 = c3c4tz3*con43*tz3;
  zzcon2 = c3c4tz3*tz3;
  zzcon3 = c3c4tz3*conz1*tz3;
  zzcon4 = c3c4tz3*con16*tz3;
  zzcon5 = c3c4tz3*c1c5*tz3;
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

static void txinvr(void) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c block-diagonal matrix-vector multiplication                  
--------------------------------------------------------------------*/

  int i, j, k;
  double t1, t2, t3, ac, ru1, uu, vv, ww, r1, r2, r3, 
    r4, r5, ac2inv;

#pragma omp for  
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {

	ru1 = rho_i[i][j][k];
	uu = us[i][j][k];
	vv = vs[i][j][k];
	ww = ws[i][j][k];
	ac = speed[i][j][k];
	ac2inv = ainv[i][j][k]*ainv[i][j][k];

	r1 = rhs[0][i][j][k];
	r2 = rhs[1][i][j][k];
	r3 = rhs[2][i][j][k];
	r4 = rhs[3][i][j][k];
	r5 = rhs[4][i][j][k];

	t1 = c2 * ac2inv * ( qs[i][j][k]*r1 - uu*r2  - 
			     vv*r3 - ww*r4 + r5 );
	t2 = bt * ru1 * ( uu * r1 - r2 );
	t3 = ( bt * ru1 * ac ) * t1;

	rhs[0][i][j][k] = r1 - t1;
	rhs[1][i][j][k] = - ru1 * ( ww*r1 - r4 );
	rhs[2][i][j][k] =   ru1 * ( vv*r1 - r3 );
	rhs[3][i][j][k] = - t2 + t3;
	rhs[4][i][j][k] =   t2 + t3;
      }
    }
  }
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

static void tzetar(void) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c   block-diagonal matrix-vector multiplication                       
c-------------------------------------------------------------------*/

  int i, j, k;
  double t1, t2, t3, ac, xvel, yvel, zvel, r1, r2, r3, 
    r4, r5, btuz, acinv, ac2u, uzik1;
  
#pragma omp for
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {

	xvel = us[i][j][k];
	yvel = vs[i][j][k];
	zvel = ws[i][j][k];
	ac   = speed[i][j][k];
	acinv = ainv[i][j][k];

	ac2u = ac*ac;

	r1 = rhs[0][i][j][k];
	r2 = rhs[1][i][j][k];
	r3 = rhs[2][i][j][k];
	r4 = rhs[3][i][j][k];
	r5 = rhs[4][i][j][k];

	uzik1 = u[0][i][j][k];
	btuz  = bt * uzik1;

	t1 = btuz*acinv * (r4 + r5);
	t2 = r3 + t1;
	t3 = btuz * (r4 - r5);

	rhs[0][i][j][k] = t2;
	rhs[1][i][j][k] = -uzik1*r2 + xvel*t2;
	rhs[2][i][j][k] =  uzik1*r1 + yvel*t2;
	rhs[3][i][j][k] =  zvel*t2  + t3;
	rhs[4][i][j][k] =  uzik1*(-xvel*r2 + yvel*r1) + 
	  qs[i][j][k]*t2 + c2iv*ac2u*t1 + zvel*t3;
      }
    }
  }
}


/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

static void verify(int no_time_steps, char *cclass, boolean *verified) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c  verification routine                         
--------------------------------------------------------------------*/

  double xcrref[5],xceref[5],xcrdif[5],xcedif[5], 
    epsilon, xce[5], xcr[5], dtref;
  int m;

/*--------------------------------------------------------------------
c   tolerance level
--------------------------------------------------------------------*/
  epsilon = 1.0e-08;


/*--------------------------------------------------------------------
c   compute the error norm and the residual norm, and exit if not printing
--------------------------------------------------------------------*/
  error_norm(xce);
  compute_rhs();

  rhs_norm(xcr);

  for (m = 0; m < 5; m++) {
    xcr[m] = xcr[m] / dt;
  }

  *cclass = 'U';
  *verified = TRUE;

  for (m = 0; m < 5; m++) {
    xcrref[m] = 1.0;
    xceref[m] = 1.0;
  }

/*--------------------------------------------------------------------
c    reference data for 12X12X12 grids after 100 time steps, with DT = 1.50d-02
--------------------------------------------------------------------*/
  if ( grid_points[0] == 12 &&
       grid_points[1] == 12 &&
       grid_points[2] == 12 &&
       no_time_steps == 100) {

    *cclass = 'S';
    dtref = 1.5e-2;

/*--------------------------------------------------------------------
c    Reference values of RMS-norms of residual.
--------------------------------------------------------------------*/
    xcrref[0] = 2.7470315451339479e-02;
    xcrref[1] = 1.0360746705285417e-02;
    xcrref[2] = 1.6235745065095532e-02;
    xcrref[3] = 1.5840557224455615e-02;
    xcrref[4] = 3.4849040609362460e-02;

/*--------------------------------------------------------------------
c    Reference values of RMS-norms of solution error.
--------------------------------------------------------------------*/
    xceref[0] = 2.7289258557377227e-05;
    xceref[1] = 1.0364446640837285e-05;
    xceref[2] = 1.6154798287166471e-05;
    xceref[3] = 1.5750704994480102e-05;
    xceref[4] = 3.4177666183390531e-05;


/*--------------------------------------------------------------------
c    reference data for 36X36X36 grids after 400 time steps, with DT = 1.5d-03
--------------------------------------------------------------------*/
  } else if (grid_points[0] == 36 &&
	     grid_points[1] == 36 &&
	     grid_points[2] == 36 &&
	     no_time_steps == 400) {

    *cclass = 'W';
    dtref = 1.5e-3;

/*--------------------------------------------------------------------
c    Reference values of RMS-norms of residual.
--------------------------------------------------------------------*/
    xcrref[0] = 0.1893253733584e-02;
    xcrref[1] = 0.1717075447775e-03;
    xcrref[2] = 0.2778153350936e-03;
    xcrref[3] = 0.2887475409984e-03;
    xcrref[4] = 0.3143611161242e-02;

/*--------------------------------------------------------------------
c    Reference values of RMS-norms of solution error.
--------------------------------------------------------------------*/
    xceref[0] = 0.7542088599534e-04;
    xceref[1] = 0.6512852253086e-05;
    xceref[2] = 0.1049092285688e-04;
    xceref[3] = 0.1128838671535e-04;
    xceref[4] = 0.1212845639773e-03;

/*--------------------------------------------------------------------
c    reference data for 64X64X64 grids after 400 time steps, with DT = 1.5d-03
--------------------------------------------------------------------*/
  } else if (grid_points[0] == 64 &&
	     grid_points[1] == 64 &&
	     grid_points[2] == 64 &&
	     no_time_steps == 400 ) {

    *cclass = 'A';
    dtref = 1.5e-3;

/*--------------------------------------------------------------------
c    Reference values of RMS-norms of residual.
--------------------------------------------------------------------*/
    xcrref[0] = 2.4799822399300195;
    xcrref[1] = 1.1276337964368832;
    xcrref[2] = 1.5028977888770491;
    xcrref[3] = 1.4217816211695179;
    xcrref[4] = 2.1292113035138280;

/*--------------------------------------------------------------------
c    Reference values of RMS-norms of solution error.
--------------------------------------------------------------------*/
    xceref[0] = 1.0900140297820550e-04;
    xceref[1] = 3.7343951769282091e-05;
    xceref[2] = 5.0092785406541633e-05;
    xceref[3] = 4.7671093939528255e-05;
    xceref[4] = 1.3621613399213001e-04;

/*--------------------------------------------------------------------
c    reference data for 102X102X102 grids after 400 time steps,
c    with DT = 1.0d-03
--------------------------------------------------------------------*/
  } else if (grid_points[0] == 102 &&
	     grid_points[1] == 102 &&
	     grid_points[2] == 102 &&
	     no_time_steps == 400) {

    *cclass = 'B';
    dtref = 1.0e-3;

/*--------------------------------------------------------------------
c    Reference values of RMS-norms of residual.
--------------------------------------------------------------------*/
    xcrref[0] = 0.6903293579998e+02;
    xcrref[1] = 0.3095134488084e+02;
    xcrref[2] = 0.4103336647017e+02;
    xcrref[3] = 0.3864769009604e+02;
    xcrref[4] = 0.5643482272596e+02;

/*--------------------------------------------------------------------
c    Reference values of RMS-norms of solution error.
--------------------------------------------------------------------*/
    xceref[0] = 0.9810006190188e-02;
    xceref[1] = 0.1022827905670e-02;
    xceref[2] = 0.1720597911692e-02;
    xceref[3] = 0.1694479428231e-02;
    xceref[4] = 0.1847456263981e-01;


/*--------------------------------------------------------------------
c    reference data for 162X162X162 grids after 400 time steps,
c    with DT = 0.67d-03
--------------------------------------------------------------------*/
  } else if (grid_points[0] == 162 &&
	     grid_points[1] == 162 &&
	     grid_points[2] == 162 &&
	     no_time_steps == 400) {

    *cclass = 'C';
    dtref = 0.67e-3;

/*--------------------------------------------------------------------
c    Reference values of RMS-norms of residual.
--------------------------------------------------------------------*/
    xcrref[0] = 0.5881691581829e+03;
    xcrref[1] = 0.2454417603569e+03;
    xcrref[2] = 0.3293829191851e+03;
    xcrref[3] = 0.3081924971891e+03;
    xcrref[4] = 0.4597223799176e+03;

/*--------------------------------------------------------------------
c    Reference values of RMS-norms of solution error.
--------------------------------------------------------------------*/
    xceref[0] = 0.2598120500183e+00;
    xceref[1] = 0.2590888922315e-01;
    xceref[2] = 0.5132886416320e-01;
    xceref[3] = 0.4806073419454e-01;
    xceref[4] = 0.5483377491301e+00;
    
  } else {
    *verified = FALSE;
  }

/*--------------------------------------------------------------------
c    verification test for residuals if gridsize is either 12X12X12 or 
c    64X64X64 or 102X102X102 or 162X162X162
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c    Compute the difference of solution values and the known reference values.
--------------------------------------------------------------------*/
  for (m = 0; m < 5; m++) {
           
    xcrdif[m] = fabs((xcr[m]-xcrref[m])/xcrref[m]) ;
    xcedif[m] = fabs((xce[m]-xceref[m])/xceref[m]);
           
  }

/*--------------------------------------------------------------------
c    Output the comparison of computed results to known cases.
--------------------------------------------------------------------*/

  if (*cclass != 'U') {
    printf(" Verification being performed for cclass %1c\n", *cclass);
    printf(" accuracy setting for epsilon = %20.13e\n", epsilon);
    
    if (fabs(dt-dtref) > epsilon) {
      *verified = FALSE;
      *cclass = 'U';
      printf(" DT does not match the reference value of %15.8e\n", dtref);
    }
  } else {
    printf(" Unknown cclass\n");
  }

  if (*cclass != 'U') {
    printf(" Comparison of RMS-norms of residual\n");
  } else {
    printf(" RMS-norms of residual\n");
  }

  for (m = 0; m < 5; m++) {
    if (*cclass == 'U') {
      printf("          %2d%20.13e\n", m, xcr[m]);
    } else if (xcrdif[m] > epsilon) {
      *verified = FALSE;
      printf(" FAILURE: %2d%20.13e%20.13e%20.13e\n",
	     m,xcr[m],xcrref[m],xcrdif[m]);
    } else {
      printf("          %2d%20.13e%20.13e%20.13e\n",
	     m,xcr[m],xcrref[m],xcrdif[m]);
    }
  }

  if (*cclass != 'U') {
    printf(" Comparison of RMS-norms of solution error\n");
  } else {
    printf(" RMS-norms of solution error\n");
  }
        
  for (m = 0; m < 5; m++) {
    if (*cclass == 'U') {
      printf("          %2d%20.13e\n", m, xce[m]);
    } else if (xcedif[m] > epsilon) {
      *verified = FALSE;
      printf(" FAILURE: %2d%20.13e%20.13e%20.13e\n",
	     m,xce[m],xceref[m],xcedif[m]);
    } else {
      printf("          %2d%20.13e%20.13e%20.13e\n",
	     m,xce[m],xceref[m],xcedif[m]);
    }
  }
        
  if (*cclass == 'U') {
    printf(" No reference values provided\n");
    printf(" No verification performed\n");
  } else if (*verified) {
    printf(" Verification Successful\n");
  } else {
    printf(" Verification failed\n");
  }
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

static void x_solve(void) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c this function performs the solution of the approximate factorization
c step in the x-direction for all five matrix components
c simultaneously. The Thomas algorithm is employed to solve the
c systems for the x-lines. Boundary conditions are non-periodic
--------------------------------------------------------------------*/

  int i, j, k, n, i1, i2, m;
  double fac1, fac2;

/*--------------------------------------------------------------------
c                          FORWARD ELIMINATION  
--------------------------------------------------------------------*/
  lhsx();

/*--------------------------------------------------------------------
c      perform the Thomas algorithm; first, FORWARD ELIMINATION     
--------------------------------------------------------------------*/
  n = 0;
  for (i = 0; i <= grid_points[0]-3; i++) {
    i1 = i  + 1;
    i2 = i  + 2;
#pragma omp for
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {

      	malloc(10.24);
        int iter=i*k*j;
        L_Heartbeat_OpenMP_Generate(omp_get_thread_num(),1,iter); 
	fac1               = 1./lhs[n+2][i][j][k];
	lhs[n+3][i][j][k]   = fac1*lhs[n+3][i][j][k];
	lhs[n+4][i][j][k]   = fac1*lhs[n+4][i][j][k];
	for (m = 0; m < 3; m++) {
	  rhs[m][i][j][k] = fac1*rhs[m][i][j][k];
	}
	lhs[n+2][i1][j][k] = lhs[n+2][i1][j][k] -
	  lhs[n+1][i1][j][k]*lhs[n+3][i][j][k];
	lhs[n+3][i1][j][k] = lhs[n+3][i1][j][k] -
	  lhs[n+1][i1][j][k]*lhs[n+4][i][j][k];
	for (m = 0; m < 3; m++) {
	  rhs[m][i1][j][k] = rhs[m][i1][j][k] -
	    lhs[n+1][i1][j][k]*rhs[m][i][j][k];
	}
	lhs[n+1][i2][j][k] = lhs[n+1][i2][j][k] -
	  lhs[n+0][i2][j][k]*lhs[n+3][i][j][k];
	lhs[n+2][i2][j][k] = lhs[n+2][i2][j][k] -
	  lhs[n+0][i2][j][k]*lhs[n+4][i][j][k];
	for (m = 0; m < 3; m++) {
	  rhs[m][i2][j][k] = rhs[m][i2][j][k] -
	    lhs[n+0][i2][j][k]*rhs[m][i][j][k];
	}
      }
    }
  }

/*--------------------------------------------------------------------
c      The last two rows in this grid block are a bit different, 
c      since they do not have two more rows available for the
c      elimination of off-diagonal entries
--------------------------------------------------------------------*/

  i  = grid_points[0]-2;
  i1 = grid_points[0]-1;
#pragma omp for
  for (j = 1; j <= grid_points[1]-2; j++) {
    for (k = 1; k <= grid_points[2]-2; k++) {
      fac1               = 1.0/lhs[n+2][i][j][k];
      lhs[n+3][i][j][k]   = fac1*lhs[n+3][i][j][k];
      lhs[n+4][i][j][k]   = fac1*lhs[n+4][i][j][k];
      for (m = 0; m < 3; m++) {
	rhs[m][i][j][k] = fac1*rhs[m][i][j][k];
      }
      lhs[n+2][i1][j][k] = lhs[n+2][i1][j][k] -
	lhs[n+1][i1][j][k]*lhs[n+3][i][j][k];
      lhs[n+3][i1][j][k] = lhs[n+3][i1][j][k] -
	lhs[n+1][i1][j][k]*lhs[n+4][i][j][k];
      for (m = 0; m < 3; m++) {
	rhs[m][i1][j][k] = rhs[m][i1][j][k] -
	  lhs[n+1][i1][j][k]*rhs[m][i][j][k];
      }

/*--------------------------------------------------------------------
c            scale the last row immediately 
--------------------------------------------------------------------*/
      fac2               = 1./lhs[n+2][i1][j][k];
      for (m = 0; m < 3; m++) {
	rhs[m][i1][j][k] = fac2*rhs[m][i1][j][k];
      }
    }
  }

/*--------------------------------------------------------------------
c      do the u+c and the u-c factors                 
--------------------------------------------------------------------*/

  for (m = 3; m < 5; m++) {
    n = (m-3+1)*5;
    for (i = 0; i <= grid_points[0]-3; i++) {
      i1 = i  + 1;
      i2 = i  + 2;
#pragma omp for
      for (j = 1; j <= grid_points[1]-2; j++) {
	for (k = 1; k <= grid_points[2]-2; k++) {
	  fac1               = 1./lhs[n+2][i][j][k];
	  lhs[n+3][i][j][k]   = fac1*lhs[n+3][i][j][k];
	  lhs[n+4][i][j][k]   = fac1*lhs[n+4][i][j][k];
	  rhs[m][i][j][k] = fac1*rhs[m][i][j][k];
	  lhs[n+2][i1][j][k] = lhs[n+2][i1][j][k] -
	    lhs[n+1][i1][j][k]*lhs[n+3][i][j][k];
	  lhs[n+3][i1][j][k] = lhs[n+3][i1][j][k] -
	    lhs[n+1][i1][j][k]*lhs[n+4][i][j][k];
	  rhs[m][i1][j][k] = rhs[m][i1][j][k] -
	    lhs[n+1][i1][j][k]*rhs[m][i][j][k];
	  lhs[n+1][i2][j][k] = lhs[n+1][i2][j][k] -
	    lhs[n+0][i2][j][k]*lhs[n+3][i][j][k];
	  lhs[n+2][i2][j][k] = lhs[n+2][i2][j][k] -
	    lhs[n+0][i2][j][k]*lhs[n+4][i][j][k];
	  rhs[m][i2][j][k] = rhs[m][i2][j][k] -
	    lhs[n+0][i2][j][k]*rhs[m][i][j][k];
	}
      }
    }

/*--------------------------------------------------------------------
c         And again the last two rows separately
--------------------------------------------------------------------*/
    i  = grid_points[0]-2;
    i1 = grid_points[0]-1;
    
#pragma omp for
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	fac1               = 1./lhs[n+2][i][j][k];
	lhs[n+3][i][j][k]   = fac1*lhs[n+3][i][j][k];
	lhs[n+4][i][j][k]   = fac1*lhs[n+4][i][j][k];
	rhs[m][i][j][k]     = fac1*rhs[m][i][j][k];
	lhs[n+2][i1][j][k] = lhs[n+2][i1][j][k] -
	  lhs[n+1][i1][j][k]*lhs[n+3][i][j][k];
	lhs[n+3][i1][j][k] = lhs[n+3][i1][j][k] -
	  lhs[n+1][i1][j][k]*lhs[n+4][i][j][k];
	rhs[m][i1][j][k]   = rhs[m][i1][j][k] -
	  lhs[n+1][i1][j][k]*rhs[m][i][j][k];
/*--------------------------------------------------------------------
c               Scale the last row immediately
--------------------------------------------------------------------*/
	fac2               = 1./lhs[n+2][i1][j][k];
	rhs[m][i1][j][k]   = fac2*rhs[m][i1][j][k];

      }
    }
  }

/*--------------------------------------------------------------------
c                         BACKSUBSTITUTION 
--------------------------------------------------------------------*/

  i  = grid_points[0]-2;
  i1 = grid_points[0]-1;
  n = 0;
  for (m = 0; m < 3; m++) {
#pragma omp for
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] -
	  lhs[n+3][i][j][k]*rhs[m][i1][j][k];
      }
    }
  }

  for (m = 3; m < 5; m++) {
#pragma omp for
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	n = (m-3+1)*5;
	rhs[m][i][j][k] = rhs[m][i][j][k] -
	  lhs[n+3][i][j][k]*rhs[m][i1][j][k];
     
     
     
      }
    }
  }

/*--------------------------------------------------------------------
c      The first three factors
--------------------------------------------------------------------*/
  n = 0;
  for (i = grid_points[0]-3; i >= 0; i--) {
    i1 = i  + 1;
    i2 = i  + 2;
#pragma omp for
    for (m = 0; m < 3; m++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	      for (k = 1; k <= grid_points[2]-2; k++) {
	  rhs[m][i][j][k] = rhs[m][i][j][k] - 
	    lhs[n+3][i][j][k]*rhs[m][i1][j][k] -
	    lhs[n+4][i][j][k]*rhs[m][i2][j][k];

  }
      }
    }
  }

/*--------------------------------------------------------------------
c      And the remaining two
--------------------------------------------------------------------*/
  for (m = 3; m < 5; m++) {
    n = (m-3+1)*5;
    for (i = grid_points[0]-3; i >= 0; i--) {
      i1 = i  + 1;
      i2 = i  + 2;
#pragma omp for
      for (j = 1; j <= grid_points[1]-2; j++) {
	for (k = 1; k <= grid_points[2]-2; k++) {
	  rhs[m][i][j][k] = rhs[m][i][j][k] - 
	    lhs[n+3][i][j][k]*rhs[m][i1][j][k] -
	    lhs[n+4][i][j][k]*rhs[m][i2][j][k];
	}
      }
    }
  }

/*--------------------------------------------------------------------
c      Do the block-diagonal inversion          
--------------------------------------------------------------------*/
  ninvr();
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

static void y_solve(void) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c this function performs the solution of the approximate factorization
c step in the y-direction for all five matrix components
c simultaneously. The Thomas algorithm is employed to solve the
c systems for the y-lines. Boundary conditions are non-periodic
--------------------------------------------------------------------*/

  int i, j, k, n, j1, j2, m;
  double fac1, fac2;

/*--------------------------------------------------------------------
c                          FORWARD ELIMINATION  
--------------------------------------------------------------------*/
  lhsy();

  n = 0;

  for (j = 0; j <= grid_points[1]-3; j++) {
    j1 = j  + 1;
    j2 = j  + 2;
#pragma omp for      
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	fac1               = 1./lhs[n+2][i][j][k];
	lhs[n+3][i][j][k]   = fac1*lhs[n+3][i][j][k];
	lhs[n+4][i][j][k]   = fac1*lhs[n+4][i][j][k];
        for (m = 0; m < 3; m++) {
	  rhs[m][i][j][k] = fac1*rhs[m][i][j][k];
	}
	lhs[n+2][i][j1][k] = lhs[n+2][i][j1][k] -
	  lhs[n+1][i][j1][k]*lhs[n+3][i][j][k];
	lhs[n+3][i][j1][k] = lhs[n+3][i][j1][k] -
	  lhs[n+1][i][j1][k]*lhs[n+4][i][j][k];
	for (m = 0; m < 3; m++) {
	  rhs[m][i][j1][k] = rhs[m][i][j1][k] -
	    lhs[n+1][i][j1][k]*rhs[m][i][j][k];
	}
	lhs[n+1][i][j2][k] = lhs[n+1][i][j2][k] -
	  lhs[n+0][i][j2][k]*lhs[n+3][i][j][k];
	lhs[n+2][i][j2][k] = lhs[n+2][i][j2][k] -
	  lhs[n+0][i][j2][k]*lhs[n+4][i][j][k];
	for (m = 0; m < 3; m++) {
	  rhs[m][i][j2][k] = rhs[m][i][j2][k] -
	    lhs[n+0][i][j2][k]*rhs[m][i][j][k];
	}
      }
    }
  }

/*--------------------------------------------------------------------
c      The last two rows in this grid block are a bit different, 
c      since they do not have two more rows available for the
c      elimination of off-diagonal entries
--------------------------------------------------------------------*/

  j  = grid_points[1]-2;
  j1 = grid_points[1]-1;
#pragma omp for      
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (k = 1; k <= grid_points[2]-2; k++) {
      fac1               = 1./lhs[n+2][i][j][k];
      lhs[n+3][i][j][k]   = fac1*lhs[n+3][i][j][k];
      lhs[n+4][i][j][k]   = fac1*lhs[n+4][i][j][k];
      for (m = 0; m < 3; m++) {
	rhs[m][i][j][k] = fac1*rhs[m][i][j][k];
      }
      lhs[n+2][i][j1][k] = lhs[n+2][i][j1][k] -
	lhs[n+1][i][j1][k]*lhs[n+3][i][j][k];
      lhs[n+3][i][j1][k] = lhs[n+3][i][j1][k] -
	lhs[n+1][i][j1][k]*lhs[n+4][i][j][k];
      for (m = 0; m < 3; m++) {
	rhs[m][i][j1][k] = rhs[m][i][j1][k] -
	  lhs[n+1][i][j1][k]*rhs[m][i][j][k];
      }
/*--------------------------------------------------------------------
c            scale the last row immediately 
--------------------------------------------------------------------*/
      fac2               = 1./lhs[n+2][i][j1][k];
      for (m = 0; m < 3; m++) {
	rhs[m][i][j1][k] = fac2*rhs[m][i][j1][k];
      }
    }
  }

/*--------------------------------------------------------------------
c      do the u+c and the u-c factors                 
--------------------------------------------------------------------*/
  for (m = 3; m < 5; m++) {
    n = (m-3+1)*5;
    for (j = 0; j <= grid_points[1]-3; j++) {
      j1 = j  + 1;
      j2 = j  + 2;
#pragma omp for      
      for (i = 1; i <= grid_points[0]-2; i++) {
	for (k = 1; k <= grid_points[2]-2; k++) {
	  fac1               = 1./lhs[n+2][i][j][k];
	  lhs[n+3][i][j][k]   = fac1*lhs[n+3][i][j][k];
	  lhs[n+4][i][j][k]   = fac1*lhs[n+4][i][j][k];
	  rhs[m][i][j][k] = fac1*rhs[m][i][j][k];
	  lhs[n+2][i][j1][k] = lhs[n+2][i][j1][k] -
	    lhs[n+1][i][j1][k]*lhs[n+3][i][j][k];
	  lhs[n+3][i][j1][k] = lhs[n+3][i][j1][k] -
	    lhs[n+1][i][j1][k]*lhs[n+4][i][j][k];
	  rhs[m][i][j1][k] = rhs[m][i][j1][k] -
	    lhs[n+1][i][j1][k]*rhs[m][i][j][k];
	  lhs[n+1][i][j2][k] = lhs[n+1][i][j2][k] -
	    lhs[n+0][i][j2][k]*lhs[n+3][i][j][k];
	  lhs[n+2][i][j2][k] = lhs[n+2][i][j2][k] -
	    lhs[n+0][i][j2][k]*lhs[n+4][i][j][k];
	  rhs[m][i][j2][k] = rhs[m][i][j2][k] -
	    lhs[n+0][i][j2][k]*rhs[m][i][j][k];
	}
      }
    }

/*--------------------------------------------------------------------
c         And again the last two rows separately
--------------------------------------------------------------------*/
    j  = grid_points[1]-2;
    j1 = grid_points[1]-1;
#pragma omp for      
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	fac1               = 1./lhs[n+2][i][j][k];
	lhs[n+3][i][j][k]   = fac1*lhs[n+3][i][j][k];
	lhs[n+4][i][j][k]   = fac1*lhs[n+4][i][j][k];
	rhs[m][i][j][k]     = fac1*rhs[m][i][j][k];
	lhs[n+2][i][j1][k] = lhs[n+2][i][j1][k] -
	  lhs[n+1][i][j1][k]*lhs[n+3][i][j][k];
	lhs[n+3][i][j1][k] = lhs[n+3][i][j1][k] -
	  lhs[n+1][i][j1][k]*lhs[n+4][i][j][k];
	rhs[m][i][j1][k]   = rhs[m][i][j1][k] -
	  lhs[n+1][i][j1][k]*rhs[m][i][j][k];
/*--------------------------------------------------------------------
c               Scale the last row immediately 
--------------------------------------------------------------------*/
	fac2               = 1./lhs[n+2][i][j1][k];
	rhs[m][i][j1][k]   = fac2*rhs[m][i][j1][k];
      }
    }
  }

/*--------------------------------------------------------------------
c                         BACKSUBSTITUTION 
--------------------------------------------------------------------*/

  j  = grid_points[1]-2;
  j1 = grid_points[1]-1;
  n = 0;
  for (m = 0; m < 3; m++) {
#pragma omp for      
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] -
	  lhs[n+3][i][j][k]*rhs[m][i][j1][k];
      }
    }
  }

  for (m = 3; m < 5; m++) {
#pragma omp for      
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	n = (m-3+1)*5;
	rhs[m][i][j][k] = rhs[m][i][j][k] -
	  lhs[n+3][i][j][k]*rhs[m][i][j1][k];
      }
    }
  }

/*--------------------------------------------------------------------
c      The first three factors
--------------------------------------------------------------------*/
  n = 0;
  for (m = 0; m < 3; m++) {
    for (j = grid_points[1]-3; j >= 0; j--) {
      j1 = j  + 1;
      j2 = j  + 2;
#pragma omp for      
      for (i = 1; i <= grid_points[0]-2; i++) {
	for (k = 1; k <= grid_points[2]-2; k++) {
	  rhs[m][i][j][k] = rhs[m][i][j][k] - 
	    lhs[n+3][i][j][k]*rhs[m][i][j1][k] -
	    lhs[n+4][i][j][k]*rhs[m][i][j2][k];
	}
      }
    }
  }

/*--------------------------------------------------------------------
c      And the remaining two
--------------------------------------------------------------------*/
  for (m = 3; m < 5; m++) {
    n = (m-3+1)*5;
    for (j = grid_points[1]-3; j >= 0; j--) {
      j1 = j  + 1;
      j2 = j1 + 1;
#pragma omp for      
      for (i = 1; i <= grid_points[0]-2; i++) {
	for (k = 1; k <= grid_points[2]-2; k++) {
	  rhs[m][i][j][k] = rhs[m][i][j][k] - 
	    lhs[n+3][i][j][k]*rhs[m][i][j1][k] -
	    lhs[n+4][i][j][k]*rhs[m][i][j2][k];
	}
      }
    }
  }

  pinvr();
}

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

static void z_solve(void) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c this function performs the solution of the approximate factorization
c step in the z-direction for all five matrix components
c simultaneously. The Thomas algorithm is employed to solve the
c systems for the z-lines. Boundary conditions are non-periodic
c-------------------------------------------------------------------*/

  int i, j, k, n, k1, k2, m;
  double fac1, fac2;

/*--------------------------------------------------------------------
c                          FORWARD ELIMINATION  
c-------------------------------------------------------------------*/

  lhsz();

  n = 0;

#pragma omp for
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 0; k <= grid_points[2]-3; k++) {
	k1 = k  + 1;
	k2 = k  + 2;
	fac1               = 1./lhs[n+2][i][j][k];
	lhs[n+3][i][j][k]   = fac1*lhs[n+3][i][j][k];
	lhs[n+4][i][j][k]   = fac1*lhs[n+4][i][j][k];
	for (m = 0; m < 3; m++) {
	  rhs[m][i][j][k] = fac1*rhs[m][i][j][k];
	}
	lhs[n+2][i][j][k1] = lhs[n+2][i][j][k1] -
	  lhs[n+1][i][j][k1]*lhs[n+3][i][j][k];
	lhs[n+3][i][j][k1] = lhs[n+3][i][j][k1] -
	  lhs[n+1][i][j][k1]*lhs[n+4][i][j][k];
	for (m = 0; m < 3; m++) {
	  rhs[m][i][j][k1] = rhs[m][i][j][k1] -
	    lhs[n+1][i][j][k1]*rhs[m][i][j][k];
	}
	lhs[n+1][i][j][k2] = lhs[n+1][i][j][k2] -
	  lhs[n+0][i][j][k2]*lhs[n+3][i][j][k];
	lhs[n+2][i][j][k2] = lhs[n+2][i][j][k2] -
	  lhs[n+0][i][j][k2]*lhs[n+4][i][j][k];
	for (m = 0; m < 3; m++) {
	  rhs[m][i][j][k2] = rhs[m][i][j][k2] -
	    lhs[n+0][i][j][k2]*rhs[m][i][j][k];
	}
      }
    }
  }

/*--------------------------------------------------------------------
c      The last two rows in this grid block are a bit different, 
c      since they do not have two more rows available for the
c      elimination of off-diagonal entries
c-------------------------------------------------------------------*/
  k  = grid_points[2]-2;
  k1 = grid_points[2]-1;
#pragma omp for
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (j = 1; j <= grid_points[1]-2; j++) {
      fac1               = 1./lhs[n+2][i][j][k];
      lhs[n+3][i][j][k]   = fac1*lhs[n+3][i][j][k];
      lhs[n+4][i][j][k]   = fac1*lhs[n+4][i][j][k];
      for (m = 0; m < 3; m++) {
	rhs[m][i][j][k] = fac1*rhs[m][i][j][k];
      }
      lhs[n+2][i][j][k1] = lhs[n+2][i][j][k1] -
	lhs[n+1][i][j][k1]*lhs[n+3][i][j][k];
      lhs[n+3][i][j][k1] = lhs[n+3][i][j][k1] -
	lhs[n+1][i][j][k1]*lhs[n+4][i][j][k];
      for (m = 0; m < 3; m++) {
	rhs[m][i][j][k1] = rhs[m][i][j][k1] -
	  lhs[n+1][i][j][k1]*rhs[m][i][j][k];
      }

/*--------------------------------------------------------------------
c               scale the last row immediately
c-------------------------------------------------------------------*/
      fac2               = 1./lhs[n+2][i][j][k1];
      for (m = 0; m < 3; m++) {
	rhs[m][i][j][k1] = fac2*rhs[m][i][j][k1];
      }
    }
  }

/*--------------------------------------------------------------------
c      do the u+c and the u-c factors               
c-------------------------------------------------------------------*/
  for (m = 3; m < 5; m++) {
    n = (m-3+1)*5;
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	for (k = 0; k <= grid_points[2]-3; k++) {
	k1 = k  + 1;
	k2 = k  + 2;
	  fac1               = 1./lhs[n+2][i][j][k];
	  lhs[n+3][i][j][k]   = fac1*lhs[n+3][i][j][k];
	  lhs[n+4][i][j][k]   = fac1*lhs[n+4][i][j][k];
	  rhs[m][i][j][k] = fac1*rhs[m][i][j][k];
	  lhs[n+2][i][j][k1] = lhs[n+2][i][j][k1] -
	    lhs[n+1][i][j][k1]*lhs[n+3][i][j][k];
	  lhs[n+3][i][j][k1] = lhs[n+3][i][j][k1] -
	    lhs[n+1][i][j][k1]*lhs[n+4][i][j][k];
	  rhs[m][i][j][k1] = rhs[m][i][j][k1] -
	    lhs[n+1][i][j][k1]*rhs[m][i][j][k];
	  lhs[n+1][i][j][k2] = lhs[n+1][i][j][k2] -
	    lhs[n+0][i][j][k2]*lhs[n+3][i][j][k];
	  lhs[n+2][i][j][k2] = lhs[n+2][i][j][k2] -
	    lhs[n+0][i][j][k2]*lhs[n+4][i][j][k];
	  rhs[m][i][j][k2] = rhs[m][i][j][k2] -
	    lhs[n+0][i][j][k2]*rhs[m][i][j][k];
	}
      }
    }

/*--------------------------------------------------------------------
c         And again the last two rows separately
c-------------------------------------------------------------------*/
    k  = grid_points[2]-2;
    k1 = grid_points[2]-1;
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	fac1               = 1./lhs[n+2][i][j][k];
	lhs[n+3][i][j][k]   = fac1*lhs[n+3][i][j][k];
	lhs[n+4][i][j][k]   = fac1*lhs[n+4][i][j][k];
	rhs[m][i][j][k]     = fac1*rhs[m][i][j][k];
	lhs[n+2][i][j][k1] = lhs[n+2][i][j][k1] -
	  lhs[n+1][i][j][k1]*lhs[n+3][i][j][k];
	lhs[n+3][i][j][k1] = lhs[n+3][i][j][k1] -
	  lhs[n+1][i][j][k1]*lhs[n+4][i][j][k];
	rhs[m][i][j][k1]   = rhs[m][i][j][k1] -
	  lhs[n+1][i][j][k1]*rhs[m][i][j][k];
/*--------------------------------------------------------------------
c               Scale the last row immediately (some of this is overkill
c               if this is the last cell)
c-------------------------------------------------------------------*/
	fac2               = 1./lhs[n+2][i][j][k1];
	rhs[m][i][j][k1]   = fac2*rhs[m][i][j][k1];

      }
    }
  }

/*--------------------------------------------------------------------
c                         BACKSUBSTITUTION 
c-------------------------------------------------------------------*/

  k  = grid_points[2]-2;
  k1 = grid_points[2]-1;
  n = 0;
  for (m = 0; m < 3; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] -
	  lhs[n+3][i][j][k]*rhs[m][i][j][k1];
      }
    }
  }

  for (m = 3; m < 5; m++) {
    n = (m-3+1)*5;
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] -
	  lhs[n+3][i][j][k]*rhs[m][i][j][k1];
      }
    }
  }

/*--------------------------------------------------------------------
c      Whether or not this is the last processor, we always have
c      to complete the back-substitution 
c-------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c      The first three factors
c-------------------------------------------------------------------*/
  n = 0;
  for (m = 0; m < 3; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	for (k = grid_points[2]-3; k >= 0; k--) {
	  k1 = k  + 1;
	  k2 = k  + 2;
	  rhs[m][i][j][k] = rhs[m][i][j][k] - 
	    lhs[n+3][i][j][k]*rhs[m][i][j][k1] -
	    lhs[n+4][i][j][k]*rhs[m][i][j][k2];
	}
      }
    }
  }

/*--------------------------------------------------------------------
c      And the remaining two
c-------------------------------------------------------------------*/
  for (m = 3; m < 5; m++) {
    n = (m-3+1)*5;
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	for (k = grid_points[2]-3; k >= 0; k--) {
	  k1 = k  + 1;
	  k2 = k  + 2;
	  rhs[m][i][j][k] = rhs[m][i][j][k] - 
	    lhs[n+3][i][j][k]*rhs[m][i][j][k1] -
	    lhs[n+4][i][j][k]*rhs[m][i][j][k2];
	}
      }
    }
  }

  tzetar();
}



/* cat ./common/c_print_results.c */
/*****************************************************************/
/******     C  _  P  R  I  N  T  _  R  E  S  U  L  T  S     ******/
/*****************************************************************/
void c_print_results( char   *name,
                      char   cclass,
                      int    n1, 
                      int    n2,
                      int    n3,
                      int    niter,
                      int    nthreads,
                      double t,
                      double mops,
                      char   *optype,
                      int    passed_verification,
                      char   *npbversion,
                      char   *compiletime,
                      char   *cc,
                      char   *clink,
                      char   *c_lib,
                      char   *c_inc,
                      char   *cflags,
                      char   *clinkflags,
                      char   *rand)
{
    char *evalue="1000";

    printf( "\n\n %s Benchmark Completed\n", name ); 

    printf( " Class           =                        %c\n", cclass );

    if( n2 == 0 && n3 == 0 )
        printf( " Size            =             %12d\n", n1 );   /* as in IS */
    else
        printf( " Size            =              %3dx%3dx%3d\n", n1,n2,n3 );

    printf( " Iterations      =             %12d\n", niter );
    
    printf( " Threads         =             %12d\n", nthreads );
 
    printf( " Time in seconds =             %12.2f\n", t );

    printf( " Mop/s total     =             %12.2f\n", mops );

    printf( " Operation type  = %24s\n", optype);

    if( passed_verification )
        printf( " Verification    =               SUCCESSFUL\n" );
    else
        printf( " Verification    =             UNSUCCESSFUL\n" );

    printf( " Version         =             %12s\n", npbversion );

    printf( " Compile date    =             %12s\n", compiletime );

    printf( "\n Compile options:\n" );

    printf( "    CC           = %s\n", cc );

    printf( "    CLINK        = %s\n", clink );

    printf( "    C_LIB        = %s\n", c_lib );

    printf( "    C_INC        = %s\n", c_inc );

    printf( "    CFLAGS       = %s\n", cflags );

    printf( "    CLINKFLAGS   = %s\n", clinkflags );

    printf( "    RAND         = %s\n", rand );
#ifdef SMP
    evalue = getenv("MP_SET_NUMTHREADS");
    printf( "   MULTICPUS = %s\n", evalue );
#endif

/*    printf( "\n\n" );
    printf( " Please send the results of this run to:\n\n" );
    printf( " NPB Development Team\n" );
    printf( " Internet: npb@nas.nasa.gov\n \n" );
    printf( " If email is not available, send this to:\n\n" );
    printf( " MS T27A-1\n" );
    printf( " NASA Ames Research Center\n" );
    printf( " Moffett Field, CA  94035-1000\n\n" );
    printf( " Fax: 415-604-3957\n\n" );*/
}


/*
cat ./common/c_timers.c
*/
/*
#include "wtime.h"
#if defined(IBM)
#define wtime wtime
#elif defined(CRAY)
#define wtime WTIME
#else
#define wtime wtime_
#endif

*/
/*  Prototype  */
void wtime( double * );


/*****************************************************************/
/******         E  L  A  P  S  E  D  _  T  I  M  E          ******/
/*****************************************************************/
double elapsed_time( void )
{
    double t;

    wtime( &t );
    return( t );
}


double start[64], elapsed[64];

/*****************************************************************/
/******            T  I  M  E  R  _  C  L  E  A  R          ******/
/*****************************************************************/
void timer_clear( int n )
{
    elapsed[n] = 0.0;
}


/*****************************************************************/
/******            T  I  M  E  R  _  S  T  A  R  T          ******/
/*****************************************************************/
void timer_start( int n )
{
    start[n] = elapsed_time();
}


/*****************************************************************/
/******            T  I  M  E  R  _  S  T  O  P             ******/
/*****************************************************************/
void timer_stop( int n )
{
    double t, now;

    now = elapsed_time();
    t = now - start[n];
    elapsed[n] += t;

}


/*****************************************************************/
/******            T  I  M  E  R  _  R  E  A  D             ******/
/*****************************************************************/
double timer_read( int n )
{
    return( elapsed[n] );
}


void wtime(double *t)
{
  static int sec = -1;
  struct timeval tv;
//  gettimeofday(&tv, (void *)0);
  gettimeofday(&tv, (struct timezone *)0);
  if (sec < 0) sec = tv.tv_sec;
  *t = (tv.tv_sec - sec) + 1.0e-6*tv.tv_usec;
}

// common/c_randdp.c
/*
*/
#if defined(USE_POW)
#define r23 pow(0.5, 23.0)
#define r46 (r23*r23)
#define t23 pow(2.0, 23.0)
#define t46 (t23*t23)
#else
#define r23 (0.5*0.5*0.5*0.5*0.5*0.5*0.5*0.5*0.5*0.5*0.5*0.5*0.5*0.5*0.5*0.5*0.5*0.5*0.5*0.5*0.5*0.5*0.5)
#define r46 (r23*r23)
#define t23 (2.0*2.0*2.0*2.0*2.0*2.0*2.0*2.0*2.0*2.0*2.0*2.0*2.0*2.0*2.0*2.0*2.0*2.0*2.0*2.0*2.0*2.0*2.0)
#define t46 (t23*t23)
#endif

/*c---------------------------------------------------------------------
c---------------------------------------------------------------------*/

double randlc (double *x, double a) {

/*c---------------------------------------------------------------------
c---------------------------------------------------------------------*/

/*c---------------------------------------------------------------------
c
c   This routine returns a uniform pseudorandom double precision number in the
c   range (0, 1) by using the linear congruential generator
c
c   x_{k+1} = a x_k  (mod 2^46)
c
c   where 0 < x_k < 2^46 and 0 < a < 2^46.  This scheme generates 2^44 numbers
c   before repeating.  The argument A is the same as 'a' in the above formula,
c   and X is the same as x_0.  A and X must be odd double precision integers
c   in the range (1, 2^46).  The returned value RANDLC is normalized to be
c   between 0 and 1, i.e. RANDLC = 2^(-46) * x_1.  X is updated to contain
c   the new seed x_1, so that subsequent calls to RANDLC using the same
c   arguments will generate a continuous sequence.
c
c   This routine should produce the same results on any computer with at least
c   48 mantissa bits in double precision floating point data.  On 64 bit
c   systems, double precision should be disabled.
c
c   David H. Bailey     October 26, 1990
c
c---------------------------------------------------------------------*/

    double t1,t2,t3,t4,a1,a2,x1,x2,z;

/*c---------------------------------------------------------------------
c   Break A into two parts such that A = 2^23 * A1 + A2.
c---------------------------------------------------------------------*/
    t1 = r23 * a;
    a1 = (int)t1;
    a2 = a - t23 * a1;
/*c---------------------------------------------------------------------
c   Break X into two parts such that X = 2^23 * X1 + X2, compute
c   Z = A1 * X2 + A2 * X1  (mod 2^23), and then
c   X = 2^23 * Z + A2 * X2  (mod 2^46).
c---------------------------------------------------------------------*/
    t1 = r23 * (*x);
    x1 = (int)t1;
    x2 = (*x) - t23 * x1;
    t1 = a1 * x2 + a2 * x1;
    t2 = (int)(r23 * t1);
    z = t1 - t23 * t2;
    t3 = t23 * z + a2 * x2;
    t4 = (int)(r46 * t3);
    (*x) = t3 - t46 * t4;

    return (r46 * (*x));
}


/*c---------------------------------------------------------------------
c---------------------------------------------------------------------*/

void vranlc (int n, double *x_seed, double a, double* y) {
/* void vranlc (int n, double *x_seed, double a, double y[]) { */

/*c---------------------------------------------------------------------
c---------------------------------------------------------------------*/

/*c---------------------------------------------------------------------
c
c   This routine generates N uniform pseudorandom double precision numbers in
c   the range (0, 1) by using the linear congruential generator
c
c   x_{k+1} = a x_k  (mod 2^46)
c
c   where 0 < x_k < 2^46 and 0 < a < 2^46.  This scheme generates 2^44 numbers
c   before repeating.  The argument A is the same as 'a' in the above formula,
c   and X is the same as x_0.  A and X must be odd double precision integers
c   in the range (1, 2^46).  The N results are placed in Y and are normalized
c   to be between 0 and 1.  X is updated to contain the new seed, so that
c   subsequent calls to VRANLC using the same arguments will generate a
c   continuous sequence.  If N is zero, only initialization is performed, and
c   the variables X, A and Y are ignored.
c
c   This routine is the standard version designed for scalar or RISC systems.
c   However, it should produce the same results on any single processor
c   computer with at least 48 mantissa bits in double precision floating point
c   data.  On 64 bit systems, double precision should be disabled.
c
c---------------------------------------------------------------------*/

    int i;
    double x,t1,t2,t3,t4,a1,a2,x1,x2,z;

/*c---------------------------------------------------------------------
c   Break A into two parts such that A = 2^23 * A1 + A2.
c---------------------------------------------------------------------*/
    t1 = r23 * a;
    a1 = (int)t1;
    a2 = a - t23 * a1;
    x = *x_seed;

/*c---------------------------------------------------------------------
c   Generate N results.   This loop is not vectorizable.
c---------------------------------------------------------------------*/
    for (i = 1; i <= n; i++) {

/*c---------------------------------------------------------------------
c   Break X into two parts such that X = 2^23 * X1 + X2, compute
c   Z = A1 * X2 + A2 * X1  (mod 2^23), and then
c   X = 2^23 * Z + A2 * X2  (mod 2^46).
c---------------------------------------------------------------------*/
        t1 = r23 * x;
        x1 = (int)t1;
        x2 = x - t23 * x1;
        t1 = a1 * x2 + a2 * x1;
        t2 = (int)(r23 * t1);
        z = t1 - t23 * t2;
        t3 = t23 * z + a2 * x2;
        t4 = (int)(r46 * t3);
        x = t3 - t46 * t4;
        y[i] = r46 * x;
    }
    *x_seed = x;
}
