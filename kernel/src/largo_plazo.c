#include "largo_plazo.h"

void largo_plazo()
{ 
    pcb* pcb_p;
    while (1) {
        //sem_wait(&sem_largo_plazo);
        //sem_wait(&sem_multiprogramacion);

        pthread_mutex_lock(&m_cola_new);
        pcb_p = list_remove(sch_cola_ready, 0);
        pthread_mutex_unlock(&m_cola_new);

        t_paquete *paquete_pcb = crear_paquete (PCB_CPU);
        agregar_a_paquete(paquete_pcb, &(pcb_p->pid), sizeof(int));
        enviar_paquete(paquete_pcb, cliente); 
        eliminar_paquete(paquete_pcb);

        recibir_operacion(cliente);
        t_list* lista = recibir_paquete(cliente);
        int* tabla = list_remove(lista, 0);
        pcb_p->tabla_paginas = *tabla;

        list_destroy(lista);

        cambiar_estado(pcb_p, READY);

        pthread_mutex_lock(&m_cola_ready);
        list_add(sch_cola_ready, pcb_p);
        pthread_mutex_unlock(&m_cola_ready);

        log_info(logger, "Proceso con id %d: New -> Ready", pcb_p->pid);

        //sem_post(&sem_procesos_en_ready);
        //sem_post(&sem_corto_plazo);
    }
}

