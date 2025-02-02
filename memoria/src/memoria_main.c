#include <stdlib.h>
#include <stdio.h>
#include "memoria_main.h"



int Saludar(void) {
    logger = log_create("memoria.log", "memoria", 1, LOG_LEVEL_DEBUG);
	log_info(logger,"Hola! CPU");
    return 0;
}

int main(void) {

	pthread_t tid_kernel;
	pthread_t tid_cpu;
	pthread_t tid_io;
	char *ret_value;
	char *arg_io[2]; // [PUERTO | IP]
	char *arg_cpu[2]; // [PUERTO | IP]
	char *arg_kernel[2]; // [PUERTO | IP]
	logger = log_create("memoria.log", "memoria", 1, LOG_LEVEL_DEBUG);
	config_global = config_create("../utils/config/config_global.config");

	arg_io[0] = config_get_string_value(config_global, "PUERTO_IO->MEMORIA");
	arg_io[1] = config_get_string_value(config_global, "IP_IO");

	arg_cpu[0] = config_get_string_value(config_global, "PUERTO_CPU->MEMORIA");
	arg_cpu[1] = config_get_string_value(config_global, "IP_CPU");

	arg_kernel[0] = config_get_string_value(config_global, "PUERTO_KERNEL->MEMORIA");
	arg_kernel[1] = config_get_string_value(config_global, "IP_KERNEL");
	
	//conexiones
	pthread_create(&tid_kernel, NULL, cliente_conexion_IO, arg_io);
	pthread_create(&tid_cpu, NULL, cliente_conexion_CPU, arg_cpu);
	pthread_create(&tid_io, NULL, cliente_conexion_KERNEL, arg_kernel);
	//conexiones

	//espero fin conexiones
	pthread_join(tid_kernel,ret_value);
	pthread_join(tid_cpu,ret_value);
	pthread_join(tid_io,ret_value);
	//espero fin conexiones

	return 0;
}
void *cliente_conexion_IO(char * arg_io[]){
	
	t_paquete* send_handshake_io;
	int conexion_IO_MEMORIA;
	protocolo_socket op;
	int flag=1;
	char *valor_IO;
	valor_IO = config_get_string_value(config_global, "CLAVE_MEMORIA");

	do
	{
		conexion_IO_MEMORIA = crear_conexion(arg_io[1], arg_io[0]);
		sleep(1);

	}while(conexion_IO_MEMORIA == -1);
	
	
	send_handshake_io = crear_paquete(HANDSHAKE);
	agregar_a_paquete (send_handshake_io,valor_IO, strlen(valor_IO)+1); 

	while(flag){
		enviar_paquete(send_handshake_io, conexion_IO_MEMORIA);
		sleep(1);
		op = recibir_operacion(conexion_IO_MEMORIA);
		switch (op)
		{
		case HANDSHAKE:
			log_info(logger, "recibi handshake de IO");
			break;
		case TERMINATE:
			flag = 0;
			break;

		default:
			break;
		}
	}

	eliminar_paquete(send_handshake_io);
	liberar_conexion(conexion_IO_MEMORIA);
}


void *cliente_conexion_CPU(char *arg_cpu[]) {
    t_paquete *send_handshake_cpu;
    int conexion_CPU_MEMORIA;
    protocolo_socket op;
    int flag = 1;
    char *valor_CPU;
    valor_CPU = config_get_string_value(config_global, "CLAVE_MEMORIA");
	
    do {
        conexion_CPU_MEMORIA = crear_conexion(arg_cpu[1], arg_cpu[0]);
        sleep(1);

    } while (conexion_CPU_MEMORIA == -1);

    send_handshake_cpu = crear_paquete(INSTRUCCIONES);
    agregar_a_paquete(send_handshake_cpu, valor_CPU, strlen(valor_CPU) + 1);
	char **instrucciones = interpretarArchivo();
    if (instrucciones == NULL) {
        log_error(logger, "Error al interpretar el archivo");
        eliminar_paquete(send_handshake_cpu);
        liberar_conexion(conexion_CPU_MEMORIA);
        return NULL;
    }

    // Enviar las instrucciones línea por línea
    for (int i = 0; instrucciones[i] != NULL; i++) {
        agregar_a_paquete(send_handshake_cpu, instrucciones[i], strlen(instrucciones[i]) + 1);
        enviar_paquete(send_handshake_cpu, conexion_CPU_MEMORIA);
        sleep(1);
    }

    // Liberar la memoria asignada a las instrucciones
    liberarInstrucciones(instrucciones);

    while (flag) {
        op = recibir_operacion(conexion_CPU_MEMORIA);
        switch (op) {
            case HANDSHAKE:
                log_info(logger, "recibi handshake de CPU");
                break;
            case INSTRUCCIONES:
                log_info(logger, "envio instrucciones a CPU");
                // Aquí podrías procesar algo después de enviar todas las instrucciones
                break;
            case TERMINATE:
                flag = 0;
                break;
            default:
                break;
        }
    }

    eliminar_paquete(send_handshake_cpu);
    liberar_conexion(conexion_CPU_MEMORIA);
}


void *cliente_conexion_KERNEL(char * arg_kernel[]){
	t_paquete* send_handshake_kernel;
	int conexion_CPU_MEMORIA;
	protocolo_socket op;
	int flag=1;
	char *valor_KERNEL;
	valor_KERNEL = config_get_string_value(config_global, "CLAVE_MEMORIA");

	do
	{
		conexion_CPU_MEMORIA = crear_conexion(arg_kernel[1], arg_kernel[0]);
		sleep(1);

	}while(conexion_CPU_MEMORIA == -1);
	
	
	send_handshake_kernel = crear_paquete(HANDSHAKE);
	agregar_a_paquete (send_handshake_kernel,valor_KERNEL, strlen(valor_KERNEL)+1); 

	while(flag){
		enviar_paquete(send_handshake_kernel, conexion_CPU_MEMORIA);
		sleep(1);
		op = recibir_operacion(conexion_CPU_MEMORIA);
		switch (op)
		{
		case HANDSHAKE:
			log_info(logger, "recibi handshake de KERNEL");
			break;
		
		case TERMINATE:
			flag = 0;
			break;

		default:
			break;
		}
	}

	eliminar_paquete(send_handshake_kernel);
	liberar_conexion(conexion_CPU_MEMORIA);
}

char **interpretarArchivo() {
    FILE *entrada = fopen("entrada.txt", "r");
    if (entrada == NULL) {
        perror("Error al abrir el archivo");
        return NULL;
    }

    char linea[100];
    char *token;
    char **instrucciones = malloc(100 * sizeof(char *));
    if (instrucciones == NULL) {
        perror("Error de asignación de memoria");
        fclose(entrada);
        return NULL;
    }

    int indice = 0;
    while (fgets(linea, sizeof(linea), entrada) != NULL) {
        const char *delimitador = "\n";
        token = strtok(linea, delimitador);
        if (token != NULL) {
            instrucciones[indice] = strdup(token); // Almacenar cada línea
            indice++;
        }
    }
    instrucciones[indice] = NULL; 
    fclose(entrada);
    return instrucciones;
}

void liberarInstrucciones(char **instrucciones) {
    if (instrucciones == NULL)
        return;

    for (int i = 0; instrucciones[i] != NULL; i++) {
        free(instrucciones[i]); // Liberar cada línea del array
    }
    free(instrucciones); // Liberar el array
}