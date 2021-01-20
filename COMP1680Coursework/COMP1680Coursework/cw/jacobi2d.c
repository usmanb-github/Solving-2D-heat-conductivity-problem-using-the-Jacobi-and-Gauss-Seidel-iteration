#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[])
{
int m; 
int n;
double tol;// = 0.0001;

int i, j, iter;

struct timeval startTime, endTime;
gettimeofday(&startTime, NULL);

m = atoi(argv[1]);
n = atoi(argv[2]);
tol = atof(argv[3]);

double t[m+2][n+2], tnew[m+1][n+1], diff, difmax;

printf("%d %d %lf\n",m,n, tol);

// initialise temperature array
for (i=0; i <= m+1; i++) {
	for (j=0; j <= n+1; j++) {
		t[i][j] = 30.0;
	}
}

// fix boundary conditions
for (i=1; i <= m; i++) {
	t[i][0] = 20.0;//left
	t[i][n+1] = 100.0;//right
}
for (j=1; j <= n; j++) {
	t[0][j] = 10.0;//top
	t[m+1][j] = 140.0; //bottom
}


// main loop
iter = 0;
difmax = 1000000.0;
while (difmax > tol) {
	iter++;

	// update temperature for next iteration
	for (i=1; i <= m; i++) {
		for (j=1; j <= n; j++) {
			tnew[i][j] = (t[i-1][j]+t[i+1][j]+t[i][j-1]+t[i][j+1])/4.0;
		}
	}

	// work out maximum differences[32;47;16M between old and new temperatures
	difmax = 0.0;
	for (i=1; i <= m; i++) {
		for (j=1; j <= n; j++) {
			diff = fabs(tnew[i][j]-t[i][j]);
			if (diff > difmax) {
				difmax = diff;
			}
			// copy new to old temperatures
			t[i][j] = tnew[i][j];
		}
	}

}
	// print results
	printf("iter = %d  difmax = %9.11lf", iter, difmax);
	/*for (i=0; i <= m+1; i++) {
		printf("\n");
		for (j=0; j <= n+1; j++) {
			printf("%3.5lf ", t[i][j]);
		}
	}
	printf("\n");*/

gettimeofday(&endTime, NULL);
long totalTimeResult = ((endTime.tv_sec * 1000000 + endTime.tv_usec) - (startTime.tv_sec * 1000000 + startTime.tv_usec));
  printf("Runtime = %ld\n",totalTimeResult);
}

