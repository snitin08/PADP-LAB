#include <iostream>
#include <omp.h>
#include <string.h>
#include <math.h>
using namespace std;
double t;

inline long strike(bool composite[], long i, long stride, long limit) {
	for(; i <= limit; i += stride)
		composite[i] = true;
	return i;
}

long unfriendly_sieve(long n) {
	long m = (long)sqrt((double)n);
	long count = 0;
	bool *composite = new bool[n+1];
	memset(composite, 0 , n);
	t = omp_get_wtime();
	for(long i = 2; i <= m; i++) {
		if(!composite[i]) {
			count++;
			strike(composite, 2 * i, i, n);
		}
	}
	for(long i = m + 1; i <= n; i++)
		if(!composite[i])
			count++;
	t = omp_get_wtime() - t;
	delete[] composite;
	return count;
}

long cache_friendly_sieve(long n) {
	long count = 0;
	long m = (long) sqrt((double) n);
	bool *composite = new bool[n+1];
	memset(composite, 0, n);
	t = omp_get_wtime();
	long* factor = new long[m];
	long* striker = new long[m];
	long n_factor = 0;
	
	for(long i = 2; i <= m; i++) {
		if(!composite[i]) {
			count++;
			striker[n_factor] = strike(composite, 2 * i, i, m);
			factor[n_factor++] = i;
		}
	}

	for(long window = m+1; window <= n; window += m) {
		long limit = min(window + m - 1, n);
		for(long k = 0; k < n_factor; k++) {
			striker[k] = strike(composite, striker[k], factor[k], limit);
		}
		for(long i = window; i <= limit; i++)
			if(!composite[i])
				count++;
	}
	t = omp_get_wtime() - t;
	delete[] striker;
	delete[] factor;
	delete[] composite;

	return count;
}

long parallel_sieve(long n) {
	long count = 0;
	long m = (long)sqrt((double)n);
	long n_factor = 0;
	long *factor = new long[m];

	t = omp_get_wtime();
	
	#pragma omp parallel 
	{
		bool * composite = new bool[m+1];
		long * striker = new long[m];
		
		#pragma omp single 
		{
			memset(composite, 0, m);
			for(long i = 2; i <= m; i++) 		
				if(!composite[i]) {
					count++;
					striker[n_factor] = strike(composite, 2 * i, i, m);
					factor[n_factor++] = i;
				}
		}
		long base = -1;
		#pragma omp for reduction(+: count)
		for(long window = m + 1; window<=n; window += m) {
			memset(composite, 0, m);
			if(base != window) {
				base = window;
				for(long k = 0; k < n_factor; k++) {
					striker[k] = (base + factor[k] - 1) / factor[k] * factor[k] - base;				
				}
			
			}
			long limit = min(window + m - 1, n) - base;
			for(long k = 0; k < n_factor; ++k)
				striker[k] = strike(composite, striker[k], factor[k], limit) - m;
			for(long i = 0; i <= limit; i++)
				if(!composite[i])
					++count;
			base += m;
				
		}
		delete[] striker;
		delete[] composite;
	}
	t = omp_get_wtime() - t;
	delete[] factor;
	return count;
	
}

int main(int argc, char **argv) {
	long count = unfriendly_sieve(1000000000);
	cout << "Unfriendly sieve: " << t <<"\n";
	cout << "Count: " << count << "\n\n";
	count = cache_friendly_sieve(1000000000);
	cout << "Friendly sieve: " << t <<"\n";
	cout << "Count: " << count << "\n\n";
	count = parallel_sieve(1000000000);
	cout << "Parallel sieve: " << t <<"\n";
	cout << "Count: " << count << "\n\n";
	return 0;
}
