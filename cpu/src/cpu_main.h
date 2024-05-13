#ifndef CPU_MAIN_H_
#define CPU_MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>

#include "../../utils/src/utils.h"

void cliente_conexion_KERNEL(char *, char *);
void conexion_memoria(char*);
void levantar_conexiones();

#endif /* CPU_MAIN_H_ */