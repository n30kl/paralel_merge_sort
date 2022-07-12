#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <malloc.h>
#include <random>

#define size 5000000
double startT, stopT;

int* merge(int* A, int asize, int* B, int bsize) {
	int ai, bi, ci, i;
	int* C;
	int csize = asize + bsize;

	ai = 0;
	bi = 0;
	ci = 0;

	C = (int*)malloc(csize * sizeof(int));
	while ((ai < asize) && (bi < bsize)) {
		if (A[ai] <= B[bi]) {
			C[ci] = A[ai];
			ci++; ai++;
		}
		else {
			C[ci] = B[bi];
			ci++; bi++;
		}
	}

	if (ai >= asize)
		for (i = ci; i < csize; i++, bi++)
			C[i] = B[bi];
	else if (bi >= bsize)
		for (i = ci; i < csize; i++, ai++)
			C[i] = A[ai];

	for (i = 0; i < asize; i++)
		A[i] = C[i];
	for (i = 0; i < bsize; i++)
		B[i] = C[asize + i];

	return C;
}

void swap(int* v, int i, int j)
{
	int t;
	t = v[i];
	v[i] = v[j];
	v[j] = t;
}

void m_sort(int* A, int min, int max)
{
	int* C;
	int mid = (min + max) / 2;
	int lowerCount = mid - min + 1;
	int upperCount = max - mid;

	if (max == min) {
		return;
	}
	else {
		/* сортування першої частини */
		m_sort(A, min, mid);
		/* сортування другої частини */
		m_sort(A, mid + 1, max);
		/* Злиття частин */
		C = merge(A + min, lowerCount, A + mid + 1, upperCount);
	}
}

void non_growth_check(int* data)
{
	bool check = true;
	for (int i = 0; i < size-1; i++)
	{
		if (data[i] > data[i + 1])
		{
			check = false;
			printf("\nERROR!");
			break;
		}
	}
}

int main(int argc, char** argv) {
	int * data = (int*)malloc(size * sizeof(int));
	int* chunk;
	int* other;
	int m, n = size;
	int id, p;
	int s = 0;
	int i;
	int step;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	startT = clock();
	if (id == 0)
	{
		int r;
		s = n / p;
		r = n % p;
		data = (int*)malloc((n + s - r) * sizeof(int));
		for (i = 0; i < n; i++)
			data[i] = rand();
		if (r != 0)
		{
			for (i = n; i < n + s - r; i++)
				data[i] = 0;
			s = s + 1;
		}


		MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
		chunk = (int*)malloc(s * sizeof(int));
		MPI_Scatter(data, s, MPI_INT, chunk, s, MPI_INT, 0, MPI_COMM_WORLD);
		m_sort(chunk, 0, s - 1);
	}
	else
	{
		MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
		chunk = (int*)malloc(s * sizeof(int));
		MPI_Scatter(data, s, MPI_INT, chunk, s, MPI_INT, 0, MPI_COMM_WORLD);
		m_sort(chunk, 0, s - 1);
	}

	step = 1;
	while (step < p)
	{
		if (id % (2 * step) == 0)
		{
			if (id + step < p)
			{
				MPI_Recv(&m, 1, MPI_INT, id + step, 0, MPI_COMM_WORLD, &status);
				other = (int*)malloc(m * sizeof(int));
				MPI_Recv(other, m, MPI_INT, id + step, 0, MPI_COMM_WORLD, &status);
				chunk = merge(chunk, s, other, m);
				s = s + m;
			}
		}
		else
		{
			int near = id - step;
			MPI_Send(&s, 1, MPI_INT, near, 0, MPI_COMM_WORLD);
			MPI_Send(chunk, s, MPI_INT, near, 0, MPI_COMM_WORLD);
			break;
		}
		step = step * 2;
	}

	stopT = clock();

	MPI_Finalize();


	//printf("Input array: \n");
	//for (i = 0; i < n; i++)
	//{
	//	data[i] = rand();
	//	printf("%d ", data[i]);
	//}

	//m_sort(data, 0, n);
	non_growth_check(data);

	//printf("\nOutput array: \n");
	//for (i = 0; i < n; i++)
	//{
	//	data[i] = rand();
	//	printf("%d ", data[i]);
	//}

	printf("\nTime spend: %f secs", (stopT - startT) / CLOCKS_PER_SEC);
	scanf_s("%d", & i);
}
