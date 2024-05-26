#include "pcb.h"
#include "t_list.h"
pcb* crear_pcb(int pid, int quantum, RegistroCPU registros, t_list* instrucciones); //el pc_id no es necesario porque al crear el pcb apunta al 0
{
    pcb* pcb_p = (pcb*)(malloc(sizeof(pcb));// el (pcb*) convierte el puntero genérico (void* malloc) a un puntero específico de tipo pcb*.

    pcb_p->pid = pid;
    pcb_p->pc = 0;
    pcb_p->quantum = 0;
    pcb_p->instrucciones = instrucciones;
    pcb_p->pc = 0;
    pcb_p->estado = NEW;
    
    return pcb;
}