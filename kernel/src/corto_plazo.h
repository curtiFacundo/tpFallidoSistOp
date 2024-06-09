#ifndef CORTO_PLAZO_H_
#define CORTO_PLAZO_H_

#include "kernel_main.h"

//algoritmos corto plazo
typedef enum
{
    FIFO,
    RR,
    VRR
}algoritmo_planificacion;

void *corto_plazo(algoritmo_planificacion arg);

#endif /* CORTO_PLAZO_H_ */