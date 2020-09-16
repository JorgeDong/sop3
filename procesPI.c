#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <stdio.h>
#include <math.h>

#define CICLES 2000000000 //Dos mil millones de ciclos
#define NTHREADS 4 

void pi(void * args,double shm_id)
{
	double* addr = (double*)shmat(shm_id,0,0);
	if(addr == (double*) -1){
		printf("shmat error\n");
	    exit(1);
	}

	nt nthread=*((int *)args);
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

	int p,ret;
	double shm_id;   
	shm_id = shmget(IPC_PRIVATE, 4*sizeof(double), IPC_CREAT | 0666);
	if (shm_id < 0) {
	     printf("shmget error\n");
	     exit(1);
	}


	for(int i=0;i<2;i++)
	{
		p=fork();
		if(p==0){
			pi(i,shm_id);
		}
	}
	
	for(i=0;i<2;i++)
	{
		ret=wait(NULL);
		printf("%d\n",ret);
	}


	double resultado = 	*addr;



}

