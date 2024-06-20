#ifndef CPU_MAIN_H_
#define CPU_MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include "../../kernel/src/pcb.h"
#include "../../utils/src/utils.h"
extern pcb* pcb_p;
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

void Fetch();
void Decode();
/*
void SET (RegistroCPU,int *); //Asigna al registro el valor pasado como parámetro.
void SUM (RegistroCPU,RegistroCPU); //Suma al Registro Destino el Registro Origen y deja el resultado en el Registro Destino.
void SUB (RegistroCPU,RegistroCPU); //Resta al Registro Destino el Registro Origen y deja el resultado en el Registro Destino.
void JNZ (RegistroCPU,int *); //Si el valor del registro es distinto de cero, actualiza el program counter al número de instrucción pasada por parámetro.
void IO_GEN_SLEEP (char *, int *); //Esta instrucción solicita al Kernel que se envíe a una interfaz de I/O a que realice un sleep por una cantidad de unidades de trabajo.
void CheckInterrupt();
*/


#endif /* CPU_MAIN_H_ */