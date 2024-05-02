#include <stdlib.h>
#include <stdio.h>
#include "kernel_main.h"

t_config* config_global;
t_list *handshake;
t_paquete* send_handshake_io;

int main(int argc, char* argv[]) 
{    
	/*
	TO DO list
	* Mover la secuencia de arranque del server de kernel a su propia funcion
	* Conexion con CPU: hacer que Kernel sea Server
	* iniciar_config(): Mover a utils.c
	* serServidorIO(): generalizar? hasta que tengamos hilos, atajar las operaciones una por una
	*/
	int conexion_IO_KERNEL;
	char* ip_MEMORIA, *ip_IO;
	char* puerto, *puerto_IO_KERNEL;
	char* valor_MEMORIA, *valor_IO;
	char* mensaje;


    logger = log_create("kernel.log", "Kernel", 1, LOG_LEVEL_DEBUG);
    //int socket_id = iniciar_servidor();
    config_global = config_create("../utils/config/config_global.config");

	//Config para cliente Memoria. KERNEL->MEMORIA
	ip_MEMORIA = config_get_string_value(config_global, "IP_MEMORIA");
    puerto = config_get_string_value(config_global, "PUERTO_KERNEL->MEMORIA");
	valor_MEMORIA = config_get_string_value(config_global, "CLAVE_KERNEL"); //Decia CLAVE.
	EscucharServidor(puerto, "memoria");
	
	//Config para cliente CPU. KERNEL->CPU
	ip_MEMORIA = config_get_string_value(config_global, "IP_CPU");
    puerto = config_get_string_value(config_global, "PUERTO_KERNEL->CPU");
	EscucharServidor(puerto, "cpu");

	//terminar_programa(server_fd_memoria, logger, config_global); //logger: redundante (global) pero esta definido asi en utils.h
    
	//Config para conexion con EntradaSalida. IO->KERNEL
	ip_IO = config_get_string_value(config_global, "IP_IO");
    puerto_IO_KERNEL = config_get_string_value(config_global, "PUERTO_IO->KERNEL");
	valor_IO = config_get_string_value(config_global, "CLAVE_KERNEL"); //Decia CLAVE.

	conexion_IO_KERNEL = crear_conexion(ip_IO, puerto_IO_KERNEL, logger);
	send_handshake_io = crear_paquete(HANDSHAKE);

	agregar_a_paquete (send_handshake_io, valor_IO, strlen(valor_IO)+1);
	enviar_paquete(send_handshake_io, conexion_IO_KERNEL);
	eliminar_paquete(send_handshake_io);
	liberar_conexion(conexion_IO_KERNEL);
	
	return 0;
}

void EscucharServidor(char* puerto, char* CLIENTE) 
{
	int server = iniciar_servidor(puerto);
		log_info(logger, "Servidor listo para recibir al cliente");
		log_info(logger, CLIENTE);
		int cliente = esperar_cliente(server);
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
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	close(server);
}
void conexion_cpu(char* puerto, char* CLIENTE) 
{
	int server = iniciar_servidor(puerto);
		log_info(logger, "Servidor listo para recibir al cliente");
		log_info(logger, CLIENTE);
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