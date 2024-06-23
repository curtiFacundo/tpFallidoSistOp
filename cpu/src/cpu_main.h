#ifndef CPU_MAIN_H_
#define CPU_MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include "../../kernel/src/pcb.h"
#include "../../utils/src/utils.h"

typedef enum  
{
    SET,
    SUB,
    SUM,
    JNZ,
    IO_GEN_SLEEP,
    MOV_IN,
    MOV_OUT,
    RESIZE,
    COPY_STRING,
    IO_STDIN_READ,
    IO_STDOUT_WRITE,
    IO_FS_CREATE,
    IO_FS_DELETE,
    IO_FS_TRUNCATE,
    IO_FS_WRITE,
    IO_FS_READ,
    WAIT,
    SIGNAL,
    SALIR,
    INVALID_OPERATION 
}t_operaciones;

typedef struct{
    RegistroCPU parametros[2];
    char ID_instruccion;
}t_instruccion;

// Declaraciones de funciones

void *cliente_conexion_KERNEL(char *arg_kernel[]);
void *conexion_memoria(char *puerto);
void separarInstrucciones(t_paquete *handshake_recv);
void Fetch(pcb* pcb);
void decode();
void Execute(pcb* pcb, RegistroCPU *registros);



#endif /* CPU_MAIN_H_ */