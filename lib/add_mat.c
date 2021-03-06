/*
  Add a matrix to a second matrix in Fortran storage format.  
  */

#include <stdlib.h>
#include <stdio.h>
#include "declarations.h"

void add_mat(A,B)
     struct blockmatrix A;
     struct blockmatrix B;
{
  int blk;
  int i,j;

  for (blk=1; blk<=A.nblocks; blk++)
    {
      switch (A.blocks[blk].blockcategory)
	{
	case DIAG:
#pragma omp simd
	  for (i=1; i<=A.blocks[blk].blocksize; i++)
	    {
	      B.blocks[blk].data.vec[i]+=A.blocks[blk].data.vec[i];
	    };
	  break;
	case MATRIX:
#pragma omp parallel for schedule(dynamic,64) default(none) private(i,j) shared(A,B,blk)
	  for (j=1; j<=A.blocks[blk].blocksize; j++)
#pragma omp simd
	    for (i=1; i<=A.blocks[blk].blocksize; i++)
	      B.blocks[blk].data.mat[ijtok(i,j,B.blocks[blk].blocksize)] +=
		A.blocks[blk].data.mat[ijtok(i,j,A.blocks[blk].blocksize)];
	  break;
	case PACKEDMATRIX:
	default:
	  printf("addscaledmat illegal block type \n");
	  exit(206);
	};
    };

}

