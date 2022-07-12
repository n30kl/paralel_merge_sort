/* merge sort */
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
	int n = size;
	int s = 0;
	int i;

	startT = clock();

	//printf("Input array: \n");
	//for (i = 0; i < n; i++)
	//{
	//	data[i] = rand();
	//	printf("%d ", data[i]);
	//}

	m_sort(data, 0, n);
	non_growth_check(data);

	//printf("\nOutput array: \n");
	//for (i = 0; i < n; i++)
	//{
	//	data[i] = rand();
	//	printf("%d ", data[i]);
	//}

	stopT = clock();

	printf("\nTime spend: %f secs", (stopT - startT) / CLOCKS_PER_SEC);
	scanf_s("%d", & i);
} 
