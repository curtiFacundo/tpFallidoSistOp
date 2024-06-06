#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *sample(){
    printf("Thread sample. \n");
}

/*void *bifurcacion(void *data){
    char *texto = (char *) data;
    struct timespec tiempo = { 
        1 , 0
    }//segundos y nanosegundos
    while(1){
        printf("%s\n", texto);
        pthread_delay_np(&tiempo);
    }
}
*/
int main(){
    pthread_t tid;
    pthread_create(&tid, NULL, &sample, NULL);
    pthread_join(fid, NULL);
    /*
    //Bifurcacion
    while(1){
        printf("Hola\n");
    }
    while(1){
        printf("Chau\n");
    }
    */

    pthread_t proceso1;
    pthread_t proceso2;
    //Crear el proceso(direccion de memoria del proceso1, null, funcion a ejecutar/direccion de memoria de la funcion, parametro a evaluar)
    pthread_create(&proceso1, NULL, &bifurcacion, "hola");
    pthread_create(&proceso2, NULL, &bifurcacion, "chau");
    //Unir el hilo de ejecucion al hilo/programa principal, 
    //cada funcion representa un hilo de ejecucion diferente
    pthread_join(proceso1, NULL);
    pthread_join(proceso2, NULL);
    return 0;
}