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
void terminar_programa(int, t_log*, t_config*);
int crearConexionCPU(int conexion,char* ip, char* puerto,char* valor); //--logger. No es necesario ya que la variable es global
int serServidorIO(void);
void iterator(char* value);

#endif /* KERNEL_MAIN_H_ */