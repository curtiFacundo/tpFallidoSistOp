#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "kernel_main.h"

t_config* config_global;


int main(int argc, char* argv[]) 
{    
	/*
	TO DO list
	* Mover la secuencia de arranque del server de kernel a su propia funcion
	* Conexion con CPU: hacer que Kernel sea Server
	* iniciar_config(): Mover a utils.c
	* serServidorIO(): generalizar? hasta que tengamos hilos, atajar las operaciones una por una
	*/

	pthread_t tid_memoria;
	pthread_t tid_cpu;
	pthread_t tid_io;
	char *ret_value;

	logger = log_create("kernel.log", "Kernel", 1, LOG_LEVEL_DEBUG);
    config_global = config_create("../utils/config/config_global.config");
   	
	//conexiones
	pthread_create(&tid_memoria, NULL, thread_crear_conexion_server, KERNEL_MEMORIA);
	pthread_create(&tid_cpu, NULL, thread_crear_conexion_server, KERNEL_CPU);
	pthread_create(&tid_io, NULL, thread_crear_conexion_cliente, IO_KERNEL);
	//conexiones

	//espero fin conexiones
	pthread_join(tid_memoria, &ret_value);
	log_info(logger,ret_value);
	pthread_join(tid_cpu, &ret_value);
	log_info(logger,ret_value);
	pthread_join(tid_io, &ret_value);
	log_info(logger, ret_value);
	//espero fin conexiones
	
	return 0;
}

void conexion_cpu(char* puerto)
{
	t_list *handshake;
	
	int server = iniciar_servidor(puerto);
		log_info(logger, "Servidor listo para recibir al cliente CPU");
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
void conexion_memoria(char* puerto) 
{
	t_list *handshake;
	int server = iniciar_servidor(puerto);
		log_info(logger, "Servidor listo para recibir al cliente MEMORIA");

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
						//log_error(logger, "el cliente se desconecto. Terminando servidor");
						//return EXIT_FAILURE;
						break;
				default:
					log_warning(logger,"Operacion desconocida. No quieras meter la pata");
					break;
			}
		}
		
	close(server);
	close(cliente);
}
void cliente_conexion_IO(char * puerto, char * ip){

	t_paquete* send_handshake_io;
	int conexion_IO_KERNEL;
	protocolo_socket op;
	int flag=1;
	char *valor_IO;
	valor_IO = config_get_string_value(config_global, "CLAVE_KERNEL");

	//sem_wait(server_kernel_cpu);
	conexion_IO_KERNEL = crear_conexion(ip, puerto);
	send_handshake_io = crear_paquete(HANDSHAKE);
	agregar_a_paquete (send_handshake_io,valor_IO, strlen(valor_IO)+1); 

	while(flag){
		enviar_paquete(send_handshake_io, conexion_IO_KERNEL);
		sleep(1);
		op = recibir_operacion(conexion_IO_KERNEL);
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
	liberar_conexion(conexion_IO_KERNEL);
}