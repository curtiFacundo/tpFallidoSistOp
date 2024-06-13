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
    IO_STDIN_WRITE,
}t_operaciones;

void *cliente_conexion_KERNEL(char **);
void *conexion_memoria(char*);
void levantar_conexiones();

void Fetch(pcb* pcb, t_operaciones* operacion);
void decode(t_operaciones* operacion);
void Execute(pcb* pcb, t_operaciones *operacion, RegistroCPU *registros);
void CheckInterrupt();



#endif /* CPU_MAIN_H_ */