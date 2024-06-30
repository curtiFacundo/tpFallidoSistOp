#include "largo_plazo.h"

void largo_plazo()
{ 
    pcb* pcb_p;
    while (1) {
        //sem_wait(&sem_largo_plazo);
        //sem_wait(&sem_multiprogramacion);
        if(list_size(sch_cola_ready) < 3){

            pthread_mutex_lock(&m_cola_new);
            pcb_p = list_remove(sch_cola_new, 0);
            pthread_mutex_unlock(&m_cola_new);

            t_paquete *paquete_pcb = crear_paquete (PCB_CPU);
            agregar_a_paquete(paquete_pcb, &(pcb_p->pid), sizeof(int));
            agregar_a_paquete(paquete_pcb, &(pcb_p->quantum), sizeof(int));
            enviar_paquete(paquete_pcb, cliente_memoria); 
            eliminar_paquete(paquete_pcb);

            cambiar_estado(pcb_p, READY);

            pthread_mutex_lock(&m_cola_ready);
            list_add(sch_cola_ready, pcb_p);
            pthread_mutex_unlock(&m_cola_ready);

            log_info(logger, "Proceso con id %d: New -> Ready", pcb_p->pid);

            //sem_post(&sem_procesos_en_ready);
            //sem_post(&sem_corto_plazo);
        }
    }
}

void eliminar_proceso(pcb* pcb_p) {
    // Enviar señal de interrupción si el proceso está en ejecución
    if (pcb_p->estado == EXEC) {
        enviar_interrupcion(pcb_p->pid);
        // Esperar a que retorne el contexto de ejecución
        recibir_contexto();
    }

    // Solicitar a la memoria liberar los recursos asociados al proceso
    t_paquete* paquete = crear_paquete(FIN_PRO);
    agregar_a_paquete(paquete, &(pcb_p->pid), sizeof(int));
    enviar_paquete(paquete, cliente_memoria);
    eliminar_paquete(paquete);

    // Actualizar el grado de multiprogramación
    // sem_post(&sem_multiprogramacion);

    // Mover un proceso de NEW a READY si es posible
    // sem_post(&sem_largo_plazo);

    log_info(logger, "Proceso con id %d eliminado", pcb_p->pid);
}

void enviar_interrupcion(int pid) {
    t_paquete* paquete = crear_paquete(INTERRUPCION);
    agregar_a_paquete(paquete,&pid, sizeof(int));
    enviar_paquete(paquete, socket_cliente_cpu);
    eliminar_paquete(paquete);

    log_info(logger, "Interrupción enviada al proceso con id %d", pid);
}

void recibir_contexto() {
    recibir_operacion(socket_cliente_cpu);
    t_list* lista = recibir_paquete(socket_cliente_cpu);
    RegistroCPU* registros = list_remove(lista, 0); // contiene los registros de la CPU del proceso
    int contador_programa = (int)list_remove(lista, 1); // contiene el contador de programa

    pthread_mutex_lock(&m_cola_exec);
    pcb* pcb_p = list_remove(sch_cola_exec, 0); //saco el pcb de kernel
    pcb_p->registros = registros;
    pcb_p->pc = contador_programa;
    list_add(sch_cola_exec,pcb_p);
    pthread_mutex_unlock(&m_cola_exec);
    sem_post(&sem_p_ready);


    list_destroy(lista);

    log_info(logger, "Contexto del proceso con id %d recibido", pcb_p->pid);

}