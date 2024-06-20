#ifndef KERNEL_MAIN_H_
#define KERNEL_MAIN_H_


#include "../../utils/src/utils.h"
#include "pcb.h"
#include "corto_plazo.h"

extern int socket_cliente_cpu;
extern int cliente_memoria;

t_log* iniciar_logger(void);
t_config* iniciar_config(void);
void paquete(int);
void EscucharServidor(char*,char*);
void *conexion_cpu(char*);
void *conexion_memoria(char*);
void *cliente_conexion_IO(char **);


#endif /* KERNEL_MAIN_H_ */
