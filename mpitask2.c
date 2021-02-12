#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double f(double x) {
	return (1 + x * x*x);
}

int main(int argc, char** argv) {
	int rank, size;
	double a = 0.0, b = 10.0;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int pro_num = size;
	int per_num = 10000, k;
	double w = (b - a) / (1.0 * pro_num * per_num);
	double part_num = 0.0, x;
	for (k = 0; k < per_num; ++k) {
		x = a+ (1.0 * rank * per_num + k) * w + w / 2.0;
		part_num += f(x) * w;
	}
	if (0 != rank) {
		MPI_Send(&part_num, 1, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD);
	}
	else {
		int j = 0;
		double result = part_num;
		double temp_res;
		for (j = 1; j < size; ++j) {
			MPI_Recv(&temp_res, 1, MPI_DOUBLE, j, j, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
			result += temp_res;
		}
		printf("result is : %.f\n", result);
	}
	MPI_Finalize();
	return 0;
}
