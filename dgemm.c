#include <stdio.h>

int TraceLimit = 100000;

const char* dgemm_desc = "A naive C version";

/* This routine performs a dgemm operation
 *  C := C + A * B
 * where A, B, and C are lda-by-lda matrices stored in row-major format.
 * On exit, A and B maintain their input values. */    
void square_dgemm (int n, float* A, float* B, float* C)
{
  /* For each row i of A */
  for (int i = 0; i < n; ++i)
    /* For each column j of B */
    for (int j = 0; j < n; ++j) 
    {
      /* Compute C(i,j) */
      float cij = C[i*n+j]; /* cij = C[i][j] */
      for( int k = 0; k < n; k++ )
		cij += A[k+i*n] * B[j+k*n]; /* cij += A[i][k]*B[k][j] */
      C[i*n+j] = cij;   /*C[i][j] = cij*/
    }
}

void square_dgemm_trace (int n, float* A, float* B, float* C, FILE *fp)
{
  int c = 0;
  /* For each row i of A */
  for (int i = 0; i < n; ++i)
    /* For each column j of B */
    for (int j = 0; j < n; ++j) 
    {
      /* Compute C(i,j) */
      fprintf(fp, "0\t%08x\n", (int) &C[i*n+j]); /* cij = C[i][j] */
      c++;
      if ( c > TraceLimit )
        return;
      for( int k = 0; k < n; k++ )
      {
        fprintf(fp, "0\t%08x\n", (int) &A[k+i*n]);
        fprintf(fp, "0\t%08x\n", (int) &B[j+k*n]);
        c+=2;
        if ( c > TraceLimit )
          return;
      }
      fprintf(fp, "1\t%08x\n", (int) &C[i*n+j]); /* cij = C[i][j] */
      c++;
      if ( c > TraceLimit )
        return;
    }
}
