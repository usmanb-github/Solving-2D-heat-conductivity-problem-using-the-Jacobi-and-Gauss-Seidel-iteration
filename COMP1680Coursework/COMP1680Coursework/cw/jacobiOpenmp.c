#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {


	int m;
	int n;
	double tol;// = 0.0001;
	double tstart, tstop;
	int i, j, iter, nthreads;

	m = atoi(argv[1]);
	n = atoi(argv[2]);
	tol = atof(argv[3]);

	/*printf("enter the problem sizes and the convergence tolerance\n");
	scanf("%d %d %lf", &n, &m, &tol);*/
	printf("Enter the number of threads (max 4) ");
	scanf("%d", &nthreads);
	printf("%d %d %lf\n", n, m, tol);

	double t[m + 2][n + 2], tnew[m + 1][n + 1], diff, difmax, priv_difmax;



	/* define the number of threads to be used */
	omp_set_num_threads(nthreads);
	tstart = omp_get_wtime();
	// initialise temperature array

	// initialise temperature array
	for (i = 0; i <= m + 1; i++) {
		for (j = 0; j <= n + 1; j++) {
			t[i][j] = 30.0;
		}
	}

	// fix boundary conditions
	for (i = 1; i <= m; i++) {
		t[i][0] = 20.0;//left
		t[i][n + 1] = 100.0;//right
	}
	for (j = 1; j <= n; j++) {
		t[0][j] = 10.0;//top
		t[m + 1][j] = 140.0; //bottom
	}
	iter = 0;
	difmax = 100000.0;

	while (difmax > tol) {
		iter++;
		// update temperature for next iteration

#pragma omp parallel for schedule(static) \
        default(shared) private(i,j)
		for (i = 1; i <= m; i++) {
			for (j = 1; j <= n; j++) {
				tnew[i][j] = (t[i - 1][j] + t[i + 1][j] + t[i][j - 1] + t[i][j + 1]) / 4.0;
			}
		}

#pragma omp parallel default(shared) private(i,j, diff, priv_difmax)
		{
			difmax = 0.0;
#pragma omp for schedule(static)
			for (i = 1; i <= m; i++) {
				for (j = 1; j <= n; j++) {
					diff = fabs(tnew[i][j] - t[i][j]);
					if (diff > difmax) {
						difmax = diff;
					}
					// copy new to old temperatures
					t[i][j] = tnew[i][j];
				}
			}
#pragma omp critical
			if (priv_difmax > difmax) {
				difmax = priv_difmax;
			}
		}

	}//while (difmax > tol)

	tstop = omp_get_wtime();

	// print results
	printf("iter = %d  difmax = %9.11lf", iter, difmax);
	/*for (i=0; i <= m+1; i++) {
		printf("\n");
		for (j=0; j <= n+1; j++) {
			printf("%3.5lf ", t[i][j]);
		}
	}
	printf("\n");*/

	printf("iterations = %d  maximum difference = %-5.7lf  \n", iter, difmax);
	printf("time taken is %4.3lf\n", (tstop - tstart));
}
