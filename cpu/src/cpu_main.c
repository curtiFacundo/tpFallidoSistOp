#include <stdlib.h>
#include <stdio.h>
#include "cpu_main.h"


int main(void) {
    
	/*
	TO DO list:
	
	*/

	logger = log_create("cpu.log", "cpu", 1, LOG_LEVEL_DEBUG); //movido al mail desde abrirServerCPU
	
	pthread_t tid_memoria;
	pthread_t tid_kernel;
	char *ret_value;
	

    //int socket_id = iniciar_servidor();
    config_global = config_create("../utils/config/config_global.config");

	//conexiones
	pthread_create(&tid_memoria, NULL, thread_crear_conexion_server, CPU_MEMORIA);
	pthread_create(&tid_kernel, NULL, thread_crear_conexion_cliente, KERNEL_CPU);
	//conexiones

	//espero fin conexiones
	pthread_join(tid_memoria, ret_value);
	log_info(ret_value);
	pthread_join(tid_kernel, ret_value);
	log_info(ret_value);
	//espero fin conexiones

	terminar_programa(server_fd_memoria, logger, config_global); //logger: redundante (global) pero esta definido asi en utils.h
    return 0;
}
void cliente_conexion_KERNEL(char * puerto, char * ip){
	t_paquete* send_handshake;
	int conexion;
	protocolo_socket op;
	int flag=1;
	char* valor_KERNEL;
	
	sem_wait(server_kernel_cpu);
	conexion = crear_conexion(ip, puerto);
	send_handshake = crear_paquete(HANDSHAKE);
	agregar_a_paquete (send_handshake, valor_KERNEL , strlen(valor_KERNEL)+1); // no seria CLAVE_CPU?

	while(flag){
		enviar_paquete(send_handshake, conexion);
		sleep(1);
		op = recibir_operacion(conexion);
		switch (op)
		{
		case HANDSHAKE:
			log_info("recibi handshake de KERNEL");
			break;
		
		case TERMINATE:
			flag = 0;
			break;

		default:
			break;
		}
	}

	eliminar_paquete(send_handshake);
	liberar_conexion(conexion);
}
void conexion_memoria(char* puerto) 
{
	int server = iniciar_servidor(puerto);
		log_info(logger, "Servidor listo para recibir al cliente Memoria");
		sem_post(server_cpu_memoria);
		int cliente = esperar_cliente(server);
		while(true){
			int cod_op = recibir_operacion(cliente);
			switch (cod_op)
			{
				case HANDSHAKE:
					handshake = recibir_paquete(cliente);
					log_info(logger, "me llego:\n");
					list_iterate(handshake, (void*) iterator); //no se como funciona esto 💁🏼
					break;
				case -1:
					log_error(logger, "el cliente se desconecto. Terminando servidor");
					return EXIT_FAILURE;
					break;
				default:
					log_warning(logger,"Operacion desconocida. No quieras meter la pata");
					break;
			}
		}
		
	close(server);
	close(cliente);
}

void Fetch(){
	esperarProximaInstruccion();

}

void Decode(){
	//ToDo interpretar qué instrucción es la que se va a ejecutar y si requiere de una traducción de dirección lógica a dirección física.

	// [número_pagina | desplazamiento] paginacion

}