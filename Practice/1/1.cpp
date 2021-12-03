#include<iostream>
#include<omp.h>
using namespace std;

int main(int argc, char *argv[]) {
	int count = 0, n, i;
	cout << "Enter the number of iterations\n";
	cin >> n;

	srand(42);
	cout << "\t";
	for(i = 1; i <= 8; i *= 2)
		cout << i << "\t";
	cout << "\n";
	for (n = 1000; n <= 10000; n *= 10) {
		cout << n << "\t";
		for(int threads = 1; threads <= 8; threads *= 2) {
			double start_time = omp_get_wtime();
			count = 0;
			omp_set_num_threads(threads);
			#pragma omp parallel for 
			//reduction(+: count)
			for(i = 0; i < n; i++) {
				double x = (rand() % n) / (double) n;
				double y = (rand() % n) / (double) n;
				double z = x * x + y * y;
				if(z <= 1) 
					count++;
			}
			double end_time = omp_get_wtime();
			double calculation_time = end_time - start_time;
			double pi = (double) count / n * 4;
			
			cout << calculation_time << "\t";
		}
	cout << "\n";
	}	
}
