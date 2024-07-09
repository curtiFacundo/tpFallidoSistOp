#ifndef PCB_H_
#define PCB_H_

#include  <stdint.h> 
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
    uint32_t PC;
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
}RegistroCPU;
typedef struct {
    uint32_t parametros[2];
    char* ID_instruccion;
    int parametros_validos;
} t_instruccion;

typedef struct {
    int pid;
    int pc;
    int quantum;
    int tabla_paginas;
    t_estado estado;
    RegistroCPU* registros;
    t_list* instrucciones;
} pcb;

pcb* crear_pcb(int pid, int quantum, RegistroCPU registros);
pcb* armar_pcb(int pid, int pc, int quantum, RegistroCPU registros, FILE* archivo, t_estado estado);
void eliminar_PCB(pcb* pcb_p);
void cambiar_estado(pcb* pcb_p, t_estado estado);
t_list* interpretarArchivo(FILE* archivo);
void liberarInstrucciones(t_list* instrucciones);
void element_destroyer(void* elemento);

#endif