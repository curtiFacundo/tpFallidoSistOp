#include <stdlib.h>
#include "pcb.h"



// Función para crear un PCB
pcb* crear_pcb(int pid, int quantum, RegistroCPU registros, t_list* instrucciones)
{
    pcb* pcb_p = (pcb*)(malloc(sizeof(pcb)));
    pcb_p->pid = pid;
    pcb_p->pc = 0;
    pcb_p->quantum = 0;
    pcb_p->estado = NEW;
    pcb_p->registros = malloc(sizeof(RegistroCPU));
    return pcb_p;
}
pcb* armar_pcb(int pid, int pc, int quantum, RegistroCPU registros, t_list* instrucciones, t_estado estado)
{
    pcb* pcb_p = crear_pcb(pid, quantum, registros, instrucciones);
    pcb_p->pid = pid;
    pcb_p->pc = pc;
    pcb_p->quantum = quantum;
    pcb_p->instrucciones = instrucciones;
   
    *(pcb_p->registros) = registros;  // Copiar los valores de los registros
    pcb_p->estado = estado;

    return pcb_p;
}

// Función para destruir un elemento
void element_destroyer(void* elemento) {
    free(elemento);
}

// Función para eliminar un PCB
void eliminar_PCB(pcb* pcb_p) {
    list_destroy_and_destroy_elements(pcb_p->instrucciones, element_destroyer);
    free(pcb_p->registros);
    free(pcb_p);
}

// Función para cambiar el estado de un PCB
void cambiar_estado(pcb* pcb_p, t_estado estado)
{
    pcb_p->estado = estado;
}
