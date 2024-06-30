#include "corto_plazo.h"

void *corto_plazo(void * arg){
    pcb *pcb;
    argumentos_thread_scheduler * algoritmo = arg;
    switch(algoritmo->algoritmo){
        
        case FIFO:
        while(true){
            sleep(4);
            log_info(logger, "se inicio el planificador FIFO");
        
            sem_wait(&sem_p_ready); //espero que se desaloje el proceso anterior, sea por interrupcion o porque termino de ejecutar

            pthread_mutex_lock(&m_cola_ready);
            pcb = list_remove(sch_cola_ready, 0);
            pthread_mutex_unlock(&m_cola_ready);

            //enviar paquete de PCB al CPU
            list_add(sch_cola_exec, pcb); //agrego a cola de exec
            t_paquete *paquete_pcb = crear_paquete (PCB_CPU);
            enviar_paquete(paquete_pcb, socket_cliente_cpu);
        }
        

        break;

        case RR:
        log_info(logger, "se inicio el planificador Round Robin");
        break;

        case VRR:
        log_info(logger, "se inicio el planificador Virtual Round Robin");
        break;

    }
    
}