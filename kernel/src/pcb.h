#ifndef PCB_H_
#define PCB_H_

#include  <stdint.h> // asegura que usamos tipos de enteros estándar 
#include <utils.h>

typedef enum  
{
    NEW,
    READY,
    EXEC,
    BLOCK,
    EXIT
}t_estado;

typedef struct{
    uint32_t PC;//PROGRAM COUNTER
    uint8_t AX;
    uint8_t BX;
    uint8_t CX;
    uint8_t DX;
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;
    uint32_t esp;
}RegistroCPU; // dice registros generales, supongo que se refiere a hacer esto :calavera:
typedef struct{
    int pid; // id del proceso
    int pc; // id program counter 
    int quantum; 
    t_estado estado; // t_estado
    t_list* instrucciones;
    RegistroCPU* registros;
}pcb;


pcb* crear_pcb(int pid, int quantum, RegistroCPU registros, t_list* instrucciones);
pcb* armar_pcb(int pc, int pid, int quantum, RegistroCPU registros, t_list* instrucciones,t_estado estado);
void eliminar_PCB(pcb* pcb_p);
void cambiar_estado(pcb* pcb_p, t_estado estado);
void element_destroyer(void* elemento);


#endif /* CORTO_PLAZO_H_ */