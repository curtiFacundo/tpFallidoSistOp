#ifndef LARGO_PLAZO_H_
#define LARGO_PLAZO_H_

#include "kernel_main.h"
#include "pcb.h"
#include "utils.h"
#include <pthread.h>

void largo_plazo();
void eliminar_proceso(pcb* pcb_p)

void recibir_contexto(int pid);
void enviar_interrupcion(int pid);
#endif /* LARGO_PLAZO_H_ */