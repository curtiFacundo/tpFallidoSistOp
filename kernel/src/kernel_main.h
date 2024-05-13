#ifndef KERNEL_MAIN_H_
#define KERNEL_MAIN_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

#include "../../utils/src/utils/utils.h"


t_log* iniciar_logger(void);
t_config* iniciar_config(void);
void paquete(int);
void EscucharServidor(char*,char*);
void conexion_cpu(char*);
void conexion_memoria(char*);
void cliente_conexion_IO(char *, char *);



#endif /* KERNEL_MAIN_H_ */