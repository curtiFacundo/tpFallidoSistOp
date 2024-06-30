#include "corto_plazo.h"

void *corto_plazo(void * arg){
    pcb *pcb;
    argumentos_thread_scheduler * algoritmo = arg;
    t_config *config_sch = config_create("../utils/config/corto_plazo.config");
    int quantum = config_get_int_value(config_sch, "QUANTUM");

    switch(algoritmo->algoritmo){
        
        case FIFO:
        sleep(4); //TESTEAR Y REMOVER DESPUES
        log_info(logger, "se inicio el planificador FIFO");
        while(true){    
            sem_wait(&sem_p_ready); //espero que se desaloje el proceso anterior, sea por interrupcion o porque termino de ejecutar

            pthread_mutex_lock(&m_cola_ready);
            pcb = list_remove(sch_cola_ready, 0);
            pthread_mutex_unlock(&m_cola_ready);

            //enviar paquete de PCB al CPU
            pthread_mutex_lock(&m_cola_exec);
            list_add(sch_cola_exec, pcb); //agrego a cola de exec
            pthread_mutex_unlock(&m_cola_exec);

            t_paquete *paquete_pcb = crear_paquete (PCB_CPU);
            enviar_paquete(paquete_pcb, socket_cliente_cpu);
        }
        

        break;

        case RR:
        log_info(logger, "se inicio el planificador Round Robin con Quantum = %d", quantum);
        while(true){    
            
            pcb = list_get(sch_cola_exec, 0);

            if(pcb->quantum >= quantum){

                pcb->quantum = 0;

                pthread_mutex_lock(&m_cola_exec);
                list_remove(sch_cola_exec,0);
                pthread_mutex_unlock(&m_cola_exec);

                pthread_mutex_lock(&m_cola_ready);
                pcb = list_remove(sch_cola_ready, 0);
                pthread_mutex_unlock(&m_cola_ready);

                pthread_mutex_lock(&m_cola_exec);
                list_add(sch_cola_exec, pcb);
                pthread_mutex_unlock(&m_cola_exec);

                t_paquete *paquete_pcb = crear_paquete (PCB_CPU);
                enviar_paquete(paquete_pcb, socket_cliente_cpu);

            }
        }
        break;

        case VRR:
        log_info(logger, "se inicio el planificador Virtual Round Robin con Quantum = %d", quantum);
        while(true){    
            
            pcb = list_get(sch_cola_exec, 0);

            if(pcb->quantum >= quantum){

                pcb->quantum = 0;

                pthread_mutex_lock(&m_cola_exec);
                list_remove(sch_cola_exec,0);
                pthread_mutex_unlock(&m_cola_exec);

                pthread_mutex_lock(&m_cola_ready);
                pcb = list_remove(sch_cola_ready, 0);
                pthread_mutex_unlock(&m_cola_ready);

                pthread_mutex_lock(&m_cola_exec);
                list_add(sch_cola_exec, pcb);
                pthread_mutex_unlock(&m_cola_exec);

                t_paquete *paquete_pcb = crear_paquete (PCB_CPU);
                enviar_paquete(paquete_pcb, socket_cliente_cpu);

            }
        }
        break;

    }
    
}