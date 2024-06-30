#ifndef KERNEL_MAIN_H_
#define KERNEL_MAIN_H_


#include "../../utils/src/utils.h"
#include "pcb.h"

t_log* iniciar_logger(void);
t_config* iniciar_config(void);
void paquete(int);
void EscucharServidor(char*,char*);
void *conexion_cpu(void*);
void *conexion_memoria(void*);
void *cliente_conexion_IO(void *);

#endif /* KERNEL_MAIN_H_ */