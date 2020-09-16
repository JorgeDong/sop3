#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#define CICLES 2000000000 // total de iteraciones del FOR

double serialPi();

int main(){
    //variables para contar el tiempo
    long long start_ts;
    long long stop_ts;
    long long elapsedTime;
    long lElapsedTime;
    struct timeval ts;

    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec; //Tiempo inicial
    
     double piResult = serialPi();

    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec; //Tiempo final

    elapsedTime = stop_ts - start_ts;
    printf("----------------------\n");
    printf("Tiempo total, %lld segundos\n", elapsedTime);
    printf("Resultado, %f\n", piResult);
}

 double serialPi(){
     double PI=0;
    for(long i = 0; i <CICLES; i++ ){
        PI +=  ((pow(-1,i))/((2*i)+1)); // formula de Leibnz
        //printf("%ld\n",i);
    }
    PI = PI * 4; // Multiplicar el resultado por 4 para obtener PI

    return PI;
}