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

struct arg_struct_sch {
    algoritmo_planificacion algoritmo;
}typedef argumentos_thread_scheduler;

void *corto_plazo(void*);

#endif /* CORTO_PLAZO_H_ */