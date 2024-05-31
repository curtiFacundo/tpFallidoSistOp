#include <stdlib.h>
#include "pcb.h"

pcb* crear_pcb(int pid, int quantum, RegistroCPU registros, t_list* instrucciones) //el pc_id no es necesario porque al crear el pcb apunta al 0
{
    pcb* pcb_p = (pcb*)(malloc(sizeof(pcb));// el (pcb*) convierte el puntero genérico (void* malloc) a un puntero específico de tipo pcb*. Se le llama cast.

    pcb_p->pid = pid;
    pcb_p->pc = 0;
    pcb_p->quantum = 0;
    pcb_p->instrucciones = instrucciones;
    pcb_p->registros = NULL;
    pcb_p->estado = NEW;

    
    return pcb_p;
}

pcb* armar_pcb(int pid, int pc_id,int quantum, RegistroCPU registros, t_list* instrucciones)
{
    pcb* pcb_p = crear_pcb(pid, quantum, registros, instrucciones);
    pcb_p->pc_id = pc_id;
    pcb_p->quantum = quantum;
    pcb_p->estado = estado;
    pcb_p->registros = registros;
    return pcb_p;
}

void eliminar_PCB(pcb* pcb_p) { 
    list_destroy_and_destroy_elements(pcb_p->instrucciones, element_destroyer); /*Pregunta: "el pcb_p, es una estructura donde su parametro pc es un puntero
    que apunta a otra estructura? porque si es así tiene sentido crear un pcb_aux y liberar este, ya que si borro el original pierdo la cadena de la estructura*/
    free(pcb_p);
}
void element_destroyer(void* elemento){ // en el list.h de commons explica como funciona en relacion a list_destroy_and_destroy_elements
	free(elemento);
}
void cambiar_estado(pcb* pcb_p, int estado)
{
    pcb_p->estado = estado;
}