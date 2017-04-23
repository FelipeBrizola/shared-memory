
/** Jantar dos Canibais 
 * Autor: Prof. Carlos R. Moratelli - PUCRS
 * Ano 2017
 */ 

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h>

/* Termina as threads quando assume valor 0.*/
volatile int running = 1;

/* Semáforo de contagem representando a travessa. */
sem_t travessa;

/* Mutex do cozinheiro. */
pthread_mutex_t acordaCozinheiro;

/* Mutex que representa a fila de canibais para se servir. */
pthread_mutex_t mfila;

/* Usado pelos canibais para se servirem. */
int seserve() {

	/* Canibal entra na fila para comer. */
	pthread_mutex_lock(&mfila);	
	
	/* Serve-se */
	if(sem_trywait(&travessa) < 0){
		/* Travessa vazia, acorda o cozinheiro
		 * se ele ainda não estiver cozinhado. 
		 */
		pthread_mutex_unlock(&acordaCozinheiro);
		
		/* Canibal não conseguiu se servir, mantem a fila parada. */
		return 0;
	}
	
	/* Canibal libera o lugar na fila */
	pthread_mutex_unlock(&mfila);
	
	/* Canibal servido! */
	return 1;
}

/* Usado pelo cozinheiro para encher a travessa. */
void enchetravessa(int porcoes){
	int i;
	
	printf("Enchendo a Travessa.\n");
	fflush(stdout);
	
	/* Enche a travessa. */
	for(i=0;i<porcoes;i++){
		sem_post(&travessa);
	}
	
}

/* Thread canibal */
void * canibal(void *param){
	int id = *(int*)param;
	
	while(running){
		/* Canibal serve-se. */
		if (seserve() == 1){
			printf("Canibal Comendo %d\n", id);
			fflush(stdout);
			
			/* Canibal demora entre 500 e 1000 milisegundos para comer. */
			usleep( 500000 + rand() % 500000 ); 
		}
	}
	
	return NULL;
}

/* Thread Cozinheiro */
void * cozinheiro(void *param){
	int porcoes = *(int*)param;
	
	while(running){
		/* Espera ser acordado. */
		pthread_mutex_lock(&acordaCozinheiro);
		
		enchetravessa(porcoes);
		
		/* Libera a fila de canibais para se servirem. */
		pthread_mutex_unlock(&mfila);
	}
	
	return NULL;
}


int main(int argc, char** argv){
	pthread_t cozinheirothr;
	pthread_t *canibais;
	int i;
	int * ids;
	int runtime;
	int numcanibais;
	int numporcoes;

	/* Entradas do programa. 
	 * 
	 * Número de porções na travessa.
	 * Número de canibais participantes do jantar.
	 * Tempo de duração do jantar em segundos.
	 */
	if(argc < 4){
		printf("Uso: %s <#porções> <#canibais> <duração>\n", argv[0]);
		return 0;
	}
	
	/* Inicializa o gerador de números pseudo-aleatórios. */
	srand(time(NULL));
	
	/* Travessa começa vazia. */
	sem_init(&travessa, 0, 0);
	
	/* Inicializa mutex do cozinheiro */
	pthread_mutex_init(&acordaCozinheiro, NULL);
	
	/*Inicializa mutex do canibal */ 
	pthread_mutex_init(&mfila, NULL);
	
	/* Pega parâmetros do usuário. */
	numporcoes = atoi(argv[1]);
	numcanibais = atoi(argv[2]);
	runtime = atoi(argv[3]);
	
	/* Inicializa array com descritores dos canibais. */
	canibais = (pthread_t*)malloc(numcanibais * sizeof(pthread_t));
	
	/* Array com os ids dos canibais. */
	ids = (int*)malloc(numcanibais * sizeof(int));

	/* Dispara threads canibais. */
	for(i=0; i<numcanibais; i++){
		ids[i] = i;
		if(pthread_create(&canibais[i], NULL, canibal, (void*)&ids[i])) {
			fprintf(stderr, "Error creating thread\n");
			return 1;
		}
	}
	
	/* Dispara cozinheiro. */
	pthread_create(&cozinheirothr, NULL, cozinheiro, &numporcoes);

	/* Espera o jantar acontecer pelo tempo definido pelo usuário. */
	sleep(runtime);
	
	/* Sinaliza o término do jantar.*/
	running = 0;
	
	/* Espera os canibais terminarem. */
	for(i=0; i<numcanibais; i++){
		if(pthread_join(canibais[i], NULL)) {
			fprintf(stderr, "Error joining thread\n");
			return 1;
		}
	}
	
	pthread_mutex_destroy(&mfila);
	pthread_mutex_destroy(&acordaCozinheiro);
	sem_destroy(&travessa);
	free(canibais);
	free(ids);
	
	return 0;
}