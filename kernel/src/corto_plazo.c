#include "corto_plazo.h"

void *corto_plazo(algoritmo_planificacion arg){
    pcb *pcb;

    switch(arg){
        
        case FIFO:
        sleep(4);
        log_info(logger, "se inicio el planificador FIFO");
        
        sem_wait(&sem_p_ready);

        pthread_mutex_lock(&m_cola_ready);
        pcb = list_remove(sch_cola_ready, 0);
        pthread_mutex_lock(&m_cola_ready);

        //enviar paquete de PCB al CPU
        t_paquete *paquete_pcb = crear_paquete (PCB_CPU);
        enviar_paquete(paquete_pcb, socket_cliente_cpu);

        break;

        case RR:
        log_info(logger, "se inicio el planificador Round Robin");
        break;

        case VRR:
        log_info(logger, "se inicio el planificador Virtual Round Robin");
        break;

    }
    
}