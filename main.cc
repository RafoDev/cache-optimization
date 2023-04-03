#include <iostream>
#include <cstdint>
#include <vector>
#include "timer.hpp"
using namespace std;

int main(int argc, char *argv[])
{
	int e = atoi(argv[1]);

	int le = 1 << e;

	if (argc > 2)
		le = 1 << atoi(argv[2]);

	// matrix shapes

	const uint64_t m = 1 << e;
	const uint64_t n = 1 << e;
	const uint64_t l = le;

	TIMERSTART(init)
	// sum_k A_ik * B_kj = sum_k A_ik * B^t_jk = C_ij
	std::vector<float> A(m * l, 0);	 // m x l
	std::vector<float> B(l * n, 0);	 // l x n
	std::vector<float> Bt(n * l, 0); // n x l
	std::vector<float> C(m * n, 0);	 // m x n
	TIMERSTOP(init)

	TIMERSTART(naive_mult)

	for (uint64_t i = 0; i < m; i++)
		for (uint64_t j = 0; j < n; j++)
		{
			float accum = 0;
			for (uint64_t k = 0; k < l; k++)
				accum += A[i * l + k] * B[k * n + j];
			C[i * n + j] = accum;
		}

	TIMERSTOP(naive_mult)

	TIMERSTART(transpose_and_mult)
	TIMERSTART(transpose)
	for (uint64_t k = 0; k < l; k++)
		for (uint64_t j = 0; j < n; j++)
			Bt[j * l + k] = B[k * n + j];
	TIMERSTOP(transpose)

	TIMERSTART(transpose_mult)
	for (uint64_t i = 0; i < m; i++)
		for (uint64_t j = 0; j < n; j++)
		{
			float accum = 0;
			for (uint64_t k = 0; k < l; k++)
				accum += A[i * l + k] * Bt[j * l + k];
			C[i * n + j] = accum;
		}

	TIMERSTOP(transpose_mult)
	TIMERSTOP(transpose_and_mult)
}