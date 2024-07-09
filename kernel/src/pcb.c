#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pcb.h"

pcb* crear_pcb(int pid, int quantum, RegistroCPU registros)
{
    pcb* pcb_p = (pcb*)(malloc(sizeof(pcb)));
    pcb_p->pid = pid;
    pcb_p->pc = 0;
    pcb_p->quantum = 0;
    pcb_p->estado = NEW;
    pcb_p->registros = malloc(sizeof(RegistroCPU));
    pcb_p->instrucciones = malloc(sizeof(t_list));
    return pcb_p;
}

pcb* armar_pcb(int pid, int pc, int quantum, RegistroCPU registros, FILE* archivo, t_estado estado)
{
    t_list* instrucciones = interpretarArchivo(archivo);
    pcb* pcb_p = crear_pcb(pid, quantum, registros);
    pcb_p->pid = pid;
    pcb_p->pc = pc;
    pcb_p->quantum = quantum;
    pcb_p->instrucciones = instrucciones;
   
    *(pcb_p->registros) = registros;
    pcb_p->estado = estado;

    return pcb_p;
}

void element_destroyer(void* elemento) {
    t_instruccion* instruccion = (t_instruccion*)elemento;
    free(instruccion->ID_instruccion);
    free(instruccion);
}

void eliminar_PCB(pcb* pcb_p) {
    if (pcb_p->instrucciones != NULL) {
        liberarInstrucciones(pcb_p->instrucciones);
    }
    free(pcb_p->registros);
    free(pcb_p);
}

void cambiar_estado(pcb* pcb_p, t_estado estado)
{
    pcb_p->estado = estado;
}

// interpretar un archivo y crear una lista de instrucciones
t_list* interpretarArchivo(FILE* archivo) {
    FILE *entrada = archivo;
    if (entrada == NULL) {
        perror("Error al abrir el archivo");
        return NULL;
    }

    char linea[100];
    t_list* instrucciones = list_create();
    if (instrucciones == NULL) {
        perror("Error de asignación de memoria");
        fclose(entrada);
        return NULL;
    }

    while (fgets(linea, sizeof(linea), entrada) != NULL) {
        linea[strcspn(linea, "\n")] = 0; 

        t_instruccion* instruccion = malloc(sizeof(t_instruccion));
        if (instruccion == NULL) {
            perror("Error de asignación de memoria para la instrucción");
            list_destroy_and_destroy_elements(instrucciones, element_destroyer);
            fclose(entrada);
            return NULL;
        }

        char* token = strtok(linea, " ");
        if (token != NULL) {
            instruccion->ID_instruccion = strdup(token);

            // Si la instrucción es "SALIR", no asignar parámetros
            if (strcmp(instruccion->ID_instruccion, "SALIR") == 0) {
                instruccion->parametros_validos = 0;
                instruccion->parametros[0] = 0;
                instruccion->parametros[1] = 0;
            } else {
                instruccion->parametros_validos = 1;
                for (int i = 0; i < 2; i++) {
                    token = strtok(NULL, " ");
                    if (token != NULL) {
                        instruccion->parametros[i] = atoi(token);
                    } else {
                        instruccion->parametros[i] = 0;
                    }
                }
            }

            list_add(instrucciones, instruccion);
        } else {
            free(instruccion);
        }
    }

    fclose(entrada);
    return instrucciones;
}

void liberarInstrucciones(t_list* instrucciones) {
    list_destroy_and_destroy_elements(instrucciones, element_destroyer);
}
