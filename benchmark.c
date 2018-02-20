#include <stdlib.h> // For: exit, drand48, malloc, free, NULL, EXIT_FAILURE
#include <stdio.h>  // For: perror
#include <string.h> // For: memset

#ifdef GETTIMEOFDAY
#include <sys/time.h> // For struct timeval, gettimeofday
#else
#include <time.h> // For struct timespec, clock_gettime, CLOCK_MONOTONIC
#endif

/* Your function must have the following signature: */
extern const char* dgemm_desc;
extern void square_dgemm (int, float*, float*, float*);
extern void square_dgemm_trace (int, float*, float*, float*, FILE*);

/* Auxiliary functions */

double wall_time ()
{
#ifdef GETTIMEOFDAY
  struct timeval t;
  gettimeofday (&t, NULL);
  return 1.*t.tv_sec + 1.e-6*t.tv_usec;
#else
  struct timespec t;
  clock_gettime (CLOCK_MONOTONIC, &t);
  return 1.*t.tv_sec + 1.e-9*t.tv_nsec;
#endif
}

void die (const char* message)
{
  perror (message);
  exit (EXIT_FAILURE);
}

void fill (float* p, int n)
{
  for (int i = 0; i < n; ++i)
    p[i] =  ((float)rand()/(float)RAND_MAX)*2 - 1; // Uniformly distributed over [-1, 1]
}

void flush_cache()
{
  int bigsize = 10000000;
  float *clr = NULL;
  clr = (float *)  calloc(bigsize,sizeof(float));
  if (clr == NULL) die ("failed to allocate buffer");
  fill( clr, bigsize);
}

/* The benchmarking program */
int main (int argc, char **argv)
{
  double seconds = 0;
  int n = 256;
  FILE *fp = NULL;

  if ( argc > 1 ) n = atoi(argv[1]);

  printf ("Description:\t%s on matrices of size %d x %d\n\n", dgemm_desc, n, n);

  if ( argc > 2 )
  {
    fp = fopen(argv[2], "w"); 
    if ( fp == NULL ) die("failed to open file");
  }

  /* allocate memory : IT MUST BE ALIGNED in order to use avx extensions */
  float* buf = NULL;
  buf = (float*) valloc (3 * n * n * sizeof(float));
  if (buf == NULL) die ("failed to allocate matrices");

  float* A = buf + 0;
  float* B = A + n*n;
  float* C = B + n*n;

  srand(time(NULL)); /* getting a new seed*/
  fill (A, n*n);
  fill (B, n*n);
  fill (C, n*n);
  if ( argc > 2 )
    square_dgemm_trace (n, A, B, C, fp);

  seconds = wall_time();
  square_dgemm (n, A, B, C);
  seconds = wall_time() - seconds;
  printf("Time %e s\n", seconds);

  free (buf);
  if ( argc > 2 ) fclose(fp);

  return 0;
}
