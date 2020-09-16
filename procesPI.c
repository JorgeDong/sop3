//Autores:
// Jorge Alejandro Dong Llauger IS714046
// Kevin Antonio Moreno Melgoza IS714714
//Fecha: 15/09/2020
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>

#define CICLES 2000000000 //Dos mil millones de ciclos
#define NTHREADS 4 

void pi(void * args,double shm_id)
{
	double* addr = (double*)shmat(shm_id,0,0); // apunta al inicio de la memoria
	if(addr == (double*) -1){
		printf("shmat error\n");
	    exit(1);
	}

	int nthread=*((int *)args);
    int inicio=nthread*(CICLES/NTHREADS);
    int fin=(nthread+1)*(CICLES/NTHREADS);
    double tempPi=0;// variable local para cada hilo
    for(long i=inicio;i<fin;i++){
        tempPi+=(pow(-1,i)/((2*i)+1)); // Formula en sumatoria de Leibniz
    }
    tempPi=tempPi*4;

	*addr +=tempPi;

	//printf("Soy el proceso %d, mi pid es %d\n",n,getpid());

	exit(0);
}

int main(){
	long long start_ts;
    long long stop_ts;
    long long elapsedTime;
    long lElapsedTime;
    struct timeval ts;

    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec; //Tiempo inicial

	int params[NTHREADS];
	int p,ret;
	double shm_id;  //id de espacio en memoria 
	shm_id = shmget(IPC_PRIVATE, sizeof(double), IPC_CREAT | 0666);// crea espacio en memoria. (espacio desocupado,tamaño a reservar,permisos de escritua y lectura)
	if (shm_id < 0) { // en caso de error son shmget
	     printf("shmget error\n");
	     exit(1);
	}

	//apuntador a double, apunta al espacio en memoria
	double* addr = (double*)shmat(shm_id,0,0); 
	for(int i=0;i<4;i++)
	{
		params[i]=i;
		p=fork();
		if(p==0){
			pi(&params[i],shm_id); // mandar a llamar la funcion en cada proceso hijo
		}
	}
	
	for(int i=0;i<4;i++)
	{
		ret=wait(NULL); // esperar a todos los procesos
	}


	double resultado = 	*addr; //acceder al resultado guardado en memoria

	printf("Resultado, %f\n", resultado);  
    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec; //Tiempo final
    elapsedTime = stop_ts - start_ts;
    printf("Tiempo total, %lld segundos\n", elapsedTime);
     


}

