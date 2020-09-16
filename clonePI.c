#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#define CICLES 2000000000 //Dos mil millones de ciclos
#define NTHREADS 4 
double PI=0;

void *threadPI(void * args){
    int nthread=*((int *)args);
    int inicio=nthread*(CICLES/NTHREADS);
    int fin=(nthread+1)*(CICLES/NTHREADS);
    double tempPi=0;// variable local para cada hilo
    for(long i=inicio;i<fin;i++){
        tempPi+=(pow(-1,i)/((2*i)+1)); // Formula en sumatoria de Leibniz
    }
    tempPi=tempPi*4;

    PI+=tempPi;
    //return PI;

}

int main(){
    long long start_ts;
    long long stop_ts;
    long long elapsedTime;
    long lElapsedTime;
    struct timeval ts;

    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec; //Tiempo inicial

    //Creacion de hilos

    pthread_t tid[NTHREADS];
	int args [NTHREADS];

    for(int i=0;i<NTHREADS;i++){
		args[i]=i;
		pthread_create(&tid[i],0,threadPI,&args[i]);
	}

    //Recibir hilos
	for(int i=0;i<NTHREADS;i++){
		pthread_join(tid[i],NULL);
	}

    printf("PI= %f\n",PI);

   gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec; //Tiempo final

    elapsedTime = stop_ts - start_ts;
    printf("----------------------\n");
    printf("Tiempo total, %lld segundos\n", elapsedTime);
    printf("Resultado, %f\n", PI);
}