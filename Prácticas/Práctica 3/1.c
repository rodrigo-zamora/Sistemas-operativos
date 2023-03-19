#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define ITERACIONES 2000000000
#define PI 4

int main(){
	long long start_ts;
	long long stop_ts;
	int elapsed_time;
	struct timeval ts;
	int i;
	double sumatoria = 0;
	double num_elevado, pi_aproximado;
	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo inicial

	for(i=0; i<ITERACIONES; i++){
		num_elevado = pow(-1, i);
		sumatoria += (num_elevado / (2 * i + 1));
	}
	
	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo final

	printf("Resultado: %lf\n", sumatoria);
	pi_aproximado = sumatoria * PI;
	printf("Aproximación de pi: %lf\n", pi_aproximado);
	elapsed_time = (int) (stop_ts - start_ts);
	printf("Proceso %d, %d microsegundos\n", getpid(), elapsed_time);
	return 0;
}

