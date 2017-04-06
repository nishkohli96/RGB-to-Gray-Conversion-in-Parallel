
#include<stdio.h>
#include "mpi.h"
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<math.h>
int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Status status;
	
	MPI_Finalize();
	return 0;
}
