#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double f(double x) {
	return (2 + x );
}

int main(int argc, char** argv) {
	int rank, size;
	double a = 0.0, b = 5.0;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Status status;
	int pro_num = size;
	int per_num = 10000, k;
	double w = (b - a) / (1.0 * pro_num * per_num);
	double part_num = 0.0, x;
	for (k = 0; k < per_num; ++k) {
		x = a+ (1.0 * rank * per_num + k) * w + w / 2.0;
		part_num += f(x) * w;
	}
	if (0 != rank) {
		MPI_Recv(rank, 1, MPI_DOUBLE, rank, rank, MPI_COMM_WORLD, &status);
		MPI_Send(&part_num, 1, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD);
	}
	else {
		int j = 0;
		double result = part_num;
		double temp_res;
		for (j = 1; j < size; ++j) {
			MPI_Send(j, 1, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD);
		}
		for (j = 1; j < size; ++j) {
			MPI_Probe(MPI_ANY_SOURCE, rank, MPI_COMM_WORLD, &status);
			MPI_Get_count(&status, MPI_CHAR, &temp_res);
			MPI_Recv(&temp_res, 1, MPI_DOUBLE, j, j, MPI_COMM_WORLD, &status);
			result += temp_res;
		}
		printf("result is : %.3f\n", result);
	}
	MPI_Finalize();
	return 0;
}
