#define _GNU_SOURCE     // Importante para poder usar clone
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <malloc.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>

// 64kB stack
#define CICLES 2000000000
#define FIBER_STACK 1024*64
#define NPROCS 4

double PI=0;

// The child thread will execute this function
int threadPI(void * args){
    int nthread=*((int *)args);
    int inicio=nthread*(CICLES/NPROCS);
    int fin=(nthread+1)*(CICLES/NPROCS);
    double tempPi=0;// variable local para cada hilo
    for(long i=inicio;i<fin;i++){
        tempPi+=(pow(-1,i)/((2*i)+1)); // Formula en sumatoria de Leibniz
    }
    tempPi=tempPi*4;

    PI+=tempPi;
    //return PI;
}

int main()
{
    long long start_ts;
    long long stop_ts;
    long long elapsedTime;
    long lElapsedTime;
    struct timeval ts;

    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec; //Tiempo inicial

	void* stack;
	pid_t pid;
	int status;
	int i;
	int params[NPROCS];
        
	// Guardar espacio en memoria para el stack
	stack = malloc( FIBER_STACK * NPROCS );
	if ( stack == 0 )
	{
		perror( "malloc: could not allocate stack" );
		exit( 1 );
	}
        
	printf( "Creating child thread\n" );
        
	for(i=0;i<NPROCS;i++)
	{
		params[i]=i;
		// Mandar llamar a clone con la funcion que haran los procesos hijos,  la direccion de su stack y parametros.
		pid = clone( &threadPI, (char*) stack + FIBER_STACK*(i+1), 
				SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, &params[i] );
		if ( pid == -1 )
		{
			perror( "clone" );
			exit( 2 );
		}
		//printf("%d\n",i);
		usleep(1000);	// Dar tiempo para evitar errores
	}
	// Esperar a que los procesos clonados terminesn

	for(i=0;i<NPROCS;i++)
	{
		pid=wait(&status);
		if ( pid == -1 )
		{
			perror( "waitpid" );
			exit( 3 );
		}
	}
        
	// Liberar el stack
	free( stack );
	printf("Resultado, %f\n", PI);  
    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec; //Tiempo final

    elapsedTime = stop_ts - start_ts;
    printf("----------------------\n");
    printf("Tiempo total, %lld segundos\n", elapsedTime);
     
	return 0;
}