#include <iostream>
#include <omp.h>
using namespace std;

int check(int **a1, int **a2, int n) {
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			if(a1[i][j] != a2[i][j])
				return 0;
	return 1;
}

int main(int argc, char **argv) {
	int r;
	int sizes[] = {500, 750, 1000, 1250};
	for(int threads = 1; threads <= 8; threads *= 2) {
		for(int s = 0; s < 4; s++) {
			int r = sizes[s];
			int **a1 = (int **)malloc(r * sizeof(int *));
			int **a2 = (int **)malloc(r * sizeof(int *));
			int **a3 = (int **)malloc(r * sizeof(int *));
			int **a4 = (int **)malloc(r * sizeof(int *));
			
			for(int i = 0; i < r; i++)
				a1[i] = (int *)malloc(r * sizeof(int));
			for(int i = 0; i < r; i++)
				a2[i] = (int *)malloc(r * sizeof(int));
			for(int i = 0; i < r; i++)
				a3[i] = (int *)malloc(r * sizeof(int));
			for(int i = 0; i < r; i++)
				a4[i] = (int *)malloc(r * sizeof(int));

			int i, j, k;
			for(i = 0; i < r; i++) 
				for(j = 0; j < r; j++)
					for(k = 0; k < r; k++)
						a3[i][j] = a1[i][k] * a2[k][j];
			omp_set_num_threads(threads);
			double t = omp_get_wtime();
		
			#pragma omp parallel for private(j, k)
			for(int i = 0; i < r; i++) 
				for(int j = 0; j < r; j++)
					for(int k = 0; k < r; k++)
						a4[i][j] = a1[i][k] * a2[k][j];

			t = omp_get_wtime() - t;
			cout << "Threads: " << threads << "\n";
			cout << "Size: " << r << "\n";
			cout << "Time: " << t << "\n";
			if(check(a1, a2, r) == 1)
				cout << "Output is correct\n\n";	
			else 
				cout << "Output is wrong\n\n";	
			
		}
	}
	return 0;
}
