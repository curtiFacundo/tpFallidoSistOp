#ifndef MEMORIA_MAIN_H_
#define MEMORIA_MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>

#include "../../utils/src/utils.h"

int Saludar(void);
void *cliente_conexion_IO(void *);
void *cliente_conexion_CPU(void *);
void *cliente_conexion_KERNEL(void *);

#endif /* MEMORIA_MAIN_H_ */