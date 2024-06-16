#ifndef MEMORIA_MAIN_H_
#define MEMORIA_MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>

#include "../../utils/src/utils.h"

int Saludar(void);
void *cliente_conexion_IO(char **);
void *cliente_conexion_CPU(char **);
void *cliente_conexion_KERNEL(char **);
char **interpretarArchivo();

#endif /* MEMORIA_MAIN_H_ */