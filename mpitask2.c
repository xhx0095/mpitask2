#include "mpi.h"
#include <stdio.h>
#define min(x, y) ((x) > (y) ? (y) : (x))
float f(float x) {
	return  x;
}
float integral(float a, float b, float n) {
	float h = (b - a) / n, T = 0;
	for (float i = 1; i < n; i++)  T += f(a + i * h);  //1 <= k <= n - 1
	return h * (f(a) + 2 * T + f(b)) / 2;
}
int main(int argc, char *argv[])
{
	int m = 10, g= 10;
	int master = 0;
	int myid, numprocs;
	int i, j;
	float a[10],d[10];
	float part_result,c=0;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	/*master process*/
	if (master == myid) {
	
		for (i = 0; i < m; i++)
		{
			a[i] = i;
			
		}
		int numsent = 0;
		for (i = 1; i < min(numprocs, m + 1); i++)
		{
			MPI_Send(&a[i - 1], g, MPI_FLOAT, i, ++numsent, MPI_COMM_WORLD);
		}
		
		for (i = 0; i < m; i++)
		{
			MPI_Recv(&part_result, 1, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			 c+= part_result;
			if (numsent < m) {
				MPI_Send(&a[numsent], g, MPI_FLOAT, status.MPI_SOURCE, numsent + 1, MPI_COMM_WORLD);
				numsent = numsent + 1;

			}
			else { /*close slave process*/
				float close = 1.0;
				MPI_Send(&close, 1, MPI_FLOAT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);

			}
		}
		printf(" the integral of f(x)=x from 0 to 10 is %1.3f\t", c);
	}
	/*slave process calculate integral */
	else {
		while (1)
		{
			part_result = 0;
			MPI_Recv(&d[0], g, MPI_FLOAT, master, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			if (0 != status.MPI_TAG) {
				part_result = integral(d[0],d[0]+1,1000);
				MPI_Send(&part_result, 1, MPI_FLOAT, master, status.MPI_TAG, MPI_COMM_WORLD);
			}
			else {
				break;
			}
		}

	}
	MPI_Finalize();
}
